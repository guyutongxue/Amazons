# 关于亚马逊棋的博弈算法

谷雨同学 于 2019年11月

## 估值算法

通过亚马逊棋的下棋规则可以看到，在亚马逊棋博弈中，博弈一方的最终目的是用本方的棋子以及箭将对方的棋子堵死，使其不能移动，由此产生了两种走棋策略。**思路一是堵策略**，即将对方棋子堵死在有限的区域里面，让对方无棋可下，这种策略进攻性强；**思路二是占领土策略**，我方棋子自行为自己圈定一个较大的领地，在这个领地里，对方棋子无法入侵，而我方棋子却有很大的自由活动范围，当对方棋子在域外未能占领足够大的领地时，也会因无棋可下而输掉比赛，这种策略注重防守。

那么针对这两种思路，便衍生了不同的估值参数，分别是 `territory` 和 `mobility` 。同时，考虑到双方“地理优势”的差距，便有 `position` 评估参数。最后综合考虑局面的进行程度以及上述三者来形成最终的估值算法。

现在需要得到对于某一方来说棋盘状态的优势程度，故以下称“我方”“本方”代表执棋双方，“我方”优势越高最终的估值结果也越大。

### 空格控制权 —— `territory`

如果一方对某个格的控制权高，即更容易到达这个空格，那么这个空格更有可能为这一方所有。故在考虑“达到”这个过程，需要用到两种走棋方式：

1. QueenMove: 指按照国际象棋中 Queen （皇后）的走法能走到某个空格的最小移动步数。这个走法与亚马逊棋中 Amazons 的走法、 Arrow 的走法相同。
2. KingMove: 指按照国际象棋中 King （国王）的走法能走到某个空格的最小移动步数。

通过计算某一方控制权高的空格的总数，可以得到这一方对整个棋盘的控制程度。

$$t_i=\sum_A\Delta(D_i^1(A), D_i^2(A)) \qquad (i=1 \text{ or } 2)$$

$$\Delta(x,y)=
\begin{cases}
0  & x=y=\infty \\
k  & x=y<\infty \\
1  & x<y        \\
-1 & x>y
\end{cases}
$$

其中，下标 $i$ 表示走法，$1$ 为 QueenMove ， $2$ 为 KingMove 。上标表示执棋方， $1$ 是本方， $2$ 是对方。 $k$ 是本方的先手优势， $-1<k<1$ 。计算结果 $t_i$ 为两种走棋方式下本方对棋盘的的总体控制情况。

### 地理优势 —— `position`

如果棋子距离空格远，那么它的地理优势便较弱。有公式如下：

$$
\begin{aligned}
p_1&=2\sum_A(2^{-D_1^1(A)}-2^{-D_1^2(A)}) \\
p_2&=\sum_A\min\{1,\max\{-1,\frac{D_2^2(A)-D_2^1(A)}6\}\}
\end{aligned}
$$

其中， $D_i^j$ 的含义与上节相同。计算结果 $p_i$ 为两种走棋方式下本方对棋盘的地理优势程度。

### 灵活度 —— `mobility`

若一个空格周围相邻的空格数越多，那么称这个空格越灵活，其灵活度越高。一个棋子一步之内能到达的空格的灵活度之和被称为这个棋子的灵活度。于是对于灵活度，有如下的计算方式：

1. 计算棋盘中所有空格的灵活度值，即相邻空格数；
2. 记录棋子 $a$ 采用 QueenMove 走法一步之内能到达的空格；
3. 对于上一步中每个空格 $\xi$ ，其灵活度为 $m_\xi$ ，则 $\displaystyle F(a)=\sum_\xi \frac {m_\xi} {K^a_\xi}$ 记为棋子 $a$ 的灵活度，其中 $K^a_\xi$ 为棋子 $a$ 采用 KingMove 到达空格 $\xi$ 的步数。

那么若 $A$ 为本方棋子集合， $B$ 为对方棋子集合，则整个棋盘的灵活度

