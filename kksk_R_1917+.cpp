//*****************************//
//      Amazons kksk_R by      //
//    Mt_Nomad & 12f23eddde    //
//        Version 1917+        //
//*****************************//
#include <iostream>
#include <string.h>
#include <stdlib.h> 
#include <algorithm>
#include <time.h>
#include <queue>
#include <math.h>
#include <iomanip>
using namespace std;
int grid[8][8] = { 0 };
int dx[8] = { -1,-1,-1,0,0,1,1,1 };
int dy[8] = { -1,0,1,-1,1,-1,0,1 };
int turns;
// 调整参数 res=f1*t1+f2*t2+f3*c1+f4*c2+f5*m
float f1[23]={0.1080,0.1080,0.1235,0.1332,0.1400,0.1468,0.1565,0.1720,0.1949,0.2217,0.2476,0.2680,0.2800,0.2884,0.3000,0.3208,0.3535,0.4000,0.4613,0.5350,0.6181,0.7075,0.8000};
float f2[23]={0.3940,0.3940,0.3826,0.3753,0.3700,0.3647,0.3574,0.3460,0.3294,0.3098,0.2903,0.2740,0.2631,0.2559,0.2500,0.2430,0.2334,0.2200,0.2020,0.1800,0.1550,0.1280,0.1000};
float f3[23]={0.1160,0.1160,0.1224,0.1267,0.1300,0.1333,0.1376,0.1440,0.1531,0.1640,0.1754,0.1860,0.1944,0.1995,0.2000,0.1950,0.1849,0.1700,0.1510,0.1287,0.1038,0.0773,0.0500};
float f4[23]={0.1160,0.1160,0.1224,0.1267,0.1300,0.1333,0.1376,0.1440,0.1531,0.1640,0.1754,0.1860,0.1944,0.1995,0.2000,0.1950,0.1849,0.1700,0.1510,0.1287,0.1038,0.0773,0.0500};
float f5[23]={0.2300,0.2300,0.2159,0.2067,0.2000,0.1933,0.1841,0.1700,0.1496,0.1254,0.1010,0.0800,0.0652,0.0557,0.0500,0.0464,0.0436,0.0400,0.0346,0.0274,0.0190,0.0097,0};
typedef pair<int, int> P;
const int INF = INT16_MAX;
const int GRIDSIZE = 8;
const float firstadv = 0.2;//先手优势（计算Territory时用到的参数k）
struct Move{
	int x0, y0, x1, y1, x2, y2;
	float value;
};
inline bool inMap(int x, int y);
void GetPos(int map[8][8], int color, int pos[4][2]);
inline void ProcStep(Move a, int color, int temp_grid[8][8]);
Move SearchStep(int color, int map[8][8]);
void qmove(int color,int map[8][8], int qd[8][8]);//对某一个棋盘，计算某一方棋子的queenmove数 
void kmove(int color,int map[8][8], int kd[8][8]);//对某一个棋盘，计算某一方棋子的Kingmove数 
float Evaluation(int color, int map[8][8]);
float PVS(int color, int map[8][8], float alpha, float beta, int depth, int d);
inline int Pow2(int n);
inline void StepBack(Move a, int color, int temp_grid[8][8]);
//***************************************************************************************************************//全局变量
int max_depth;
clock_t a,b;
int search_count = 0;
int max_time = 960;
float values[10] = {0,0,0,0,0,0,0,0,0,0};
int searches[10] = {0,0,0,0,0,0,0,0,0,0};
float PVS_width = 1.25;
//**************************************************************************************************************//函数
//对二维数组排序
inline bool comp(Move a, Move b)
{
	return a.value>b.value;
}
// 判断是否在地图内
inline bool inMap(int x, int y)
{
	if (x < 0 || x >= GRIDSIZE || y < 0 || y >= GRIDSIZE)
		return false;
	return true;
}
//函数：求2的整数次方
inline int Pow2(int n)
{
	if(n>=31)
	{
		return 2147483647;
	}
	return 1<<n;
}

//获取color棋子坐标
void GetPos(int map[8][8], int color, int pos[4][2])
{
    int counter = 0;
    for (int i = 0; i < 8; i++) 
	{
        for (int j = 0; j < 8; j++) 
		{
            if (map[i][j] == color)
			{
                pos[counter][0] = i;
                pos[counter][1] = j;
                counter++;
            }
        }
        if (counter == 4)
            break;
    }
}

