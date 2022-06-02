#include "pathfinding.h"

#include "game/physics/collisionmanager.h"
#include <vector>

int16_t Pathfinding::Node::goal_x = 0;
int16_t Pathfinding::Node::goal_y = 0;

ObjectPool<Pathfinding::Node,24> Pathfinding::s_visited;
ObjectPool<Pathfinding::Node,24> Pathfinding::s_unvisited;

Vec2f Pathfinding::getPath(const Vec2f &start, const Vec2f &goal, uint16_t collision_mask, std::vector<Vec2f> * pts)
{
    s_unvisited.clear();
    s_visited.clear();

    Node::goal_x = std::floor(goal.x() / 6);
    Node::goal_y = std::floor(goal.y() / 6);

    s_unvisited.activateNext()->config(0, 0, start.x(), start.y());

    Node * current = s_unvisited.objects();
    bool done = false;
    while (!done && s_visited.objectCount() < 60 && s_unvisited.objectCount() > 0) {
        int active_idx = 0;
        current = s_unvisited.objects();
        for(int i = 0; i < s_unvisited.objectCount(); ++i) {
            if (current->f() > s_unvisited.objects()[i].f()) {
                current = s_unvisited.objects() + i;
                active_idx = i;
            }
        }

        Node * next = s_visited.activateNext();
        if (next == nullptr) {
            done = true;
            continue;
        }
        next->copy(current);
        s_unvisited.deactivate(active_idx);

        current = s_visited.objects() + s_visited.objectCount() - 1;
        current->index = s_visited.objectCount() - 1;

        if (current->isGoal()) {
            done = true;
            continue;
        }


        for (int i = -1; i <= 1; ++i) {
            for (int j = -1; j <= 1; ++j) {
                if (i == 0 && j == 0) continue;
                Vec2f pos((current->x + i)*6+3, (current->y + j)*6+3);
                if (CollisionManager::collides(pos, collision_mask)) {
                    continue;
                }
                Node n;
                n.config(current->index, current->g + (i == 0 || j == 0 ? 10 : 14), pos.x(), pos.y());
                bool visited = false;
                for(int i = 0; i < s_visited.objectCount(); i++) {
                    if (s_visited.objects()[i] == n) {
                        visited = true;
                    }
                }
                if (visited) {
                    continue;
                } else {
                    bool unvisited = false;
                    active_idx = 0;
                    for(int i = 0; i < s_unvisited.objectCount(); i++) {
                        if (s_unvisited.objects()[i] == n) {
                            unvisited = true;
                            active_idx = i;
                        }
                    }
                    if (unvisited) {
                        s_unvisited.objects()[active_idx].update(current->index, current->g + (i == 0 || j == 0 ? 10 : 14));
                    } else {
                        next = s_unvisited.activateNext();
                        if (next != nullptr) {
                            next->copy(&n);
                        }
                    }
                }
            }
        }
    }

    if (current == nullptr || current->index == 0) {
        return goal;
    }

    int8_t current_idx = current->index;
    Vec2f output(0,0);
    while (current_idx > 0) {
        output = Vec2f(s_visited.objects()[current_idx].x, s_visited.objects()[current_idx].y);
        current_idx = s_visited.objects()[current_idx].parent_index;
        if (pts != nullptr) pts->push_back(output);
    }
    return output;
}

bool Pathfinding::canReach(const Vec2f &start, const Vec2f &goal, uint16_t collision_mask)
{
    s_unvisited.clear();
    s_visited.clear();

    Node::goal_x = std::floor(goal.x() / 6);
    Node::goal_y = std::floor(goal.y() / 6);

    s_unvisited.activateNext()->config(0, 0, start.x(), start.y());

    Node * current = s_unvisited.objects();
    bool done = false;
    while (!done && s_visited.objectCount() < 60 && s_unvisited.objectCount() > 0) {
        int active_idx = 0;
        current = s_unvisited.objects();
        for(int i = 0; i < s_unvisited.objectCount(); ++i) {
            if (current->f() > s_unvisited.objects()[i].f()) {
                current = s_unvisited.objects() + i;
                active_idx = i;
            }
        }

        Node * next = s_visited.activateNext();
        if (next == nullptr) {
            return false;
        }
        next->copy(current);
        s_unvisited.deactivate(active_idx);

        current = s_visited.objects() + s_visited.objectCount() - 1;
        current->index = s_visited.objectCount() - 1;

        if (current->isGoal()) {
            return true;
        }


        for (int i = -1; i <= 1; ++i) {
            for (int j = -1; j <= 1; ++j) {
                if (i == 0 && j == 0) continue;
                Vec2f pos((current->x + i)*6+3, (current->y + j)*6+3);
                if (CollisionManager::collides(pos, collision_mask)) {
                    continue;
                }
                Node n;
                n.config(current->index, current->g + (i == 0 || j == 0 ? 10 : 14), pos.x(), pos.y());
                bool visited = false;
                for(int i = 0; i < s_visited.objectCount(); i++) {
                    if (s_visited.objects()[i] == n) {
                        visited = true;
                    }
                }
                if (visited) {
                    continue;
                } else {
                    bool unvisited = false;
                    active_idx = 0;
                    for(int i = 0; i < s_unvisited.objectCount(); i++) {
                        if (s_unvisited.objects()[i] == n) {
                            unvisited = true;
                            active_idx = i;
                        }
                    }
                    if (unvisited) {
                        s_unvisited.objects()[active_idx].update(current->index, current->g + (i == 0 || j == 0 ? 10 : 14));
                    } else {
                        next = s_unvisited.activateNext();
                        if (next != nullptr) {
                            next->copy(&n);
                        }
                    }
                }
            }
        }
    }
    return false;
}
