// Copyright (c) 2019 Guyutongxue
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef BOT_H
#define BOT_H

#include <algorithm>
#include <cstring>
#include <ctime>
#include <queue>
#include "iplayer.h"

/**
 * @brief 计算机玩家类。
 * 继承 @c IPlayer 接口，并通过算法实现整体估值，搜索最优解。
 */
class Bot : public IPlayer {
public:
    /**
     * @brief 通过执棋方构造函数。
     * @param piece 计算机玩家的执棋方。
     */
    Bot(Piece piece);

    /**
     * @brief 默认析构函数。（空）
     */
    ~Bot();

    /**
     * @brief 执行计算机玩家的决策。
     * 实现 @c IPlayer 执行决策的纯虚函数。
     * @param board 当前棋盘状态。
     * @param turns 当前游戏回合数。
     * @param[out] move 计算机玩家决定的落子。
     * @return 是否成功决策。永远返回真。
     */
    bool execute(Chessboard board, int turns, Move& move) override;

private:
    // 参见定义。
    class Coordinate;

    /** 计算机玩家所持子颜色。 */
    Piece piece;

    /** 八个方向的 X 变化量。 */
    constexpr static const int dx[8] = {-1, -1, -1, 0, 0, 1, 1, 1};

    /** 八个方向的 Y 变化量。 */
    constexpr static const int dy[8] = {-1, 0, 1, -1, 1, -1, 0, 1};

    // 在不同局面进行程度下，各种估值的参数。
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

    /** 先手优势参数 k 。 */
    constexpr static const double firstHandAdvantage = 0.2;

    /** 无穷大。 */
    constexpr static const int INF = INT_MAX;

    /** 当前回合数。 */
    int turns;

    /** 存储决策开始时间。 */
    std::clock_t start_time;

    /** 设定计算机玩家最大搜索时间。 */
    int max_time;

    /** 设定 PVS 搜索的最小窗口宽度。 */
    int PVS_width;

    /**
     * @brief 获取某方四枚棋子的坐标。
     * 从 @c board 中读取 @c piece 方棋子的坐标存入 @c pos 中。
     * @param board 当前棋盘状态。
     * @param piece 需要获取坐标的执棋方。
     * @param[out] pos 棋子坐标。
     */
    void getPos(const Chessboard& board, Piece piece, Coordinate pos[4]);

    /**
     * @brief 寻找最优落子。
     * 在当前局面展开博弈树，并通过深入搜索找到最优落子。
     * @param piece 当前执子方。
     * @param board 当前棋盘状态。
     * @return 最优落子。
     */
    Move searchStep(Piece piece, Chessboard& board);

    /**
     * @brief 返回某一棋盘、某一执棋方的 QueenMove 数。
     * 将该方的 QueenMove 存入 qd 数组中。
     * @param piece 要计算的执棋方。
     * @param board 当前棋盘状态。
     * @param[out] qd 每一个点的 QueenMove 值。
     */
    void queenMove(Piece piece, const Chessboard& board, int qd[8][8]);

    /**
     * @brief 返回某一棋盘、某一执棋方的 KingMove 数。
     * 将该方的 KingMove 存入 qd 数组中。
     * @param piece 要计算的执棋方。
     * @param board 当前棋盘状态。
     * @param[out] qd 每一个点的 KingMove 值。
     */
    void kingMove(Piece piece, const Chessboard& board, int kd[8][8]);

    /**
     * @brief 返回当前棋盘对于某一执棋方的估值。
     * 综合考虑 'territory' 'position' 和 'mobility' 及棋局进行程度进行估值。
     * @param piece 要计算的执棋方。
     * @param board 当前棋盘状态。
     * @param turns 回合数，棋局进行程度。
     * @return 计算得的估值大小。
     */
    double evaluation(Piece piece, const Chessboard& board, const int turns);

    /**
     * @brief 使用 PVS 算法取得当前节点的估值。
     * @param piece 执棋玩家。
     * @param board 当前棋盘。
     * @param alpha 下界 alpha 。
     * @param beta 上界 beta 。
     * @param depth 剩余深度。
     * @param d 总深度。
     * @return 取得的估值大小。
     */
    double PVS(Piece piece, Chessboard& board, double alpha, double beta, int depth, int d);

    /**
     * @brief 计算 2 的 n 次方。
     * 用于计算 'position' 估值。
     * @param n 指数。
     * @return 结果。
     */
    inline int pow2(int n) {
        return n >= 31 ? INT_MAX : 1 << n;
    }

    /**
     * @brief 在某棋盘上执行某一落子。
     * @param a 要执行的落子信息。
     * @param piece 执棋玩家。
     * @param board 当前棋盘状态。
     */
    inline void makeMove(Move a, Piece piece, Chessboard& board) {
        board.at(a.x0, a.y0) = Square::Empty;
        board.at(a.x1, a.y1) = (Square)piece;
        board.at(a.x2, a.y2) = Square::Arrow;
    }

    /**
     * @brief 在某棋盘上撤销某一落子。
     * @param a 要撤销的落子信息。
     * @param piece 执棋玩家。
     * @param board 当前棋盘状态。
     */
    inline void unmakeMove(Move a, Piece piece, Chessboard& board) {
        board.at(a.x1, a.y1) = Square::Empty;
        board.at(a.x2, a.y2) = Square::Empty;
        // 注意必须最后回复原点状态，否则当 Arrow 发射到原点时将错误。
        board.at(a.x0, a.y0) = (Square)piece;
    }
};

/**
 * @brief 坐标类。
 */
class Bot::Coordinate {
public:
    int x; /**< X 坐标。 */
    int y; /**< Y 坐标。 */
    /**
     * @brief 默认构造函数。
     * 初始化为 (0,0) 。
     */
    Coordinate() {
        x = y = 0;
    }

    /**
     * @brief 基于数值的构造函数。
     * @param x X 坐标。
     * @param y Y 坐标。
     */
    Coordinate(int x, int y) {
        this->x = x;
        this->y = y;
    }
};

#endif  // BOT_H