#include "gtest/gtest.h"

#include <smallengine/se_maths.h>
#include <smallengine/se_sys.h>
#include <smallengine/se_graphics.h>

/**********************
 * COLOR TESTS
 *********************/

TEST(ColorTest, ColorNewTest) {

        struct color c = color_rgb(0.4, 0.5, 0.6);
        EXPECT_EQ(double_equal(0.4, c.r) , 1);
        EXPECT_EQ(double_equal(0.5, c.g) , 1);
        EXPECT_EQ(double_equal(0.6, c.b) , 1);

        c = color_rgba(0.8, 1.0, 1.2, 0.5);
        EXPECT_EQ(double_equal(0.4, c.r), 1);
        EXPECT_EQ(double_equal(0.5, c.g), 1);
        EXPECT_EQ(double_equal(0.6, c.b), 1);

        c = color_rgb_int(64, 128, 255);
        EXPECT_EQ(double_equal(0.250980, c.r), 1);
        EXPECT_EQ(double_equal(0.501960, c.g), 1);
        EXPECT_EQ(double_equal(1.0, c.b) , 1);

        c = color_rgba_int(64, 128, 255, 128);
        EXPECT_EQ(double_equal(0.125982, c.r), 1);
        EXPECT_EQ(double_equal(0.251964, c.g), 1);
        EXPECT_EQ(double_equal(0.501960, c.b), 1);
        EXPECT_EQ(double_equal(0.501960, c.a), 1);
}

TEST(ColorTest, ColorEqualTest) {
        struct color c1 = color_rgb(1.0, 3.4, 0.8);
        struct color c2 = color_rgba(1.0, 3.4, 0.8, 1.0);
        struct color c3 = color_rgb(0.7, 0.7, 0.7);

        EXPECT_EQ(color_equal(c1, c2), 1);
        EXPECT_EQ(color_equal(c1, c3), 0);
}

TEST(ColorTest, ColorConvertTest) {
        struct color c1 = color_rgb(2.5, 1.3, -9.0);
        struct color c2 = color_cap(c1);
        struct color res = color_rgb(1.0, 1.0, 0.0);

        EXPECT_EQ(color_equal(c2, res), 1);

        uint32_t argb = color_to_ARGB(c1);
        EXPECT_EQ(argb, 0xffffff00);
}

TEST(ColorTest, ColorAddTest) {
        struct color c1 = color_rgb(0.8, 0.1, 0.005);
        struct color c2 = color_rgb(0.25, 0.333, 0.5);
        struct color c3 = color_rgb(1.05, 0.433, 0.505);
        struct color c4 = color_rgb(1.85, 0.533, 0.51);

        EXPECT_EQ(color_equal(color_add(c1, c2), c3), 1);
        EXPECT_EQ(color_equal(color_add(c1, c3), c4), 1);
}

TEST(ColorTest, ColorSubtractTest) {
        struct color c1 = color_rgb(0.8, 0.1, 0.5);
        struct color c2 = color_rgb(0.25, 0.333, 0.005);
        struct color c3 = color_rgb(0.55, -0.233, 0.495);
        struct color c4 = color_rgb(0.25, 0.333, 0.005);

        EXPECT_EQ(color_equal(color_subtract(c1, c2), c3), 1);
        EXPECT_EQ(color_equal(color_subtract(c1, c3), c4), 1);
}

TEST(ColorTest, ColorScaleTest) {
        struct color c1 = color_rgb(0.2, 0.4, 0.8);
        struct color c2 = color_rgb(0.4, 0.8, 1.6);
        struct color c3 = color_rgb(0.5, 1.0, 2.0);

        EXPECT_EQ(color_equal(color_scale(c1, 2.0), c2), 1);
        EXPECT_EQ(color_equal(color_scale(c1, 2.5), c3), 1);
}

TEST(ColorTest, ColorMultiplyTest) {
        struct color c1 = color_rgb(0.2, 0.3, 0.4);
        struct color c2 = color_rgb(0.4, 0.6, 0.8);
        struct color c3 = color_rgb(0.08, 0.18, 0.32);
        struct color c4 = color_rgb(0.016, 0.054, 0.128);

        EXPECT_EQ(color_equal(color_multiply(c1, c2), c3), 1);
        EXPECT_EQ(color_equal(color_multiply(c1, c3), c4), 1);
}

