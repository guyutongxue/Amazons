/**
 * Copyright (c) 2019 Guyutongxue
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "amazons.h"

Amazons::Amazons() {
    chessboard=Chessboard::start;
}

const Chessboard& Amazons::getChessboard(){
    return chessboard;
}

void Amazons::step(Move move){
    chessboard.at(move.x1,move.y1)=chessboard.at(move.x0,move.y0);
    chessboard.at(move.x0,move.y0)=Square::Empty;
    chessboard.at(move.x2,move.y2)=Square::Arrow;
}

bool Amazons::isOver(Player pl)const{
    int dx[8] = {-1, -1, -1, 0, 0, 1, 1, 1};
    int dy[8] = {-1, 0, 1, -1, 1, -1, 0, 1};
    struct Coordinate{int x;int y;} pos[4];
    {
        int counter = 0;
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                if (int(chessboard.at(i, j)) == int(pl)) {
                    pos[counter].x = i;
                    pos[counter].y = j;
                    counter++;
                }
            }
            if (counter == 4) break;
        }
    }
    for (int i = 0; i < 4; i++) {
        int k, l, len1, len2, x1, x2, y1, y2;
        for (k = 0; k < 8; k++) {
            for (len1 = 1; len1 < 8; len1++) {
                x1 = pos[i].x + dx[k] * len1;
                y1 = pos[i].y + dy[k] * len1;
                if (chessboard.at(x1, y1) != Square::Empty || !(x1 >= 0 && x1 < 8 && y1 >= 0 && y1 < 8)) break;
                for (l = 0; l < 8; l++) {
                    for (len2 = 1; len2 < 8; len2++) {
                        x2 = x1 + dx[l] * len2;
                        y2 = y1 + dy[l] * len2;
                        if (!(x2 >= 0 && x2 < 8 && y2 >= 0 && y2 < 8)) break;
                        if (chessboard.at(x1, y1) != Square::Empty &&
                            !(pos[i].x == x2 && pos[i].y == y2))
                            break;
                        return false;
                    }
                }
            }
        }
    }
    return true;
}