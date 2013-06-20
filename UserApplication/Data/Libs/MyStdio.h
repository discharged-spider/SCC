new out (new str []) //Simple str out
{
    for (new i = 0; str [i] != 0; i = i + 1)
    {
        echo str [i]; //echo each symbol
    } 
}

new strcmp (new str1 [], new str2 [])
{
    new Result = 0;

    for (new i = 0; str1 [i] != 0 && str2 [i] != 0; i = i + 1)
    {
        if (str1 [i] != str2 [i]) Result = Result + 1;
    }

    return Result; //Don't try to hide it
}

new NumOut (new Num)
{
    new Start = false;
    for (new i = 9; i > -1; i = i - 1)
    {
        new b = 1;
        for (new j = 0; j < i; j = j + 1)
        {
            b = b * 10;
        }

        new c = (Num / b) % 10;

        if (Start || c != 0)
        {
            Start = true; 
            echo c + '0';
        }
    } 
} 
