#ifndef MENU_HPP
#define MENU_HPP

#ifndef NCURSES_NOMACROS
#define NCURSES_NOMACROS
#endif
#include <ncurses.h>

#include "Platform/Arena.hpp"
#include "Object/Snake.hpp"

namespace cfg {
constexpr int PopupPadX = 1;
constexpr int ResultFixedRows = 3;
}

namespace menu {

constexpr const char* TextCommands = "P1: WASD   P2: Panah   P: jeda   R: ulang   Q: keluar";
constexpr const char* TextPaused   = "JEDA - tekan P untuk lanjut";
constexpr const char* TextRestart  = "R: main lagi   Q: keluar";

namespace detail {

inline int textLen(const char* s) {
    int n = 0;
    while (s[n] != '\0') ++n;
    return n;
}

inline int maxInt(int a, int b) { return a > b ? a : b; }

inline const char* titleOf(Outcome outcome) {
    switch (outcome) {
        case Outcome::Player1Wins: return "PLAYER 1 MENANG";
        case Outcome::Player2Wins: return "PLAYER 2 MENANG";
        case Outcome::Draw:        return "SERI";
        default:                   return "";
    }
}

inline const char* causeText(DeathCause cause) {
    switch (cause) {
        case DeathCause::Wall:        return "menabrak dinding";
        case DeathCause::SelfBite:     return "menabrak tubuh sendiri";
        case DeathCause::FailedAttack: return "serangan gagal (nilai kurang)";
        case DeathCause::HeadToHead:   return "kalah adu kepala";
        case DeathCause::Wiped:  return "seluruh tubuh dicuri";
        default:                      return "";
    }
}

inline int reasonWidth(DeathCause cause) { return 4 + textLen(causeText(cause)); }

inline void putCentered(WINDOW* w, int row, const char* text) {
    int col = (getmaxx(w) - textLen(text)) / 2;
    if (col < 0) col = 0;
    mvwaddstr(w, row, col, text);
}

inline WINDOW* openPopup(WINDOW* frame, int rows, int cols) {
    const int fh = getmaxy(frame);
    const int fw = getmaxx(frame);
    if (rows > fh - 2 || cols > fw - 2) return nullptr;
    WINDOW* p = derwin(frame, rows, cols, (fh - rows) / 2, (fw - cols) / 2);
    if (p == nullptr) return nullptr;
    wbkgd(p, static_cast<chtype>(' ') | A_REVERSE);
    werase(p);
    box(p, 0, 0);
    wattron(p, A_BOLD);
    return p;
}

inline void closePopup(WINDOW* p) {
    wattroff(p, A_BOLD);
    delwin(p);
}

inline void drawPlayerStatus(WINDOW* hud, int col, int index, const Snake& s, int attr) {
    const int len = s.length();
    const int pend = s.pending();
    wattron(hud, attr);
    mvwprintw(hud, 0, col, "P%d  Len:%d  HV:%d  Pend:%d", index + 1, len, len + pend, pend);
    wattroff(hud, attr);
}

}

inline void drawStatus(WINDOW* hud, const Snake* snakes, const int* attrs) {
    const int half = getmaxx(hud) / 2;
    for (int i = 0; i < PlayerCount; ++i) {
        detail::drawPlayerStatus(hud, i == 0 ? 0 : half, i, snakes[i], attrs[i]);
    }
}

inline void drawCommands(WINDOW* hud) {
    mvwaddstr(hud, 1, 0, TextCommands);
}

inline void drawResult(WINDOW* frame, const Snake* snakes, Outcome outcome) {
    if (outcome == Outcome::None) return;

    int losers = 0;
    int width = detail::maxInt(detail::textLen(detail::titleOf(outcome)),
                               detail::textLen(TextRestart));
    for (int i = 0; i < PlayerCount; ++i) {
        if (!snakes[i].isDead()) continue;
        ++losers;
        width = detail::maxInt(width, detail::reasonWidth(snakes[i].cause()));
    }

    const int rows = 2 + cfg::ResultFixedRows + losers;
    const int cols = width + 2 * cfg::PopupPadX + 2;
    WINDOW* p = detail::openPopup(frame, rows, cols);
    if (p == nullptr) return;

    int row = 1;
    detail::putCentered(p, row++, detail::titleOf(outcome));
    for (int i = 0; i < PlayerCount; ++i) {
        if (!snakes[i].isDead()) continue;
        char label[5] = {'P', static_cast<char>('1' + i), ':', ' ', '\0'};
        const char* reason = detail::causeText(snakes[i].cause());
        const int len = 4 + detail::textLen(reason);
        int col = (getmaxx(p) - len) / 2;
        if (col < 0) col = 0;
        mvwaddstr(p, row, col, label);
        waddstr(p, reason);
        ++row;
    }
    ++row;
    detail::putCentered(p, row, TextRestart);

    detail::closePopup(p);
}

inline void drawPaused(WINDOW* frame) {
    const int cols = detail::textLen(TextPaused) + 2 * cfg::PopupPadX + 2;
    WINDOW* p = detail::openPopup(frame, 3, cols);
    if (p == nullptr) return;
    detail::putCentered(p, 1, TextPaused);
    detail::closePopup(p);
}

}

#endif
