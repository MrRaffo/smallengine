#include <stdio.h>
#include <assert.h>

#include <SDL2/SDL.h>

#include <smallengine/se_sys.h>
#include <smallengine/se_graphics.h>

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
