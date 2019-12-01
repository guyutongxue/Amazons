/**
 * Copyright (c) 2019 Guyutongxue
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "amazons.h"

Amazons::Amazons(GameMode gamemode) {
    chessboard = Chessboard::start;
    gameMode = gamemode;
    firstHand = Piece::Black;
    isOver = false;
    turns = 1;
}

Amazons::Amazons(){
    std::ifstream input("backup.amz",std::ios::binary);
    char a,b;
    for(int i=0;i<8;i++){
        for(int j=0;j<8;j++){
            input>>a;
            chessboard.at(i,j)=(Square)(int(a));
        }
    }
    input>>a>>b;
    gameMode=(GameMode)(int(a));
    firstHand=(Piece)(int(b));
    input.read((char*)&turns,sizeof(int));
    input.close();
    std::cout<<turns;
    isOver=false;
}

Amazons::~Amazons() {}

const Chessboard& Amazons::getChessboard() {
    return chessboard;
}

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
            Human* human = new Human(Piece::Black, pUi);
            Bot* bot = new Bot(Piece::White);
            _play(human, bot, pUi);
            delete human;
            delete bot;
            break;
        }
        case GameMode::BotHuman: {
            Bot* bot = new Bot(Piece::Black);
            Human* human = new Human(Piece::White, pUi);
            _play(bot, human, pUi);
            delete human;
            delete bot;
            break;
        }
        case GameMode::BotBot: {
            Bot* bot1 = new Bot(Piece::Black);
            Bot* bot2 = new Bot(Piece::White);
            _play(bot1, bot2, pUi);
            delete bot1;
            delete bot2;
            break;
        }
        case GameMode::HumanHuman: {
            Human* human1 = new Human(Piece::Black, pUi);
            Human* human2 = new Human(Piece::White, pUi);
            _play(human1, human2, pUi);
            delete human1;
            delete human2;
            break;
        }
    }
}

void Amazons::_play(IPlayer* black, IPlayer* white, UI* pUi) {
    Move move;
    IPlayer* current = black;
    pUi->printBoardBackground();
    pUi->printGame(chessboard);
    while (true) {
        if (!canMove(firstHand)) {
            // 若当前先手无路可走，则后手获胜
            winner = firstHand == Piece::White ? Piece::Black : Piece::White;
            isOver = true;
            return;
        }
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

void Amazons::save() const {
    std::ofstream output("backup.amz", std::ios::binary | std::ios::trunc);
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            output << char(int(chessboard.at(i, j)));
        }
    }
    output << char(int(gameMode)) << char(int(firstHand));
    output.write((char*)&turns, sizeof(int));
    output.close();
}