/**
* @�ļ�����Quadtree.cpp
* @��Ҫ����������Quadtree���ʵ�֣�����Ϊ�����Ĳ������л���ƽ���ϵĵ��Ӧλ�õ���ɫ����ѯ
*
* @�汾��Ϣ�� 1.1
* @���ߣ� Lucio
* @���䣺 Wangyang3434@gmail.com
* @���ڣ�2018/5/10
* @�޸��ߣ�Lucio
* @�������ڣ�2018/6/10
*/

#include "stdafx.h"
#include "Quadtree.h"

/**
* ���ܣ������࣬����ʼ�����Ա
* �����˻������ص�ʱʹ�õ�Բ�뾶
*
* ���ߣ�Lucio
* ���ڣ�2018/6/9
* �汾��Ϣ�� 1.1
*/
Quadtree::Quadtree()
{
	root = NULL;
}

/**
* ���ܣ�ɾ��������Ĳ������ͷ�ռ�ÿռ�
* ����˵����node_Quad * &now����ǰ���ڵ�
*
* ���ߣ�Lucio
* ���ڣ�2018/6/9
* �汾��Ϣ�� 1.1
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
* ���ܣ�ɾ������������Ĳ������ͷ�ռ�ÿռ�
*
* ���ߣ�Lucio
* ���ڣ�2018/6/9
* �汾��Ϣ�� 1.1
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
* ���ܣ������Ĳ����ڵ�
* ����˵����COLORREF color���ڵ��Ӧ����ɫֵ��int x, y���ڵ���ͶӰƽ���ϵ�λ�ã�node* now�����ڵ�
* ����ֵ˵����node_Quad *Ϊ�·���Ľڵ��ָ��
*
* ���ߣ�Lucio
* ���ڣ�2018/6/9
* �汾��Ϣ�� 1.1
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
* ���ܣ���ǰ���в���ڵ�
* ����˵����COLORREF color��Ҫ����ڵ����ɫֵ�� int x, y���ڵ���ͶӰƽ���ϵ�λ��
*
* ���ߣ�Lucio
* ���ڣ�2018/6/9
* �汾��Ϣ�� 1.1
*/
void Quadtree::insert(COLORREF color, int x, int y)
{
	insert(root, color, x, y);
}

/**
* ���ܣ���ǰ���в���ڵ�
* ����˵����node_Quad* &now����ǰ�ڵ�λ�ã�COLORREF color��Ҫ����ڵ����ɫֵ�� int x, y���ڵ���ͶӰƽ���ϵ�λ��
*
* ���ߣ�Lucio
* ���ڣ�2018/6/9
* �汾��Ϣ�� 1.1
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
* ���ܣ��ӵ�ǰ���дӸ��ڵ㿪ʼѰ�ҽڵ�
* ����˵����int x, y���ڵ���ͶӰƽ���ϵ�λ��
* ����ָ���ҵ��Ľڵ�ָ��
*
* ���ߣ�Lucio
* ���ڣ�2018/6/9
* �汾��Ϣ�� 1.1
*/
node_Quad * Quadtree::find(int x, int y)
{
	return find(root, x, y);
}

/**
* ���ܣ��ӵ�ǰ���дӸ��ڵ㿪ʼѰ������λ����������Ľڵ�
* ����˵����node_Quad * now����ǰ�ڵ�λ�ã�int x, y���ڵ���ͶӰƽ���ϵ�λ��
* ����ָ���ҵ��Ľڵ�ָ��
*
* ���ߣ�Lucio
* ���ڣ�2018/6/9
* �汾��Ϣ�� 1.1
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
