// Copyright (c) 2019 Guyutongxue
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "amazons.h"

void playLoop(Amazons& amazons, UI& ui);

int main(int argc, char* argv[]) {
    UI ui;
    if (argc > 1) {
        Amazons amazons;
        if (!amazons.load(argv[1])) {
            ui.printSLMsg(false);
        } else {
            ui.printSLMsg(true);
            playLoop(amazons, ui);
        }
    }
    int choice;
    while (choice = ui.printMainMenu(), choice != 3) {
        if (choice == 2) {
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
        playLoop(amazons, ui);
    }
    ui.clearScreen();
    return 0;
}

void playLoop(Amazons& amazons, UI& ui) {
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