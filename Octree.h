/**
* @�ļ�����Octree.h
* @��Ҫ����������Octree�࣬����Ϊ���ð˲�������ȥ����ɫ�Ļ��ơ���ѯ�Լ�������ɫ���Ӧ�����ص�λ���Լ��ٲ���
* @��������G_3DView�Լ�Quadtree
*
* @�汾��Ϣ�� 1.1
* @���ߣ� Lucio
* @���䣺 Wangyang3434@gmail.com
* @���ڣ�2018/5/10
* @�޸��ߣ�Lucio
* @�������ڣ�2018/6/9
*/

#pragma once
#include <vector>
#include "G_3DView.h"
#include "Quadtree.h"

/*
* ����˲����ڵ�Ľṹ��
* ��������ɫCOLORREF color����ɫֵ��Ӧ����άRGB����int x, y, z������ͬɫ����ԭͼ����std::vector<CPoint> pos��
* �Լ����ڵ���ӽڵ㸸�ڵ�λ��
*/
struct node{
	COLORREF color;
	int x, y, z;    //��ɫֵ��Ӧ����άRGB����
	std::vector<CPoint> pos;   //������ͬɫ����ԭͼ����Ĵ洢

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
	G_3DView coordination;    //����ϵ����
	Quadtree set_coordinationToColor;   //��ά��ϵ����ɫֵ��ӳ��

private:
	node* root;
	float radius;
};

