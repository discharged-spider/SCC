#ifndef TREETODOT__
#define TREETODOT__

#include "..\Lib\Tree.h"
#include "..\Lib\BaseTreeDesc.h"
#include "..\InfoMaker\InfoMaker.h"
#include "..\..\_SystemLibs\Throw\Throw.h"
#include "ctime"
#include "Dotter.h"

//==============================================================================

void DrawTreeNode (FILE* To, newNode Node, newNodeInfo NodeInfo, int i);

//==============================================================================

void DotDrawTree (FILE* To, newTree& Tree, newVector <newNodeInfo> NodeInfo, newTreeInfo Info, FILE* Text = NULL);

//==============================================================================

void DrawTreeNode (FILE* To, newNode Node, newNodeInfo NodeInfo, int i)
{
    if (!To) throw TH_ERROR "No output file");

    newDotLinkStyle Left, Right;

    dtNodeStyle ().shape ("rect")
                  .style ("rounded, filled");

    const int MaxSize = 100;
    char Label [MaxSize] = "";

    int n = MaxSize;

    n -= _snprintf (Label + MaxSize - n, n, "#%d", i);

    newNodeData Data = Node.Data;

    switch (Data.Descriptor)
    {
        case (N_NODE):
        {
            dtNodeStyle ().shape ("circle");

            dtNodeStyle ().fontcolor ("black")
                          .color     ("black")
                          .fillcolor ("#DDDDDD");

            break;
        }

        case (N_NUM):
        {
            dtNodeStyle ().fontcolor ("darkgreen")
                          .color     ("darkgreen")
                          .fillcolor ("#98FF66");

            n -= _snprintf (Label + MaxSize - n, n, "\nNUM = %lg\n", Data.Data);

            break;
        }
        case (N_VAR):
        {
            dtNodeStyle ().fontcolor ("darkgreen")
                          .color     ("darkgreen")
                          .fillcolor ("#C4F6C1");

            n -= _snprintf (Label + MaxSize - n, n, "\nVAR = \"%s\"[%d] &%d\n", Data.Name, NodeInfo.ID, NodeInfo.Addr);

            break;
        }
        case (N_ARR):
        {
            dtNodeStyle ().fontcolor ("darkgreen")
                          .color     ("darkgreen")
                          .fillcolor ("#C4F6C1");

            n -= _snprintf (Label + MaxSize - n, n, "\nARR = \"%s\"[%d] &%d\n", Data.Name, NodeInfo.ID, NodeInfo.Addr);

            break;
        }
        case (N_FUN):
        {
            dtNodeStyle ().fontcolor ("darkgreen")
                          .color     ("darkgreen")
                          .fillcolor ("#C4F6C1");

            n -= _snprintf (Label + MaxSize - n, n, "\nFUNC = \"%s\"[%d]\n", Data.Name, NodeInfo.ID);

            break;
        }
        case (N_LNK):
        {
            dtNodeStyle ().fontcolor ("darkgreen")
                          .color     ("darkgreen")
                          .fillcolor ("#C4F6C1");

            n -= _snprintf (Label + MaxSize - n, n, "\nLINK = \"%s\"[%d] &%d\n", Data.Name, NodeInfo.ID, NodeInfo.Addr);

            break;
        }

        case (N_NEW):
        {
            dtNodeStyle ().fontcolor ("darkslateblue")
                          .color     ("darkslateblue")
                          .fillcolor ("#CCEDFF");

            n -= _snprintf (Label + MaxSize - n, n, "\nNEW\n");

            break;
        }
        case (N_DEL):
        {
            dtNodeStyle ().fontcolor ("darkslateblue")
                          .color     ("darkslateblue")
                          .fillcolor ("#CCEDFF");

            n -= _snprintf (Label + MaxSize - n, n, "\nDEL\n");

            break;
        }
        case (N_NATIVE):
        {
            dtNodeStyle ().fontcolor ("darkslateblue")
                          .color     ("darkslateblue")
                          .fillcolor ("#CCEDFF");

            n -= _snprintf (Label + MaxSize - n, n, "\nNATIVE\n");

            break;
        }

        case (N_EQ):
        {
            dtNodeStyle ().fontcolor ("red")
                          .color     ("red")
                          .fillcolor ("#FFCCC9");

            n -= _snprintf (Label + MaxSize - n, n, "\n=\n");

            break;
        }

        case (N_SUM):
        {
            dtNodeStyle ().fontcolor ("red")
                          .color     ("red")
                          .fillcolor ("#FFCCC9");

            n -= _snprintf (Label + MaxSize - n, n, "\n+\n");

            break;
        }
        case (N_SUB):
        {
            dtNodeStyle ().fontcolor ("red")
                          .color     ("red")
                          .fillcolor ("#FFCCC9");

            n -= _snprintf (Label + MaxSize - n, n, "\n-\n");

            break;
        }
        case (N_MUL):
        {
            dtNodeStyle ().fontcolor ("red")
                          .color     ("red")
                          .fillcolor ("#FFCCC9");

            n -= _snprintf (Label + MaxSize - n, n, "\n*\n");

            break;
        }
        case (N_DIV):
        {
            dtNodeStyle ().fontcolor ("red")
                          .color     ("red")
                          .fillcolor ("#FFCCC9");

            n -= _snprintf (Label + MaxSize - n, n, "\n/\n");

            break;
        }
        case (N_MOD):
        {
            dtNodeStyle ().fontcolor ("red")
                          .color     ("red")
                          .fillcolor ("#FFCCC9");

            n -= _snprintf (Label + MaxSize - n, n, "\n%%\n");

            break;
        }

        case (N_TRUE):
        {
            dtNodeStyle ().fontcolor ("darkgreen")
                          .color     ("darkgreen")
                          .fillcolor ("#98FF66");

            n -= _snprintf (Label + MaxSize - n, n, "\ntrue\n");

            break;
        }
        case (N_FALSE):
        {
            dtNodeStyle ().fontcolor ("darkgreen")
                          .color     ("darkgreen")
                          .fillcolor ("#98FF66");

            n -= _snprintf (Label + MaxSize - n, n, "\nfalse\n");

            break;
        }

        case (N_NOT):
        {
            dtNodeStyle ().fontcolor ("red")
                          .color     ("red")
                          .fillcolor ("#FFCCC9");

            n -= _snprintf (Label + MaxSize - n, n, "\n!\n");

            break;
        }
        case (N_AND):
        {
            dtNodeStyle ().fontcolor ("red")
                          .color     ("red")
                          .fillcolor ("#FFCCC9");

            n -= _snprintf (Label + MaxSize - n, n, "\n&&\n");

            break;
        }
        case (N_OR):
        {
            dtNodeStyle ().fontcolor ("red")
                          .color     ("red")
                          .fillcolor ("#FFCCC9");

            n -= _snprintf (Label + MaxSize - n, n, "\n||\n");

            break;
        }

        case (N_OPEQ):
        {
            dtNodeStyle ().fontcolor ("red")
                          .color     ("red")
                          .fillcolor ("#FFCCC9");

            n -= _snprintf (Label + MaxSize - n, n, "\n==\n");

            break;
        }
        case (N_NONEQ):
        {
            dtNodeStyle ().fontcolor ("red")
                          .color     ("red")
                          .fillcolor ("#FFCCC9");

            n -= _snprintf (Label + MaxSize - n, n, "\n!=\n");

            break;
        }
        case (N_MORE):
        {
            dtNodeStyle ().fontcolor ("red")
                          .color     ("red")
                          .fillcolor ("#FFCCC9");

            n -= _snprintf (Label + MaxSize - n, n, "\n>\n");

            break;
        }
        case (N_LESS):
        {
            dtNodeStyle ().fontcolor ("red")
                          .color     ("red")
                          .fillcolor ("#FFCCC9");

            n -= _snprintf (Label + MaxSize - n, n, "\n<\n");

            break;
        }
        case (N_MREQ):
        {
            dtNodeStyle ().fontcolor ("red")
                          .color     ("red")
                          .fillcolor ("#FFCCC9");

            n -= _snprintf (Label + MaxSize - n, n, "\n>=\n");

            break;
        }
        case (N_LSEQ):
        {
            dtNodeStyle ().fontcolor ("red")
                          .color     ("red")
                          .fillcolor ("#FFCCC9");

            n -= _snprintf (Label + MaxSize - n, n, "\n<=\n");

            break;
        }

        case (N_IF):
        {
            dtNodeStyle ().fontcolor ("darkslateblue")
                          .color     ("darkslateblue")
                          .fillcolor ("#CCEDFF");

            n -= _snprintf (Label + MaxSize - n, n, "\nIF\n");

            break;
        }
        case (N_ELSE):
        {
            dtNodeStyle ().fontcolor ("darkslateblue")
                          .color     ("darkslateblue")
                          .fillcolor ("#CCEDFF");

            n -= _snprintf (Label + MaxSize - n, n, "\nIF-ELSE\n");

            break;
        }

        case (N_WHILE):
        {
            dtNodeStyle ().fontcolor ("darkslateblue")
                          .color     ("darkslateblue")
                          .fillcolor ("#CCEDFF");

            n -= _snprintf (Label + MaxSize - n, n, "\nWHILE\n");

            break;
        }
        case (N_FOR):
        {
            dtNodeStyle ().fontcolor ("darkslateblue")
                          .color     ("darkslateblue")
                          .fillcolor ("#CCEDFF");

            n -= _snprintf (Label + MaxSize - n, n, "\nFOR\n");

            break;
        }

        case (N_RETURN):
        {
            dtNodeStyle ().fontcolor ("darkslateblue")
                          .color     ("darkslateblue")
                          .fillcolor ("#CCEDFF");

            n -= _snprintf (Label + MaxSize - n, n, "\nRETURN\n");

            break;
        }

        case (N_BREAK):
        {
            dtNodeStyle ().fontcolor ("darkslateblue")
                          .color     ("darkslateblue")
                          .fillcolor ("#CCEDFF");

            n -= _snprintf (Label + MaxSize - n, n, "\nBREAK\n");

            break;
        }

        case (N_ECHO):
        {
            dtNodeStyle ().fontcolor ("darkslateblue")
                          .color     ("darkslateblue")
                          .fillcolor ("#CCEDFF");

            n -= _snprintf (Label + MaxSize - n, n, "\nECHO\n");

            break;
        }
        case (N_GET):
        {
            dtNodeStyle ().fontcolor ("darkslateblue")
                          .color     ("darkslateblue")
                          .fillcolor ("#CCEDFF");

            n -= _snprintf (Label + MaxSize - n, n, "\nGET\n");

            break;
        }

        case (N_MARK):
        {
            dtNodeStyle ().fontcolor ("darkslateblue")
                          .color     ("darkslateblue")
                          .fillcolor ("#CCEDFF");

            n -= _snprintf (Label + MaxSize - n, n, "\nMARK \"%s\"[%d]\n", Data.Name, NodeInfo.ID);

            break;
        }
        case (N_GOTO):
        {
            dtNodeStyle ().fontcolor ("darkslateblue")
                          .color     ("darkslateblue")
                          .fillcolor ("#CCEDFF");

            n -= _snprintf (Label + MaxSize - n, n, "\nGOTO \"%s\"[%d]\n", Data.Name, NodeInfo.ID);

            break;
        }

        default:
        {
            dtNodeStyle ().shape ("egg");

            dtNodeStyle ().fontcolor ("#500000")
                          .color     ("#500000")
                          .fillcolor ("#FF4040");

            n -= _snprintf (Label + MaxSize - n, n, "\n [ERROR](Unknown Descriptor = %d)\n", Data.Descriptor);

            break;
        }
    };

    dtNode (i, Label);

    if (Node.Left  != -1) dtLink (i, Node.Left,  "L", Left );
    if (Node.Right != -1) dtLink (i, Node.Right, "R", Right);

    fprintf (To, "\n");
}


