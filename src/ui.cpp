// Copyright (c) 2019 Guyutongxue
// 
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "ui.h"

UI::UI() {
    // 更换输出代码页到 UTF-8
    SetConsoleOutputCP(CP_UTF8);
    // 获取标准输出句柄
    hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    // 获取控制台信息
    GetConsoleScreenBufferInfo(hOut, &bInfo);

    if (bInfo.dwSize.X < 80) bInfo.dwSize.X = 80;
    center_x = bInfo.dwSize.X / 2;

    COORD bufSize = {bInfo.dwSize.X, 30};
    SetConsoleScreenBufferSize(hOut, bufSize);

    // 设置控制台标题为“亚马逊棋”（使用 Unicode(UTF-16-BE) ）
    SetConsoleTitleW(L"亚马逊棋");

    setTextColor(Color::White, Color::Black);

    // 隐藏光标
    CONSOLE_CURSOR_INFO cInfo = {1, 0};
    SetConsoleCursorInfo(hOut, &cInfo);
}

UI::~UI() {
    CloseHandle(hOut);
}

int UI::printPauseMenu() {
    clearScreen();
    for (int i = 0; i < 5; i++) {
        setCursorPos(center_x - 17, i + 3);
        std::cout << graphicPaused[i];
    }
    std::string chs[3] = {"1. 继续游戏", "2. 保存游戏", "3. 结束游戏"};
    short pos[3] = {12, 15, 18};
    return printMenu("游戏已暂停", chs, pos, 3);
}

int UI::printMainMenu() {
    clearScreen();
    for (int i = 0; i < 6; i++) {
        setCursorPos(center_x - 27, i + 3);
        std::cout << graphicTitle[i];
    }
    std::string chs[4] = {"1. 开始游戏", "2. 读取游戏", "3. 关于","4. 退出"};
    short pos[4] = {12, 15, 18, 21};
    return printMenu("亚马逊棋", chs, pos, 4);
}

int UI::printModeMenu() {
    clearScreen();
    std::string chs[5] = {"1. 人-机对战", "2. 机-人对战", "3. 机-机对战", "4. 人-人对战",
                          "5. 返回"};
    short pos[5] = {6, 9, 12, 15, 18};
    return printMenu("选择模式", chs, pos, 5);
}

