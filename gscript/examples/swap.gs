func swap(a, b)
{
    var tmp = a;
    a = b;
    b = tmp;
}

func main()
{
    var x = 5, y = 10;
    writeln(x, y);
    swap(ref x, ref y);
    writeln(x, y);
}
