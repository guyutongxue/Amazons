// Copyright (c) 2019 Guyutongxue
// 
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef IPLAYER_H
#define IPLAYER_H

#include "ui.h"

/**
 * @brief 玩家接口。
 * 统一计算机玩家与人类玩家的接口。
*/
class IPlayer {
public:

    /**
     * @brief 执行玩家决策的纯虚函数。
     * @param board 当前棋盘状态。
     * @param turns 当前游戏回合数。
     * @param[out] move  玩家决定的落子。
     * @return 是否成功决策。
     */
    virtual bool execute(Chessboard board, int turns, Move& move) = 0;
};

#endif  // IPLAYER_H