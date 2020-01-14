Extending DGL
=============
DGL is designed to be easily extendable - in other words, you can add new features without modifying the engine itself. This can be, for example, a particle effect system, terrain renderer, animated water, etc. You have full access to OpenGL functions, so you can render virtually anything!

Let's start with something simple - a new geometric shape. DGL already provides a number of simplest shapes (box, sphere, cylinder, cone, ellipsoid), and we can extend this list. We a going to render a simple disk shape. Obviously, to understand this tutorial, you should have familiarity with OpenGL.

Any object that is meant to be drawn should be an implementation of `Drawable` interface from `dgl.core.interfaces`. This interface defines just one method, `void draw(double dt)`.

```d
import std.math;
import dlib.math.vector;
import dgl.core.api;
import dgl.core.interfaces;

class ShapeDisk: Drawable
{
    override void draw(double dt)
    {
        //...
    }
}
```

To fasten things up, we will render the disk as display list (this is just for the sake of simplicity and compatibility with legacy hardware - you better use VBOs):

```d
class ShapeDisk: Drawable
{
    uint displayList;
    
    this(float radius, uint steps)
    {
        // ...
    }

    override void draw(double dt)
    {
        glCallList(displayList);
    }
    
    ~this()
    {
        glDeleteLists(displayList, 1);
    }
}
```

Now the rendering code in constructor:

```d
this(float radius, uint steps)
{
    displayList = glGenLists(1);
    glNewList(displayList, GL_COMPILE);
    glBegin(GL_TRIANGLE_FAN);
    glNormal3f(0, 0, 1);
    glVertex3f(0, 0, 0);
    float stepAngle = (2 * PI) / steps;
    foreach(i; 0..steps+1)
    {
        float angle = i * stepAngle;
        Vector3f v = Vector3f(cos(angle) * radius, sin(angle) * radius, 0.0f);
        glVertex3fv(v.arrayof.ptr);
    }
    glEnd();
    glEndList();
}
```

Disk will face Z-axis. `step` is a number of vertices the disk is approximated with. Note that our code is fairly simple and imperfect - we haven't defined any texture coordinates. But this is something that can be easily done later.

Now we can create `ShapeDisk` object and attach it to entity:

```d
auto disk = New!ShapeDisk(5.0f, 20);
createEntity3D(disk);
```

That's it! Look into `dgl.graphics.shapes` to take a hint of how different shapes are created. Have fun extending DGL and rendering your own unique procedural graphics!