void kmove(int color,int map[8][8], int kd[8][8])//对某一个棋盘，计算某一方棋子的Kingmove数 
{
    queue<P> que;
    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++)
            kd[i][j] = INF;//所有的位置都初始化为INF
	int pos[4][2];
	GetPos(map,color,pos);
	for(int i=0;i<4;i++)
	{
		que.push(P(pos[i][0],pos[i][1]));//将起点加入队列中
		//cout << P(pos[i][0],pos[i][1]).first << ' ' << P(pos[i][0],pos[i][1]).second << endl; //debug
		kd[pos[i][0]][pos[i][1]] = 0;//并把这一地点的距离设置为0
		while(!que.empty())//队列不空
		{
			P p = que.front();// 从队列的最前端取出元素
			que.pop();//取出后从队列中删除该元素
			for (int j = 0; j < 8; j++)//向8个方向
 	        {
    	        int nx = p.first + dx[j], ny = p.second + dy[j];//移动后的位置标记为(nx,ny)
        	    if (inMap(nx,ny) and map[nx][ny] == 0 and kd[nx][ny] > kd[p.first][p.second] + 1)//判断是否可以移动以及是否比kd中的距离小）
                {
                	que.push(P(nx, ny));//可以移动，添加到队列
         	        kd[nx][ny] = kd[p.first][p.second] + 1;//到该位置的距离为到p的距离+1
         	    }
       		}
		} 
	}
}

void qmove(int color,int map[8][8], int qd[8][8])//对某一个棋盘，计算某一方棋子的Kingmove数 
{
    queue<P> que;
    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++)
            qd[i][j] = INF;//所有的位置都初始化为INF
	int pos[4][2];
	GetPos(map,color,pos);

	for(int i=0;i<4;i++)
	{
		que.push(P(pos[i][0],pos[i][1]));//将起点加入队列中
		//cout << P(pos[i][0],pos[i][1]).first << ' ' << P(pos[i][0],pos[i][1]).second << endl;
		qd[pos[i][0]][pos[i][1]] = 0;//并把这一地点的距离设置为0
		while(!que.empty())//队列不空
		{
			P p = que.front();// 从队列的最前端取出元素
			que.pop();//取出后从队列中删除该元素
			
			for (int j = 0; j < 8; j++)//向8个方向
    	    {
        	    for (int step = 1; step < GRIDSIZE; step++) 
				{//向某方向走的步数
            	    int nx = p.first + dx[ j ] * step, ny = p.second + dy[ j ] * step;//移动后的位置标记为(nx,ny)
                	//cout<<"("<<sx<<","<<sy<<") → ("<<nx<<"," <<ny<<") t="<<temp_grid[nx][ny]<<" k="<< kd[nx][ny]<<" S="<<step<<endl;
	                if (inMap(nx, ny) && map[ nx ][ ny ] == 0 && qd[ nx ][ ny ] > qd[ p.first ][ p.second ] + 1) 
					{ 
    	                que.push(P(nx, ny));//可以移动，添加到队列
                	    qd[ nx ][ ny ] = qd[ p.first ][ p.second ] + 1;//到该位置的步数为到p的步数+1
               		} else break;//发现障碍 跳出
        		}
     	    }
		} 
	}

}

