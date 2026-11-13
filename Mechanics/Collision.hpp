#pragma once
#include  "Object/Snake.hpp"

namespace collision {

    inline bool hitsWall(Vec2 head, const Arena& arena) {
        if (head.x < 0 || head.x >= arena.width ||
        head.y < 0 || head.y >= arena.height) {
            return true;
        }
        return false;
    }

    inline bool sameCell(Vec2 head1, Vec2 head2) {
        return head1 == head2;
    }

    inline bool swapped(Vec2 head1, Vec2 head2,  Vec2 prev1, Vec2 prev2) {
        return (head1 == prev2 && head2 == prev1);
    }

    inline bool losesClash(int headValueA, int headValueB) {
        return headValueA <= headValueB;
    }

    inline bool isSolid(Vec2 cell, const Snake& snake, bool justAte) {
        if (snake.isDead()) return false;
        int limit = justAte ? snake.length() - 1 : snake.length();
        for (int i = 0; i < limit; ++i) {
            if (snake.segment(i) == cell) return true;
            }
            return false;
        }

   inline bool hitsSelf(const Snake& snake, bool justAte) {
    if (snake.isDead()) return false;
    int limit = justAte ? snake.length() - 1 : snake.length();
        for (int i = 1; i < limit; ++i) {
            if (snake.segment(i) == snake.head()) return true;
        }
        return false;
    }

}