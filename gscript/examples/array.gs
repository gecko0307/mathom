import std.array;

func main()
{
    var x = [21, 4, 53, 6, 90];

    // this will append [0,0] to the end of x
    x = flatten(x, array(2));

    x[3] = 100;

    // this will assign 99 to x[0]
    modify(ref x[0]);

    printArrayElements(x);
}

func printArrayElements(arr)
{
    var i = 0;
    while(i < arr:length)
    {
        writeln(arr[i]);
        i = i + 1;
    }
}

func modify(x)
{
    x = 99;
}
