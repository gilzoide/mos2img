# Maae, a curses Mosaic ASC Art Editor
# Created by Gil Barbosa Reis

Help ("""
Welcome to the mos2img's build script

This package provides Mos2img: a utility that converts Mosaics into images.

You can `scons install` it in the /usr/bin/ directory,
and it can be uninstalled running `scons uninstall`.
""")

if not GetOption ('help'):
    env = Environment (
        LIBS = ['mosaic', 'mosaic_color', 'mosaic_stream_io', 'SDL2', 'SDL2_ttf'],
        LIBPATH = ['/usr/lib', '/usr/local/lib'],
        CCFLAGS = '-Wall -pipe -O2 -g',
        CPPPATH = ['#include', '/usr/include'],
    )
    env.Decider ('MD5-timestamp')

    # if user pass debug=1, add -g flag for the compiler
    #debug = ARGUMENTS.get ('debug', 0)
    #if int (debug):
        #env.Append (CCFLAGS = ' -g')

    # build the editor in the 'build' directory, without duplicating
    VariantDir ('build', 'src', duplicate = 0)
    SConscript ('build/SConscript', exports = 'env')

    ## UNINSTALL ##
    env.Command ("uninstall", None, Delete (FindInstalledFiles()))
