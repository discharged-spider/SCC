#define PROGRAM_NAME "SCC_JIT"

//#define DEBUG

#include "SCC_JIT.h"

#include "cstdio"
#include "windows.h"

#include "..\\..\\SystemFolder\\_SystemLibs\\ConsoleApp\\ConsoleApp.h"

int main (int ArgN, char** ARG)
{
    if (ArgN == 1)
    {
        printf ("%s help.\n", PROGRAM_NAME);
        printf ("Arguments:\n");
        printf ("-i input_file_name (default = \"Program.scc_bin\")\n");
        printf ("-e error_output_file_name (default = \"Error.txt\")\n");
        exit (0);
    }

    #define ADDR ARG [0]
    string Way = GetMyWay (ADDR);
    string Temp = Way;

    char InputName [] = "Program.scc_bin";
    FILE* Input = NULL;
    FILE* ErrorOutput = NULL;

    try
    {
        for (int i = 1; i < ArgN; i ++)
        {
            if (strcmp (ARG[i], "-i") == 0)
            {
                i ++;
                if (i >= ArgN) throw TH_ERROR "Sudden end after argument.");

                if (Input) fclose (Input);
                Input = fopen (ARG[i], "rb");
            }
            if (strcmp (ARG[i], "-e") == 0)
            {
                i ++;
                if (i >= ArgN) throw TH_ERROR "Sudden end after argument.");

                if (ErrorOutput) fclose (ErrorOutput);
                ErrorOutput = fopen (ARG[i], "ab");
            }
        }

        if (!Input) Input = fopen (InputName, "rb");
        if (!Input) throw TH_ERROR "No input file!");

        if (!ErrorOutput) ErrorOutput = fopen ("Error.txt", "ab");

        int Mode = false;
        if (!fread (&Mode, sizeof (int), 1, Input)) throw TH_ERROR "Input is empty");

        if (Mode != 'J' + 'I' + 'T') throw TH_ERROR "I can run only scc_jit files (not scc_exe)");

        int ProgramSize = 0;
        vector <double> Program;

        Load (Input, Program, &ProgramSize);

        fclose (Input);

        unsigned int Size = 0;
        vector <unsigned char> BIN;

        stack <unsigned int> ToLink;

        Compile (&Program [0], ProgramSize, BIN, &Size, ToLink);

        Link (BIN, Size, ToLink, STANDART_LINK_FUNCTIONS);

        printf ("Size = %d\n", Size);

        ST_EXECUTE (BIN);

        fclose (Input);

        fclose (ErrorOutput);
    }
    catch (newThrowError& Error)
    {
        printf ("%s", Error.ErrorText_);

        if (!ErrorOutput) ErrorOutput = fopen ("Error.txt", "ab");
        fprintf (ErrorOutput, "%s", Error.ErrorText_);
    }
    catch (...)
    {
        printf ("Unknown error");
    }

    _fcloseall ();
}
