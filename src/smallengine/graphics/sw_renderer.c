#include <stdint.h>
#include <SDL2/SDL.h>

#include <smallengine/sys/mem.h>
#include <smallengine/graphics/canvas.h>
#include <smallengine/graphics/color.h>

static SDL_Window *screen_window = NULL;
static SDL_Surface *window_surface = NULL;
static SDL_Surface *render_surface = NULL;
static struct canvas screen_canvas;

static int window_width = 0;
static int window_height = 0;

static int res_width = 0;
static int res_height = 0;

/*
 * takes the dimensions of the window on the screen and the resolution
 * the game is to appear to be rendered at and initializes the graphics
 * targets and main display window
 */
int renderer_init(char *title, int win_res_w, int win_res_h,
                                int game_res_w, int game_res_h)
{
       if (SDL_InitSubSystem(SDL_INIT_VIDEO) != 0) {
                SDL_Log("Unable to initialize SDL video: %s", SDL_GetError());
                return 0;
        }

        screen_window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED,
                                        SDL_WINDOWPOS_UNDEFINED,
                                        win_res_w, win_res_h,
                                        SDL_WINDOW_SHOWN);
        if (screen_window == NULL) {
                SDL_Log("Unable to create window: %s", SDL_GetError());
                return 0;
        }

        window_surface = SDL_GetWindowSurface(screen_window);
        if (window_surface == NULL) {
                SDL_Log("Unable to get window surface: %s", SDL_GetError());
                return 0;
        }

        render_surface = SDL_CreateRGBSurface(SDL_SWSURFACE,
                                              game_res_w, game_res_h, 32,
                                              RMASK, GMASK, BMASK, AMASK);
        if (render_surface == NULL) {
                SDL_Log("Unable to create render surface: %s", SDL_GetError());
                return 0;
        }

        screen_canvas = canvas(game_res_w, game_res_h);

        window_width = win_res_w;
        window_height = win_res_h;
        res_width = game_res_w;
        res_height = game_res_h;

        return 1;
}

/*
 * returns the canvas that is written to the main window so it can be
 * manipulated directly
 */
struct canvas renderer_get_window_canvas()
{
        return screen_canvas;
}

/*
 * creates a new canvas with the dimensions of the existing screen
 */
struct canvas renderer_new_canvas()
{
        return canvas(res_width, res_height);
}

/*
 * write the contents of the screen_canvas to the window_surface and update
 * the screen to show the result
 */
void renderer_update_display()
{
        SDL_FillRect(render_surface, NULL, 0);
        int offset = (render_surface->pitch / 4);
        uint32_t *pixels = render_surface->pixels;

        int x, y;
        for (x = 0; x < screen_canvas.w; x++) {
                for (y = 0; y < screen_canvas.h; y++) {
                        pixels[y*offset+x] = color_to_RGBA(
                                             canvas_read_pixel(screen_canvas,
                                                               x, y));
                }
        }

        SDL_BlitScaled(render_surface, NULL, window_surface, NULL);
        SDL_UpdateWindowSurface(screen_window);
}

/*
 * destroy SDL allocated memory and shutdown the video subsystem
 */
void renderer_quit()
{
        SDL_DestroyWindow(screen_window);
        SDL_FreeSurface(render_surface);
        mem_free(screen_canvas.pixels);
        screen_canvas.w = 0;
        screen_canvas.h = 0;

        SDL_VideoQuit();
}