$$m=\sum_{i\in A}F(i)-\sum_{j \in B}F(j)$$

当 $m > 0$ 时，本方棋子更灵活； $m < 0$ 时，对方棋子更灵活。

### 综合结果

最终的棋盘状态优势程度 `evalutaion` 还受到棋局进行程度的影响。在不同的棋局阶段， $t_1$ 、 $t_2$ 、 $p_1$ 、 $p_2$ 和 $m$ 都有不同的权重占比 $f_1(w)$ 、 $f_2(w)$ 、 $f_3(w)$ 、 $f_4(w)$ 和 $f_5(w)$ ，其中 $w$ 为对局数目，代表棋局进行的程度。那么最终得到了优势程度估值函数

$$E=t_1 \cdot f_1(w) + t_2 \cdot f_2(w) + p_1 \cdot f_3(w) + p_2 \cdot f_4(w) + m \cdot f_5(w)$$

## 搜索算法

如果想要通过估值函数判断下一步的走法，必然要经过一定程度上的演算才可以。而计算机进行演算的过程就是一种搜索（特别地，在这种博弈问题里被称作“博弈搜索”）。下面给出一些搜索的算法。

### 极大极小搜索 (Max Min)

极大极小搜索是一种最基本的计算机博弈搜索算法。其适用条件有两点：
1. 零和博弈: 指参与博弈的双方只能有一者胜利，一者失败，不存在其它结局；
2. 完全信息：指参与博弈的双方都可以从当前棋盘状态的读取全部博弈历史信息。

发现亚马逊棋符合上述条件。那么可以考虑“我”与“他”博弈，已经有了对于某一状态的估值函数 `double evaluation(Status st);` 可以返回我的“优势程度”。

那么考虑这样的博弈树：

```
                              [x]
                               |
       -------------------------------------------------...
       |                  |                      |
      [o]                [o]                    [o]
       |                  |                      |
 --------------...    --------------...    -------------...
 |     |     |        |     |     |        |     |     |
[x]   [x]   [x]      [x]   [x]   [x]      [x]   [x]   [x]
 |     |     |        |     |     |        |     |     |
...   ...   ...      ...   ...   ...      ...   ...   ...
```

其中\[x\]为我执棋，\[o\]为他执棋。

那么，在我执棋的时候，我必然希望在走完某步 `move[i]` 之后得到的 `evaluation(st)` 值最高；同样地，在他执棋的时候也希望得到的 `Evaluation(st)` 值最低。

那么这就是极大极小搜索的原理了。我定义函数  `int maxMin(Status st,int depth,Player pl)` ，它将返回一个“双方都采取最优策略时的优势程度”。当前 `depth` 是预先设置好需要搜索的深度，而 `pl` 则代表是我在执棋还是他在执棋。如果是我，我将选择优势程度最高的为我的策略，反之他将选择优势程度最低的为他的策略。

```C++
int maxMin(Status st,int depth,Player pl){
    int result,value;
    //检查是否到达叶子节点，即游戏结束或者搜索到最深
    if(st.game_over()||depth<=0) 
        //返回估值 
        return evaluation(st);
    if(pl==Player::Max){//我在执棋，我将找到优势最大的策略 
        result=-INF;
        for(Move m : moves){
            makeMove(m);//走棋 
            value=maxMin(st,depth-1,Player::Min);
            unmakeMove(m);//回溯 
            result=max(result,value);//取最大值 
        }
    else{//他在执棋，它将找到对我来说优势最小的策略 
        result=INF;
        for(Move m : moves){
            makeMove(m);//走棋 
            value=maxMin(st,depth-1,Player::Max);
            unmakeMove(m);//回溯 
            result=min(result,value);//取最小值
        }
    }
    return result;
}
```

### Alpha-Beta 搜索

考虑到极大极小搜索的分支数量过多时，必须采取必要的分支进行剪枝。那么 Alpha-Beta 就是为此而诞生的。

