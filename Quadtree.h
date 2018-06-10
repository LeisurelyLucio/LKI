#pragma once
#include <vector>

struct node_Quad {
	int x, y;    //三维RGB坐标对应的二维投影点
	std::vector<COLORREF> colorSet;   //对所有同色点在原图坐标的存储

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

