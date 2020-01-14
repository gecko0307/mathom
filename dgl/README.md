DGL
===
DGL is a minimalistic 3D and 2D real-time graphics engine written in D language and built on top of OpenGL and SDL.

> **Important:** DGL is not maintained anymore and won't get any updates. Please, consider using [Dagon](https://github.com/gecko0307/dagon) - a fork of DGL which is based on modern technologies, has more features, and is much easier to use.

Screenshots
-----------
[![Screenshot1](/screenshots/005_thumb.jpg)](/screenshots/005.jpg)
[![Screenshot2](/screenshots/007_thumb.jpg)](/screenshots/007.jpg)
[![Screenshot2](/screenshots/009_thumb.jpg)](/screenshots/009.jpg)
[![Screenshot2](/screenshots/010_thumb.jpg)](/screenshots/010.jpg)

To see what DGL is capable to, check out [Atrium](https://github.com/gecko0307/atrium), a work-in-progress sci-fi first person puzzle based on physics simulation.

Features
--------
* Fully GC-free
* Supports Windows, Linux, OSX and FreeBSD
* Event system with user-defined events and Unicode keyboard input
* Resource manager with threaded loading 
* Own scene file format (DGL3) with Blender exporter
* Loading textures from PNG
* Loading materials from text files with simple human-editable markup
* Dynamic soft shadows
* Unlimited number of dynamic light sources
* GLSL shaders
* Normal mapping and parallax mapping
* Image-based lighting
* Physically based rendering (PBR)
* Unlimited number of render passes, 2D or 3D
* Render to texture
* Antialiasing
* Built-in trackball camera
* 3D geometric shapes
* 2D sprites, including animated ones
* 2D text rendering with TTF fonts and Unicode support
* VFS
* Configuration system

TODO:
* Billboards
* Actors, IQM format loading
* Particle system
* I8n
* Terrain rendering
* Water rendering

Demos
-----
DGL comes with a number of usage examples. To build one, run `dub build --config=demoname`, where `demoname` can be `minimal`, `pbr`, `textio`.
* minimal.d - 'Hello, World' application, demonstrates how to create a window and print text with TrueType font
* pbr.d - physically-based rendering demo (also demonstrates how to use shadows)
* textio.d - text input demo, with support for international keyboard layouts.

Documentation
-------------
Warning: documentation can be outdated!
* [Introduction to DGL](/tutorials/001-intro.md)
* [Basic 3D Graphics](/tutorials/002-3d-graphics.md)
* [Model Loading](/tutorials/003-model-loading.md)
* [Extending DGL](/tutorials/004-extending-dgl.md)
* [Event System](/tutorials/005-event-system.md)

License
-------
Copyright (c) 2013-2016 Timur Gafarov. Distributed under the Boost Software License, Version 1.0 (see accompanying file COPYING or at http://www.boost.org/LICENSE_1_0.txt).
