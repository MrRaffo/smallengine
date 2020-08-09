#ifndef __SE_GRAPHICS_H__
#define __SE_GRAPHICS_H__

#include <SDL2/SDL.h>

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
        #define RMASK 0xff000000
        #define GMASK 0x00ff0000
        #define BMASK 0x0000ff00
        #define AMASK 0x000000ff

        #define RSHIFT 24
        #define GSHIFT 16
        #define BSHIFT 8
        #define ASHIFT 0
#else
        #define RMASK 0x000000ff
        #define GMASK 0x0000ff00
        #define BMASK 0x00ff0000
        #define AMASK 0xff000000

        #define RSHIFT 0
        #define GSHIFT 8
        #define BSHIFT 16
        #define ASHIFT 24
#endif      // SDL_BYTEORDER

struct color {
        double r;
        double g;
        double b;
        double a;
};

struct palette {
        struct color **colors;
        int size;               // total number of colors that can be stored
        int assigned;           // number of colors in the palette
};

struct canvas {
        int w;
        int h;
        struct color *pixels;
};

enum blit_mode {
        BLIT_ABS,
        BLIT_ADD,
        BLIT_MUL,
        NUM_BLIT_MODES
};

struct texture {
        int w;
        int h;
        struct canvas canvas;   // direct color data
        int *mask;              // indices of associated palette
        struct palette palette; // colors to match indices of the mask
};

/**********************
 * COLOUR
 *********************/

/*
 * Creation and Initialization
 */

/* create a new color with given components */
struct color color_rgb(const double r, const double g, const double b);

/* create a color with given alpha, r, g and b components will hold the value
 * of the given values multiplied by the alpha, the alpha will be store to
 * allow the original values to be calculated */
struct color color_rgba(const double r, const double g, const double b, 
        const double a);

/* create a color using integer values instead of doubles */
struct color color_rgb_int(const int r, const int g, const int b);

/* int components with alpha */
struct color color_rgba_int(const int r, const int g, const int b, 
        const int a);

/*
 * Comparison
 */

/* returns 1 if the two colours given are the same, 0 otherwise */
int color_equal(const struct color c1, const struct color c2);

/*
 * Return Values
 */

/* return a new color with the r g and b values restricted to the 
 *range 0.0-1.0 */
const struct color color_cap(const struct color c);

/* return a ppm style string of the color (ie "255 0 128")*/
char *color_to_ppm_string(const struct color c);

/* return the color as a 32 bit integer ARGB (bitmap) format, alpha is
 * always 0xff */
const uint32_t color_to_ARGB(struct color c);

/* 
 * return the color as a 32 bit integer in RGBA format, alpha is always 0xff
 */
const uint32_t color_to_RGBA(struct color c);

/*
 * Operations
 */

/* add 2 colors component by component and return the new color */
const struct color color_add(const struct color c1, const struct color c2);

/* subtract the corresponding component values of c2 from c1 and return the new
 * color */
const struct color color_subtract(const struct color c1, const struct color c2);

/* multiply all components in a colour by the given factor */
const struct color color_scale(const struct color c, const double factor);

/* multiply the components of the colours together and return the resulting
 * new colour, also known as the Hadamard Product or Schur Product */
const struct color color_multiply(const struct color c1, const struct color c2);

/***********************
 * PALETTE
 **********************/

/*
 * Palette Creation
 */

/*
 * create a new palette with no colors and of the size given
 */
struct palette palette(int size);

/*
 * create a palette from a canvas. the canvas will be checked from left to
 * right, row by row, each unique color added in the order it is encountered
 */
struct palette palette_from_canvas(const struct canvas can);

/*
 * Palette Destruction
 */

/*
 * free all memory used by a palette
 */
void palette_destroy(struct palette *pal);

/*
 * Palette Operations
 */

/*
 * check if a palette contains a certain color, returns index if so, -1 otherwise
 */
int palette_check_color(struct palette pal, const struct color col);

/*
 * get the color of the specified index, returns (0.0, 0.0, 0.0) if the index
 * is invalid
 */
struct color palette_get_by_index(struct palette pal, int index);

// check if palette contains a range of colors? replace a range of colors?

/*
 * add a color to a palette, returns the new size of the palette, 0 if the 
 * palette cannot accept new colors
 */
int palette_add_color(struct palette *p, const struct color col);

/*
 * replace a color in a palette with a new one, return 1 on success, 0 on fail
 */
