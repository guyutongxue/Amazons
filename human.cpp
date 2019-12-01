/**
 * Copyright (c) 2019 Guyutongxue
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "human.h"

Human::Human(Piece piece,UI* pUi) {
    this->piece = piece;
    this->pUi=pUi;
}

bool Human::execute(Chessboard board,int turns,Move& move){
    return pUi->generateMove(board,piece,move);
}