#define PROGRAM_NAME "SCC_Tree_Print"
#define TREE_VERSION 9
#define BeautifulDot
#define __DOT__ "PLEASE_DONT_USE"

#include "TreeInfoToDot.h"
#include "windows.h"
#include "io.h"

#include "..\\..\\_SystemLibs\\ConsoleApp\\ConsoleApp.h"

int main (int ArgN, char** ARG)
{
    if (ArgN == 1)
    {
        printf ("%s help.\n", PROGRAM_NAME);
        printf ("Arguments:\n");
        printf ("-it input_tree_name (default = \"Tree.scc_tree\")\n");
        printf ("-ii input_info_name (default = \"Info.scc_tree_info\")\n");
        printf ("-ip input_program_name (default not used)\n");
        printf ("-o output_file_name (default = \"Tree.jpg\")\n");
        printf ("-e error_output_file_name (default = \"Error.txt\")\n");
        exit (0);
    }

    #define ADDR ARG [0]
    string Way = GetMyWay (ADDR);
    string Temp = Way;

    FILE* Input = NULL;
    FILE* InputInfo = NULL;
    FILE* ErrorOutput = NULL;
    int OutputNum = 0;
    char OutputName [] = "Tree.jpg";

    try
    {
        //MODULES CHECK
        if (!ModuleExist ("DotExe\\dot.exe", Way)) throw TH_ERROR "One of the nessesary modules missing");

        int InputProgramName = -1;

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
            if (strcmp (ARG[i], "-ip") == 0)
            {
                i ++;
                if (i >= ArgN) throw TH_ERROR "Sudden end after argument.");

                InputProgramName = i;
            }
            if (strcmp (ARG[i], "-o") == 0)
            {
                i ++;
                if (i >= ArgN) throw TH_ERROR "Sudden end after argument.");

                OutputNum = i;
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
        if (!Input) throw TH_ERROR "No input tree file.");

        if (!InputInfo) InputInfo = fopen ("Info.scc_tree_info", "rb");
        if (!InputInfo) throw TH_ERROR "No input info file.");

        if (!ErrorOutput) ErrorOutput = fopen ("Error.txt", "ab");

        newTree Tree;

        Tree.LoadFromFile (Input);
        fclose (Input);

        newTreeInfo Info;
        newVector <newNodeInfo> NodeInfo;

        ReadInfoFile (Info, NodeInfo, InputInfo);
        fclose (InputInfo);

        FILE* To = fopen ("TreeTemp.dot", "w");

        FILE* Program = NULL;
        if (InputProgramName != -1)
        {
            Program = fopen (ARG [InputProgramName], "rb");
            if (!Program) throw TH_ERROR "Input program not exist!");
        }

        DotDrawTree (To, Tree, NodeInfo, Info, Program);
        if (InputProgramName != -1) fclose (Program);

        fclose (To);

        Temp = Way;
        Temp += "DotExe\\Dot.exe -Tjpg TreeTemp.dot -o ";
        Temp += (OutputNum == 0)? OutputName : ARG [OutputNum];
        system (Temp.c_str ());

        remove ("TreeTemp.dot");

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
        printf ("I don't know, what they said!");
    }

    _fcloseall ();
}