//==============================================================================

void DotDrawTree (FILE* To, newTree& Tree, newVector <newNodeInfo> NodeInfo, newTreeInfo Info, FILE* Text)
{
    if (!To) throw TH_ERROR "No output file");

    dtBegin (To);

    fprintf (To, "    subgraph clusterAll\n");
    fprintf (To, "    {\n");
    fprintf (To, "        shape=\"rect\";\n");
    fprintf (To, "        style=\"rounded, filled\";\n");
    fprintf (To, "        color=\"black\";\n");
    fprintf (To, "        fillcolor=\"aquamarine\";\n");
    fprintf (To, "        \n");
    DotterSystem::Ind = 8;

    for (int i = 0; i < Tree.Size (); i ++)
    {
        DrawTreeNode (To, Tree.GetNode (i), NodeInfo [i], i);
    }

    dtNodeStyle ().shape ("point")
                  .style ("bold")
                  .color ("red");

    dtLinkStyle ().color ("red");

    dtNode (Tree.Size ()                     );
    dtLink (Tree.Size (), Tree.CurrentNode ());

    fprintf (To, "        subgraph clusterText\n");
    fprintf (To, "        {\n");
    fprintf (To, "            shape=\"rect\";\n");
    fprintf (To, "            style=\"rounded, filled\";\n");
    fprintf (To, "            color=\"black\";\n");
    fprintf (To, "            fillcolor=\"lightgray\";\n");
    fprintf (To, "            \n");
    fprintf (To, "            fontcolor=\"red\";\n");
    fprintf (To, "            fontsize=\"18\";\n");
    fprintf (To, "            \n");
    fprintf (To, "            label=\"\\nExpression Tree Dump\\n\\n\";\n");
    fprintf (To, "            \n");

    srand (time(NULL));
    int FontN = rand() % 5;
    //FontN = 0;

    char Fonts [5][30] = {"Times-Roman", "Comic Sans MS", "Jokerman", "Ravie", "David"};

    fprintf (To, "            Info[shape=\"rect\", style=\"rounded, filled\", color=\"white\", fillcolor=\"#EAEAEA\", fontname=\"%s\", label=\"", Fonts [FontN]);
    fprintf (To, "Info:\\l");
    fprintf (To, "Var number  = %d\\l", Info.VarNumber);
    fprintf (To, "Var memory  = %d\\l", Info.VarMemory);

    fprintf (To, "Arr number  = %d\\l", Info.ArrNumber);
    fprintf (To, "Arr memory  = %d\\l", Info.ArrMemory);

    fprintf (To, "Use natives = %s\\l", (Info.UseNatives)? "true" : "false");
    fprintf (To, "Func number = %d\\l", Info.FuncNumber);

    fprintf (To, "Mark number = %d\\l", Info.MarkNumber);
    fprintf (To, "\\l\"];\n");

    fprintf (To, "            CopyRight [shape=\"plaintext\", fontsize=\"8\", label=\"(C) Danila Baygushev 2012-2014\"];\n");

    if (Text != NULL)
    {
        fprintf (To, "            Text[shape=\"rect\", style=\"rounded, filled\", color=\"white\", fillcolor=\"#EAEAEA\", fontname=\"%s\", label=\"", Fonts [FontN]);

        char c = 0;

        while (fread (&c, 1, 1, Text) == 1)
        {
            if      (c == '\n') fprintf (To, "\\l");
            else if (c == '\r') fprintf (To, "");
            else if (c == '\\') fprintf (To, "\\\\");
            else if (c == '"')  fprintf (To, "\\\"");
            else                fprintf (To, "%c", c);
        }

        fprintf (To, "\\l\"];\n");
        fprintf (To, "            Text -> CopyRight[color=\"lightgray\"];\n");
    }
    fprintf (To, "        }\n");
    fprintf (To, "    }\n");
    dtEnd ();
}

#endif
