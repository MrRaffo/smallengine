#include <smallengine/se_sys.h>
#include <smallengine/se_graphics.h>

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
        // create the palette
        struct palette pal = palette_from_canvas(c);
        
        // create the texture and space for the mask
        struct texture tex = {c.w, c.h, c, NULL, pal};
        tex.mask = (int *)mem_alloc(c.w * c.h * sizeof(int));
        
        for (int i = 0; i < c.w * c.h; i++) {
                struct color col = c.pixels[i];
                // check transparancy
                if (color_equal(col, *trans)) {
                        tex.mask[i] = -1;
                        continue;
                }
                
                // this will automatically set any colors not found in the
                // palette to transparent
                tex.mask[i] = palette_check_color(tex.palette, col);
        }

        return tex;
}

/*
 * return the value of the mask (palette color index) at the given coordinate
 * a negative value indicates the pixel is transparent
 */
int texture_read_mask(struct texture tex, int x, int y)
{
        if (x < 0 || x >= tex.w || y < 0 || y >= tex.h) {return -1;}
        return tex.mask[y*tex.w + x];
}

/*
 * read the value of the pixel using the texture's palette
 */
struct color texture_read_pixel(struct texture tex, int x, int y)
{
        int index = texture_read_mask(tex, x, y);
        if (index < 0) {return palette_get_by_index(tex.palette, 0);}

        return palette_get_by_index(tex.palette, index);
}

/*
 * clip the boundary for blits on x axis
 */
static int _clip_blit_x(struct texture src, int srx1, int srx2,
                        struct canvas dst, int dsx)
{
        // check limits
        if (srx1 >= src.w || dsx >= dst.w) { return 0; }

        // check this doesn't over run the source canvas
        if (srx2 >= src.w) {
                srx2 = src.w - 1;
        }

        // get width of blit area
        int dx = srx2 - srx1;

        // or the destination canvas
        if ((dsx + dx) >= dst.w) {
                dx = dst.w - dsx;
        }

        return dx;
}

static int _clip_blit_y(struct texture src, int sry1, int sry2,
                        struct canvas dst, int dsy)
{
        // check stupid inputs
        if (sry1 >= src.h || dsy >= dst.h) { return 0; }

        if (sry2 >= src.h) {
                sry2 = src.h - 1;
        }
                        
        int dy = sry2 - sry1;

        // check area is within destination bounds
        if ((dsy + dy) >= dst.h) {
                dy = dst.h - dsy;
        }

        return dy;
}

/*
 * blit an area of a texture to a canvas using the specified blending mode
 * int srx1: start of blit area x-coord on source
 * int srx2: end of blit area x-coord on source
 * int sry1, sry2: as srx1 and srx2 but for the y coords
 * dsx, dsy: start of area to blit to on destination
 */
void texture_blit_to_canvas(struct texture tex, int srx1, int sry1, int srx2, 
                            int sry2, struct canvas dst, int dsx, int dsy, 
                            enum blit_mode mode)
{
        int dx = _clip_blit_x(tex, srx1, srx2, dst, dsx);
        int dy = _clip_blit_y(tex, sry1, sry2, dst, dsy);

        int x, y;
        for (x = 0; x <= dx; x++) {
                for (y = 0; y <= dy; y++) {
                        if (texture_read_mask(tex, srx1+x, sry1+y) < 0) {
                                continue;
                        }

                        canvas_write_pixel(dst, dsx+x, dsy+y, 
                                texture_read_pixel(tex, srx1+x, sry1+y), mode);
                }
        }
}

       


