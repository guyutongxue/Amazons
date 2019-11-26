/**
 * Copyright (c) 2019 Guyutongxue
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

//*****************************//
//      Amazons kksk_R by      //
//    Mt_Nomad & 12f23eddde    //
//        Version 1917+        //
//*****************************//

#include "bot.h"

void Bot::getPos(Chessboard map, Player pl, Coordinate pos[4]) {
    int counter = 0;
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (int(map.at(i, j)) == int(pl)) {
                pos[counter].x = i;
                pos[counter].y = j;
                counter++;
            }
        }
        if (counter == 4) break;
    }
}

void Bot::kingMove(Player pl, const Chessboard& map, int kd[8][8]) {
    std::queue<Coordinate> que;
    // 所有的位置都初始化为无穷大（不可到达）
    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++) kd[i][j] = INF;
    Coordinate pos[4];
    getPos(map, pl, pos);
    // 对每一个棋子进行广度优先搜索
    for (int i = 0; i < 4; i++) {
        // 起点入列
        que.push(Coordinate(pos[i].x, pos[i].y));
        kd[pos[i].x][pos[i].y] = 0;
        while (!que.empty()) {
            // 首元素出列
            Coordinate p = que.front();
            que.pop();
            // 8个方向
            for (int j = 0; j < 8; j++) {
                int nx = p.x + dx[j], ny = p.y + dy[j];
                // 判断是否为空以及是否需要更新（比原先的要小）
                if (inMap(nx, ny) && map.at(nx, ny) == Square::Empty &&
                    kd[nx][ny] > kd[p.x][p.y] + 1) {
                    // 入列并更新值
                    que.push(Coordinate(nx, ny));
                    kd[nx][ny] = kd[p.x][p.y] + 1;
                }
            }
        }
    }
}
//对某一个棋盘，计算某一方棋子的Kingmove数
void Bot::queenMove(Player pl, const Chessboard& map, int qd[8][8]) {
    std::queue<Coordinate> que;
    // 所有的位置都初始化为无穷大（不可到达）
    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++) qd[i][j] = INF;
    Coordinate pos[4];
    getPos(map, pl, pos);

    // 对每一个棋子进行广度优先搜索
    for (int i = 0; i < 4; i++) {
        // 起点入列
        que.push(Coordinate(pos[i].x, pos[i].y));
        qd[pos[i].x][pos[i].y] = 0;
        while (!que.empty()) {
            // 弹出首元素
            Coordinate p = que.front();
            que.pop();
            // 8个方向
            for (int j = 0; j < 8; j++) {
                // 移动长度
                for (int step = 1; step < 8; step++) {
                    int nx = p.x + dx[j] * step, ny = p.y + dy[j] * step;
                    // 判断是否为空以及是否需要更新（比原先的要小）
                    if (inMap(nx, ny) && map.at(nx, ny) == Square::Empty &&
                        qd[nx][ny] > qd[p.x][p.y] + 1) {
                        // 入列并更新值
                        que.push(Coordinate(nx, ny));
                        qd[nx][ny] = qd[p.x][p.y] + 1;
                    } else
                        break;
                }
            }
        }
    }
}

