#pragma once
#include <graphics.h>	// 需要安装easyx图形库
#include <vector>
using namespace std;

struct Point {
	int row;
	int col;
};

class Block {
public:
	// 构造函数
	Block();
	void drop();	// 下降
	void moveLeftRight(int offset);	// 左右移动
	void rotate();	// 旋转
	void draw(int leftMargin, int topMargin);	// 画方块
	static IMAGE** getImages();
	Block& operator=(const Block& other);
	bool blockInMap(const vector<vector<int>>& map);
	void solidify(vector<vector<int>>& map);
	int getBlockType();

private:
	int blockType;
	Point smallBlocks[4];
	IMAGE* img;	// 定义指针：因为每一个小方块都是一样的，无需重复定义，定义一个指针，从内存中取即可

	static IMAGE* imgs[7];	// 定义7种方块
	static int size;
};
