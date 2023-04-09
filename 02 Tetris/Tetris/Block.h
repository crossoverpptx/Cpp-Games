#pragma once
#include <graphics.h>	// ��Ҫ��װeasyxͼ�ο�
#include <vector>
using namespace std;

struct Point {
	int row;
	int col;
};

class Block {
public:
	// ���캯��
	Block();
	void drop();	// �½�
	void moveLeftRight(int offset);	// �����ƶ�
	void rotate();	// ��ת
	void draw(int leftMargin, int topMargin);	// ������
	static IMAGE** getImages();
	Block& operator=(const Block& other);
	bool blockInMap(const vector<vector<int>>& map);
	void solidify(vector<vector<int>>& map);
	int getBlockType();

private:
	int blockType;
	Point smallBlocks[4];
	IMAGE* img;	// ����ָ�룺��Ϊÿһ��С���鶼��һ���ģ������ظ����壬����һ��ָ�룬���ڴ���ȡ����

	static IMAGE* imgs[7];	// ����7�ַ���
	static int size;
};
