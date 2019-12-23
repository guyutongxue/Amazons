# 2019年北京大学计算概论 A 大作业——亚马逊棋

## 简介

根据作业要求，我实现了“亚马逊棋”这一简单游戏的计算机程序。本程序运行于 Windows 平台上，全部采用 C++ 语言实现。作为控制台程序，本项目没有调用任何图形化界面库。

## 编译运行

编译需要的环境配置与软件：

- MinGW 或者 Visual Studio 2015+
- CMake
- Doxygen （生成文档，可选）
- Git (克隆仓库，可选)

并将上述软件与环境的 `bin` 配置到系统 `Path` 环境变量。

执行以下命令克隆本仓库，或者直接下载本仓库：

```cmd
git clone https://github.com/Guyutongxue/Amazons.git
CD Amazons
```

若使用 MinGW ，则在此仓库内执行以下命令来编译：

```cmd
cmake -G"MinGW Makefiles" .
mingw32-make
```

若使用 Visual Studio ，则执行

```cmd
cmake .
```

即可，并在 Visual Studio 内生成 `amazons.sln` 解决方案。

编译生成的文件将存放在 `build` 文件夹内，双击运行即可。

还可以执行以下命令生成文档：

```cmd
doxygen
```

**注意** 我暂时不提供任何供下载的发布版本，因为测试表明不同机器的编译结果不能通用，会出现一些问题。

## 特点

- 精美的控制台用户界面
- 创新地使用键盘模拟方位进行操作
- 可自由选择人类玩家与计算机玩家的对战模式
- 内置一个比较但不很聪明的计算机玩家
- 可中途暂停并保存游戏到文件
- 可从文件读取游戏并继续
- 详细的帮助提示
- 基于 [Mozilla 公共许可协议 2.0](http://mozilla.org/MPL/2.0/) 的开源生态

## 关于人类玩家

此程序使用键盘控制人类玩家的落子。它提供了三组按键用于选择、移动、发射棋子，切合不同用户的习惯。

## 关于计算机玩家

正如之前所说，本项目内置了计算机玩家 `Bot` ，采用 PVS 搜索算法进行决策。如需查看相关资料，可以访问[这里](https://github.com/Guyutongxue/Amazons/blob/master/algorithm.md)。请注意，这个 Markdown 文件采用了较多数学公式，为了更好的阅读体验，请下载并使用支持 [MathJax](https://www.mathjax.org/) 或 [KaTeX](https://katex.org/) 的软件阅读。

请注意，本项目的计算机玩家 `Bot` 不参加 [Botzone](https://www.botzone.org.cn/) 天梯对局。这是因为，出于结构化程序的设计， `Bot` 的算法完全使用对象包装，导致运算时间开销过大，不适合参加比赛。不过，我已将 `Bot` 的核心算法进行面向过程的等价重构，改造为 `KSSBot` 参加天梯对局。可以通过我的用户名 `Guyutongxue` 找到该 Bot 。`KSSBot` 暂时不开放源代码。

## [更新日志](versionlog.md)

## 声明

本项目遵守 Mozilla 公共许可协议 2.0 的条款。这意味着，*您的*任何*修改*不得删除或更改适用软件的源代码形式中包含的任何许可证声明，否则将终止本许可授予*您的*权利。