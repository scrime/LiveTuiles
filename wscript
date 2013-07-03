#!/usr/bin/env python

import sys, os

def options(opt):
        opt.load('compiler_cxx')

def configure(conf):
        #generic configuration
        conf.load('compiler_cxx')
        conf.check_cfg(path='llvm-config', args='--cflags --libs --ldflags',
                        package='', uselib_store='LLVM')
        conf.env.STLIB_COMMON    =['faust', 'm', 'jack', 'sndfile',
                                    'samplerate', 'fltk', 'xml2', 'fltk_gl']
        conf.env.STLIBPATH_COMMON = ['/usr/local/lib/faust']
        conf.env.INCLUDES =['/usr/include','/usr/local/include', 
                            '../libTuiles/']

        #platform specific
        if sys.platform == 'darwin':
            conf.env.FRAMEWORK_OS = ['Cocoa','OpenGL','AGL']

        #release specific
        conf.env.STLIBPATH = [os.path.join(os.getcwd(),'../libTuiles/build')]
        conf.env.STLIB    = ['Tuiles']
        conf.env.CXXFLAGS = ['-O3', '-Wall'] 
        conf.env.DEFINES  = ['DEBUG(x)=//x']

        #debug specific
        conf.setenv('debug', env=conf.env.derive())
        conf.env.CXXFLAGS = ['-g', '-Wall']
        conf.env.DEFINES  = ['DEBUG(x)=std::cout<< x <<std::endl;']
        conf.env.STLIB    = ['Tuilesdebug']
        conf.env.STLIBPATH = [os.path.join(os.getcwd(),'../libTuiles/build/debug')]



def build(bld):
    bld.program(
        source       = bld.path.ant_glob('src/**/*.cpp'),
        target       = 'livetuiles'+bld.variant,
        use			 = ['LLVM', 'OS', 'COMMON'],
        vnum         = '0.0.1',
        install_path = '${PREFIX}/bin',
        mac_app      = True,
        mac_plist     = 'Mac/Info.plist',
        mac_resources = 'Mac/livetuiles.icns'
    )


from waflib.Build import BuildContext, CleanContext
class debug(BuildContext): 
        cmd = 'debug'
        variant = 'debug' 

