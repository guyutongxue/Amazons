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
        Black,         // 黑
        Blue,          // 蓝
        Green,         // 绿
        Cyan,          // 青
        Red,           // 红
        Magenta,       // 洋红
        Brown,         // 棕
        LightGrey,     // 淡灰
        DarkGrey,      // 深灰
        LightBlue,     // 淡兰
        LightGreen,    // 淡绿
        LightCyan,     // 淡青
        LightRed,      // 淡红
        LightMagenta,  // 淡洋红
        Yellow,        // 黄
        White          // 白
    };
private:
    Amazons game;
    HANDLE hOut;
    CONSOLE_SCREEN_BUFFER_INFO bInfo;
    void setPos(short x, short y);
    void setColor(Color foreground,Color background);
    constexpr static const char* boardLine[17] = {
        "┌─┬─┬─┬─┬─┬─┬─┬─┐", "| | | | | | | | |", "├─┼─┼─┼─┼─┼─┼─┼─┤", "| | | | | | | | |",
        "├─┼─┼─┼─┼─┼─┼─┼─┤", "| | | | | | | | |", "├─┼─┼─┼─┼─┼─┼─┼─┤", "| | | | | | | | |",
        "├─┼─┼─┼─┼─┼─┼─┼─┤", "| | | | | | | | |", "├─┼─┼─┼─┼─┼─┼─┼─┤", "| | | | | | | | |",
        "├─┼─┼─┼─┼─┼─┼─┼─┤", "| | | | | | | | |", "├─┼─┼─┼─┼─┼─┼─┼─┤", "| | | | | | | | |",
        "└─┴─┴─┴─┴─┴─┴─┴─┘"};
};

#endif  // UI_H