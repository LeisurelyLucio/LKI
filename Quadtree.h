/**
* @文件名：Quadtree.h
* @简要描述：包含Quadtree类，功能为利用四叉树进行绘制平面上的点对应位置的颜色集查询
*
* @版本信息： 1.1
* @作者： Lucio
* @邮箱： Wangyang3434@gmail.com
* @日期：2018/5/10
* @修改者：Lucio
* @更新日期：2018/6/10
*/

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

