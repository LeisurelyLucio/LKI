/**
* @文件名：Octree.h
* @简要描述：包含Octree类，功能为利用八叉树进行去重颜色的绘制、查询以及储存颜色相对应的像素点位置以加速操作
* @类依赖：G_3DView以及Quadtree
*
* @版本信息： 1.1
* @作者： Lucio
* @邮箱： Wangyang3434@gmail.com
* @日期：2018/5/10
* @修改者：Lucio
* @更新日期：2018/6/9
*/

#pragma once
#include <vector>
#include "G_3DView.h"
#include "Quadtree.h"

/*
* 定义八叉树节点的结构体
* 储存有颜色COLORREF color；颜色值对应的三维RGB坐标int x, y, z；所有同色点在原图坐标std::vector<CPoint> pos；
* 以及树节点的子节点父节点位置
*/
struct node{
	COLORREF color;
	int x, y, z;    //颜色值对应的三维RGB坐标
	std::vector<CPoint> pos;   //对所有同色点在原图坐标的存储

	node *top_left_front, *top_left_back, *top_right_front, *top_right_back, *bottom_left_front, *bottom_left_back, *bottom_right_front, *bottom_right_back;
	node *father;
};

class Octree
{
public:
	Octree();

	void deletetree(node* &now);
	void deletetree();
	~Octree();

	node* createNode(COLORREF color, int x, int y, int z, CPoint position, node* now);
	void insert(COLORREF color, CPoint position);
	void insert(node* &now, int x, int y, int z, CPoint position);
	node* find(node* now, COLORREF color);
	node* find(node* now, int x, int y, int z);
	node* find(COLORREF color);
	void Rdraw(CDC *pDc, node* now);
	void Rdraw(CDC *pDc);

public:
	G_3DView coordination;    //坐标系绘制
	Quadtree set_coordinationToColor;   //二维点系到颜色值的映射

private:
	node* root;
	float radius;
};

