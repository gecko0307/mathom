func main()
{
    var arr = [6, 1, 4, 2, 8, 5, 10, 7];
    writeln(arr);
    writeln(arr:sort);
}

func sort(data)
{
    var len = data:length;
    var j = 0;
    var tmp = 0;

    var i = 0; 
    while(i < len)
    {
        j = i;
        var k = i; 
        while(k < len)
        {
            if (data[j] > data[k])
                j = k;
            k += 1;
        }
    
        tmp = data[i];
        data[i] = data[j];
        data[j] = tmp;

        i += 1;
    }

    return data;
}
