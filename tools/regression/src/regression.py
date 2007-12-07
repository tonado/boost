#!/usr/bin/python
# -*- coding: utf-8 -*-

# Copyright MetaCommunications, Inc. 2003-2007
# Copyright Redshift Software, Inc. 2007
#
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at
# http://www.boost.org/LICENSE_1_0.txt)

import glob
import optparse
import os
import os.path
import platform
import sys
import time
from failure_markup import FailureMarkup

#~ Place holder for xsl_reports/util module
utils = None

repo_root = {
    'anon'          : 'http://svn.boost.org/svn/boost/',
    'user'          : 'https://svn.boost.org/svn/boost/'
    }
repo_path = {
    'trunk'         : 'trunk',
    'release'       : 'branches/release',
    'build'         : 'trunk/tools/build/v2',
    'jam'           : 'tags/tools/jam/Boost_Jam_3_1_15/src',
    'regression'    : 'trunk/tools/regression'
    }

class runner:
    
    def __init__(self,root):
        commands = map(
            lambda m: m[8:].replace('_','-'),
            filter(
                lambda m: m.startswith('command_'),
                runner.__dict__.keys())
            )
        commands.sort()
        commands = "commands: %s" % ', '.join(commands)
        
        opt = optparse.OptionParser(
            usage="%prog [options] [commands]",
            description=commands)
            
        #~ Base Options:
        opt.add_option( '--runner',
            help="runner ID (e.g. 'Metacomm')" )
        opt.add_option( '--comment',
            help="an HTML comment file to be inserted in the reports" )
        opt.add_option( '--tag',
            help="the tag for the results" )
        opt.add_option( '--toolsets',
            help="comma-separated list of toolsets to test with" )
        opt.add_option( '--incremental',
            help="do incremental run (do not remove previous binaries)",
            action='store_true' )
        opt.add_option( '--timeout',
            help="specifies the timeout, in minutes, for a single test run/compilation",
            type='int' )
        opt.add_option( '--library',
            help="the library subdirectory in which to run tests")
        opt.add_option( '--bjam-options',
            help="options to pass to the regression test" )
        opt.add_option( '--bjam-toolset',
            help="bootstrap toolset for 'bjam' executable" )
        opt.add_option( '--platform' )

        opt.add_option( '--reflect-test-status',
            help="if a test fails, exit with a nonzero status",
            action='store_true' )
        
        #~ Source Options:
        opt.add_option( '--user',
            help="Boost SVN user ID" )
        opt.add_option( '--revision',
            help="Boost SVN revision ID", type='int' )
        opt.add_option( '--local',
            help="the name of the boost tarball" )
        opt.add_option( '--force-update',
            help="do an SVN update (if applicable) instead of a clean checkout, even when performing a full run",
            action='store_true' )
        opt.add_option( '--have-source',
            help="do neither a tarball download nor an SVN update; used primarily for testing script changes",
            action='store_true' )

        #~ Connection Options:
        opt.add_option( '--proxy',
            help="HTTP proxy server address and port (e.g.'http://www.someproxy.com:3128')" )
        opt.add_option( '--ftp-proxy',
            help="FTP proxy server (e.g. 'ftpproxy')" )
        opt.add_option( '--dart-server',
            help="the dart server to send results to" )
        opt.add_option( '--bitten-report',
            help="Name of bitten XML report file to generate")

        #~ Debug Options:
        opt.add_option( '--debug-level',
            help="debugging level; controls the amount of debugging output printed",
            type='int' )
        opt.add_option( '--mail',
            help="email address to send run notification to" )
        opt.add_option( '--smtp-login',
            help="STMP server address/login information, in the following form: <user>:<password>@<host>[:<port>]" )
        opt.add_option( '--skip-tests',
            help="do not run bjam; used for testing script changes" )
        
        #~ Defaults
        self.runner = None
        self.comment='comment.html'
        self.tag='trunk'
        self.toolsets=None
        self.incremental=False
        self.timeout=5
        self.bjam_options=''
        self.bjam_toolset=''
        self.platform=self.platform_name()
        self.user='anonymous'
        self.local=None
        self.force_update=False
        self.have_source=False
        self.proxy=None
        self.ftp_proxy=None
        self.dart_server=None
        self.bitten_report=None
        self.debug_level=-1
        self.library=None
        self.mail=None
        self.smtp_login=None
        self.skip_tests=False
        self.exit_status=0
        self.reflect_test_status=False
        ( _opt_, self.actions ) = opt.parse_args(None,self)
        if not self.actions or self.actions == []:
            self.actions = [ 'regression' ]
        
        #~ Initialize option dependent values.
        self.regression_root = root
        self.boost_root = os.path.join( self.regression_root, 'boost' )
        self.regression_results = os.path.join( self.regression_root, 'results')
        markup_path = os.path.join(
            self.boost_root, 'status', 'explicit-failures-markup.xml')
        self.failure_markup = FailureMarkup(open(markup_path))
        
        self.tools_root = os.path.join( self.regression_root,'tools' )
        self.tools_bb_root = os.path.join( self.tools_root,'build', 'v2' )
        self.tools_bjam_root = os.path.join( self.tools_root,'jam', 'src' )
        self.tools_regression_root = os.path.join( self.tools_root,'regression' )
        self.xsl_reports_dir = os.path.join( self.tools_regression_root, 'xsl_reports' )
        self.timestamp_path = os.path.join( self.regression_root, 'timestamp' )
        if sys.platform == 'win32':
            self.patch_boost = 'patch_boost.bat'
            self.bjam = { 'name' : 'bjam.exe' }
        else:
            self.patch_boost = 'patch_boost'
            self.bjam = { 'name' : 'bjam' }
        self.bjam = {
            'name' : self.bjam['name'],
            'build_cmd' : self.bjam_build_cmd,
            'path' : os.path.join(self.regression_root,self.bjam['name']),
            'source_dir' : self.tools_bjam_root,
            'build_dir' : self.tools_bjam_root,
            'build_args' : ''
            }

        if self.library:
            out_xml = os.path.join( *(self.library + '-x.xml').split('/') )
            self.library_markup = self.failure_markup.libraries.get(self.library)
            
            # The assumption is that most libraries' markup is not separated
            # specifically by subdirectory (e.g. "libs/foo/test",
            # "libs/foo/example",...).  So if markup wasn't found we might try
            # again.
            libdir,subdir = os.path.split(self.library)
            if subdir == 'test' and not self.library_markup:
                self.library_markup = self.failure_markup.libraries.get(libdir)
        else:
            out_xml = 'test_results.xml'
            self.library_markup = None
            
        self.out_xml = os.path.join(self.regression_results,out_xml)
            
        self.boost_build_use_xml = ('create-bitten-report' in self.actions and
                                    'test-process' not in self.actions)

        self.dev_null = {'nt':'nul','posix':'/dev/null'}[os.name]
        
        if self.debug_level > 0:
            self.log('Regression root =     %s'%self.regression_root)
            self.log('Boost root =          %s'%self.boost_root)
            self.log('Regression results =  %s'%self.regression_results)
            self.log('BB root =             %s'%self.tools_bb_root)
            self.log('Bjam root =           %s'%self.tools_bjam_root)
            self.log('Tools root =          %s'%self.tools_regression_root)
            self.log('XSL reports dir =     %s'%self.xsl_reports_dir)
            self.log('Timestamp =           %s'%self.timestamp_path)
            self.log('Patch Boost script =  %s'%self.patch_boost)
        
        self.main()
    
    #~ The various commands that make up the testing sequence...
    
    def command_cleanup(self,*args):
        if not args or args == None or args == []: args = [ 'source', 'bin' ]

        if 'source' in args:
            self.log( 'Cleaning up "%s" directory ...' % self.boost_root )
            self.rmtree( self.boost_root )

        if 'bin' in args:
            boost_bin_dir = os.path.join( self.boost_root, 'bin' )
            self.log( 'Cleaning up "%s" directory ...' % boost_bin_dir )
            self.rmtree( boost_bin_dir )

            boost_binv2_dir = os.path.join( self.boost_root, 'bin.v2' )
            self.log( 'Cleaning up "%s" directory ...' % boost_binv2_dir )
            self.rmtree( boost_binv2_dir )

            self.log( 'Cleaning up "%s" directory ...' % self.regression_results )
            self.rmtree( self.regression_results )

    def _get_tool(self, tool_name, dest_dir):
        repo_key = tool_name.split('/')[0]
        local_path = os.path.join( self.regression_root, 'tools', tool_name )
        if os.path.exists( local_path ):
            import shutil
            shutil.rmtree( dest_dir, ignore_errors=True )
            shutil.copytree( local_path, dest_dir )
        elif self.user and self.user != '':
            os.chdir( os.path.dirname(dest_dir) )
            self.svn_command( 'co %s %s' % (
                self.svn_repository_url(repo_path[repo_key]),
                os.path.basename(dest_dir) ) )
        else:
            self.retry( lambda: self.download_tarball(
                os.path.basename(dest_dir)+".tar.bz2",
                self.tarball_url(repo_path[repo_key]) ) )
            self.unpack_tarball(
                dest_dir+".tar.bz2",
                os.path.basename(dest_dir) )
            
    def command_get_tools(self):
        self.log( 'Getting Boost.Build v2...' )
        self._get_tool('build', self.tools_bb_root)

        self.log( 'Getting Boost.Jam...' )
        self._get_tool('jam/src', self.tools_bjam_root)
        
        self.log( 'Getting regression tools and utilities...' )
        self._get_tool('regression', self.tools_regression_root)
    
    def command_get_source(self):
        self.refresh_timestamp()
        self.log( 'Getting sources (%s)...' % self.timestamp() )

        if self.user and self.user != '':
            self.retry( self.svn_checkout )
        else:
            self.retry( self.get_tarball )
        pass
    
    def command_update_source(self):
        if self.user and self.user != '' \
            or os.path.exists( os.path.join( self.boost_root, '.svn' ) ):
            open( self.timestamp_path, 'w' ).close()
            self.log( 'Updating sources from SVN (%s)...' % self.timestamp() )
            self.retry( self.svn_update )
        else:
            self.command_get_source( )
        pass
    
    def command_patch(self):
        self.import_utils()
        patch_boost_path = os.path.join( self.regression_root, self.patch_boost )
        if os.path.exists( patch_boost_path ):
            self.log( 'Found patch file "%s". Executing it.' % patch_boost_path )
            os.chdir( self.regression_root )
            self._system( [ patch_boost_path ] )
        pass
    
    def command_setup(self):
        self.command_patch()
        self.build_if_needed(self.bjam,self.bjam_toolset)
    
    def command_test(self, *args):
        if not args: args = [ "test" ]
        self.import_utils()

        self.log( 'Making "%s" directory...' % self.regression_results )
        utils.makedirs( self.regression_results )

        results_libs = os.path.join( self.regression_results, 'libs' )
        results_status = os.path.join( self.regression_results, 'status' )

        if "clean" in args:
            self.command_test_clean()

        if "test" in args:
            self.command_test_run()

    def command_test_clean(self):
        results_libs = os.path.join( self.regression_results, 'libs' )
        results_status = os.path.join( self.regression_results, 'status' )
        self.rmtree( results_libs )
        self.rmtree( results_status )

    def toolsets_to_test(self):
        """filter out any toolsets marked unusable"""
        toolsets = set(self.toolsets.split(','))
        if self.library_markup is not None:
            unusable_toolsets = set()
            unusability_reasons = set()
            
            # Check for usability
            for t in toolsets:
                markup = self.library_markup.unusable(t)
                if markup:
                    unusable_toolsets.add(t)
                    unusability_reasons |= set(markup)

            if unusable_toolsets:
                try:
                    import xml.etree.cElementTree as ET
                except ImportError:
                    import elementtree.cElementTree as ET
                print 'skipping build with toolsets:', ', '.join(unusable_toolsets)
                print '\n'.join( [ ET.tostring(x) for x in unusability_reasons ] )
                toolsets -= unusable_toolsets
        return toolsets
        
    def command_test_run(self):
        toolsets = self.toolsets_to_test()
        build_target = '' # build all the tests
        
        if not toolsets:
            # Nothing left to test for this library; just build the output XML
            # file.
            build_target = self.out_xml
            
        self.import_utils()
        
        utils.makedirs( os.path.split(self.out_xml)[0] )

        test_cmd = '%s %s %s --out-xml=%s "--build-dir=%s" > "%s" 2>&1' % (
            self.bjam_cmd( toolsets ),
            self.bjam_options,
            build_target,
            self.out_xml,
            self.regression_results,
            self.dev_null
            )
        
        self.log( 'Starting tests (%s)...' % test_cmd )
        cd = os.getcwd()
        if self.library:
            os.chdir( os.path.join( self.boost_root, 'libs', self.library ) )
        else:
            os.chdir( os.path.join( self.boost_root, 'status' ) )

        try:
            self.log( '...in (%s).' % os.getcwd() )
        
            exit_status = self._system( [ test_cmd ] )
            self.exit_status = self.exit_status or exit_status
            if exit_status:
                self.log('test run exited with status %s.' % exit_status)
        finally:
            os.chdir( cd )

    def command_create_bitten_report(self):
        self.import_utils()
        from bitten_reports import create_bitten_reports
        
        markup_path = os.path.join(
            self.boost_root, 'status', 'explicit-failures-markup.xml')
        
        markup = FailureMarkup(open(markup_path))
        failed, report = create_bitten_reports(
            self.out_xml, markup)

        self.exit_status = failed and 1 or 0
        
        dir = os.path.split(self.bitten_report)[0]
        if not os.path.exists(dir):
            os.makedirs(dir)
            
        open(self.bitten_report, 'w').write(
            '\n'.join(('<?xml version="1.0" encoding="UTF-8"?>', report)))
    
    def command_upload_logs(self):
        self.import_utils()
        from collect_and_upload_logs import upload_logs
        self.retry(
            lambda:
                upload_logs(
                    self.regression_results,
                    self.runner, self.tag,
                    self.user,
                    self.ftp_proxy,
                    self.debug_level, self.send_bjam_log,
                    self.timestamp_path,
                    self.dart_server )
            )
    
    def command_regression(self):
        import socket
        import string
        try:
            mail_subject = 'Boost regression for %s on %s' % ( self.tag,
                string.split(socket.gethostname(), '.')[0] )
            start_time = time.localtime()
            if self.mail:
                self.log( 'Sending start notification to "%s"' % self.mail )
                self.send_mail(
                    '%s started at %s.' % ( mail_subject, format_time( start_time ) )
                    )
            
            self.command_get_tools()

            if self.local is not None:
                self.log( 'Using local file "%s"' % self.local )
                b = os.path.basename( self.local )
                tag = b[ 0: b.find( '.' ) ]
                self.log( 'Tag: "%s"' % tag  )
                self.unpack_tarball( local, self.boost_root )
                
            elif self.have_source:
                if not self.incremental: self.command_cleanup( [ 'bin' ] )
                
            else:
                if self.force_update:
                    self.command_cleanup( [ 'bin' ] )
                elif not self.incremental:
                    self.command_cleanup()
                self.command_get_source()

            self.command_setup()

            # Not specifying --toolset in command line is not enough
            # that would mean to use Boost.Build default ones
            # We can skip test only we were explictly 
            # told to have no toolsets in command line "--toolset="
            if self.toolsets != '': # --toolset=,
                if not self.skip_tests:
                    self.command_test()
                self.command_collect_logs()
                self.command_upload_logs()

            if self.mail:
                self.log( 'Sending report to "%s"' % self.mail )
                end_time = time.localtime()
                self.send_mail(
                    '%s completed successfully at %s.' % ( mail_subject, format_time( end_time ) )
                    )
        except:
            if self.mail:
                self.log( 'Sending report to "%s"' % self.mail )
                traceback_ = '\n'.join( apply( traceback.format_exception, sys.exc_info() ) )
                end_time = time.localtime()
                self.send_mail(
                    '%s failed at %s.' % ( mail_subject, format_time( end_time ) ),
                    traceback_ )
            raise

    def command_show_revision(self):
        modified = '$Date$'
        revision = '$Revision$'

        import re
        re_keyword_value = re.compile( r'^\$\w+:\s+(.*)\s+\$$' )
        print '\n\tRevision: %s' % re_keyword_value.match( revision ).group( 1 )
        print '\tLast modified on: %s\n' % re_keyword_value.match( modified ).group( 1 )
    
    #~ Utilities...
    
    def main(self):
        for action in self.actions:
            action_m = "command_"+action.replace('-','_')
            if hasattr(self,action_m):
                getattr(self,action_m)()
                
        if self.exit_status != 0:
            if self.reflect_test_status:
                self.log('exiting with status: %s' % self.exit_status)
                sys.exit(self.exit_status)
            else:
                self.log(
                    '*** warning: tests failed with exit status: %s, but exiting cleanly' % self.exit_status)

    def platform_name(self):
        # See http://article.gmane.org/gmane.comp.lib.boost.testing/933
        if sys.platform == 'win32':
            return 'Windows'
        elif sys.platform == 'cygwin':
            return 'Windows/Cygwin'
        return platform.system()

    def log(self,message):
        if self.debug_level >= 0:
            sys.stdout.flush()
            sys.stderr.flush()
            sys.stderr.write( '# %s\n' % message )
            sys.stderr.flush()

    def rmtree(self,path):
        if os.path.exists( path ):
            import shutil
            #~ shutil.rmtree( unicode( path ) )
            if sys.platform == 'win32':
                os.system( 'del /f /s /q "%s" >nul 2>&1' % path )
                shutil.rmtree( unicode( path ) )
            else:
                os.system( 'rm -f -r "%s"' % path )

    def refresh_timestamp( self ):
        if os.path.exists( self.timestamp_path ):
           os.unlink( self.timestamp_path )
        open( self.timestamp_path, 'w' ).close()

    def timestamp( self ):
        return time.strftime(
            '%Y-%m-%dT%H:%M:%SZ',
            time.gmtime( os.stat( self.timestamp_path ).st_mtime ) )

    def retry( self, f, max_attempts=5, sleep_secs=10 ):
        for attempts in range( max_attempts, -1, -1 ):
            try:
                return f()
            except Exception, msg:
                self.log( '%s failed with message "%s"' % ( f.__name__, msg ) )
                if attempts == 0:
                    self.log( 'Giving up.' )
                    raise

                self.log( 'Retrying (%d more attempts).' % attempts )
                time.sleep( sleep_secs )

    def http_get( self, source_url, destination_file ):
        import urllib
        
        proxies = None
        if hasattr(self,'proxy') and self.proxy is not None:
            proxies = { 'http' : self.proxy }

        src = urllib.urlopen( source_url, proxies = proxies )

        f = open( destination_file, 'wb' )
        while True:
            data = src.read( 16*1024 )
            if len( data ) == 0: break
            f.write( data )

        f.close()
        src.close()

    def import_utils(self):
        global utils
        if utils is None:
            sys.path.append( self.xsl_reports_dir )
            import utils as utils_module
            utils = utils_module

    def build_if_needed( self, tool, toolset ):
        self.import_utils()
        if os.path.exists( tool[ 'path' ] ):
            self.log( 'Found preinstalled "%s"; will use it.' % tool[ 'path' ] )
            return

        self.log( 'Preinstalled "%s" is not found; building one...' % tool[ 'path' ] )

        if toolset is None:
            if self.toolsets is not None:
                toolset = self.toolsets.split(',')[0]
            else:
                toolset = tool[ 'default_toolset' ]
                self.log( 'Warning: No bootstrap toolset for "%s" was specified.' % tool[ 'name' ] )
                self.log( '         Using default toolset for the platform (%s).' % toolset )

        if os.path.exists( tool[ 'source_dir' ] ):
            self.log( 'Found "%s" source directory "%s"' % ( tool[ 'name' ], tool[ 'source_dir' ] ) )
            build_cmd = tool[ 'build_cmd' ]( toolset, tool['build_args'] )
            self.log( 'Building "%s" (%s)...' % ( tool[ 'name'], build_cmd ) )
            self._system( [ 'cd "%s"' % tool[ 'source_dir' ], build_cmd ] )
        else:
            raise 'Could not find "%s" source directory "%s"' % ( tool[ 'name' ], tool[ 'source_dir' ] )

        if not tool.has_key( 'build_path' ):
            tool[ 'build_path' ] = self.tool_path( tool )

        if not os.path.exists( tool[ 'build_path' ] ):
            raise 'Failed to find "%s" after build.' % tool[ 'build_path' ]

        self.log( '%s succesfully built in "%s" location' % ( tool[ 'name' ], tool[ 'build_path' ] ) )

    def tool_path( self, name_or_spec ):
        if isinstance( name_or_spec, basestring ):
            return os.path.join( self.regression_root, name_or_spec )

        if os.path.exists( name_or_spec[ 'path' ] ):
            return name_or_spec[ 'path' ]

        if name_or_spec.has_key( 'build_path' ):
            return name_or_spec[ 'build_path' ]

        build_dir = name_or_spec[ 'build_dir' ]
        self.log( 'Searching for "%s" in "%s"...' % ( name_or_spec[ 'name' ], build_dir ) )
        for root, dirs, files in os.walk( build_dir ):
            if name_or_spec[ 'name' ] in files:
                return os.path.join( root, name_or_spec[ 'name' ] )

        raise Exception( 'Cannot find "%s" in any of the following locations:\n%s' % (
              name_or_spec[ 'name' ]
            , '\n'.join( [ name_or_spec[ 'path' ], build_dir ] )
            ) )

    def _system( self, cmds ):
        self.log('running system commands in %s: %s'  % (os.getcwd(), cmds))
        return utils.system( cmds )
        
    def _checked_system( self, cmds ):
        self.log('running checked system commands in %s: %s'  % (os.getcwd(), cmds))
        return utils.checked_system( cmds )
        
    def bjam_build_cmd( self, *rest ):
        if sys.platform == 'win32':
            cmd = 'build.bat %s' % self.bjam_toolset
        else:
            cmd = './build.sh %s' % self.bjam_toolset
            
        if self.incremental: # Don't force a rebuild of bjam
            cmd += ' --update'
            
        env_setup_key = 'BJAM_ENVIRONMENT_SETUP'
        if os.environ.has_key( env_setup_key ):
            return '%s & %s' % ( os.environ[env_setup_key], cmd )
        return cmd
    
    def bjam_cmd( self, toolsets, args = '', *rest ):
        build_path = self.regression_root
        if build_path[-1] == '\\': build_path += '\\'
        
        if self.timeout > 0:
            args += ' -l%s' % (self.timeout*60)

        cmd = '"%(bjam)s" "--boost-build=%(bb)s" "-sBOOST_ROOT=%(boost)s" "--boost=%(boost)s" %(arg)s' % {
            'bjam' : self.tool_path( self.bjam ),
            'bb' : self.tools_bb_root,
            'boost' : self.boost_root,
            'arg' : args }

        if toolsets:
            import string
            cmd += ' ' + string.join(toolsets, ' ' )

        return cmd

    def send_mail( self, subject, msg = '' ):
        import smtplib
        if not self.smtp_login:
            server_name = 'mail.%s' % mail.split( '@' )[-1]
            user_name = None
            password = None
        else:
            server_name = self.smtp_login.split( '@' )[-1]
            ( user_name, password ) = string.split( self.smtp_login.split( '@' )[0], ':' )

        self.log( '    Sending mail through "%s"...' % server_name )
        smtp_server = smtplib.SMTP( server_name )
        smtp_server.set_debuglevel( self.debug_level )
        if user_name:
            smtp_server.login( user_name, password )

        smtp_server.sendmail( self.mail, [ self.mail ],
            'Subject: %s\nTo: %s\n\n%s' % ( subject, self.mail, msg ) )

    #~ Dowloading source, from SVN...

    def svn_checkout( self ):
        os.chdir( self.regression_root )
        self.svn_command( 'co %s %s' % (self.svn_repository_url(self.tag),'boost') )

    def svn_update( self ):
        os.chdir( self.boost_root )
        self.svn_command( 'update' )

    def svn_command( self, command ):
        svn_anonymous_command_line  = 'svn --non-interactive %(command)s'
        svn_command_line            = 'svn --non-interactive --username=%(user)s %(command)s'
        
        if not hasattr(self,'user') or self.user is None or self.user == 'anonymous':
            cmd = svn_anonymous_command_line % { 'command': command }
        else:
            cmd = svn_command_line % { 'user': self.user, 'command': command }

        if self.revision:
            cmd += ' -r%s' % self.revision
            
        self.log( 'Executing SVN command "%s"' % cmd )
        rc = os.system( cmd )
        if rc != 0:
            raise Exception( 'SVN command "%s" failed with code %d' % ( cmd, rc ) )

    def svn_repository_url( self, path ):
        if hasattr(self,'user') and self.user is not None and self.user != 'anonymous':
            return '%s%s' % (repo_root['user'],path)
        else:
            return '%s%s' % (repo_root['anon'],path)
    
    #~ Downloading and extracting source archives, from tarballs or zipballs...
    
    def get_tarball( self, *args ):
        if not args or args == []:
            args = [ 'download', 'unpack' ]

        tarball_path = None

        if hasattr(self,'local') and self.local is not None:
            tarball_path = self.local
        elif 'download' in args:
            tarball_path = self.download_tarball(self.boost_tarball_name(),self.boost_tarball_url())
        if not tarball_path:
            tarball_path = os.path.join( self.regression_root, self.boost_tarball_url() )

        if 'unpack' in args:
            self.unpack_tarball( tarball_path, self.boost_root )
        pass

    def download_tarball( self, tarball_name, tarball_url ):
        tarball_path = os.path.join( self.regression_root, tarball_name )

        self.log( 'Downloading "%s" to "%s"...'  % ( tarball_url, os.path.dirname( tarball_path ) ) )

        if os.path.exists( tarball_path ):
            os.unlink( tarball_path )
        self.http_get( tarball_url, tarball_path )

        return tarball_path
    
    def tarball_url( self, path ):
        return 'http://beta.boost.org/development/snapshot.php/%s' % path

    def boost_tarball_name( self ):
        return 'boost-%s.tar.bz2' % self.tag.split( '/' )[-1]

    def boost_tarball_url( self ):
        return self.tarball_url( self.tag )

    def unpack_tarball( self, tarball_path, target_path ):
        self.log( 'Looking for old unpacked archives...' )
        old_boost_dirs = self.find_boost_dirs( )

        for old_boost_dir in old_boost_dirs:
            if old_boost_dir != tarball_path:
                self.log( 'Deleting old directory %s.' % old_boost_dir )
                self.rmtree( old_boost_dir )

        self.log( 'Unpacking boost tarball ("%s")...' % tarball_path )

        tarball_name = os.path.basename( tarball_path )
        extension = tarball_name[ tarball_name.find( '.' ) : ]

        if extension in ( ".tar.gz", ".tar.bz2" ):
            import tarfile
            import stat
            
            mode = os.path.splitext( extension )[1][1:]
            tar = tarfile.open( tarball_path, 'r:%s' % mode )
            for tarinfo in tar:
                tar.extract( tarinfo, self.regression_root )
                if sys.platform == 'win32' and not tarinfo.isdir():
                    # workaround what appears to be a Win32-specific bug in 'tarfile'
                    # (modification times for extracted files are not set properly)
                    f = os.path.join( self.regression_root, tarinfo.name )
                    os.chmod( f, stat.S_IWRITE )
                    os.utime( f, ( tarinfo.mtime, tarinfo.mtime ) )
            tar.close()
        elif extension in ( ".zip" ):
            import zipfile

            z = zipfile.ZipFile( tarball_path, 'r', zipfile.ZIP_DEFLATED )
            for f in z.infolist():
                destination_file_path = os.path.join( self.regression_root, f.filename )
                if destination_file_path[-1] == "/": # directory
                    if not os.path.exists( destination_file_path  ):
                        os.makedirs( destination_file_path  )
                else: # file
                    result = open( destination_file_path, 'wb' )
                    result.write( z.read( f.filename ) )
                    result.close()
            z.close()
        else:
            raise 'Do not know how to unpack archives with extension \"%s\"' % extension

        boost_dir = self.find_boost_dirs()[0]
        self.log( '    Unpacked into directory "%s"' % boost_dir )

        if os.path.exists( target_path ):
            self.log( 'Deleting "%s" directory...' % target_path )
            self.rmtree( target_path )

        self.log( 'Renaming "%s" into "%s"' % ( boost_dir, target_path ) )
        os.rename( boost_dir, target_path )

    def find_boost_dirs( self ):
        return [
            x for x in
                glob.glob( os.path.join( self.regression_root, 'boost[-_]*' ) )
                if os.path.isdir( x )
            ]


