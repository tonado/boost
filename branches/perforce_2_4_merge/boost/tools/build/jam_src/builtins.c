/*
 * Copyright 1993-2002 Christopher Seiwald and Perforce Software, Inc.
 *
 * This file is part of Jam - see jam.c for Copyright information.
 */

# include "jam.h"

# include "lists.h"
# include "parse.h"
# include "builtins.h"
# include "rules.h"
# include "filesys.h"
# include "newstr.h"
# include "frames.h"
# include "hash.h"
# include "strings.h"
# include "regexp.h"

/*
 * builtins.c - builtin jam rules
 *
 * External routines:
 *
 * 	load_builtin() - define builtin rules
 *
 * Internal routines:
 *
 *	builtin_depends() - DEPENDS/INCLUDES rule
 *	builtin_echo() - ECHO rule
 *	builtin_exit() - EXIT rule
 *	builtin_flags() - NOCARE, NOTFILE, TEMPORARY rule
 *	builtin_glob() - GLOB rule
 *	builtin_match() - MATCH rule
 *
 * 01/10/01 (seiwald) - split from compile.c
 */

/*
 * compile_builtin() - define builtin rules
 */

# define P0 (PARSE *)0
# define C0 (char *)0

int glob( char *s, char *c );

static void lol_build( LOL* lol, char** elements );
void backtrace( FRAME *frame );
void backtrace_line( FRAME *frame );
void print_source_line( PARSE* p );

RULE* bind_builtin( char* name, LIST*(*f)(PARSE*, FRAME*), int flags, char** args )
{
    argument_list* arg_list = 0;
    
    if ( args )
    {
        arg_list = args_new();
        lol_build( arg_list->data, args );
    }

    return new_rule_body( root_module(), name, arg_list,
                          parse_make( f, P0, P0, P0, C0, C0, flags ), 1 );
}

RULE* duplicate_rule( char* name, RULE* other )
{
    return import_rule( other, root_module(), name );
}

void
load_builtins()
{
    duplicate_rule( "Always" ,
      bind_builtin( "ALWAYS" ,
                    builtin_flags, T_FLAG_TOUCHED, 0 ) );

    duplicate_rule( "Depends" ,
      bind_builtin( "DEPENDS" ,
                    builtin_depends, T_DEPS_DEPENDS, 0 ) );

    duplicate_rule( "echo" ,
    duplicate_rule( "Echo" ,
      bind_builtin( "ECHO" ,
                    builtin_echo, 0, 0 ) ) );

    duplicate_rule( "exit" ,
    duplicate_rule( "Exit" ,
      bind_builtin( "EXIT" ,
                    builtin_exit, 0, 0 ) ) );

    duplicate_rule( "Glob" ,
      bind_builtin( "GLOB" ,
                    builtin_glob, 0, 0 ) );

    duplicate_rule( "Includes" ,
      bind_builtin( "INCLUDES" ,
                    builtin_depends, T_DEPS_INCLUDES, 0 ) );

    duplicate_rule( "Leaves" ,
      bind_builtin( "LEAVES" ,
                    builtin_flags, T_FLAG_LEAVES, 0 ) );

    duplicate_rule( "NoCare" ,
      bind_builtin( "NOCARE" ,
                    builtin_flags, T_FLAG_NOCARE, 0 ) );

    duplicate_rule( "NOTIME" ,
    duplicate_rule( "NotFile" ,
      bind_builtin( "NOTFILE" ,
                    builtin_flags, T_FLAG_NOTFILE, 0 ) ) );

    duplicate_rule( "NoUpdate" ,
      bind_builtin( "NOUPDATE" ,
                    builtin_flags, T_FLAG_NOUPDATE, 0 ) );

    duplicate_rule( "Temporary" ,
      bind_builtin( "TEMPORARY" ,
                    builtin_flags, T_FLAG_TEMP, 0 ) );

    duplicate_rule( "HdrMacro" ,
      bind_builtin( "HDRMACRO" ,
                    builtin_hdrmacro, 0, 0 ) );

    duplicate_rule( "Match" ,
      bind_builtin( "MATCH" ,
	            builtin_match, 0, 0 ) );
    
    /* FAIL_EXPECTED is an experimental built-in that is used to indicate        */
    /* that the result of a target build action should be inverted (ok <=> fail) */
    /* this can be useful when performing test runs from Jamfiles..              */
    /*                                                                           */
    /* Beware that this rule might disappear or be renamed in the future..       */
    /* contact david.turner@freetype.org for more details..                      */
      bind_builtin( "FAIL_EXPECTED" ,
                    builtin_flags, T_FLAG_FAIL_EXPECTED, 0 );

      {
          char * args[] = { "string", "pattern", "replacements", "+", 0 };
          duplicate_rule( "subst" ,
            bind_builtin( "SUBST" ,
                          builtin_subst, 0, args ) );
      }

      {
          char * args[] = { "module", "?", 0 };
          bind_builtin( "RULENAMES" ,
                         builtin_rulenames, 0, args );
      }

      {
           char * args[] = { "source_module", "?",
                             ":", "source_rules", "*",
                             ":", "target_module", "?",
                             ":", "target_rules", "*",
                             ":", "localize", "?", 0 };
           bind_builtin( "IMPORT" ,
                         builtin_import, 0, args );
      }

      {
          char * args[] = { "module", "?", ":", "rules", "*", 0 };
          bind_builtin( "EXPORT" ,
                        builtin_export, 0, args );
      }

      {
          char * args[] = { "levels", "?", 0 };
          bind_builtin( "CALLER_MODULE" ,
                         builtin_caller_module, 0, args );
      }

      {
          char * args[] = { 0 };
          bind_builtin( "BACKTRACE" ,
                        builtin_backtrace, 0, args );
      }
}

