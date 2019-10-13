#ifndef __renderer_h__
#define __renderer_h__

#include <stdint.h>

#include <SDL2/SDL.h>

#include <smallengine/graphics/canvas.h>
#include <smallengine/graphics/color.h>

/*
 * takes the dimensions of the window on the screen and the resolution
 * the game is to appear to be rendered at and initializes the graphics
 * targets and main display window, returns 0 on failure
 */
int renderer_init(char *title, int win_res_w, int win_res_h,
                                int game_res_w, int game_res_h);

/*
 * returns the canvas that is written to the main window so it can be
 * manipulated directly
 */
struct canvas renderer_get_window_canvas();

/*
 * creates a new canvas with the dimensions of the existing screen
 */
struct canvas renderer_new_canvas();

/*
 * write the contents of the screen_canvas to the window_surface and update
 * the screen to show the result
 */
void renderer_update_display();

/*
 * destroy SDL allocated memory and shutdown the video subsystem
 */
void renderer_quit();

#endif // __renderer_h__
