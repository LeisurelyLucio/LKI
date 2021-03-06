/**
* @文件名：LeftView.h
* @简要描述：包含CLeftView类，功能为显示图片并可进行缩放、平移以及选择像素点、填充等操作
*
* @版本信息： 1.1
* @作者： Lucio
* @邮箱： Wangyang3434@gmail.com
* @日期：2018/5/10
* @修改者：Lucio
* @更新日期：2018/6/9
*
* @使用了MFC Microsoft Office Fluent 用户界面，此部分版权所有(C) Microsoft Corporation，保留所有权利
*/

// LeftView.h: CLeftView 类的接口
//

#pragma once

class CLKIDoc;

class CLeftView : public CView
{
protected: // 仅从序列化创建
	CLeftView();
	DECLARE_DYNCREATE(CLeftView)

// 特性
public:
	CLKIDoc* GetDocument();
	CPoint draw_LT;  //绘制图片时左上角的点
	CPoint begin_move, end_move; //平移点
	bool mouse_L_down;    //判断鼠标左键是否按下

// 操作
public:

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

protected:
	

// 实现
public:
	virtual ~CLeftView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnFileOpen();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
};

#ifndef _DEBUG  // LeftView.cpp 中的调试版本
inline CLKIDoc* CLeftView::GetDocument()
   { return reinterpret_cast<CLKIDoc*>(m_pDocument); }
#endif

