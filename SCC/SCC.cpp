#define PROGRAM_NAME "SCC"

#define TEMP_OUTPUT "TEMP\\_TEMP_DELETE_ME_"

#include "windows.h"
#include "..\\SystemFolder\\_SystemLibs\\Throw\\Throw.h"
#include "stdio.h"
#include "string"
#include "io.h"

#include "..\\SystemFolder\\_SystemLibs\\ConsoleApp\\ConsoleApp.h"

const int TypesNumber = 5;

enum SCC_TYPES
{
    SCC_C,
    SCC_ASM,
    SCC_NEW_ASM,
    SCC_SPL,
    SCC_TREE
};

bool CheckModules (string Way);

int GetType (const char File []);

string ComandFileToTree (int From, const char FromName [], const char ToName [], const char ToInfoName [], const char ErrorOutputName [], string Way);
string ComandOptimazeTree (const char FromName [], const char ToInfoName [], const char ErrorOutputName [], string Way);
string ComandInfoFromTree (const char FromName [], const char ToInfoName [], const char ErrorOutputName [], string Way);
string ComandTreeToFile (int To, const char FromName [], const char FromInfoName [], const char ToName [], const char ErrorOutputName [], string Way);

bool CheckError (const char ErrorOutputName []);

bool SCC_Compile (int From, int To, const char FromName [], const char ToName [], const char ErrorOutputName [], string Way, bool UseOptimizator);

int main (int ArgN, char** ARG)
{
    if (ArgN == 1)
    {
        printf ("%s help.\n", PROGRAM_NAME);
        printf ("Arguments:\n");
        printf ("[] - not requide params\n");
        printf ("input_file_name\n");
        printf ("-i [-scc_c || -scc_spl] /*input type*/\n");
        printf ("-o output_file_name (default = \"Output.txt\")\n");
        printf ("-m [-scc_c || -scc_asm || -scc_new_asm || -scc_spl] /*output type*/\n");
        printf ("-e error_output_file_name (default = \"Error.txt\")\n");
        printf ("-no - non optimaze (don't use optimizator) \n");
        exit (0);
    }

    #define ADDR ARG [0]
    string Way = GetMyWay (ADDR);
    string Temp = Way;

    int InputType = -1;
    int InputNumber = -1;

    int OutputType = -1;
    int OutputNumber = 0;
    char OutputFileName [] = "Output.txt";

    int ErrorOutputNumber = 0;
    char ErrorFileName [] = "Error.txt";

    bool UseOptimizator = true;

    try
    {
        if (!CheckModules (Way)) throw TH_ERROR "One of the nessesary modules missing");

        for (int i = 1; i < ArgN; i ++)
        {
            if (strcmp (ARG[i], "-i") == 0)
            {
                i ++;
                if (i >= ArgN) throw TH_ERROR "Sudden end after argument.");

                InputType = GetType (ARG [i]);

                if (InputType == -1) throw TH_ERROR "Can't match input type");

                continue;
            }
            if (strcmp (ARG[i], "-o") == 0)
            {
                i ++;
                if (i >= ArgN) throw TH_ERROR "Sudden end after argument.");

                OutputNumber = i;

                if (OutputType == -1) OutputType = GetType (ARG [i]);

                continue;
            }
            if (strcmp (ARG[i], "-m") == 0)
            {
                i ++;
                if (i >= ArgN) throw TH_ERROR "Sudden end after argument.");

                OutputType = GetType (ARG [i]);

                if (OutputType == -1) throw TH_ERROR "Can't match output type");

                continue;
            }
            if (strcmp (ARG[i], "-e") == 0)
            {
                i ++;
                if (i >= ArgN) throw TH_ERROR "Sudden end after argument.");

                ErrorOutputNumber = i;

                continue;
            }

            if (strcmp (ARG[i], "-no") == 0)
            {
                UseOptimizator = false;

                continue;
            }

            InputNumber = i;
            if (InputType == -1) InputType = GetType (ARG [i]);
        }

        if (InputNumber == -1)  throw TH_ERROR "No input file.");

        if (OutputType == -1) throw TH_ERROR "Can't match output type, try use -m.");
        if (InputType == -1)  throw TH_ERROR "Can't match input type, try use -i.");

        if (!SCC_Compile (InputType, OutputType, ARG [InputNumber], (OutputNumber == 0)? OutputFileName : ARG [OutputNumber], (ErrorOutputNumber == 0)? ErrorFileName : ARG [ErrorOutputNumber], Way, UseOptimizator)) throw TH_ERROR "Look ErrorOutputFile.");

        Temp = Way;
        Temp += TEMP_OUTPUT;
        remove (Temp.c_str ());

        remove ((ErrorOutputNumber == 0)? ErrorFileName : ARG [ErrorOutputNumber]);
    }
    catch (newThrowError& Error)
    {
        printf ("%s", Error.ErrorText_);

        FILE* ErrorOutput = fopen ((ErrorOutputNumber == 0)? ErrorFileName : ARG [ErrorOutputNumber], "ab");
        fprintf (ErrorOutput, "%s", Error.ErrorText_);
    }
    catch (...)
    {
        printf ("Unknown error");
    }

    _fcloseall ();
}

