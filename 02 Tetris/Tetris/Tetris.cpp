#include "Tetris.h"
#include <time.h>
#include <stdlib.h>
#include <conio.h>
#include "Block.h"
#include <fstream>
#include <iostream>
#include <graphics.h>

#include <mmsystem.h>
#pragma comment(lib,"winmm.lib")
#define RECORDER_FILE "recorder.txt"

#define MAX_LEVEL 5 
//const int SPEED_NORMAL = 500;	//MS
const int SPEED_NORMAL[MAX_LEVEL] = { 500,400,300,200,100 };
const int SPEED_QUICK = 30; 

Tetris::Tetris(int rows, int cols, int left, int top, int blockSize) {
	this->rows = rows;
	this->cols = cols;
	this->leftMargin = left;
	this->topMargin = top;
	this->blockSize = blockSize;

	for (int i = 0; i < rows; i++) {
		vector<int> mapRow;
		for (int j = 0; j < cols; j++) {
			mapRow.push_back(0);
		}
		map.push_back(mapRow);
	}
}

void Tetris::init() {

	mciSendString("play res/bg.mp3 repeat", 0, 0, 0);
	delay = SPEED_NORMAL[level];

	//delay = SPEED_NORMAL[0];

	// 配置随机种子
	srand(time(NULL));

	// 创建游戏窗口
	initgraph(928, 896);

	// 加载背景图片
	loadimage(&imgBg, "res/bg2.png");
	loadimage(&imgOver, "res/over.png");
	loadimage(&imgWin, "res/win.png");

	// 初始化游戏区中的数据
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			map[i][j] = 0;
		}
	}

	score = 0;
	level = 1;
	lineCount = 0;

	highestScore = 0;
	ifstream file(RECORDER_FILE);
	if (!file.is_open()) {
		cout << RECORDER_FILE << "打开失败" << endl;
		highestScore = 0;
	}
	else {
		file >> highestScore;
	}
	file.close();

	gameOver = false;
}

void Tetris::paly() {
	init();	// 游戏初始化

	nextBlock = new Block;
	curBlock = nextBlock;
	nextBlock = new Block;

	int timer = 0;	// 定时器
	while (1) {
		// 接受用户的输入
		keyEvent();

		timer += getDelay();
		// 这里可以将延时的30ms设置成私有数据成员delay，方便修改
		if (timer > delay) {
			// 是否延时渲染
			timer = 0;
			drop();
			// 是否即使渲染
			update = true;
		}

		if (update) {
			update = false;
			// 渲染游戏画面
			updateWindow();

			// 更新游戏数据（清行）
			clearLine();
		}

		if (gameOver) {
			//保存分数
			saveScore();

			//更新游戏结束界面
			displayOver();

			system("pause");
			init(); //重新开局
		}
	}

}

void Tetris::keyEvent() {
	unsigned char ch;
	bool rotateFlag = false;
	int dx = 0;
	if (_kbhit()) {
		ch = _getch();	// 向上方向键返回224,72，向下返回224,80，向左返回224,75，向右返回224,77
		if (ch == 224) {
			update = true;
			ch = _getch();
			switch (ch)
			{
			case 72:	// 向上，旋转
				rotateFlag = true;
				break;
			case 80:	// 向下，快速
				delay = SPEED_QUICK;
				break;
			case 75:	// 向左
				dx = -1;
				break;
			case 77:	// 向右
				dx = 1;
				break;
			default:
				break;
			}
		}

		if (rotateFlag) {	// 旋转方块
			rotate();
			update = true;
		}
		if (dx != 0) {	// 平移方块
			moveLeftRight(dx);
			update = true;
		}
	}
}

void Tetris::updateWindow() {
	putimage(0, 0, &imgBg);	// 绘制背景图片

	IMAGE** imgs = Block::getImages();
	BeginBatchDraw();
	for (int i = 0; i < map.size(); i++) {
		for (int j = 0; j < map[0].size(); j++) {
			if (map[i][j] == 0) continue;
			int x = j * blockSize + leftMargin;
			int y = i * blockSize + topMargin;
			putimage(x, y, imgs[map[i][j] - 1]);
		}
	}

	//// test
	//block block;
	//block.draw(leftmargin, topmargin);
	curBlock->draw(leftMargin, topMargin);
	nextBlock->draw(689, 150);
	drawScore();
	EndBatchDraw();
}

