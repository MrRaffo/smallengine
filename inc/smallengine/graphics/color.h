#ifndef __color_h__
#define __color_h__

#include <stdint.h>
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


/*
 * The basic color type used with the canvas structure to produce images
 */

struct color {
        double r;
        double g;
        double b;
        double a;
};

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


#endif // __color_h__
