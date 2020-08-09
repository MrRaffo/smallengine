#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <smallengine/se_graphics.h>
#include <smallengine/se_sys.h>

/*
 * Creation and Initialization
 */

/*
 * Create a new canvas, every colour will be initialised to (0, 0, 0)
 */
struct canvas canvas(const int w, const int h)
{
        struct canvas c = {w, h, NULL};

        c.pixels = (struct color *)mem_alloc(w * h * sizeof(struct color));

        int i;
        for (i = 0; i < w * h; i++) {
                c.pixels[i] = color_rgb(0.0, 0.0, 0.0);
        }

        return c;
}

/*
 * Operations
 */

static int _within_bounds(struct canvas c, int x, int y)
{
        if (x < 0 || x >= c.w || y < 0 || y >= c.h) {
                return 0;
        }

        return 1;
}

/*
 * Read the value of a given pixel, the color (0, 0, 0) will be returned if a
 * pixel coordinate outside the canvas area was requested
 */
struct color canvas_read_pixel(struct canvas canvas, const int x, const int y)
{
        if (!_within_bounds(canvas, x, y)) {
                return color_rgb(0.0, 0.0, 0.0);
        }

        return canvas.pixels[y * canvas.w + x];
}
        

/*
 * Write the given color value to the coordinate on the canvas, returns 1 if
 * successful, 0 otherwise
 */
const int canvas_write_pixel(struct canvas can, int x, int y, struct color col,
                             enum blit_mode mode)
{
        if (!_within_bounds(can, x, y)) {
                return 0;
        }

        // temporary storage for blending
        struct color new;

        switch (mode) {
                case BLIT_ABS: 
                        can.pixels[y * can.w + x] = col;
                        break;

                case BLIT_ADD:
                        new = color_add(can.pixels[y * can.w + x], col);
                        can.pixels[y * can.w + x] = new;
                        break;

                case BLIT_MUL:
                        new = color_multiply(can.pixels[y * can.w + x], col);
                        can.pixels[y * can.w + x] = new;
                        break;

                default:
                        return 0;
                        break;  // yeah, I know...
        }
                        
        return 1;
}

/*
 * Fill the canvas with the color given
 */
void canvas_fill(struct canvas canvas, struct color color)
{
        int i;
        for (i = 0; i < canvas.w * canvas.h; i++) {
                canvas.pixels[i] = color;
        }
}

/*
 * Set every pixel in the canvas to black (0, 0, 0)
 */
void canvas_clear(struct canvas canvas)
{
        int i;
        struct color black = color_rgb(0.0, 0.0, 0.0);
        for (i = 0; i < canvas.w * canvas.h; i++) {
                canvas.pixels[i] = black;
        }
}

/*
 * Blitting
 */

