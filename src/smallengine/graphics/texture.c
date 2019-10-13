#include <smallengine/sys/mem.h>
#include <smallengine/graphics/texture.h>
#include <smallengine/graphics/canvas.h>
#include <smallengine/graphics/color.h>


struct texture texture(const int width, const int height)
{
        struct canvas c = canvas(width, height);
        struct texture t = {width, height, c, NULL};

        t.mask = (int *)mem_alloc(width * height * sizeof(int));

        return t;
}

/* 
 * create a texture from a canvas providing a colour to treat as transparency
 * will assume no transparency if NULL is passed
 */
struct texture texture_from_canvas(const struct canvas c, struct color *trans)
{
        struct texture t = {c.w, c.h, c, NULL};
        t.mask = (int *)mem_alloc(c.w * c.h * sizeof(int));

        /* this function currently assigns each non transparent colour a mask
         * value of '1', however this will need to be changed to allow each 
         * unique colour to be given its own mask value to allow palette 
         * swapping */
        
        for (int i = 0; i < c.w * c.h; i++) {
                if (trans == NULL) {
                        t.mask[i] = 1;
                } else {
                        if (color_equal(c.pixels[i], *trans)) {
                                t.mask[i] = 0;
                        } else {
                                t.mask[i] = 1;
                        }
                }
        }

        return t;
}
