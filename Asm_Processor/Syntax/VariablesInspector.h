#ifndef VARIABLES_INSPECTOR__
#define VARIABLES_INSPECTOR__

#include "stdio.h"
#include "map"

using std::map;

//==============================================================================

struct newVariableInfo
{
    int get_count;
    int set_count;

    void save (FILE* to);
    void load (FILE* from);
};

void newVariableInfo::save (FILE* to)
{
    fwrite (&get_count, sizeof (int), 1, to);
    fwrite (&set_count, sizeof (int), 1, to);
}
void newVariableInfo::load (FILE* from)
{
    fread (&get_count, sizeof (int), 1, from);
    fread (&set_count, sizeof (int), 1, from);
}

//==============================================================================

class newVariablesInspector
{
    private:
    map <int, newVariableInfo> Vars_;
    map <int, newVariableInfo> Arrs_;

    void get (int i, map <int, newVariableInfo>& base);
    void set (int i, map <int, newVariableInfo>& base);

    void save (FILE* to, map <int, newVariableInfo>& base);
    void load (FILE* from, map <int, newVariableInfo>& base);

    public:

    newVariablesInspector ();

    void get_var (int i);
    void get_arr (int i);
    void set_var (int i);
    void set_arr (int i);

    map <int, newVariableInfo>::iterator get_vars_begin ();
    map <int, newVariableInfo>::iterator get_vars_end ();

    map <int, newVariableInfo>::iterator get_arrs_begin ();
    map <int, newVariableInfo>::iterator get_arrs_end ();

    void save (FILE* to);
    void load (FILE* from);
};

//------------------------------------------------------------------------------

newVariablesInspector::newVariablesInspector () :
    Vars_ (),
    Arrs_ ()
{}

//------------------------------------------------------------------------------

void newVariablesInspector::get (int i, map <int, newVariableInfo>& base)
{
    base [i].get_count ++;
}

void newVariablesInspector::set (int i, map <int, newVariableInfo>& base)
{
    base [i].set_count ++;
}

//------------------------------------------------------------------------------

void newVariablesInspector::get_var (int i)
{
    get (i, Vars_);
}
void newVariablesInspector::get_arr (int i)
{
    get (i, Arrs_);
}

void newVariablesInspector::set_var (int i)
{
    set (i, Vars_);
}
void newVariablesInspector::set_arr (int i)
{
    set (i, Arrs_);
}

//------------------------------------------------------------------------------

map <int, newVariableInfo>::iterator newVariablesInspector::get_vars_begin ()
{
    return Vars_.begin ();
}
map <int, newVariableInfo>::iterator newVariablesInspector::get_vars_end ()
{
    return Vars_.end ();
}

map <int, newVariableInfo>::iterator newVariablesInspector::get_arrs_begin ()
{
    return Arrs_.begin ();
}
map <int, newVariableInfo>::iterator newVariablesInspector::get_arrs_end ()
{
    return Arrs_.end ();
}

//------------------------------------------------------------------------------

void newVariablesInspector::save (FILE* to, map <int, newVariableInfo>& base)
{
    int size = base.size ();
    fwrite (&size, sizeof (int), 1, to);

    for (map <int, newVariableInfo>::iterator it = base.begin (); it != base.end (); it ++)
    {
        fwrite (&(it -> first), sizeof (int), 1, to);
        (it -> second).save (to);
    }
}

void newVariablesInspector::load (FILE* from, map <int, newVariableInfo>& base)
{
    int size = 0;
    fread (&size, sizeof (int), 1, from);

    for (int i = 0; i < size; i ++)
    {
        int id = 0;
        fread (&id, sizeof (int), 1, from);
        base [id].load (from);
    }
}

void newVariablesInspector::save (FILE* to)
{
    save (to, Vars_);
    save (to, Arrs_);
}

void newVariablesInspector::load (FILE* from)
{
    load (from, Vars_);
    load (from, Arrs_);
}

#endif
