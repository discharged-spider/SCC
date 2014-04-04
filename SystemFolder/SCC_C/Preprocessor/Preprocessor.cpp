#define PROGRAM_NAME "SCC_Preprocessor"
#define TREE_VERSION 8

#include "..\\..\\_SystemLibs\\ConsoleApp\\ConsoleApp.h"
#include "Preprocessor.h"
#include "windows.h"

int main (int ArgN, char** ARG)
{
    try
    {
        if (ArgN != 3) throw TH_ERROR "Arguments error! Arg form: InputFile Outputfile.\n");

        FILE* From = fopen (ARG [1], "rb");
        FILE* To   = fopen (ARG [2], "w+");

        if (!From) throw TH_ERROR "No input file!");
        if (!To)   throw TH_ERROR "No output file!");

        #define ADDR ARG [1]
        string Way = GetMyWay (ADDR);

        Preprocess (From, To, Way.c_str ());

        fclose (From);
        fclose (To);
    }
    catch (newThrowError& Error)
    {
        printf ("%s", Error.ErrorText_);
    }
    catch (...)
    {
        printf ("Unknown error");
    }
}
