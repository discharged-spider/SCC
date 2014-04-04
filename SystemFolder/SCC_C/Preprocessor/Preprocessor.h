#ifndef PREPROCESSOR_H
#define PREPROCESSOR_H

#include "..\\_ToTree\\Assem\\Assem.h"

void Preprocess (FILE* From, FILE* To, const char FromPath [])
{
    bool MoveComment = false;
    bool EndComment  = false;

    char LastC = '\n';
    char c = 0;
    if (fread (&c, sizeof (c), 1, From) != 1) return;

    while (true)
    {
        if (MoveComment == false && EndComment == false && c == '/')
        {
            if (fread (&c, sizeof (c), 1, From) == 1 && c == '*')
            {
                MoveComment = true;

                if (fread (&c, sizeof (c), 1, From) != 1) return;

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
            if (fread (&c, sizeof (c), 1, From) == 1 && c == '/')
            {
                MoveComment = false;
                if (MoveComment < 0) MoveComment = 0;

                if (fread (&c, sizeof (c), 1, From) != 1) return;

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
            if (fread (&c, sizeof (c), 1, From) == 1 && c == '/')
            {
                EndComment = true;

                if (fread (&c, sizeof (c), 1, From) != 1) return;

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
                if (fread (&c, sizeof (c), 1, From) != 1) return;

                if (c != '\\') fprintf (To, "%d", c);
                else
                {
                    if (fread (&c, sizeof (c), 1, From) != 1) return;
                    if (c == 'n') fprintf (To, "%d", '\n');
                    if (c == '\'') fprintf (To, "%d", 39);
                }

                if (fread (&c, sizeof (c), 1, From) != 1) return;

                if (c == 39 && fread (&c, sizeof (c), 1, From) != 1) return;

                continue;
            }

            if (c == '"')
            {
                fprintf (To, "{");

                bool Comment = false;

                while (true)
                {
                    if (fread (&c, sizeof (c), 1, From) != 1) return;

                    if (c == '\\')
                    {
                        if (Comment == true)
                        {
                            Comment = false;

                            fprintf (To, "%d, ", '\\');
                        }
                        else
                        {
                            Comment = true;
                        }
                    }
                    else
                    {
                        if (c == '"')
                        {
                            if (Comment == true)
                            {
                                Comment = false;

                                fprintf (To, "%d, ", '"');
                            }
                            else
                            {
                                fprintf (To, "0}");

                                break;
                            }
                        }
                        else
                        {
                            if (Comment == true)
                            {
                                Comment = false;

                                if (c == 'n') fprintf (To, "%d, ", '\n');

                                if ('0' <= c && c <= '9')
                                {
                                    for (int i = 0; i < 3; i ++)
                                    {
                                        fprintf (To, "%c", c);

                                        if (fread (&c, sizeof (c), 1, From) != 1) return;

                                        if ('0' > c || c > '9' || i == 2)
                                        {
                                            fseek (From, -1, SEEK_CUR);
                                            break;
                                        }
                                    }

                                    fprintf (To, ", ");
                                }
                            }
                            else fprintf (To, "%d, ", c);
                        }
                    }
                }

                if (fread (&c, sizeof (c), 1, From) != 1) return;
                continue;
            }

            if (c == '#')
            {
                char Word [60] = "";
                EndComment = true;

                if (!ReadWord (From, Word)) return;

                if (strcmp (Word, "include") == 0)
                {
                    if (!ReadWord (From, Word)) return;

                    if (Word [0] == '"')
                    {
                        for (int i = 1; Word [i] != 0; i ++)
                        {
                           if (Word [i + 1] == 0) Word [i] = 0;
                           Word [i - 1] = Word [i];
                        }
                    }

                    string FileName = FromPath;
                    FileName += Word;

                    FILE* File = fopen (FileName.c_str (), "rb");
                    if (!File) throw TH_ERROR "Unknown #include %s", FileName.c_str ());

                    Preprocess (File, To, FromPath);

                    fclose (File);

                    fprintf (To, "\n");


                    continue;
                }

                if (strcmp (Word, "end") == 0)
                {
                    return ;
                }

                continue;
            }

            if (LastC == '\n')
            {
                if (c != '\n'&& c != '\r')
                {
                    LastC = c;
                    fprintf (To, "%c", c);
                }
            }
            else
            {
                LastC = c;
                fprintf (To, "%c", c);
            }
        }

        if (fread (&c, sizeof (c), 1, From) != 1) return;
    }
}

#endif