同上述情景：如果在执行极大极小搜索时发现，在检索**我执棋的**节点 $A$ 的可行策略时找到了某个节点 $A::B$ ，且这个 $B$ 的 `maxMin` 值为 $\alpha$ ，它成功地成为了目前的最大值（即函数内部的 `result` ）。

那么考虑这样一个问题，如果之后在检索到另一个节点 $A::C$ 时发现它的子节点 $A::C::D$ 的 `maxMin` 值 $v \leqslant \alpha$ 。那么可以说这个 $A::C$ 节点不必再考虑，即其子树被剪。

这是因为，节点 $A::C$ 是**他执棋的**，意味着他只想要 $C$ 的子节点的最小 `maxMin` 值。所以说我的最终结果 `result` 必然小于等于某一个子节点，即 `result` $\leqslant v$ 。所以，节点 $C$ 的 `maxMin` 值必然小于等于节点 $B$ 的 `maxMin` 值。但是 $A::B$ 和 $A::C$ 作为**我执棋的**节点 $A$ 的子节点，只取最大的那个，即 $\alpha$ 。故 $A::C$ 节点及其子树完全被剪。

同理，在**他执棋的**节点 $E$ ，若 $E::F$ 的 `maxMin` 值等于 $\beta$ ，那么但凡 $\exist$ 节点 $E::G::H$ 的 `maxMin` 值 $u \geqslant \beta$，则 $E::G$ 子树可剪。

而且可以发现，每一个节点的内部 `result` 值 $\alpha$ 或 $\beta$ （取决于谁执棋）都由其**子节点**得知、更新。而它们所造成的限制（剪枝范围）可以影响到所有同一人执棋的层的节点（比如 $A$ 例中，既然 $A::C::D$ 的值 $v \leqslant \alpha$ ，那再次对 $A::C::D$ 应用上述结论，若 $A::C::D::P::Q$ 的值 $v' \leqslant v$ ，则应用上述结论可得 $Q$ 也被 $\alpha$ 所限制。）

既然如此，就可以给每一个节点添加这样两个“属性”，一个叫 $\alpha$ ，当这个节点是**我执棋**时，其子节点不能遍历出一个小于它的值，否则就会被剪；一个叫 $\beta$ ，当这个节点是**他执棋**时，其子节点不能遍历大于它的值，否则就会被剪。记 $\alpha$ 为这个节点的下界， $\beta$ 为这个节点的上界。

那么可以得到，当我执棋时， $\alpha$ 将不断更新到我的最优策略结果，但 $\beta$ 值只能从父节点继承；同理当他执棋时， $\beta$ 将不断更新到我的最差策略结果，但 $\alpha$ 值只能从父节点继承。（这里的继承是指，这个值将被父节点所限制从而决定是否剪枝。）

所以将 `maxMin` 函数修改为 `alphaBeta` 函数，增加了两个参数 `alpha` 和 `beta` ，表示当前正在处理的节点的 $\alpha$ 和 $\beta$ 属性。

```C++
int alphaBeta(Status st,int alpha,int beta,int depth,Player pl){
    int value;
    if(st.game_over()||depth<=0) 
        //返回估值 
        return evaluation(st);
    if(pl==Player::Max){//我执棋
        for(Move m : moves){
            makeMove(m);//走棋
            value=alphaBeta(st,alpha,beta,depth-1,Player::Min)
            unmakeMove(m);//回溯
            if(value>alpha){
                alpha=value;//不断地更新 alpha 值
                //如果发生了遍历大于等于 beta 的情况，就要被剪掉 
                if(alpha>=beta)
                    return beta;
             } 
        }
        return alpha;//返回最小值 
    } 
    else{//他执棋
        for(Move m : moves){
            makeMove(m);//走棋
            value=alphaBeta(st,alpha,beta,depth-1,Player::Max)
            unmakeMove(m);//回溯
            if(value<beta){
                beta=value;//不断地更新 beta 值
                //如果发生了遍历小于等于 alpha 的情况，就要被剪掉
                if(alpha>=beta)
                    return alpha;
             } 
        }
        return beta;//返回最大值
    }
}
```

