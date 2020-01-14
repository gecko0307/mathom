// std.string
// String manipulation functions

// Makes string out of array
func string(data)
{
    var res = "";
    if (data is Array)
    {
        foreach(var v in data)
        {
            if (v is Float)
                res ~= v;
            else if (v is String)
                res ~= v;
            else if (v is Array)
                res ~= string(v);
        }
    }
    else
        res ~= data;
    return res;
}

// Makes character array from a string
func charr(str)
{
    var res = [];
    assert(str is String);

    foreach(var c in str)
        res ~= c;

    return res;
}
