// std.array
// Array processing

// In-place aray reverse 
func reverse(data)
{
    var left = 0;
    var right = data:length - 1;

    while (left < right)
    {
        var temp = data[left];
        data[left] = data[right];
        data[right] = temp;

        left += 1;
        right -= 1;
    }

    return data;
}

// In-place array sort
func sort(data)
{
    var len = data:length;
    var j = 0;
    var tmp = 0;

    foreach(var i,v in data)
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
    }

    return data;
}

// Returns a new array that is a one-dimensional 
// flattening of the input
func flatten(data)
{
    var res = [];
    var i = 0;

    foreach(var v in data)
    {
        if (v is Float)
            res ~= v;
        else if (v is String)
            res ~= v;
        else if (v is Array)
            res ~= flatten(v);
    }
    return res;
}

// Returns a multidimensional array with given sizes
func table(sizes)
{
    return _table(sizes, 0);
}

func _table(sizes, index)
{
    if (index == sizes:length)
        return 0;

    var res = array(sizes[index]);

    foreach(var i,v in res)
    {
        res[i] = _table(sizes, index+1);
    }

    return res;
}
