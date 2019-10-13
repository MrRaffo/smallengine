/*
 * A first attempt at creating a text-based console or terminal style
 * program. Will be used to create the console part of the engine which
 * will be used to change values and settings at run time.
 */

#include <stdio.h>

#include <smallengine/maths/maths.h>
#include <smallengine/sys/mem.h>
#include <smallengine/sys/timer.h>
#include <smallengine/graphics/renderer.h>
#include <smallengine/graphics/canvas.h>
#include <smallengine/graphics/color.h>

int main(int argc, char *argv[])
{
        mem_init(100 * MEM_MEGABYTE);

        // load assets
        //      -- how to handle this? should renderer data such as fonts
        //      be loaded by the renderer, or loaded by an assets module
        //      and passed to the renderer separately?

        renderer_init("Console", 512, 256, 256, 128);
        struct canvas screen_canvas = renderer_get_window_canvas();
        canvas_test(screen_canvas);

        renderer_update_display();

        SDL_Delay(1000);

        // prepare input detection

        // create rendering context

        // enter main loop with timer

        renderer_quit();

        mem_destroy();

        return 0;
}
