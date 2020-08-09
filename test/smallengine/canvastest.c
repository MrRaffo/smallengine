#include <stdio.h>
#include <assert.h>

#include <smallengine/se_sys.h>
#include <smallengine/se_graphics.h>

void TST_CanvasNew()
{
        struct canvas c = canvas(3, 4);

        assert(c.w == 3);
        assert(c.h == 4);
        assert(c.pixels != NULL);

        assert(color_equal(c.pixels[1], color_rgb(0.0, 0.0, 0.0)) == 1);

        printf("[Canvas New] Complete, all tests pass!\n");
}

void TST_CanvasReadWrite()
{
        struct canvas c = canvas(3, 3);
        struct color col1 = color_rgb(1.0, 0.5, 0.4);
        struct color col2 = color_rgb(0.1, 0.1, 0.1);

        assert(canvas_write_pixel(c, 0, 0, col1, BLIT_ABS) == 1);
        assert(canvas_write_pixel(c, 1, 1, col1, BLIT_ABS) == 1);
        assert(canvas_write_pixel(c, 2, 2, col2, BLIT_ABS) == 1);

        // out of bounds write
        assert(canvas_write_pixel(c, 3, 2, col2, BLIT_ABS) == 0);

        // READING
        assert(color_equal(canvas_read_pixel(c, 0, 0), col1) == 1);
        assert(color_equal(canvas_read_pixel(c, 1, 1), col1) == 1);
        assert(color_equal(canvas_read_pixel(c, 2, 2), col2) == 1);

        struct color black = color_rgb(0.0, 0.0, 0.0);
        assert(color_equal(canvas_read_pixel(c, 40, 40), black) == 1);

        printf("[Canvas Read Write] Complete, all tests pass!\n");
}

void TST_CanvasFill()
{
        struct canvas can = canvas(3, 3);
        struct color black = color_rgb(0.0, 0.0, 0.0);
        struct color red = color_rgb(1.0, 0.0, 0.0);
        
        assert(color_equal(canvas_read_pixel(can, 1, 1), black) == 1);

        canvas_fill(can, red);
        assert(color_equal(canvas_read_pixel(can, 1, 1), red) == 1);

        canvas_clear(can);
        assert(color_equal(canvas_read_pixel(can, 1, 1), black) == 1);

        printf("[Canvas Fill] Complete, all tests pass!\n");
}

void TST_CanvasBlit()
{
        struct canvas src = canvas(30, 30);
        struct canvas dst = canvas(50, 50);

        struct color black = color_rgb(0.0, 0.0, 0.0);
        struct color red = color_rgb(1.0, 0.0, 0.0);

        canvas_fill(src, red);

        assert(color_equal(canvas_read_pixel(dst, 10, 10), black) == 1);
        assert(color_equal(canvas_read_pixel(src, 10, 10), red) == 1);

        canvas_blit(src, 0, 0, 29, 29, dst, 10, 10, BLIT_ABS);

        assert(color_equal(canvas_read_pixel(dst, 0, 0), black) == 1);
        assert(color_equal(canvas_read_pixel(dst, 10, 10), red) == 1);
        assert(color_equal(canvas_read_pixel(dst, 39, 39), red) == 1);
        assert(color_equal(canvas_read_pixel(dst, 49, 49), black) == 1);

        struct color add_to_get_white = color_rgb(0.0, 1.0, 1.0);
        canvas_fill(src, add_to_get_white);

        canvas_blit(src, 0, 0, 29, 29, dst, 10, 10, BLIT_ADD);
        struct color white = color_rgb(1.0, 1.0, 1.0);
        assert(color_equal(canvas_read_pixel(dst, 10, 10), white) == 1);

        printf("[Canvas Blit] Complete, all tests pass!\n");
}

int main()
{
        mem_init(5 * MEM_MEGABYTE);

        TST_CanvasNew();
        TST_CanvasReadWrite();
        TST_CanvasFill();
        TST_CanvasBlit();

        mem_destroy();

        return 0;
}

