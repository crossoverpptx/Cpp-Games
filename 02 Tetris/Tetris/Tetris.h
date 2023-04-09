#pragma once
#include <vector>
#include <graphics.h>
#include "Block.h"
using namespace std;

class Tetris {
public:
	// ���캯�����У��У���߽磬�ϱ߽磬����ߴ�
	Tetris(int rows, int cols, int left, int top, int blockSize);
	void init();	// ��ʼ��
	void paly();	// ��ʼ��Ϸ

private:
	void keyEvent();	// �����û������롣����Ϊ˽�г�Ա������ֻ����ʹ��
	void updateWindow();	// ��Ⱦ��Ϸ����
	int getDelay();	// ���ؾ�����һ�ε��øú���������˶���ʱ�䣨ms��
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
	bool update = false;	// �Ƿ���Ⱦ

	//int map[20][10];
	vector<vector<int>> map;	// 0���հף��޷��飻5����5�ֶ���˹����
	int rows;
	int cols;
	int leftMargin;
	int topMargin;
	int blockSize;
	IMAGE imgBg;

	Block* curBlock;
	Block* nextBlock;	// Ԥ�淽��
	Block bakBlock;	// ��ǰ���齵������У�����������һ���Ϸ�λ�õ�

	int score;// �÷�
	int highestScore;
	int level;
	int lineCount;

	bool gameOver;
	IMAGE imgOver;
	IMAGE imgWin;
};