/**********************
 * CANVAS TESTS
 *********************/

TEST(CanvasTest, CanvasNewTest) {

        mem_init(1 * MEM_MEGABYTE);

        struct canvas c = canvas(3, 4);

        EXPECT_EQ(c.w, 3);
        EXPECT_EQ(c.h, 4);
        EXPECT_TRUE(c.pixels);

        EXPECT_EQ(color_equal(c.pixels[1], color_rgb(0.0, 0.0, 0.0)), 1);

        mem_destroy();
}

TEST(CanvasTest, CanvasReadWriteTest) {

        mem_init(1 * MEM_MEGABYTE);

        struct canvas c = canvas(3, 3);
        struct color col1 = color_rgb(1.0, 0.5, 0.4);
        struct color col2 = color_rgb(0.1, 0.1, 0.1);

        EXPECT_EQ(canvas_write_pixel(c, 0, 0, col1, BLIT_ABS), 1);
        EXPECT_EQ(canvas_write_pixel(c, 1, 1, col1, BLIT_ABS), 1);
        EXPECT_EQ(canvas_write_pixel(c, 2, 2, col2, BLIT_ABS), 1);

        // out of bounds write
        EXPECT_EQ(canvas_write_pixel(c, 3, 2, col2, BLIT_ABS), 0);

        // READING
        EXPECT_EQ(color_equal(canvas_read_pixel(c, 0, 0), col1), 1);
        EXPECT_EQ(color_equal(canvas_read_pixel(c, 1, 1), col1), 1);
        EXPECT_EQ(color_equal(canvas_read_pixel(c, 2, 2), col2), 1);

        struct color black = color_rgb(0.0, 0.0, 0.0);
        EXPECT_EQ(color_equal(canvas_read_pixel(c, 40, 40), black), 1);

        mem_destroy();
}

TEST(CanvasTest, CanvasFillTest) {

        mem_init(1 * MEM_MEGABYTE);

        struct canvas can = canvas(3, 3);
        struct color black = color_rgb(0.0, 0.0, 0.0);
        struct color red = color_rgb(1.0, 0.0, 0.0);

        EXPECT_EQ(color_equal(canvas_read_pixel(can, 1, 1), black), 1);

        canvas_fill(can, red);
        EXPECT_EQ(color_equal(canvas_read_pixel(can, 1, 1), red), 1);

        canvas_clear(can);
        EXPECT_EQ(color_equal(canvas_read_pixel(can, 1, 1), black), 1);

        mem_destroy();
}

TEST(CanvasTest, CanvasBlitTest) {
        mem_init(1 * MEM_MEGABYTE);

        struct canvas src = canvas(30, 30);
        struct canvas dst = canvas(50, 50);

        struct color black = color_rgb(0.0, 0.0, 0.0);
        struct color red = color_rgb(1.0, 0.0, 0.0);

        canvas_fill(src, red);

        EXPECT_EQ(color_equal(canvas_read_pixel(dst, 10, 10), black), 1);
        EXPECT_EQ(color_equal(canvas_read_pixel(src, 10, 10), red), 1);

        canvas_blit(src, 0, 0, 29, 29, dst, 10, 10, BLIT_ABS);

        EXPECT_EQ(color_equal(canvas_read_pixel(dst, 0, 0), black), 1);
        EXPECT_EQ(color_equal(canvas_read_pixel(dst, 10, 10), red), 1);
        EXPECT_EQ(color_equal(canvas_read_pixel(dst, 39, 39), red), 1);
        EXPECT_EQ(color_equal(canvas_read_pixel(dst, 49, 49), black), 1);

        struct color add_to_get_white = color_rgb(0.0, 1.0, 1.0);
        canvas_fill(src, add_to_get_white);

        canvas_blit(src, 0, 0, 29, 29, dst, 10, 10, BLIT_ADD);
        struct color white = color_rgb(1.0, 1.0, 1.0);
        EXPECT_EQ(color_equal(canvas_read_pixel(dst, 10, 10), white), 1);

        mem_destroy();
}

/**********************
 * PALETTE TESTS
 *********************/