float Evaluation(int color, int map[8][8],int currturns)
{//返回Territory值     当前颜色-对方颜色  默认为白-黑 黑的话加一负号 
    search_count++;
    int k_w[8][8], k_b[8][8], q_w[8][8], q_b[8][8];
    kmove(-1, map, k_w), kmove(1, map, k_b);
    qmove(-1, map, q_w), qmove(1, map, q_b);
    float c1 = 0,c2 = 0,t1 = 0,t2 = 0;//计算position(c1,c2)
    for(int i = 0; i < GRIDSIZE;i++){
        for(int j = 0;j < GRIDSIZE;j++){
            if(q_b[i][j]==INF&&q_b[i][j]==q_w[i][j])
			t1+=0;
            else if (q_b[i][j]!=INF&&q_b[i][j]==q_w[i][j])
			t1+=-firstadv;
            else if(q_w[i][j]<q_b[i][j])
			t1+=1;
            else 
			t1+=-1;
			
            if(k_b[i][j]==INF&&k_b[i][j]==k_w[i][j]) 
			t2+=0;
            else if (k_b[i][j]!=INF&&k_b[i][j]==k_w[i][j]) 
			t2+=-firstadv;
            else if(k_w[i][j]<k_b[i][j]) 
			t2+=1;
            else 
			t2+=-1;

			c1+=2.0/Pow2(q_w[i][j])-2.0/Pow2(q_b[i][j]);
			c2 += min(1.0, max(-1.0, (1.0 / 6.0) * (k_b[i][j] - k_w[i][j])));
        }
    }
	int emptyblocks[8][8];
	memset(emptyblocks, 0, sizeof(emptyblocks));
    for(int i = 0; i < GRIDSIZE;i++){
        for(int j = 0; j < GRIDSIZE;j++){
            if(map[i][j]==0){
                for(int m = 0;m < 8; m++){ //八个方向
                    if(inMap(i+dx[m],j+dy[m])&&map[i+dx[m]][j+dy[m]]==0) emptyblocks[i][j]++;
                }
            }
        }
    }
    int pos_black[4][2], pos_white[4][2];
    GetPos(map, 1, pos_black), GetPos(map, -1, pos_white);
    //计算mobility_white,mobility_black
    float m_w = 0,m_b = 0;
    for(int j = 0; j < 4;j++) {//第j个点
        for (int i = 0; i < 8; i++) { // 向8个方向
            for(int step = 1;step < 8;step++) {
                int nx = pos_white[ j ][ 0 ] + dx[ i ]*step, ny = pos_white[ j ][ 1 ] + dy[ i ]*step;//移动后的位置标记为(nx,ny)
                //cout << "posx="<<pos_white[ j ][ 0 ]<<"posy=" << pos_white[ j ][ 1 ] << "nx=" << nx << "ny=" << ny << endl;
                if (inMap(nx, ny) && map[ nx ][ ny ] == 0 and q_w[ nx ][ ny ] != INF) { //判断是否可以移动
                    m_w += (float)emptyblocks[ nx ][ ny ]/(float)step;
                }
                else break; //此方向不能移动
            }
        }
    }
    for(int j = 0; j < 4;j++) {//第j个点
        for (int i = 0; i < 8; i++) { // 向8个方向
            for(int step = 1; step < 8; step++) {
                int nx = pos_black[ j ][ 0 ] + dx[ i ]*step, ny = pos_black[ j ][ 1 ] + dy[ i ]*step;//移动后的位置标记为(nx,ny)
                //cout << "posx="<<pos_white[ j ][ 0 ]<<"posy=" << pos_white[ j ][ 1 ] << "nx=" << nx << "ny=" << ny << endl;
                if (inMap(nx, ny) && map[ nx ][ ny ] == 0 and q_b[ nx ][ ny ] != INF) { //判断是否可以移动
                    m_b += (float)emptyblocks[ nx ][ ny ]/(float)step;
                }
                else break;
            }
        }
    }
    float value = 0;
    if(turns+currturns<22) {
        value = f1[turns+currturns]*t1+f2[turns+currturns]*t2+f3[turns+currturns]*c1+f4[turns+currturns]*c2+f5[turns+currturns]*(m_w-m_b);
    }else{
        value = f1[22]*t1+f2[22]*t2+f3[22]*c1+f4[22]*c2+f5[22]*(m_w-m_b);
    }
    if(color == 1)
    return -value;
    else
    return value;
}

// 检测某次操作是否合法
bool CheckStep(int x0, int y0, int x1, int y1, int x2, int y2, int color)
{
    if ((!inMap(x0, y0)) || (!inMap(x1, y1)) || (!inMap(x2, y2)))
        return false;
    else if (grid[x0][y0] != color || grid[x1][y1] != 0)
        return false;
    else if ((grid[x2][y2] != 0) && !(x2 == x0 && y2 == y0))
        return false;
    else return true;
}

// 在temp_grid中落子
inline void ProcStep(Move a, int color, int temp_grid[8][8])
{

		temp_grid[a.x0][a.y0] = 0;
		temp_grid[a.x1][a.y1] = color;
		temp_grid[a.x2][a.y2] = 2;
	
}

