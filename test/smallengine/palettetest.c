#include <stdio.h>
#include <assert.h>

#include <smallengine/graphics/palette.h>
#include <smallengine/graphics/canvas.h>
#include <smallengine/graphics/color.h>

#include <smallengine/sys/mem.h>

void TST_PaletteNew()
{
        struct palette p = palette(5);
        assert(p.colors != NULL);
        assert(p.size == 5);

        printf("[Palette New] Complete, all tests pass!\n");
}

void TST_PaletteAddColor()
{
        struct palette p = palette(2);
        assert(p.colors != NULL);
        assert(p.size == 2);

        struct color c = color_rgb(0.5, 0.25, 0.0);
        palette_add_color(&p, c);

        assert(color_equal(*(p.colors[0]), c) == 1);
        assert(p.assigned == 1);

        struct color c1 = color_rgb(1.0, 1.0, 1.0);
        palette_add_color(&p, c1);

        assert(color_equal(*(p.colors[1]), c1) == 1);
        assert(p.assigned == 2);

        assert(palette_add_color(&p, c1) == 0);

        printf("[Palette Add Color] Complete, all tests pass!\n");
}

void TST_GetColorByIndex()
{
        struct palette p = palette(5);
        struct color r = color_rgb(1.0, 0.0, 0.0);
        struct color g = color_rgb(0.0, 1.0, 0.0);
        struct color b = color_rgb(0.0, 0.0, 1.0);
        struct color black = color_rgb(0.0, 0.0, 0.0);

        palette_add_color(&p, r);
        palette_add_color(&p, g);
        palette_add_color(&p, b);

        assert(color_equal(r, palette_get_by_index(p, 0)) == 1);
        assert(color_equal(g, palette_get_by_index(p, 1)) == 1);
        assert(color_equal(b, palette_get_by_index(p, 2)) == 1);
        assert(color_equal(black, palette_get_by_index(p, 62)) == 1);

        printf("[Palette Color by Index] Complete, all tests pass!\n");
}

void TST_CheckColorInPalette()
{
        struct palette p = palette(5);
        struct color r = color_rgb(1.0, 0.0, 0.0);
        struct color g = color_rgb(0.0, 1.0, 0.0);
        struct color b = color_rgb(0.0, 0.0, 1.0);
        struct color black = color_rgb(0.0, 0.0, 0.0);
 
        palette_add_color(&p, r);
        palette_add_color(&p, g);
        palette_add_color(&p, b);

        assert(palette_check_color(p, r) == 0);
        assert(palette_check_color(p, b) == 2);
        assert(palette_check_color(p, black) == -1);
        
        printf("[Palette Check Color] Complete, all tests pass!\n");
}

void TST_ReplaceColor()
{
        struct palette p = palette(5);
        struct color r = color_rgb(1.0, 0.0, 0.0);
        struct color g = color_rgb(0.0, 1.0, 0.0);
        struct color b = color_rgb(0.0, 0.0, 1.0);
        struct color yellow = color_rgb(1.0, 1.0, 0.0);

        palette_add_color(&p, r);
        palette_add_color(&p, g);
        palette_add_color(&p, b);

        assert(palette_check_color(p, yellow) == -1);
        assert(palette_check_color(p, g) == 1);

        palette_replace_color(p, g, yellow);

        assert(palette_check_color(p, yellow) == 1);
        assert(palette_check_color(p, g) == -1);

        printf("[Palette Replace Color] Complete, all tests pass!\n");

}

void TST_ReplaceIndex()
{
        struct palette p = palette(5);
        struct color r = color_rgb(1.0, 0.0, 0.0);
        struct color g = color_rgb(0.0, 1.0, 0.0);
        struct color b = color_rgb(0.0, 0.0, 1.0);
        struct color yellow = color_rgb(1.0, 1.0, 0.0);

        palette_add_color(&p, r);
        palette_add_color(&p, g);
        palette_add_color(&p, b);

        assert(palette_check_color(p, yellow) == -1);
        assert(palette_check_color(p, g) == 1);

        palette_replace_index(p, 1, yellow);

        assert(palette_check_color(p, yellow) == 1);
        assert(palette_check_color(p, g) == -1);

        printf("[Palette Replace Index] Complete, all tests pass!\n");
}

void TST_PaletteFromCanvas()
{
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

        assert(pal.assigned == 4);
        assert(pal.size == 4);

        assert(palette_check_color(pal, r) == 0);
        assert(palette_check_color(pal, y) == 1);
        assert(palette_check_color(pal, b) == 2);
        assert(palette_check_color(pal, g) == 3);
        assert(palette_check_color(pal, w) == -1);

        printf("[Palette From Canvas] Complete, all tests pass!\n");
}

int main()
{
        mem_init(MEM_MEGABYTE * 50);

        TST_PaletteNew();
        TST_PaletteAddColor();
        TST_GetColorByIndex();
        TST_CheckColorInPalette();
        TST_ReplaceColor();
        TST_ReplaceIndex();
        TST_PaletteFromCanvas();

        mem_destroy();

        return 0;
}
