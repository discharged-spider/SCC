#define PROGRAM_NAME "SCC_Tree_To_Asm"
#define TREE_VERSION 9
/**/
#include "TreeToMyAsm.h"

#include "..\\..\\_SystemLibs\\ConsoleApp\\ConsoleApp.h"

int main (int ArgN, char** ARG)
{
    if (ArgN == 1)
    {
        printf ("%s help.\n", PROGRAM_NAME);
        printf ("Arguments:\n");
        printf ("-it input_tree_name (default = \"Tree.scc_tree\")\n");
        printf ("-ii input_info_name (default = \"Info.scc_tree_info\")\n");
        printf ("-o output_file_name (default = \"Program.scc_asm\")\n");
        printf ("-e error_output_file_name (default = \"Error.txt\")\n");
        exit (0);
    }

    #define ADDR ARG [0]
    string Way = GetMyWay (ADDR);
    string Temp = Way;

    FILE* Input = NULL;
    FILE* InputInfo = NULL;
    FILE* Output = NULL;
    FILE* ErrorOutput = NULL;

    try
    {
        for (int i = 1; i < ArgN; i ++)
        {
            if (strcmp (ARG[i], "-it") == 0)
            {
                i ++;
                if (i >= ArgN) throw TH_ERROR "Sudden end after argument.");

                if (Input) fclose (Input);
                Input = fopen (ARG [i], "rb");
            }
            if (strcmp (ARG[i], "-ii") == 0)
            {
                i ++;
                if (i >= ArgN) throw TH_ERROR "Sudden end after argument.");

                if (InputInfo) fclose (Input);
                InputInfo = fopen (ARG [i], "rb");
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

        if (!Input) Input = fopen ("Tree.scc_tree", "rb");
        if (!Input) throw TH_ERROR "No input file.");

        if (!InputInfo) InputInfo = fopen ("Info.scc_tree_info", "rb");
        if (!InputInfo) throw TH_ERROR "No input file.");

        if (!Output) Output = fopen ("Program.scc_asm", "wb");

        if (!ErrorOutput) ErrorOutput = fopen ("Error.txt", "ab");

        newTree Tree;

        Tree.LoadFromFile (Input);
        fclose (Input);

        newTreeInfo Info;
        newVector <newNodeInfo> NodeInfo;

        ReadInfoFile (Info, NodeInfo, InputInfo);
        fclose (InputInfo);

        CreateAsm (Output, Tree, Info, NodeInfo);
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

    _fcloseall ();
}