TEST(PaletteTest, PaletteNewTest) {

        mem_init(1 * MEM_MEGABYTE);

        struct palette p = palette(5);
        EXPECT_TRUE(p.colors);
        EXPECT_EQ(p.size, 5);

        mem_destroy();
}

TEST(PaletteTest, PaletteAddColor) {
        mem_init(1 * MEM_MEGABYTE);

        struct palette p = palette(2);
        EXPECT_TRUE(p.colors);
        EXPECT_EQ(p.size, 2);

        struct color c = color_rgb(0.5, 0.25, 0.0);
        palette_add_color(&p, c);

        EXPECT_EQ(color_equal(*(p.colors[0]), c), 1);
        EXPECT_EQ(p.assigned, 1);

        struct color c1 = color_rgb(1.0, 1.0, 1.0);
        palette_add_color(&p, c1);

        EXPECT_EQ(color_equal(*(p.colors[1]), c1), 1);
        EXPECT_EQ(p.assigned, 2);

        EXPECT_EQ(palette_add_color(&p, c1), 0);

        mem_destroy();
}

TEST(PaletteTest, GetColorByIndexTest) {

        mem_init(1 * MEM_MEGABYTE);

        struct palette p = palette(5);
        struct color r = color_rgb(1.0, 0.0, 0.0);
        struct color g = color_rgb(0.0, 1.0, 0.0);
        struct color b = color_rgb(0.0, 0.0, 1.0);
        struct color black = color_rgb(0.0, 0.0, 0.0);

        palette_add_color(&p, r);
        palette_add_color(&p, g);
        palette_add_color(&p, b);

        EXPECT_EQ(color_equal(r, palette_get_by_index(p, 0)), 1);
        EXPECT_EQ(color_equal(g, palette_get_by_index(p, 1)), 1);
        EXPECT_EQ(color_equal(b, palette_get_by_index(p, 2)), 1);
        EXPECT_EQ(color_equal(black, palette_get_by_index(p, 62)), 1);

        mem_destroy();
}

TEST(PaletteTest, CheckColorInPaletteTest) {

        mem_init(1 * MEM_MEGABYTE);

        struct palette p = palette(5);
        struct color r = color_rgb(1.0, 0.0, 0.0);
        struct color g = color_rgb(0.0, 1.0, 0.0);
        struct color b = color_rgb(0.0, 0.0, 1.0);
        struct color black = color_rgb(0.0, 0.0, 0.0);

        palette_add_color(&p, r);
        palette_add_color(&p, g);
        palette_add_color(&p, b);

        EXPECT_EQ(palette_check_color(p, r), 0);
        EXPECT_EQ(palette_check_color(p, b), 2);
        EXPECT_EQ(palette_check_color(p, black), -1);

        mem_destroy();
}

TEST(PaletteTest, PaletteReplaceColor) {

        mem_init(1 * MEM_MEGABYTE);

        struct palette p = palette(5);
        struct color r = color_rgb(1.0, 0.0, 0.0);
        struct color g = color_rgb(0.0, 1.0, 0.0);
        struct color b = color_rgb(0.0, 0.0, 1.0);
        struct color yellow = color_rgb(1.0, 1.0, 0.0);

        palette_add_color(&p, r);
        palette_add_color(&p, g);
        palette_add_color(&p, b);

        EXPECT_EQ(palette_check_color(p, yellow), -1);
        EXPECT_EQ(palette_check_color(p, g), 1);

        palette_replace_color(p, g, yellow);

        EXPECT_EQ(palette_check_color(p, yellow), 1);
        EXPECT_EQ(palette_check_color(p, g), -1);

        mem_destroy();
}

TEST(PaletteTest, PaletteReplaceByIndex) {

        mem_init(1 * MEM_MEGABYTE);

        struct palette p = palette(5);
        struct color r = color_rgb(1.0, 0.0, 0.0);
        struct color g = color_rgb(0.0, 1.0, 0.0);
        struct color b = color_rgb(0.0, 0.0, 1.0);
        struct color yellow = color_rgb(1.0, 1.0, 0.0);

        palette_add_color(&p, r);
        palette_add_color(&p, g);
        palette_add_color(&p, b);

        EXPECT_EQ(palette_check_color(p, yellow), -1);
        EXPECT_EQ(palette_check_color(p, g), 1);

        palette_replace_index(p, 1, yellow);

        EXPECT_EQ(palette_check_color(p, yellow), 1);
        EXPECT_EQ(palette_check_color(p, g), -1);

        mem_destroy();
}

