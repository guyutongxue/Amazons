/**
 * Copyright (c) 2019 Guyutongxue
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef BOT_H
#define BOT_H

#include <algorithm>
#include <cstring>
#include <ctime>
#include <queue>
#include "chessboard.h"

class Bot {
public:
    Bot(Player player);
    Move execute(Chessboard board, int turns);
    Player player;

private:
    class Coordinate;

    // DeltaX/Y of eight direction
    constexpr static const int dx[8] = {-1, -1, -1, 0, 0, 1, 1, 1};
    constexpr static const int dy[8] = {-1, 0, 1, -1, 1, -1, 0, 1};

    
    // The parameter of calculating 'value' in formula
    // 调整参数 res=f1*t1+f2*t2+f3*c1+f4*c2+f5*m
    constexpr static const double f1[23] = {0.1080, 0.1080, 0.1235, 0.1332, 0.1400, 0.1468,
                                            0.1565, 0.1720, 0.1949, 0.2217, 0.2476, 0.2680,
                                            0.2800, 0.2884, 0.3000, 0.3208, 0.3535, 0.4000,
                                            0.4613, 0.5350, 0.6181, 0.7075, 0.8000};
    constexpr static const double f2[23] = {0.3940, 0.3940, 0.3826, 0.3753, 0.3700, 0.3647,
                                            0.3574, 0.3460, 0.3294, 0.3098, 0.2903, 0.2740,
                                            0.2631, 0.2559, 0.2500, 0.2430, 0.2334, 0.2200,
                                            0.2020, 0.1800, 0.1550, 0.1280, 0.1000};
    constexpr static const double f3[23] = {0.1160, 0.1160, 0.1224, 0.1267, 0.1300, 0.1333,
                                            0.1376, 0.1440, 0.1531, 0.1640, 0.1754, 0.1860,
                                            0.1944, 0.1995, 0.2000, 0.1950, 0.1849, 0.1700,
                                            0.1510, 0.1287, 0.1038, 0.0773, 0.0500};
    constexpr static const double f4[23] = {0.1160, 0.1160, 0.1224, 0.1267, 0.1300, 0.1333,
                                            0.1376, 0.1440, 0.1531, 0.1640, 0.1754, 0.1860,
                                            0.1944, 0.1995, 0.2000, 0.1950, 0.1849, 0.1700,
                                            0.1510, 0.1287, 0.1038, 0.0773, 0.0500};
    constexpr static const double f5[23] = {0.2300, 0.2300, 0.2159, 0.2067, 0.2000, 0.1933,
                                            0.1841, 0.1700, 0.1496, 0.1254, 0.1010, 0.0800,
                                            0.0652, 0.0557, 0.0500, 0.0464, 0.0436, 0.0400,
                                            0.0346, 0.0274, 0.0190, 0.0097, 0};
    constexpr static const double firstHandAdvantage =
        0.2;  //先手优势（计算 'territory' 时用到的参数 k ）
    // typedef std::pair<int, int> PAIR;
    constexpr static const int INF = INT16_MAX;
    void getPos(Chessboard map, Player pl, Coordinate pos[4]);
    Move searchStep(Player pl, Chessboard board);
    //对某一个棋盘，计算某一方棋子的queenmove数
    void queenMove(Player pl, const Chessboard& map, int qd[8][8]);
    //对某一个棋盘，计算某一方棋子的Kingmove数
    void kingMove(Player pl, const Chessboard& map, int kd[8][8]);

    double evaluation(Player pl, const Chessboard& board, const int turns);
    /**
     * @brief Use PVS algorithm to calculate value?
     * @param color The color of placing Queen
     * @param map   The grid
     * @param alpha The minimum
     * @param beta  The maximum
     * @param depth The depth remainder
     * @param d     ?
     * @return The value.
     */
    double PVS(Player pl, Chessboard map, double alpha, double beta, int depth, int d);

    int turns;
    int max_depth;
    clock_t start_time;
    int search_count;
    int max_time;
    int PVS_width;

    // 判断是否在棋盘内
    inline bool inMap(int x, int y) {
        return x >= 0 && x < 8 && y >= 0 && y < 8;
    }
    //求2的整数次方
    inline int pow2(int n) {
        return n >= 31 ? INT_MAX : 1 << n;
    }
    // 在temp_grid中落子
    inline void makeMove(Move a, Player pl, Chessboard board) {
        board.at(a.x0, a.y0) = Square::Empty;
        board.at(a.x1, a.y1) = (Square)pl;
        board.at(a.x2, a.y2) = Square::Arrow;
    }

    //恢复到上一步
    inline void unmakeMove(Move a, Player pl, Chessboard board) {
        board.at(a.x0, a.y0) = Square::Empty;
        board.at(a.x1, a.y1) = Square::Empty;
        board.at(a.x2, a.y2) = (Square)pl;
    }
};

class Bot::Coordinate {
public:
    int x;
    int y;
    Coordinate() {}
    Coordinate(int x, int y) {
        this->x = x;
        this->y = y;
    }
};

#endif  // BOT_H