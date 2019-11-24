/**
 * Copyright (c) 2019 Guyutongxue
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "ui.h"
#include "amazons.h"

using std::cin;
using std::cout;
using std::endl;

UI::UI() {
    hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    GetConsoleScreenBufferInfo(hOut, &bInfo);

    SetConsoleTitle(TEXT("ÑÇÂíÑ·Æå"));
}

UI::~UI() {
    CloseHandle(hOut);
}

void UI::printGame() {
    Chessboard target = game.getChessboard();
    short ori_x = bInfo.dwSize.X / 2 - 9;
    setColor(Color::Black, Color::White);
    for (int i = 0; i < 17; i++) {
        setPos(ori_x, i);
        std::cout << boardLine[i] << std::endl;
    }
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (target.at(i, j) == Square::White) {
                setPos(ori_x + i * 2 + 1, j * 2 + 1);
                cout << "¡ð";
            } else if (target.at(i, j) == Square::Black) {
                setPos(ori_x + i * 2 + 1, j * 2 + 1);
                cout << "¡ñ";
            }
        }
    }
    setColor(Color::White, Color::Black);
    setPos(ori_x, 18);
}

void UI::setPos(short x, short y) {
    COORD pos = {x, y};
    SetConsoleCursorPosition(hOut, pos);
}

void UI::setColor(Color foreground, Color background) {
    WORD wColor = short(foreground) + short(background) * 16;
    SetConsoleTextAttribute(hOut, wColor);
}