double Bot::evaluation(Player pl, const Chessboard& map, int currturns) {
    search_count++;
    int k_w[8][8], k_b[8][8], q_w[8][8], q_b[8][8];
    kingMove(Player::Black, map, k_b);
    kingMove(Player::White, map, k_w);
    queenMove(Player::Black, map, q_b);
    queenMove(Player::White, map, q_w);

    // 以白方为己方
    // t1/2:  'territory'
    // p1/2:  'position'
    double p1 = 0, p2 = 0, t1 = 0, t2 = 0;
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            // QueenMove
            if (q_b[i][j] == INF && q_b[i][j] == q_w[i][j])
                t1 += 0;
            else if (q_b[i][j] != INF && q_b[i][j] == q_w[i][j])
                t1 += firstHandAdvantage;
            else if (q_w[i][j] < q_b[i][j])
                t1 += 1;
            else
                t1 += -1;

            // KingMove
            if (k_b[i][j] == INF && k_b[i][j] == k_w[i][j])
                t2 += 0;
            else if (k_b[i][j] != INF && k_b[i][j] == k_w[i][j])
                t2 += firstHandAdvantage;
            else if (k_w[i][j] < k_b[i][j])
                t2 += 1;
            else
                t2 += -1;

            p1 += 2.0 / pow2(q_w[i][j]) - 2.0 / pow2(q_b[i][j]);
            p2 += std::min(1.0, std::max(-1.0, (1.0 / 6.0) * (k_b[i][j] - k_w[i][j])));
        }
    }

    // 计算空格的 ’mobility‘
    int emptyblocks[8][8];
    memset(emptyblocks, 0, sizeof(emptyblocks));
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (map.at(i, j) == Square::Empty) {
                // 记录八个方向上的相邻空格
                for (int m = 0; m < 8; m++) {
                    if (inMap(i + dx[m], j + dy[m]) &&
                        map.at(i + dx[m], j + dy[m]) == Square::Empty)
                        emptyblocks[i][j]++;
                }
            }
        }
    }

    Coordinate pos_black[4], pos_white[4];
    getPos(map, Player::White, pos_white);
    getPos(map, Player::Black, pos_black);
    // 计算棋子的 'mobility'
    double m_w = 0, m_b = 0;
    // 4个白方棋子
    for (int j = 0; j < 4; j++) {
        // 8个方向
        for (int i = 0; i < 8; i++) {
            // 以 QueenMove 方式移动
            for (int step = 1; step < 8; step++) {
                // 当前移动到的位置 (nx,ny)
                int nx = pos_white[j].x + dx[i] * step, ny = pos_white[j].y + dy[i] * step;
                // 遇到障碍停止
                if (inMap(nx, ny) && map.at(nx, ny) == Square::Empty && q_w[nx][ny] != INF) {
                    m_w += (double)emptyblocks[nx][ny] / (double)step;
                } else
                    break;
            }
        }
    }
    // 4个黑方棋子同理
    for (int j = 0; j < 4; j++) {
        for (int i = 0; i < 8; i++) {
            for (int step = 1; step < 8; step++) {
                int nx = pos_black[j].x + dx[i] * step, ny = pos_black[j].y + dy[i] * step;
                if (inMap(nx, ny) && map.at(nx, ny) == Square::Empty && q_b[nx][ny] != INF) {
                    m_b += (double)emptyblocks[nx][ny] / (double)step;
                } else
                    break;
            }
        }
    }
    double value = 0;

    // 计算白方为己方的优势程度
    if (turns + currturns < 22) {
        value = f1[turns + currturns] * t1 + f2[turns + currturns] * t2 +
                f3[turns + currturns] * p1 + f4[turns + currturns] * p2 +
                f5[turns + currturns] * (m_w - m_b);
    } else {
        value = f1[22] * t1 + f2[22] * t2 + f3[22] * p1 + f4[22] * p2 + f5[22] * (m_w - m_b);
    }

    // 若黑方为己方返回相反数
    if (pl == Player::Black)
        return -value;
    else
        return value;
}

double Bot::PVS(Player pl, Chessboard map, double alpha, double beta, int depth, int d) {
    if (depth == 0 || 1000 * (clock()-start_time) / CLOCKS_PER_SEC >= max_time) {
        return evaluation(pl, map, (d - depth) / 2);
    }
    Move moves[1500];
    memset(moves, 0, sizeof(moves));
    int pos = 0;
    double value = 0;
    Coordinate pos_color[4];
    getPos(map, pl, pos_color);

    pos = 0;
    // 生成子节点
    for (int i = 0; i < 4; i++) {
        int k, l, len1, len2, x1, x2, y1, y2;
        for (k = 0; k < 8; k++) {
            for (len1 = 1; len1 < 8; len1++) {
                x1 = pos_color[i].x + dx[k] * len1;
                y1 = pos_color[i].y + dy[k] * len1;
                if (map.at(x1, y1) != Square::Empty || !inMap(x1, y1)) break;
                for (l = 0; l < 8; l++) {
                    for (len2 = 1; len2 < 8; len2++) {
                        x2 = x1 + dx[l] * len2;
                        y2 = y1 + dy[l] * len2;
                        if (!inMap(x2, y2)) break;
                        if (map.at(x1, y1) != Square::Empty &&
                            !(pos_color[i].x == x2 && pos_color[i].y == y2))
                            break;
                        moves[pos].x0 = pos_color[i].x;
                        moves[pos].y0 = pos_color[i].y;
                        moves[pos].x1 = x1;
                        moves[pos].y1 = y1;
                        moves[pos].x2 = x2;
                        moves[pos++].y2 = y2;
                    }
                }
            }
        }
    }
    // 若无路可走，游戏结束
    if (pos == 0) {
        return evaluation(pl, map, (d - depth) / 2);
    }

    // 搜索第一步，并假设它是最优秀的走法
    double best;
    makeMove(moves[0], pl, map);
    best = -PVS((Player) - int(pl), map, -beta, -alpha, depth - 1, d);
    unmakeMove(moves[0], pl, map);
    if (best > alpha) alpha = best;

    for (int i = 1; i < pos; i++) {
        makeMove(moves[i], pl, map);
        value = -PVS((Player) - int(pl), map, -best - PVS_width, -best, depth - 1, d);

        // 假设验证失败
        if (value >= best + 1 && value < beta)
            //以value作为下界再次搜索
            best = -PVS((Player) - int(pl), map, -beta, -value, depth - 1, d);

        // 假设验证成功
        else if (value > best)
            best = value;
        unmakeMove(moves[i], pl, map);

        // 更新 alpha
        if (best > alpha) alpha = best;
        // 剪枝
        if (alpha >= beta) break;
    }
    return best;
}

