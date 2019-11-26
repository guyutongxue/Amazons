/**
 * Copyright (c) 2019 Guyutongxue
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "ui.h"

UI::UI() {
    // 更换输出代码页到 UTF-8
    SetConsoleOutputCP(CP_UTF8);
    // 获取标准输出句柄
    hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    // 获取控制台信息
    GetConsoleScreenBufferInfo(hOut, &bInfo);
    center_x = bInfo.dwSize.X / 2 - 9;

    COORD bufSize={bInfo.dwSize.X,30};
    SetConsoleScreenBufferSize(hOut,bufSize);

    // 设置控制台标题为“亚马逊棋”（使用 Unicode(UTF-16-BE) ）
    SetConsoleTitleW(L"亚马逊棋");

    setColor(Color::White, Color::Black);

    // 隐藏光标
    CONSOLE_CURSOR_INFO cInfo = {1, 0};
    SetConsoleCursorInfo(hOut, &cInfo);

}

UI::~UI() {
    CloseHandle(hOut);
}

int UI::printMainMenu(){
    std::string chs[3]={"1. 开始游戏","2. 读取游戏","3. 退出"};
    short pos[3]={5,10,15};
    return printMenu("亚马逊棋",chs,pos,3);
}

int UI::printModeMenu(){
    std::string chs[4]={"1. 人机对战","2. 机机对战","3. 人人对战","4. 返回"};
    short pos[4]={5,10,15,20};
    return printMenu("选择模式",chs,pos,4);
}

int UI::printMenu(const std::string& title,std::string* choices,short* pos,int num) {
    clearScreen();
    setPos(center_x - 4, 0);
    std::cout << title;
    for (int i = 0; i < num; i++) {
        setPos(center_x - 6, pos[i]);
        std::cout << choices[i];
    }
    DWORD buffer;
    int keycode, currentChoice = 0;
    // 选中第0选项
    FillConsoleOutputAttribute(hOut, 0xF0, 15, COORD{short(center_x - 7), pos[currentChoice]},
                               &buffer);
    while (keycode = getch(), keycode != 13) {
        keycode = getch();
        // 取消选中选项
        FillConsoleOutputAttribute(hOut, 0x0F, 15, COORD{short(center_x - 7), pos[currentChoice]},
                                   &buffer);
        switch (keycode) {
            case 72: {  // up
                currentChoice = (currentChoice == 0 ? 0 : currentChoice - 1);
                break;
            }
            case 80: {  // down
                currentChoice = (currentChoice == num-1 ? num-1 : currentChoice + 1);
                break;
            }
            default:
                break;
        }
        // 选中当前选项
        FillConsoleOutputAttribute(hOut, 0xF0, 15, COORD{short(center_x - 7), pos[currentChoice]},
                                   &buffer);
    }
    return currentChoice;
    // setPos(center_x-)
}

void UI::clearScreen() {
    DWORD buffer;
    for (short i = 0; i < 30; i++) {
        COORD pos = {0, i};
        FillConsoleOutputAttribute(hOut, 0x0F, bInfo.dwSize.X - 10, pos, &buffer);
        FillConsoleOutputCharacter(hOut, ' ', bInfo.dwSize.X - 10, pos, &buffer);
    }
}

void UI::printBoardBackground() {
    setColor(Color::Black, Color::White);
    for (int i = 0; i < 17; i++) {
        setPos(center_x - 9, i);
        std::cout << boardLine[i] << std::endl;
    }
}

void UI::printGame(Chessboard board) {
    setColor(Color::Black, Color::White);
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            setPos(center_x - 9 + i * 2 + 1, j * 2 + 1);
            if (board.at(i, j) == Square::White) {
                std::cout << "○";
            } else if (board.at(i, j) == Square::Black) {
                std::cout << "●";
            }
            else if (board.at(i, j) == Square::Arrow) {
                std::cout << "×";
            }
            else std::cout << " ";
        }
    }
    // setPos(ori_x,17);
    // std::cout<<" 0 1 2 3 4 5 6 7 "<<std::endl;
    setColor(Color::White, Color::Black);
    setPos(center_x - 9, 18);
}

void UI::setPos(short x, short y) {
    COORD pos = {x, y};
    SetConsoleCursorPosition(hOut, pos);
}

void UI::setColor(Color foreground, Color background) {
    WORD wColor = short(foreground) + short(background) * 16;
    SetConsoleTextAttribute(hOut, wColor);
}