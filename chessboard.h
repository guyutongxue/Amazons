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

class Chessboard {
public:
    Chessboard();
    Chessboard(const Chessboard& origin);
    Chessboard(const int (&origin)[8][8]);
    static const Chessboard start;
    Chessboard& operator=(const Chessboard& src);
    Square& at(int x,int y);
    void print();

private:
    Square board[8][8];
};

#endif  // CHECHKBOARD_H