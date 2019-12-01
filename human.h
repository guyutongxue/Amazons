/**
 * Copyright (c) 2019 Guyutongxue
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef HUMAN_H
#define HUMAN_H

#include "iplayer.h"

class Human : public IPlayer {
public:
    Human(Piece piece, UI* pUi);
    bool execute(Chessboard board, int turns, Move& move)override;

private:
    Piece piece;
    UI* pUi;
};

#endif // HUMAN_H