module textio;

import std.conv;
import dlib.core.memory;
import dlib.image.color;
import dgl.core.api;
import dgl.core.event;
import dgl.core.application;
import dgl.templates.app3d;
import dgl.ui.ftfont;
import dgl.ui.textline;

class TextListener: EventListener
{
    dchar[100] arr;
    string str;
    uint pos = 0;

    this(EventManager emngr)
    {
        super(emngr);
    }

    override void onTextInput(dchar code)
    {
        if (pos < 100)
        {
            arr[pos] = code;
            pos++;
        }
    }

    override void onKeyDown(int key)
    {
        if (key == SDLK_BACKSPACE)
            back();
        str = toString();
    }

    void reset()
    {
        arr[] = 0;
        pos = 0;
    }

    void back()
    {
        if (pos > 0)
            pos--;
    }

    override string toString()
    {
        return to!string(arr[0..pos]);
    }
}

class TextIOApp: Application3D
{
    FreeTypeFont font;
    TextLine text;
    TextListener tlistener;

    this()
    {
        super();
        
        font = New!FreeTypeFont("data/DroidSans.ttf", 20);
        
        text = New!TextLine(font, "Hello, World!");
        text.color = Color4f(1.0f, 1.0f, 1.0f, 1.0f);

        auto entityText = createEntity2D(text);
        entityText.position.x = 10;
        entityText.position.y = 10;
        
        tlistener = New!TextListener(eventManager);
        text.setText(tlistener.str);
    }

    ~this()
    {
        Delete(tlistener);
        Delete(text);
        Delete(font);
    }
    
    override void onUpdate(double dt)
    {
        super.onUpdate(dt);
        tlistener.processEvents();
        text.setText(tlistener.str);
    }

    override void onKeyDown(int key)
    {
        if (key == SDLK_ESCAPE)
        {
            exit();
        }
    }
}

void main()
{
    initDGL();
    auto app = New!TextIOApp();
    app.run();
    Delete(app);
    deinitDGL();
}

