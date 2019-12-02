// Copyright (c) 2019 Guyutongxue
// 
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef AMAZONS_H
#define AMAZONS_H

#include <fstream>
#include <iostream>
#include <string>
#include "bot.h"
#include "human.h"

/**
 * @brief 游戏模式枚举类。
 * 设定亚马逊棋的游戏模式。
 */
enum class GameMode {
    HumanBot = 0,  /**< 人-机对战模式（人类先手）   */
    BotHuman = 1,  /**< 机-人对战模式（计算机先手） */
    BotBot = 2,    /**< 机-机对战模式               */
    HumanHuman = 3 /**< 人-人对战模式               */
};

/**
 * @brief 亚马逊棋游戏类。
 * 管理游戏的读取、保存、游戏的步进，并判断游戏结果。
 */
class Amazons {
public:
    /**
     * @brief 默认构造函数。
     * 初始化游戏棋盘为开局布局，初始化先手为黑棋。
     */
    Amazons();

    /**
     * @brief 默认析构函数。（空）
     */
    ~Amazons();

    /**
     * @brief 标记游戏是否结束。
     */
    bool isOver;

    /**
     * @brief 胜利执棋方。
     * 仅当 @c isOver 为真时获取。
     */
    Piece winner;

    /**
     * @brief 进行游戏。
     * 开始从 @c firstHand 设定的先手进行游戏，游戏结束或者暂停便返回。
     * @param  pUi 指向 @c UI 对象的指针，用于读取人类玩家落子或暂停。
     */
    void play(UI* pUi);

    /**
     * @brief 保存游戏到文件。
     * 将当前棋局、游戏模式、先手方和回合数写入文件。
     * @param path 写入文件路径。
     * @return 指示是否写入成功。
     */
    bool save(std::string path) const;

    /**
     * @brief 从文件读取游戏。
     * 从文件读取棋局、游戏模式、先手方和回合数。
     * @param path 读取文件路径。
     * @return 指示是否读取成功。
     */
    bool load(std::string path);

    /**
     * @brief 设置游戏模式。
     * @param gamemode 要设置的游戏模式。
     */
    void setGameMode(GameMode gamemode);

private:
    Chessboard chessboard;
    GameMode gameMode;
    std::string nameBlack;
    std::string nameWhite;
    Piece firstHand;
    int turns;
    void _play(IPlayer* black, IPlayer* white, UI* pUi);
    bool canMove(Piece pl) const;
    void step(Move move);
};

#endif  // AMAZONS_H