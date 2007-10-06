/* Generated by mkjambase from Jambase */
char *jambase[] = {
/* Jambase */
"if $(NT)\n",
"{\n",
"CP		?= copy ;\n",
"RM		?= del /f/q ;\n",
"SLASH		?= \\\\ ;\n",
"SUFLIB		?= .lib ;\n",
"SUFOBJ		?= .obj ;\n",
"SUFEXE		?= .exe ;\n",
"if $(BCCROOT)\n",
"{\n",
"Echo \"Compiler is Borland C++\" ;\n",
"AR		?= tlib /C /P64 ;\n",
"CC		?= bcc32 ;\n",
"CCFLAGS		?= -v -w- -q -DWIN -tWR -tWM -tWC ;\n",
"C++		?= $(CC) ;\n",
"C++FLAGS	?= $(CCFLAGS) -P ;\n",
"LINK		?= $(CC) ;\n",
"LINKFLAGS	?= $(CCFLAGS) ;\n",
"STDLIBPATH	?= $(BCCROOT)\\\\lib ;\n",
"STDHDRS		?= $(BCCROOT)\\\\include ;\n",
"NOARSCAN	?= true ;\n",
"}\n",
"else if $(MSVC)\n",
"{\n",
"Echo \"Compiler is Microsoft Visual C++ 16 bit\" ;\n",
"AR		?= lib /nologo ;\n",
"CC		?= cl /nologo ;\n",
"CCFLAGS		?= /D \\\"WIN\\\" ;\n",
"C++		?= $(CC) ;\n",
"C++FLAGS	?= $(CCFLAGS) ;\n",
"LINK		?= $(CC) ;\n",
"LINKFLAGS	?= $(CCFLAGS) ;\n",
"LINKLIBS	?= \n",
"$(MSVC)\\\\lib\\\\mlibce.lib\n",
"$(MSVC)\\\\lib\\\\oldnames.lib\n",
";\n",
"LINKLIBS	?= ;\n",
"NOARSCAN	?= true ;\n",
"OPTIM		?=  ;\n",
"STDHDRS		?= $(MSVC)\\\\include ;\n",
"UNDEFFLAG	?= \"/u _\" ;\n",
"}\n",
"else if $(MSVCNT)\n",
"{\n",
"Echo \"Compiler is Microsoft Visual C++\" ;\n",
"AR		?= lib ;\n",
"AS		?= masm386 ;\n",
"CC		?= cl /nologo ;\n",
"CCFLAGS		?= \"\" ;\n",
"C++		?= $(CC) ;\n",
"C++FLAGS	?= $(CCFLAGS) ;\n",
"LINK		?= link /nologo ;\n",
"LINKFLAGS	?= \"\" ;\n",
"LINKLIBS	?= $(MSVCNT)\\\\lib\\\\advapi32.lib\n",
"$(MSVCNT)\\\\lib\\\\libc.lib\n",
"$(MSVCNT)\\\\lib\\\\oldnames.lib\n",
"$(MSVCNT)\\\\lib\\\\kernel32.lib ;\n",
"OPTIM		?= \"\" ;\n",
"STDHDRS		?= $(MSVCNT)\\\\include ;\n",
"UNDEFFLAG	?= \"/u _\" ;\n",
"}\n",
"else\n",
"{\n",
"Exit On NT, set BCCROOT, MSVCNT, or MSVC to the root of the\n",
"Borland or Microsoft directories. ;\n",
"}\n",
"}\n",
"else if $(OS2)\n",
"{\n",
"WATCOM		?= $(watcom) ;\n",
"if ! $(WATCOM)\n",
"{\n",
"Exit On OS2, set WATCOM to the root of the Watcom directory. ;\n",
"}\n",
"AR		?= wlib ;\n",
"BINDIR		?= \\\\os2\\\\apps ;\n",
"CC		?= wcc386 ;\n",
"CCFLAGS		?= /zq /DOS2 /I$(WATCOM)\\\\h ; # zq=quiet\n",
"C++		?= wpp386 ;\n",
"C++FLAGS	?= $(CCFLAGS) ;\n",
"CP		?= copy ;\n",
"DOT		?= . ;\n",
"DOTDOT		?= .. ;\n",
"LINK		?= wcl386 ;\n",
"LINKFLAGS	?= /zq ; # zq=quiet\n",
"LINKLIBS	?= ;\n",
"MV		?= move ;\n",
"NOARSCAN	?= true ;\n",
"OPTIM		?= ;\n",
"RM		?= del /f ;\n",
"SLASH		?= \\\\ ;\n",
"STDHDRS		?= $(WATCOM)\\\\h ;\n",
"SUFEXE		?= .exe ;\n",
"SUFLIB		?= .lib ;\n",
"SUFOBJ		?= .obj ;\n",
"UNDEFFLAG	?= \"/u _\" ;\n",
"}\n",
"else if $(VMS)\n",
"{\n",
"C++		?= cxx ;\n",
"C++FLAGS	?= ;\n",
"CC		?= cc ;\n",
"CCFLAGS		?= ;\n",
"CHMOD		?= set file/prot= ;\n",
"CP		?= copy/replace ;\n",
"CRELIB		?= true ;\n",
"DOT		?= [] ;\n",
"DOTDOT		?= [-] ;\n",
"EXEMODE		?= (w:e) ;\n",
"FILEMODE	?= (w:r) ;\n",
"HDRS		?= ;\n",
"LINK		?= link ;\n",
"LINKFLAGS	?= \"\" ;\n",
"LINKLIBS	?= ;\n",
"MKDIR		?= create/dir ;\n",
"MV		?= rename ;\n",
"OPTIM		?= \"\" ;\n",
"RM		?= delete ;\n",
"RUNVMS		?= mcr ;\n",
"SHELLMODE	?= (w:er) ;\n",
"SLASH		?= . ;\n",
"STDHDRS		?= decc$library_include ;\n",
"SUFEXE		?= .exe ;\n",
"SUFLIB		?= .olb ;\n",
"SUFOBJ		?= .obj ;\n",
"switch $(OS) \n",
"{\n",
"case OPENVMS : CCFLAGS ?= /stand=vaxc ;\n",
"case VMS     : LINKLIBS ?= sys$library:vaxcrtl.olb/lib ;\n",
"}\n",
"}\n",
"else if $(MAC)\n",
"{\n",
"local OPT ;\n",
"CW	?= \"{CW}\" ;\n",
"MACHDRS ?=\n",
"\"$(UMACHDRS):Universal:Interfaces:CIncludes\"\n",
"\"$(CW):MSL:MSL_C:MSL_Common:Include\"\n",
"\"$(CW):MSL:MSL_C:MSL_MacOS:Include\" ;\n",
"MACLIBS ?=\n",
"\"$(CW):MacOS Support:Universal:Libraries:StubLibraries:Interfacelib\"\n",
"\"$(CW):MacOS Support:Universal:Libraries:StubLibraries:Mathlib\" ;\n",
"MPWLIBS ?= \n",
"\"$(CW):MacOS Support:Libraries:Runtime:Runtime PPC:MSL MPWCRuntime.lib\"\n",
"\"$(CW):MSL:MSL_C:MSL_MacOS:Lib:PPC:MSL C.PPC MPW.Lib\" ;\n",
"MPWNLLIBS ?= \n",
"\"$(CW):MacOS Support:Libraries:Runtime:Runtime PPC:MSL MPWCRuntime.lib\"\n",
"\"$(CW):MSL:MSL_C:MSL_MacOS:Lib:PPC:MSL C.PPC MPW(NL).Lib\" ;\n",
"SIOUXHDRS ?= ;\n",
"SIOUXLIBS ?= \n",
"\"$(CW):MacOS Support:Libraries:Runtime:Runtime PPC:MSL RuntimePPC.lib\"\n",
"\"$(CW):MSL:MSL_C:MSL_MacOS:Lib:PPC:MSL SIOUX.PPC.Lib\" \n",
"\"$(CW):MSL:MSL_C:MSL_MacOS:Lib:PPC:MSL C.PPC.Lib\" ;\n",
"C++		?= mwcppc ;\n",
"C++FLAGS	?= -w off -nomapcr ;\n",
"CC		?= mwcppc ;\n",
"CCFLAGS		?= -w off -nomapcr ;\n",
"CP		?= duplicate -y ;\n",
"DOT		?= \":\" ;\n",
"DOTDOT		?= \"::\" ;\n",
"HDRS 		?= $(MACHDRS) $(MPWHDRS) ;\n",
"LINK		?= mwlinkppc ;\n",
"LINKFLAGS	?= -mpwtool -warn ;				\n",
"LINKLIBS 	?= $(MACLIBS) $(MPWLIBS) ;				\n",
"MKDIR		?= newfolder ;\n",
"MV		?= rename -y ;\n",
"NOARSCAN	?= true ;\n",
"OPTIM		?= ;\n",
"RM		?= delete -y ;\n",
"SLASH		?= \":\" ;\n",
"STDHDRS		?= ; \n",
"SUFLIB		?= .lib ;\n",
"SUFOBJ		?= .o ;\n",
"}\n",
"else if $(OS) = BEOS && $(METROWERKS)\n",
"{\n",
"AR		?= mwld -xml -o ;\n",
"BINDIR		?= /boot/apps ;\n",
"CC		?= mwcc ;\n",
"CCFLAGS		?= -nosyspath ;\n",
"C++		?= $(CC) ;\n",
"C++FLAGS	?= -nosyspath ;\n",
"FORTRAN		?= \"\" ;\n",
"LIBDIR		?= /boot/develop/libraries ;\n",
"LINK		?= mwld ;\n",
"LINKFLAGS	?= \"\" ;\n",
"MANDIR		?= /boot/documentation/\"Shell Tools\"/HTML ;\n",
"NOARSCAN	?= true ;\n",
"STDHDRS		?= /boot/develop/headers/posix ;\n",
"}\n",
"else if $(OS) = BEOS \n",
"{\n",
"BINDIR		?= /boot/apps ;\n",
"CC		?= gcc ;\n",
"C++		?= $(CC) ;\n",
"FORTRAN		?= \"\" ;\n",
"LIBDIR		?= /boot/develop/libraries ;\n",
"LINK		?= gcc ;\n",
"LINKLIBS	?= -lnet ;\n",
"NOARSCAN	?= true ;\n",
"STDHDRS		?= /boot/develop/headers/posix ;\n",
"}\n",
"else if $(UNIX)\n",
"{\n",
"switch $(OS)\n",
"{\n",
"case AIX :\n",
"LINKLIBS	?= -lbsd ;\n",
"case AMIGA :\n",
"CC		?= gcc ;\n",
"YACC		?= bison -y ;\n",
"case CYGWIN :	\n",
"CC		?= gcc ;\n",
"CCFLAGS 	+= -D__cygwin__ ;\n",
"LEX		?= flex ;\n",
"JAMSHELL	?= sh -c ;\n",
"RANLIB		?= \"\" ;\n",
"SUFEXE		?= .exe ;\n",
"YACC		?= bison -y ;\n",
"case DGUX :\n",
"RANLIB		?= \"\" ;\n",
"RELOCATE 	?= true ;\n",
"case HPUX :\n",
"RANLIB		?= \"\" ;\n",
"case INTERIX :\n",
"CC		?= gcc ;\n",
"JAMSHELL	?= sh -c ;\n",
"RANLIB		?= \"\" ;\n",
"case IRIX :\n",
"RANLIB		?= \"\" ;\n",
"case MPEIX :\n",
"CC		?= gcc ;\n",
"C++		?= gcc ;\n",
"CCFLAGS		+= -D_POSIX_SOURCE ;\n",
"HDRS		+= /usr/include ;\n",
"RANLIB		?= \"\" ; \n",
"NOARSCAN	?= true ;\n",
"NOARUPDATE	?= true ;\n",
"case MVS :\n",
"RANLIB		?= \"\" ; \n",
"case NEXT :\n",
"AR		?= libtool -o ;\n",
"RANLIB		?= \"\" ;\n",
"case MACOSX :\n",
"AR		?= libtool -o ;\n",
"C++		?= c++ ;\n",
"MANDIR		?= /usr/local/share/man ;\n",
"RANLIB		?= \"\" ;\n",
"case NCR :\n",
"RANLIB		?= \"\" ;\n",
"case PTX :\n",
"RANLIB		?= \"\" ;\n",
"case QNX :\n",
"AR		?= wlib ;\n",
"CC		?= cc ;\n",
"CCFLAGS		?= -Q ;	# quiet\n",
"C++		?= $(CC) ;\n",
"C++FLAGS	?= -Q ;	# quiet\n",
"LINK		?= $(CC) ;\n",
"LINKFLAGS	?= -Q ;	# quiet\n",
"NOARSCAN	?= true ;\n",
"RANLIB		?= \"\" ;\n",
"case SCO :\n",
"RANLIB		?= \"\" ;\n",
"RELOCATE 	?= true ;\n",
"case SINIX :\n",
"RANLIB		?= \"\" ;\n",
"case SOLARIS :\n",
"RANLIB		?= \"\" ;\n",
"AR		?= \"/usr/ccs/bin/ar ru\" ;\n",
"case UNICOS :\n",
"NOARSCAN 	?= true ;\n",
"OPTIM 		?= -O0 ;\n",
"case UNIXWARE :\n",
"RANLIB		?= \"\" ;\n",
"RELOCATE 	?= true ;\n",
"}\n",
"CCFLAGS		?= ;\n",
"C++FLAGS	?= $(CCFLAGS) ;\n",
"CHMOD		?= chmod ;\n",
"LEX		?= lex ;\n",
"LINKFLAGS	?= $(CCFLAGS) ;\n",
"LINKLIBS	?= ;\n",
"OPTIM		?= -O ;\n",
"RANLIB		?= ranlib ;\n",
"YACC		?= yacc ;\n",
"YACCGEN		?= .c ;\n",
"YACCFILES	?= y.tab ;\n",
"YACCFLAGS	?= -d ;\n",
"}\n",
"AR		?= ar ru ;\n",
"AS		?= as ;\n",
"ASFLAGS		?= ;\n",
"AWK		?= awk ;\n",
"BINDIR		?= /usr/local/bin ;\n",
"C++		?= cc ;\n",
"C++FLAGS	?= ;\n",
"CC		?= cc ;\n",
"CCFLAGS		?= ;\n",
"CP		?= cp -f ;\n",
"CRELIB		?= ;\n",
"DOT		?= . ;\n",
"DOTDOT		?= .. ;\n",
"EXEMODE		?= 711 ;\n",
"FILEMODE	?= 644 ;\n",
"FORTRAN		?= f77 ;\n",
"FORTRANFLAGS	?= ;\n",
"HDRS		?= ;\n",
"JAMFILE		?= Jamfile ;\n",
"JAMRULES	?= Jamrules ;\n",
"LEX		?= ;\n",
"LIBDIR		?= /usr/local/lib ;\n",
"LINK		?= $(CC) ;\n",
"LINKFLAGS	?= ;\n",
"LINKLIBS	?= ;\n",
"LN		?= ln ;\n",
"MANDIR		?= /usr/local/man ;\n",
"MKDIR		?= mkdir ;\n",
"MV		?= mv -f ;\n",
"OPTIM		?= ;\n",
"RCP		?= rcp ;\n",
"RM		?= rm -f ;\n",
"RSH		?= rsh ;\n",
"SED		?= sed ;\n",
"SHELLHEADER	?= \"#!/bin/sh\" ;\n",
"SHELLMODE	?= 755 ;\n",
"SLASH		?= / ;\n",
"STDHDRS		?= /usr/include ;\n",
"SUFEXE		?= \"\" ;\n",
"SUFLIB		?= .a ;\n",
"SUFOBJ		?= .o ;\n",
"UNDEFFLAG	?= \"-u _\" ;\n",
"YACC		?= ;\n",
"YACCGEN		?= ;\n",
"YACCFILES	?= ;\n",
"YACCFLAGS	?= ;\n",
"HDRPATTERN = \n",
"\"^[ 	]*#[ 	]*include[ 	]*[<\\\"]([^\\\">]*)[\\\">].*$\" ;\n",
"OSFULL = $(OS)$(OSVER)$(OSPLAT) $(OS)$(OSPLAT) $(OS)$(OSVER) $(OS) ;\n",
"Depends all : shell files lib exe obj ;\n",
"Depends all shell files lib exe obj : first ;\n",
"NotFile all first shell files lib exe obj dirs clean uninstall ;\n",
"Always clean uninstall ;\n",
"rule As\n",
"{\n",
"Depends $(<) : $(>) ;\n",
"ASFLAGS on $(<) += $(ASFLAGS) $(SUBDIRASFLAGS) ;\n",
"ASHDRS on $(<) = [ FIncludes $(SEARCH_SOURCE) $(SUBDIRHDRS) $(HDRS) ] ;\n",
"}\n",
"rule Bulk\n",
"{\n",
"local i ;\n",
"for i in $(>)\n",
"{\n",
"File $(i:D=$(<)) : $(i) ;\n",
"}\n",
"}\n",
"rule Cc\n",
"{\n",
"Depends $(<) : $(>) ;\n",
"if $(RELOCATE)\n",
"{\n",
"CcMv $(<) : $(>) ;\n",
"}\n",
"CCFLAGS on $(<) += $(CCFLAGS) $(SUBDIRCCFLAGS) $(OPTIM) ;\n",
"CCHDRS on $(<) = [ on $(<) FIncludes $(HDRS) ] ;\n",
"CCDEFS on $(<) = [ on $(<) FDefines $(DEFINES) ] ;\n",
"}\n",
"rule C++\n",
"{\n",
"Depends $(<) : $(>) ;\n",
"if $(RELOCATE)\n",
"{\n",
"CcMv $(<) : $(>) ;\n",
"}\n",
"C++FLAGS on $(<) += $(C++FLAGS) $(SUBDIRC++FLAGS) $(OPTIM) ;\n",
"CCHDRS on $(<) = [ on $(<) FIncludes $(HDRS) ] ;\n",
"CCDEFS on $(<) = [ on $(<) FDefines $(DEFINES) ] ;\n",
"}\n",
"rule Chmod\n",
"{\n",
"if $(CHMOD) { Chmod1 $(<) ; }\n",
"}\n",
"rule File\n",
"{\n",
"Depends files : $(<) ;\n",
"Depends $(<) : $(>) ;\n",
"SEARCH on $(>) = $(SEARCH_SOURCE) ;\n",
"MODE on $(<) = $(FILEMODE) ;\n",
"Chmod $(<) ;\n",
"}\n",
"rule Fortran\n",
"{\n",
"Depends $(<) : $(>) ;\n",
"}\n",
"rule GenFile \n",
"{\n",
"local _t = [ FGristSourceFiles $(<) ] ;\n",
"local _s = [ FAppendSuffix $(>[1]) : $(SUFEXE) ] ;\n",
"Depends $(_t) : $(_s) $(>[2-]) ;\n",
"GenFile1 $(_t) : $(_s) $(>[2-]) ;\n",
"Clean clean : $(_t) ;\n",
"}\n",
"rule GenFile1\n",
"{\n",
"MakeLocate $(<) : $(LOCATE_SOURCE) ;\n",
"SEARCH on $(>) = $(SEARCH_SOURCE) ;\n",
"}\n",
"rule HardLink\n",
"{\n",
"Depends files : $(<) ;\n",
"Depends $(<) : $(>) ;\n",
"SEARCH on $(>) = $(SEARCH_SOURCE) ;\n",
"}\n",
"rule HdrRule\n",
"{\n",
"local s = $(>:G=$(HDRGRIST:E)) ;\n",
"Includes $(<) : $(s) ;\n",
"SEARCH on $(s) = $(HDRSEARCH) ;\n",
"NoCare $(s) ;\n",
"HDRSEARCH on $(s) = $(HDRSEARCH) ;\n",
"HDRSCAN on $(s) = $(HDRSCAN) ;\n",
"HDRRULE on $(s) = $(HDRRULE) ;\n",
"HDRGRIST on $(s) = $(HDRGRIST) ;\n",
"}\n",
"rule InstallInto\n",
"{\n",
"local i t ;\n",
"t = $(>:G=installed) ;\n",
"Depends install : $(t) ;\n",
"Depends $(t) : $(>) ;\n",
"SEARCH on $(>) = $(SEARCH_SOURCE) ;\n",
"MakeLocate $(t) : $(<) ;\n",
"Clean uninstall : $(t) ;\n",
"for i in $(>)\n",
"{\n",
"Install $(i:G=installed) : $(i) ;\n",
"}\n",
"Chmod $(t) ;\n",
"if $(UNIX)\n",
"{\n",
"if $(OWNER) { Chown $(t) ; OWNER on $(t) = $(OWNER) ; }\n",
"if $(GROUP) { Chgrp $(t) ; GROUP on $(t) = $(GROUP) ; }\n",
"}\n",
"}\n",
"rule InstallBin\n",
"{\n",
"local _t = [ FAppendSuffix $(>) : $(SUFEXE) ] ;\n",
"InstallInto $(<) : $(_t) ;\n",
"MODE on $(_t:G=installed) = $(EXEMODE) ;\n",
"}\n",
"rule InstallFile\n",
"{\n",
"InstallInto $(<) : $(>) ;\n",
"MODE on $(>:G=installed) = $(FILEMODE) ;\n",
"}\n",
"rule InstallLib\n",
"{\n",
"InstallInto $(<) : $(>) ;\n",
"MODE on $(>:G=installed) = $(FILEMODE) ;\n",
"}\n",
"rule InstallMan\n",
"{\n",
"local i s d ;\n",
"for i in $(>)\n",
"{\n",
"switch $(i:S)\n",
"{\n",
"case .1 : s = 1 ; case .2 : s = 2 ; case .3 : s = 3 ;\n",
"case .4 : s = 4 ; case .5 : s = 5 ; case .6 : s = 6 ;\n",
"case .7 : s = 7 ; case .8 : s = 8 ; case .l : s = l ;\n",
"case .n : s = n ; case .man : s = 1 ;\n",
"}\n",
"d = man$(s) ;\n",
"InstallInto $(d:R=$(<)) : $(i) ;\n",
"}\n",
"MODE on $(>:G=installed) = $(FILEMODE) ;\n",
"}\n",
"rule InstallShell\n",
"{\n",
"InstallInto $(<) : $(>) ;\n",
"MODE on $(>:G=installed) = $(SHELLMODE) ;\n",
"}\n",
"rule Lex\n",
"{\n",
"LexMv $(<) : $(>) ;\n",
"Depends $(<) : $(>) ;\n",
"MakeLocate $(<) : $(LOCATE_SOURCE) ;\n",
"Clean clean : $(<) ;\n",
"}\n",
"rule Library\n",
"{\n",
"LibraryFromObjects $(<) : $(>:S=$(SUFOBJ)) ;\n",
"Objects $(>) ;\n",
"}\n",
"rule LibraryFromObjects\n",
"{\n",
"local _i _l _s ;\n",
"_s = [ FGristFiles $(>) ] ;\n",
"_l = $(<:S=$(SUFLIB)) ;\n",
"if $(KEEPOBJS)\n",
"{\n",
"Depends obj : $(_s) ;\n",
"}\n",
"else\n",
"{\n",
"Depends lib : $(_l) ;\n",
"}\n",
"if ! $(_l:D)\n",
"{\n",
"MakeLocate $(_l) $(_l)($(_s:BS)) : $(LOCATE_TARGET) ;\n",
"}\n",
"if $(NOARSCAN) \n",
"{ \n",
"Depends $(_l) : $(_s) ;\n",
"}\n",
"else\n",
"{\n",
"Depends $(_l) : $(_l)($(_s:BS)) ;\n",
"for _i in $(_s)\n",
"{\n",
"Depends $(_l)($(_i:BS)) : $(_i) ;\n",
"}\n",
"}\n",
"Clean clean : $(_l) ;\n",
"if $(CRELIB) { CreLib $(_l) : $(_s[1]) ; }\n",
"Archive $(_l) : $(_s) ;\n",
"if $(RANLIB) { Ranlib $(_l) ; }\n",
"if ! ( $(NOARSCAN) || $(KEEPOBJS) ) { RmTemps $(_l) : $(_s) ; }\n",
"}\n",
"rule Link\n",
"{\n",
"MODE on $(<) = $(EXEMODE) ;\n",
"Chmod $(<) ;\n",
"}\n",
"rule LinkLibraries\n",
"{\n",
"local _t = [ FAppendSuffix $(<) : $(SUFEXE) ] ;\n",
"Depends $(_t) : $(>:S=$(SUFLIB)) ;\n",
"NEEDLIBS on $(_t) += $(>:S=$(SUFLIB)) ;\n",
"}\n",
"rule Main\n",
"{\n",
"MainFromObjects $(<) : $(>:S=$(SUFOBJ)) ;\n",
"Objects $(>) ;\n",
"}\n",
"rule MainFromObjects\n",
"{\n",
"local _s _t ;\n",
"_s = [ FGristFiles $(>) ] ;\n",
"_t = [ FAppendSuffix $(<) : $(SUFEXE) ] ;\n",
"if $(_t) != $(<)\n",
"{\n",
"Depends $(<) : $(_t) ;\n",
"NotFile $(<) ;\n",
"}\n",
"Depends exe : $(_t) ;\n",
"Depends $(_t) : $(_s) ;\n",
"MakeLocate $(_t) : $(LOCATE_TARGET) ;\n",
"Clean clean : $(_t) ;\n",
"Link $(_t) : $(_s) ;\n",
"}\n",
"rule MakeLocate\n",
"{\n",
"if $(>)\n",
"{\n",
"LOCATE on $(<) = $(>) ;\n",
"Depends $(<) : $(>[1]) ;\n",
"MkDir $(>[1]) ;\n",
"}\n",
"}\n",
"rule MkDir\n",
"{\n",
"NoUpdate $(<) ;\n",
"if $(<) != $(DOT) && ! $($(<)-mkdir) \n",
"{\n",
"local s ;\n",
"$(<)-mkdir = true ;\n",
"MkDir1 $(<) ;\n",
"Depends dirs : $(<) ;\n",
"s = $(<:P) ;\n",
"if $(NT)\n",
"{\n",
"switch $(s)\n",
"{\n",
"case *:   : s = ;\n",
"case *:\\\\ : s = ;\n",
"}\n",
"}\n",
"if $(s) && $(s) != $(<)\n",
"{\n",
"Depends $(<) : $(s) ;\n",
"MkDir $(s) ;\n",
"}\n",
"else if $(s)\n",
"{\n",
"NotFile $(s) ;\n",
"}\n",
"}\n",
"}\n",
"rule Object\n",
"{\n",
"Clean clean : $(<) ;\n",
"MakeLocate $(<) : $(LOCATE_TARGET) ;\n",
"SEARCH on $(>) = $(SEARCH_SOURCE) ;\n",
"HDRS on $(<) = $(SEARCH_SOURCE) $(SUBDIRHDRS) $(HDRS) ;\n",
"HDRRULE on $(>) = HdrRule ;\n",
"HDRSCAN on $(>) = $(HDRPATTERN) ;\n",
"HDRSEARCH on $(>) = \n",
"$(SEARCH_SOURCE:E) $(SUBDIRHDRS) $(HDRS) $(STDHDRS) ;\n",
"HDRGRIST on $(>) = $(HDRGRIST) ;\n",
"DEFINES on $(<) += $(DEFINES) ;\n",
"switch $(>:S)\n",
"{\n",
"case .asm : As $(<) : $(>) ;\n",
"case .c :	Cc $(<) : $(>) ;\n",
"case .C :	C++ $(<) : $(>) ;\n",
"case .cc :	C++ $(<) : $(>) ;\n",
"case .cpp : C++ $(<) : $(>) ;\n",
"case .f :	Fortran $(<) : $(>) ;\n",
"case .l :	Cc $(<) : $(<:S=.c) ;\n",
"Lex $(<:S=.c) : $(>) ;\n",
"case .s :	As $(<) : $(>) ;\n",
"case .y :	Cc $(<) : $(<:S=$(YACCGEN)) ;\n",
"Yacc $(<:S=$(YACCGEN)) : $(>) ;\n",
"case * :	UserObject $(<) : $(>) ;\n",
"}\n",
"}\n",
"rule ObjectCcFlags\n",
"{\n",
"CCFLAGS on [ FGristFiles $(<:S=$(SUFOBJ)) ] += $(>) ;\n",
"}\n",
"rule ObjectC++Flags\n",
"{\n",
"C++FLAGS on [ FGristFiles $(<:S=$(SUFOBJ)) ] += $(>) ;\n",
"}\n",
"rule ObjectDefines\n",
"{\n",
"local s = [ FGristFiles $(<:S=$(SUFOBJ)) ] ;\n",
"DEFINES on $(s) += $(>) ;\n",
"CCDEFS on $(s) = [ on $(s) FDefines $(DEFINES) ] ;\n",
"}\n",
"rule ObjectHdrs\n",
"{\n",
"local s = [ FGristFiles $(<:S=$(SUFOBJ)) ] ;\n",
"HDRS on $(s) += $(>) ;\n",
"CCHDRS on $(s) = [ on $(s) FIncludes $(HDRS) ] ;\n",
"}\n",
"rule Objects\n",
"{\n",
"local _i ;\n",
"for _i in [ FGristFiles $(<) ]\n",
"{\n",
"Object $(_i:S=$(SUFOBJ)) : $(_i) ;\n",
"Depends obj : $(_i:S=$(SUFOBJ)) ;\n",
"}\n",
"}\n",
"rule RmTemps\n",
"{\n",
"Temporary $(>) ;\n",
"}\n",
"rule Setuid\n",
"{\n",
"MODE on [ FAppendSuffix $(<) : $(SUFEXE) ] = 4711 ;\n",
"}\n",
"rule Shell\n",
"{\n",
"Depends shell : $(<) ;\n",
"Depends $(<) : $(>) ;\n",
"SEARCH on $(>) = $(SEARCH_SOURCE) ;\n",
"MODE on $(<) = $(SHELLMODE) ;\n",
"Clean clean : $(<) ;\n",
"Chmod $(<) ;\n",
"}\n",
"rule SubDir\n",
"{\n",
"local _top = $(<[1]) ;\n",
"if ! $($(_top))\n",
"{\n",
"if ! $(_top)\n",
"{\n",
"Exit SubDir syntax error ;\n",
"}\n",
"$(_top) = [ FSubDir $(<[2-]) ] ;\n",
"}\n",
"if ! $($(_top)-included)\n",
"{\n",
"$(_top)-included = TRUE ;\n",
"include $($(_top)RULES:E=$(JAMRULES:R=$($(_top)))) ;\n",
"}\n",
"local _s = [ FDirName $(<[2-]) ] ;\n",
"SUBDIR = $(_s:R=$($(_top))) ;\n",
"SUBDIR_TOKENS = $(<[2-]) ;\n",
"SEARCH_SOURCE = $(SUBDIR) ;\n",
"LOCATE_SOURCE = $(ALL_LOCATE_TARGET) $(SUBDIR) ;\n",
"LOCATE_TARGET = $(ALL_LOCATE_TARGET) $(SUBDIR) ;\n",
"SOURCE_GRIST = [ FGrist $(<[2-]) ] ;\n",
"SUBDIRCCFLAGS = ;\n",
"SUBDIRC++FLAGS = ;\n",
"SUBDIRHDRS = ;\n",
"}\n",
"rule SubDirCcFlags\n",
"{\n",
"SUBDIRCCFLAGS += $(<) ;\n",
"}\n",
"rule SubDirC++Flags\n",
"{\n",
"SUBDIRC++FLAGS += $(<) ;\n",
"}\n",
"rule SubDirHdrs\n",
"{\n",
"SUBDIRHDRS += [ FDirName $(<) ] ;\n",
"}\n",
"rule SubInclude\n",
"{\n",
"local _s ;\n",
"if ! $($(<[1]))\n",
"{\n",
"Exit Top level of source tree has not been set with $(<[1]) ;\n",
"}\n",
"_s = [ FDirName $(<[2-]) ] ;\n",
"include $(JAMFILE:D=$(_s):R=$($(<[1]))) ;\n",
"}\n",
"rule Undefines\n",
"{\n",
"UNDEFS on [ FAppendSuffix $(<) : $(SUFEXE) ] += $(UNDEFFLAG)$(>) ;\n",
"}\n",
"rule UserObject\n",
"{\n",
"Exit \"Unknown suffix on\" $(>) \"- see UserObject rule in Jamfile(5).\" ;\n",
"}\n",
"rule Yacc\n",
"{\n",
"local _h ;\n",
"_h = $(<:BS=.h) ;\n",
"MakeLocate $(<) $(_h) : $(LOCATE_SOURCE) ;\n",
"if $(YACC)\n",
"{\n",
"Depends $(<) $(_h) : $(>) ;\n",
"Yacc1 $(<) $(_h) : $(>) ;\n",
"YaccMv $(<) $(_h) : $(>) ;\n",
"Clean clean : $(<) $(_h) ;\n",
"}\n",
"Includes $(<) : $(_h) ;\n",
"}\n",
"rule FGrist\n",
"{\n",
"return $(<:J=!) ;\n",
"}\n",
"rule FGristFiles \n",
"{\n",
"return $(<:G=$(SOURCE_GRIST:E)) ;\n",
"}\n",
"rule FGristSourceFiles\n",
"{\n",
"if ! $(SOURCE_GRIST)\n",
"{\n",
"return $(<) ;\n",
"}\n",
"else \n",
"{\n",
"local _i _o ;\n",
"for _i in $(<)\n",
"{\n",
"switch $(_i)\n",
"{\n",
"case *.h :	_o += $(_i) ;\n",
"case * : 	_o += $(_i:G=$(SOURCE_GRIST)) ;\n",
"}\n",
"}\n",
"return $(_o) ;\n",
"}\n",
"}\n",
"rule FSubDir\n",
"{\n",
"if ! $(<[1]) \n",
"{\n",
"return $(DOT) ;\n",
"} \n",
"else\n",
"{\n",
"local _i _d ;\n",
"_d = $(DOTDOT) ;\n",
"for _i in $(<[2-])\n",
"{\n",
"_d = $(_d:R=$(DOTDOT)) ;\n",
"}\n",
"return $(_d) ;\n",
"}\n",
"}\n",
"rule _makeCommon\n",
"{\n",
"if $($(<)[1]) && $($(<)[1]) = $($(>)[1])\n",
"{\n",
"$(<) = $($(<)[2-]) ;\n",
"$(>) = $($(>)[2-]) ;\n",
"_makeCommon $(<) : $(>) ;\n",
"}\n",
"}\n",
"rule FRelPath\n",
"{\n",
"local _l _r ;\n",
"_l = $(<) ;\n",
"_r = $(>) ;\n",
"_makeCommon _l : _r ;\n",
"_l = [ FSubDir $(_l) ] ;\n",
"_r = [ FDirName $(_r) ] ;\n",
"if $(_r) = $(DOT) {\n",
"return $(_l) ;\n",
"} else {\n",
"return $(_r:R=$(_l)) ;\n",
"}\n",
"}\n",
"rule FAppendSuffix\n",
"{\n",
"if $(>)\n",
"{\n",
"local _i _o ;\n",
"for _i in $(<)\n",
"{\n",
"if $(_i:S)\n",
"{\n",
"_o += $(_i) ;\n",
"}\n",
"else\n",
"{\n",
"_o += $(_i:S=$(>)) ;\n",
"}\n",
"}\n",
"return $(_o) ;\n",
"}\n",
"else\n",
"{\n",
"return $(<) ;\n",
"}\n",
"}\n",
"rule FQuote { return \\\\\\\"$(<)\\\\\\\" ; }\n",
"rule FDefines { return -D$(<) ; }\n",
"rule FIncludes { return -I$(<) ; }\n",
"rule FDirName\n",
"{\n",
"local _i ;\n",
"local _s = $(DOT) ;\n",
"for _i in $(<)\n",
"{\n",
"_s = $(_i:R=$(_s)) ;\n",
"}\n",
"return $(_s) ;\n",
"}\n",
"if $(OS2)\n",
"{\n",
"rule FQuote { return \\\"$(<)\\\" ; }\n",
"rule FInclude { return -I$(<) ; }\n",
"}\n",
"else if $(NT)\n",
"{\n",
"rule FInclude { return -I$(<) ; }\n",
"}\n",
"else if $(MAC)\n",
"{\n",
"rule FQuote { return \\\"$(<)\\\" ; }\n",
"rule FDefines { return \"-define '$(<)'\" ; }\n",
"rule FIncludes { return \\\"$(<:J=,)\\\" ; }\n",
"}\n",
"else if $(VMS)\n",
"{\n",
"rule FQuote { return \\\"\\\"\\\"$(<)\\\"\\\"\\\" ; }\n",
"rule FDefines { return \"/define=( $(<:J=,) )\" ; }\n",
"rule FIncludes { return \"/inc=( $(<:J=,) )\" ; }\n",
"rule FDirName\n",
"{\n",
"local _s _i ;\n",
"if ! $(<)\n",
"{\n",
"_s = $(DOT) ;\n",
"}\n",
"else \n",
"{\n",
"switch $(<[1])\n",
"{\n",
"case *:* : _s = $(<[1]) ;\n",
"case \\\\[*\\\\] : _s = $(<[1]) ;\n",
"case * : _s = [.$(<[1])] ;\n",
"}\n",
"for _i in [.$(<[2-])]\n",
"{\n",
"_s = $(_i:R=$(_s)) ;\n",
"}\n",
"}\n",
"return $(_s) ;\n",
"}\n",
"}\n",
"actions updated together piecemeal Archive\n",
"{\n",
"$(AR) $(<) $(>)\n",
"}\n",
"actions As\n",
"{\n",
"$(AS) $(ASFLAGS) $(ASHDRS) -o $(<) $(>)\n",
"}\n",
"actions C++\n",
"{\n",
"$(C++) -c -o $(<) $(C++FLAGS) $(CCDEFS) $(CCHDRS) $(>)\n",
"}\n",
"actions Cc\n",
"{\n",
"$(CC) -c -o $(<) $(CCFLAGS) $(CCDEFS) $(CCHDRS) $(>)\n",
"}\n",
"actions Chgrp\n",
"{\n",
"chgrp $(GROUP) $(<)\n",
"}\n",
"actions Chmod1\n",
"{\n",
"$(CHMOD) $(MODE) $(<)\n",
"}\n",
"actions Chown\n",
"{\n",
"chown $(OWNER) $(<)\n",
"}\n",
"actions piecemeal together existing Clean\n",
"{\n",
"$(RM) $(>)\n",
"}\n",
"actions File\n",
"{\n",
"$(CP) $(>) $(<)\n",
"}\n",
"actions GenFile1\n",
"{\n",
"$(>[1]) $(<) $(>[2-])\n",
"}\n",
"actions Fortran\n",
"{\n",
"$(FORTRAN) $(FORTRANFLAGS) -o $(<) $(>)\n",
"}\n",
"actions HardLink\n",
"{\n",
"$(RM) $(<) && $(LN) $(>) $(<)\n",
"}\n",
"actions Install\n",
"{\n",
"$(CP) $(>) $(<) \n",
"}\n",
"actions Lex\n",
"{\n",
"$(LEX) $(>)\n",
"}\n",
"actions LexMv\n",
"{\n",
"$(MV) lex.yy.c $(<)\n",
"}\n",
"actions Link bind NEEDLIBS\n",
"{\n",
"$(LINK) $(LINKFLAGS) -o $(<) $(UNDEFS) $(>) $(NEEDLIBS) $(LINKLIBS) \n",
"}\n",
"actions MkDir1\n",
"{\n",
"$(MKDIR) $(<)\n",
"}\n",
"actions together Ranlib\n",
"{\n",
"$(RANLIB) $(<)\n",
"}\n",
"actions quietly updated piecemeal together RmTemps\n",
"{\n",
"$(RM) $(>)\n",
"}\n",
"actions Shell\n",
"{\n",
"$(AWK) '\n",
"NR == 1 { print \"$(SHELLHEADER)\" }\n",
"NR == 1 && /^[#:]/ { next }\n",
"/^##/ { next }\n",
"{ print }\n",
"' < $(>) > $(<)\n",
"}\n",
"actions Yacc1\n",
"{\n",
"$(YACC) $(YACCFLAGS) $(>)\n",
"}\n",
"actions YaccMv\n",
"{\n",
"$(MV) $(YACCFILES).c $(<[1])\n",
"$(MV) $(YACCFILES).h $(<[2])\n",
"}\n",
"if $(RELOCATE)\n",
"{\n",
"actions C++\n",
"{\n",
"$(C++) -c $(C++FLAGS) $(CCDEFS) $(CCHDRS) $(>)\n",
"}\n",
"actions Cc\n",
"{\n",
"$(CC) -c $(CCFLAGS) $(CCDEFS) $(CCHDRS) $(>)\n",
"}\n",
"actions ignore CcMv\n",
"{\n",
"[ $(<) != $(>:BS=$(SUFOBJ)) ] && $(MV) $(>:BS=$(SUFOBJ)) $(<)\n",
"}\n",
"}\n",
"if $(NOARUPDATE)\n",
"{\n",
"actions Archive\n",
"{\n",
"$(AR) $(<) $(>)\n",
"}\n",
"}\n",
"if $(NT) && $(MSVCNT)\n",
"{\n",
"actions updated together piecemeal Archive\n",
"{\n",
"if exist $(<) set _$(<:B)_=$(<)\n",
"$(AR) /out:$(<) %_$(<:B)_% $(>)\n",
"}\n",
"actions As\n",
"{\n",
"$(AS) /Ml /p /v /w2 $(>) $(<) ,nul,nul;\n",
"}\n",
"actions Cc\n",
"{\n",
"$(CC) /c /Fo$(<) $(CCFLAGS) $(CCDEFS) $(CCHDRS) /I$(STDHDRS) $(>)\n",
"}\n",
"actions C++\n",
"{\n",
"$(C++) /c /Fo$(<) $(C++FLAGS) $(CCDEFS) $(CCHDRS) /I$(STDHDRS) /Tp$(>)\n",
"}\n",
"actions Link bind NEEDLIBS\n",
"{\n",
"$(LINK) $(LINKFLAGS) /out:$(<) $(UNDEFS) $(>) $(NEEDLIBS) $(LINKLIBS)\n",
"}\n",
"}\n",
"else if $(NT) && $(MSVC)\n",
"{\n",
"actions updated together piecemeal Archive\n",
"{\n",
"$(AR) $(<) -+$(>)\n",
"}\n",
"actions Cc\n",
"{\n",
"$(CC) /c /Fo$(<) $(CCFLAGS) $(CCDEFS) $(CCHDRS) $(>)\n",
"}\n",
"actions C++\n",
"{\n",
"$(C++) /c /Fo$(<) $(C++FLAGS) $(CCDEFS) $(CCHDRS) /Tp$(>)\n",
"}\n",
"actions Link bind NEEDLIBS\n",
"{\n",
"$(LINK) $(LINKFLAGS) /out:$(<) $(UNDEFS) $(>) $(NEEDLIBS) $(LINKLIBS)\n",
"}\n",
"}\n",
"else if $(NT) && $(BCCROOT)\n",
"{\n",
"actions updated together piecemeal Archive\n",
"{\n",
"$(AR) $(<) -+$(>)\n",
"}\n",
"actions Link bind NEEDLIBS\n",
"{\n",
"$(LINK) -e$(<) $(LINKFLAGS) $(UNDEFS) -L$(LINKLIBS) $(NEEDLIBS) $(>)\n",
"}\n",
"actions C++\n",
"{\n",
"$(C++) -c -o$(<) $(C++FLAGS) $(CCDEFS) $(CCHDRS) $(>)\n",
"}\n",
"actions Cc\n",
"{\n",
"$(CC) -c -o$(<) $(CCFLAGS) $(CCDEFS) $(CCHDRS) $(>)\n",
"}\n",
"}\n",
"else if $(OS2) && $(WATCOM)\n",
"{\n",
"actions together piecemeal Archive\n",
"{\n",
"$(AR) $(<) +-$(>) \n",
"}\n",
"actions Cc\n",
"{\n",
"$(CC) /Fo=$(<) $(CCFLAGS) $(CCDEFS) $(CCHDRS) $(>)\n",
"}\n",
"actions C++\n",
"{\n",
"$(C++) /Fo=$(<) $(C++FLAGS) $(CCDEFS) $(CCHDRS) $(>)\n",
"}\n",
"actions Link bind NEEDLIBS\n",
"{\n",
"$(LINK) $(LINKFLAGS) /Fe=$(<) $(UNDEFS) $(>) $(NEEDLIBS) $(LINKLIBS)\n",
"}\n",
"actions Shell\n",
"{\n",
"$(CP) $(>) $(<)\n",
"}\n",
"}\n",
"else if $(VMS)\n",
"{\n",
"actions updated together piecemeal Archive \n",
"{\n",
"lib/replace $(<) $(>[1]) ,$(>[2-])\n",
"}\n",
"actions Cc\n",
"{ \n",
"$(CC)/obj=$(<) $(CCFLAGS) $(CCDEFS) $(CCHDRS) $(>) \n",
"}\n",
"actions C++\n",
"{ \n",
"$(C++)/obj=$(<) $(C++FLAGS) $(CCDEFS) $(CCHDRS) $(>) \n",
"}\n",
"actions piecemeal together existing Clean\n",
"{\n",
"$(RM) $(>[1]);* ,$(>[2-]);*\n",
"}\n",
"actions together quietly CreLib\n",
"{\n",
"if f$search(\"$(<)\") .eqs. \"\" then lib/create $(<)\n",
"}\n",
"actions GenFile1\n",
"{\n",
"mcr $(>[1]) $(<) $(>[2-])\n",
"}\n",
"actions Link bind NEEDLIBS\n",
"{\n",
"$(LINK)/exe=$(<) $(LINKFLAGS) $(>:J=,) ,$(NEEDLIBS)/lib ,$(LINKLIBS)\n",
"}\n",
"actions quietly updated piecemeal together RmTemps\n",
"{\n",
"$(RM) $(>[1]);* ,$(>[2-]);*\n",
"}\n",
"actions Shell\n",
"{\n",
"$(CP) $(>) $(<)\n",
"}\n",
"}\n",
"else if $(MAC)\n",
"{\n",
"actions together Archive \n",
"{\n",
"$(LINK) -library -o $(<) $(>)\n",
"}\n",
"actions Cc\n",
"{\n",
"set -e MWCincludes $(CCHDRS)\n",
"$(CC) -o $(<) $(CCFLAGS) $(CCDEFS) $(>) \n",
"}\n",
"actions C++\n",
"{\n",
"set -e MWCincludes $(CCHDRS)\n",
"$(CC) -o $(<) $(C++FLAGS) $(CCDEFS) $(>) \n",
"}\n",
"actions Link bind NEEDLIBS\n",
"{\n",
"$(LINK) -o $(<) $(LINKFLAGS) $(>) $(NEEDLIBS) \"$(LINKLIBS)\"\n",
"}\n",
"}\n",
"if $(WIN98)\n",
"{\n",
"actions existing Clean\n",
"{\n",
"del $(>)\n",
"}\n",
"}\n",
"rule BULK { Bulk $(<) : $(>) ; }\n",
"rule FILE { File $(<) : $(>) ; }\n",
"rule HDRRULE { HdrRule $(<) : $(>) ; }\n",
"rule INSTALL { Install $(<) : $(>) ; }\n",
"rule LIBRARY { Library $(<) : $(>) ; }\n",
"rule LIBS { LinkLibraries $(<) : $(>) ; }\n",
"rule LINK { Link $(<) : $(>) ; }\n",
"rule MAIN { Main $(<) : $(>) ; }\n",
"rule SETUID { Setuid $(<) ; }\n",
"rule SHELL { Shell $(<) : $(>) ; }\n",
"rule UNDEFINES { Undefines $(<) : $(>) ; }\n",
"rule INSTALLBIN { InstallBin $(BINDIR) : $(<) ; }\n",
"rule INSTALLLIB { InstallLib $(LIBDIR) : $(<) ; }\n",
"rule INSTALLMAN { InstallMan $(MANDIR) : $(<) ; }\n",
"rule addDirName { $(<) += [ FDirName $(>) ] ; }\n",
"rule makeCommon { _makeCommon $(<) : $(>) ; }\n",
"rule makeDirName { $(<) = [ FDirName $(>) ] ; }\n",
"rule makeGrist { $(<) = [ FGrist $(>) ] ; }\n",
"rule makeGristedName { $(<) = [ FGristSourceFiles $(>) ] ; }\n",
"rule makeRelPath { $(<[1]) = [ FRelPath $(<[2-]) : $(>) ] ; }\n",
"rule makeString { $(<) = [ FConcat $(>) ] ; }\n",
"rule makeSubDir { $(<) = [ FSubDir $(>) ] ; }\n",
"rule makeSuffixed { $(<[1]) = [ FAppendSuffix $(>) : $(<[2]) ] ; }\n",
"include $(JAMFILE) ;\n",
0 };
