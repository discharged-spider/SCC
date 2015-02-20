#define PROGRAM_NAME "SCC_OLYMPIAD_INFO_DECODER"

//#define DEBUG

#include "../Asm_Processor/Syntax/VariablesInspector.h"

#include "cstdio"
#include "windows.h"

#include "../SystemFolder/_SystemLibs/ConsoleApp/ConsoleApp.h"
#include "../SystemFolder/_SystemLibs/Throw/Throw.h"

using std::map;
using std::string;

void registerModes (map <string, void (*) (newVariablesInspector, FILE*)>& modes);

void mode_user_friendly (newVariablesInspector inspector, FILE* Output);
void mode_sum (newVariablesInspector inspector, FILE* Output);
void mode_max (newVariablesInspector inspector, FILE* Output);

int main (int ArgN, char** ARG)
{
    if (ArgN == 1)
    {
        printf ("%s help.\n", PROGRAM_NAME);
        printf ("Arguments:\n");
        printf ("-i input_file_name\n");
        printf ("-o output_file_name\n");
        printf ("-e error_output_file_name (default = \"Error.txt\")\n");
        printf ("-m mode (default = user_friendly)\n");
        exit (0);
    }

    #define ADDR ARG [0]
    string Way = GetMyWay (ADDR);
    string Temp = Way;

    FILE* Input = NULL;
    FILE* Output = NULL;
    FILE* ErrorOutput = NULL;

    string mode = "user_friendly";
    map <string, void (*) (newVariablesInspector, FILE*)> MODES;
    registerModes (MODES);

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
            if (strcmp (ARG[i], "-o") == 0)
            {
                i ++;
                if (i >= ArgN) throw TH_ERROR "Sudden end after argument.");

                if (Output) fclose (Output);
                Output = fopen (ARG[i], "ab");
            }
            if (strcmp (ARG[i], "-e") == 0)
            {
                i ++;
                if (i >= ArgN) throw TH_ERROR "Sudden end after argument.");

                if (ErrorOutput) fclose (ErrorOutput);
                ErrorOutput = fopen (ARG[i], "ab");
            }

            if (strcmp (ARG[i], "-m") == 0)
            {
                i ++;
                if (i >= ArgN) throw TH_ERROR "Sudden end after argument.");

                mode = ARG [i];
            }
        }

        if (!Input) throw TH_ERROR "No input file!");

        if (!ErrorOutput) ErrorOutput = fopen ("Error.txt", "ab");

        if (!MODES [mode]) throw TH_ERROR "Wrong mode!");

        newVariablesInspector inspector;
        inspector.load (Input);
        fclose (Input);

        MODES [mode] (inspector, Output);

        fclose (Output);

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

void registerModes (map <string, void (*) (newVariablesInspector, FILE*)>& modes)
{
    #define ADD(a) modes [#a] = mode_##a;

    ADD (user_friendly)
    ADD (sum)
    ADD (max)
}

void mode_user_friendly (newVariablesInspector inspector, FILE* Output)
{
    int MAX_VAR = 0;
    for (map <int, newVariableInfo>::iterator it = inspector.get_vars_begin (); it != inspector.get_vars_end (); it ++)
    {
        newVariableInfo info = it -> second;
        if (MAX_VAR < info.get_count + info.set_count) MAX_VAR = info.get_count + info.set_count;
    }
    int MAX_ARR = 0;
    for (map <int, newVariableInfo>::iterator it = inspector.get_arrs_begin (); it != inspector.get_arrs_end (); it ++)
    {
        newVariableInfo info = it -> second;
        if (MAX_ARR < info.get_count + info.set_count) MAX_ARR = info.get_count + info.set_count;
    }

    fprintf (Output, "vars = %d, arrs = %d\n", MAX_VAR, MAX_ARR);
}

void mode_sum (newVariablesInspector inspector, FILE* Output)
{
    int SUM_VAR = 0;
    for (map <int, newVariableInfo>::iterator it = inspector.get_vars_begin (); it != inspector.get_vars_end (); it ++)
    {
        newVariableInfo info = it -> second;
        if (SUM_VAR < info.get_count + info.set_count) SUM_VAR += info.get_count + info.set_count;
    }
    int SUM_ARR = 0;
    for (map <int, newVariableInfo>::iterator it = inspector.get_arrs_begin (); it != inspector.get_arrs_end (); it ++)
    {
        newVariableInfo info = it -> second;
        if (SUM_ARR < info.get_count + info.set_count) SUM_ARR += info.get_count + info.set_count;
    }

    fprintf (Output, "%d %d\n", SUM_VAR, SUM_ARR);
}

void mode_max (newVariablesInspector inspector, FILE* Output)
{
    int MAX_VAR = 0;
    for (map <int, newVariableInfo>::iterator it = inspector.get_vars_begin (); it != inspector.get_vars_end (); it ++)
    {
        newVariableInfo info = it -> second;
        if (MAX_VAR < info.get_count + info.set_count) MAX_VAR = info.get_count + info.set_count;
    }
    int MAX_ARR = 0;
    for (map <int, newVariableInfo>::iterator it = inspector.get_arrs_begin (); it != inspector.get_arrs_end (); it ++)
    {
        newVariableInfo info = it -> second;
        if (MAX_ARR < info.get_count + info.set_count) MAX_ARR = info.get_count + info.set_count;
    }

    fprintf (Output, "%d %d\n", MAX_VAR, MAX_ARR);
}
