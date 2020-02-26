module dvk.surface;

import core.sys.windows.windows;
import std.stdio;
import derelict.vulkan;
import dvk.instance;

/*
 * DvkSurface - a class that incapsulates VkSurfaceKHR
 */
abstract class DvkSurface
{
    VkSurfaceKHR surface;
    uint graphicsQueueNodeIndex;
    uint presentQueueNodeIndex;
}

/*
 * DvkWindowsSurface - DvkSurface implementation for Windows
 */
class DvkWindowsSurface: DvkSurface
{
    DvkInstance dvkInstance;
    HWND window;
    HINSTANCE appInstance;
    
    this(DvkInstance inst, HWND hwnd)
    {
        this.dvkInstance = inst;
        this.window = hwnd;
        this.appInstance = cast(HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE);
        
        initialize();
    }
    
    ~this()
    {
        destroy();
    }
    
    void initialize()
    {
        // Create a WSI surface for the window
        VkWin32SurfaceCreateInfoKHR createInfo = 
        {
            sType: VkStructureType.VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR,
            pNext: null,
            flags: 0,
            hinstance: appInstance,
            hwnd: window
        };

        auto res = vkCreateWin32SurfaceKHR(dvkInstance.instance, &createInfo, null, &surface);
        assert(res == VkResult.VK_SUCCESS);
        
        uint queueCount;
        
        vkGetPhysicalDeviceQueueFamilyProperties(dvkInstance.physicalDevice, &queueCount, null);
        debug writefln("queueCount: %s", queueCount);
        assert(queueCount >= 1);
        
        // Iterate over each queue to learn whether it supports presenting:
        VkBool32[] supportsPresent = new VkBool32[queueCount];
        for (uint i = 0; i < queueCount; i++)
        {
            vkGetPhysicalDeviceSurfaceSupportKHR(dvkInstance.physicalDevice, i, surface, &supportsPresent[0]);
        }

        debug writefln("supportsPresent: %s", supportsPresent);
        
        auto queue_props = new VkQueueFamilyProperties[queueCount];
        vkGetPhysicalDeviceQueueFamilyProperties(dvkInstance.physicalDevice, &queueCount, queue_props.ptr);
        
        graphicsQueueNodeIndex = uint.max;
        presentQueueNodeIndex = uint.max;
        
        for (uint i = 0; i < queueCount; i++)
        {
            if (queue_props[i].queueFlags & VkQueueFlagBits.VK_QUEUE_GRAPHICS_BIT)
            {
                if (graphicsQueueNodeIndex == uint.max)
                    graphicsQueueNodeIndex = i;

                if (supportsPresent[i] == VK_TRUE)
                {
                    graphicsQueueNodeIndex = i;
                    presentQueueNodeIndex = i;
                    break;
                }
            }
        }
        debug writefln("graphicsQueueNodeIndex: %s", graphicsQueueNodeIndex);
        
        if (presentQueueNodeIndex == uint.max)
        {
            for (uint i = 0; i < queueCount; ++i)
            {
                if (supportsPresent[i] == VK_TRUE)
                {
                    presentQueueNodeIndex = i;
                    break;
                }
            }
        }
        debug writefln("presentQueueNodeIndex: %s", presentQueueNodeIndex);
        
        //assert(graphicsQueueNodeIndex != uint.max && 
        //       presentQueueNodeIndex != uint.max && 
        //       graphicsQueueNodeIndex == presentQueueNodeIndex);
    }
    
    void destroy()
    {
        vkDestroySurfaceKHR(dvkInstance.instance, surface, null);
    }
}
