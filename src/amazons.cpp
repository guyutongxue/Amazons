// Copyright (c) 2019 Guyutongxue
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "amazons.h"

Amazons::Amazons() {
    chessboard = Chessboard::getStart();
    gameMode = GameMode::BotHuman;
    firstHand = Piece::Black;
    isOver = false;
    turns = 1;
}

void Amazons::setGameMode(GameMode gamemode) {
    gameMode = gamemode;
}

bool Amazons::load(std::string path) {
    std::ifstream input(path, std::ios::binary);
    input.unsetf(std::ios_base::skipws);
    if (input.fail()) return false;
    unsigned char a, b;
    // Magic Numbers
    input >> a >> b;
    if (a != (unsigned char)0xAA || b != (unsigned char)0x20) return false;
    signed char c, d;
    input >> c >> d;
    gameMode = (GameMode)(int(c));
    firstHand = (Piece)(int(d));
    input.read((char*)&turns, sizeof(int));
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            input >> c;
            chessboard.at(i, j) = (Square)(int(c));
        }
    }
    isOver = false;
    return true;
}

Amazons::~Amazons() {}

void Amazons::step(Move move) {
    chessboard.at(move.x1, move.y1) = chessboard.at(move.x0, move.y0);
    chessboard.at(move.x0, move.y0) = Square::Empty;
    chessboard.at(move.x2, move.y2) = Square::Arrow;
}

bool Amazons::canMove(Piece pl) const {
    int dx[8] = {-1, -1, -1, 0, 0, 1, 1, 1};
    int dy[8] = {-1, 0, 1, -1, 1, -1, 0, 1};
    struct Coordinate {
        int x;
        int y;
    } pos[4];
    {  // 获取棋子位置
        int cnt = 0;
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                if (int(chessboard.at(i, j)) == int(pl)) {
                    pos[cnt].x = i;
                    pos[cnt].y = j;
                    cnt++;
                }
            }
            if (cnt == 4) break;
        }
    }
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 8; j++) {
            int x1 = pos[i].x + dx[j];
            int y1 = pos[i].y + dy[j];
            if (Chessboard::isInside(x1, y1) && chessboard.at(x1, y1) == Square::Empty) return true;
        }
    }
    return false;
}

void Amazons::play(UI* pUi) {
    switch (gameMode) {
        case GameMode::HumanBot: {
            auto human=std::make_shared<Human>(Piece::Black, pUi);
            auto bot=std::make_shared<Bot>(Piece::White);
            nameBlack = "您";
            nameWhite = "Bot";
            _play(human, bot, pUi);
            break;
        }
        case GameMode::BotHuman: {
            auto bot=std::make_shared<Bot>(Piece::Black);
            auto human=std::make_shared<Human>(Piece::White, pUi);
            nameBlack = "Bot";
            nameWhite = "您";
            _play(bot, human, pUi);
            break;
        }
        case GameMode::BotBot: {
            auto bot1=std::make_shared<Bot>(Piece::Black);
            auto bot2=std::make_shared<Bot>(Piece::White);
            nameBlack = "Bot 1";
            nameWhite = "Bot 2";
            _play(bot1, bot2, pUi);
            break;
        }
        case GameMode::HumanHuman: {
            auto human1=std::make_shared<Human>(Piece::Black, pUi);
            auto human2=std::make_shared<Human>(Piece::White, pUi);
            nameBlack = "玩家1";
            nameWhite = "玩家2";
            _play(human1, human2, pUi);
            break;
        }
    }
}

void Amazons::_play(std::shared_ptr<IPlayer> black, std::shared_ptr<IPlayer> white, UI* pUi) {
    Move move;
    std::shared_ptr<IPlayer> current = black;
    pUi->printBoardBackground(nameBlack, nameWhite);
    pUi->printGame(chessboard);
    while (true) {
        if (!canMove(firstHand)) {
            // 若当前先手无路可走，则后手获胜
            winner = firstHand == Piece::White ? Piece::Black : Piece::White;
            isOver = true;
            return;
        }
        pUi->switchPlayers(firstHand);
        pUi->printCalcHelp();
        if (!current->execute(chessboard, turns, move)) return;
        step(move);
        pUi->printGame(chessboard, move);
        if (firstHand == Piece::Black) {
            firstHand = Piece::White;
            current = white;
        } else {
            firstHand = Piece::Black;
            current = black;
            turns++;
        }
    }
}

bool Amazons::save(std::string path) const {
    std::ofstream output(path, std::ios::binary | std::ios::trunc);
    if (output.fail()) return false;
    // Magic Numbers
    output << (unsigned char)0xAA << (unsigned char)0x20;

    output << (signed char)(int)gameMode << (signed char)(int)firstHand;
    output.write((char*)&turns, sizeof(int));
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            output << (signed char)(int)chessboard.at(i, j);
        }
    }
    return true;
}