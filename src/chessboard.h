// Copyright (c) 2019 Guyutongxue
// 
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef CHECKBOARD_H
#define CHECKBOARD_H

#include <cstring>
#include <iostream>

/**
 * @brief 棋子枚举类。
 * 表示棋盘上某一坐标的状态：
 * 空位、黑棋、白棋、 Arrow 。
 */
enum class Square {
    Empty = 0,  /**< 空位  */
    White = 1,  /**< 白棋  */
    Black = -1, /**< 黑棋  */
    Arrow = 2   /**< Arrow */
};

/**
 * @brief 执棋方枚举类。
 * 表示某一执棋方所执棋子颜色。
 */
enum class Piece {
    White = 1, /**< 白棋，白方 */
    Black = -1 /**< 黑棋，黑方 */
};

/**
 * @brief 落子类。
 * 表示某次落子的起点、终点和 Arrow 位置。
 * 对友元类 @c Bot 提供估值。
 */
class Move {
public:
    int x0; /**< 落子起点 X 坐标。 */
    int y0; /**< 落子起点 Y 坐标。 */
    int x1; /**< 落子终点 X 坐标。 */
    int y1; /**< 落子终点 Y 坐标。 */
    int x2; /**< Arrow X 坐标。    */
    int y2; /**< Arrow Y 坐标。    */
    friend class Bot;

    /**
     * @brief 比较落子之间的估值。
     * 用于计算机执棋时的算法。
     * @return 左侧操作数是否大于右侧操作数。
     */
    bool operator>(Move b) {
        return value > b.value;
    }

    /**
     * @brief 默认构造函数。
     * 初始化为 0 。
     */
    Move() {
        x0 = x1 = x2 = y0 = y1 = y2 = 0;
    }

private:
    double value;
};

/**
 * @brief 棋盘类。
 * 表示某一棋盘状态。
 */
class Chessboard {
public:
    /**
     * @brief 默认构造函数。
     * 将棋盘全部初始化为空位。
     */
    Chessboard();

    /**
     * @brief 拷贝构造函数。
     * 将棋盘各个位置拷贝初始化。
     */
    Chessboard(const Chessboard& origin);

    /**
     * @brief 从二维数组构造函数。
     * 将棋盘各个位置依照数组中元素的值初始化。
     */
    Chessboard(const int (&origin)[8][8]);

    /**
     * @brief 静态成员函数，返回开局棋盘状态。
     * @return 开局棋盘状态。
     */
    static const Chessboard getStart();

    /**
     * @brief 赋值运算符重载。
     * 将棋盘各个位置按照源的各个赋值。
     * @param src 源棋盘。
     * @return 当前棋盘的引用。
     */
    Chessboard& operator=(const Chessboard& src);

    /**
     * @brief 读写棋盘上某一坐标的状态。
     * @param x X 坐标。
     * @param y Y 坐标。
     * @return (x,y) 处的坐标状态的引用。
     */
    Square& at(int x, int y);

    /**
     * @brief 读取棋盘上某一坐标的状态。
     * 此函数重载不得写入。
     * @param x X 坐标。
     * @param y Y 坐标。
     * @return (x,y) 处的坐标状态。
     */
    const Square& at(int x, int y) const;

    /**
     * @brief 静态函数，判断某一棋子坐标是否合法。
     * @param x X 坐标。
     * @param y Y 坐标。
     * @return 该坐标是否合法。
    */
    inline static bool isInside(int x, int y) {
        return x >= 0 && x < 8 && y >= 0 && y < 8;
    }

    friend class Bot;

private:

    /** 保存棋盘各个位置的状态的数组。 */
    Square board[8][8];
};

#endif  // CHECHKBOARD_H