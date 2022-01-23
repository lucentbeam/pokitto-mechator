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

void processLevelElements(std::ofstream &file, nlohmann::json &json) {
    for (int i = 0; i < json["objects"].size(); i++) {
        std::vector<std::string> name;
        split(json["objects"][i]["name"], name, '_');
        int xloc = (json["objects"][i]["x"].get<float>() + json["objects"][i]["width"].get<float>() / 2.0f ) / 6.0f;
        int yloc = (json["objects"][i]["y"].get<float>() + json["objects"][i]["height"].get<float>() / 2.0f ) / 6.0f;

        std::string leading = "\tSpawnPoint({";
        leading += std::to_string(xloc);
        leading += ", ";
        leading += std::to_string(yloc);
        leading += "}, ";

        if (name[0] == "hackingkit") {
            file << leading << "Pickups::spawnHackingKit),\n";
        } else if (name[0] == "shop") {
            int x1 = xloc + std::stoi(name[1]);
            int y1 = yloc + std::stoi(name[2]);
            int x2 = xloc + std::stoi(name[3]);
            int y2 = yloc + std::stoi(name[4]);
            int x3 = xloc + std::stoi(name[5]);
            int y3 = yloc + std::stoi(name[6]);
            file << leading << "spawnShop<" << x1 << "," << y1 << "," << x2 << "," << y2 << "," << x3 << "," << y3 << ">),\n";
        } else if (name[0] == "barracks") {
            int x = xloc + std::stoi(name[1]);
            int y = yloc + std::stoi(name[2]);
            int w = std::stoi(name[3]);
            int h = std::stoi(name[4]);
            file << leading << "spawnBarracks<" << x << "," << y << "," << w << "," << h << ">),\n";
            barracks_indices.push_back(x + y * 255);
        } else if (name[0] == "door") {
            int x = xloc + std::stoi(name[2]);
            int y = yloc + std::stoi(name[3]);
            int w = std::stoi(name[4]);
            int h = std::stoi(name[5]);
            if (name[1] == "a") {
                file << leading << "spawnDoorA<" << x << "," << y << "," << w << "," << h << ">),\n";
            } else if (name[1] == "b") {
                file << leading << "spawnDoorB<" << x << "," << y << "," << w << "," << h << ">),\n";
            } else if (name[1] == "c") {
                file << leading << "spawnDoorC<" << x << "," << y << "," << w << "," << h << ">),\n";
            } else if (name[1] == "none") {
                file << leading << "spawnDoorNone<" << x << "," << y << "," << w << "," << h << ">),\n";
            }
        } else if (name[0] == "keycard") {
            if (name[1] == "a") {
                file << leading << "Pickups::spawnKeycardA),\n";
            } else if (name[1] == "b") {
                file << leading << "Pickups::spawnKeycardB),\n";
            } else if (name[1] == "c") {
                file << leading << "Pickups::spawnKeycardC),\n";
            }
        } else if (name[0] == "enemy") {
            if (name[1] == "mech") {
                file << leading << "Enemy::spawnMech),\n";
            } else if (name[1] == "turret") {
                file << leading << "Enemy::spawnTurret),\n";
            } else if (name[1] == "disabledturret") {
                file << leading << "Enemy::spawnTurretDisabled),\n";
            } else if (name[1] == "tank") {
                file << leading << "Enemy::spawnTank),\n";
            } else if (name[1] == "mine") {
                file << leading << "Enemy::spawnMine),\n";
            } else if (name[1] == "bomber") {
                file << leading << "Enemy::spawnBomber),\n";
            } else if (name[1] == "lasers") {
                int x = json["objects"][i]["x"].get<float>() / 6.0f;
                int y = json["objects"][i]["y"].get<float>() / 6.0f;
                int w = json["objects"][i]["width"].get<float>() / 6.0f;
                w++;
                int h = json["objects"][i]["height"].get<float>() / 6.0f;
                h++;
                if (name[2] == "ns") {
                    file << leading << "spawnVerticalLasers< " << y << ", " << h <<">),\n";
                } else {
                    file << leading << "spawnHorizontalLasers< " << x << ", " << w <<">),\n";
                }
            } else if (name[1] == "heli") {
                file << leading << "Enemy::spawnHelicopter),\n";
            }
        } else if (name[0] == "blueprint") {
            file << leading << "Pickups::spawnBlueprint),\n";
            int x = json["objects"][i]["x"].get<float>() / 6.0f;
            int y = json["objects"][i]["y"].get<float>() / 6.0f;
            int idx = std::stoi(name[1]);
            while (blueprint_indices.size() <= idx) {
                blueprint_indices.push_back(0);
            }
            blueprint_indices[idx] = x + y * 216;
        }
    }
}

