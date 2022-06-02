#ifndef PATHFINDING_H
#define PATHFINDING_H

#include "core/utilities/vec.h"

#include "core/utilities/objectpool.h"

#include "game/utilities/mapmanager.h"
#include "core/rendering/tilemap.h"

typedef Tilemap<6,6> BackgroundMap;

class Pathfinding
{

    struct Node {
        static int16_t goal_x, goal_y;
        uint8_t parent_index = 0;
        uint8_t index = 0;
        int16_t x, y, n_idx, g = 0, h = 0;
        void config(int8_t parent, uint16_t g, float lx, float ly) {
            parent_index = parent;
            x = std::floor(lx/6.0f);
            y = std::floor(ly/6.0f);
            n_idx = y * 1000 + x;
            int dx = std::abs(int(x) - int(goal_x));
            int dy = std::abs(int(y) - int(goal_y));
            h = std::min(dx, dy) * 4 + std::max(dx, dy) * 10; // reduced form of [ min * 14 + (max - min) * 10 ]
        }
        void copy(Node * other) {
            parent_index = other->parent_index;
            x = other->x;
            y = other->y;
            g = other->g;
            h = other->h;
        }
        bool operator==(const Node &n) const { return n_idx == n.n_idx; }
        void update(int8_t potential_parent, uint16_t g_cost) {
            if (g_cost < g) {
                parent_index = potential_parent;
                g = g_cost;
            }
        }

        bool isGoal() const {
            return x == goal_x && y == goal_y;
        }

        int16_t f() const { return g + h; }
    };

    static ObjectPool<Node, 24> s_visited;
    static ObjectPool<Node, 24> s_unvisited;

public:

    static Vec2f getPath(const Vec2f &start, const Vec2f &goal, uint16_t collision_mask, std::vector<Vec2f> * pts = nullptr);

    static bool canReach(const Vec2f &start, const Vec2f &goal, uint16_t collision_mask);
};

#endif // PATHFINDING_H
