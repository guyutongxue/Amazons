/**
 * Copyright (c) 2019 Guyutongxue
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "amazons.h"

int main() {
    UI ui;
    int choice;
    while (choice = ui.printMainMenu(), choice != 2) {
        if (choice == 0) {
            int mode = ui.printModeMenu();
            if (mode != 4) {
                int pauseChoice=0;
                Amazons amazons((GameMode)mode);
                do{
                    amazons.play(&ui);
                    if(amazons.isOver){
                        ui.printEnd(amazons.winner);
                        break;
                    }
                    pauseChoice = ui.printPauseMenu();
                    if(pauseChoice==1){
                        amazons.save();
                    }
                }while(pauseChoice==0);
            }
        }
        else{
            Amazons amazons;
            int pauseChoice=0;
            do{
                amazons.play(&ui);
                if(amazons.isOver){
                    ui.printEnd(amazons.winner);
                    break;
                }
                pauseChoice = ui.printPauseMenu();
                if(pauseChoice==1){
                    amazons.save();
                }
            }while(pauseChoice==0);
        }
    }
    return 0;
}