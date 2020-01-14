func main()
{
    var arr = [3, 6, 7, 2, 1];

    foreach (var i, v in arr)
    {
        writeln(v);
    }

    assert(7 in arr);

    // This will fail:
    //assert(10 in arr);
}
