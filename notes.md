## [Bot API](https://wiki.botzone.org.cn/index.php?title=Bot)

## MPL notice
>  This Source Code Form is subject to the terms of the Mozilla Public
>  License, v. 2.0. If a copy of the MPL was not distributed with this
>  file, You can obtain one at http://mozilla.org/MPL/2.0/.

## Class Treeview
```C++
class Amazons{
    public:
        class Chessboard{
            public:
                Chessboard(Chessboard origin);
        };
        enum class Square{
            Empty = 0,
            White = 1,
            Black = -1,
            Arrow = 2
        };
        class Bot;
        class Interface;
        Amazons();
        ~Amazons();
};
```