//单独把第一层的搜索做成一个函数 为了预先排序
//因为第二层大概率搜不完 并且这一层不是返回估值而是走法
Move Bot::searchStep(Player pl, Chessboard board) {
    // Get a copy of original grid
    Chessboard temp_grid = board;

    // Initialize moves' array
    Move moves[1232];
    memset(moves, 0, sizeof(moves));
    int i, j;
    int pos = 0;
    double value = 0;

    Coordinate pos_color[4];

    getPos(board, pl, pos_color);
    // 4枚棋子
    for (int i = 0; i < 4; i++) {
        int x1, x2;  // 棋子落点
        int y1, y2;  // Arrow 落点
        // 8个方向
        for (int k = 0; k < 8; k++) {
            // 不同的移动长度
            for (int len1 = 1; len1 < 8; len1++) {
                x1 = pos_color[i].x + dx[k] * len1;
                y1 = pos_color[i].y + dy[k] * len1;

                // 遇到障碍便结束
                if (board.at(x1, y1) != Square::Empty || !inMap(x1, y1)) break;

                // 发射 Arrow
                for (int l = 0; l < 8; l++) {
                    // 发射长度
                    for (int len2 = 1; len2 < 8; len2++) {
                        x2 = x1 + dx[l] * len2;
                        y2 = y1 + dy[l] * len2;

                        // 遇到障碍（注意要忽略已经被移走的棋子）结束
                        if (!inMap(x2, y2)) break;
                        if (board.at(x2, y2) != Square::Empty &&
                            !(pos_color[i].x == x2 && pos_color[i].y == y2))
                            break;

                        // 生成走法
                        moves[pos].x0 = pos_color[i].x;
                        moves[pos].y0 = pos_color[i].y;
                        moves[pos].x1 = x1;
                        moves[pos].y1 = y1;
                        moves[pos].x2 = x2;
                        moves[pos].y2 = y2;

                        // 落子，然后计算估值，最后撤回
                        makeMove(moves[pos], pl, temp_grid);
                        value = evaluation(pl, temp_grid, 0);
                        unmakeMove(moves[pos], pl, temp_grid);
                        moves[pos++].value = value;
                    }
                }
            }
        }
    }

    // 预先排序，降序搜索，更易优先找到优势走法
    std::sort(moves, moves + pos, [](Move a, Move b) { return a > b; });
    int t = 0;
    // 尝试搜索10层
    for (int d = 1; d < 10; d++) {
        max_depth = d;
        for (t = 0; t < pos; t++) {
            if (1000 * (clock()-start_time) / CLOCKS_PER_SEC >= max_time) break;
            makeMove(moves[t], pl, temp_grid);
            value = -PVS(pl, temp_grid, -INF, INF, d, d);
            moves[t].value = value;
            unmakeMove(moves[t], pl, temp_grid);
        }
        std::sort(moves, moves + t, [](Move a, Move b) { return a > b; });
        if (1000 * (clock()-start_time) / CLOCKS_PER_SEC >= max_time) break;
    }
    return moves[0];
}

Bot::Bot(Player player) {
    PVS_width = 1.25;
    this->player=player;
}

Move Bot::execute(Chessboard board, int turns) {
    start_time=clock();
    this->turns = turns;
    if (turns == 1)
        max_time = 1960;
    else
        max_time = 960;
    return searchStep(player, board);
}
