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
#include <fstream>
#include <string>
#include "bot.h"
#include "human.h"

enum class GameMode { HumanBot = 0, BotHuman = 1, BotBot = 2, HumanHuman = 3 };

class Amazons {
public:
    Amazons(GameMode gamemode);
    Amazons();
    ~Amazons();
    bool isOver;
    Piece winner;
    const Chessboard& getChessboard();
    void play(UI* pUi);
    void save()const;

private:
    Chessboard chessboard;
    GameMode gameMode;
    std::string nameBlack;
    std::string nameWhite;
    Piece firstHand;
    int turns;
    void _play(IPlayer* black, IPlayer* white, UI* pUi);
    bool canMove(Piece pl) const;
    void step(Move move);
};

#endif  // AMAZONS_H