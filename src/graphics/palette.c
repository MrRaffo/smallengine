#include <stdio.h>

#include <smallengine/se_graphics.h>
#include <smallengine/se_sys.h>

/*
 * Palette Creation
 */

/*
 * create a new palette with no colors and size 0
 */
struct palette palette(int size)
{
        struct color **ptr = mem_alloc(sizeof(struct color *) * size);
        struct palette p = {ptr, size, 0};
        return p;
}

/*
 * create a palette from a canvas. the canvas will be checked from left to
 * right, row by row, each unique color added in the order it is encountered
 */
struct palette palette_from_canvas(const struct canvas can)
{
        // create a list of colors large enough to count all colors
        // even if each pixel on the canvas contains a unique value.
        // For a 1080 * 1920 image this would be around 50MB
        // This is destroyed once the palette is made
        int size = can.w * can.h;
        struct color *list = 
                (struct color *)mem_alloc(size * sizeof(struct color));

        // get all unique colors - slowly...
        int i, j, count = 0, check = 0;
        for (i = 0; i < size; i++) {
                check = 0;
                
                // check if color at current pixel has already been added
                // to the list
                for (j = 0; j < count; j++) {
                        if (color_equal(can.pixels[i], list[j])) {
                                check = 1;
                                break;
                        }
                }

                if (check == 0) {
                        list[count++] = can.pixels[i];
                }
        }

        struct palette p = palette(count);
        for (i = 0; i < count; i++) {
                palette_add_color(&p, list[i]);
        }

        mem_free(list);

        return p;
}

/*
 * Palette Destruction
 */

/*
 * free all memory used by a palette
 */
void palette_destroy(struct palette *pal)
{
        free(pal->colors);
        pal->size = 0;
        pal->assigned = 0;
}

/*
 * Palette Operations
 */

/*
 * check if a palette contains a certain color, returns index if so, -1 otherwise
 */
int palette_check_color(struct palette pal, const struct color col)
{
        for (int i = 0; i < pal.assigned; i++) {
                if (color_equal(*pal.colors[i], col)) {
                        return i;
                }
        }

        return -1;
}

/*
 * get the color of the specified index, returns (0.0, 0.0, 0.0) if the index
 * is invalid
 */
struct color palette_get_by_index(struct palette pal, int index)
{
        if (index < 0 || index >= pal.assigned) {
                return color_rgb(0.0, 0.0, 0.0);
        }
        
        return *pal.colors[index];
}


// check if palette contains a range of colors? replace a range of colors?

/*
 * add a color to a palette, returns the new size of the palette
 */
int palette_add_color(struct palette *p, const struct color col)
{
        // check if there is room for a new color
        if (p->assigned >= p->size) {
                return 0;
        }

        p->colors[p->assigned] = mem_alloc(sizeof(struct color));
        *p->colors[p->assigned] = col;
        return p->assigned++;
}

/*
 * replace a color in a palette with a new one, return 1 on success, 0 on fail
 */
int palette_replace_color(struct palette p, const struct color oldc, 
                                             const struct color newc)
{
        int index = palette_check_color(p, oldc);
        
        if (index < 0) {
                return -1;
        }

        *p.colors[index] = newc;
        return 1;
}

/*
 * replace a color at a given index regardless of it's value, returns 1 on
 * success, 0 on failure
 */
int palette_replace_index(struct palette p, int index, const struct color c)
{
        if (index < 0 || index >= p.assigned) {
                return 0;
        }

        *p.colors[index] = c;
        return 1;
}

