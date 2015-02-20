#define PROGRAM_NAME "SCC_Processor"

//#define DEBUG
#include "Processor.h"
#include <time.h>

#include "..\\..\\SystemFolder\\_SystemLibs\\ConsoleApp\\ConsoleApp.h"

#define GetPTinMS ((double)clock() * (double)1000 / (double)CLOCKS_PER_SEC)

void Dump (newProcessor& Processor, double Time);

int main (int ArgN, char** ARG)
{
if (ArgN == 1)
    {
        printf ("%s help.\n", PROGRAM_NAME);
        printf ("Arguments:\n");
        printf ("-i input_file_name (default = \"Program.scc_exe\")\n");
        printf ("-e error_output_file_name (default = \"Error.txt\")\n");
        printf ("-fi file_input_name - to get input by file\n");
        printf ("-fo file_output_name - to get input by file\n");
        printf ("-insp log_file_name - to enable vars and arrs get/set logging\n");
        exit (0);
    }

    #define ADDR ARG [0]
    string Way = GetMyWay (ADDR);
    string Temp = Way;

    char InputName [] = "Program.scc_exe";
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

        if (Mode != Syntax::PROC_MODE) throw TH_ERROR "I can run only scc_proc files (not scc_jit)");

        newVector <double> Program;
        int Size = 0;

        CreateProcessorProgram (Input, Program, &Size);

        fclose (Input);

        newProcessor Processor;
        Processor.SetProgram (&Program [0], Size);

        if (VarsArrsLog) Processor.UseInspector_ = true;
        Processor.input = FInput;
        Processor.output = FOutput;

        printf ("__Processor__\n");

        printf ("Out:\n");
        double TimeStart = GetPTinMS;
        printf ("<------------>\n");
        Processor.ExecuteProgram ();
        printf ("\n<------------>\n");
        double TimeEnd = GetPTinMS;

        Dump (Processor, TimeEnd - TimeStart);

        if (VarsArrsLog) Processor.Inspector_.save (VarsArrsLog);

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

void Dump (newProcessor& Processor, double Time)
{
    printf ("__Info__\n");
    printf ("Program done for %lg ms.\n", Time);

    /*printf ("Memory(used):\n");
    for (int i = 0; i < Processor.Memory_.GetSize (); i ++)
    {
        printf ("[%d] %lg\n", i, Processor.Memory_[i]);
    }
    printf ("\n");

    printf ("Stack:\n");

    newStack<double> PrintfCopy;

    if (Processor.Stack_.GetCellNum () == 0) printf ("Empty.\n");
    while (Processor.Stack_.GetCellNum () > 0)
    {
        PrintfCopy.Push (Processor.Stack_.Pop ());
    }
    while (PrintfCopy.GetCellNum () > 0)
    {
        printf ("%lg\n", PrintfCopy.Pop ());
    }*/
}
