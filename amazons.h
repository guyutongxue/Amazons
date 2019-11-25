/**
 * Copyright (c) 2019 Guyutongxue
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef AMAZONS_H
#define AMAZONS_H

#include <iostream>
#include "chessboard.h"

enum class GameMode { HumanBot = 0, BotBot = 1, HumanHuman = 2 };

class Amazons {
public:
    GameMode gameMode;
    Amazons();
    void step(Move move);
    const Chessboard& getChessboard();

private:
    Chessboard chessboard;
};

#endif  // AMAZONS_H