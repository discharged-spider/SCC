#define PROGRAM_NAME "SCC_OLYMPIAD_VIZUALIZER"

//#define DEBUG

#include "TXLib.h"

#include "..\Asm_Processor\Syntax\VariablesInspector.h"

#include "cstdio"
#include "windows.h"

#include "../SystemFolder/_SystemLibs/ConsoleApp/ConsoleApp.h"
#include "../SystemFolder/_SystemLibs/Throw/Throw.h"

const int WINDOW_SX = 800, WINDOW_SY = 600;

const int SW_SX = 600, SW_SY = 400;
const int SW_X  = 100, SW_Y  = 100;

bool DEBUG_MODE = false;

struct agent_t
{
    bool disqualified;

    string name;

    vector <int> data;

    COLORREF color;

    agent_t ()
    {
        disqualified = false;
    }
};

int test (vector <agent_t>& agents, int max_n, string Way);
string commandTest (agent_t agent, string Way, string input, string output, string test);
string commandProcessTest (string Way, string test, string test_result);

void drawBar (int max, int load);

void draw (agent_t agent, int sx, int sy);

void draw (int sx, int sy);

void draw (vector <agent_t>& agents);

COLORREF HVAtoRGB (int H, int S, int V);

int main (int ArgN, char** ARG)
{
    srand (time (NULL));

    if (ArgN == 1)
    {
        printf ("%s help.\n", PROGRAM_NAME);
        printf ("Arguments:\n");
        printf ("list of agents to test");
        exit (0);
    }

    #define ADDR ARG [0]
    string Way = GetMyWay (ADDR);
    string Temp = Way;

    FILE* ErrorOutput = NULL;

    try
    {
        vector <agent_t> agents;

        for (int i = 1; i < ArgN; i ++)
        {
            if (strcmp (ARG [i], "-d") == 0)
            {
                DEBUG_MODE = true;

                continue;
            }
            if (strcmp (ARG [i], "-r") == 0)
            {
                srand (time (NULL));

                continue;
            }

            agent_t temp;
            temp.name = ARG [i];

            agents.push_back (temp);
        }

        for (int an = 0; an < agents.size (); an ++)
        {
            agents [an].color = HVAtoRGB (360 * (an + 1) / (agents.size () + 1), 100, 100);
        }

        if (!ErrorOutput) ErrorOutput = fopen ("Error.txt", "ab");

        txCreateWindow (WINDOW_SX, WINDOW_SY);

        const int N = 40;

        int sy = test (agents, N, Way);

        txSetFillColor (TX_BLACK);
        txClear ();

        txBegin ();

        for (int an = 0; an < agents.size (); an ++)
        {
            draw (agents [an], N - 1, sy);
        }
        draw (N - 1, sy);

        txEnd ();

        while (!GetAsyncKeyState (VK_SPACE)) txSleep (20);

        draw (agents);

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

int test (vector <agent_t>& agents, int max_n, string Way)
{
    int MAX = 1;

    const string input_name = "input.txt";
    const string output_name = "output.txt";
    const string test_name = "test.data";
    const string test_result_name = "test.txt";

    for (int an = 0; an < agents.size (); an ++)
    {
        agents [an].data.resize (max_n);
    }

    for (int n = 1; n <= max_n; n ++)
    {
        vector <int> test (n);
        for (int i = 0; i < test.size (); i ++) test [i] = rand () % (2 * n + 10);

        FILE* input = fopen (input_name.c_str (), "wb");
        fprintf (input, "%d ", n);
        for (int i = 0; i < test.size (); i ++) fprintf (input, "%d ", test [i]);
        fclose (input);

        std::sort (test.begin (), test.end ());

        for (int an = 0; an < agents.size (); an ++)
        {
            drawBar (max_n * agents.size (), (n - 1) * agents.size () + an);

            system (commandTest (agents [an], Way, input_name, output_name, test_name).c_str ());

            agents [an].data [n - 1] = 0;

            FILE* output = fopen (output_name.c_str (), "rb");
            for (int i = 0; i < test.size (); i ++)
            {
                int answer = 0;
                if (!fscanf (output, "%d", &answer))
                {
                    agents [an].data [n - 1] = -1;

                    break;
                }

                if (answer != test [i])
                {
                    agents [an].data [n - 1] = -1;

                    break;
                }
            }
            fclose (output);

            if (agents [an].data [n - 1] != 0)
            {
                if (DEBUG_MODE)
                {
                    printf ("Error! (%s)\n", agents [an].name.c_str ());

                    printf ("Blank answer:\n");

                    for (int i = 0; i < test.size (); i ++)
                    {
                        printf ("%d ", test [i]);
                    }

                    exit (0);
                }

                agents [an].disqualified = true;

                continue;
            }

            system (commandProcessTest (Way, test_name, test_result_name).c_str ());

            FILE* test = fopen (test_result_name.c_str (), "rb");

            int vars = -1, arrs = -1;
            fscanf (test, "%d", &vars);
            fscanf (test, "%d", &arrs);

            agents [an].data [n - 1] = arrs;

            if (arrs > MAX) MAX = arrs;
            fclose (test);

            remove (test_name.c_str ());
            remove (test_result_name.c_str ());
        }
    }

    remove (input_name.c_str ());
    remove (output_name.c_str ());

    return MAX;
}

string commandTest (agent_t agent, string Way, string input, string output, string test)
{
    string Command = "\"";
    Command += Way;
    Command += "..\\Asm_Processor\\";

    Command += "JIT\\JIT.exe\" ";

    Command += "-i ";
    Command += agent.name;
    Command += " ";

    Command += "-fi ";
    Command += input;
    Command += " ";

    Command += "-fo ";
    Command += output;
    Command += " ";

    Command += "-insp ";
    Command += test;
    Command += " ";

    Command += " >NUL 2>NUL";

    return Command;
}
string commandProcessTest (string Way, string test, string test_result)
{
    string Command = "\"";
    Command += Way;
    Command += "\\";

    Command += "InfoDecoder.exe\" ";

    Command += "-i ";
    Command += test;
    Command += " ";

    Command += "-o ";
    Command += test_result;
    Command += " ";

    Command += "-m max";
    Command += " ";

    Command += " >NUL 2>NUL";

    return Command;
}

void draw (agent_t agent, int sx, int sy)
{
    txSetColor (agent.color, 2);
    txSetFillColor (agent.color);

    for (int i = 0; i < agent.data.size () - 1; i ++)
    {
        txLine (SW_X + i * SW_SX / sx, SW_Y + SW_SY - agent.data [i] * SW_SY / sy, SW_X + (i + 1) * SW_SX / sx, SW_Y + SW_SY - agent.data [i + 1] * SW_SY / sy);
    }
}

void draw (int sx, int sy)
{
    #define FONT "Arial"
    #define FONT_SIZE 18

    txSetColor (TX_GRAY);

    txLine (SW_X, SW_Y + SW_SY, SW_X + SW_SX, SW_Y + SW_SY);
    txLine (SW_X, SW_Y + SW_SY, SW_X        , SW_Y);

    txSetTextAlign (TA_CENTER);

    int SIZE    = 20;
    int O_SIZE  = 3;

    int baseline = SW_Y + SW_SY + FONT_SIZE / 2 - 3;

    txSelectFont (FONT, FONT_SIZE, -1, FW_DONTCARE, false, false, false, 0);
    txTextOut (SW_X + SW_SX, baseline + FONT_SIZE, "Размер");

    char num [100] = "";

    double dist = (double)SW_SX / sx;

    double pos = SW_X;
    double end = 0;

    for (int i = 0; i <= sx; i ++)
    {
        if (end >= SIZE)
        {
            pos += end;
            end = 0;

            sprintf (num, "%d", i + 1);
            txTextOut (pos, baseline, num);

            txLine (pos, SW_Y + SW_SY + O_SIZE, pos, SW_Y + SW_SY - O_SIZE);

            txLine (pos, SW_Y + SW_SY, pos, SW_Y);
        }

        end += dist;
    }

    SIZE    = 30;
    O_SIZE  = 3;

    baseline = SW_X - FONT_SIZE - 5;

    txSelectFont (FONT, FONT_SIZE, -1, FW_DONTCARE, false, false, false, 90);
    txTextOut (baseline - FONT_SIZE, SW_Y, "Операции");

    dist = (double)SW_SY / sy;

    pos = SW_Y + SW_SY;
    end = 0;

    for (int i = 0; i <= sy; i ++)
    {
        if (end >= SIZE)
        {
            pos -= end;
            end = 0;

            sprintf (num, "%.1f", (double)i/sy);
            txTextOut (baseline, pos, num);

            txLine (SW_X + O_SIZE, pos, SW_X - O_SIZE, pos);

            txLine (SW_X, pos, SW_X + SW_SX, pos);
        }

        end += dist;
    }
}

void draw (vector <agent_t>& agents)
{
    txSetTextAlign (TA_TOP | TA_LEFT);

    txSelectFont (FONT, FONT_SIZE, -1, FW_DONTCARE, false, false, false, 0);

    for (int i = 0; i < agents.size (); i ++)
    {
        string output = agents [i].name;

        if (agents [i].disqualified) output += " (banned)";

        txSetColor (agents [i].color);
        txTextOut (0, i * (FONT_SIZE + 1), output.c_str ());
    }
}

COLORREF HVAtoRGB (int H, int S, int V)
{
    int R = 0, G = 0, B = 0;

    int Hi = H / 60;
    double V_min = (100 - S) * V / 100.0;
    double a = (V - V_min) * (H % 60) / 60.0;
    double V_inc = V_min + a;
    double V_dec = V - a;

    switch (Hi)
    {
        case (0):
        {
            R = V * 255;
            G = V_inc * 255;
            B = V_min * 255;

            break;
        }
        case (1):
        {
            R = V_dec * 255;
            G = V * 255;
            B = V_min * 255;

            break;
        }
        case (2):
        {
            R = V_min * 255;
            G = V * 255;
            B = V_inc * 255;

            break;
        }
        case (3):
        {
            R = V_min * 255;
            G = V_dec * 255;
            B = V * 255;

            break;
        }
        case (4):
        {
            R = V_inc * 255;
            G = V_min * 255;
            B = V * 255;

            break;
        }
        case (5):
        {
            R = V * 255;
            G = V_min * 255;
            B = V_dec * 255;

            break;
        }
    };

    return RGB (R, G, B);
}

void drawBar (int max, int load)
{
    txBegin ();

    const int size = 25;
    const int margin = 2;

    const int pos = WINDOW_SY / 4 + size;

    txSetFillColor (TX_BLACK);
    txClear ();

    txSetColor (TX_WHITE);
    txSetFillColor (TX_NULL);

    txRectangle (margin, WINDOW_SY - margin - pos, WINDOW_SX - margin, WINDOW_SY - margin - size - pos);

    txSetFillColor (TX_BLUE);

    txRectangle (margin, WINDOW_SY - margin - pos, margin + (WINDOW_SX - 2 * margin) / max * load, WINDOW_SY - margin - size - pos);

    txSetTextAlign (TA_CENTER | TA_BASELINE);

    txSelectFont (FONT, FONT_SIZE * 4, -1, FW_DONTCARE, false, false, false, 0);
    txTextOut (WINDOW_SX / 2, WINDOW_SY / 2 - 3 * margin - size, "Тестирую");

    txEnd ();
}
