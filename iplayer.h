/**
 * Copyright (c) 2019 Guyutongxue
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef IPLAYER_H
#define IPLAYER_H

#include "ui.h"

class IPlayer {
public:
    virtual bool execute(Chessboard board, int turns, Move& move) = 0;
};

#endif  // IPLAYER_H