//恢复到上一步 因为不想多次memcpy
inline void StepBack(Move a, int color, int temp_grid[8][8]) 
{	
	temp_grid[a.x2][a.y2] = 0;
	temp_grid[a.x1][a.y1] = 0;
	temp_grid[a.x0][a.y0] = color;
}


float PVS(int color, int map[8][8], float alpha, float beta, int depth, int d)//alpha为当前层估值下界 beta为上界 depth为剩余深度 //color为当前按执子方 
{
	if(depth==0 or 1000*clock()/CLOCKS_PER_SEC>=max_time)
	{
		return Evaluation(color,map,(d-depth)/2);
	} 
	Move moves[1500];
	memset(moves,0,sizeof(moves));
	int pos=0, pos_rival=0;
	float value = 0;
	int pos_color[8][2];

	for(int i=0;i<8;i++)//getpos
	{
		for(int j=0;j<8;j++)
		{
			if(map[i][j]==color)
			{
				pos_color[pos][0] = i;
				pos_color[pos][1] = j;
				pos = pos+1;
			}
			if(map[i][j]==-color)
			{
				pos_color[pos_rival+4][0] = i;
				pos_color[pos_rival+4][1] = j;
				pos_rival = pos_rival+1;
			}
		}
		if(pos==4 and pos_rival==4)
		break;
	}
	pos = 0;//cout << "OK" << endl; 
	for(int i=0;i<4;i++)	
	{
		int k, l, len1 ,len2, x1, x2, y1, y2;
		for(k=0;k<8;k++)
		{
			for(len1=1;len1<8;len1++)
			{
				x1 = pos_color[i][0] + dx[k] * len1;
				y1 = pos_color[i][1] + dy[k] * len1;
				if(map[x1][y1]!=0 or !inMap(x1, y1))
				break;
				for(l=0;l<8;l++)
				{
					for(len2=1;len2<8;len2++)
					{
						x2 = x1 + dx[l] * len2;
						y2 = y1 + dy[l] * len2;
						if (!inMap(x2, y2))
						break;
						if (map[x2][y2] != 0 && !(pos_color[i][0] == x2 && pos_color[i][1] == y2))
						break;
						moves[pos].x0=pos_color[i][0];
						moves[pos].y0=pos_color[i][1];
						moves[pos].x1=x1;
						moves[pos].y1=y1;
						moves[pos].x2=x2;
						moves[pos++].y2=y2;
					}
				}
			}
		}
	}//以上为步法生成部分 同时利用pos还可以做游戏是否结束的判断 
	if(pos==0)
	{
		return Evaluation(color, map, (d-depth)/2);
	}//边界条件
	
	//pvs全窗口搜索 
	float best;
	ProcStep(moves[0], color, map);
	best = -PVS(-color,map,-beta,-alpha,depth-1,d);
	StepBack(moves[0], color, map);
	if(best>alpha) alpha = best;
	
	for(int i=1;i<pos;i++)
	{
		ProcStep(moves[i], color, map);
		value = -PVS(-color,map,-best-PVS_width,-best,depth-1,d);
		
		if(value>=best+1 and value<beta)//出现了窗口以外的最大值，这时要更新best
		best = -PVS(-color,map,-beta,-value,depth-1,d);//以value作为下界再次搜索
		
		else if(value>best)//窗口以内找到
		best = value; 
		StepBack(moves[i], color, map);
		
		//更新alpha以及剪枝
		//cout << best << endl;
		if(best>alpha) alpha = best;
		if(alpha>=beta) break;//下界大于等于上界 剪枝 
	} 
	return best;
}

