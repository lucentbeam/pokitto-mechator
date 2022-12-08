#ifndef DEBUGLOG_H
#define DEBUGLOG_H

#ifndef DESKTOP_BUILD
#include "Pokitto.h"
#include <File>
#endif

class DebugLog
{
    static bool s_init;

#ifndef DESKTOP_BUILD
    static const char * s_file;
#endif
public:

    static void initialize(const char * fname);

    static void log(const char * msg);
};

#endif // DEBUGLOG_H
