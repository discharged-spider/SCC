#define PROGRAM_NAME "SCC_Asm_Compiler"
/**/
#include "Assem.h"
#include "windows.h"
#include "io.h"

#include "..\\..\\SystemFolder\\_SystemLibs\\ConsoleApp\\ConsoleApp.h"

int main (int ArgN, char** ARG)
{
    if (ArgN == 1)
    {
        printf ("%s help.\n", PROGRAM_NAME);
        printf ("Arguments:\n");
        printf ("-i input_file_name (default = \"Program.scc_asm\")\n");
        printf ("-o output_file_name (default = \"Out.scc_exe\" for scc_proc or \"Out.scc_bin\" for scc_jit)\n");
        printf ("-e error_output_file_name (default = \"Error.txt\")\n");
        printf ("-p (use scc_processor mode, default = scc_jit mode)\n");
        printf ("-j (use scc_jit mode, default)\n");
        exit (0);
    }

    #define ADDR ARG [0]
    string Way = GetMyWay (ADDR);
    string Temp = Way;

    int InputNameNumber = 0;
    char InputName [] = "Program.scc_asm";
    FILE* Input = NULL;
    FILE* Output = NULL;
    FILE* ErrorOutput = NULL;
    bool JITMode = true;

    try
    {
        //MODULES CHECK
        if (!ModuleExist ("..\\..\\SystemFolder\\SCC_C\\Preprocessor\\Preprocessor.exe", Way)) throw TH_ERROR "One of the nessesary modules missing");

        for (int i = 1; i < ArgN; i ++)
        {
            if (strcmp (ARG[i], "-i") == 0)
            {
                i ++;
                if (i >= ArgN) throw TH_ERROR "Sudden end after argument.");

                InputNameNumber = i;
            }
            if (strcmp (ARG[i], "-o") == 0)
            {
                i ++;
                if (i >= ArgN) throw TH_ERROR "Sudden end after argument.");

                if (Output) fclose (Output);
                Output = fopen (ARG[i], "wb");
            }
            if (strcmp (ARG[i], "-e") == 0)
            {
                i ++;
                if (i >= ArgN) throw TH_ERROR "Sudden end after argument.");

                if (ErrorOutput) fclose (ErrorOutput);
                ErrorOutput = fopen (ARG[i], "ab");
            }
            if (strcmp (ARG[i], "-p") == 0)
            {
                JITMode = false;
            }
            if (strcmp (ARG[i], "-j") == 0)
            {
                JITMode = true;
            }
        }

        if (access ((InputNameNumber == 0)? InputName : ARG [InputNameNumber], 0) == -1) throw TH_ERROR "No input file.");

        Temp = Way;
        Temp += "..\\..\\SystemFolder\\SCC_C\\Preprocessor\\Preprocessor.exe ";
        Temp += (InputNameNumber == 0)? InputName : ARG [InputNameNumber];
        Temp += " Program.scc_pre_asm";

        system (Temp.c_str ());

        Input = fopen ("Program.scc_pre_asm", "rb");

        if (!Input) throw TH_ERROR "Unknown preprocessor error.");

        if (!Output) Output = fopen ((JITMode)? "Out.scc_bin" : "Out.scc_exe", "wb");

        if (!ErrorOutput) ErrorOutput = fopen ("Error.txt", "ab");

        vector <double> InputProgram;
        int InputSize = 0;

        if (!ReadProgram (Input, InputProgram, &InputSize, ErrorOutput)) throw TH_ERROR "Error in program, look error file.");

        fclose (Input);
        remove ("Program.scc_pre_asm");

        int Mode = (JITMode)? Syntax::JIT_MODE : Syntax::PROC_MODE;
        fwrite (&Mode, sizeof (int), 1, Output);

        if (JITMode)
        {
            if (!JITPrepare (InputProgram, &InputSize, ErrorOutput)) throw TH_ERROR "Error in program, look error file.");

            WriteProgram (Output, &InputProgram[0], InputSize);
        }
        else
        {
            vector <double> Program;
            int Size = 0;
            if (!Assemble (InputProgram, InputSize, Program, &Size, ErrorOutput)) throw TH_ERROR "Error in program, look error file.");

            WriteProgram (Output, &Program[0], Size);
        }
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

    remove ("Program.scc_pre_c");
    //_fcloseall ();

    return 0;
}