Move SearchStep(int color, int map[8][8])//单独把第一层的搜索做成一个函数 为了预先排序 因为第二层大概率搜不完 并且这一层不是返回估值二十走法 
{
	int temp_grid[8][8];
	memcpy(temp_grid, map, 256);

	Move moves[1232];
	memset(moves,0,sizeof(moves));
	int i,j;
	int pos=0, pos_rival=0;
	float value = 0;
	int pos_color[8][2];
	for(int i=0;i<8;i++)
	{
		for(int j=0;j<8;j++)
		{
			if(map[i][j]==color)
			{
				pos_color[pos][0] = i;
				pos_color[pos][1] = j;
				pos = pos+1;
			}
			if(map[i][j]==-color)
			{
				pos_color[pos_rival+4][0] = i;
				pos_color[pos_rival+4][1] = j;
				pos_rival = pos_rival+1;
			}
		}
		if(pos==4 and pos_rival==4)
		break;
	}
	pos = 0;
	for(int i=0;i<4;i++)	
	{
		int k, l, len1 ,len2, x1, x2, y1, y2;
		for(k=0;k<8;k++)
		{
			for(len1=1;len1<8;len1++)
			{
				x1 = pos_color[i][0] + dx[k] * len1;
				y1 = pos_color[i][1] + dy[k] * len1;
				if(map[x1][y1]!=0 or !inMap(x1, y1))
				break;
				for(l=0;l<8;l++)
				{
					for(len2=1;len2<8;len2++)
					{
						x2 = x1 + dx[l] * len2;
						y2 = y1 + dy[l] * len2;
						if (!inMap(x2, y2))
						break;
						if (map[x2][y2] != 0 && !(pos_color[i][0] == x2 && pos_color[i][1] == y2))
						break;
						
						//cout << value << endl;
						moves[pos].x0=pos_color[i][0];
						moves[pos].y0=pos_color[i][1];
						moves[pos].x1=x1;
						moves[pos].y1=y1;
						moves[pos].x2=x2;
						moves[pos].y2=y2;
						ProcStep(moves[pos], color, temp_grid);
						value = Evaluation(color, temp_grid,0);
						StepBack(moves[pos], color, temp_grid);
						moves[pos++].value=value;
						//cout << i << ' ' << value << endl;
					}
				}
			}
		}
	}
	sort(moves,moves+pos,comp);
	values[0] = moves[0].value;
	//预先排序 
	int t=0;
	for(int d=1;d<10;d++)//我知道我必定搜不了10层 但梦想总是要有的 
	{
		max_depth = d;
		for(t=0;t<pos;t++)
		{	
			if(1000*clock()/CLOCKS_PER_SEC>=max_time)
			break;
			ProcStep(moves[t],color,temp_grid);
			value = -PVS(-color,temp_grid,-INF,INF,d,d);
			moves[t].value = value;
			StepBack(moves[t],color,temp_grid);
			
		}
		sort(moves,moves+t,comp);
		if(1000*clock()/CLOCKS_PER_SEC>=max_time)
		break;
	}
	return moves[0];
}


int main()
{
	
	cin >> turns;
	int x0, y0, x1, y1, x2, y2, color;
	if(turns == 1) max_time = 1960;
	else max_time = 960;
	grid[0][2]=grid[0][5]=grid[2][0]=grid[2][7]=1;
	grid[7][2]=grid[7][5]=grid[5][0]=grid[5][7]=-1;
	
	cin >> y0 >> x0 >> y1 >> x1 >> y2 >> x2;
//	a = clock();
	if(x0==-1)
	color = 1;
	else
	color = -1;

	if(color==-1) {
        //ProcStep(x0, y0, x1, y1, x2, y2, 1);//对方落子
        grid[x0][y0] = 0;
        grid[x1][y1] = 1;
        grid[x2][y2] = 2;
    }
	for(int i=1;i<turns;i++)//以后的回合，每回合有两次移动，且都是自己先动，对方后动 
	{
		cin >> y0 >> x0 >> y1 >> x1 >> y2 >> x2;
		//ProcStep(x0, y0, x1, y1, x2, y2, color);//己方落子
        grid[x0][y0] = 0;
        grid[x1][y1] = color;
        grid[x2][y2] = 2;
		
		cin >> y0 >> x0 >> y1 >> x1 >> y2 >> x2;
		//ProcStep(x0, y0, x1, y1, x2, y2, -color);//对方落子
        grid[x0][y0] = 0;
        grid[x1][y1] = -color;
        grid[x2][y2] = 2;
	}
	
	Move move;
	move = SearchStep(color, grid);
	cout << move.y0 << ' ' << move.x0 << ' ' << move.y1 << ' ' << move.x1 << ' ' << move.y2 << ' ' << move.x2 << endl;
	return 0;
}