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

//获取color棋子坐标
void Bot::getPos(Chessboard map, Player pl, int pos[4][2]) {
    int counter = 0;
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (int(map.at(i, j)) == int(pl)) {
                pos[counter][0] = i;
                pos[counter][1] = j;
                counter++;
            }
        }
        if (counter == 4) break;
    }
}
//对某一个棋盘，计算某一方棋子的Kingmove数
void Bot::kmove(Player pl, Chessboard map, int kd[8][8]) {
    queue<P> que;
    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++) kd[i][j] = INF;  //所有的位置都初始化为INF
    int pos[4][2];
    getPos(map, pl, pos);
    for (int i = 0; i < 4; i++) {
        que.push(P(pos[i][0], pos[i][1]));  //将起点加入队列中
        // cout << P(pos[i][0],pos[i][1]).first << ' ' <<
        // P(pos[i][0],pos[i][1]).second << endl; //debug
        kd[pos[i][0]][pos[i][1]] = 0;  //并把这一地点的距离设置为0
        while (!que.empty())           //队列不空
        {
            P p = que.front();           // 从队列的最前端取出元素
            que.pop();                   //取出后从队列中删除该元素
            for (int j = 0; j < 8; j++)  //向8个方向
            {
                int nx = p.first + dx[j],
                    ny = p.second + dy[j];  //移动后的位置标记为(nx,ny)
                //判断是否可以移动以及是否比kd中的距离小）
                if (inMap(nx, ny) && map.at(nx, ny) == Square::Empty &&
                    kd[nx][ny] > kd[p.first][p.second] + 1) {
                    que.push(P(nx, ny));  //可以移动，添加到队列
                    //到该位置的距离为到p的距离+1
                    kd[nx][ny] = kd[p.first][p.second] + 1;
                }
            }
        }
    }
}
//对某一个棋盘，计算某一方棋子的Kingmove数
void Bot::qmove(Player pl, Chessboard map, int qd[8][8]) {
    queue<P> que;
    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++) qd[i][j] = INF;  //所有的位置都初始化为INF
    int pos[4][2];
    getPos(map, pl, pos);

    for (int i = 0; i < 4; i++) {
        que.push(P(pos[i][0], pos[i][1]));  //将起点加入队列中
        // cout << P(pos[i][0],pos[i][1]).first << ' ' <<
        // P(pos[i][0],pos[i][1]).second << endl;
        qd[pos[i][0]][pos[i][1]] = 0;  //并把这一地点的距离设置为0
        while (!que.empty())           //队列不空
        {
            P p = que.front();  // 从队列的最前端取出元素
            que.pop();          //取出后从队列中删除该元素

            for (int j = 0; j < 8; j++)  //向8个方向
            {
                for (int step = 1; step < 8; step++) {  //向某方向走的步数
                    int nx = p.first + dx[j] * step,
                        ny = p.second + dy[j] * step;  //移动后的位置标记为(nx,ny)
                    // cout<<"("<<sx<<","<<sy<<") → ("<<nx<<"," <<ny<<")
                    // t="<<temp_grid[nx][ny]<<" k="<< kd[nx][ny]<<"
                    // S="<<step<<endl;
                    if (inMap(nx, ny) && map.at(nx, ny) == Square::Empty &&
                        qd[nx][ny] > qd[p.first][p.second] + 1) {
                        que.push(P(nx, ny));                     //可以移动，添加到队列
                        qd[nx][ny] = qd[p.first][p.second] + 1;  //到该位置的步数为到p的步数+1
                    } else
                        break;  //发现障碍 跳出
                }
            }
        }
    }
}

