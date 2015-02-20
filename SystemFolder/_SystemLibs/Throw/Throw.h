#ifndef THROW__
#define THROW__

#include "cstdio"
#include "cstdarg"

#define TH_ERROR newThrowError (__FILE__, __LINE__, __PRETTY_FUNCTION__,

class newThrowError
{
    public:
    char ErrorText_[1000];

    newThrowError (const char* File, const int Line, const char* Func, const char* MessageFormat, ...);
};

//------------------------------------------------------------------------------

newThrowError:: newThrowError (const char* File, const int Line, const char* Func, const char* MessageFormat, ...)
{
    int Size = sizeof (ErrorText_) - 1;
    int n    = Size;

    n -= _snprintf (ErrorText_ + Size - n, n, "<<<<<<<<<<<<<<<<%s>>>>>>>>>>>>>\n", PROGRAM_NAME);
    //n -= _snprintf (ErrorText_ + Size - n, n, "    ERROR!    \n");
    n -= _snprintf (ErrorText_ + Size - n, n, "vvvvvvvvvvvvvvvvvvvvvvvvvvvvv\n");

    #ifdef DEBUG
    n -= _snprintf (ErrorText_ + Size - n, n, "Error pos:\n");
    n -= _snprintf (ErrorText_ + Size - n, n, "    File: %s\n", File);
    n -= _snprintf (ErrorText_ + Size - n, n, "    Line: %d\n", Line);
    n -= _snprintf (ErrorText_ + Size - n, n, "    Func: %s\n", Func);
    #endif

    n -= _snprintf (ErrorText_ + Size - n, n, "Message: \"");

    va_list Params;
    va_start (Params, MessageFormat);

    n -= _vsnprintf (ErrorText_ + Size - n, n, MessageFormat, Params);

    va_end (Params);

    n -= _snprintf (ErrorText_ + Size - n, n, "\"\n");

    n -= _snprintf (ErrorText_ + Size - n, n, "^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n");
    n -= _snprintf (ErrorText_ + Size - n, n, "\n");
}

#endif
