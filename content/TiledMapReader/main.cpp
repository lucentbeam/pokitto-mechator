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

std::vector<int> barracks_indices;
std::vector<int> blueprint_indices;
std::vector<int> extra_data;
std::vector<int> extra_data_indices;
const int index_span = 1000; // ugh. how wide should the "pseudo map" be so that we can store x/y pairs as a single index (across multiple sub maps)

void appendLevelElements(std::ofstream &door_file, std::ofstream &file, nlohmann::json &json, int dx, int dy) {

    // first pass: all elements with extra data
    for (int i = 0; i < json["objects"].size(); i++) {
        std::vector<std::string> name;
        split(json["objects"][i]["name"], name, '_');
        int xloc = std::floor((json["objects"][i]["x"].get<float>() + json["objects"][i]["width"].get<float>() / 2.0f ) / 6.0f) + dx;
        int yloc = std::floor((json["objects"][i]["y"].get<float>() + json["objects"][i]["height"].get<float>() / 2.0f ) / 6.0f) + dy;

        std::string leading = "\tSpawnPoint({";
        leading += std::to_string(xloc);
        leading += ", ";
        leading += std::to_string(yloc);
        leading += "}, SpawnPoint::";

        if (name[0] == "shop") {
            file << leading << "Shop";
            file << "),\n";
            extra_data_indices.push_back(extra_data.size());
            extra_data.push_back(xloc + std::stoi(name[1]));
            extra_data.push_back(yloc + std::stoi(name[2]));
            extra_data.push_back(xloc + std::stoi(name[3]));
            extra_data.push_back(yloc + std::stoi(name[4]));
            extra_data.push_back(xloc + std::stoi(name[5]));
            extra_data.push_back(yloc + std::stoi(name[6]));
        } else if (name[0] == "barracks") {
            int x = xloc + std::stoi(name[1]);
            int y = yloc + std::stoi(name[2]);
            file << leading << "EnemyBarracks";
            file << "),\n";
            barracks_indices.push_back(x + y * index_span);
            extra_data_indices.push_back(extra_data.size());
            extra_data.push_back(x);
            extra_data.push_back(y);
            extra_data.push_back(std::stoi(name[3]));
            extra_data.push_back(std::stoi(name[4]));
        } else if (name[0] == "door") {
            int x = xloc + std::stoi(name[2]);
            int y = yloc + std::stoi(name[3]);
            int w = std::stoi(name[4]);
            int h = std::stoi(name[5]);
            int t = name.size() > 6 ? std::stoi(name[6]) : 184;
            if (name[1] == "a") {
                file << leading << "DoorA";
                file << "),\n";
            } else if (name[1] == "b") {
                file << leading << "DoorB";
                file << "),\n";
            } else if (name[1] == "c") {
                file << leading << "DoorC";
                file << "),\n";
            } else if (name[1] == "none") {
                file << leading << "DoorNone";
                file << "),\n";
            }
            extra_data_indices.push_back(extra_data.size());
            extra_data.push_back(x);
            extra_data.push_back(y);
            extra_data.push_back(w);
            extra_data.push_back(h);
            extra_data.push_back(t);
        } else if (name[0] == "enemy") {
            if (name[1] == "lasers") {
                int x = std::floor(json["objects"][i]["x"].get<float>() / 6.0f) + dx;
                int y = std::floor(json["objects"][i]["y"].get<float>() / 6.0f) + dy;
                int w = json["objects"][i]["width"].get<float>() / 6.0f;
                w++;
                int h = json["objects"][i]["height"].get<float>() / 6.0f;
                h++;

                leading = "\tSpawnPoint({";
                leading += std::to_string(x);
                leading += ", ";
                leading += std::to_string(y);
                leading += "}, SpawnPoint::";
                extra_data_indices.push_back(extra_data.size());
                if (name[2] == "ns") {
                    file << leading << "LasersVert";
                    file << "),\n";
                    extra_data.push_back(h);
                } else {
                    file << leading << "LasersHoriz";
                    file << "),\n";
                    extra_data.push_back(w);
                }
            } else if (name[1] == "watermine") {
                int w = json["objects"][i]["width"].get<float>() / 6.0f;
                w++;
                int h = json["objects"][i]["height"].get<float>() / 6.0f;
                h++;
                file << leading << "WaterMine";
                file << "),\n";
                extra_data_indices.push_back(extra_data.size());
                extra_data.push_back(w);
                extra_data.push_back(h);
            }
        }
    }

    // second pass: all elements without additional data
    for (int i = 0; i < json["objects"].size(); i++) {
        std::vector<std::string> name;
        split(json["objects"][i]["name"], name, '_');
        int xloc = std::floor((json["objects"][i]["x"].get<float>() + json["objects"][i]["width"].get<float>() / 2.0f ) / 6.0f) + dx;
        int yloc = std::floor((json["objects"][i]["y"].get<float>() + json["objects"][i]["height"].get<float>() / 2.0f ) / 6.0f) + dy;

        std::string leading = "\tSpawnPoint({";
        leading += std::to_string(xloc);
        leading += ", ";
        leading += std::to_string(yloc);
        leading += "}, SpawnPoint::";

        if (name[0] == "hackingkit") {
            file << leading << "HackingKit";
            file << "),\n";
        } else if (name[0] == "keycard") {
            if (name[1] == "a") {
                file << leading << "KeyA";
                file << "),\n";
            } else if (name[1] == "b") {
                file << leading << "KeyB";
                file << "),\n";
            } else if (name[1] == "c") {
                file << leading << "KeyC";
                file << "),\n";
            }
        } else if (name[0] == "enemy") {
            if (name[1] == "mech") {
                file << leading << "Mech";
                file << "),\n";
            } else if (name[1] == "turret") {
                file << leading << "Turret";
                file << "),\n";
            } else if (name[1] == "disabledturret") {
                file << leading << "TurretDisabled";
                file << "),\n";
            } else if (name[1] == "tank") {
                file << leading << "Tank";
                file << "),\n";
            } else if (name[1] == "mine") {
                file << leading << "Mine";
                file << "),\n";
            } else if (name[1] == "bomber") {
                file << leading << "Bomber";
                file << "),\n";
            } else if (name[1] == "heli") {
                file << leading << "Helicopter";
                file << "),\n";
            } else if (name[1] == "boat") {
                file << leading << "Boat";
                file << "),\n";
            }
        } else if (name[0] == "blueprint") {
            file << leading << "Blueprint";
            file << "),\n";
            int x = std::floor(json["objects"][i]["x"].get<float>() / 6.0f);
            int y = std::floor(json["objects"][i]["y"].get<float>() / 6.0f);
            int idx = std::stoi(name[1]);
            while (blueprint_indices.size() <= idx) {
                blueprint_indices.push_back(0);
            }
            blueprint_indices[idx] = x + dx + (y + dy) * index_span;
        }
    }
}

