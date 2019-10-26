#ifndef __texture_h__
#define __texture_h__

/*
 * texture
 * 
 * This module covers textures, which are primarily made up of a canvas of rgb
 * colour data and a matching grid of integer values making up a mask. This
 * allows the renderer to draw only those colors on the canvas that have a 
 * matching non-zero value in the mask and allows a new palette to be swapped
 * in for any texture using a paint-by-numbers technique with the mask values
 * as numbers. This may replace the canvas altogher depending on how things go.
 *
 */

#include <smallengine/graphics/canvas.h>
#include <smallengine/graphics/color.h>
#include <smallengine/graphics/palette.h>

struct texture {
        int w;
        int h;
        struct canvas canvas;   // direct color data
        int *mask;              // indices of associated palette
        struct palette palette; // colors to match indices of the mask
};

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


#endif // __texture_h__
