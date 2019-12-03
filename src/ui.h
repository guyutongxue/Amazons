// Copyright (c) 2019 Guyutongxue
// 
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef UI_H
#define UI_H

#include <conio.h>
#include <windows.h>
#include <functional>
#include <string>
#include "chessboard.h"

/**
 * @brief 用户交互界面类。
 * 提供近似图形化的用户交互界面。
 * 负责打印菜单、棋盘和帮助，读取用户落子等。
 */
class UI {
public:
    /**
     * @brief 默认构造函数。
     * 获取控制台句柄，并设置代码页，光标和缓冲区属性。
     */
    UI();

    /**
     * @brief 默认析构函数。
     * 关闭控制台句柄。
     */
    ~UI();

    /**
     * @brief 打印棋盘背景。
     * 打印棋盘背景和玩家名。
     * @param black 黑方玩家名。
     * @param white 白方玩家名。
     */
    void printBoardBackground(std::string black, std::string white);

    /**
     * @brief 打印棋子。
     * 将棋盘中的棋子打印到正确的位置上。
     * @param board 要打印的棋盘。
     */
    void printGame(const Chessboard& board);

    /**
     * @brief 打印棋子和落子。
     * 将棋盘中的棋子打印到正确的位置上，同时标记上一步落子。
     * @param board 要打印的棋盘。
     * @param lastmove 要打印的上一步落子。
     */
    void printGame(const Chessboard& board, Move lastmove);

    /**
     * @brief 读取人类玩家落子。
     * 通过键盘输入和高亮显示读取人类玩家落子。
     * @param board 将要落子的棋盘。
     * @param piece 人类玩家所操控的执棋方。
     * @param[out] move 读取到的落子。
     * @return 是否成功落子。若人类玩家选择暂停，返回假。
     */
    bool generateMove(Chessboard board, Piece piece, Move& move);

    /**
     * @brief 打印主菜单界面。
     * @return 选中并确定的菜单选项序号，从 0 开始。
     */
    int printMainMenu();

    /**
     * @brief 打印选择模式菜单界面。
     * @return 选中并确定的菜单选项序号，从 0 开始。
     */
    int printModeMenu();

    /**
     * @brief 打印暂停菜单界面。
     * @return 选中并确定的菜单选项序号，从 0 开始。
     */
    int printPauseMenu();

    

    /**
     * @brief 打印读取/保存输入框。
     * 在屏幕特定位置打印输入提示和输入栏。
     * @return 输入的字符串。
    */
    std::string printSL();

    /**
     * @brief 打印读取/保存结果提示框。
     * 若读写成功，弹出绿色提示文字；
     * 若读写失败，弹出红色提示文字。
     * 提示框持续一秒。
     * @param isSuccess 是否读取成功。
     */
    void printSLMsg(bool isSuccess);

    /**
     * @brief 打印游戏结束界面。
     * 保留棋盘终局的同时，打印游戏结果和帮助提示。
     * @param winner 游戏胜利执棋方。
     */
    void printEnd(Piece winner);

    /**
     * @brief 打印关于界面。
    */
    void printAbout();

    /**
     * @brief 切换玩家名高亮显示。
    */
    void switchPlayers(Piece piece);

    /**
     * @brief 打印执棋中的帮助提示。
     * 只在计算机落子时显示。
    */
    void printCalcHelp();
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
        R"*( /_/    \_\_|  |_/_/    \_\/_____\____/|_| \_|_____/ )*",
    };
    constexpr static const char* graphicPaused[5] = {
        R"*(  ____                          _ )*",
        R"*( |  _ \ __ _ _   _ ___  ___  __| |)*",
        R"*( | |_) / _` | | | / __|/ _ \/ _` |)*",
        R"*( |  __/ (_| | |_| \__ \  __/ (_| |)*",
        R"*( |_|   \__,_|\__,_|___/\___|\__,_|)*"};
    constexpr static const char* helpTitle = "----------------||帮助||----------------\n";
    constexpr static const char* aboutText[4] = {
        "亚马逊棋游戏，由谷雨同学制作，版本 0.1.1。 ", "基于 Windows 平台及 MinGW 环境开发。",
        "本项目遵守 Mozilla 公共许可协议 2.0 的条款；",
        "您可以在这里获取源代码：https://github.com/Guyutongxue/Amazons 。"};
    COORD getCOORD(short x, short y);
};

#endif  // UI_H