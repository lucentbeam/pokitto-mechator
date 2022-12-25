#include "debuglog.h"

//bool DebugLog::s_init = false;

//#ifndef DESKTOP_BUILD
//const char * DebugLog::s_file;
//#else
//#include <iostream>
//#endif

//void DebugLog::initialize(const char *fname)
//{
//#ifndef DESKTOP_BUILD
//    s_file = fname;
////    if (!s_file.openRW(fname, false, true)) {
////        return;
////    }
//#endif
//    s_init = true;
//}

//void DebugLog::log(const char *msg)
//{
//    if (!s_init) return;
//#ifndef DESKTOP_BUILD
//    File f;
//    if (f.openRW(s_file, false, true)) {
//        f.write(msg, strlen(msg));
//        f.write("\n", strlen("\n"));
//        f.close();
//    }
//#else
//    std::cout << msg << std::endl;
//#endif
//}
