#define PROGRAM_NAME "SCC_Processor"

//#define DEBUG
#include "Processor.h"
#include <time.h>

#define GetPTinMS ((double)clock() * (double)1000 / (double)CLOCKS_PER_SEC)

void PrintfHelp ();
void SetConfig (FILE* Config, newProcessor& Processor);
void Dump (newProcessor& Processor, double Time);

int main (int argc, char** argv)
{
    try
    {
        const int MCS = 30;

        if (argc == 1 || argc > 3)
        {
            PrintfHelp ();

            return 0;
        }

        char ProgramName [MCS] = "";
        char ConfigName  [MCS] = "";

        if (argc == 2)
        {
            if (strlen (argv [1]) + 1 > (int)MCS) throw TH_ERROR "File name too big");
            else                            sprintf (ProgramName, "%s", argv [1]);

            sprintf (ConfigName, "Config.dat");

            FILE* ConfigFile = fopen (ConfigName, "r");
            if (!ConfigFile)
            {
                ConfigFile = fopen (ConfigName, "w");

                fprintf (ConfigFile, "0");
            }

            fclose (ConfigFile);
        }
        else if (argc == 3)
        {
            if (strlen (argv [1]) + 1 > (int)MCS) throw TH_ERROR "File name too big");
            else                            sprintf (ProgramName, "%s", argv [1]);

            if (strlen (argv [2]) + 1 > (int)MCS) throw TH_ERROR "File name too big");
            else                            sprintf (ConfigName, "%s", argv [2]);
        }
        else throw TH_ERROR "Invalid params");

        FILE* ConfigFile  = fopen (ConfigName,  "r");
        FILE* ProgramFile = fopen (ProgramName, "r");

        if (!ConfigFile)  throw TH_ERROR "Config file doesn't exist.");
        if (!ProgramFile) throw TH_ERROR "Input file doesn't exist.");

        newProcessor Processor;

        newVector <double> Program;
        int Size = 0;

        SetConfig (ConfigFile, Processor);

        int Mode = false;
        if (!fread (&Mode, sizeof (int), 1, ProgramFile)) throw TH_ERROR "Input is empty");

        if (Mode != 'P' + 'R' + 'O' + 'C') throw TH_ERROR "I can run only scc_proc files (not scc_jit)");

        CreateProcessorProgram (ProgramFile, Program, &Size);
        Processor.SetProgram (&Program [0], Size);

        fclose (ConfigFile);
        fclose (ProgramFile);

        printf ("__Processor__\n");

        printf ("Out:\n");
        double TimeStart = GetPTinMS;
        printf ("<------------>\n");
        Processor.ExecuteProgram ();
        printf ("\n<------------>\n");
        double TimeEnd = GetPTinMS;

        Dump (Processor, TimeEnd - TimeStart);
    }

    catch (newThrowError& Error)
    {
        printf ("__Processor__\n");
        printf ("%s", Error.ErrorText_);
    }
    catch (...)
    {
        printf ("I don't know, what they said!");
    }
}

void PrintfHelp ()
{
    printf ("___Help___\n");
    printf ("Params: ConfigFileName ProgramFileName\n");
    printf ("ProgramFileName  - Necessarily. Use assembler to compile.\n");
    printf ("ConfigFileName - Optional. If not used config file name equals Config.dat. If not exist, will be created automaticaly. Contains params for memory (default = rand) and pushes for stack(comand = push), going after them.\n");

    printf ("Comands:\n");
    #define COMAND_DEF(NUM, NAME, DESCRIPTOR, PARAMS, CODE) printf ("%d need %d params after.\n", DESCRIPTOR, PARAMS);
    #include "..\Syntax\ComandDef.h"
    #undef COMAND_DEF
}
void SetConfig (FILE* Config, newProcessor& Processor)
{
    for (int i = 0; true; i ++)
    {
        if (fscanf (Config, "%lg", &Processor.Memory_[i]) == EOF) break;
    }

    char Word [35] = "";
    while (true)
    {
        if (!ReadWord (Config, Word)) break;
        if (strcmp (Word, "push") == 0)
        {
            double n = 0;
            if (!fscanf (Config, "%lg", &n)) throw TH_ERROR "No number after push!");

            Processor.Stack_.Push (n);
        }
    }
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
