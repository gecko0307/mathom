2. Basic 3D Graphics
====================
Creating 3D applications with DGL is really easy. There are a number of built-in 3D objects which will help you to get started with something drawn on a screen. Moreover, DGL includes an easy to use freeview mode with trackball style camera (that is, a camera that you can rotate around center and zoom in/out, similar to those found in 3D model viewers).

Here is the code for an application that draws a cube, allowing the user to rotate the camera with MMB, translate it with Shift+MMB, and zoom with Ctrl+MMB or mouse wheel:

```d
import dlib.core.memory;
import dlib.math.vector;
import dgl.core.api;
import dgl.core.event;
import dgl.core.application;
import dgl.templates.app3d;
import dgl.templates.freeview;
import dgl.graphics.shapes;

class Simple3DApp: Application3D
{
    Freeview freeview;
    ShapeBox box;
    
    this()
    {
        super();
        
        freeview = New!Freeview(eventManager);
        
        box = New!ShapeBox(Vector3f(1, 1, 1));
        createEntity3D(box);
        
        addPointLight(Vector3f(3, 3, 3));
    }
    
    ~this()
    {
        Delete(freeview);
        Delete(box);
    }
    
    override void onUpdate(double dt)
    {
        super.onUpdate(dt);
        
        freeview.update();
        setCameraMatrix(freeview.getCameraMatrix());
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
    auto app = New!Simple3DApp();
    app.run();
    Delete(app);
    deinitDGL();
}
```

There are two main concepts in DGL: `Drawable` and `Entity`. `Drawable` (defined in dlib.core.interfaces) is an interface for anything that can be drawn (3D model, sprite, text, etc). It defines a single method, `void draw(double dt)`. `ShapeBox` in our example is an implementation of `Drawable` that draws a box with given half size. `Drawable` usually doesn't apply any transformation to the model being drawn. Transformation is stored in `Entity` class, which is the main way to add objects to our world. `Entity` has a `Drawable` associated with it, so multiple objects in your game can share the same 3D model. `Entity` calculates (or takes as input) a transformation matrix and applies it to the drawn model.

Note: in some places in DGL source code `Drawable` variables are called `model`, which may be confusing, but they have essentially the same meaning.
