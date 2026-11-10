#pragma once
#include  "../Platform/SemArena.hpp"

namespace collision {

    bool hitsWall(Vec2 head, const Arena& arena) {
        if (head.x < 0 || head.x >= arena.width ||
        head.y < 0 || head.y >= arena.height) {
            return true;
        }
        return false;
    }

    bool sameCell(Vec2 head1, Vec2 head2) {
        return head1 == head2;
    }

}