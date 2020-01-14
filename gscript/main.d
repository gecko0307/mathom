module main;

import std.stdio;
import std.file;
import std.path;
import std.conv;

import gscript.gs;
import gscript.dynamic;
import gscript.vm;

import std.process;

Dynamic hello(VirtualMachine vm, Dynamic[] args)
{
    writeln("Hello!");
    return Dynamic(0);
}

void main(string[] args)
{
    string filename;
    string text;

    if (args.length > 1)
    {
        filename = args[1];
        text = readText(filename);
    }
    else
    {
        writeln("Please, specify program file");
        return;
    }

    GScript gs = new GScript();
    gs.expose("hello", 0, &hello);
    auto script = gs.loadScript(text, filename);

    Dynamic gsArgs = Dynamic(Type.Array);
    if (args.length > 2)
        foreach(a; args[2..$])
            gsArgs.asArray ~= Dynamic(a.to!float);

    //writeln(script.code);

    assert(script.hasLocalFunction("main"));
    gs.executeFunction(script, "main", [gsArgs]);

    version(Windows) 
        std.process.system("pause");
}