### 主要变例搜索 (PVS, Principal Variation Search)

这是对 Alpha-Beta 的一种优化。

由 Alpha-Beta 的特点，知道对于某**我执棋的**节点的子节点优势程度 `value`：
1. $\exist$ `value` $\geqslant \beta$ ，则此节点被剪；
2. $\forall$ `value` $\leqslant \alpha$ ，则此节点完全等价于极大极小搜索。

那么 Alpha-Beta 的缺点就在于，如果始终不发生第一种情况，剪枝将不会发生。更坏地，如果一直发生第二种情况， Alpha-Beta 没有起到任何优化作用。所以执行这样的剪枝方法：如果取到了一个 `value` $= \alpha '$，就可以大胆地**猜测**其余的 `value` 都小于等于 $\alpha '$ （或者大于等于 $\beta$ ，这时就直接被剪掉从而不用考虑）。如果这个**猜测**成立，则这个节点的 `result` 就等于 $\alpha '$。

那么基于这个原理，则只需要**验证猜测**是否成立。当**猜测**成立时，直接返回 $\alpha '$，否则退回到一般的 Alpha-Beta 搜索。

那么可以优化的地方就在于，可以将**验证猜测**的条件限制地更加苛刻，从而减少运算次数。注意到**验证猜测**的核心仅在于 $\alpha '$ 这一个下界，那么就将其上界压缩到一个很小的范围内，比如 $\alpha ' + 1$。换句话说，如果在 $(\alpha ', \alpha '+1)$ 内**验证猜测**成功，那么在 $(\alpha ', \beta)$ 内验证也必定能够成功；如果**验证猜测**失败（即 $\alpha '<$ `value` $< \beta$ ），那么必须退回 Alpha-Beta 搜索。尽管这样可能会使得**验证猜测**的成功率有所下降，但是它提供了一个极其显著的优点，就是在**验证猜测**的过程中会有更多被剪掉的分支。（由于上界的缩小，在**我执棋**时可能会有更多的可能被剪掉。）

同理，对于**他执棋的**节点也可采用相同的策略。事实上，实验表明这种方法会提高大约 $10\%$ 的效率。这个算法中，作为**猜测**的第一个 `value` 被称作*主要变例*，故得名。另外这种算法也被叫做*最小窗口搜索*，其中窗口就是指 $(\alpha, \beta)$ 这一区间；因为取到了形如 $(\alpha ', \alpha '+1)$ 的极小窗口（又名*零窗口*），故得名。

所以再将 `alphaBeta` 函数改为 `pvs` 函数。内容上，只考虑我执棋的情况时，首先对于第一个 `value` 采取等同于 Alpha-Beta 搜索的策略，然后将这个 `value` 赋值给 `alpha` （即上文中的 $\alpha '$ ）。然后对于其余值得计算，将他们的上下界限定在 $(\alpha ', \alpha '+1)$ 中，如果**验证猜测**失败，则回退到 Alpha-Beta 搜索。注意，这个时候的 `alpha` 没有被更新，所以应该直接用 `value` 作为下界。他执棋的情况同理。

```C++
int pvs(Status st,int alpha,int beta,int depth,Player pl){
    int value;
    if(st.game_over()||depth<=0) 
        //返回估值 
        return evaluation(st);
    if(pl==Player::Max){//我执棋
        for(Move m : moves){
            makeMove(m);//走棋
            if(m==moves[0])//如果是第一个
                //采用平凡的操作
                value=pvs(st,alpha,beta,depth-1,Player::Min)
            else{
                //若不是第一个，直接将子节点限制在很小的范围内
                value=pvs(st,alpha,alpha+1,Player::min);
                //如果验证失败，即第一个并不是最大的结果
                if(value>alpha&&value<beta)
                    //那么再改用 Alpha-Beta 搜索
                    value=pvs(st,value,beta,depth-1,Player::Min)
            }
            unmakeMove(m);//回溯
            if(value>alpha){
                alpha=value;//不断地更新 alpha 值
                //如果发生了遍历大于等于 beta 的情况，就要被剪掉 
                if(alpha>=beta)
                    return beta;
             } 
        }
        return alpha;//返回最小值 
    } 
    else{//他执棋
        for(Move m : moves){
            makeMove(m);//走棋
            if(m==moves[0])//如果是第一个
                //采用平凡的操作
                value=pvs(st,alpha,beta,depth-1,Player::Min)
            else{
                //若不是第一个，直接将子节点限制在很小的范围内
                value=pvs(st,beta,beta-1,Player::min);
                //如果验证失败，即第一个并不是最小的结果
                if(value>alpha&&value<beta)
                    //那么再改用 Alpha-Beta 搜索
                    value=pvs(st,alpha,value,depth-1,Player::Min)
            }
            unmakeMove(m);//回溯
            if(value<beta){
                beta=value;//不断地更新 beta 值
                //如果发生了遍历小于等于 alpha 的情况，就要被剪掉
                if(alpha>=beta)
                    return alpha;
             } 
        }
        return beta;//返回最大值
    }
}
```

------

## 附录：一些亚马逊棋估值算法中用到的系数

```C++
//棋局程度权重系数
double f1[23] = {0.1080, 0.1080, 0.1235, 0.1332, 0.1400,
                0.1468, 0.1565, 0.1720, 0.1949, 0.2217, 
                0.2476, 0.2680, 0.2800, 0.2884, 0.3000, 
                0.3208, 0.3535, 0.4000, 0.4613, 0.5350, 
                0.6181, 0.7075, 0.8000};

double f2[23] = {0.3940, 0.3940, 0.3826, 0.3753, 0.3700, 
                0.3647, 0.3574, 0.3460, 0.3294, 0.3098, 
                0.2903, 0.2740, 0.2631, 0.2559, 0.2500,
                0.2430, 0.2334, 0.2200, 0.2020, 0.1800,
                0.1550, 0.1280, 0.1000};

double f3[23] = {0.1160, 0.1160, 0.1224, 0.1267, 0.1300, 
                0.1333, 0.1376, 0.1440, 0.1531, 0.1640, 
                0.1754, 0.1860, 0.1944, 0.1995, 0.2000, 
                0.1950, 0.1849, 0.1700, 0.1510, 0.1287, 
                0.1038, 0.0773, 0.0500};

double f4[23] = {0.1160, 0.1160, 0.1224, 0.1267, 0.1300, 
                0.1333, 0.1376, 0.1440, 0.1531, 0.1640, 
                0.1754, 0.1860, 0.1944, 0.1995, 0.2000, 
                0.1950, 0.1849, 0.1700, 0.1510, 0.1287, 
                0.1038, 0.0773, 0.0500};

double f5[23] = {0.2300, 0.2300, 0.2159, 0.2067, 0.2000, 
                0.1933, 0.1841, 0.1700, 0.1496, 0.1254, 
                0.1010, 0.0800, 0.0652, 0.0557, 0.0500, 
                0.0464, 0.0436, 0.0400, 0.0346, 0.0274, 
                0.0190, 0.0097, 0.0000};

//先手优势系数
double k = 0.2;
```
## 参考文献

郭琴琴,李淑琴,包华.亚马逊棋机器博弈系统中评估函数的研究\[J\].计算机工程与应用,2012,48(34):50-54.

-----

<a rel="license" href="http://creativecommons.org/licenses/by-nc-sa/3.0/cn/"><img alt="知识共享许可协议" style="border-width:0" src="https://i.creativecommons.org/l/by-nc-sa/3.0/cn/88x31.png" /></a><br />
本文采用<a rel="license" href="http://creativecommons.org/licenses/by-nc/3.0/cn/">知识共享署名-非商业性使用-相同方式共享 3.0 中国大陆许可协议</a>进行许可。