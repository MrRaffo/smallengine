#ifndef __palette_h__
#define __palette_h__

/*
 * a data struct to contain colors used for sprites and other objects
 */

#include <smallengine/graphics/color.h>
#include <smallengine/graphics/canvas.h>

struct palette {
        struct color **colors;
        int size;               // total number of colors that can be stored
        int assigned;           // number of colors in the palette
};

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
 * check if a palette contains a certain color, returns index if so, 0 otherwise
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

#endif // __palette_h__
