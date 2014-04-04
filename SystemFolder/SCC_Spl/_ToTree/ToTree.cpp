#define PROGRAM_NAME "SCC_Shakespear_To_Tree"
#define TREE_VERSION 9

//#define INFO_DEBUG
//#define PAUSE

#include "..\\..\\_SystemLibs\\ConsoleApp\\ConsoleApp.h"

#include "ShekspearToTree2.h"
#include "io.h"

int main (int ArgN, char** ARG)
{
    if (ArgN == 1)
    {
        printf ("%s help.\n", PROGRAM_NAME);
        printf ("Arguments:\n");
        printf ("-i input_file_name (default = \"Program.scc_spl\")\n");
        printf ("-o output_tree_name (default = \"Tree.scc_tree\")\n");
        printf ("-e error_output_file_name (default = \"Error.txt\")\n");
        exit (0);
    }

    #define ADDR ARG [0]
    string Way = GetMyWay (ADDR);

    FILE* Input = NULL;
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
            !ModuleExist ("..\\Lists\\third_person_possessive.wordlist", Way) ||

            !ModuleExist ("NumIn.scc_tree", Way) ||
            !ModuleExist ("NumOut.scc_tree", Way))
        {
            throw TH_ERROR "One of the nessesary modules missing");
        }

        for (int i = 1; i < ArgN; i ++)
        {
            if (strcmp (ARG[i], "-i") == 0)
            {
                i ++;
                if (i >= ArgN) throw TH_ERROR "Sudden end after argument.");

                if (Input) fclose (Input);
                Input = fopen (ARG [i], "rb");
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

        if (!Input) Input = fopen ("Program.scc_spl", "rb");
        if (!Input) throw TH_ERROR "No input file.");

        if (!Output) Output = fopen ("Tree.scc_tree", "wb");

        if (!ErrorOutput) ErrorOutput = fopen ("Error.txt", "ab");

        newTree Tree;

        newTree NumIn, NumOut;

        string Temp = Way;
        Temp = Way;
        Temp += "NumIn.scc_tree";

        FILE* TempFile = fopen (Temp.c_str (), "rb");
        NumIn.LoadFromFile (TempFile);
        fclose (TempFile);

        Temp = Way;
        Temp += "NumOut.scc_tree";

        TempFile = fopen (Temp.c_str (), "rb");
        NumOut.LoadFromFile (TempFile);
        fclose (TempFile);

        if (!CreateAll (Tree, Input, ErrorOutput, NumIn, NumOut, Way)) throw TH_ERROR "Some syntax error. Look error output.");

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

    _fcloseall ();
}
