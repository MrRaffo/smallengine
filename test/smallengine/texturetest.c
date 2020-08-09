#include <stdio.h>
#include <assert.h>

#include <smallengine/se_sys.h>
#include <smallengine/se_graphics.h>

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
        struct color green = color_rgb(0.0, 1.0, 0.0);
        struct color blue = color_rgb(0.0, 0.0, 1.0);

        canvas_pattern(c, white, red, 1);
        canvas_write_pixel(c, 2, 0, green, BLIT_ABS);
        canvas_write_pixel(c, 3, 0, blue, BLIT_ABS);
        struct texture t = texture_from_canvas(c, &white);

        assert(t.w == 6);
        assert(t.h == 3);
        assert(color_equal(canvas_read_pixel(t.canvas, 0, 0), white) == 1);

        assert(t.mask[0] == -1);
        assert(t.mask[1] == 1);

        assert(t.mask[2] == 2);
        assert(t.mask[3] == 3);
        assert(t.palette.size == 4);

        printf("[Textue From Canvas] Complete, all tests pass!\n");
}

void TST_TextureBlit()
{
        struct color black = color_rgb(0.0, 0.0, 0.0);
        struct color white = color_rgb(1.0, 1.0, 1.0);
        struct color red = color_rgb(1.0, 0.0, 0.0);
        struct color green = color_rgb(0.0, 1.0, 0.0);
        struct color blue = color_rgb(0.0, 0.0, 1.0);

        struct canvas c = canvas(30, 30);
        canvas_pattern(c, white, red, 1);
        canvas_write_pixel(c, 2, 2, green, BLIT_ABS);
        canvas_write_pixel(c, 3, 2, blue, BLIT_ABS);
        struct texture tex = texture_from_canvas(c, &white);

        struct canvas dst = canvas(50, 50);
        canvas_fill(dst, black);

        texture_blit_to_canvas(tex, 0, 0, 29, 29, 
                               dst, 10, 10, BLIT_ABS);


        assert(color_equal(canvas_read_pixel(dst, 0, 0), black) == 1);
        // white should not be transferred as it is transparent
        assert(color_equal(canvas_read_pixel(dst, 10, 12), black) == 1);
        assert(color_equal(canvas_read_pixel(dst, 11, 12), red) == 1);
        assert(color_equal(canvas_read_pixel(dst, 12, 12), green) == 1);
        assert(color_equal(canvas_read_pixel(dst, 13, 12), blue) == 1);
        assert(color_equal(canvas_read_pixel(dst, 14, 12), black) == 1);
        assert(color_equal(canvas_read_pixel(dst, 49, 49), black) == 1);

        printf("[Texture Blit] Complete, all tests pass!\n");
}

int main()
{
        mem_init(32 * MEM_MEGABYTE);

        TST_TextureNew();
        TST_TextureFromCanvas();
        TST_TextureBlit();

        mem_destroy();

        return 0;
}
