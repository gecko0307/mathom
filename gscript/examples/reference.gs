func main()
{
    // Variable reference
    var a = 10;
    var b = ref a;
    b += 5;
    assert(a == 15);

    // This will fail due to circular reference:
    //b = ref b;

    // Function reference
    var f = ref foo;
    assert(f is Function);

    var res = f(10);
    assert(res == 100);
}

func foo(x)
{
    return x * x;
}

