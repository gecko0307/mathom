func main()
{
    var str = "abcçйцъ";

    assert(str is String);

    writeln(str[4]);

    str ~= "ЫЖ";
    
    writeln(str[$-1]);
    writeln(str:length);

    foreach(var i, v in str)
    {
        writeln(v);
    }

    assert("йцъ" in str);
}