std::string getWorldDataRLE(nlohmann::json &json, std::string asname) {
    std::string file;

    file += "const uint8_t " + asname + "[] = {\n\t";

    int last_color = json["data"][0].get<int>()-1;
    int counter = 0;
    int w_count = json["width"].get<int>();
    int width = json["width"].get<int>();
    int height = json["height"].get<int>();
    file += std::to_string(width) + ", " + std::to_string(json["height"].get<int>()) + ", ";
    int idx_ct = 0;
    std::string header;
    for(int j = 0; j < json["data"].size(); ++j) {
        int idx = json["data"][j].get<int>() - 1;
        if (idx == last_color) {
            ++counter;
        } else {
            while (counter > 0) {
                int count = std::min(w_count, counter);
                file += std::to_string(last_color) + std::string(", ") + std::to_string(count) + std::string(", ");
                counter -= count;
                w_count -= count;
                idx_ct += 2;
                if (w_count <= 0) {
                    if (w_count < 0) {
                        std::cout << "WARNING! w_count is " << w_count << std::endl;
                    }
                    header += std::to_string(idx_ct) + ", ";
                    w_count += width;
                    --height;
                }
            }
            counter = 1;
            last_color = idx;
        }
    }
    while (counter > 0) {
        int count = std::min(w_count, counter);
        file += std::to_string(last_color) + std::string(", ") + std::to_string(count) + std::string(", ");
        counter -= count;
        w_count -= count;
        idx_ct += 2;
        if (w_count <= 0) {
            if (w_count < 0) {
                std::cout << "WARNING! w_count is " << w_count << std::endl;
            }
            header += std::to_string(idx_ct) + ", ";
            w_count += width;
            --height;
        }
    }
    while (height > 0) {
        header += std::to_string(0) + ", " + std::to_string(width) + ", ";
        --height;
    }
    file += "\n};\n\nconst uint16_t " + asname + "_indices[] = {\n\t";
    file += header + "\n};\n\n";

    return file;
}

