#include <stdio.h>
#include <assert.h>
#include <stdint.h>

#include <smallengine/maths/maths.h>

#include <smallengine/graphics/color.h>

void TST_ColorNew()
{
        struct color c = color_rgb(0.4, 0.5, 0.6);
        assert(double_equal(0.4, c.r) == 1);
        assert(double_equal(0.5, c.g) == 1);
        assert(double_equal(0.6, c.b) == 1);

        c = color_rgba(0.8, 1.0, 1.2, 0.5);
        assert(double_equal(0.4, c.r) == 1);
        assert(double_equal(0.5, c.g) == 1);
        assert(double_equal(0.6, c.b) == 1);

        c = color_rgb_int(64, 128, 255);
        assert(double_equal(0.250980, c.r) == 1);
        assert(double_equal(0.501960, c.g) == 1);
        assert(double_equal(1.0, c.b) == 1);

        c = color_rgba_int(64, 128, 255, 128);
        assert(double_equal(0.125982, c.r) == 1);
        assert(double_equal(0.251964, c.g) == 1);
        assert(double_equal(0.501960, c.b) == 1);
        assert(double_equal(0.501960, c.a) == 1);

        printf("[Color New] Complete, all tests pass!\n");
}

void TST_ColorEqual()
{
       struct color c1 = color_rgb(1.0, 3.4, 0.8);
       struct color c2 = color_rgba(1.0, 3.4, 0.8, 1.0);
       struct color c3 = color_rgb(0.7, 0.7, 0.7);

       assert(color_equal(c1, c2) == 1);
       assert(color_equal(c1, c3) == 0);

       printf("[Color Equal] Complete, all tests pass!\n");
}

void TST_ColorConvert()
{
        struct color c1 = color_rgb(2.5, 1.3, -9.0);
        struct color c2 = color_cap(c1);
        struct color res = color_rgb(1.0, 1.0, 0.0);

        assert(color_equal(c2, res) == 1);

        uint32_t argb = color_to_ARGB(c1);
        assert(argb == 0xffffff00);

        printf("[Color Convert] Complete, all tests pass!\n");
}

void TST_ColorAdd()
{
        struct color c1 = color_rgb(0.8, 0.1, 0.005);
        struct color c2 = color_rgb(0.25, 0.333, 0.5);
        struct color c3 = color_rgb(1.05, 0.433, 0.505);
        struct color c4 = color_rgb(1.85, 0.533, 0.51);

        assert(color_equal(color_add(c1, c2), c3));
        assert(color_equal(color_add(c1, c3), c4));
        
        printf("[Color Add] Complete, all tests pass!\n");
}

void TST_ColorSubtract()
{
        struct color c1 = color_rgb(0.8, 0.1, 0.5);
        struct color c2 = color_rgb(0.25, 0.333, 0.005);
        struct color c3 = color_rgb(0.55, -0.233, 0.495);
        struct color c4 = color_rgb(0.25, 0.333, 0.005);

        assert(color_equal(color_subtract(c1, c2), c3));
        assert(color_equal(color_subtract(c1, c3), c4));
        
        printf("[Color Subract] Complete, all tests pass!\n");
}

void TST_ColorScale()
{
        struct color c1 = color_rgb(0.2, 0.4, 0.8);
        struct color c2 = color_rgb(0.4, 0.8, 1.6);
        struct color c3 = color_rgb(0.5, 1.0, 2.0);

        assert(color_equal(color_scale(c1, 2.0), c2));
        assert(color_equal(color_scale(c1, 2.5), c3));

        printf("[Color Scale] Complete, all tests pass!\n");
}

void TST_ColorMultiply()
{
        struct color c1 = color_rgb(0.2, 0.3, 0.4);
        struct color c2 = color_rgb(0.4, 0.6, 0.8);
        struct color c3 = color_rgb(0.08, 0.18, 0.32);
        struct color c4 = color_rgb(0.016, 0.054, 0.128);

        assert(color_equal(color_multiply(c1, c2), c3));
        assert(color_equal(color_multiply(c1, c3), c4));

        printf("[Color Multiply] Complete, all tests pass!\n");
}

int main()
{
        TST_ColorNew();
        TST_ColorEqual();
        TST_ColorConvert();
        TST_ColorAdd();
        TST_ColorSubtract();
        TST_ColorScale();
        TST_ColorMultiply();

        return 0;
}
