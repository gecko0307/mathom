GScript
=======
GScript is a minimalistic imperative scripting language and accompanying VM. It's main targets are simplicity and emeddability: it would make a good choise as a lightweight embedded scripting engine in bigger applications, where even Lua would be an overkill. Since the implementation is pretty  straightforward, GScript may also be used for educational purposes.

Be aware that the project is in highly experimental state, though. The language is incomplete and the VM is just a prototype. The code may contain bugs.

Both GScript compiler and VM are written in D language. 

Features
--------
The language is, in some extent, inspired by D, but has a dynamic nature. The syntax is close to that of JavaScript. The following is a list of implemented features:

* D-like module system. Circular dependencies are handled correctly
* Dynamic type system with runtime introspection
* Functions. Recursive calls are supported
* Pass by value and pass by reference
* Variadic functions
* UFCS
* Heterogeneous arrays
* Strings with full Unicode support (UTF-8)
* String formatting (like in Python)
* Support for exposing native D functions to scripts
* Function and variable references
* "if...else" statements
* "for" and "foreach" loops
* "while" loop
* "do...while" loop

Example
-------
```
import std.array;

func main()
{
    var x = 10;
    var arr = [x, "foo", table(3)];

    assert(arr is Array);
    assert(arr:length == 3);

    foreach (var i, v in arr)
    {
        writeln("%0: %1" % (i, v));
    }

    var arr2 = arr:flatten;
    assert(arr2:length == 5);
    writeln(arr2);
}
```

Planned features
----------------

* Closures and lambda expressions
* Some kind of OOP

Standard library
----------------
GScript comes with a small collection of general-purpose code written in it:

* std.core - common algorithms
* std.array - array processing functions
* std.string - string manipulating functions

