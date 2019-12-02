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
    while (choice = ui.printMainMenu(), choice != 3) {
        if(choice==2){
            ui.printAbout();
            continue;
        }
        Amazons amazons;
        if (choice == 0) {
            int mode = ui.printModeMenu();
            if (mode == 4) continue;
            amazons.setGameMode(GameMode(mode));
        } else {
            std::string path = ui.printSL();
            if (!amazons.load(path)) {
                ui.printSLMsg(false);
                continue;
            }
            ui.printSLMsg(true);
        }
        int pauseChoice;
        do {
            amazons.play(&ui);
            if (amazons.isOver) {
                ui.printEnd(amazons.winner);
                break;
            }
            while (pauseChoice = ui.printPauseMenu(), pauseChoice == 1) {
                std::string path = ui.printSL();
                if (amazons.save(path)) {
                    ui.printSLMsg(true);
                    break;
                }
                ui.printSLMsg(false);
            }
        } while (pauseChoice == 0);
    }
    return 0;
}