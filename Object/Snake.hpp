#pragma once

#include "Platform/Arena.hpp"

namespace cfg {

constexpr int InitialLength = 2;
constexpr int MaxHeadValue = 99;  

}  
constexpr int PlayerCount = 2;

inline int otherPlayer(int player) { return 1 - player; }

enum class DeathCause { None, Wall, SelfBite, FailedAttack, HeadToHead, Wiped };

enum class Outcome { None, Player1Wins, Player2Wins, Draw };


class Snake {
public:
    void reset(Vec2 head, Direction heading) {
        first_ = 0;
        length_ = cfg::InitialLength;
        pending_ = 0;
        cause_ = DeathCause::None;
        heading_ = queued_ = heading;

        Vec2 p = head;
        for (int i = 0; i < length_; ++i) {
            cells_[i] = p;
            p = stepFrom(p, opposite(heading));
        }
    }

    int length() const { return length_; }
    int pending() const { return pending_; }
    int headValue() const { return length_ + pending_; }
    int valueAt(int index) const { return length_ - index; }

    Vec2 segment(int index) const { return cells_[(first_ + index) % Capacity]; }
    Vec2 head() const { return segment(0); }

    int indexOf(Vec2 p) const {
        for (int i = 0; i < length_; ++i) {
            if (segment(i) == p) return i;
        }
        return -1;
    }

    Direction heading() const { return heading_; }

    
    void requestTurn(Direction d) {
        if (d != opposite(heading_)) queued_ = d;
    }
    void commitTurn() { heading_ = queued_; }

    
    void moveTo(Vec2 newHead) { pushHead(newHead); }

    
    void growTo(Vec2 newHead) {
        pushHead(newHead);
        ++length_;
    }

    void truncate(int newLength) { length_ = newLength; }
    void setPending(int amount) { pending_ = amount; }

    DeathCause cause() const { return cause_; }
    bool isDead() const { return cause_ != DeathCause::None; }
    void setCause(DeathCause cause) { cause_ = cause; }

private:
    
    static constexpr int Capacity = cfg::MaxHeadValue + 1;

    void pushHead(Vec2 p) {
        first_ = (first_ + Capacity - 1) % Capacity;
        cells_[first_] = p;
    }

    Vec2 cells_[Capacity];  
    int first_ = 0;         
    int length_ = 0;
    int pending_ = 0;
    DeathCause cause_ = DeathCause::None;
    Direction heading_ = Direction::Right;
    Direction queued_ = Direction::Right;
};


inline Outcome outcomeOf(const Snake* snakes) {
    bool dead0 = snakes[0].isDead();
    bool dead1 = snakes[1].isDead();

    if (dead0 && dead1) return Outcome::Draw;
    if (dead0) return Outcome::Player2Wins;
    if (dead1) return Outcome::Player1Wins;
    return Outcome::None;
}