int palette_replace_color(struct palette p, const struct color oldc, 
                                             const struct color newc);

/*
 * replace a color at a given index regardless of it's value, returns 1 on
 * success, 0 on failure
 */
int palette_replace_index(struct palette p, int index, const struct color c);



/*********************
 * CANVAS
 ********************/

/*
 * the canvas that all graphics are drawn on, will be used to store graphics
 * assets at runtime as well as constructing the image of each frame of the
 * game, which can be transferred to the main rendering surface (such as the
 * on provided by SDL) to be displayed. Can also output to BMP
 */

/*
 * Creation and Initialization
 */

/*
 * Create a new canvas, every colour will be initialised to (0, 0, 0)
 */
struct canvas canvas(const int w, const int h);

/*
 * Operations
 */

/*
 * Read the value of a given pixel, the color (0, 0, 0) will be returned if a
 * pixel coordinate outside the canvas area was requested
 */
struct color canvas_read_pixel(struct canvas canvas, const int x, const int y);

/*
 * Write the given color value to the coordinate on the canvas, returns 1 if
 * successful, 0 otherwise, writes according to the specified blit mode
 */
const int canvas_write_pixel(struct canvas can, int x, int y, struct color col,
                             enum blit_mode mode);


/*
 * fill a canvas with alternating colored squares of size tile_size
 */
void canvas_pattern(struct canvas src, struct color col1, 
                    struct color col2, int tile_size);

/*
 * Fill the canvas with the color given
 */
void canvas_fill(struct canvas canvas, struct color color);

/*
 * Set every pixel in the canvas to black (0, 0, 0)
 */
void canvas_clear(struct canvas canvas);

/*
 * Blitting
 */

/*
 * blit an area of one canvas to another using the specified blending mode
 * int srx1: start of blit area x-coord on source
 * int srx2: end of blit area x-coord on source
 * int sry1, sry2: as srx1 and srx2 but for the y coords
 * dsx, dsy: start of area to blit to on destination
 */
void canvas_blit(struct canvas src, int srx1, int sry1, int srx2, int sry2,
                 struct canvas dst, int dsx, int dsy, enum blit_mode mode);

/*
 * fills a canvas with red and white squared for testing purposes
 */
void canvas_test(struct canvas src);

/*
 * Exporting and Importing
 */

/* 
 * Write the contents of a canvas to a ppm file - this is mostly
 * for testing and more robust, portable file i/o code should be
 * written for screenshots etc
 */
const int canvas_export_to_ppm(struct canvas c, const char *filename);

/* 
 * Write the contents of a canvas to a bitmap file - this is mostly
 * for testing and more robust, portable file i/o code should be
 * written for screenshots etc
 */
const int canvas_export_to_bmp(struct canvas c, const char *filename);

/*
 * Create a canvas from a bitmap file
 */
struct canvas canvas_from_bmp(const char *filename);

/******************
 * RENDERER
 *****************/

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

/*******************
 * TEXTURE
 ******************/

/*
 * This module covers textures, which are primarily made up of a canvas of rgb
 * colour data and a matching grid of integer values making up a mask. This
 * allows the renderer to draw only those colors on the canvas that have a 
 * matching non-zero value in the mask and allows a new palette to be swapped
 * in for any texture using a paint-by-numbers technique with the mask values
 * as numbers. This may replace the canvas altogher depending on how things go.
 */

/* create a new blank texture */
struct texture texture(const int width, const int height);

/* 
 * create a texture from a canvas providing a colour to treat as transparency
 * will assume no transparency if NULL is passed
 */
struct texture texture_from_canvas(const struct canvas c, struct color *trans);

/*
 * return the value of the mask (palette color index) at the given coordinate
 * a negative value indicates the pixel is transparent
 */
int texture_read_mask(struct texture tex, int x, int y);

/*
 * read the value of the pixel using the texture's palette
 */
struct color texture_read_pixel(struct texture tex, int x, int y);

/*
 * blit an area of a texture to a canvas using the specified blending mode
 * int srx1: start of blit area x-coord on source
 * int srx2: end of blit area x-coord on source
 * int sry1, sry2: as srx1 and srx2 but for the y coords
 * dsx, dsy: start of area to blit to on destination
 */
void texture_blit_to_canvas(struct texture tex, int srx1, int sry1, int srx2, 
                            int sry2, struct canvas dst, int dsx, int dsy, 
                            enum blit_mode mode);



#endif // header guard
