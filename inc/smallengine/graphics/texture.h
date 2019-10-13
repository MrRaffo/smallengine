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

#endif // __texture_h__
