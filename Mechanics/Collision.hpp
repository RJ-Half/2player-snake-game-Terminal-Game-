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

    bool swapped(Vec2 head1, Vec2 head2,  Vec2 prev1, Vec2 prev2) {
        return (head1 == prev2 && head2 == prev1);
    }

    bool losesClash(int headValueA, int headValueB) {
        return headValueA <= headValueB;
    }

    bool isSolid(Vec2 cell, const Vec2* body, int length, bool justAte) {
        int limit = justAte ? length - 1 : length;
        for (int i = 0; i < limit; i++) {
            if (body[i] == cell) {
                return true;
            }
        }
        return false;
    }

    bool hitsSelf(const Vec2* body, int length, bool justAte) {
        return isSolid(body[0], body + 1, length - 1, justAte);
    }

}