#ifndef __canvas_h__
#define __canvas_h__

/*
 * the canvas that all graphics are drawn on, will be used to store graphics
 * assets at runtime as well as constructing the image of each frame of the
 * game, which can be transferred to the main rendering surface (such as the
 * on provided by SDL) to be displayed. Can also output to BMP
 */

#include <smallengine/graphics/color.h>

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

#endif // __canvas_h__

