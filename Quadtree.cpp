#include "stdafx.h"
#include "Quadtree.h"


Quadtree::Quadtree()
{
	root = NULL;
}

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

void Quadtree::deletetree()
{
	deletetree(root);
}


Quadtree::~Quadtree()
{
	deletetree(root);
}

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

void Quadtree::insert(COLORREF color, int x, int y)
{
	insert(root, color, x, y);
}

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

node_Quad * Quadtree::find(int x, int y)
{
	return find(root, x, y);
}

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
