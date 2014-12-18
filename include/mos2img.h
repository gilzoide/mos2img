/** @file mos2img.h
 */
#ifndef MOS2IMG_H
#define MOS2IMG_H

#include <mosaic/mosaic.h>
#include <mosaic/stream_io.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include <stdio.h>
#include <stdlib.h>

/*   SDL stuff   */

/// Inits SDL stuff
int InitSDL ();
/// Quit SDL, to be executed at exit
void QuitSDL ();

#endif