/*
 * builtin_depends() - DEPENDS/INCLUDES rule
 *
 * The DEPENDS builtin rule appends each of the listed sources on the 
 * dependency list of each of the listed targets.  It binds both the 
 * targets and sources as TARGETs.
 */

LIST *
builtin_depends(
	PARSE	*parse,
	FRAME *frame )
{
	LIST *targets = lol_get( frame->args, 0 );
	LIST *sources = lol_get( frame->args, 1 );
	int which = parse->num;
	LIST *l;

	for( l = targets; l; l = list_next( l ) )
	{
	    TARGET *t = bindtarget( l->string );
	    t->deps[ which ] = targetlist( t->deps[ which ], sources );
	}

	return L0;
}

/*
 * builtin_echo() - ECHO rule
 *
 * The ECHO builtin rule echoes the targets to the user.  No other 
 * actions are taken.
 */

LIST *
builtin_echo(
	PARSE	*parse,
	FRAME *frame )
{
	list_print( lol_get( frame->args, 0 ) );
	printf( "\n" );
	return L0;
}

/*
 * builtin_exit() - EXIT rule
 *
 * The EXIT builtin rule echoes the targets to the user and exits
 * the program with a failure status.
 */

LIST *
builtin_exit(
	PARSE	*parse,
	FRAME *frame )
{
	list_print( lol_get( frame->args, 0 ) );
	printf( "\n" );
	exit( EXITBAD ); /* yeech */
	return L0;
}

/*
 * builtin_flags() - NOCARE, NOTFILE, TEMPORARY rule
 *
 * Builtin_flags() marks the target with the appropriate flag, for use
 * by make0().  It binds each target as a TARGET.
 */

LIST *
builtin_flags(
	PARSE	*parse,
	FRAME *frame )
{
	LIST *l = lol_get( frame->args, 0 );

	for( ; l; l = list_next( l ) )
	    bindtarget( l->string )->flags |= parse->num;

	return L0;
}

/*
 * builtin_globbing() - GLOB rule
 */

struct globbing {
	LIST	*patterns;
	LIST	*results;
} ;

static void
builtin_glob_back(
	void	*closure,
	char	*file,
	int	status,
	time_t	time )
{
	struct globbing *globbing = (struct globbing *)closure;
	LIST		*l;

	for( l = globbing->patterns; l; l = l->next )
	    if( !glob( l->string, file ) )
	{
	    globbing->results = list_new( globbing->results, newstr( file ) );
	    break;
	}
}

LIST *
builtin_glob(
	PARSE	*parse,
	FRAME *frame )
{
	LIST *l = lol_get( frame->args, 0 );
	LIST *r = lol_get( frame->args, 1 );

	struct globbing globbing;

	globbing.results = L0;
	globbing.patterns = r;

	for( ; l; l = list_next( l ) )
	    file_dirscan( l->string, builtin_glob_back, &globbing );

	return globbing.results;
}

LIST *
builtin_hdrmacro(
    PARSE    *parse,
    FRAME *frame )
{
  LIST*  l = lol_get( frame->args, 0 );
  
  for ( ; l; l = list_next(l) )
  {
    TARGET*  t = bindtarget( l->string );

    /* scan file for header filename macro definitions */    
    if ( DEBUG_HEADER )
      printf( "scanning '%s' for header file macro definitions\n",
              l->string );

    macro_headers( t );
  }
  
  return L0;
}

