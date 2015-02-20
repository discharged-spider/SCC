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
        printf ("-fi file_input_name - to get input by file\n");
        printf ("-fo file_output_name - to get input by file\n");
        printf ("-insp log_file_name - to enable vars and arrs get/set logging\n");
        exit (0);
    }

    #define ADDR ARG [0]
    string Way = GetMyWay (ADDR);
    string Temp = Way;

    char InputName [] = "Program.scc_bin";
    FILE* Input = NULL;
    FILE* ErrorOutput = NULL;
    FILE* VarsArrsLog = NULL;

    FILE* FInput = NULL;
    FILE* FOutput = NULL;

    try
    {
        for (int i = 1; i < ArgN; i ++)
        {
            if (strcmp (ARG[i], "-i") == 0)
            {
                i ++;
                if (i >= ArgN) throw TH_ERROR "Sudden end after argument.");

                PARAMS_FILE_OPEN (Input, ARG[i], "rb")
            }
            if (strcmp (ARG[i], "-e") == 0)
            {
                i ++;
                if (i >= ArgN) throw TH_ERROR "Sudden end after argument.");

                PARAMS_FILE_OPEN (ErrorOutput, ARG[i], "ab")
            }
            if (strcmp (ARG[i], "-insp") == 0)
            {
                i ++;
                if (i >= ArgN) throw TH_ERROR "Sudden end after argument.");

                PARAMS_FILE_OPEN (VarsArrsLog, ARG[i], "wb")
            }
            if (strcmp (ARG[i], "-fi") == 0)
            {
                i ++;
                if (i >= ArgN) throw TH_ERROR "Sudden end after argument.");

                PARAMS_FILE_OPEN (FInput, ARG[i], "rb")
            }
            if (strcmp (ARG[i], "-fo") == 0)
            {
                i ++;
                if (i >= ArgN) throw TH_ERROR "Sudden end after argument.");

                PARAMS_FILE_OPEN (FOutput, ARG[i], "wb")
            }
        }

        if (!Input) Input = fopen (InputName, "rb");
        if (!Input) throw TH_ERROR "No input file!");

        if (!ErrorOutput) ErrorOutput = fopen ("Error.txt", "ab");

        int Mode = 0;
        if (!fread (&Mode, sizeof (int), 1, Input)) throw TH_ERROR "Input is empty");

        if (Mode != Syntax::JIT_MODE) throw TH_ERROR "I can run only scc_jit files (not scc_exe)");

        JITLinkFunctions::input = FInput;
        JITLinkFunctions::output = FOutput;

        int ProgramSize = 0;
        vector <double> Program;

        Load (Input, Program, &ProgramSize);

        fclose (Input);

        unsigned int Size = 0;
        vector <unsigned char> BIN;

        stack <unsigned int> ToLink;

        Compile (&Program [0], ProgramSize, BIN, &Size, ToLink, VarsArrsLog != NULL);

        Link (BIN, Size, ToLink, STANDART_LINK_FUNCTIONS);

        printf ("Size = %d\n", Size);

        ST_EXECUTE (BIN);

        if (VarsArrsLog) JITLinkFunctions::Inspector.save (VarsArrsLog);

        fclose (ErrorOutput);
    }
    catch (newThrowError& Error)
    {
        printf ("%s", Error.ErrorText_);

        if (!ErrorOutput) ErrorOutput = fopen ("Error.txt", "ab");
        fprintf (ErrorOutput, "%s", Error.ErrorText_);
        fclose (ErrorOutput);
    }
    catch (...)
    {
        printf ("Unknown error");
    }

    //_fcloseall ();
    return 0;
}
