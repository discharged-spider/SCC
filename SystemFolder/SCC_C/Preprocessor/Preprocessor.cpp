#define PROGRAM_NAME "SCC_Preprocessor"
#define TREE_VERSION 8

#include "Preprocessor.h"
#include "windows.h"

int main (int ArgN, char** ARG)
{
    try
    {
        if (ArgN != 3) throw TH_ERROR "%s: Arguments error! Arg form: InputFile Outputfile.\n");
    
        FILE* From = fopen (ARG [1], "rb");
        FILE* To   = fopen (ARG [2], "w+");
        
        if (!From) throw TH_ERROR "No input file!");
        if (!To)   throw TH_ERROR "No output file!");
        
        for (int i = strlen (ARG [1]) - 1; ARG [1][i] != '\\' && i > -1; i --) ARG [1][i] = 0;
        
        Preprocess (From, To, ARG [1]);
        
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