/*  builtin_rulenames() - RULENAMES ( MODULE ? )
 *
 *  Returns a list of the non-local rule names in the given MODULE. If
 *  MODULE is not supplied, returns the list of rule names in the
 *  global module.
 */

/* helper function for builtin_rulenames(), below */
static void add_rule_name( void* r_, void* result_ )
{
    RULE* r = r_;
    LIST** result = result_;

    if ( r->exported )
        *result = list_new( *result, copystr( r->name ) );
}

LIST *
builtin_rulenames(
    PARSE   *parse,
    FRAME *frame )
{
    LIST *arg0 = lol_get( frame->args, 0 );
    LIST *result = L0;
    module* source_module = bindmodule( arg0 ? arg0->string : 0 );

    hashenumerate( source_module->rules, add_rule_name, &result );
    return result;
}

static void unknown_rule( FRAME *frame, char* key, char *module_name, char *rule_name )
{
    backtrace_line( frame->prev );
    printf( "%s error: rule \"%s\" unknown in module \"%s\"\n", key, rule_name, module_name );
    backtrace( frame->prev );
    exit(1);
    
}

/*
 * builtin_import() - IMPORT ( SOURCE_MODULE ? : SOURCE_RULES * : TARGET_MODULE ? : TARGET_RULES * : LOCALIZE ? )
 *
 * The IMPORT rule imports rules from the SOURCE_MODULE into the
 * TARGET_MODULE as local rules. If either SOURCE_MODULE or
 * TARGET_MODULE is not supplied, it refers to the global
 * module. SOURCE_RULES specifies which rules from the SOURCE_MODULE
 * to import; TARGET_RULES specifies the names to give those rules in
 * TARGET_MODULE. If SOURCE_RULES contains a name which doesn't
 * correspond to a rule in SOURCE_MODULE, or if it contains a
 * different number of items than TARGET_RULES, an error is issued.
 * if LOCALIZE is specified, the rules will be executed in
 * TARGET_MODULE, with corresponding access to its module local
 * variables.
 */
LIST *
builtin_import(
    PARSE *parse,
    FRAME *frame )
{
    LIST *source_module_list = lol_get( frame->args, 0 );
    LIST *source_rules = lol_get( frame->args, 1 );
    LIST *target_module_list = lol_get( frame->args, 2 );
    LIST *target_rules = lol_get( frame->args, 3 );
    LIST *localize = lol_get( frame->args, 4 );

    module* target_module = bindmodule( target_module_list ? target_module_list->string : 0 );
    module* source_module = bindmodule( source_module_list ? source_module_list->string : 0 );
    
    LIST *source_name, *target_name;
            
    for ( source_name = source_rules, target_name = target_rules;
          source_name && target_name;
          source_name = list_next( source_name )
          , target_name = list_next( target_name ) )
    {
        RULE r_, *r = &r_, *imported;
        r_.name = source_name->string;
                
        if ( !hashcheck( source_module->rules, (HASHDATA**)&r ) )
            unknown_rule( frame, "IMPORT", source_module->name, r_.name );
        
        imported = import_rule( r, target_module, target_name->string );
        if ( localize )
            imported->module = target_module;
        imported->exported = 0; /* this rule is really part of some other module; just refer to it here, but don't let it out */
    }
    
    if ( source_name || target_name )
    {
        backtrace_line( frame->prev );
        printf( "import error: length of source and target rule name lists don't match" );
        backtrace( frame->prev );
        exit(1);
    }

    return L0;
}


/*
 * builtin_export() - EXPORT ( MODULE ? : RULES * )
 *
 * The EXPORT rule marks RULES from the SOURCE_MODULE as non-local
 * (and thus exportable). If an element of RULES does not name a rule
 * in MODULE, an error is issued.
 */
LIST *
builtin_export(
    PARSE *parse,
    FRAME *frame )
{
    LIST *module_list = lol_get( frame->args, 0 );
    LIST *rules = lol_get( frame->args, 1 );

    module* m = bindmodule( module_list ? module_list->string : 0 );
    
            
    for ( ; rules; rules = list_next( rules ) )
    {
        RULE r_, *r = &r_;
        r_.name = rules->string;
                
        if ( !hashcheck( m->rules, (HASHDATA**)&r ) )
            unknown_rule( frame, "EXPORT", m->name, r_.name );
        
        r->exported = 1;
    }
    return L0;
}

