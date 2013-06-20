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

def build(bld):
    bld.program(
        source       = bld.path.ant_glob('src/**/*.cpp'),
        target       = 'livetuiles',
        use			 = ['LLVM', 'OS'],
        includes     = ['/usr/include','/usr/local/include','../libTuiles2/'],
        stlib        = ['Tuiles', 'faust', 'm', 'jack',
                        'sndfile', 'samplerate', 'fltk', 'xml2', 'fltk_gl'],
        stlibpath    = ['../build/', '/usr/local/lib/faust', 
                        '../../libTuiles2/build'],
        vnum         = '0.0.1',
        install_path = '${PREFIX}/bin',
        cxxflags     = ['-O3','-Wall'],
        mac_app      = True,
        mac_plist     = 'Mac/Info.plist',
        mac_resources = 'Mac/livetuiles.icns'
    )



