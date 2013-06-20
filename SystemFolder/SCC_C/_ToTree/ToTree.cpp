#define PROGRAM_NAME "SCC_C_To_Tree"
#define TREE_VERSION 8
/**/
#include "windows.h"
#include "Recursia.h"
#include "Assem\Assem.h"
#include "windows.h"
#include "io.h"

#include "..\\..\\_SystemLibs\\ConsoleApp\\ConsoleApp.h"

int main (int ArgN, char** ARG)
{
    if (ArgN == 1)
    {
        printf ("%s help.\n", PROGRAM_NAME);
        printf ("Arguments:\n");
        printf ("-i input_file_name (default = \"Program.scc_c\")\n");
        printf ("-o output_file_name (default = \"Tree.scc_tree\")\n");
        printf ("-e error_output_file_name (default = \"Error.txt\")\n");
        exit (0);
    }

    #define ADDR ARG [0]
    string Way = GetMyWay (ADDR);
    string Temp = Way;

    int InputNameNumber = 0;
    char InputName [] = "Program.scc_c";
    FILE* Input = NULL;
    FILE* Output = NULL;
    FILE* ErrorOutput = NULL;

    try
    {
        //MODULES CHECK
        if (!ModuleExist ("..\\Preprocessor\\Preprocessor.exe", Way)) throw TH_ERROR "One of the nessesary modules missing");

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
        }

        if (access ((InputNameNumber == 0)? InputName : ARG [InputNameNumber], 0) == -1) throw TH_ERROR "No input file.");

        Temp = "\"";
        Temp += Way;
        Temp += "..\\Preprocessor\\Preprocessor.exe\" ";
        Temp += (InputNameNumber == 0)? InputName : ARG [InputNameNumber];
        Temp += " Program.scc_pre_c";

        system (Temp.c_str ());

        Input = fopen ("Program.scc_pre_c", "rb");

        if (!Input) throw TH_ERROR "Unknown preprocessor error.");

        if (!Output) Output = fopen ("Tree.scc_tree", "wb");

        if (!ErrorOutput) ErrorOutput = fopen ("Error.txt", "ab");

        newTree Tree;

        newVector <newNodeData> Program;
        int Size = 0;
        LexemProgram (Input, Program, &Size);

        fclose (Input);
        remove ("Program.scc_pre_c");

        if (!CreateTree (Tree, &Program[0], Size, ErrorOutput)) throw TH_ERROR "Some syntax error. Look error output.");

        Tree.SaveToFile (Output);
        fclose (Output);

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

    remove ("Program.scc_pre_c");
    _fcloseall ();
}
