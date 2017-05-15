print("Outside of all functions!\n");

function foo(i, f, s)
{
    print("Function call with arguments, i="+i+", f="+f+", s='"+s+"'\n");
    print("Now i will fizzbuzz with i\n");

    for(local si = 0; si < i; si++)
    {
        local printed = false;

        if(si % 3 == 0)
        {
            print("Fizz");
            printed = true;
        }

        if(si % 5 == 0)
        {
            print("Buzz");
            printed = true;
        }

        if(printed)
        {
            print("\n");
        }
    }

}


function asd()
{
	print("Squirrel function call without arguments!\n");
}