// pass in the file that contains the world data; mutable indices need to be linked with its array
void processMutableData(std::ofstream &file, nlohmann::json &mutables, nlohmann::json &world, std::string asname, int mapwidth) {
//    file << "const uint8_t can_tile_mutate[] = {";
    std::vector<int> defaults, indices;
    std::cout << "Getting indices" << std::endl;
    for(int j = 0; j < mutables.size(); j++) {
        bool can_mutate = mutables[j].get<int>() > 0;
        if (can_mutate) {
            defaults.push_back(world[j].get<int>() - 1);
            indices.push_back(j);
        }
    }

    std::cout << "Got indices " << std::endl;
    std::vector<int> mutable_starts;
    file << "const uint16_t " << asname << "_last_mutable_index = " << indices.back() << ";\n";
    file << "const uint16_t " << asname << "_mutable_indices[] = { ";
    int ct = 0;
    for(int t : indices) {
        file << t << ", ";
        while (int(t/(mapwidth * 8)) >= mutable_starts.size()) { // every 8 y-values, record the location of the first index to appear
            mutable_starts.push_back(ct);
        }
        ct++;
    }

    file << "};\n\nconst uint16_t " << asname << "_mutable_index_indices[] = { ";
    for(int t : mutable_starts) {
        file << t << ", ";
    }
    file << "};\n\nstatic uint8_t " << asname << "_current_tiles[] = { ";
    for(int t : defaults) {
        file << t << ", ";
    }
    file << "};\n\nconst uint8_t " << asname << "_default_tiles[] = { ";
    for(int t : defaults) {
        file << t << ", ";
    }
    file << "};\n\n";
}

void dumpBarracksAndBlueprintData(std::ofstream &file) {

    file << "\n\nconst int barracks_indices[] = { ";
    for(int t : barracks_indices) {
        file << t << ", ";
    }

    file << "};\nconst int barracks_count = " << barracks_indices.size();

    file << ";\n\nconst int blueprints_indices[] = { ";
    for(int t : blueprint_indices) {
        file << t << ", ";
    }

    file << "};\n\nstatic uint8_t barracks_data[] = { ";
    for(int t : barracks_indices) {
        file << "0, ";
    }

    file << "};\n\nstatic int16_t spawnpoint_data[] = { ";
    for(int t : extra_data) {
        file << t << ", ";
    }

    file << "};\n\nstatic uint16_t spawnpoint_data_indices[] = { ";
    for(int t : extra_data_indices) {
        file << t << ", ";
    }
    file << "};\nconst int spawnpoint_data_count = " << extra_data_indices.size();
    file << ";\n\n";

}