/*  Retrieve the file and line number that should be indicated for a
 *  given procedure in debug output or an error backtrace
 */
static void get_source_line( PARSE* procedure, char** file, int* line )
{
    if ( procedure )
    {
        char* f = procedure->file;
        int l = procedure->line;
        if ( !strcmp( f, "+" ) )
        {
            f = "jambase.c";
            l += 3;
        }
        *file = f;
        *line = l;
    }
    else
    {
        *file = "(builtin)";
        *line = -1;
    }
}

void print_source_line( PARSE* p )
{
    char* file;
    int line;

    get_source_line( p, &file, &line );
    if ( line < 0 )
        printf( "(builtin):" );
    else
        printf( "%s:%d:", file, line);
}

/* Print a single line of error backtrace for the given frame */
void backtrace_line( FRAME *frame )
{
    print_source_line( frame->procedure );
    printf( " in %s\n", frame->rulename );
}

/*  Print the entire backtrace from the given frame to the Jambase
 *  which invoked it.
 */
void backtrace( FRAME *frame )
{
    while ( frame = frame->prev )
    {
        backtrace_line( frame );
    }
}

/*  A Jam version of the backtrace function, taking no arguments and
 *  returning a list of quadruples: FILENAME LINE MODULE. RULENAME
 *  describing each frame. Note that the module-name is always
 *  followed by a period.
 */
LIST *builtin_backtrace( PARSE *parse, FRAME *frame )
{
    LIST* result = L0;
    while ( frame = frame->prev )
    {
        char* file;
        int line;
        char buf[32];
        get_source_line( frame->procedure, &file, &line );
        sprintf( buf, "%d", line );
        result = list_new( result, newstr( file ) );
        result = list_new( result, newstr( buf ) );
        result = list_new( result, newstr( frame->module->name ) );
        result = list_new( result, newstr( frame->rulename ) );
    }
    return result;
}

/*
 * builtin_caller_module() - CALLER_MODULE ( levels ? )
 *
 * If levels is not supplied, returns the name of the module of the rule which
 * called the one calling this one. If levels is supplied, it is interpreted as
 * an integer specifying a number of additional levels of call stack to traverse
 * in order to locate the module in question. If no such module exists,
 * returns the empty list. Also returns the empty list when the module in
 * question is the global module. This rule is needed for implementing module
 * import behavior.
 */
LIST *builtin_caller_module( PARSE *parse, FRAME *frame )
{
    LIST* levels_arg = lol_get( frame->args, 0 );
    int levels = levels_arg ? atoi( levels_arg->string ) : 0 ;

    int i;
    for (i = 0; i < levels + 2 && frame->prev; ++i)
        frame = frame->prev;

    if ( frame->module == root_module() )
    {
        return L0;
    }
    else
    {
        LIST* result;
        
        string name;
        string_copy( &name, frame->module->name );
        string_pop_back( &name );

        result = list_new( L0, newstr(name.value) );
        
        string_free( &name );
        
        return result;
    }
}

static void lol_build( LOL* lol, char** elements )
{
    LIST* l = L0;
    lol_init( lol );
    
    while ( elements && *elements )
    {
        if ( !strcmp( *elements, ":" ) )
        {
            lol_add( lol, l );
            l = L0 ;
        }
        else
        {
            l = list_new( l, newstr( *elements ) );
        }
        ++elements;
    }
    
    if ( l != L0 )
        lol_add( lol, l );
}

/*
 * builtin_match() - MATCH rule, regexp matching
 */

LIST *
builtin_match(
	PARSE	*parse,
	FRAME	*frame )
{
	LOL *args = frame->args;
	LIST *l = lol_get( args, 0 );
	LIST *r = lol_get( args, 1 );
	LIST *result = 0;
	regexp *re;

	/* No pattern or string?  No results. */

	if( !l || !r )
	    return L0;

	/* Just use first arg of each list. */

	re = regcomp( l->string );

	if( regexec( re, r->string ) )
	{
	    int i, top;

	    /* Find highest parameter */

	    for( top = NSUBEXP; top-- > 1; )
		if( re->startp[top] != re->endp[top] )
		    break;

	    /* And add all parameters up to highest onto list. */
	    /* Must have parameters to have results! */

	    for( i = 1; i <= top; i++ )
	    {
		char buf[ MAXSYM ];
		int l = re->endp[i] - re->startp[i];
		memcpy( buf, re->startp[i], l );
		buf[ l ] = 0;
		result = list_new( result, newstr( buf ) );
	    }
	}

	free( (char *)re );

	return result;
}