static int _clip_blit_x(struct canvas src, int srx1, int srx2,
                        struct canvas dst, int dsx)
{
        // check stupid inputs
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

static int _clip_blit_y(struct canvas src, int sry1, int sry2,
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
 * blit an area of one canvas to another using the specified blending mode
 * int srx1: start of blit area x-coord on source
 * int srx2: end of blit area x-coord on source
 * int sry1, sry2: as srx1 and srx2 but for the y coords
 * dsx, dsy: start of area to blit to on destination
 */
void canvas_blit(struct canvas src, int srx1, int sry1, int srx2, int sry2,
                 struct canvas dst, int dsx, int dsy, enum blit_mode mode)
{
        int dx = _clip_blit_x(src, srx1, srx2, dst, dsx);
        int dy = _clip_blit_y(src, sry1, sry2, dst, dsy);

        int x, y;
        for (x = 0; x <= dx; x++) {
                for (y = 0; y <= dy; y++) {
                        canvas_write_pixel(dst, dsx+x, dsy+y, 
                                canvas_read_pixel(src, srx1+x, sry1+y), mode);
                }
        }
}

/*
 * fill a canvas with alternating colored squares of size tile_size
 */
void canvas_pattern(struct canvas src, struct color col1, 
                    struct color col2, int tile_size)
{
        int x, y;
        for (x = 0; x < src.w; x++) {
                for (y = 0; y < src.h; y++) {
                        if (((x / tile_size) + (y / tile_size)) & 1) {
                                canvas_write_pixel(src, x, y, col2, BLIT_ABS);
                        } else {
                                canvas_write_pixel(src, x, y, col1, BLIT_ABS);
                        }
                }
        }
}

/*
 * fills a canvas with red and white squares for testing purposes
 */
void canvas_test(struct canvas src)
{
        struct color red = color_rgb(1.0, 0.0, 0.0);
        struct color white = color_rgb(1.0, 1.0, 1.0);
        int sq = 4;     // tile size
        
        canvas_pattern(src, red, white, sq);

}

/*
 * Exporting
 */

/*
 * Create a header for a ppm file, it will follow this format:
 * "P3
 *  WW HH
 *  255"
 * Where WW and HH are strings of the decimal representations of the width and
 * height respectively (not restricted to 2 chars) (P3 is the file format
 * signature and 255 is the max colour component value (R, G and B = 0-255)
 */
static char *_create_ppm_header(const struct canvas c)
{
        // get header length:
        int wc = 1, hc = 1, w = c.w, h = c.h;
        while ((w /= 10) > 0) {
                wc++;
        }

        while ((h /= 10) > 0) {
                hc++;
        }

        #define SIG "P3\n"
        #define END "255\n"

        // "P3\n" + (wc + ' ' + hc copy string safe+ '\n') + "255\n" + '\0'
        int len = 3 + wc + 1 + hc + 1 + 4 + 1;
        char *header = (char *)mem_alloc(len);
        char *ptr = header;

        ptr += sprintf(ptr, SIG);
        ptr += sprintf(ptr, "%d %d\n", c.w, c.h);
        ptr += sprintf(ptr, END);

        return header; 
}

/*
 * Return a string formatted to the ppm pixel data format.
 * ie: 255 0 0 252 152 0 212 124 0 22 76
 * where each number is an r, g or b component
 * restricts each line to 12 components before a newline (ppm has a 70 char
 * legacy limit)
 */
static char *_create_ppm_data(struct canvas c)
{
        int num_pix = c.w * c.h;

        // reserve space assuming each R, G and B will be 3 chars long
        // and for a space between them
        char *data = (char *)mem_alloc(num_pix * 3 * 4);
        char *ptr = data;

        // ppm files like a line break every 70 chars (legacy stuff), I'll
        // put one every 5 pixels (60 chars worst case scenario, 20 best)
        #define LINE_BREAK 5
        for (int i = 0; i < c.w * c.h; i++) {
                char end = (i % LINE_BREAK == (LINE_BREAK-1)) ? '\n' : ' ';
                ptr += sprintf(ptr, "%s%c", color_to_ppm_string(c.pixels[i]), end);
        }

        return data;   
}


/*
 * Export the current canvas to a ppm file with the given name
 * Returns 1 on success, 0 otherwise
 */
const int export_to_ppm(const struct canvas c, const char *filename)
{
        char *header = _create_ppm_header(c);
        char *data = _create_ppm_data(c);

        FILE *file = fopen(filename, "w");
        if (file == NULL) {
                fprintf(stderr, "%s\n", strerror(errno));
                return 0;
        }

        fprintf(file, "%s%s", header, data);
        fclose(file);

        mem_free(header);
        mem_free(data);
        
        return 1;
}

#define BMP_FILE_INFO_SIZE 14
#define BMP_HEADER_SIZE 54

/*
 * BMP files start with a 14 header containing various info about the file
 */
static void *_create_bmp_file_header(const struct canvas c)
{        
        char *buf = mem_alloc(BMP_HEADER_SIZE);
        
        // signature
        memcpy(buf, "BM", 2);

        // the total size of the header for bitmaps varies, but this will
        // always use 54, rest of the file is 4 bytes per pixel (ARGB)
        uint32_t word32 = BMP_HEADER_SIZE + c.w * c.h * 4;
        memcpy(buf+2, &word32, sizeof(word32));
        word32 = 0;       // next 4 bytes must be 0
        memcpy(buf+6, &word32, sizeof(word32));
        word32 = BMP_HEADER_SIZE;      // offset to start of pixel data in file
        memcpy(buf+10, &word32, sizeof(word32));

        word32 = BMP_HEADER_SIZE - BMP_FILE_INFO_SIZE;
        memcpy(buf+BMP_FILE_INFO_SIZE, &word32, sizeof(word32));
        word32 = c.w;
        memcpy(buf+BMP_FILE_INFO_SIZE+4, &word32, sizeof(word32));
        word32 = c.h * -1;      // windows bmp writes stuff upside down...  
        memcpy(buf+BMP_FILE_INFO_SIZE+8, &word32, sizeof(word32));
        uint16_t wee_num = 1;   // biPlanes
        memcpy(buf+BMP_FILE_INFO_SIZE+12, &wee_num, sizeof(wee_num));
        wee_num = 32;   // bit per pixel
        memcpy(buf+BMP_FILE_INFO_SIZE+14, &wee_num, sizeof(wee_num));
        word32 = 0;       // compression (0 = none)
        memcpy(buf+BMP_FILE_INFO_SIZE+16, &word32, sizeof(word32));
        // image size, 0 for uncompressed images
        memcpy(buf+BMP_FILE_INFO_SIZE+20, &word32, sizeof(word32));
        // preferred resolution, optional
        memcpy(buf+BMP_FILE_INFO_SIZE+24, &word32, sizeof(word32));
        // preferred resolution, optional
        memcpy(buf+BMP_FILE_INFO_SIZE+28, &word32, sizeof(word32));
        // number of colours used, 0 for no restrictions
        memcpy(buf+BMP_FILE_INFO_SIZE+32, &word32, sizeof(word32));
        // significant colours (?), usually 0 apparently
        memcpy(buf+BMP_FILE_INFO_SIZE+36, &word32, sizeof(word32));

        return buf;
}

static void *_create_bmp_data(const struct canvas c)
{
        uint32_t *buf = mem_alloc(c.w * c.h * 4);
        for (int i = 0; i < c.w * c.h; i++) {
                uint32_t val = color_to_ARGB(c.pixels[i]);
                *(buf+i) = val;
        }

        return buf;
}

/* 
 * Write the contents of a canvas to a bitmap file - this is mostly
 * for testing and more robust, portable file i/o code should be
 * written for screenshots etc. Returns 1 on success, 0 on failure
 */
const int canvas_export_to_bmp(const struct canvas c, const char *filename)
{
        FILE *file = fopen(filename, "w");
        if (file == NULL) {
                fprintf(stderr, "Unable to create bmp\n");
                fprintf(stderr, "%s\n", strerror(errno));
                return 0;
        }

        void *bmp_head = _create_bmp_file_header(c);
        void *bmp_data = _create_bmp_data(c);

        fwrite(bmp_head, BMP_HEADER_SIZE, 1, file);
        fwrite(bmp_data, (c.w * c.h * 4), 1, file);

        fclose(file);

        return 1;
}

/*
 * create a canvas from a bitmap file
 */
struct canvas canvas_from_bmp(const char *filename)
{
        FILE *file = fopen(filename, "r");
        if (file == NULL) {
                fprintf(stderr, "%s\n", strerror(errno));
                exit(0);
        }

        // get image properties
        fseek(file, 18, SEEK_SET);
        uint32_t width, height;
        fread(&width, 4, 1, file);
        fread(&height, 4, 1, file);
        height *= -1;   // windows stores bmps upside down

        // get color depth
        fseek(file, 28, SEEK_SET); 
        uint32_t color_depth;
        fread(&color_depth, 4, 1, file);
        color_depth /= 8;
        if (color_depth != 3 && color_depth != 4) {
                exit(0);
        }

        // get the pixels
        uint32_t file_pixel_offset;
        fseek(file, 10, SEEK_SET);
        fread(&file_pixel_offset, 4, 1, file);

        double r, g, b, a;
        uint32_t *data = (uint32_t *)mem_alloc(width * height * color_depth);
        fseek(file, file_pixel_offset, SEEK_SET);
        fread(data, sizeof(uint32_t), width * height, file);

        struct canvas image = canvas(width, height);

        for (int i = 0; i < width * height; i++) {

                uint32_t pixel;
                
                pixel = data[i];
                
                a = (double)((pixel >> 24) & 0xff) / 255.0;
                r = (double)((pixel >> 16) & 0xff) / 255.0;
                g = (double)((pixel >> 8) & 0xff) / 255.0;
                b = (double)((pixel) & 0xff) / 255.0;
        
                struct color color = color_rgb(r, g, b);
                image.pixels[i] = color;
        }

        mem_free(data);
        fclose(file);

        return image;
}
