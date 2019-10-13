#include <stdio.h>
#include <assert.h>

#include <SDL2/SDL.h>

#include <smallengine/sys/mem.h>
#include <smallengine/graphics/renderer.h>
#include <smallengine/graphics/canvas.h>
#include <smallengine/sys/timer.h>

void TST_RenderInit()
{
        renderer_init("RenderTest", 256, 256, 128, 128);

        struct canvas new = renderer_new_canvas();
        assert(new.w == 128);
        assert(new.h == 128);

        printf("[Render Init] Complete, all tests pass!\n");
}        
        
int main()
{
        mem_init(100 * MEM_MEGABYTE);

        TST_RenderInit();

        SDL_VideoQuit();
        SDL_Quit();
        mem_destroy();
}
