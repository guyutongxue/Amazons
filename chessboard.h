/**
 * Copyright (c) 2019 Guyutongxue
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef CHECKBOARD_H
#define CHECKBOARD_H

#include <cstring>
#include <iostream>

enum class Square { Empty = 0, White = 1, Black = -1, Arrow = 2 };
enum class Player { White = 1, Black = -1 };

class Move {
public:
    int x0, y0;
    int x1, y1;
    int x2, y2;
    friend class Bot;
    bool operator>(Move b) {
        return value > b.value;
    }
    Move(){
        x0=x1=x2=y0=y1=y2=0;
    }

private:
    double value;
};

class Chessboard {
public:
    Chessboard();
    Chessboard(const Chessboard& origin);
    Chessboard(const int (&origin)[8][8]);
    static const Chessboard start;
    Chessboard& operator=(const Chessboard& src);
    Square& at(int x, int y);
    const Square& at(int x, int y) const;
    void print();
    
    // 判断是否在棋盘内
    inline static bool isInside(int x, int y) {
        return x >= 0 && x < 8 && y >= 0 && y < 8;
    }

    friend class Bot;

private:
    Square board[8][8];
};

#endif  // CHECHKBOARD_H