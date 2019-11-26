/**
 * Copyright (c) 2019 Guyutongxue
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "chessboard.h"

Chessboard::Chessboard() {
    memset(board, (char)Square::Empty, sizeof(board));
}

Chessboard::Chessboard(const Chessboard& origin) {
    memcpy(board, origin.board, sizeof(board));
}

Chessboard::Chessboard(const int (&origin)[8][8]) {
    memcpy(board, origin, sizeof(board));
}

Chessboard& Chessboard::operator=(const Chessboard& src) {
    memcpy(board, src.board, sizeof(board));
    return *this;
}

Square& Chessboard::at(int x, int y) {
    return board[x][y];
}

const Square& Chessboard::at(int x, int y) const {
    return board[x][y];
}

const Chessboard Chessboard::start =
    (int[8][8]){{0, 0, 1, 0, 0, 1, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0},  {1, 0, 0, 0, 0, 0, 0, 1},
                {0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0},  {-1, 0, 0, 0, 0, 0, 0, -1},
                {0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, -1, 0, 0, -1, 0, 0}};

void Chessboard::print() {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            std::cout << (int)board[i][j] << " ";
        }
        std::cout << std::endl;
    }
}