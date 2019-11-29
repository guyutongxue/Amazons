/**
 * Copyright (c) 2019 Guyutongxue
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "amazons.h"
#include "ui.h"
#include "bot.h"

int main() {
    UI ui;
    int choice;
    while (choice = ui.printMainMenu(), choice != 2) {
        if (choice == 0) {
            int mode = ui.printModeMenu();
            switch (mode) {
                case 0:{
                    Amazons amazons;
                    Move move;
                    ui.printBoardBackground();
                    ui.printGame(amazons.getChessboard());
                    Bot bot(Player::White);
                    for(int i=1;;i++){
                        if(amazons.isOver(Player::Black)){
                            std::cout<<"White win\n";
                            system("pause");
                            break;
                        }
                        if(!ui.generateMove(amazons.getChessboard(),Player::Black,move))break;
                        amazons.step(move);
                        ui.printGame(amazons.getChessboard(),move);

                        if(amazons.isOver(Player::White)){
                            std::cout<<"Black win\n";
                            system("pause");
                            break;
                        }
                        move=bot.execute(amazons.getChessboard(),i);
                        amazons.step(move);
                        ui.printGame(amazons.getChessboard(),move);
                    }
                    break;
                }
                case 1: {
                    Amazons amazons;
                    Move move;
                    ui.printBoardBackground();
                    ui.printGame(amazons.getChessboard());
                    Bot bot1(Player::Black),bot2(Player::White);
                    for(int i=1;;i++){
                        getchar();
                        if(amazons.isOver(Player::Black)){
                            std::cout<<"White win\n";
                            system("pause");
                            break;
                        }
                        move=bot1.execute(amazons.getChessboard(),i);
                        amazons.step(move);
                        ui.printGame(amazons.getChessboard(),move);

                        getchar();
                        if(amazons.isOver(Player::White)){
                            std::cout<<"Black win\n";
                            system("pause");
                            break;
                        }
                        move=bot2.execute(amazons.getChessboard(),i);
                        amazons.step(move);
                        ui.printGame(amazons.getChessboard(),move);
                    }
                    break;
                }
                case 2:
                case 3:
                    break;
                default:
                    break;
            }
        }
    }
    return 0;
}