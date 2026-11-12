#pragma once

#ifndef NCURSES_NOMACROS
#define NCURSES_NOMACROS
#endif
#include <ncurses.h>

namespace cfg {

constexpr int MinArenaW = 20;
constexpr int MaxArenaW = 120;
constexpr int MinArenaH = 10;
constexpr int MaxArenaH = 60;
constexpr int CellWidth = 2;
constexpr int HudRows = 2;
constexpr int MinTermCols = 60;
constexpr int FrameBorder = 2;
constexpr int MaxCells = MaxArenaW * MaxArenaH;

}

struct Vec2 {
    int x = 0;
    int y = 0;

    constexpr bool operator==(const Vec2& o) const { return x == o.x && y == o.y; }
    constexpr bool operator!=(const Vec2& o) const { return !(*this == o); }
};

enum class Direction { Up, Down, Left, Right };

inline Direction opposite(Direction d) {
    switch (d) {
        case Direction::Up:    return Direction::Down;
        case Direction::Down:  return Direction::Up;
        case Direction::Left:  return Direction::Right;
        case Direction::Right: return Direction::Left;
    }
    return d;
}

inline Vec2 stepFrom(Vec2 from, Direction d) {
    switch (d) {
        case Direction::Up:    return Vec2{from.x, from.y - 1};
        case Direction::Down:  return Vec2{from.x, from.y + 1};
        case Direction::Left:  return Vec2{from.x - 1, from.y};
        case Direction::Right: return Vec2{from.x + 1, from.y};
    }
    return from;
}

struct Arena {
    int width = cfg::MinArenaW;
    int height = cfg::MinArenaH;

    constexpr bool contains(Vec2 p) const {
        return p.x >= 0 && p.y >= 0 && p.x < width && p.y < height;
    }
};

namespace detail {

inline int clampInt(int v, int lo, int hi) {
    return v < lo ? lo : (v > hi ? hi : v);
}

}

class ArenaWindow {
public:
    ArenaWindow() = default;
    ArenaWindow(const ArenaWindow&) = delete;
    ArenaWindow& operator=(const ArenaWindow&) = delete;
    ~ArenaWindow() { release(); }

    bool open(Arena& arena) {
        release();
        sized_ = false;

        WINDOW* probe = newwin(0, 0, cfg::HudRows, 0);
        if (probe == nullptr) return false;
        const int winW = getmaxx(probe);
        const int winH = getmaxy(probe);
        delwin(probe);

        arena.width = detail::clampInt((winW - cfg::FrameBorder) / cfg::CellWidth,
                                       cfg::MinArenaW, cfg::MaxArenaW);
        arena.height = detail::clampInt(winH - cfg::FrameBorder,
                                        cfg::MinArenaH, cfg::MaxArenaH);
        cellsW_ = arena.width;
        cellsH_ = arena.height;
        sized_ = true;

        if (COLS < needCols() || LINES < needRows() || !build()) {
            sized_ = false;
            return false;
        }
        return true;
    }

    bool rebuild() {
        if (!sized_) return false;
        release();
        if (COLS < needCols() || LINES < needRows()) return false;
        return build();
    }

    bool fits() const {
        return hud_ != nullptr && frame_ != nullptr && field_ != nullptr &&
               sized_ && COLS >= needCols() && LINES >= needRows();
    }

    int needCols() const {
        if (!sized_) return cfg::MinTermCols;
        const int fw = frameW();
        return fw > cfg::MinTermCols ? fw : cfg::MinTermCols;
    }
    int needRows() const {
        if (!sized_) return cfg::HudRows + cfg::MinArenaH + cfg::FrameBorder;
        return cfg::HudRows + frameH();
    }

    int cellsW() const { return sized_ ? cellsW_ : 0; }
    int cellsH() const { return sized_ ? cellsH_ : 0; }
    int frameW() const { return sized_ ? cfg::CellWidth * cellsW_ + cfg::FrameBorder : 0; }
    int frameH() const { return sized_ ? cellsH_ + cfg::FrameBorder : 0; }

    WINDOW* hud() const { return hud_; }
    WINDOW* frame() const { return frame_; }
    WINDOW* field() const { return field_; }

private:
    bool build() {
        release();
        const int fw = frameW();
        const int fh = frameH();
        const int hudW = needCols();

        hud_ = newwin(cfg::HudRows, hudW, 0, (COLS - hudW) / 2);
        frame_ = newwin(fh, fw, cfg::HudRows, (COLS - fw) / 2);
        if (frame_ != nullptr) {
            field_ = derwin(frame_, cellsH_, cfg::CellWidth * cellsW_, 1, 1);
        }
        if (hud_ == nullptr || frame_ == nullptr || field_ == nullptr) {
            release();
            return false;
        }
        return true;
    }

    void release() {
        if (field_ != nullptr) { delwin(field_); field_ = nullptr; }
        if (frame_ != nullptr) { delwin(frame_); frame_ = nullptr; }
        if (hud_ != nullptr)   { delwin(hud_);   hud_ = nullptr; }
    }

    WINDOW* hud_ = nullptr;
    WINDOW* frame_ = nullptr;
    WINDOW* field_ = nullptr;
    int cellsW_ = 0;
    int cellsH_ = 0;
    bool sized_ = false;
};