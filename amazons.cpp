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
    chessboard.at(move.x0,move.y0)=Square::Arrow;
}