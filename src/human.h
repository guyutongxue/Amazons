// Copyright (c) 2019 Guyutongxue
// 
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef HUMAN_H
#define HUMAN_H

#include "iplayer.h"

/**
 * @brief 人类玩家类。
 * 继承 @c IPlayer 接口，通过 @c UI 获取人类玩家决策。
 */
class Human : public IPlayer {
public:
    /**
     * @brief 通过执棋方和 @c UI 构造函数。
     * @param piece 计算机玩家的执棋方。
     * @param pUi 指向 @c UI 对象的指针，用于读取人类决策。
     */
    Human(Piece piece, UI* pUi);

    /**
     * @brief 默认析构函数。（空）
     */
    ~Human();

    /**
     * @brief 执行人类玩家的决策。
     * 实现 @c IPlayer 执行决策的纯虚函数。
     * @param board 当前棋盘状态。
     * @param turns 当前游戏回合数。
     * @param[out] move 人类玩家决定的落子。
     * @return 是否成功决策。若人类玩家选择暂停，返回假。
     */
    bool execute(Chessboard board, int turns, Move& move)override;

private:
    Piece piece;
    UI* pUi;
};

#endif // HUMAN_H