int UI::printMenu(const std::string& title, std::string* choices, short* pos, int num) {
    setTextColor(Color::White, Color::Black);
    setCursorPos(center_x - 4, 0);
    std::cout << title;
    printMenuHelp();
    for (int i = 0; i < num; i++) {
        setCursorPos(center_x - 6, pos[i]);
        std::cout << choices[i];
    }
    DWORD buffer;
    int keycode, currentChoice = 0;
    // 选中第0选项
    FillConsoleOutputAttribute(hOut, 0xF0, 15, COORD{short(center_x - 7), pos[currentChoice]},
                               &buffer);
    while (keycode = _getch(), keycode != 13) {
        if (keycode == 0 || keycode == 0xE0) {
            keycode = _getch();
            keycode += 300;
        }
        if (keycode >= '1' && keycode <= num + 1 + '0') {
            return keycode - '1';
        }
        // 取消选中选项
        FillConsoleOutputAttribute(hOut, 0x0F, 15, COORD{short(center_x - 7), pos[currentChoice]},
                                   &buffer);
        switch (keycode) {
            case 372: {  // up
                currentChoice = (currentChoice == 0 ? 0 : currentChoice - 1);
                break;
            }
            case 380: {  // down
                currentChoice = (currentChoice == num - 1 ? num - 1 : currentChoice + 1);
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

void UI::printEnd(Piece winner) {
    DWORD buffer;
    COORD pos = {0, 0};
    FillConsoleOutputAttribute(hOut, 0x0F, bInfo.dwSize.X, pos, &buffer);
    setCursorPos(center_x - 10, 21);
    if (winner == Piece::Black)
        std::cout << "黑方";
    else
        std::cout << "白方";
    std::cout << "胜利。";
    printEndHelp();
    _getch();
}

void UI::printAbout(){
    clearScreen();
    setCursorPos(center_x-2,10);
    std::cout<<"关于";
    for(int i=0;i<4;i++){
        setCursorPos(center_x-25,13+i);
        std::cout<<aboutText[i];
    }
    printEndHelp();
    _getch();
}

void UI::clearScreen() {
    DWORD buffer;
    for (short i = 0; i < 30; i++) {
        COORD pos = {0, i};
        FillConsoleOutputAttribute(hOut, 0x0F, bInfo.dwSize.X, pos, &buffer);
        FillConsoleOutputCharacter(hOut, ' ', bInfo.dwSize.X, pos, &buffer);
    }
}

void UI::printBoardBackground(std::string black,std::string white) {
    clearScreen();
    setCursorPos(0, 0);
    std::cout << "黑方：" << black;
    setCursorPos(bInfo.dwSize.X - 15, 0);
    std::cout << "白方：" << white;
    setTextColor(Color::Black, Color::White);
    for (int i = 0; i < 17; i++) {
        setCursorPos(center_x - 17, i + 2);
        std::cout << boardLine[i];
    }
    setTextColor(Color::White, Color::Black);
}

void UI::printGame(const Chessboard& board) {
    for (int j = 0; j < 8; j++) {
        for (int i = 0; i < 8; i++) {
            SetConsoleCursorPosition(hOut, getCOORD(i, j));
            // 使用不同的终端字体时，棋子的宽度不确定。
            // 故不输出棋子后面的空格
            if (board.at(i, j) == Square::White) {
                std::cout << " ○";
            } else if (board.at(i, j) == Square::Black) {
                std::cout << " ●";
            } else if (board.at(i, j) == Square::Arrow) {
                std::cout << " ×";
            } else {
                std::cout << "   ";
            }
            setPosColor(Color::Black, Color::White, i, j);
        }
    }
    setCursorPos(center_x - 9, 18);
}

void UI::printGame(const Chessboard& board, Move lastmove) {
    printGame(board);
    setPosColor(Color::Black, Color::LightGrey, lastmove.x1, lastmove.y1);
    setPosColor(Color::Black, Color::LightGrey, lastmove.x2, lastmove.y2);
}

std::string UI::printSL() {
    printInputHelp();
    std::string result =
        printInputField(10, 16, bInfo.dwSize.X - 20, "输入存档路径（默认 backup.amz ）：");
    if (result == "") return "backup.amz";
    return result;
}

void UI::printSLMsg(bool isSuccess) {
    if (!isSuccess)
        printMessage(center_x - 4, 16, "读写失败。", true);
    else
        printMessage(center_x - 4, 16, "读写成功。", false);
    Sleep(1000);
}

std::string UI::printInputField(short x, short y, short length, std::string prompt) {
    setCursorPos(x, y);
    DWORD buffer;
    COORD pos{x, y};
    // 显示光标
    CONSOLE_CURSOR_INFO cInfo = {1, 1};
    SetConsoleCursorInfo(hOut, &cInfo);

    // 设置文本框底色
    FillConsoleOutputAttribute(hOut, 0x80, length, pos, &buffer);

    // 设置提示文本
    setTextColor(Color::White, Color::Black);
    std::cout << prompt;
    std::string result;

    // 获取单行输入
    setTextColor(Color::White, Color::DarkGrey);
    std::getline(std::cin, result);

    // 恢复属性
    FillConsoleOutputAttribute(hOut, 0x0F, length, pos, &buffer);
    FillConsoleOutputCharacter(hOut, ' ', length, pos, &buffer);
    setTextColor(Color::White, Color::Black);
    cInfo.bVisible = 0;
    SetConsoleCursorInfo(hOut, &cInfo);
    return result;
}

void UI::printMessage(short x, short y, std::string text, bool isWarning) {
    setCursorPos(x, y);
    COORD pos{x, y};
    setTextColor(Color::White, isWarning ? Color::Red : Color::Green);
    std::cout << text;
    setTextColor(Color::White, Color::Black);
}

void UI::printHelpHeader() {
    COORD pos = {0, 23};
    DWORD buffer;
    FillConsoleOutputCharacter(hOut, ' ', bInfo.dwSize.X * 6, pos, &buffer);
    setCursorPos(0, 23);
    std::cout << helpTitle;
}

void UI::printMenuHelp() {
    printHelpHeader();
    std::cout << "使用[上键][下键]切换选项，按下[回车键]确定。\n";
    std::cout << "也可以按下[数字键]选择对应选项。" << std::endl;
}

void UI::printInputHelp() {
    printHelpHeader();
    std::cout << "按下[回车键]确定。" << std::endl;
}

void UI::printCalcHelp() {
    printHelpHeader();
    std::cout << "正在博弈中......请耐心等待。" << std::endl;
}

void UI::printMoveHelp(int status) {
    printHelpHeader();
    std::cout << "使用任意下列三组按键来控制";
    switch (status) {
        case 1: {
            std::cout << "移动的棋子。\n";
            break;
        }
        case 2: {
            std::cout << "移动的终点。\n";
            break;
        }
        case 3: {
            std::cout << " Arrow 的位置。\n";
            break;
        }
    }
    std::cout << " 7 8 9  |  Q W E  |    ^\n";
    std::cout << " 4   6  |  A   D  |  <   >\n";
    std::cout << " 1 2 3  |  Z S C  |    v\n";
    std::cout << "按下[回车键]确定，按下[ESC]" << (status == 1 ? "暂停。" : "取消。") << std::endl;
}

void UI::printEndHelp() {
    printHelpHeader();
    std::cout << "按下任意键返回主菜单。" << std::endl;
}

void UI::switchPlayers(Piece piece) {
    DWORD buffer;
    COORD pos = {0, 0};
    FillConsoleOutputAttribute(hOut, 0x0F, bInfo.dwSize.X, pos, &buffer);
    if (piece == Piece::Black)
        FillConsoleOutputAttribute(hOut, 0xF0, 15, pos, &buffer);
    else {
        pos.X = bInfo.dwSize.X - 15;
        FillConsoleOutputAttribute(hOut, 0xF0, 15, pos, &buffer);
    }
}

bool UI::generateMove(Chessboard board, Piece piece, Move& move) {
    int dx[8] = {-1, -1, -1, 0, 0, 1, 1, 1};
    int dy[8] = {-1, 0, 1, -1, 1, -1, 0, 1};
    Chessboard backup = board;
    bool avail[8][8];
    auto isAmazon = [&board, &piece, &dx, &dy](int x, int y) -> bool {
        return Chessboard::isInside(x, y) && board.at(x, y) == (Square)piece && [&](int x, int y) {
            for (int k = 0; k < 8; k++) {
                int x1 = x + dx[k];
                int y1 = y + dy[k];
                if (Chessboard::isInside(x1, y1) && board.at(x1, y1) == Square::Empty) return true;
            }
            return false;
        }(x, y);
    };
    auto isStep = [&avail](int x, int y) -> bool {
        return Chessboard::isInside(x, y) && avail[x][y];
    };
    auto chooseTarget = [&](int& x, int& y, std::function<bool(int, int)> judge,
                            Color recall) mutable -> bool {
        setPosColor(Color::Black, Color::Brown, x, y);
        int keycode;
        while (keycode = _getch(), keycode != 13) {
            if (keycode == 0 || keycode == 0xE0) {
                keycode = _getch();
                // 若是方向键或功能键，键值加 300 以防冲突
                keycode += 300;
            }
            setPosColor(Color::Black, recall, x, y);
            switch (keycode) {
                case 27: {  // Esc
                    // 恢复初始盘面
                    printGame(backup);
                    board = backup;
                    return false;
                }
                case '8':
                case 'W':
                case 'w':  // up
                case 372: {
                    do {
                        if (y == 0) {
                            if (x == 7)
                                x = 0;
                            else
                                x++;
                            y = 7;
                        } else
                            y--;
                    } while (!judge(x, y));
                    break;
                }
                case '2':
                case 'S':
                case 's':  // down
                case 380: {
                    do {
                        if (y == 7) {
                            if (x == 7)
                                x = 0;
                            else
                                x++;
                            y = 0;
                        } else
                            y++;
                    } while (!judge(x, y));
                    break;
                }
                case '4':
                case 'A':
                case 'a':  // left
                case 375: {
                    do {
                        if (x == 0) {
                            if (y == 7)
                                y = 0;
                            else
                                y++;
                            x = 7;
                        } else
                            x--;
                    } while (!judge(x, y));
                    break;
                }
                case '6':
                case 'D':
                case 'd':  // right
                case 377: {
                    do {
                        if (x == 7) {
                            if (y == 7)
                                y = 0;
                            else
                                y++;
                            x = 0;
                        } else
                            x++;
                    } while (!judge(x, y));
                    break;
                }
                case '7':
                case 'Q':  // LU
                case 'q': {
                    if (judge(x - 1, y - 1)) x--, y--;
                    break;
                }
                case '9':
                case 'E':  // RU
                case 'e': {
                    if (judge(x + 1, y - 1)) x++, y--;
                    break;
                }
                case '1':
                case 'Z':  // LD
                case 'z': {
                    if (judge(x - 1, y + 1)) x--, y++;
                    break;
                }
                case '3':
                case 'C':  // RD
                case 'c': {
                    if (judge(x + 1, y + 1)) x++, y++;
                    break;
                }
            }
            setPosColor(Color::Black, Color::Brown, x, y);
        }
        return true;
    };
    int x, y;
    bool firstFound;
    do {      // 选择 Arrows 位置的循环（若取消退回）
        do {  // 选择落子位置的循环（若取消退回）

            firstFound = false;
            printMoveHelp(1);
            // 获取第一个棋子的位置
            for (y = 0; !firstFound && y < 8; y++) {
                for (x = 0; !firstFound && x < 8; x++) {
                    if (isAmazon(x, y)) {
                        firstFound = true;
                        move.x0 = x;
                        move.y0 = y;
                    }
                }
            }
            if (!chooseTarget(move.x0, move.y0, isAmazon, Color::White)) {
                return false;
            }
            printGame(board);

            firstFound = false;
            printMoveHelp(2);
            std::memset(avail, false, sizeof(avail));
            for (int i = 0; i < 8; i++) {
                for (int len = 1; len < 8; len++) {
                    move.x1 = move.x0 + len * dx[i];
                    move.y1 = move.y0 + len * dy[i];
                    if (board.at(move.x1, move.y1) != Square::Empty ||
                        !Chessboard::isInside(move.x1, move.y1))
                        break;
                    avail[move.x1][move.y1] = true;
                    if (!firstFound) {
                        firstFound = true;
                        x = move.x1;
                        y = move.y1;
                    }
                    setPosColor(Color::Black, Color::LightCyan, move.x1, move.y1);
                }
            }
            move.x1 = x;
            move.y1 = y;
        } while (!chooseTarget(move.x1, move.y1, isStep, Color::LightCyan));
        board.at(move.x0, move.y0) = Square::Empty;
        board.at(move.x1, move.y1) = Square(piece);
        printGame(board);

        firstFound = false;
        printMoveHelp(3);
        std::memset(avail, false, sizeof(avail));
        for (int i = 0; i < 8; i++) {
            for (int len = 1; len < 8; len++) {
                move.x2 = move.x1 + len * dx[i];
                move.y2 = move.y1 + len * dy[i];
                if (board.at(move.x2, move.y2) != Square::Empty ||
                    !Chessboard::isInside(move.x2, move.y2))
                    break;
                avail[move.x2][move.y2] = true;
                if (!firstFound) {
                    firstFound = true;
                    x = move.x2;
                    y = move.y2;
                }
                setPosColor(Color::Black, Color::LightCyan, move.x2, move.y2);
            }
        }
        move.x2 = x;
        move.y2 = y;
    } while (!chooseTarget(move.x2, move.y2, isStep, Color::LightCyan));

    // std::cout<<move.x0<<move.y0<<move.x1<<move.y1<<move.x2<<move.y2;
    // system("pause");
    return true;
}

void UI::setCursorPos(short x, short y) {
    COORD pos = {x, y};
    SetConsoleCursorPosition(hOut, pos);
}

void UI::setTextColor(Color foreground, Color background) {
    WORD wColor = short(foreground) + short(background) * 16;
    SetConsoleTextAttribute(hOut, wColor);
}

void UI::setPosColor(Color foreground, Color background, short x, short y) {
    DWORD buffer;
    FillConsoleOutputAttribute(hOut, (int)background << 4 + (int)foreground, 3, getCOORD(x, y),
                               &buffer);
}

COORD UI::getCOORD(short x, short y) {
    COORD r;
    r.X = center_x - 17 + x * 4 + 1;
    r.Y = 2 + y * 2 + 1;
    return r;
}