/**
 * Copyright (c) 2019 Guyutongxue
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef UI_H
#define UI_H

#include <windows.h>
#include <string>
#include "amazons.h"

class UI {
public:
    UI();
    ~UI();
    void printGame();
    enum class Color {
        Black,         // ºÚ
        Blue,          // À¶
        Green,         // ÂÌ
        Cyan,          // Çà
        Red,           // ºì
        Magenta,       // Ñóºì
        Brown,         // ×Ø
        LightGrey,     // µ­»Ò
        DarkGrey,      // Éî»Ò
        LightBlue,     // µ­À¼
        LightGreen,    // µ­ÂÌ
        LightCyan,     // µ­Çà
        LightRed,      // µ­ºì
        LightMagenta,  // µ­Ñóºì
        Yellow,        // »Æ
        White          // °×
    };
private:
    Amazons game;
    HANDLE hOut;
    CONSOLE_SCREEN_BUFFER_INFO bInfo;
    void setPos(short x, short y);
    void setColor(Color foreground,Color background);
    constexpr static const char* boardLine[17] = {
        "©°©¤©Ð©¤©Ð©¤©Ð©¤©Ð©¤©Ð©¤©Ð©¤©Ð©¤©´", "| | | | | | | | |", "©À©¤©à©¤©à©¤©à©¤©à©¤©à©¤©à©¤©à©¤©È", "| | | | | | | | |",
        "©À©¤©à©¤©à©¤©à©¤©à©¤©à©¤©à©¤©à©¤©È", "| | | | | | | | |", "©À©¤©à©¤©à©¤©à©¤©à©¤©à©¤©à©¤©à©¤©È", "| | | | | | | | |",
        "©À©¤©à©¤©à©¤©à©¤©à©¤©à©¤©à©¤©à©¤©È", "| | | | | | | | |", "©À©¤©à©¤©à©¤©à©¤©à©¤©à©¤©à©¤©à©¤©È", "| | | | | | | | |",
        "©À©¤©à©¤©à©¤©à©¤©à©¤©à©¤©à©¤©à©¤©È", "| | | | | | | | |", "©À©¤©à©¤©à©¤©à©¤©à©¤©à©¤©à©¤©à©¤©È", "| | | | | | | | |",
        "©¸©¤©Ø©¤©Ø©¤©Ø©¤©Ø©¤©Ø©¤©Ø©¤©Ø©¤©¼"};
};

#endif  // UI_H