void processWorldDataRLE(std::ofstream &file, nlohmann::json &json) {

    file << "const uint8_t " << json["name"].get<std::string>() << "[] = {\n\t";

    int last_color = json["data"][0].get<int>()-1;
    int counter = 0;
    int w_count = json["width"].get<int>();
    int width = json["width"].get<int>();
    int height = json["height"].get<int>();
    file << width << ", " << json["height"].get<int>() << ", ";
    int idx_ct = 0;
    std::string header;
    for(int j = 0; j < json["data"].size(); ++j) {
        int idx = json["data"][j].get<int>() - 1;
        if (idx == last_color) {
            ++counter;
        } else {
            while (counter > 0) {
                int count = std::min(w_count, counter);
                file << std::to_string(last_color) + std::string(", ") + std::to_string(count) + std::string(", ");
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
        file << std::to_string(last_color) + std::string(", ") + std::to_string(count) + std::string(", ");
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
    file << "\n};\n\nconst uint16_t " << json["name"].get<std::string>() << "_indices[] = {\n\t";
    file << header << "\n};\n\n";
}

void processWorldData(std::ofstream &file, nlohmann::json &json) {
    file << "const uint8_t " << json["name"].get<std::string>() << "[] = {";
    file << json["width"].get<int>() << ", " << json["height"].get<int>() << ", ";
    for(int j = 0; j < json["data"].size(); j++) {
        file << json["data"][j].get<int>()-1 << ", ";
    }
    file << "};\n\n";
}

void processMutableData(std::ofstream &file, nlohmann::json &mutables, nlohmann::json &world) {
//    file << "const uint8_t can_tile_mutate[] = {";
    std::vector<int> defaults;
    std::vector<int> indices;
    std::string val;
    for(int j = 0; j < mutables.size(); j++) {
        bool can_mutate = mutables[j].get<int>() > 0;
        if (can_mutate) {
            defaults.push_back(world[j].get<int>() - 1);
            indices.push_back(j);
            val = "1" + val;
        } else {
            val = "0" + val;
        }
        if ((j % 8) == 7) {
//            file << "0b" << val << ", ";
            val = "";
        }
    }

    std::vector<int> mutable_starts;

//    file << "};\n\nconst uint16_t mutable_indices[] = { ";
    file << "const uint16_t mutable_indices[] = { ";
    int ct = 0;
    for(int t : indices) {
        file << t << ", ";
        while (int(t/(216 * 8)) >= mutable_starts.size()) { // every 8 y-values, record the location of the first index to appear
            mutable_starts.push_back(ct);
        }
        ct++;
    }

    file << "};\n\nconst uint16_t mutable_index_indices[] = { ";
    for(int t : mutable_starts) {
        file << t << ", ";
    }

    file << "};\n\nstatic uint8_t current_tiles[] = { ";
    for(int t : defaults) {
        file << t << ", ";
    }
    file << "};\n\nconst uint16_t barracks_indices[] = { ";
    for(int t : barracks_indices) {
        file << t << ", ";
    }
    file << "};\n\nstatic uint8_t barracks_data[] = { ";
    for(int t : barracks_indices) {
        file << "0, ";
    }
    file << "};\n\nconst uint16_t blueprints_data[] = { ";
    for(int t : blueprint_indices) {
        file << t << ", ";
    }
    file << "};\n\n";
}

void processWorldSprite(std::ofstream &file, nlohmann::json &layer)
{
    const int spacing = 4;
    const int target_size = 54;

    int width = json["width"].get<int>();
    int height = json["height"].get<int>();

    file << "#ifndef #_MAPSPRITE\n#define _MAPSPRITE\n\n#include <cstdint>\n\n";
    file << "const uint8_t sprite_map[] = {\n\t" << target_size << ", " << target_size << ", ";
    for(int j = 0; j < json["data"].size(); j++) {
        int tile = json["data"][j].get<int>()-1;
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
        }
    }
    file << "};\n\n";
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

    std::ofstream elements("spawnpoints.h");
    elements << "#pragma once\n\n#include \"core/utilities/vec.h\"\n#include \"game/entities/enemy.h\"\n#include \"game/entities/barracks.h\"\n#include \"game/entities/pickups.h\"\n#include \"game/entities/pois.h\"\n#include \"game/maps/spawnpoint.h\"\n\n";
    elements << "const SpawnPoint points[] = {\n";

    nlohmann::json json;
    std::ifstream input(argv[1]);
    input >> json;

    bool has_mutable = false;
    nlohmann::json world_layer, mutable_layer;
    for (int i = 0; i < json["layers"].size(); i++) {
        if (json["layers"][i]["name"] == "world") {
            processWorldDataRLE(file, json["layers"][i]);
            world_layer = json["layers"][i]["data"];
        } else if (json["layers"][i]["name"] == "mutable") {
            mutable_layer = json["layers"][i]["data"];
            has_mutable = true;
        } else if (json["layers"][i]["name"] == "level_elements") {
            processLevelElements(elements, json["layers"][i]);
        } else if (!json["layers"][i]["data"].is_null()) {
            processWorldDataRLE(file, json["layers"][i]);
        }
    }
    processWorldSprite(world_layer);

    elements << "};\nconst int point_count = sizeof(points)/sizeof(SpawnPoint);\n";

    if (has_mutable) {
        std::ofstream muts("worldmutables.h");
        muts <<  "#pragma once\n\n#include <cstdint>\n\n";
        processMutableData(muts, mutable_layer, world_layer);
    }

    return 0;
}
