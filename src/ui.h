// Copyright (c) 2019 Guyutongxue
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef UI_H
#define UI_H

#include <conio.h>
#ifndef NOMINMAX
#define NOMINMAX
#endif
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

    /**
     * @brief 清空控制台屏幕缓冲区。
     */
    void clearScreen();

private:
    /** 控制台输出句柄。  */
    HANDLE hOut;

    /** 控制台缓冲区属性。 */
    CONSOLE_SCREEN_BUFFER_INFO bInfo;

    /** 绘制棋盘格子所用字符串。 */
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

    /** 绘制标题 ASCII 画所用字符串。 */
    constexpr static const char* graphicTitle[6] = {
        R"*(           __  __           __________  _   _  _____ )*",
        R"*(     /\   |  \/  |   /\    |___  / __ \| \ | |/ ____|)*",
        R"*(    /  \  | \  / |  /  \      / / |  | |  \| | (___  )*",
        R"*(   / /\ \ | |\/| | / /\ \    / /| |  | | . ` |\___ \ )*",
        R"*(  / ____ \| |  | |/ ____ \  / /_| |__| | |\  |____) |)*",
        R"*( /_/    \_\_|  |_/_/    \_\/_____\____/|_| \_|_____/ )*",
    };

    /** 绘制暂停界面 ASCII 画所用字符串。 */
    constexpr static const char* graphicPaused[5] = {
        R"*(  ____                          _ )*",
        R"*( |  _ \ __ _ _   _ ___  ___  __| |)*",
        R"*( | |_) / _` | | | / __|/ _ \/ _` |)*",
        R"*( |  __/ (_| | |_| \__ \  __/ (_| |)*",
        R"*( |_|   \__,_|\__,_|___/\___|\__,_|)*"};

    /** 帮助提示的标题。 */
    constexpr static const char* helpTitle = "----------------||帮助||----------------\n";

    /** 关于界面的文字。 */
    constexpr static const char* aboutText[4] = {
        "亚马逊棋游戏，由谷雨同学制作，版本 0.1.4 。", "基于 Windows 平台及 MinGW 环境开发。",
        "本项目遵守 Mozilla 公共许可协议 2.0 的条款；",
        "您可以在这里获取源代码：https://github.com/Guyutongxue/Amazons 。"};

    /**
     * @brief 控制台 16 色枚举类。
     * 控制控制台字体颜色时使用。
     */
    enum class Color {
        Black,        /**< 黑     */
        Blue,         /**< 蓝     */
        Green,        /**< 绿     */
        Cyan,         /**< 青     */
        Red,          /**< 红     */
        Magenta,      /**< 洋红   */
        Brown,        /**< 棕     */
        LightGrey,    /**< 淡灰   */
        DarkGrey,     /**< 深灰   */
        LightBlue,    /**< 淡蓝   */
        LightGreen,   /**< 淡绿   */
        LightCyan,    /**< 淡青   */
        LightRed,     /**< 淡红   */
        LightMagenta, /**< 淡洋红 */
        Yellow,       /**< 黄     */
        White         /**< 白     */
    };

    /** 标记控制台中心 X 坐标。 */
    short center_x;

    /**
     * @brief 设置控制台光标的坐标。
     * @param x 光标 X 坐标。
     * @param y 光标 Y 坐标。
     */
    void setCursorPos(short x, short y);

    /**
     * @brief 设置打印颜色。
     * 此函数控制将来打印到控制台上的文字字体颜色。
     * @param foreground 前景色。
     * @param background 背景色。
     */
    void setTextColor(Color foreground, Color background);

    /**
     * @brief 打印菜单并返回选择结果。
     * 此函数提供打印各种菜单的基本实现。
     * @param title 菜单标题。
     * @param choices 选项数组。
     * @param pos 选项 Y 坐标数组。
     * @param num 选项个数。
     * @return 选中并确定的菜单序号，从 0 开始。
     */
    int printMenu(const std::string& title, std::string* choices, short* pos, int num);

    /**
     * @brief 打印输入栏。
     * @param x 输入栏起点 X 坐标。
     * @param y 输入栏起点 Y 坐标。
     * @param length 输入栏长度。
     * @param prompt 输入提示。
     * @return 输入的字符串。
     */
    std::string printInputField(short x, short y, short length, std::string prompt);

    /**
     * @brief 打印提示信息。
     * @param x 提示信息起点 X 坐标。
     * @param y 提示信息起点 Y 坐标。
     * @param text 提示信息内容。
     * @param isWarning 是否为警告。
     */
    void printMessage(short x, short y, std::string text, bool isWarning);

    /**
     * @brief 打印帮助头。
     * 设置光标位置、文字颜色，并打印帮助标题。
     */
    void printHelpHeader();

    /**
     * @brief 打印菜单界面帮助。
     */
    void printMenuHelp();

    /**
     * @brief 打印输入栏界面帮助。
     */
    void printInputHelp();

    /**
     * @brief 打印人类玩家执棋界面帮助。
     * 分为三种状态：选择棋子、选择落点、选择 Arrow 位置。
     * @param status 上述三种状态，从 1 开始。
     */
    void printMoveHelp(int status);

    /**
     * @brief 打印结束界面帮助。
     */
    void printEndHelp();

    /**
     * @brief 设置棋盘上某坐标位置的颜色
     * 注意此函数设置的是棋盘坐标而非控制台坐标。
     * @param foreground 前景色。
     * @param background 背景色。
     * @param x 棋盘 X 坐标。
     * @param y 棋盘 Y 坐标。
     */
    void setPosColor(Color foreground, Color background, short x, short y);

    /**
     * @brief 获取棋盘某坐标对应的控制台坐标。
     * @param x 棋盘 X 坐标。
     * @param y 棋盘 Y 坐标。
     * @return 对应的控制台坐标。
     */
    COORD getCOORD(short x, short y);
};

#endif  // UI_H