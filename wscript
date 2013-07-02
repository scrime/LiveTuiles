#!/usr/bin/env python

import sys

def options(opt):
        opt.load('compiler_cxx')

def configure(conf):
        conf.load('compiler_cxx')
        if sys.platform == 'darwin':
            conf.env.FRAMEWORK_OS = ['Cocoa','OpenGL','AGL']
        conf.check_cfg(path='llvm-config', args='--cflags --libs --ldflags',
                        package='', uselib_store='LLVM')
        conf.env.STLIB_OS=['Tuiles', 'faust', 'm', 'jack',
                            'sndfile', 'samplerate', 'fltk', 'xml2', 'fltk_gl']
        conf.env.INCLUDES_OS=['/usr/include','/usr/local/include', 
                                '../libTuiles/']
        conf.env.STLIBPATH_OS= ['../build/', '/usr/local/lib/faust', 
                                '../../libTuiles/build']

def build(bld):
    #RELEASE
    bld.program(
        source       = bld.path.ant_glob('src/**/*.cpp'),
        target       = 'livetuiles',
        use			 = ['LLVM', 'OS'],
        vnum         = '0.0.1',
        install_path = '${PREFIX}/bin',
        cxxflags     = ['-O3','-Wall', '-DDEBUG(x)'],
        mac_app      = True,
        mac_plist     = 'Mac/Info.plist',
        mac_resources = 'Mac/livetuiles.icns'
    )
    #DEBUG
    bld.program(
        source       = bld.path.ant_glob('src/**/*.cpp'),
        target       = 'livetuiles-debug',
        use			 = ['LLVM', 'OS'],
        cxxflags     = ['-g', '-DDEBUG(x) cout<< x <<endl;'],
    )



