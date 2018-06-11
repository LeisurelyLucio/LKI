/**
* @文件名：Octree.cpp
* @简要描述：包含Octree类的实现，功能为利用八叉树进行去重颜色的绘制、查询以及储存颜色相对应的像素点位置以加速操作
* @类依赖：G_3DView以及Quadtree
*
* @版本信息： 1.1
* @作者： Lucio
* @邮箱： Wangyang3434@gmail.com
* @日期：2018/5/10
* @修改者：Lucio
* @更新日期：2018/6/9
*/

#include "stdafx.h"
#include "Octree.h"

/**
* 功能：构造类，并初始化类成员
* 设置了绘制像素点时使用的圆半径
*
* 作者：Lucio
* 日期：2018/6/9
* 版本信息： 1.1
*/
Octree::Octree()
{
	radius = 1.4;
	root = NULL;
}

/**
* 功能：删除储存的八叉树，释放占用空间
* 参数说明：node * &now：当前树节点
*
* 作者：Lucio
* 日期：2018/6/9
* 版本信息： 1.1
*/
void Octree::deletetree(node * &now)
{
	if (now == NULL) return;
	
	deletetree(now->bottom_left_back);
	deletetree(now->bottom_left_front);
	deletetree(now->bottom_right_back);
	deletetree(now->bottom_right_front);
	deletetree(now->top_left_back);
	deletetree(now->top_left_front);
	deletetree(now->top_right_back);
	deletetree(now->top_right_front);

	now->pos.clear();
	delete now;
	now = NULL;
}

/**
* 功能：删除储存的整个八叉树，释放占用空间
*
* 作者：Lucio
* 日期：2018/6/9
* 版本信息： 1.1
*/
void Octree::deletetree()
{
	deletetree(root);
}

Octree::~Octree()
{
	deletetree(root);
}

/**
* 功能：构造八叉树节点
* 参数说明：COLORREF color：节点对应的颜色值；int x, y, z：颜色值RGB坐标；
*          CPoint position：插入的像素点在原图像中的位置；node* now：父节点
* 返回值说明：node *为新分配的节点的指针
*
* 作者：Lucio
* 日期：2018/6/9
* 版本信息： 1.1
*/
node * Octree::createNode(COLORREF color, int x, int y, int z, CPoint position, node* now)
{
	node* tmp = new node();
	tmp->father = now;
	tmp->pos.push_back(position);
	tmp->color = color;
	tmp->x = x;
	tmp->y = y;
	tmp->z = z;
	tmp->bottom_left_back = NULL;
	tmp->bottom_left_front = NULL;
	tmp->bottom_right_back = NULL;
	tmp->bottom_right_front = NULL;
	tmp->top_left_back = NULL;
	tmp->top_left_front = NULL;
	tmp->top_right_back = NULL;
	tmp->top_right_front = NULL;
	return tmp;
}

/**
* 功能：向当前树中插入节点
* 参数说明：COLORREF color：要插入节点的颜色值； CPoint position：要插入像素点在原图像中的位置
*
* 作者：Lucio
* 日期：2018/6/9
* 版本信息： 1.1
*/
void Octree::insert(COLORREF color, CPoint position)
{
	insert(root, GetRValue(color), GetGValue(color), GetBValue(color), position);
}

/**
* 功能：向当前树中插入节点
* 参数说明：node* &now：当前节点位置；int x, y, z：颜色值RGB坐标； CPoint position：要插入像素点在原图像中的位置
*
* 作者：Lucio
* 日期：2018/6/9
* 版本信息： 1.1
*/
void Octree::insert(node* &now, int x, int y, int z, CPoint position)
{
	if (now == NULL) {
		now = createNode(RGB(x, y, z), x, y, z, position, NULL);
		return;
	}
	if (x < now->x) {
		if (y < now->y) {
			if (z < now->z) {
				if (now->top_left_back == NULL) {
					now->top_left_back = createNode(RGB(x, y, z), x, y, z, position, now);
				}
				else insert(now->top_left_back, x, y, z, position);
			}
			else if (z >= now->z) {
				if (now->top_left_front == NULL) {
					now->top_left_front = createNode(RGB(x, y, z), x, y, z, position, now);
				}
				else insert(now->top_left_front, x, y, z, position);
			}
		}
		else if (y >= now->y) {
			if (z < now->z) {
				if (now->bottom_left_back == NULL) {
					now->bottom_left_back = createNode(RGB(x, y, z), x, y, z, position, now);
				}
				else insert(now->bottom_left_back, x, y, z, position);
			}
			else if (z >= now->z) {
				if (now->bottom_left_front == NULL) {
					now->bottom_left_front = createNode(RGB(x, y, z), x, y, z, position, now);
				}
				else insert(now->bottom_left_front, x, y, z, position);
			}
		}
	}
	else if (x >= now->x) {
		if (y < now->y) {
			if (z < now->z) {
				if (now->top_right_back == NULL) {
					now->top_right_back = createNode(RGB(x, y, z), x, y, z, position, now);
				}
				else insert(now->top_right_back, x, y, z, position);
			}
			else if (z >= now->z) {
				if (now->top_right_front == NULL) {
					now->top_right_front = createNode(RGB(x, y, z), x, y, z, position, now);
				}
				else insert(now->top_right_front, x, y, z, position);
			}
		}
		else if (y >= now->y) {
			if (z < now->z) {
				if (now->bottom_right_back == NULL) {
					now->bottom_right_back = createNode(RGB(x, y, z), x, y, z, position, now);
				}
				else insert(now->bottom_right_back, x, y, z, position);
			}
			else if (z >= now->z) {
				if (x == now->x && y == now->y && z == now->z) {
					now->pos.push_back(position);
				}
				else {
					if (now->bottom_right_front == NULL) {
						now->bottom_right_front = createNode(RGB(x, y, z), x, y, z, position, now);
					}
					else insert(now->bottom_right_front, x, y, z, position);
				}
			}
		}
	}
}

