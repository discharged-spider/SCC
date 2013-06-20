#ifndef HELP_H__
#define HELP_H__

#include "cstdio"
#include "cstdlib"
#include "cstring"
#include "string"

bool ReadWord (FILE* file, char Word [], string way);

bool ReadShekspearWord (FILE* file, char Word [], string Way);

bool InList (const char ListName [], char Word [], string Way);

void ReadTo (FILE* file, const char Str []);

void ReadSentense (FILE* file, string Way);

//------------------------------------------------------------------------------

bool ReadWord (FILE* file, char Word [], string Way)
{
    bool Start = false;
    char c     = 0;
    if (fread (&c, sizeof (c), 1, file) != 1) return false;
    if (isupper(c)) c = tolower (c);

    int i = 0;
    for (; ; i ++)
    {
        if (c == '.' || c == '!' || c == '?' || c == ',' || c == '[' || c == ']' || c == ':')
        {
            if (i == 0)
            {
                Word [i] = c;

                i ++;

                Word [i] = 0;
            }
            else
            {
                fseek (file, -1, SEEK_CUR);

                Word [i] = 0;
            }

            break;
        }

        if (c == ' ' || c == '\n' || c == '\r' || c == '\0')
        {
           if (Start)
           {
               Word [i] = 0;

               if (i == 0) return false;

               break;
           }
           else i --;
        }
        else
        {
            if (!Start) Start = true;
            Word [i] = c;
        }

        if (fread (&c, sizeof (c), 1, file) != 1)
        {
            i ++;
            Word [i] = 0;
            break;
        }
        if (isupper(c)) c = tolower (c);
    }

    if (i > 0 && InList ("skip", Word, Way)) return ReadWord (file, Word, Way);

    return i > 0;
}

bool ReadShekspearWord (FILE* file, char Word [], string Way)
{
    if (!ReadWord (file, Word, Way)) return false;

    if (InList ("system", Word, Way)) return true;

    if (InList ("be", Word, Way)) return true;
    if (InList ("character", Word, Way)) return true;
    if (InList ("first_person", Word, Way)) return true;
    if (InList ("first_person_possessive", Word, Way)) return true;
    if (InList ("first_person_reflexive", Word, Way)) return true;
    if (InList ("negative_adjective", Word, Way)) return true;
    if (InList ("negative_comparative", Word, Way)) return true;
    if (InList ("negative_noun", Word, Way)) return true;
    if (InList ("neutral_adjective", Word, Way)) return true;
    if (InList ("neutral_noun", Word, Way)) return true;
    if (InList ("nothing", Word, Way)) return true;
    if (InList ("places", Word, Way)) return true;
    if (InList ("positive_adjective", Word, Way)) return true;
    if (InList ("positive_comparative", Word, Way)) return true;
    if (InList ("positive_noun", Word, Way)) return true;
    if (InList ("second_person", Word, Way)) return true;
    if (InList ("second_person_possessive", Word, Way)) return true;
    if (InList ("second_person_reflexive", Word, Way)) return true;
    if (InList ("sentence_end", Word, Way)) return true;
    if (InList ("third_person_possessive", Word, Way)) return true;

    #ifdef DEBUG
        printf ("Word %s - not simple shekspear word\n", Word);
    #endif

    int Pos = ftell (file);

    string WholeWord = Word;
    if (!ReadWord (file, Word, Way)) return false;
    WholeWord += " ";
    WholeWord += Word;

    strcpy (Word, WholeWord.c_str());

    if (InList ("be", Word, Way)) return true;
    if (InList ("character", Word, Way)) return true;
    if (InList ("first_person", Word, Way)) return true;
    if (InList ("first_person_possessive", Word, Way)) return true;
    if (InList ("first_person_reflexive", Word, Way)) return true;
    if (InList ("negative_adjective", Word, Way)) return true;
    if (InList ("negative_comparative", Word, Way)) return true;
    if (InList ("negative_noun", Word, Way)) return true;
    if (InList ("neutral_adjective", Word, Way)) return true;
    if (InList ("neutral_noun", Word, Way)) return true;
    if (InList ("nothing", Word, Way)) return true;
    if (InList ("places", Word, Way)) return true;
    if (InList ("positive_adjective", Word, Way)) return true;
    if (InList ("positive_comparative", Word, Way)) return true;
    if (InList ("positive_noun", Word, Way)) return true;
    if (InList ("second_person", Word, Way)) return true;
    if (InList ("second_person_possessive", Word, Way)) return true;
    if (InList ("second_person_reflexive", Word, Way)) return true;
    if (InList ("sentence_end", Word, Way)) return true;
    if (InList ("third_person_possessive", Word, Way)) return true;

    fseek (file, Pos, 0);

    return false;
}

bool InList (const char ListName [], char Word [], string Way)
{
    Way += "..\\Lists\\";
    Way += ListName;
    Way += ".wordlist";

    FILE* List = fopen (Way.c_str (), "rb");

    if (!List) throw TH_ERROR "List %s don't exist!", ListName);

    char WordC = (isupper(Word [0]))? tolower (Word [0]) : toupper (Word [0]);

    char c = 0;
    if (fread (&c, sizeof (c), 1, List) != 1) return false;
    while (true)
    {
        if (c == Word [0] || c == WordC)
        {
            for (int i = 1; true; i ++)
            {
                if (fread (&c, sizeof (c), 1, List) != 1 || c == '\n' || c == '\r')
                {
                    if (Word [i] == 0)
                    {
                        fclose (List);
                        return true;
                    }
                    else break;
                }

                WordC = (isupper(Word [i]))? tolower (Word [i]) : toupper (Word [i]);
                if (c != Word [i] && c != WordC) break;
            }

            WordC = (isupper(Word [0]))? tolower (Word [0]) : toupper (Word [0]);
        }
        else
        {
            while (c != '\n' && c != '\r')
            {
                if (fread (&c, sizeof (c), 1, List) != 1) break;
            }
        }

        if (fread (&c, sizeof (c), 1, List) != 1) break;
    }

    fclose (List);

    return false;
}

void ReadTo (FILE* file, const char Str [])
{
    char c = 0;
    if (fread (&c, sizeof (c), 1, file) != 1) return ;
    while (true)
    {
        if (c == Str [0])
        {
            for (int i = 1; true; i ++)
            {
                if (Str [i] == 0)
                {
                    return ;
                }

                if (fread (&c, sizeof (c), 1, file) != 1) break;

                if (c == '\r')
                {
                    i --;
                    continue;
                }

                if (c != Str [i]) break;
            }
        }

        if (fread (&c, sizeof (c), 1, file) != 1) break;
    }

    return ;
}

void ReadSentense (FILE* file, string Way)
{
    char c[2] = "A";
    while (fread (&c[0], sizeof (c[0]), 1, file) == 1)
    {
        if (c[0] == '\n' || c[0] == '\r' || c[0] == ' ') continue;
        if (InList ("sentence_end", c, Way)) break;
    }
}

#endif
