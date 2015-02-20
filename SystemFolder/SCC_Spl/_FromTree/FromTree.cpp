#define PROGRAM_NAME "SCC_Tree_To_Shakespear"
#define TREE_VERSION 10

//#define DEBUG

#include "..\\..\\_SystemLibs\\ConsoleApp\\ConsoleApp.h"

#include "TreeToShekspear.h"
#include "io.h"
#include "time.h"

int main (int ArgN, char** ARG)
{
    if (ArgN == 1)
    {
        printf ("%s help.\n", PROGRAM_NAME);
        printf ("Arguments:\n");
        printf ("-it input_tree_name (default = \"Tree.scc_tree\")\n");
        printf ("-ii input_info_name (default = \"Info.scc_tree_info\")\n");
        printf ("-o output_file_name (default = \"Program.scc_spl\")\n");
        printf ("-e error_output_file_name (default = \"Error.txt\")\n");
        printf ("-s to not use srand\n");
        exit (0);
    }

    bool no_rand = false;

    #define ADDR ARG [0]
    string Way = GetMyWay (ADDR);
    string Temp = Way;

    FILE* Input = NULL;
    FILE* InputInfo = NULL;
    FILE* Output = NULL;
    FILE* ErrorOutput = NULL;

    try
    {
        //MODULES CHECK
        if (!ModuleExist ("..\\Lists\\be.wordlist", Way) ||
            !ModuleExist ("..\\Lists\\character.wordlist", Way) ||
            !ModuleExist ("..\\Lists\\character.wordlist", Way) ||
            !ModuleExist ("..\\Lists\\first_person.wordlist", Way) ||
            !ModuleExist ("..\\Lists\\first_person_possessive.wordlist", Way) ||
            !ModuleExist ("..\\Lists\\first_person_reflexive.wordlist", Way) ||
            !ModuleExist ("..\\Lists\\negative_adjective.wordlist", Way) ||
            !ModuleExist ("..\\Lists\\negative_comparative.wordlist", Way) ||
            !ModuleExist ("..\\Lists\\negative_noun.wordlist", Way) ||
            !ModuleExist ("..\\Lists\\neutral_adjective.wordlist", Way) ||
            !ModuleExist ("..\\Lists\\neutral_noun.wordlist", Way) ||
            !ModuleExist ("..\\Lists\\nothing.wordlist", Way) ||
            !ModuleExist ("..\\Lists\\places.wordlist", Way) ||
            !ModuleExist ("..\\Lists\\positive_adjective.wordlist", Way) ||
            !ModuleExist ("..\\Lists\\positive_comparative.wordlist", Way) ||
            !ModuleExist ("..\\Lists\\positive_noun.wordlist", Way) ||
            !ModuleExist ("..\\Lists\\second_person.wordlist", Way) ||
            !ModuleExist ("..\\Lists\\second_person_possessive.wordlist", Way) ||
            !ModuleExist ("..\\Lists\\second_person_reflexive.wordlist", Way) ||
            !ModuleExist ("..\\Lists\\sentence_end.wordlist", Way) ||
            !ModuleExist ("..\\Lists\\skip.wordlist", Way) ||
            !ModuleExist ("..\\Lists\\system.wordlist", Way) ||
            !ModuleExist ("..\\Lists\\third_person_possessive.wordlist", Way))
        {
            throw TH_ERROR "One of the nessesary modules missing");
        }

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
            if (strcmp (ARG[i], "-s") == 0)
            {
                i ++;

                no_rand = true;
            }
        }

        if (!Input) Input = fopen ("Tree.scc_tree", "rb");
        if (!Input) throw TH_ERROR "No input file.");

        if (!InputInfo) InputInfo = fopen ("Info.scc_tree_info", "rb");
        if (!InputInfo) throw TH_ERROR "No input info file.");

        if (!Output) Output = fopen ("Program.scc_spl", "wb");

        if (!ErrorOutput) ErrorOutput = fopen ("Error.txt", "ab");

        if (!no_rand) srand (time (NULL));

        newTree Tree;

        Tree.LoadFromFile (Input);
        fclose (Input);

        newTreeInfo Info;
        newVector <newNodeInfo> NodeInfo;

        ReadInfoFile (Info, NodeInfo, InputInfo);
        fclose (InputInfo);

        if (Info.ArrNumber > 0) throw TH_ERROR "Arrs not support, sorry. :(");
        if (Info.UseNatives) throw TH_ERROR "Use natives?");

        CreateShekspear (Output, Tree, Info, NodeInfo, Way);
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
}
