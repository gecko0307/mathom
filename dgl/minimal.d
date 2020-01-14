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
        
        font = New!FreeTypeFont("data/DroidSans.ttf", 20);
        
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

