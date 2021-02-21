#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <iterator>

#include <vector>

#include "json.hpp"

template <class Container>
void split(const std::string& str, Container& cont, char delim = ' ')
{
    std::stringstream ss(str);
    std::string token;
    while (std::getline(ss, token, delim)) {
        cont.push_back(token);
    }
}

std::string getFileName(char * path) {
    std::vector<std::string> words;
    split<std::vector<std::string>>(path, words, '\\');
    std::string last = words.back();
    words.clear();
    split<std::vector<std::string>>(last, words, '.');
    return words.front();
}

int main(int argc, char *argv[])
{
    if (argc < 2) {
        std::cout << "ERROR: Requires input file argument.\n";
        return 1;
    }

    std::string fname = getFileName(argv[1]);
    std::ofstream file(fname + ".h");

    file << "#pragma once\n\n#include <cstdint>\n\n";

    nlohmann::json json;
    std::ifstream input(argv[1]);
    input >> json;

    for (int i = 0; i < json["layers"].size(); i++) {
        if (!json["layers"][i]["data"].is_null()) {
            file << "const uint8_t " << json["layers"][i]["name"].get<std::string>() << "[] = {";
            file << json["layers"][i]["width"].get<int>() << ", " << json["layers"][i]["height"].get<int>() << ", ";
            for(int j = 0; j < json["layers"][i]["data"].size(); j++) {
                file << json["layers"][i]["data"][j].get<int>()-1 << ", ";
            }
            file << "};\n\n";
        }
    }

    return 0;
}
