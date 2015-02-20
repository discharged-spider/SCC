#ifndef PREPROCESSOR_H
#define PREPROCESSOR_H

#include "..\\_ToTree\\Assem\\Assem.h"

//==============================================================================

void Preprocess (FILE* From, FILE* To, const char FromPath []);

bool ReadSpecialSymbol (FILE* From, FILE* To, const char StopSymbol);

//==============================================================================

void Preprocess (FILE* From, FILE* To, const char FromPath [])
{
    int line = 1;

    bool MoveComment = false;
    bool EndComment  = false;

    char c = 0;
    if (!fread (&c, sizeof (c), 1, From)) return;

    while (true)
    {
        if (c == '\n')
        {
            fprintf (To, "%c", c);

            line ++;
        }

        if (MoveComment == false && EndComment == false && c == '/')
        {
            if (fread (&c, sizeof (c), 1, From) && c == '*')
            {
                MoveComment = true;

                if (!fread (&c, sizeof (c), 1, From)) return;

                continue;
            }
            else
            {
                fseek (From, -1, SEEK_CUR);
                c = '/';
            }
        }
        if (MoveComment == true && EndComment == false && c == '*')
        {
            if (fread (&c, sizeof (c), 1, From) && c == '/')
            {
                MoveComment = false;
                if (MoveComment < 0) MoveComment = 0;

                if (!fread (&c, sizeof (c), 1, From)) return;

                continue;
            }
            else
            {
                fseek (From, -1, SEEK_CUR);
                c = '*';
            }
        }

        if (EndComment == false && MoveComment == false && c == '/')
        {
            if (fread (&c, sizeof (c), 1, From) && c == '/')
            {
                EndComment = true;

                if (!fread (&c, sizeof (c), 1, From)) return;

                continue;
            }
            else
            {
                fseek (From, -1, SEEK_CUR);
                c = '/';
            }
        }
        if (EndComment == true && MoveComment == false && c == '\n')
        {
            EndComment = false;
        }

        if (EndComment == false && MoveComment == false)
        {
            if (c == '\'')
            {
                if (!ReadSpecialSymbol (From, To, '\''))
                {
                    if (!fread (&c, sizeof (c), 1, From)) return;

                    if (c == '\'')
                    {
                        if (!fread (&c, sizeof (c), 1, From)) return;
                    }
                }

                continue;
            }

            if (c == '"')
            {
                fprintf (To, "{");

                while (ReadSpecialSymbol(From, To, '"'))
                {
                    fprintf (To, ", ");
                }

                fprintf (To, "0}");

                if (!fread (&c, sizeof (c), 1, From)) return;

                continue;
            }

            if (c == '#')
            {
                string Word;
                EndComment = true;

                if (!ReadWord (From, Word, &line)) return;

                if (Word == "include")
                {
                    if (!ReadWord (From, Word, &line)) return;

                    if (Word [0] == '"')
                    {
                        Word.erase (Word.begin ());
                        Word.erase (Word.end () - 1);
                    }

                    string FileName = FromPath;
                    FileName += Word;

                    FILE* File = fopen (FileName.c_str (), "rb");
                    if (!File) throw TH_ERROR "Unknown #include %s", FileName.c_str ());

                    fprintf (To, "# %d \"%s\"\n", 0, FileName.c_str ());

                    Preprocess (File, To, FromPath);

                    if (c == '\n')
                    {
                        fprintf (To, "\n");
                        line ++;
                    }

                    fprintf (To, "\n# %d \"%s\"", line, "main");

                    fclose (File);

                    continue;
                }

                continue;
            }

            if (c != '\n'&& c != '\r')
            {
                fprintf (To, "%c", c);
            }
        }

        if (!fread (&c, sizeof (c), 1, From)) return;
    }
}

bool ReadSpecialSymbol (FILE* From, FILE* To, const char StopSymbol)
{

    char c = 0;
    if (fread (&c, sizeof (c), 1, From) != 1) return true;

    if (c == StopSymbol) return true;

    if (c == '\\')
    {
        if (fread (&c, sizeof (c), 1, From) != 1) return true;

        if (c == 'n') fprintf (To, "%d", '\n');
        if (c == 'r') fprintf (To, "%d", '\r');
        if (c == 't') fprintf (To, "%d", '\t');
        if (c == 'v') fprintf (To, "%d", '\v');

        if (c == '\\') fprintf (To, "%d", '\\');
        if (c == '\'') fprintf (To, "%d", '\'');
        if (c == '\"') fprintf (To, "%d", '\"');

        if ('0' <= c && c <= '9')
        {
            int n = c - '0';
            for (int i = 0; i < 2; i ++)
            {
                if (fread (&c, sizeof (c), 1, From) != 1) break;
                n *= 10;
                n += c - '0';
            }

            fprintf (To, "%d", n);
        }

        return false;
    }

    fprintf (To, "%d", c);

    return false;
}

#endif
