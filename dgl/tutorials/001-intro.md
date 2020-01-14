1. Introduction to DGL
======================
DGL is an Open Source graphics engine for D language. It is built on top of OpenGL and SDL, so it works on different platforms: Windows, Linux, and OSX. DGL can be used to create any type of 3D or 2D game (development is mainly focused on 3D graphics, though).

Features
--------
DGL provides an easy to use APIs for graphics I/O (window creation and input handling), adding geometry, creating materials, shaders and textures, loading models and scenes from files, and manipulating everything in real time.

Recommended system requirements
-------------------------------
* Intel Core i3 3 GHz
* 4 Gb RAM
* OpenGL 2.0 capable graphics card
* Windows XP or above, Linux x86/x86_64 or OSX.

Basic principles
----------------
The engine is written in somewhat specific way: it avoids using D's garbage collector, allocating all dynamic memory via malloc-based allocators. This is necessary for the user code to be predictable: you are fully controlling how and when your application allocates and releases memory (usually this would happen between game states or levels). This approach imposes a known portion of responsibility to programmer and requires some coding discipline, so, if you don't care so much about memory control and got used to conventional D-ish way of doing things, DGL would be frustrating and may be not for you. You can try to use DGL together with GC, but remember that, while theoretically possible, this is not properly tested and can lead to all sorts of weird bugs.

Nevertheless, DGL is trying to be programmer-friendly as much as possible, providing simple to use template functions and convenient APIs. Most of them are based on dlib, which includes memory-related tools. Instancing a class in unmanaged memory and deleting it looks like this:

```d
import dlib.core.memory;

MyObject obj = New!MyObject();
Delete(obj);
```

Similar `New` function is used for arrays as well:

```d
int[] arr = New!(int[])(100);
Delete(arr);
```

The main difference is that arrays are not dynamic (in the sence that they cannot be easily resized). So, instead of D's built-in arrays, you should use dlib.container.array:

```d
import dlib.container.array;

DynamicArray!int arr;
arr.append(5);
arr.append(10);

arr[0] = 15;
writeln(arr[0]);

foreach(i, v; arr)
    writeln(v);
    
// D array still can be retrieved to interoperate with Phobos and other foreign code.
// Warning: it is unmanaged, use with care!
int[] dArr = arr.data;
    
arr.free();
```

Because `DynamicArray` is not a class but a struct, it doesn't need to be deallocated via `Delete`, but it still allocates unmanaged dynamic memory internally, so it is mandatory to call the `free` method when the array is not needed anymore.

Hello, World!
-------------
The following is a code for a simple DGL application. It opens a window, loads TrueType font and prints "Hello, World" at lower-left corner:

```d
module minimal;

import dlib.core.memory;
import dgl.core.api;
import dgl.core.event;
import dgl.core.application;
import dgl.templates.app3d;
import dgl.ui.ftfont;
import dgl.ui.textline;

class SimpleApp: Application3D
{
    FreeTypeFont font;
    TextLine text;
    
    this()
    {
        super();
        
        font = New!FreeTypeFont("DroidSans.ttf", 20);
        
        text = New!TextLine(font, "Hello, World!");
        auto entityText = createEntity2D(text);
        entityText.position.x = 10;
        entityText.position.y = 10;
    }
    
    ~this()
    {
        Delete(text);
        Delete(font);
    }

    override void onKeyDown(int key)
    {
        if (key == SDLK_ESCAPE)
        {
            exit();
        }
    }
}

void main(string[] args)
{
    initDGL();
    auto app = New!SimpleApp();
    app.run();
    Delete(app);
    deinitDGL();
}
```

`TextLine` object supports non-ASCII characters (if the font contains them). This doesn't require any additional configuration - just pass your UTF-8 string and enjoy! Text rendering system is lazy: `FreeTypeFont` object loads new Unicode characters on demand, so no wasting memory with unnecessary data.

Running
-------
To run DGL applications, a number of shared libraries are required. These are OpenGL and GLU (they are usually installed system-wide; if not, update your video card driver), SDL 1.2, Freetype. Latter two are expected to be in `lib` directory with the application. On Linux, if you want to use SDL and Freetype installed on your system, you can set `USE_SYSTEM_LIBS=1` environment variable before running the application. Currently we provide only Windows and Linux versions of the libraries.

Configuration
-------------
Video resolution, fullscreen/windowed, VSync, and other options are controlled via configuration file called `game.conf` in application directory. The following is an example of a possible configuration:

    videoWidth: 1280;
    videoHeight: 720;
    videoWindowed: 1;
    videoVSync: 1;
    videoAntialiasing: 0;
    windowResizable: 1;

    fxShadersEnabled: 1;
    fxShadowEnabled: 1;
    fxShadowMapSize: 1024;
 
Any new game-specific configuration keys can be introduced, you can handle them with corresponding API.
