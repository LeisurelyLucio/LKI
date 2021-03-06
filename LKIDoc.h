/**
* @文件名：LKIDoc.h
* @简要描述：包含CLKIDoc类，功能为记录所需数据以及实现对功能区(Ribbon)的消息响应
*
* @版本信息： 1.1
* @作者： Lucio
* @邮箱： Wangyang3434@gmail.com
* @日期：2018/5/10
* @修改者：Lucio
* @更新日期：2018/6/10
*
* @使用了MFC Microsoft Office Fluent 用户界面，此部分版权所有(C) Microsoft Corporation，保留所有权利
*/

// LKIDoc.h: CLKIDoc 类的接口
//


#pragma once

#include <vector>
#include <set>

//==============刷新显示宏=======================
#define ON_3D_COORDINATION 1   //加载图像
#define ON_OPEN_FILE_DRAW 2 //打开图像
#define ON_ZOOM_INOUT 3 //图像的缩放
#define ON_MOVE 4 //图像平移
#define ON_MOVING 5 //图像正在平移或旋转
#define ON_CHOOSING 6  //正在选择点集
#define ON_CHOOSED 7  //选择完成


//==============鼠标模式宏=======================
#define VIEWING 0 //仅查看模式
#define MOVING 1 //平移模式，//??移动一次后会重新设为查看模式
#define CHOOSING 2 //选择模式

//==============选择工具宏=======================
#define CHOOSE_RECTANGLE 100 //矩形选框工具
#define CHOOSE_PIXEL 101 //像素选择工具
#define CHOOSE_CIRCLE 102 //圆形选框工具
#define CHOOSE_UNION 10 //取并集
#define CHOOSE_DEDUCE 11 //取差集
#define CHOOSE_INTERSECT 12 //取交集


#ifndef COLOR_SET_DEF
#define COLOR_SET_DEF 1
struct Color_set
{
	COLORREF color;
	std::vector<CPoint> set;
};

bool operator ==(Color_set a, Color_set b);
bool operator <(Color_set a, Color_set b);

#endif // !COLOR_SET_DEF

class CLKIDoc : public CDocument
{
protected: // 仅从序列化创建
	CLKIDoc();
	DECLARE_DYNCREATE(CLKIDoc)

// 特性
public:
	COLORREF m_ColorBKG;		// 客户区(左)背景色
	COLORREF C1, C2;  //选择处理相关颜色
	int show_level;   //显示模式
	int mouse_level;   //鼠标模式
	int choose_style;  //选择方式
	int choose_tool;   //选择工具
	float ZoomSize;  //缩放倍数
	CRect ImageClient;  //左客户区大小
	CRect GraphClient;  //右客户区大小
	CImage image_show;   //选择集显示
	CImage image_now;   //正在修改
	CString file_path;  //文件名路径
	std::set<Color_set> Cset; //储存颜色合并处理结果
	bool SetofChoosed_Pixel[4000][2000];  //图片中的像素选择集
	bool ShowComplete;  //判断是否完全显示

// 操作
public:
	void Init_ColorSet();  //初始化颜色值集合
	void Set_chooseset(CRect chooserect);   //设置选择集
	void Set_choosesetByCoordination(CRect chooseRect);   //通过RGB坐标系设置选择集

// 重写
public:
	virtual BOOL OnNewDocument();
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);

	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// 实现
public:
	virtual ~CLKIDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// 用于为搜索处理程序设置搜索内容的 Helper 函数
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
public:
	afx_msg void OnButton3d();
	afx_msg void OnButtonZoomImageInit();
	afx_msg void OnButtonRGBGraph_ZoomInit();
	afx_msg void OnButtonZoomOut_Image();
	afx_msg void OnButtonZoomOut_Graph();
	afx_msg void OnButtonZoomIn_Image();
	afx_msg void OnButtonZoomIn_Graph();
	afx_msg void OnFileSave();
	afx_msg void OnFileSaveAs();
	afx_msg void OnButtonInitcoordination_Image();
	afx_msg void OnButtonInitcoordination_RGBGraph();
	afx_msg void OnCheck0();
	afx_msg void OnCheck1();
	afx_msg void OnCheck2();
	afx_msg void OnUpdateCheck0(CCmdUI *pCmdUI);
	afx_msg void OnUpdateCheck1(CCmdUI *pCmdUI);
	afx_msg void OnUpdateCheck2(CCmdUI *pCmdUI);
	afx_msg void OnButtonSetC1();
	afx_msg void OnButtonSetC2();
	afx_msg void OnCheckUnion();
	afx_msg void OnUpdateCheckUnion(CCmdUI *pCmdUI);
	afx_msg void OnCheckRectChoose();
	afx_msg void OnUpdateCheckRectChoose(CCmdUI *pCmdUI);
	afx_msg void OnCheckPixelChoose();
	afx_msg void OnUpdateCheckPixelChoose(CCmdUI *pCmdUI);
	afx_msg void OnCheckCircleChoose();
	afx_msg void OnUpdateCheckCircleChoose(CCmdUI *pCmdUI);
	afx_msg void OnButtonFullFill();
	afx_msg void OnButtonCancelChoose();
	afx_msg void OnCheckDeduce();
	afx_msg void OnUpdateCheckDeduce(CCmdUI *pCmdUI);
	afx_msg void OnCheckIntersect();
	afx_msg void OnUpdateCheckIntersect(CCmdUI *pCmdUI);
	afx_msg void OnCheckShowComplete();
	afx_msg void OnUpdateCheckShowComplete(CCmdUI *pCmdUI);
	afx_msg void OnCheckShowOnlyChoosed();
	afx_msg void OnUpdateCheckShowOnlyChoosed(CCmdUI *pCmdUI);
	afx_msg void OnButtonrotate1();
	afx_msg void OnButtonrotate2();
	afx_msg void OnButtonrotateinit();
	afx_msg void OnButtonInitAll();
	afx_msg void OnButtongray();
	afx_msg void OnButtoncold();
	afx_msg void OnButtonwarm();
	afx_msg void OnButtonhighlight();
	afx_msg void OnButtondarkle();
};