bool CheckModules (string Way)
{
    if (!ModuleExist ("..\\SystemFolder\\SCC_C\\_FromTree\\FromTree.exe", Way) ||
        !ModuleExist ("..\\SystemFolder\\SCC_C\\_ToTree\\ToTree.exe", Way) ||

        !ModuleExist ("..\\SystemFolder\\SCC_Spl\\_FromTree\\FromTree.exe", Way) ||
        !ModuleExist ("..\\SystemFolder\\SCC_Spl\\_ToTree\\ToTree.exe", Way) ||

        !ModuleExist ("..\\SystemFolder\\SCC_Asm\\_FromTree\\FromTree.exe", Way) ||

        !ModuleExist ("..\\SystemFolder\\SCC_New_Asm\\_FromTree\\FromTree.exe", Way) ||

        !ModuleExist ("..\\SystemFolder\\SCC_Tree\\Print\\Print.exe", Way) ||
        !ModuleExist ("..\\SystemFolder\\SCC_Tree\\Optimazer\\Optimazer.exe", Way) ||
        !ModuleExist ("..\\SystemFolder\\SCC_Tree\\InfoMaker\\InfoMaker.exe", Way))
    {
        return false;
    }

    return true;
}

int GetType (const char File [])
{
    int  TypesID    [TypesNumber]     = {SCC_C,   SCC_ASM,   SCC_NEW_ASM,   SCC_TREE,   SCC_SPL};
    //                                    scc_c    scc_asm   scc_new_asm    scc_tree    scc_spl
    char TypesNames [TypesNumber][30] = {"c_ccs", "msa_ccs", "msa_wen_ccs", "eert_ccs", "lps_ccs"};
    bool Match      [TypesNumber]     = {true,    true,      true,         true,       true};

    int Return = -1;

    int End = strlen (File) - 1;

    bool Break = false;
    for (int i = End; i > -1 && !Break; i --)
    {
        Break = true;
        for (int j = 0; j < TypesNumber; j ++)
        {
            if (Match [j] && File [i] == TypesNames [j][End - i])
            {
                if (TypesNames[j][End - i + 1] == 0)
                {
                    Return = TypesID [j];
                    Break = true;
                    break;
                }

                Break = false;
            }
            else
            {
                Match [j] = false;
            }
        }
    }

    return Return;
}

string ComandFileToTree (int From, const char FromName [], const char ToName [], const char ErrorOutputName [], string Way)
{
    string Comand = "\"";
    Comand += Way;
    Comand += "..\\SystemFolder\\";

    switch (From)
    {
        case (SCC_C):
        {
            Comand += "SCC_C";

            break;
        }
        case (SCC_SPL):
        {
            Comand += "SCC_Spl";

            break;
        }
    };

    Comand += "\\_ToTree\\ToTree.exe\" ";

    Comand += "-i ";
    Comand += FromName;
    Comand += " ";

    Comand += "-o ";
    Comand += ToName;
    Comand += " ";

    Comand += "-e ";
    Comand += ErrorOutputName;

    Comand += " >> ";
    Comand += Way;
    Comand += TEMP_OUTPUT;

    return Comand;
}

string ComandOptimazeTree (const char FromName [], const char ErrorOutputName [], string Way)
{
    string Comand = "\"";
    Comand += Way;
    Comand += "..\\SystemFolder\\SCC_Tree\\Optimazer\\Optimazer.exe\" ";

    Comand += "-i ";
    Comand += FromName;
    Comand += " ";

    Comand += "-e ";
    Comand += ErrorOutputName;

    Comand += " >> ";
    Comand += Way;
    Comand += TEMP_OUTPUT;

    return Comand;
}