void processWorldSprite(std::ofstream &file, nlohmann::json &layer)
{
    const int spacing = 4;
    const int target_size = 54;

    int width = layer["width"].get<int>();
//    int height = layer["height"].get<int>();

    file << "#ifndef _MAPSPRITE\n#define _MAPSPRITE\n\n#include <cstdint>\n\n";
    file << "const uint8_t sprite_map[] = {\n\t" << target_size << ", " << target_size << ", ";
    for(int j = 0; j < layer["data"].size(); j++) {
        int tile = layer["data"][j].get<int>()-1;
        int x = (j % width) - spacing/2;
        int y = (j / width) - spacing/2;
        if (x % spacing == 0 && y % spacing == 0) {
            uint8_t color = 1;
            if (tile == 19) { // pure water
                color = 1;
            } else if (tile < 53) { // other water tiles
                color = 2;
            } else if (tile >= 162) { // base
                color = 4;
            } else {
                color = 3; // ground/other
            }
            file << int(color) << ", ";
        }
    }
    file << "};\n\n";
    file << "#endif\n";
}

void showHelp() {
    std::cout << "ERROR: Requires argument count modulo 4. Format is:\n";
    std::cout << "\t<file name> <output array name> <delta x> <delta y>...\n";
}

struct FileInfo {
    std::string aname; // name to give the tile data array
    nlohmann::json tiles, tiles_obj, mutables, elements;
    bool has_mutables = false;
    int dx, dy;
    FileInfo(const char * path, const char * array, const char * x, const char * y) {
        dx = std::stoi(std::string(x));
        dy = std::stoi(std::string(y));
        aname = array;

        nlohmann::json json;
        std::ifstream(path) >> json;
        for (int i = 0; i < json["layers"].size(); i++) {
            if (json["layers"][i]["name"] == "world") {
                tiles = json["layers"][i]["data"];
                tiles_obj = json["layers"][i];
            } else if (json["layers"][i]["name"] == "mutable") {
                mutables = json["layers"][i]["data"];
                has_mutables = true;
            } else if (json["layers"][i]["name"] == "level_elements") {
                elements = json["layers"][i];
            }
        }
    }
};

void processWorldLayers(std::vector<FileInfo> &files) {
    std::ofstream file("worldtiles.h");

    file << "#pragma once\n\n#include <cstdint>\n\n";

    for(FileInfo &f : files) {
        file << "\n\n";
        file << "const int delta_x_" << f.aname << " = " << f.dx << ";\n";
        file << "const int delta_y_" << f.aname << " = " << f.dy << ";\n\n";
        std::cout << f.aname << " world RLE!" << std::endl;
        file << getWorldDataRLE(f.tiles_obj, f.aname);
        std::cout << f.aname << " mutables!" << std::endl;
        if (f.has_mutables) processMutableData(file, f.mutables, f.tiles, f.aname, f.tiles_obj["width"].get<int>());
    }

    dumpBarracksAndBlueprintData(file);
}

void processLevelElements(std::vector<FileInfo> &files) {
    std::ofstream elements("spawnpoints.h");
    elements << "#pragma once\n\n#include \"core/utilities/vec.h\"\n#include \"game/entities/enemy.h\"\n#include \"game/entities/barracks.h\"\n#include \"game/entities/pickups.h\"\n#include \"game/entities/pois.h\"\n#include \"game/maps/spawnpoint.h\"\n\n";
    elements << "const SpawnPoint points[] = {\n";

    std::ofstream doors("doors.h");
    doors << "#pragma once\n\n#include \"core/utilities/vec.h\"\n#include \"game/entities/enemy.h\"\n#include \"game/entities/barracks.h\"\n#include \"game/entities/pickups.h\"\n#include \"game/entities/pois.h\"\n#include \"game/maps/spawnpoint.h\"\n\n";
    doors << "const SpawnPoint doors[] = {\n";

    for(FileInfo &f : files) {
        appendLevelElements(doors, elements, f.elements, f.dx, f.dy);
    }

    elements << "};\nconst int point_count = sizeof(points)/sizeof(SpawnPoint);\n\n";
    doors << "};\nconst int door_count = sizeof(doors)/sizeof(SpawnPoint);\n\n";
}

