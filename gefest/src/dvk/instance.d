module dvk.instance;

import std.stdio;
import std.string;
import std.conv;
import derelict.vulkan;

/*
 * DvkInstance - a class that incapsulates VkInstance and VkDevice
 */
class DvkInstance
{
    bool initialized;
    string name;
    const(char)* nameCPtr;
    uint appVersion;
    uint engineVersion;
    
    VkInstance instance;
    uint numExtensions;
    VkExtensionProperties[] extensionProps;
    
    uint numPhysicalDevices;
    VkPhysicalDevice[] physicalDevices;
    VkPhysicalDevice physicalDevice;
    
    uint numDeviceExtensions;
    VkExtensionProperties[] deviceExtensionProperties;
    
    VkPhysicalDeviceProperties gpuProps;
    string gpuName;
    uint gpuID;
    uint gpuVendorID;
    
    VkDevice device;
    VkQueue deviceQueue;
    
    this(string name, uint appVersion = 0, uint engineVersion = 0)
    {
        this.initialized = false;
        this.name = name;
        this.nameCPtr = toStringz(name);
        this.appVersion = appVersion;
        this.engineVersion = engineVersion;
        
        initialize();
    }
    
    ~this()
    {
        destroy();
    }
    
    void initialize()
    {
        if (initialized)
            return;
    
        // Enumerate extension properties
        vkEnumerateInstanceExtensionProperties(null, &numExtensions, null);
        debug writefln("numExtensions: %s", numExtensions);

        if (numExtensions > 0)
        {
            extensionProps = new VkExtensionProperties[numExtensions];
            vkEnumerateInstanceExtensionProperties(null, &numExtensions, extensionProps.ptr);
        }
    
        VkApplicationInfo appInfo =
        {
            sType: VkStructureType.VK_STRUCTURE_TYPE_APPLICATION_INFO,
            pNext: null,
            pApplicationName: nameCPtr,
            applicationVersion: appVersion,
            pEngineName: nameCPtr,
            engineVersion: engineVersion,
            apiVersion: VK_API_VERSION
        };
        
        // TODO: other platforms support
        string e1 = "VK_KHR_surface";
        string e2 = "VK_KHR_win32_surface";
        
        auto instExtNames =
        [
            toStringz(e1),
            toStringz(e2)
        ];

        VkInstanceCreateInfo instanceInfo =
        {
            sType: VkStructureType.VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
            pNext: null,
            flags: 0,
            pApplicationInfo: &appInfo,
            enabledLayerCount: 0,
            ppEnabledLayerNames: null,
            enabledExtensionCount: 2,
            ppEnabledExtensionNames: instExtNames.ptr
        };
        
        auto res = vkCreateInstance(&instanceInfo, null, &instance);
        
        debug writefln("vkCreateInstance: %s", res);
        
        if (res != VkResult.VK_SUCCESS)
            throw new Exception("vkCreateInstance failed");
            
        // Enumerate available GPUs
        res = vkEnumeratePhysicalDevices(instance, &numPhysicalDevices, null);
        debug writefln("numPhysicalDevices: %s", numPhysicalDevices);
        if (res != VkResult.VK_SUCCESS || numPhysicalDevices == 0)
            throw new Exception("No GPUs found");
            
        physicalDevices = new VkPhysicalDevice[numPhysicalDevices];
        res = vkEnumeratePhysicalDevices(instance, &numPhysicalDevices, physicalDevices.ptr);
        if (res != VkResult.VK_SUCCESS)
            throw new Exception("vkEnumeratePhysicalDevices failed");
        
        // TODO: select based on properties
        physicalDevice = physicalDevices[0];

        vkEnumerateDeviceExtensionProperties(physicalDevice, null, &numDeviceExtensions, null);
        debug writefln("numDeviceExtensions: %s", numDeviceExtensions);
        deviceExtensionProperties = new VkExtensionProperties[numDeviceExtensions];
        vkEnumerateDeviceExtensionProperties(physicalDevice, null, &numDeviceExtensions, deviceExtensionProperties.ptr);

        vkGetPhysicalDeviceProperties(physicalDevice, &gpuProps);
        gpuName = to!string(gpuProps.deviceName.ptr);
        gpuID = gpuProps.deviceID;
        gpuVendorID = gpuProps.vendorID;
        debug 
        {
            writefln("gpuVendorID: 0x%04X", gpuVendorID);
            writefln("gpuID: 0x%04X", gpuID);
            writefln("gpuName: %s", gpuName);
        }
        
        // Create a device
        string e3 = "VK_KHR_swapchain";
        
        auto devExtNames =
        [
            toStringz(e3)
        ];
        
        float[1] queuePriorities = [0.0f];
        VkDeviceQueueCreateInfo queueInfo =
        {
            sType: VkStructureType.VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
            pNext: null,
            queueFamilyIndex: 0,
            queueCount: 1,
            pQueuePriorities: queuePriorities.ptr
        };

        VkDeviceCreateInfo devInfo = 
        {
            sType: VkStructureType.VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
            pNext: null,
            queueCreateInfoCount: 1,
            pQueueCreateInfos: &queueInfo,
            enabledLayerCount: 0,
            ppEnabledLayerNames: null,
            enabledExtensionCount: 1,
            ppEnabledExtensionNames: devExtNames.ptr,
            pEnabledFeatures: null
        };
        
        res = vkCreateDevice(physicalDevice, &devInfo, null, &device);
        debug writefln("vkCreateDevice: %s", res);
        assert(res == VkResult.VK_SUCCESS);
        
        vkGetDeviceQueue(device, 0, 0, &deviceQueue);
        
        initialized = true;
    }
    
    void destroy()
    {
        if (initialized)
        {
            vkDestroyInstance(instance, null);
            initialized = false;
        }
    }
}
