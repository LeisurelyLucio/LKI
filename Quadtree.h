/**
* @�ļ�����Quadtree.h
* @��Ҫ����������Quadtree�࣬����Ϊ�����Ĳ������л���ƽ���ϵĵ��Ӧλ�õ���ɫ����ѯ
*
* @�汾��Ϣ�� 1.1
* @���ߣ� Lucio
* @���䣺 Wangyang3434@gmail.com
* @���ڣ�2018/5/10
* @�޸��ߣ�Lucio
* @�������ڣ�2018/6/10
*/

#pragma once
#include <vector>

struct node_Quad {
	int x, y;    //��άRGB�����Ӧ�Ķ�άͶӰ��
	std::vector<COLORREF> colorSet;   //������ͬɫ����ԭͼ����Ĵ洢

	node_Quad *LT, *LB, *RT, *RB;
	node_Quad *father;
};

class Quadtree
{
public:
	Quadtree();

	void deletetree(node_Quad* &now);
	void deletetree();
	~Quadtree();

	node_Quad* createNode(COLORREF color, int x, int y, node_Quad* now);
	void insert(COLORREF color, int x, int y);
	void insert(node_Quad* &now,COLORREF color, int x, int y);
	node_Quad* find(int x, int y);
	node_Quad* find(node_Quad* now, int x, int y);

private:
	node_Quad * root;

};

