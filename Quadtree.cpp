/**
* @文件名：Quadtree.cpp
* @简要描述：包含Quadtree类的实现，功能为利用四叉树进行绘制平面上的点对应位置的颜色集查询
*
* @版本信息： 1.1
* @作者： Lucio
* @邮箱： Wangyang3434@gmail.com
* @日期：2018/5/10
* @修改者：Lucio
* @更新日期：2018/6/10
*/

#include "stdafx.h"
#include "Quadtree.h"

/**
* 功能：构造类，并初始化类成员
* 设置了绘制像素点时使用的圆半径
*
* 作者：Lucio
* 日期：2018/6/9
* 版本信息： 1.1
*/
Quadtree::Quadtree()
{
	root = NULL;
}

/**
* 功能：删除储存的四叉树，释放占用空间
* 参数说明：node_Quad * &now：当前树节点
*
* 作者：Lucio
* 日期：2018/6/9
* 版本信息： 1.1
*/
void Quadtree::deletetree(node_Quad *& now)
{
	if (now == NULL) return;

	deletetree(now->LB);
	deletetree(now->LT);
	deletetree(now->RB);
	deletetree(now->RT);

	now->colorSet.clear();
	delete now;
	now = NULL;
}

/**
* 功能：删除储存的整个四叉树，释放占用空间
*
* 作者：Lucio
* 日期：2018/6/9
* 版本信息： 1.1
*/
void Quadtree::deletetree()
{
	deletetree(root);
}


Quadtree::~Quadtree()
{
	deletetree(root);
}

/**
* 功能：构造四叉树节点
* 参数说明：COLORREF color：节点对应的颜色值；int x, y：节点在投影平面上的位置；node* now：父节点
* 返回值说明：node_Quad *为新分配的节点的指针
*
* 作者：Lucio
* 日期：2018/6/9
* 版本信息： 1.1
*/
node_Quad * Quadtree::createNode(COLORREF color, int x, int y, node_Quad * now)
{
	node_Quad* tmp = new node_Quad();
	tmp->father = now;
	tmp->colorSet.push_back(color);
	tmp->x = x;
	tmp->y = y;
	tmp->LB = NULL;
	tmp->LT= NULL;
	tmp->RB= NULL;
	tmp->RT= NULL;
	return tmp;
}

/**
* 功能：向当前树中插入节点
* 参数说明：COLORREF color：要插入节点的颜色值； int x, y：节点在投影平面上的位置
*
* 作者：Lucio
* 日期：2018/6/9
* 版本信息： 1.1
*/
void Quadtree::insert(COLORREF color, int x, int y)
{
	insert(root, color, x, y);
}

/**
* 功能：向当前树中插入节点
* 参数说明：node_Quad* &now：当前节点位置；COLORREF color：要插入节点的颜色值； int x, y：节点在投影平面上的位置
*
* 作者：Lucio
* 日期：2018/6/9
* 版本信息： 1.1
*/
void Quadtree::insert(node_Quad *& now, COLORREF color, int x, int y)
{
	if (now == NULL) {
		now = createNode(color, x, y, NULL);
		return;
	}
	if (x < now->x) {
		if (y < now->y) {
			if (now->LT == NULL) {
				now->LT = createNode(color, x, y, now);
			}
			else insert(now->LT, color, x, y);
		}
		else if (y >= now->y) {		
				if (now->LB == NULL) {
					now->LB = createNode(color, x, y, now);
				}
				else insert(now->LB, color, x, y);
		}
	}
	else if (x >= now->x) {
		if (y < now->y) {
			if (now->RT == NULL) {
				now->RT = createNode(color, x, y, now);
			}
			else insert(now->RT, color, x, y);
		}
		else if (y >= now->y) {
			if (x == now->x && y == now->y) {
				now->colorSet.push_back(color);
			}
			else {
				if (now->RB == NULL) {
					now->RB = createNode(color, x, y, now);
				}
				else insert(now->RB, color, x, y);
			}	
		}
	}
}

/**
* 功能：从当前树中从根节点开始寻找节点
* 参数说明：int x, y：节点在投影平面上的位置
* 返回指向找到的节点指针
*
* 作者：Lucio
* 日期：2018/6/9
* 版本信息： 1.1
*/
node_Quad * Quadtree::find(int x, int y)
{
	return find(root, x, y);
}

/**
* 功能：从当前树中从根节点开始寻找满足位置相等条件的节点
* 参数说明：node_Quad * now：当前节点位置；int x, y：节点在投影平面上的位置
* 返回指向找到的节点指针
*
* 作者：Lucio
* 日期：2018/6/9
* 版本信息： 1.1
*/
node_Quad * Quadtree::find(node_Quad * now, int x, int y)
{
	if (now == NULL) return NULL;
	if (x < now->x) {
		if (y < now->y) {
			return (find(now->LT, x, y));
		}
		else if (y >= now->y) {
			return (find(now->LB, x, y));
		}
	}
	else if (x >= now->x) {
		if (y < now->y) {
			return (find(now->RT, x, y));
		}
		else if (y >= now->y) {
			if (x == now->x && y == now->y) {
				return now;
			}
			else {
				return (find(now->RB, x, y));
			}
		}
	}
}