// 第一次调用返回0；返回距离上一次调用，间隔了多少秒
int Tetris::getDelay() {
	static unsigned long long lastTime = 0;
	unsigned long long currentTime = GetTickCount();
	if (lastTime == 0) {
		lastTime = currentTime;
		return 0;
	}
	else {
		int ret = currentTime - lastTime;
		lastTime = currentTime;
		return ret;
	}
}

void Tetris::drop() {
	bakBlock = *curBlock;
	curBlock->drop();

	if (!curBlock->blockInMap(map)) {
		// 方块到底，将其“固化”
		bakBlock.solidify(map);
		delete curBlock;
		curBlock = nextBlock;
		nextBlock = new Block;

		update = true;
		// 检测游戏是否结束
		checkOver();
	}

	//delay = SPEED_NORMAL[level - 1];
	delay = SPEED_NORMAL[level];
}

void Tetris::clearLine() {
	int lines = 0;
	int k = rows - 1;
	for (int i = rows - 1; i >= 0; i--) {
		int cnt = 0;
		for (int j = 0; j < cols; j++) {
			if (map[i][j])
				cnt++;
			map[k][j] = map[i][j];	// 一边扫描，一边存储
		}
		if (cnt < cols) {// 不是满行
			k--;
		}
		else {	// 满行
			lines++;
		}

	}

	if (lines > 0) {
		// 得分
		int addScore[4] = { 10,30,60,80 };//计算得分
		score += addScore[lines - 1];

		// 播放音效
		mciSendString("play res/xiaochu1.mp3", 0, 0, 0);
		update = true;

		// 每100分一个级别：0-100第一关 101-200第二关
		level = (score + 99) / 100;
		//通关
		if (level > MAX_LEVEL) {
			gameOver = true;
		}

		lineCount += lines;
	}
}

void Tetris::moveLeftRight(int offset) {
	bakBlock = *curBlock;
	curBlock->moveLeftRight(offset);
	if (!curBlock->blockInMap(map))
		*curBlock = bakBlock;
}

void Tetris::rotate() {
	if (curBlock->getBlockType() == 7)return;	// 判断田字型

	bakBlock = *curBlock;
	curBlock->rotate();
	if (!curBlock->blockInMap(map))
		*curBlock = bakBlock;
}

void Tetris::drawScore() {
	char scoreText[32];
	sprintf_s(scoreText, sizeof(scoreText), "%d", score);

	setcolor(RGB(180, 180, 180));

	LOGFONT f;	
	gettextstyle(&f);// 获取当前的字体
	f.lfHeight = 60;
	f.lfWidth = 30;
	f.lfQuality = ANTIALIASED_QUALITY;// 抗锯齿
	strcpy_s(f.lfFaceName, sizeof(f.lfFaceName), _T("Segoe UI Black"));//设置字体
	settextstyle(&f);

	setbkmode(TRANSPARENT); // 字体设置为透明效果

	outtextxy(670, 727, scoreText);

	// 绘制消除的行数
	sprintf_s(scoreText, sizeof(scoreText), "%d", lineCount);
	gettextstyle(&f);
	int xPos = 224 - f.lfWidth * strlen(scoreText);
	outtextxy(xPos, 817, scoreText);

	// 绘制等级
	sprintf_s(scoreText, sizeof(scoreText), "%d", level);
	outtextxy(194, 727, scoreText);


	sprintf_s(scoreText, sizeof(scoreText), "%d", highestScore);
	outtextxy(670, 817, scoreText);
}

void Tetris::checkOver() {
	gameOver = (curBlock->blockInMap(map) == false);
		
}

void Tetris::saveScore() {
	if (score > highestScore) {
		highestScore = score;

		ofstream file(RECORDER_FILE);
		file << highestScore;
		file.close();
	}
}

void Tetris::displayOver() {
	mciSendString("stop res/bg.mp3", 0, 0, 0);
	if (level <= MAX_LEVEL) {
		putimage(262, 361, &imgOver);
		mciSendString("play res/over.mp3", 0, 0, 0);
	}
	else {
		putimage(262, 361, &imgWin);
		mciSendString("play res/win.mp3", 0, 0, 0);
	}
}
