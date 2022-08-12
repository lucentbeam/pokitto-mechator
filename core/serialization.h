#ifndef SERIALIZATION_H
#define SERIALIZATION_H

#ifndef DESKTOP_BUILD
#include "Pokitto.h"
#include <File>
#else
#include <fstream>
#include <iostream>
#endif

namespace Serialization {

#ifndef DESKTOP_BUILD

    template<typename S>
    static bool tryStore(const char * fname, const S * data) {
        File file;
        if (!file.openRW(fname, true, false)) {
            return false;
        }
        file.write(data, sizeof(S));
        return true;
    }

    template<typename S>
    static bool tryAppend(const char * fname, const S * data, int count) {
        File file;
        if (!file.openRW(fname, false, true)) {
            return false;
        }
        file.write(data, sizeof(S) * count);
        return true;
    }

    template<typename S>
    static bool tryGet(const char * fname, S * data) {
        File file;
        if (!file.openRO(fname)) {
            return false;
        }
        file.read(data, sizeof(S));
        return true;
    }

    template<typename S>
    static bool tryGet(const char * fname, S * data, int offset, size_t size) {
        File file;
        if (!file.openRO(fname)) {
            return false;
        }
        file.seek(offset);
        file.read(data, sizeof(S) * size);
        return true;
    }
#else

    template<typename S>
    static bool tryStore(const char * fname, const S * data) {
        std::cout << "Storing " << fname << std::endl;
        std::ofstream file;
        file.open(fname, std::ios::binary | std::ios::out);
        if (!file.is_open()) {
            std::cout << "Error: Could not open file " << fname << std::endl;
            return false;
        }
        std::cout << "dumping " << sizeof(S) << " bytes" << std::endl;
        file.write(reinterpret_cast<const char*>(data), sizeof(S));
        file.close();
        return true;
    }

    template<typename S>
    static bool tryAppend(const char * fname, const S * data, size_t count = 1) {
        std::cout << "Appending to " << fname << std::endl;
        std::ofstream file;
        file.open(fname, std::ios::binary | std::ios::app);
        if (!file.is_open()) {
            std::cout << "Error: Could not open file " << fname << std::endl;
            return false;
        }
        std::cout << "dumping " << sizeof(S) * count << " bytes" << std::endl;
        file.write(reinterpret_cast<const char*>(data), sizeof(S) * count);
        file.close();
        return true;
    }

    template<typename S>
    static bool tryGet(const char * fname, S * data) {
        std::cout << "Fetching " << fname << std::endl;
        std::ifstream file;
        file.open(fname, std::ios::binary | std::ios::in);
        if (!file.is_open()) {
            std::cout << "Error: Could not open file " << fname << std::endl;
            return false;
        }
        std::cout << "reading " << sizeof(S) << " bytes" << std::endl;
        file.read(reinterpret_cast<char*>(data), sizeof(S));
        file.close();
        return true;
    }

    template<typename S>
    static bool tryGet(const char * fname, S * data, int offset, size_t size) {
        std::cout << "Fetching with offset from " << fname << std::endl;
        std::ifstream file;
        file.open(fname, std::ios::binary | std::ios::in);
        if (!file.is_open()) {
            std::cout << "Error: Could not open file " << fname << std::endl;
            return false;
        }
        file.seekg(offset, std::ios::cur);
        std::cout << "reading " << sizeof(S) * size << " bytes with offset " << offset << std::endl;
        file.read(reinterpret_cast<char*>(data), sizeof(S) * size);
        file.close();
        return true;
    }

#endif
};

#endif // SERIALIZATION_H
