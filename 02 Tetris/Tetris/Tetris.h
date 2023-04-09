#pragma once
#include <vector>
#include <graphics.h>
#include "Block.h"
using namespace std;

class Tetris {
public:
	// 构造函数：行，列，左边界，上边界，方块尺寸
	Tetris(int rows, int cols, int left, int top, int blockSize);
	void init();	// 初始化
	void paly();	// 开始游戏

private:
	void keyEvent();	// 接受用户的输入。定义为私有成员函数，只给类使用
	void updateWindow();	// 渲染游戏画面
	int getDelay();	// 返回距离上一次调用该函数，间隔了多少时间（ms）
	void drop();
	void clearLine();
	void moveLeftRight(int offset);
	void rotate();
	void drawScore();
	void checkOver();
	void saveScore();
	void displayOver();

private:
	int delay;
	bool update = false;	// 是否渲染

	//int map[20][10];
	vector<vector<int>> map;	// 0：空白，无方块；5：第5种俄罗斯方块
	int rows;
	int cols;
	int leftMargin;
	int topMargin;
	int blockSize;
	IMAGE imgBg;

	Block* curBlock;
	Block* nextBlock;	// 预告方块
	Block bakBlock;	// 当前方块降落过程中，用来备份上一个合法位置的

	int score;// 得分
	int highestScore;
	int level;
	int lineCount;

	bool gameOver;
	IMAGE imgOver;
	IMAGE imgWin;
};
