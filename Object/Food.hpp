#pragma once

#include  "Snake.hpp"

namespace cfg {
    constexpr int FoodLifetime = 50;
    constexpr int FoodSpawnInterval = 70;
    constexpr int FoodWaveMin = 1;
    constexpr int FoodWaveMax = 5;
    constexpr int FoodWeights[9] = {25, 20, 15, 12, 10, 7, 5, 4, 2};
    constexpr int FoodValues[9] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
}
struct Food {
    Vec2 pos;
    int age;
    int value;
};

struct Random {
    unsigned int state = 1;

    void seed(unsigned int s) {
        if (s == 0) s = 0x9E779B9;
        state = s;
        for (int i = 0; i < 8; i++) next();
    }

    unsigned int next() {
        unsigned int x = state;
        x ^= x << 13;
        x ^= x >> 17;
        x ^= x << 5;
        return state = x;
    }

    int range(int min, int max) {
        return min + (next() % (max - min + 1));
    }

};

inline int randomValue(Random& rng) {
    int roll = rng.range(0, 99);
    for (int i = 0; i < 9; i++) {
        if (roll < cfg::FoodWeights[i]) {
            return cfg::FoodValues[i];
        }
        roll -= cfg::FoodWeights[i];
    }
    return cfg::FoodValues[0];
}

class FoodField {
    private:
    Food items_[cfg:: FoodWaveMax];
    int count_ = 0;
    int WaveTimer_ = 0;

    public:
    void clear() {
        count_ = 0;
        WaveTimer_ = 0;
    }
     void start(const Arena& arena, const Snake snakes[PlayerCount], Random& rng) {
       clear();
       spawnWave(arena, snakes, rng);
    }
     void remove(int index) {
        if (index < 0 || index >= count_) return;
        items_[index] = items_[count_ - 1];
        count_--;
     }

     void update(const Arena& arena, const Snake snakes[PlayerCount], Random& rng) {
        for (int i = 0; i < count_; i++) {
            items_[i].age++;
            if (items_[i].age >= cfg::FoodLifetime) {
                remove(i);
                i--;
            }
        }

        WaveTimer_++;
        if (WaveTimer_ >= cfg::FoodSpawnInterval) {
            WaveTimer_ = 0;
            spawnWave(arena, snakes, rng);
        }

     }

     int count() const { return count_; }
     const Food& get(int index) const { return items_[index]; }
     bool isFoodAt(Vec2 pos) const {
        for (int i = 0; i < count_; i++) {
            if (items_[i].pos == pos) {
                return true;
            }
        }
        return false;
     }

     void spawnWave(const Arena& arena, const Snake snakes[PlayerCount], Random& rng) {
        bool occupied[cfg::MaxArenaH][cfg::MaxArenaW] = {false};
        for (int i = 0; i < PlayerCount; i++) {
            const Snake& s = snakes[i];
            if (s.isDead()) continue;
            for (int j = 0; j < s.length(); j++) {
                Vec2 pos = s.segment(j);
                if (pos.x >= 0 && pos.x < cfg::MaxArenaW && pos.y >= 0 && pos.y < cfg::MaxArenaH) {
                    occupied[pos.y][pos.x] = true;
                }
            }
        }

            for (int i = 0; i < count_; i++) 
                occupied[items_[i].pos.y][items_[i].pos.x] = true; 
            Vec2 emptyCells[cfg::MaxCells]{};
            int emptyCount = 0;
            for (int y = 0; y < arena.height; y++) {
                for (int x = 0; x < arena.width; x++) {
                    if (!occupied[y][x]) {
                        emptyCells[emptyCount++] = {x, y};
                    }
                }
            }

            int waveSize = rng.range(cfg::FoodWaveMin, cfg::FoodWaveMax);
            int remainingSlots = cfg::FoodWaveMax - count_;
            if (remainingSlots <= 0) return;
            int toSpawn = waveSize < emptyCount ? waveSize : emptyCount;
            if (toSpawn > remainingSlots) toSpawn = remainingSlots;
            for (int i = 0; i < toSpawn; i++) {
                if (count_ >= cfg::FoodWaveMax) break;
                int index = rng.range(0, emptyCount - 1);
                Vec2 pos = emptyCells[index];
                int value = randomValue(rng);
                items_[count_++] = {pos, 0, value};
                emptyCells[index] = emptyCells[emptyCount - 1];
                emptyCount--;
            }

        }

     int take(Vec2 pos) {
        for(int i = 0; i < count_; i++) {
            if (items_[i].pos == pos) {
                int value = items_[i].value;
                remove(i);
                return value;
            }
        }
        return 0;
     }

};