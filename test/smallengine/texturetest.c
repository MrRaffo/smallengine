#include <stdio.h>
#include <assert.h>

#include <smallengine/sys/mem.h>
#include <smallengine/graphics/texture.h>
#include <smallengine/graphics/canvas.h>
#include <smallengine/graphics/color.h>

void TST_TextureNew()
{
       struct texture t = texture(5, 4);
       assert(t.w == 5);
       assert(t.h == 4);
       assert(t.canvas.w == 5);
       assert(t.canvas.h == 4);
       assert(t.mask != NULL);

       printf("[Texture New] Complete, all tests pass!\n");
}

void TST_TextureFromCanvas()
{
        struct canvas c = canvas(6, 3);
        struct color white = color_rgb(1.0, 1.0, 1.0);
        struct color red = color_rgb(1.0, 0.0, 0.0);

        canvas_pattern(c, white, red, 1);
        struct texture t = texture_from_canvas(c, &white);

        assert(t.w == 6);
        assert(t.h == 3);
        assert(color_equal(canvas_read_pixel(t.canvas, 0, 0), white) == 1);

        assert(t.mask[0] == 0);
        assert(t.mask[1] == 1);

        printf("[Textue From Canvas] Complete, all tests pass!\n");
}

int main()
{
        mem_init(32 * MEM_MEGABYTE);

        TST_TextureNew();
        TST_TextureFromCanvas();

        mem_destroy();

        return 0;
}
