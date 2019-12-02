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
#include <functional>
#include <string>
#include "chessboard.h"

class UI {
public:
    UI();
    ~UI();
    void printBoardBackground(std::string black,std::string white);
    void printGame(const Chessboard& board);
    void printGame(const Chessboard& board, Move lastmove);
    bool generateMove(Chessboard board, Piece piece, Move& move);

    int printMainMenu();
    int printModeMenu();
    int printPauseMenu();

    void printCalcHelp();
    std::string printSL();
    void printSLMsg(bool isSuccess);
    void printEnd(Piece winner);
    void printAbout();

    void switchPlayers(Piece piece);

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
    void setPosColor(Color foreground, Color background, short x, short y);
    void setTextColor(Color foreground, Color background);

    /**
     * @brief 在某一棋盘中，用户通过键盘选择某一种格子。
     * @param board 要选择的棋盘
     * @param target 要选择的格子种类
     * @param x 格子坐标 x
     * @param y 格子坐标 y
     * @return 空
     */
    void chooseTarget(const Chessboard& board, Square target, int& x, int& y);
    int printMenu(const std::string& title, std::string* choices, short* pos, int num);
    std::string printInputField(short x, short y, short length, std::string prompt);
    void printMessage(short x, short y, std::string text, bool isWarning);

    void printHelpHeader();
    void printMenuHelp();
    void printInputHelp();
    void printMoveHelp(int status);
    void printEndHelp();

    constexpr static const char* boardLine[17] = {
        "┌───┬───┬───┬───┬───┬───┬───┬───┐", "│   │   │   │   │   │   │   │   │",
        "├───┼───┼───┼───┼───┼───┼───┼───┤", "│   │   │   │   │   │   │   │   │",
        "├───┼───┼───┼───┼───┼───┼───┼───┤", "│   │   │   │   │   │   │   │   │",
        "├───┼───┼───┼───┼───┼───┼───┼───┤", "│   │   │   │   │   │   │   │   │",
        "├───┼───┼───┼───┼───┼───┼───┼───┤", "│   │   │   │   │   │   │   │   │",
        "├───┼───┼───┼───┼───┼───┼───┼───┤", "│   │   │   │   │   │   │   │   │",
        "├───┼───┼───┼───┼───┼───┼───┼───┤", "│   │   │   │   │   │   │   │   │",
        "├───┼───┼───┼───┼───┼───┼───┼───┤", "│   │   │   │   │   │   │   │   │",
        "└───┴───┴───┴───┴───┴───┴───┴───┘"};
    constexpr static const char* graphicTitle[6] = {
        R"*(           __  __           __________  _   _  _____ )*",
        R"*(     /\   |  \/  |   /\    |___  / __ \| \ | |/ ____|)*",
        R"*(    /  \  | \  / |  /  \      / / |  | |  \| | (___  )*",
        R"*(   / /\ \ | |\/| | / /\ \    / /| |  | | . ` |\___ \ )*",
        R"*(  / ____ \| |  | |/ ____ \  / /_| |__| | |\  |____) |)*",
        R"*( /_/    \_\_|  |_/_/    \_\/_____\____/|_| \_|_____/ )*",};
    constexpr static const char* graphicPaused[5]={
        R"*(  ____                          _ )*",
        R"*( |  _ \ __ _ _   _ ___  ___  __| |)*",
        R"*( | |_) / _` | | | / __|/ _ \/ _` |)*",
        R"*( |  __/ (_| | |_| \__ \  __/ (_| |)*",
        R"*( |_|   \__,_|\__,_|___/\___|\__,_|)*"};
    constexpr static const char* helpTitle="----------------||帮助||----------------\n";
    constexpr static const char* aboutText[4]={
        "亚马逊棋游戏，由谷雨同学制作，版本 0.1.0。 ",
        "基于 Windows 平台及 MinGW 环境开发。",
        "本项目遵守 Mozilla 公共许可协议 2.0 的条款；",
        "您可以在这里获取源代码：https://github.com/Guyutongxue/Amazons 。"
    };
    COORD getCOORD(short x, short y);
};

#endif  // UI_H