/**
 * Copyright (c) 2019 Guyutongxue
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef UI_H
#define UI_H

#include <conio.h>
#include <windows.h>
#include <string>
#include <functional>
#include "chessboard.h"

class UI {
public:
    UI();
    ~UI();
    void printBoardBackground();
    void printGame(const Chessboard& board);
    void printGame(const Chessboard& board, Move lastmove);
    bool generateMove(Chessboard board, Player pl,Move& move);
    int printMainMenu();
    int printModeMenu();

private:
    HANDLE hOut;
    CONSOLE_SCREEN_BUFFER_INFO bInfo;
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
        LightBlue,     // 淡蓝
        LightGreen,    // 淡绿
        LightCyan,     // 淡青
        LightRed,      // 淡红
        LightMagenta,  // 淡洋红
        Yellow,        // 黄
        White          // 白
    };
    short center_x;
    void clearScreen();
    void setCursorPos(short x, short y);
    void setPosColor(Color foreground, Color background,short x,short y);
    void setTextColor(Color foreground, Color background);
    /**
     * @brief 在某一棋盘中，用户通过键盘选择某一种格子。
     * @param board 要选择的棋盘
     * @param target 要选择的格子种类
     * @param x 格子坐标 x 
     * @param y 格子坐标 y
     * @return 空
     */
    void chooseTarget(const Chessboard& board, Square target,int& x,int& y);
    int printMenu(const std::string& title, std::string* choices, short* pos, int num);
    constexpr static const char* boardLine[17] = {
        "┌─┬─┬─┬─┬─┬─┬─┬─┐", "│ │ │ │ │ │ │ │ │", "├─┼─┼─┼─┼─┼─┼─┼─┤", "│ │ │ │ │ │ │ │ │",
        "├─┼─┼─┼─┼─┼─┼─┼─┤", "│ │ │ │ │ │ │ │ │", "├─┼─┼─┼─┼─┼─┼─┼─┤", "│ │ │ │ │ │ │ │ │",
        "├─┼─┼─┼─┼─┼─┼─┼─┤", "│ │ │ │ │ │ │ │ │", "├─┼─┼─┼─┼─┼─┼─┼─┤", "│ │ │ │ │ │ │ │ │",
        "├─┼─┼─┼─┼─┼─┼─┼─┤", "│ │ │ │ │ │ │ │ │", "├─┼─┼─┼─┼─┼─┼─┼─┤", "│ │ │ │ │ │ │ │ │",
        "└─┴─┴─┴─┴─┴─┴─┴─┘"};
    COORD getCOORD(short x, short y);
};

#endif  // UI_H