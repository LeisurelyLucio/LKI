#include "stdafx.h"
#include "Octree.h"


Octree::Octree()
{
	radius = 1.4;
	root = NULL;
}


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

void Octree::deletetree()
{
	deletetree(root);
}

Octree::~Octree()
{
	deletetree(root);
}

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

void Octree::insert(COLORREF color, CPoint position)
{
	insert(root, GetRValue(color), GetGValue(color), GetBValue(color), position);
}

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

node * Octree::find(node* now, COLORREF color)
{
	return find(now, GetRValue(color), GetGValue(color), GetBValue(color));
}

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

node * Octree::find(COLORREF color)
{
	return find(root, GetRValue(color), GetGValue(color), GetBValue(color));
}

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

void Octree::Rdraw(CDC * pDc)
{
	Rdraw(pDc, root);
}
