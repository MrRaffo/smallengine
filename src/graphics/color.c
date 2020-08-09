#include <stdio.h>
#include <stdint.h>

#include <smallengine/se_graphics.h>
#include <smallengine/se_maths.h>
#include <smallengine/se_sys.h>

/*
 * Creation and Initialization
 */

/* create a new color with given components */
struct color color_rgb(const double r, const double g, const double b)
{
        struct color c = {r, g, b, 1.0};
        return c;
}

/* create a color with given alpha, r, g and b components will hold the value
 * of the given values multiplied by the alpha, the alpha will be store to
 * allow the original values to be calculated */
struct color color_rgba(const double r, const double g, const double b, 
        const double a)
{
        struct color c = {r*a, g*a, b*a, a};
        return c;
}

/* create a color using integer values instead of doubles */
struct color color_rgb_int(const int r, const int g, const int b)
{
        struct color c = {(double)r/255.0, 
                          (double)g/255.0,
                          (double)b/255.0, 
                          1.0};
        return c;
}

/* int components with alpha */
struct color color_rgba_int(const int r, const int g, const int b, 
        const int a)
{
        double alpha = (double)a/255.0;
        struct color c = {((double)r/255.0)*alpha,
                          ((double)g/255.0)*alpha,
                          ((double)b/255.0)*alpha,
                          alpha};
        return c;
}

/*
 * Comparison
 */

/* returns 1 if the two colours given are the same, 0 otherwise */
int color_equal(const struct color c1, const struct color c2)
{
        if (!double_equal(c1.r, c2.r)) { return 0; }
        if (!double_equal(c1.g, c2.g)) { return 0; }
        if (!double_equal(c1.b, c2.b)) { return 0; }

        return 1;
}

/*
 * Return Values
 */

static double _clamp_component_double(double comp)
{
        if (comp < 0.0) { return 0.0; }
        if (comp > 1.0) { return 1.0; }
        return comp;
}

static int _clamp_component_int(double comp)
{
        if (comp < 0.0) { return 0; }
        if (comp > 1.0) { return 255; }

        return (int)(comp * 255.0 + 0.5);
}

/* return a new color with the r g and b values restricted to the 
 *range 0.0-1.0 */
const struct color color_cap(const struct color c)
{
        struct color c1 = {_clamp_component_double(c.r),
                          _clamp_component_double(c.g),
                          _clamp_component_double(c.b),
                          _clamp_component_double(c.a)};
        return c1;
}

/* return a ppm style string of the color (ie "255 0 128")*/
char *color_to_ppm_string(const struct color c)
{
        int r = _clamp_component_int(c.r);
        int g = _clamp_component_int(c.g);
        int b = _clamp_component_int(c.b);

        // 12 = 3 for each component and a space between, and a final '\0'
        char *cstr = (char *)mem_alloc(12);
        sprintf(cstr, "%d %d %d", r, g, b);

        return cstr;
}

/* return the color as a 32 bit integer ARGB (bitmap) format, alpha is
 * always 0xff */
const uint32_t color_to_ARGB(struct color c)
{
        uint32_t val = 0x0;
        val = 0xff << 24 |
                (_clamp_component_int(c.r) & 0xff) << 16 |
                (_clamp_component_int(c.g) & 0xff) << 8  |
                (_clamp_component_int(c.b) & 0xff);

        return val;
}

/* 
 * return the color as a 32 bit integer in RGBA format, alpha is always 0xff
 */
const uint32_t color_to_RGBA(struct color c)
{
        uint32_t val = 0x0;
        val = (_clamp_component_int(c.r) & 0xff) << RSHIFT |
              (_clamp_component_int(c.g) & 0xff) << GSHIFT |
              (_clamp_component_int(c.b) & 0xff) << BSHIFT |
              0xff << ASHIFT;

        return val;
}

/*
 * Operations
 */

/* add 2 colors component by component and return the new color */
const struct color color_add(const struct color c1, const struct color c2)
{
        struct color new = {c1.r + c2.r, c1.g + c2.g, c1.b + c2.b, 1.0};
        return new;
}

/* subtract the corresponding component values of c2 from c1 and return the new
 * color */
const struct color color_subtract(const struct color c1, const struct color c2)
{
        struct color new = {c1.r - c2.r, c1.g - c2.g, c1.b - c2.b, 1.0};
        return new;
}

/* multiply all components in a colour by the given factor */
const struct color color_scale(const struct color c, const double f)
{
        struct color new = {c.r * f, c.g * f, c.b * f, 1.0};
        return new;
}

/* multiply the components of the colours together and return the resulting
 * new colour, also known as the Hadamard Product or Schur Product */
const struct color color_multiply(const struct color c1, const struct color c2)
{
        struct color new = {c1.r * c2.r, c1.g * c2.g, c1.b * c2.b, 1.0};
        return new;
}