string ComandInfoFromTree (const char FromName [], const char ToInfoName [], const char ErrorOutputName [], string Way)
{
    string Comand = "\"";
    Comand += Way;
    Comand += "..\\SystemFolder\\SCC_Tree\\InfoMaker\\InfoMaker.exe\" ";

    Comand += "-i ";
    Comand += FromName;
    Comand += " ";

    Comand += "-o ";
    Comand += ToInfoName;
    Comand += " ";

    Comand += "-e ";
    Comand += ErrorOutputName;

    Comand += " >> ";
    Comand += Way;
    Comand += TEMP_OUTPUT;

    return Comand;
}

string ComandTreeToFile (int To, const char FromName [], const char FromInfoName [], const char ToName [], const char ErrorOutputName [], string Way)
{
    string Comand = "\"";
    Comand += Way;
    Comand += "..\\SystemFolder\\";

    switch (To)
    {
        case (SCC_C):
        {
            Comand += "SCC_C";

            break;
        }
        case (SCC_SPL):
        {
            Comand += "SCC_Spl";

            break;
        }
        case (SCC_ASM):
        {
            Comand += "SCC_Asm";

            break;
        }
        case (SCC_NEW_ASM):
        {
            Comand += "SCC_New_Asm";

            break;
        }
    };

    Comand += "\\_FromTree\\FromTree.exe\" ";

    Comand += "-it ";
    Comand += FromName;
    Comand += " ";

    Comand += "-ii ";
    Comand += FromInfoName;
    Comand += " ";

    Comand += "-o ";
    Comand += ToName;
    Comand += " ";

    Comand += "-e ";
    Comand += ErrorOutputName;

    Comand += " >> ";
    Comand += Way;
    Comand += TEMP_OUTPUT;

    return Comand;
}

bool CheckError (const char ErrorOutputName [])
{
    bool Return = false;

    FILE* ErrorFile = fopen (ErrorOutputName, "rb");
    fseek (ErrorFile, 0, SEEK_END);
    if (ftell (ErrorFile) > 0) Return = true;
    fclose (ErrorFile);

    return Return;
}

bool SCC_Compile (int From, int To, const char FromName [], const char ToName [], const char ErrorOutputName [], string Way, bool UseOptimizator)
{
    #define TEMP_TREE "TEMP\\Temp.scc_tree"
    #define TEMP_INFO "TEMP\\Temp.scc_tree_info"

    string Temp;
    Temp += "If Not Exist ";
    Temp += "\"\"";
    Temp += Way;
    Temp += "\"";
    Temp += "TEMP\\*.*\" ";
    Temp += "mkdir ";
    Temp += "\"";
    Temp += Way;
    Temp += "\"";
    Temp += "TEMP";
    system (Temp.c_str ());

    string Temp1 = Way, Temp2 = Way;

    Temp1 = Way;
    Temp1 += TEMP_TREE;
    system (ComandFileToTree (From, FromName, Temp1.c_str()/*TEMP_TREE*/, ErrorOutputName, Way).c_str ());
    if (CheckError (ErrorOutputName)) return false;

    Temp1 = Way;
    Temp1 += TEMP_TREE;

    if (UseOptimizator)
    {
        Temp2 = Way;
        Temp2 += TEMP_INFO;
        system (ComandOptimazeTree (Temp1.c_str()/*TEMP_TREE*/, ErrorOutputName, Way).c_str ());
        if (CheckError (ErrorOutputName)) return false;
    }

    Temp1 = Way;
    Temp1 += TEMP_TREE;

    Temp2 = Way;
    Temp2 += TEMP_INFO;
    system (ComandInfoFromTree (Temp1.c_str()/*TEMP_TREE*/, Temp2.c_str()/*TEMP_INFO*/, ErrorOutputName, Way).c_str ());
    if (CheckError (ErrorOutputName)) return false;

    Temp1 = Way;
    Temp1 += TEMP_TREE;

    Temp2 = Way;
    Temp2 += TEMP_INFO;
    system (ComandTreeToFile (To, Temp1.c_str()/*TEMP_TREE*/, Temp2.c_str()/*TEMP_INFO*/, ToName, ErrorOutputName, Way).c_str ());
    if (CheckError (ErrorOutputName)) return false;

    return true;
}