void processWorldMap(std::vector<FileInfo> &files) {

//    const int spacing = 4;
//    const int target_size = 54;

//    int width = layer["width"].get<int>();
////    int height = layer["height"].get<int>();

//    file << "#ifndef _MAPSPRITE\n#define _MAPSPRITE\n\n#include <cstdint>\n\n";
//    file << "const uint8_t sprite_map[] = {\n\t" << target_size << ", " << target_size << ", ";
//    for(int j = 0; j < layer["data"].size(); j++) {
//        int tile = layer["data"][j].get<int>()-1;
//        int x = (j % width) - spacing/2;
//        int y = (j / width) - spacing/2;
//        if (x % spacing == 0 && y % spacing == 0) {
//            uint8_t color = 1;
//            if (tile == 19) { // pure water
//                color = 1;
//            } else if (tile < 53) { // other water tiles
//                color = 2;
//            } else if (tile >= 162) { // base
//                color = 4;
//            } else {
//                color = 3; // ground/other
//            }
//            file << int(color) << ", ";
//        }
//    }
//    file << "};\n\n";
//    file << "#endif\n";
}

int main(int argc, char *argv[])
{
    if (argc < 5 || ((argc-1) % 4) != 0) {
        showHelp();
        return 1;
    }
    int fcount = (argc - 1) / 4;
    std::vector<FileInfo> files;
    for(int i = 0; i < fcount; ++i) {
        files.push_back(FileInfo(argv[i*4+1],argv[i*4+2],argv[i*4+3],argv[i*4+4]));
    }
    std::cout << "Elements!" << std::endl;
    processLevelElements(files);
    std::cout << "Layers!" << std::endl;
    processWorldLayers(files);
    std::cout << "Map!" << std::endl;
    processWorldMap(files);
    return 0;

//    std::string fname = getFileName(argv[1]);
//    std::ofstream file(fname + ".h");

//    file << "#pragma once\n\n#include <cstdint>\n\n";

//    std::ofstream elements("spawnpoints.h");
//    elements << "#pragma once\n\n#include \"core/utilities/vec.h\"\n#include \"game/entities/enemy.h\"\n#include \"game/entities/barracks.h\"\n#include \"game/entities/pickups.h\"\n#include \"game/entities/pois.h\"\n#include \"game/maps/spawnpoint.h\"\n\n";
//    elements << "const SpawnPoint points[] = {\n";

//    std::ofstream doors("doors.h");
//    doors << "#pragma once\n\n#include \"core/utilities/vec.h\"\n#include \"game/entities/enemy.h\"\n#include \"game/entities/barracks.h\"\n#include \"game/entities/pickups.h\"\n#include \"game/entities/pois.h\"\n#include \"game/maps/spawnpoint.h\"\n\n";
//    doors << "const SpawnPoint doors[] = {\n";

//    nlohmann::json json;
//    std::ifstream input(argv[1]);
//    input >> json;

//    bool has_mutable = false;
//    nlohmann::json world_layer, mutable_layer, map_layer;
//    for (int i = 0; i < json["layers"].size(); i++) {
//        if (json["layers"][i]["name"] == "world") {
//            processWorldDataRLE(file, json["layers"][i]);
//            world_layer = json["layers"][i]["data"];
//            map_layer = json["layers"][i];
//        } else if (json["layers"][i]["name"] == "mutable") {
//            mutable_layer = json["layers"][i]["data"];
//            has_mutable = true;
//        } else if (json["layers"][i]["name"] == "level_elements") {
//            processLevelElements(doors, elements, json["layers"][i]);
//        } else if (!json["layers"][i]["data"].is_null()) {
//            processWorldDataRLE(file, json["layers"][i]);
//        }
//    }

//    elements << "};\nconst int point_count = sizeof(points)/sizeof(SpawnPoint);\n\n";
//    doors << "};\nconst int door_count = sizeof(doors)/sizeof(SpawnPoint);\n\n";

//    if (has_mutable) {
//        std::ofstream muts("worldmutables.h");
//        muts <<  "#pragma once\n\n#include <cstdint>\n\n";
//        processMutableData(muts, mutable_layer, world_layer);
//    }

//    std::ofstream mapspr("mapsprite.h");
//    processWorldSprite(mapspr, map_layer);

    return 0;
}