TEST(PaletteTest, PaletteFromCanvasTest) {

        mem_init(1 * MEM_MEGABYTE);

        struct color r = color_rgb(1.0, 0.0, 0.0);
        struct color g = color_rgb(0.0, 1.0, 0.0);
        struct color b = color_rgb(0.0, 0.0, 1.0);
        struct color y = color_rgb(1.0, 1.0, 0.0);
        struct color w = color_rgb(1.0, 1.0, 1.0);

        struct canvas c = canvas(5, 5);
        canvas_fill(c, r);

        canvas_write_pixel(c, 3, 0, y, BLIT_ABS);
        canvas_write_pixel(c, 1, 1, b, BLIT_ABS);
        canvas_write_pixel(c, 4, 2, g, BLIT_ABS);
        canvas_write_pixel(c, 4, 4, y, BLIT_ABS);

        struct palette pal = palette_from_canvas(c);

        EXPECT_EQ(pal.assigned, 4);
        EXPECT_EQ(pal.size, 4);

        EXPECT_EQ(palette_check_color(pal, r), 0);
        EXPECT_EQ(palette_check_color(pal, y), 1);
        EXPECT_EQ(palette_check_color(pal, b), 2);
        EXPECT_EQ(palette_check_color(pal, g), 3);
        EXPECT_EQ(palette_check_color(pal, w), -1);

        mem_destroy();
}

/**********************
 * TEXTURE TESTS
 *********************/

TEST(TextureTest, TextureNewTest) {

        mem_init(1 * MEM_MEGABYTE);

        struct texture t = texture(5, 4);
        EXPECT_EQ(t.w, 5);
        EXPECT_EQ(t.h, 4);
        EXPECT_EQ(t.canvas.w, 5);
        EXPECT_EQ(t.canvas.h, 4);
        EXPECT_TRUE(t.mask);

        mem_destroy();
}

TEST(TextureTest, TextureFromCanvasTest) {

        mem_init(1 * MEM_MEGABYTE);

        struct canvas c = canvas(6, 3);
        struct color white = color_rgb(1.0, 1.0, 1.0);
        struct color red = color_rgb(1.0, 0.0, 0.0);
        struct color green = color_rgb(0.0, 1.0, 0.0);
        struct color blue = color_rgb(0.0, 0.0, 1.0);

        canvas_pattern(c, white, red, 1);
        canvas_write_pixel(c, 2, 0, green, BLIT_ABS);
        canvas_write_pixel(c, 3, 0, blue, BLIT_ABS);
        struct texture t = texture_from_canvas(c, &white);

        EXPECT_EQ(t.w, 6);
        EXPECT_EQ(t.h, 3);
        EXPECT_EQ(color_equal(canvas_read_pixel(t.canvas, 0, 0), white), 1);

        EXPECT_EQ(t.mask[0], -1);
        EXPECT_EQ(t.mask[1], 1);

        EXPECT_EQ(t.mask[2], 2);
        EXPECT_EQ(t.mask[3], 3);
        EXPECT_EQ(t.palette.size, 4);

        mem_destroy();
}

TEST(TextureText, TextureBlitText) {
 
        mem_init(1 * MEM_MEGABYTE);

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


        EXPECT_EQ(color_equal(canvas_read_pixel(dst, 0, 0), black), 1);
        // white should not be transferred as it is transparent
        EXPECT_EQ(color_equal(canvas_read_pixel(dst, 10, 12), black), 1);
        EXPECT_EQ(color_equal(canvas_read_pixel(dst, 11, 12), red), 1);
        EXPECT_EQ(color_equal(canvas_read_pixel(dst, 12, 12), green), 1);
        EXPECT_EQ(color_equal(canvas_read_pixel(dst, 13, 12), blue), 1);
        EXPECT_EQ(color_equal(canvas_read_pixel(dst, 14, 12), black), 1);
        EXPECT_EQ(color_equal(canvas_read_pixel(dst, 49, 49), black), 1);

        mem_destroy();
}


