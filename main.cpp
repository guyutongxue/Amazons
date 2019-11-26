/**
 * Copyright (c) 2019 Guyutongxue
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "ui.h"
#include "bot.h"

int main() {
    UI ui;
    int choice;
    while (choice = ui.printMainMenu(), choice != 2) {
        if (choice == 0) {
            int mode = ui.printModeMenu();
            switch (mode) {
                case 0:
                case 1:
                case 2: {
                    Amazons amazons;
                    ui.printBoardBackground();
                    ui.printGame(amazons.getChessboard());
                    Bot bot;
                    for(int i=1;;i++){
                        getchar();
                        Move move=bot.execute(amazons.getChessboard(),i,Player::White);
                        //std::cout<<move.x0<<move.y0<<move.x1<<move.y1<<move.x2<<move.y2;
                        if(amazons.isOver(Player::White)){
                            std::cout<<"Black win";
                            system("pause");
                            break;
                        }
                        amazons.step(move);
                        ui.printGame(amazons.getChessboard());
                        getchar();
                        move=bot.execute(amazons.getChessboard(),i,Player::Black);
                        //std::cout<<move.x0<<move.y0<<move.x1<<move.y1<<move.x2<<move.y2;
                        if(amazons.isOver(Player::Black)){
                            std::cout<<"White win";
                            system("pause");
                            break;
                        }
                        amazons.step(move);
                        ui.printGame(amazons.getChessboard());
                    }
                    break;
                }
                case 3:
                    break;
                default:
                    break;
            }
        }
    }
    return 0;
}