//返回Territory值（当前颜色-对方颜色）
//默认为白-黑 黑的话加一负号
double Bot::evaluation(Player pl, Chessboard map, int currturns) {
    search_count++;
    int k_w[8][8], k_b[8][8], q_w[8][8], q_b[8][8];
    kmove(Player::Black, map, k_w), kmove(Player::White, map, k_b);
    qmove(Player::Black, map, q_w), qmove(Player::White, map, q_b);

    //计算position(c1,c2)
    // t1/2: t_1/2 'territory'
    // c1/2: c_1/2 'position'
    double c1 = 0, c2 = 0, t1 = 0, t2 = 0;
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            // QueenMove
            if (q_b[i][j] == INF && q_b[i][j] == q_w[i][j])
                t1 += 0;
            else if (q_b[i][j] != INF && q_b[i][j] == q_w[i][j])
                t1 += -firstHandAdvantage;
            else if (q_w[i][j] < q_b[i][j])
                t1 += 1;
            else
                t1 += -1;

            // KingMove
            if (k_b[i][j] == INF && k_b[i][j] == k_w[i][j])
                t2 += 0;
            else if (k_b[i][j] != INF && k_b[i][j] == k_w[i][j])
                t2 += -firstHandAdvantage;
            else if (k_w[i][j] < k_b[i][j])
                t2 += 1;
            else
                t2 += -1;

            c1 += 2.0 / pow2(q_w[i][j]) - 2.0 / pow2(q_b[i][j]);
            c2 += min(1.0, max(-1.0, (1.0 / 6.0) * (k_b[i][j] - k_w[i][j])));
        }
    }
    int emptyblocks[8][8];
    memset(emptyblocks, 0, sizeof(emptyblocks));
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (map.at(i, j) == Square::Empty) {
                for (int m = 0; m < 8; m++) {  //八个方向
                    if (inMap(i + dx[m], j + dy[m]) &&
                        map.at(i + dx[m], j + dy[m]) == Square::Empty)
                        emptyblocks[i][j]++;
                }
            }
        }
    }
    int pos_black[4][2], pos_white[4][2];
    getPos(map, Player::White, pos_black), getPos(map, Player::Black, pos_white);
    //计算mobility_white,mobility_black
    double m_w = 0, m_b = 0;
    for (int j = 0; j < 4; j++) {      //第j个点
        for (int i = 0; i < 8; i++) {  // 向8个方向
            for (int step = 1; step < 8; step++) {
                int nx = pos_white[j][0] + dx[i] * step,
                    ny = pos_white[j][1] + dy[i] * step;  //移动后的位置标记为(nx,ny)
                // cout << "posx="<<pos_white[ j ][ 0 ]<<"posy=" << pos_white[ j
                // ][ 1 ] << "nx=" << nx << "ny=" << ny << endl;
                if (inMap(nx, ny) && map.at(nx, ny) == Square::Empty &&
                    q_w[nx][ny] != INF) {  //判断是否可以移动
                    m_w += (double)emptyblocks[nx][ny] / (double)step;
                } else
                    break;  //此方向不能移动
            }
        }
    }
    for (int j = 0; j < 4; j++) {      //第j个点
        for (int i = 0; i < 8; i++) {  // 向8个方向
            for (int step = 1; step < 8; step++) {
                int nx = pos_black[j][0] + dx[i] * step,
                    ny = pos_black[j][1] + dy[i] * step;  //移动后的位置标记为(nx,ny)
                // cout << "posx="<<pos_white[ j ][ 0 ]<<"posy=" << pos_white[ j
                // ][ 1 ] << "nx=" << nx << "ny=" << ny << endl;
                if (inMap(nx, ny) && map.at(nx, ny) == Square::Empty &&
                    q_b[nx][ny] != INF) {  //判断是否可以移动
                    m_b += (double)emptyblocks[nx][ny] / (double)step;
                } else
                    break;
            }
        }
    }
    double value = 0;
    if (turns + currturns < 22) {
        value = f1[turns + currturns] * t1 + f2[turns + currturns] * t2 +
                f3[turns + currturns] * c1 + f4[turns + currturns] * c2 +
                f5[turns + currturns] * (m_w - m_b);
    } else {
        value = f1[22] * t1 + f2[22] * t2 + f3[22] * c1 + f4[22] * c2 + f5[22] * (m_w - m_b);
    }
    if (pl == Player::Black)
        return -value;
    else
        return value;
}

// alpha为当前层估值下界 beta为上界 depth为剩余深度
// color为当前按执子方

