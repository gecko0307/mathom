module main;

import core.sys.windows.windows;
import std.stdio;
import std.string;
import derelict.sdl2.sdl;
import derelict.vulkan;
import vkctx;
 
void main()
{
    DerelictSDL2.load();
    DerelictVulkan.load();
 
    uint width = 800;
    uint height = 600;
    string caption = "Vulkan Test";

    SDL_Init(SDL_INIT_EVERYTHING);
    auto window = SDL_CreateWindow(toStringz(caption), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN);

    SDL_SysWMinfo winInfo;
    SDL_GetWindowWMInfo(window, &winInfo);
    auto hwnd = winInfo.info.win.window;

    VulkanContext vkCtx = new VulkanContext("Vulkan Test", width, height, hwnd);
    
    bool running = true;
    SDL_Event event;
    while(running)
    {
        if (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
                running = false;
        }
        
        vkCtx.render();
    }
    
    vkCtx.destroy();
    SDL_Quit();
}