/**
* 功能：从当前树中寻找节点
* 参数说明：node* &now：当前节点位置；COLORREF color：寻找节点的颜色值
* 返回指向找到的节点指针
*
* 作者：Lucio
* 日期：2018/6/9
* 版本信息： 1.1
*/
node * Octree::find(node* now, COLORREF color)
{
	return find(now, GetRValue(color), GetGValue(color), GetBValue(color));
}

/**
* 功能：从当前树中寻找节点
* 参数说明：node* &now：当前节点位置；int x, y, z：寻找节点的颜色值RGB坐标；
* 返回指向找到的节点指针
*
* 作者：Lucio
* 日期：2018/6/9
* 版本信息： 1.1
*/
node * Octree::find(node * now, int x, int y, int z)
{
	if (now == NULL) return NULL;
	if (x < now->x) {
		if (y < now->y) {
			if (z < now->z) {
				return (find(now->top_left_back, x, y, z));
			}
			else if (z >= now->z) {
				return (find(now->top_left_front, x, y, z));
			}
		}
		else if (y >= now->y) {
			if (z < now->z) {
				return (find(now->bottom_left_back, x, y, z));
			}
			else if (z >= now->z) {
				return (find(now->bottom_left_front, x, y, z));
			}
		}
	}
	else if (x >= now->x) {
		if (y < now->y) {
			if (z < now->z){
				return (find(now->top_right_back, x, y, z));
			}
			else if (z >= now->z) {
				return (find(now->top_right_front, x, y, z));
			}
		}
		else if (y >= now->y) {
			if (z < now->z) {
				return (find(now->bottom_right_back, x, y, z));
			}
			else if (z >= now->z) {
				if (x == now->x && y == now->y && z == now->z) {
					return now;
				}
				else {
					return (find(now->bottom_right_front, x, y, z));
				}
			}
		}
	}
}

/**
* 功能：从当前树中从根节点开始寻找节点
* 参数说明：COLORREF color：寻找节点的颜色值
* 返回指向找到的节点指针
*
* 作者：Lucio
* 日期：2018/6/9
* 版本信息： 1.1
*/
node * Octree::find(COLORREF color)
{
	return find(root, GetRValue(color), GetGValue(color), GetBValue(color));
}

/**
* 功能：将树中节点遍历并绘制在三维系中
* 参数说明：CDC *pDc：绘制区域句柄；node* &now：当前节点位置
*
* 作者：Lucio
* 日期：2018/6/9
* 版本信息： 1.1
*/
void Octree::Rdraw(CDC *pDc, node* now)
{
	if (now == root) set_coordinationToColor.deletetree();
	if (now == NULL) return;
	
	coordination.x = now->x;
	coordination.y = now->y;
	coordination.z = now->z;
	coordination.setPixel(pDc, now->color);

	set_coordinationToColor.insert(now->color, coordination.x, coordination.y);
	
	Rdraw(pDc, now->bottom_left_back);
	Rdraw(pDc, now->bottom_left_front);
	Rdraw(pDc, now->bottom_right_back);
	Rdraw(pDc, now->bottom_right_front);
	Rdraw(pDc, now->top_left_back);
	Rdraw(pDc, now->top_left_front);
	Rdraw(pDc, now->top_right_back);
	Rdraw(pDc, now->top_right_front);
}

/**
* 功能：将树中节点从根节点遍历并绘制在三维系中
* 参数说明：CDC *pDc：绘制区域句柄
*
* 作者：Lucio
* 日期：2018/6/9
* 版本信息： 1.1
*/
void Octree::Rdraw(CDC * pDc)
{
	Rdraw(pDc, root);
}