double Bot::PVS(Player pl, Chessboard map, double alpha, double beta, int depth, int d) {
    if (depth == 0 || 1000 * clock() / CLOCKS_PER_SEC >= max_time) {
        return evaluation(pl, map, (d - depth) / 2);
    }
    Move moves[1500];
    memset(moves, 0, sizeof(moves));
    int pos = 0, pos_rival = 0;
    double value = 0;
    int pos_color[8][2];

    // getpos
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (int(map.at(i, j)) == int(pl)) {
                pos_color[pos][0] = i;
                pos_color[pos][1] = j;
                pos = pos + 1;
            }
            if (int(map.at(i, j)) == -int(pl)) {
                pos_color[pos_rival + 4][0] = i;
                pos_color[pos_rival + 4][1] = j;
                pos_rival = pos_rival + 1;
            }
        }
        if (pos == 4 && pos_rival == 4) break;
    }
    pos = 0;  // cout << "OK" << endl;
    for (int i = 0; i < 4; i++) {
        int k, l, len1, len2, x1, x2, y1, y2;
        for (k = 0; k < 8; k++) {
            for (len1 = 1; len1 < 8; len1++) {
                x1 = pos_color[i][0] + dx[k] * len1;
                y1 = pos_color[i][1] + dy[k] * len1;
                if (map.at(x1, y1) != Square::Empty || !inMap(x1, y1)) break;
                for (l = 0; l < 8; l++) {
                    for (len2 = 1; len2 < 8; len2++) {
                        x2 = x1 + dx[l] * len2;
                        y2 = y1 + dy[l] * len2;
                        if (!inMap(x2, y2)) break;
                        if (map.at(x1, y1) != Square::Empty &&
                            !(pos_color[i][0] == x2 && pos_color[i][1] == y2))
                            break;
                        moves[pos].x0 = pos_color[i][0];
                        moves[pos].y0 = pos_color[i][1];
                        moves[pos].x1 = x1;
                        moves[pos].y1 = y1;
                        moves[pos].x2 = x2;
                        moves[pos++].y2 = y2;
                    }
                }
            }
        }
    }  //以上为步法生成部分 同时利用pos还可以做游戏是否结束的判断
    if (pos == 0) {
        return evaluation(pl, map, (d - depth) / 2);
    }  //边界条件

    // pvs全窗口搜索
    double best;
    procStep(moves[0], pl, map);
    best = -PVS((Player) - int(pl), map, -beta, -alpha, depth - 1, d);
    stepBack(moves[0], pl, map);
    if (best > alpha) alpha = best;

    for (int i = 1; i < pos; i++) {
        procStep(moves[i], pl, map);
        value = -PVS((Player) - int(pl), map, -best - PVS_width, -best, depth - 1, d);

        if (value >= best + 1 && value < beta)  //出现了窗口以外的最大值，这时要更新best
            best = -PVS((Player) - int(pl), map, -beta, -value, depth - 1,
                        d);  //以value作为下界再次搜索

        else if (value > best)  //窗口以内找到
            best = value;
        stepBack(moves[i], pl, map);

        //更新alpha以及剪枝
        // cout << best << endl;
        if (best > alpha) alpha = best;
        if (alpha >= beta) break;  //下界大于等于上界 剪枝
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
    int pos = 0, pos_rival = 0;
    double value = 0;

    // The first dimension is the no of queen. The second is x or y.
    // 0-3 is this side while 4-7 the other.
    int pos_color[8][2];

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (int(board.at(i, j)) == int(pl)) {
                pos_color[pos][0] = i;
                pos_color[pos][1] = j;
                pos = pos + 1;
            }
            if (int(board.at(i, j)) == -int(pl)) {
                pos_color[pos_rival + 4][0] = i;
                pos_color[pos_rival + 4][1] = j;
                pos_rival = pos_rival + 1;
            }
        }
        if (pos == 4 && pos_rival == 4) break;
    }
    pos = 0;
    // 4 queens
    for (int i = 0; i < 4; i++) {
        int k, l, len1, len2, x1, x2, y1, y2;
        // 8 direction of moving
        for (k = 0; k < 8; k++) {
            // <8 length of moving
            for (len1 = 1; len1 < 8; len1++) {
                // Get new position (x1,y1)
                x1 = pos_color[i][0] + dx[k] * len1;
                y1 = pos_color[i][1] + dy[k] * len1;

                // If Arrows or Queens ocuupy the position OR out of range, stop
                // 'extending'
                if (board.at(x1, y1) != Square::Empty || !inMap(x1, y1)) break;

                // 8 direction of shooting
                for (l = 0; l < 8; l++) {
                    // <8 length of shooting
                    for (len2 = 1; len2 < 8; len2++) {
                        // Get the position (x2,y2) of the Arrow
                        x2 = x1 + dx[l] * len2;
                        y2 = y1 + dy[l] * len2;

                        // If Queens ( Except current Queen which just moved to
                        // (x1,y1) ) ocuupied OR out of range, stop 'extending'
                        if (!inMap(x2, y2)) break;
                        if (board.at(x2, y2) != Square::Empty &&
                            !(pos_color[i][0] == x2 && pos_color[i][1] == y2))
                            break;

                        // Generating a new move
                        // cout << value << endl;
                        moves[pos].x0 = pos_color[i][0];
                        moves[pos].y0 = pos_color[i][1];
                        moves[pos].x1 = x1;
                        moves[pos].y1 = y1;
                        moves[pos].x2 = x2;
                        moves[pos].y2 = y2;

                        // Emulate the steps and calculate the value
                        procStep(moves[pos], pl, temp_grid);
                        value = evaluation(pl, temp_grid, 0);
                        stepBack(moves[pos], pl, temp_grid);
                        moves[pos++].value = value;
                        // cout << i << ' ' << value << endl;
                    }
                }
            }
        }
    }

    // Sort to a decrease form
    sort(moves, moves + pos);
    //预先排序
    int t = 0;
    //我知道我必定搜不了10层 但梦想总是要有的
    for (int d = 1; d < 10; d++) {
        max_depth = d;
        for (t = 0; t < pos; t++) {
            if (1000 * clock() / CLOCKS_PER_SEC >= max_time) break;
            procStep(moves[t], pl, temp_grid);
            value = -PVS(pl, temp_grid, -INF, INF, d, d);
            moves[t].value = value;
            stepBack(moves[t], pl, temp_grid);
        }
        sort(moves, moves + t);
        if (1000 * clock() / CLOCKS_PER_SEC >= max_time) break;
    }
    return moves[0];
}

Bot::Bot(){
    PVS_width = 1.25;
}

Move Bot::execute(Chessboard board, int turns, Player pl) {
    if (turns == 1)
        max_time = 1960;
    else
        max_time = 960;
    Move move;
    move = searchStep(pl, board);
    return move;
}
