#pragma once
#include "../Platform/SemArena.hpp" //akan diubah setelah Arena.hpp jadi

namespace cfg {constexpr int FoodLifetime = 50, FoodSpawnInterval = 70, FoodWaveMin= 1, FoodWaveMax = 5;}
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

int randomValue(Random& rng) {
    constexpr int weights[9] = {25, 20, 15, 12, 10, 7, 5, 4, 2};
    int roll = rng.range(0, 99);

    for (int i = 0; i < 9; i++) {
        if (roll < weights[i]) {
            return i + 1;
        }
        roll -= weights[i];
    }
    return 1;
}

class FoodField {
    private:
    Food items_[cfg:: FoodWaveMax];
    int count_ = 0;
    int WaveTimer_ = 0;

    public:
    void reset() {
        count_ = 0;
        WaveTimer_ = 0;
    }
     void remove(int index) {
        if (index < 0 || index >= count_) return;
        items_[index] = items_[count_ - 1];
        count_--;
     }
     void update() {
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

     void spawnWave(Random& rng, const Arena& arena) {
        int availableSlots = cfg::FoodWaveMax - count_;
        if(availableSlots <= 0) return;

        int waveSize = rng.range(cfg::FoodWaveMin, cfg::FoodWaveMax);
        int toSpawn = waveSize;
        if(toSpawn > availableSlots) {
            toSpawn = availableSlots;
        }

        for(int i = 0; i < toSpawn; i++) {
            Food newFood;
            newFood.value = randomValue(rng);
            newFood.age = 0;

            Vec2 randomPos;
        do {
            randomPos.x = rng.range(0, arena.width - 1);
            randomPos.y = rng.range(0, arena.height - 1);
        } while (isFoodAt(randomPos));

        newFood.pos = randomPos;

            items_[count_] = newFood;
            count_++;
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