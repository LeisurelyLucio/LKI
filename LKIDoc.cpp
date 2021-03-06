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

// LKIDoc.cpp: CLKIDoc 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "LKI.h"
#endif

#include "LKIDoc.h"
#include "LKIView.h"
#include "LeftView.h"
#include "MainFrm.h"
#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CLKIDoc

IMPLEMENT_DYNCREATE(CLKIDoc, CDocument)

BEGIN_MESSAGE_MAP(CLKIDoc, CDocument)
	ON_COMMAND(ID_BUTTON_3D, &CLKIDoc::OnButton3d)
	ON_COMMAND(ID_BUTTON6, &CLKIDoc::OnButtonZoomImageInit)
	ON_COMMAND(ID_BUTTON7, &CLKIDoc::OnButtonRGBGraph_ZoomInit)
	ON_COMMAND(ID_BUTTON2, &CLKIDoc::OnButtonZoomOut_Image)
	ON_COMMAND(ID_BUTTON4, &CLKIDoc::OnButtonZoomOut_Graph)
	ON_COMMAND(ID_BUTTON3, &CLKIDoc::OnButtonZoomIn_Image)
	ON_COMMAND(ID_BUTTON5, &CLKIDoc::OnButtonZoomIn_Graph)
	ON_COMMAND(ID_FILE_SAVE, &CLKIDoc::OnFileSave)
	ON_COMMAND(ID_FILE_SAVE_AS, &CLKIDoc::OnFileSaveAs)
	ON_COMMAND(ID_BUTTON8, &CLKIDoc::OnButtonInitcoordination_Image)
	ON_COMMAND(ID_BUTTON9, &CLKIDoc::OnButtonInitcoordination_RGBGraph)
	ON_COMMAND(ID_CHECK2, &CLKIDoc::OnCheck0)
	ON_COMMAND(ID_CHECK3, &CLKIDoc::OnCheck1)
	ON_COMMAND(ID_CHECK4, &CLKIDoc::OnCheck2)
	ON_UPDATE_COMMAND_UI(ID_CHECK2, &CLKIDoc::OnUpdateCheck0)
	ON_UPDATE_COMMAND_UI(ID_CHECK3, &CLKIDoc::OnUpdateCheck1)
	ON_UPDATE_COMMAND_UI(ID_CHECK4, &CLKIDoc::OnUpdateCheck2)
	ON_COMMAND(ID_BUTTON10, &CLKIDoc::OnButtonSetC1)
	ON_COMMAND(ID_BUTTON11, &CLKIDoc::OnButtonSetC2)
	ON_COMMAND(ID_CHECK6, &CLKIDoc::OnCheckUnion)
	ON_UPDATE_COMMAND_UI(ID_CHECK6, &CLKIDoc::OnUpdateCheckUnion)
	ON_COMMAND(ID_CHECK9, &CLKIDoc::OnCheckRectChoose)
	ON_UPDATE_COMMAND_UI(ID_CHECK9, &CLKIDoc::OnUpdateCheckRectChoose)
	ON_COMMAND(ID_CHECK8, &CLKIDoc::OnCheckPixelChoose)
	ON_UPDATE_COMMAND_UI(ID_CHECK8, &CLKIDoc::OnUpdateCheckPixelChoose)
	ON_COMMAND(ID_CHECK10, &CLKIDoc::OnCheckCircleChoose)
	ON_UPDATE_COMMAND_UI(ID_CHECK10, &CLKIDoc::OnUpdateCheckCircleChoose)
	ON_COMMAND(ID_BUTTON12, &CLKIDoc::OnButtonFullFill)
	ON_COMMAND(ID_BUTTON15, &CLKIDoc::OnButtonCancelChoose)
	ON_COMMAND(ID_CHECK7, &CLKIDoc::OnCheckDeduce)
	ON_UPDATE_COMMAND_UI(ID_CHECK7, &CLKIDoc::OnUpdateCheckDeduce)
	ON_COMMAND(ID_CHECK5, &CLKIDoc::OnCheckIntersect)
	ON_UPDATE_COMMAND_UI(ID_CHECK5, &CLKIDoc::OnUpdateCheckIntersect)
	ON_COMMAND(ID_CHECK11, &CLKIDoc::OnCheckShowComplete)
	ON_UPDATE_COMMAND_UI(ID_CHECK11, &CLKIDoc::OnUpdateCheckShowComplete)
	ON_COMMAND(ID_CHECK12, &CLKIDoc::OnCheckShowOnlyChoosed)
	ON_UPDATE_COMMAND_UI(ID_CHECK12, &CLKIDoc::OnUpdateCheckShowOnlyChoosed)
	ON_COMMAND(ID_BUTTONROTATE1, &CLKIDoc::OnButtonrotate1)
	ON_COMMAND(ID_BUTTONROTATE2, &CLKIDoc::OnButtonrotate2)
	ON_COMMAND(ID_BUTTONROTATEINIT, &CLKIDoc::OnButtonrotateinit)
	ON_COMMAND(ID_BUTTON_INITALL, &CLKIDoc::OnButtonInitAll)
	ON_COMMAND(ID_BUTTONGRAY, &CLKIDoc::OnButtongray)
	ON_COMMAND(ID_BUTTONCOLD, &CLKIDoc::OnButtoncold)
	ON_COMMAND(ID_BUTTONWARM, &CLKIDoc::OnButtonwarm)
	ON_COMMAND(ID_BUTTONHIGHLIGHT, &CLKIDoc::OnButtonhighlight)
	ON_COMMAND(ID_BUTTONDARKLE, &CLKIDoc::OnButtondarkle)
END_MESSAGE_MAP()


bool operator ==(Color_set a, Color_set b) {
	return (a.color == b.color);
}

bool operator <(Color_set a, Color_set b) {
	return (a.color < b.color);
}


// CLKIDoc 构造/析构

/**
* 功能：构造类，并初始化类成员
* 参数及返回值说明：无
*
* 作者：Lucio
* 日期：2018/6/9
* 版本信息： 1.1
*/
CLKIDoc::CLKIDoc()
{
	m_ColorBKG = RGB(192,192,192);
	show_level = 0;
	mouse_level = 0;
	choose_style = CHOOSE_UNION;
	memset(SetofChoosed_Pixel, 0, sizeof(SetofChoosed_Pixel));
	ZoomSize = 1.0;
	C1 = RGB(106, 90, 205);
	C2 = RGB(255, 69, 0);
	ShowComplete = true;
}

CLKIDoc::~CLKIDoc()
{
}

/**
* 功能：通过set与vector进行颜色去重以及位置存储，由于较慢已弃用
* 参数及返回值说明：无
*
* 作者：Lucio
* 日期：2018/6/2
* 版本信息： 1.1
*/
void CLKIDoc::Init_ColorSet()
{
	COLORREF color;
	CMFCRibbonProgressBar* progress = ((CMainFrame*)AfxGetMainWnd())->progress;
	progress->SetPos(0, true);

	for (int i = 0; i < image_now.GetHeight(); i++) {
		for (int j = 0; j < image_now.GetWidth(); j++) {
			color = image_now.GetPixel(j, i);
			Color_set tmp;
			CPoint p(j,i);
			tmp.color = color;
			auto pos = Cset.find(tmp);
			if (pos != Cset.end()) {
				// 第二步：拷贝这个元素  
				Color_set e(*pos);
				// 第三步：删除这个元素；  
				Cset.erase(pos++);
				// 自增这个迭代器以保持它有效（参见条款9）  
				// 第四步：修改这个副本  
				e.set.push_back(p);
				// 第五步：插入新值；提示它的位置  
				Cset.insert(pos, e);
			}
			else {
				tmp.set.push_back(p);
				Cset.insert(tmp);
			}
		}
		progress->SetPos(100 * i / (float)image_now.GetHeight(), true);
	}
	progress->SetPos(0, true);
}

/**
* 功能：通过左窗口选择的方式设置像素点的选择集
* 参数及返回值说明：
* CRect chooserect：由于choose_tool以及choose_style的不同对应不同的储存方式
*
* 作者：Lucio
* 日期：2018/6/9
* 版本信息： 1.1
*/
void CLKIDoc::Set_chooseset(CRect chooserect)
{
	POSITION pos = GetFirstViewPosition();
	CLeftView *view_tmp = (CLeftView*)GetNextView(pos);

	//获得图像数据
	int height = image_now.GetHeight(), width = image_now.GetWidth();
	//行距
	int pitch = image_now.GetPitch();
	//首像素地址
	BYTE * src = (BYTE*)image_now.GetBits();
	BYTE * src2 = (BYTE*)image_show.GetBits();

	switch (choose_tool) {
	case CHOOSE_PIXEL:
	case CHOOSE_RECTANGLE: {
		if (choose_style == CHOOSE_INTERSECT) {
			for (int i = 0; i < width; i++) {
				for (int j = 0; j < height; j++) {
					if (i <= chooserect.right && i >= chooserect.left && j >= chooserect.top && j <= chooserect.bottom) continue;
					if (SetofChoosed_Pixel[i][j]) {
						*(src2 + pitch * j + i * 3 + 2) = *(src + pitch * j + i * 3 + 2);
						*(src2 + pitch * j + i * 3 + 1) = *(src + pitch * j + i * 3 + 1);
						*(src2 + pitch * j + i * 3) = *(src + pitch * j + i * 3);
						SetofChoosed_Pixel[i][j] = false;
					}
				}
			}
		}
		else {
			for (int i = chooserect.left; i <= chooserect.right; i++)
				for (int j = chooserect.top; j <= chooserect.bottom; j++) {
					if (choose_style == CHOOSE_UNION) {
						if (i >= image_now.GetWidth() || j >= image_now.GetHeight() || i < 0 || j < 0) continue;
						*(src2 + pitch * j + i * 3 + 2) = GetRValue(C1);
						*(src2 + pitch * j + i * 3 + 1) = GetGValue(C1);
						*(src2 + pitch * j + i * 3) = GetBValue(C1);
						SetofChoosed_Pixel[i][j] = true;
					}
					else if (choose_style == CHOOSE_DEDUCE) {
						if (i >= image_now.GetWidth() || j >= image_now.GetHeight() || i < 0 || j < 0) continue;
						if (SetofChoosed_Pixel[i][j]) {
							*(src2 + pitch * j + i * 3 + 2) = *(src + pitch * j + i * 3 + 2);
							*(src2 + pitch * j + i * 3 + 1) = *(src + pitch * j + i * 3 + 1);
							*(src2 + pitch * j + i * 3) = *(src + pitch * j + i * 3);
							SetofChoosed_Pixel[i][j] = false;
						}
					}
				}
		}
		//view_tmp->Invalidate();
		break;
	}
	case CHOOSE_CIRCLE: {
		int R = chooserect.Height() / 2;
		CPoint O((chooserect.left + chooserect.right) / 2, (chooserect.bottom + chooserect.top) / 2);
		if (choose_style == CHOOSE_INTERSECT) {
			for (int i = 0; i < image_now.GetWidth(); i++) {
				for (int j = 0; j < image_now.GetHeight(); j++) {
					if (((i-O.x)*(i-O.x)+(j-O.y)*(j-O.y)) <= R*R) continue;
					if (SetofChoosed_Pixel[i][j]) {
						image_show.SetPixel(i, j, image_now.GetPixel(i, j));
						SetofChoosed_Pixel[i][j] = false;
					}
				}
			}
		}
		else {
			for (int i = chooserect.left; i <= chooserect.right; i++)
				for (int j = chooserect.top; j <= chooserect.bottom; j++) {
					if (choose_style == CHOOSE_UNION) {
						if (((i - O.x)*(i - O.x) + (j - O.y)*(j - O.y)) >= R * R) continue;
						if (i >= image_now.GetWidth() || j >= image_now.GetHeight() || i < 0 || j < 0) continue;
						*(src2 + pitch * j + i * 3 + 2) = GetRValue(C1);
						*(src2 + pitch * j + i * 3 + 1) = GetGValue(C1);
						*(src2 + pitch * j + i * 3) = GetBValue(C1);
						SetofChoosed_Pixel[i][j] = true;
					}
					else if (choose_style == CHOOSE_DEDUCE) {
						if (i >= image_now.GetWidth() || j >= image_now.GetHeight() || i < 0 || j < 0) continue;
						if (((i - O.x)*(i - O.x) + (j - O.y)*(j - O.y)) >= R * R) continue;
						if (SetofChoosed_Pixel[i][j]) {
							*(src2 + pitch * j + i * 3 + 2) = *(src + pitch * j + i * 3 + 2);
							*(src2 + pitch * j + i * 3 + 1) = *(src + pitch * j + i * 3 + 1);
							*(src2 + pitch * j + i * 3) = *(src + pitch * j + i * 3);
							SetofChoosed_Pixel[i][j] = false;
						}
					}
				}
		}
		//view_tmp->Invalidate();
		break;
	}
	default:
		break;
	}
}

/**
* 功能：通过右窗口选择的方式设置选择集
* 参数及返回值说明：
* CRect chooserect：由于choose_tool以及choose_style的不同对应不同的储存方式
*
* 作者：Lucio
* 日期：2018/6/9
* 版本信息： 1.1
*/
void CLKIDoc::Set_choosesetByCoordination(CRect chooserect)
{
	POSITION pos = GetFirstViewPosition();
	GetNextView(pos);
	CLKIView *view_tmp = (CLKIView*)GetNextView(pos);

	switch (choose_tool) {
	case CHOOSE_PIXEL:
	case CHOOSE_RECTANGLE: {
		if (choose_style == CHOOSE_INTERSECT) {
			for (int i = -181; i < 400; i++) {
				for (int j = -181; j < 400; j++) {
					if (i <= chooserect.right && i >= chooserect.left && j >= chooserect.top && j <= chooserect.bottom) continue;
					node_Quad* tmp = NULL;
					tmp = view_tmp->m_set.set_coordinationToColor.find(i, j);
					if (tmp == NULL) continue;
					for (auto k = tmp->colorSet.begin(); k != tmp->colorSet.end(); k++) {
						node* b = NULL;
						b = view_tmp->m_set.find(*k);
						if (b == NULL) continue;
						for (auto l = b->pos.begin(); l != b->pos.end(); l++) {
							int x = l->x, y = l->y;
							if (SetofChoosed_Pixel[x][y]) {
								image_show.SetPixel(x, y, image_now.GetPixel(x, y));
								SetofChoosed_Pixel[x][y] = false;
							}
						}						
					}
				}
			}
		}
		else {
			for (int i = chooserect.left; i <= chooserect.right; i++)
				for (int j = chooserect.top; j <= chooserect.bottom; j++) {
					if (choose_style == CHOOSE_UNION) {
						node_Quad* tmp = NULL;
						tmp = view_tmp->m_set.set_coordinationToColor.find(i, j);
						if (tmp == NULL) continue;
						for (auto k = tmp->colorSet.begin(); k != tmp->colorSet.end(); k++) {
							node* b = NULL;
							b = view_tmp->m_set.find(*k);
							if (b == NULL) continue;
							for (auto l = b->pos.begin(); l != b->pos.end(); l++) {
								int x = l->x, y = l->y;
								image_show.SetPixel(x, y, C1);
								SetofChoosed_Pixel[x][y] = true;
							}
						}
					}
					else if (choose_style == CHOOSE_DEDUCE) {
						node_Quad* tmp = NULL;
						tmp = view_tmp->m_set.set_coordinationToColor.find(i, j);
						if (tmp == NULL) continue;
						for (auto k = tmp->colorSet.begin(); k != tmp->colorSet.end(); k++) {
							node* b = NULL;
							b = view_tmp->m_set.find(*k);
							if (b == NULL) continue;
							for (auto l = b->pos.begin(); l != b->pos.end(); l++) {
								int x = l->x, y = l->y;
								if (SetofChoosed_Pixel[x][y]) {
									image_show.SetPixel(x, y, image_now.GetPixel(x, y));
									SetofChoosed_Pixel[x][y] = false;
								}
							}
						}
						
					}
				}
		}
		//view_tmp->Invalidate();
		break;
	}
	case CHOOSE_CIRCLE: {
		int R = chooserect.Height() / 2;
		CPoint O((chooserect.left + chooserect.right) / 2, (chooserect.bottom + chooserect.top) / 2);

		if (choose_style == CHOOSE_INTERSECT) {
			for (int i = -181; i < 400; i++) {
				for (int j = -181; j < 400; j++) {
					if (((i - O.x)*(i - O.x) + (j - O.y)*(j - O.y)) <= R * R) continue;
					node_Quad* tmp = NULL;
					tmp = view_tmp->m_set.set_coordinationToColor.find(i, j);
					if (tmp == NULL) continue;
					for (auto k = tmp->colorSet.begin(); k != tmp->colorSet.end(); k++) {
						node* b = NULL;
						b = view_tmp->m_set.find(*k);
						if (b == NULL) continue;
						for (auto l = b->pos.begin(); l != b->pos.end(); l++) {
							int x = l->x, y = l->y;
							if (SetofChoosed_Pixel[x][y]) {
								image_show.SetPixel(x, y, image_now.GetPixel(x, y));
								SetofChoosed_Pixel[x][y] = false;
							}
						}
					}
				}
			}
		}
		else {
			for (int i = chooserect.left; i <= chooserect.right; i++)
				for (int j = chooserect.top; j <= chooserect.bottom; j++) {
					if (choose_style == CHOOSE_UNION) {
						if (((i - O.x)*(i - O.x) + (j - O.y)*(j - O.y)) >= R * R) continue;
						node_Quad* tmp = NULL;
						tmp = view_tmp->m_set.set_coordinationToColor.find(i, j);
						if (tmp == NULL) continue;
						for (auto k = tmp->colorSet.begin(); k != tmp->colorSet.end(); k++) {
							node* b = NULL;
							b = view_tmp->m_set.find(*k);
							if (b == NULL) continue;
							for (auto l = b->pos.begin(); l != b->pos.end(); l++) {
								int x = l->x, y = l->y;
								image_show.SetPixel(x, y, C1);
								SetofChoosed_Pixel[x][y] = true;
							}
						}
					}
					else if (choose_style == CHOOSE_DEDUCE) {
						if (((i - O.x)*(i - O.x) + (j - O.y)*(j - O.y)) >= R * R) continue;
						node_Quad* tmp = NULL;
						tmp = view_tmp->m_set.set_coordinationToColor.find(i, j);
						if (tmp == NULL) continue;
						for (auto k = tmp->colorSet.begin(); k != tmp->colorSet.end(); k++) {
							node* b = NULL;
							b = view_tmp->m_set.find(*k);
							if (b == NULL) continue;
							for (auto l = b->pos.begin(); l != b->pos.end(); l++) {
								int x = l->x, y = l->y;
								if (SetofChoosed_Pixel[x][y]) {
									image_show.SetPixel(x, y, image_now.GetPixel(x, y));
									SetofChoosed_Pixel[x][y] = false;
								}
							}
						}
					}
				}
		}
		//view_tmp->Invalidate();
		break;
	}
	default:
		break;
	}
}

BOOL CLKIDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// (SDI 文档将重用该文档)
	SetTitle(L"图像处理");

	return TRUE;
}

/**
* 功能：打开图像时读取图像信息以及刷新显示
* 参数及返回值说明：LPCTSTR lpszPathName：文件路径字符串
*
* 作者：Lucio
* 日期：2018/6/9
* 版本信息： 1.1
*/
BOOL CLKIDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	// 更改光标形状
	BeginWaitCursor();

	image_now.Destroy();

	// 读取图像并附加到image_old上
	image_now.Load(lpszPathName);
	if (image_now.IsNull())
	{
		EndWaitCursor();

		CString strMsg;
		strMsg = "打开文件时出错! 请确保正确且未损坏的图片文件类型";
		MessageBox(NULL, strMsg, L"系统提示", MB_ICONINFORMATION | MB_OK);// 提示出错

		return FALSE;
	}

	image_show.Destroy();
	image_show.Create(image_now.GetWidth(), image_now.GetHeight(), 24, NULL);
	HDC hdc = image_show.GetDC(); //获取目标绘制区的hDC
	image_now.Draw(hdc, 0, 0);
	image_show.ReleaseDC();     //拿到的DC要释放，不释放第二次刷新会崩溃
	mouse_level = VIEWING;

	//Init_ColorSet(); 之前解决像素去重的方法部分代码，严重拖慢速度。。。

	// 初始化胀标记为FALSE
	SetModifiedFlag(FALSE);

	UpdateAllViews(NULL);

	// 恢复光标形状
	EndWaitCursor();
	
	return TRUE;
}

BOOL CLKIDoc::OnSaveDocument(LPCTSTR lpszPathName)
{

	return TRUE;
}




// CLKIDoc 序列化

void CLKIDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 在此添加存储代码
	}
	else
	{
		// TODO: 在此添加加载代码
	}
}

#ifdef SHARED_HANDLERS

// 缩略图的支持
void CLKIDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// 修改此代码以绘制文档数据
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// 搜索处理程序的支持
void CLKIDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// 从文档数据设置搜索内容。
	// 内容部分应由“;”分隔

	// 例如:     strSearchContent = _T("point;rectangle;circle;ole object;")；
	SetSearchContent(strSearchContent);
}

void CLKIDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = nullptr;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != nullptr)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CLKIDoc 诊断

#ifdef _DEBUG
void CLKIDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CLKIDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CLKIDoc 命令

/**
* 功能：按钮消息响应，进行图像像素点对应RGB值三维显示的分析
* 参数及返回值说明：无
*
* 作者：Lucio
* 日期：2018/6/9
* 版本信息： 1.1
*/
void CLKIDoc::OnButton3d()
{
	if (!image_now.IsNull()) {
		show_level = ON_3D_COORDINATION;
		UpdateAllViews(NULL);
	}
	else {
		AfxMessageBox(L"未打开图片!");
	}
}

/**
* 功能：按钮消息响应，对于左视图中图片缩放倍数还原默认值
* 参数及返回值说明：无
*
* 作者：Lucio
* 日期：2018/6/9
* 版本信息： 1.1
*/
void CLKIDoc::OnButtonZoomImageInit()
{
	if (image_show.IsNull()) {
		ZoomSize = 1.0;
		AfxMessageBox(L"未打开文件");
		return;
	}

	POSITION pos = GetFirstViewPosition();
	CView *view_tmp = (CView*)GetNextView(pos);
	view_tmp->GetClientRect(&ImageClient);
	//自适应显示
	if (ImageClient.bottom > 250) ImageClient.bottom -= 25;
	if (ImageClient.right > 250) ImageClient.right -= 25;
	if (ImageClient.Height() / (float)ImageClient.Width() < image_show.GetHeight() / (float)image_show.GetWidth()) {
		ZoomSize = ImageClient.Height() / (float)image_show.GetHeight();
	}
	else {
		ZoomSize = ImageClient.Width() / (float)image_show.GetWidth();
	}
	view_tmp->Invalidate();
}

/**
* 功能：按钮消息响应，对于右视图中坐标系缩放倍数还原默认值
* 参数及返回值说明：无
*
* 作者：Lucio
* 日期：2018/6/9
* 版本信息： 1.1
*/
void CLKIDoc::OnButtonRGBGraph_ZoomInit()
{
	if (image_show.IsNull()) {
		AfxMessageBox(L"未打开文件");
		return;
	}
	CRect rect;
	POSITION pos = GetFirstViewPosition();
	GetNextView(pos);
	CLKIView *view_tmp = (CLKIView*)GetNextView(pos);
	view_tmp->GetClientRect(rect);
	
	float delta = 14 / 19.0;
	if (rect.Height() / (float)rect.Width() < (1 + abs(sin(view_tmp->m_set.coordination.slant))) / (1 + abs(cos(view_tmp->m_set.coordination.slant)))) {
		delta = sin(view_tmp->m_set.coordination.slant);
		view_tmp->m_set.coordination.scale = (rect.Width() * 1 / (1 + delta)) / 255.0;
	}
	else {
		delta = cos(view_tmp->m_set.coordination.slant);
		view_tmp->m_set.coordination.scale = (rect.Height() * 1 / (1 + delta)) / 255.0;
	}
	view_tmp->Invalidate();
}

/**
* 功能：按钮消息响应，增大左视图中图片缩放倍数
* 参数及返回值说明：无
*
* 作者：Lucio
* 日期：2018/6/9
* 版本信息： 1.1
*/
void CLKIDoc::OnButtonZoomOut_Image()
{
	if (image_show.IsNull()) {
		return;
	}
	ZoomSize += 0.1;

	POSITION pos = GetFirstViewPosition();
	CView *view_tmp = (CView*)GetNextView(pos);
	show_level = ON_ZOOM_INOUT;
	view_tmp->Invalidate();
}

/**
* 功能：按钮消息响应，增大右视图中坐标系缩放倍数
* 参数及返回值说明：无
*
* 作者：Lucio
* 日期：2018/6/9
* 版本信息： 1.1
*/
void CLKIDoc::OnButtonZoomOut_Graph()
{
	if (image_show.IsNull()) {
		return;
	}
	
	POSITION pos = GetFirstViewPosition();
	GetNextView(pos);
	CLKIView *view_tmp = (CLKIView*)GetNextView(pos);
	view_tmp->m_set.coordination.scale += 0.1;
	
	show_level = ON_ZOOM_INOUT;
	view_tmp->Invalidate();
}

/**
* 功能：按钮消息响应，减小左视图中图片缩放倍数
* 参数及返回值说明：无
*
* 作者：Lucio
* 日期：2018/6/9
* 版本信息： 1.1
*/
void CLKIDoc::OnButtonZoomIn_Image()
{
	if (image_show.IsNull()) {
		return;
	}
	ZoomSize -= 0.1;

	POSITION pos = GetFirstViewPosition();
	CView *view_tmp = (CView*)GetNextView(pos);
	show_level = ON_ZOOM_INOUT;
	view_tmp->Invalidate();
}

/**
* 功能：按钮消息响应，减小右视图中坐标系缩放倍数
* 参数及返回值说明：无
*
* 作者：Lucio
* 日期：2018/6/9
* 版本信息： 1.1
*/
void CLKIDoc::OnButtonZoomIn_Graph()
{
	if (image_show.IsNull()) {
		return;
	}

	POSITION pos = GetFirstViewPosition();
	GetNextView(pos);
	CLKIView *view_tmp = (CLKIView*)GetNextView(pos);
	view_tmp->m_set.coordination.scale -= 0.1;

	show_level = ON_ZOOM_INOUT;
	view_tmp->Invalidate();
}

/**
* 功能：保存图片文件
* 参数及返回值说明：无
*
* 作者：Lucio
* 日期：2018/6/9
* 版本信息： 1.1
*/
void CLKIDoc::OnFileSave()
{
	if (image_now.IsNull()) {
		AfxMessageBox(L"未打开文件");
		return;
	}
	image_now.Save(file_path);
}

/**
* 功能：将文件覆盖已存在文件
* 参数及返回值说明：无
*
* 作者：Lucio
* 日期：2018/6/9
* 版本信息： 1.1
*/
void CLKIDoc::OnFileSaveAs()
{
	if (image_now.IsNull()) {
		AfxMessageBox(L"未打开文件");
		return;
	}
	CString strFilter;
	CSimpleArray<GUID> aguidFileTypes;
	HRESULT hResult;
	hResult = image_now.GetImporterFilterString(strFilter, aguidFileTypes, _T("All Image Files"));
	if (FAILED(hResult)) { AfxMessageBox(_T("GetImporterFilter调用失败！")); return; }
	CFileDialog dlg(TRUE, NULL, NULL, OFN_FILEMUSTEXIST, strFilter);
	if (IDOK != dlg.DoModal()) return;
	else {
		image_now.Save(dlg.GetPathName());
	}
}

/**
* 功能：按钮消息响应，对于左视图中图片平移位置还原默认值
* 参数及返回值说明：无
*
* 作者：Lucio
* 日期：2018/6/9
* 版本信息： 1.1
*/
void CLKIDoc::OnButtonInitcoordination_Image()
{
	if (image_now.IsNull()) {
		AfxMessageBox(L"未打开文件");
		return;
	}

	POSITION pos = GetFirstViewPosition();
	CLeftView *view_tmp = (CLeftView*)GetNextView(pos);
	view_tmp->draw_LT = (0, 0);
	view_tmp->Invalidate();
}

/**
* 功能：按钮消息响应，对于右视图中图片平移位置还原默认值
* 参数及返回值说明：无
*
* 作者：Lucio
* 日期：2018/6/9
* 版本信息： 1.1
*/
void CLKIDoc::OnButtonInitcoordination_RGBGraph()
{
	if (image_now.IsNull()) {
		AfxMessageBox(L"未打开文件");
		return;
	}
	//自适应显示
	POSITION pos = GetFirstViewPosition();
	GetNextView(pos);
	CLKIView *view_tmp = (CLKIView*)GetNextView(pos);

	view_tmp->GetClientRect(GraphClient);
	view_tmp->m_set.coordination.O_x = GraphClient.Width() * abs(cos(view_tmp->m_set.coordination.slant))
		/ (1 + abs(cos(view_tmp->m_set.coordination.slant))) - 18 * view_tmp->m_set.coordination.scale* view_tmp->m_set.coordination.scale;
	view_tmp->m_set.coordination.O_y = GraphClient.Height() * 1
		/ (1 + abs(sin(view_tmp->m_set.coordination.slant))) + 18 * view_tmp->m_set.coordination.scale* view_tmp->m_set.coordination.scale;

	view_tmp->Invalidate();
}

/**
* 功能：按钮消息响应，设置鼠标为查看模式
* 参数及返回值说明：无
*
* 作者：Lucio
* 日期：2018/6/9
* 版本信息： 1.1
*/
void CLKIDoc::OnCheck0()
{
	mouse_level = VIEWING;
}

/**
* 功能：按钮消息响应，设置鼠标为平移工具
* 参数及返回值说明：无
*
* 作者：Lucio
* 日期：2018/6/9
* 版本信息： 1.1
*/
void CLKIDoc::OnCheck1()
{
	if (image_now.IsNull()) {
		AfxMessageBox(L"请先加载图片！");
		return;
	}
	mouse_level = MOVING;
}

/**
* 功能：按钮消息响应，设置鼠标功能为选择工具并初始化为矩形选框及并集选择
* 参数及返回值说明：无
*
* 作者：Lucio
* 日期：2018/6/9
* 版本信息： 1.1
*/
void CLKIDoc::OnCheck2()
{
	if (image_now.IsNull()) {
		AfxMessageBox(L"请先加载图片！");
		return;
	}
	mouse_level = CHOOSING;
	choose_tool = CHOOSE_RECTANGLE;
	choose_style = CHOOSE_UNION;
}

/**
* 功能：复选框UI响应
* 参数及返回值说明：无
*
* 作者：Lucio
* 日期：2018/6/9
* 版本信息： 1.1
*/
void CLKIDoc::OnUpdateCheck0(CCmdUI *pCmdUI)
{
	if (mouse_level == VIEWING) pCmdUI->SetCheck();
	else pCmdUI->SetCheck(0);
}

/**
* 功能：复选框UI响应
* 参数及返回值说明：无
*
* 作者：Lucio
* 日期：2018/6/9
* 版本信息： 1.1
*/
void CLKIDoc::OnUpdateCheck1(CCmdUI *pCmdUI)
{
	if (mouse_level == MOVING) pCmdUI->SetCheck();
	else pCmdUI->SetCheck(0);
}

/**
* 功能：复选框UI响应
* 参数及返回值说明：无
*
* 作者：Lucio
* 日期：2018/6/9
* 版本信息： 1.1
*/
void CLKIDoc::OnUpdateCheck2(CCmdUI *pCmdUI)
{
	if (mouse_level == CHOOSING) pCmdUI->SetCheck();
	else pCmdUI->SetCheck(0);
}

/**
* 功能：设置选择集显示颜色
* 参数及返回值说明：无
*
* 作者：Lucio
* 日期：2018/6/9
* 版本信息： 1.1
*/
void CLKIDoc::OnButtonSetC1()
{
	CColorDialog dlg;
	dlg.m_cc.Flags |= CC_RGBINIT | CC_FULLOPEN;
	if (IDOK == dlg.DoModal())
	{
		C1 = dlg.m_cc.rgbResult;    //将dlg.m_cc.rgbResult获取到的颜色对话框中的颜色保存到变量C1中
	}
}

/**
* 功能：设置选择集填充颜色
* 参数及返回值说明：无
*
* 作者：Lucio
* 日期：2018/6/9
* 版本信息： 1.1
*/
void CLKIDoc::OnButtonSetC2()
{
	CColorDialog dlg;
	dlg.m_cc.Flags |= CC_RGBINIT | CC_FULLOPEN;
	if (IDOK == dlg.DoModal())
	{
		C2 = dlg.m_cc.rgbResult;    //将dlg.m_cc.rgbResult获取到的颜色对话框中的颜色保存到变量C2中
	}
}

/**
* 功能：设置鼠标选择方式为并集选取
* 参数及返回值说明：无
*
* 作者：Lucio
* 日期：2018/6/9
* 版本信息： 1.1
*/
void CLKIDoc::OnCheckUnion()
{
	if (image_now.IsNull() || mouse_level != CHOOSING) {
		AfxMessageBox(L"请使用选择模式并打开一张图片！");
		return;
	}
	choose_style = CHOOSE_UNION;
	mouse_level = CHOOSING;
}

/**
* 功能：复选框UI响应
* 参数及返回值说明：无
*
* 作者：Lucio
* 日期：2018/6/9
* 版本信息： 1.1
*/
void CLKIDoc::OnUpdateCheckUnion(CCmdUI *pCmdUI)
{
	if (choose_style == CHOOSE_UNION && mouse_level == CHOOSING) pCmdUI->SetCheck();
	else pCmdUI->SetCheck(0);
}

/**
* 功能：设置鼠标选择方式为差集选取
* 参数及返回值说明：无
*
* 作者：Lucio
* 日期：2018/6/9
* 版本信息： 1.1
*/
void CLKIDoc::OnCheckDeduce()
{
	if (image_now.IsNull() || mouse_level != CHOOSING) {
		AfxMessageBox(L"请使用选择模式并打开一张图片！");
		return;
	}
	choose_style = CHOOSE_DEDUCE;
	mouse_level = CHOOSING;
}

/**
* 功能：复选框UI响应
* 参数及返回值说明：无
*
* 作者：Lucio
* 日期：2018/6/9
* 版本信息： 1.1
*/
void CLKIDoc::OnUpdateCheckDeduce(CCmdUI *pCmdUI)
{
	if (choose_style == CHOOSE_DEDUCE && mouse_level == CHOOSING) pCmdUI->SetCheck();
	else pCmdUI->SetCheck(0);
}

/**
* 功能：设置鼠标选择方式为交集选取
* 参数及返回值说明：无
*
* 作者：Lucio
* 日期：2018/6/9
* 版本信息： 1.1
*/
void CLKIDoc::OnCheckIntersect()
{
	if (image_now.IsNull() || mouse_level != CHOOSING) {
		AfxMessageBox(L"请使用选择模式并打开一张图片！");
		return;
	}
	choose_style = CHOOSE_INTERSECT;
	mouse_level = CHOOSING;
}

/**
* 功能：复选框UI响应
* 参数及返回值说明：无
*
* 作者：Lucio
* 日期：2018/6/9
* 版本信息： 1.1
*/
void CLKIDoc::OnUpdateCheckIntersect(CCmdUI *pCmdUI)
{
	if (choose_style == CHOOSE_INTERSECT && mouse_level == CHOOSING) pCmdUI->SetCheck();
	else pCmdUI->SetCheck(0);
}

/**
* 功能：设置鼠标选择工具为矩形框选工具
* 参数及返回值说明：无
*
* 作者：Lucio
* 日期：2018/6/9
* 版本信息： 1.1
*/
void CLKIDoc::OnCheckRectChoose()
{
	if (image_now.IsNull() || mouse_level != CHOOSING) {
		AfxMessageBox(L"请使用选择模式并打开一张图片！");
		return;
	}
	choose_tool = CHOOSE_RECTANGLE;
	mouse_level = CHOOSING;
}

/**
* 功能：复选框UI响应
* 参数及返回值说明：无
*
* 作者：Lucio
* 日期：2018/6/9
* 版本信息： 1.1
*/
void CLKIDoc::OnUpdateCheckRectChoose(CCmdUI *pCmdUI)
{
	if (choose_tool == CHOOSE_RECTANGLE && mouse_level == CHOOSING) pCmdUI->SetCheck();
	else pCmdUI->SetCheck(0);
}

/**
* 功能：设置鼠标选择工具为像素选取工具
* 参数及返回值说明：无
*
* 作者：Lucio
* 日期：2018/6/9
* 版本信息： 1.1
*/
void CLKIDoc::OnCheckPixelChoose()
{
	if (image_now.IsNull() || mouse_level != CHOOSING) {
		AfxMessageBox(L"请使用选择模式并打开一张图片！");
		return;
	}
	choose_tool = CHOOSE_PIXEL;
	mouse_level = CHOOSING;
}

/**
* 功能：复选框UI响应
* 参数及返回值说明：无
*
* 作者：Lucio
* 日期：2018/6/9
* 版本信息： 1.1
*/
void CLKIDoc::OnUpdateCheckPixelChoose(CCmdUI *pCmdUI)
{
	if (choose_tool == CHOOSE_PIXEL && mouse_level == CHOOSING) pCmdUI->SetCheck();
	else pCmdUI->SetCheck(0);
}

/**
* 功能：设置鼠标选择工具为圆形框选工具
* 参数及返回值说明：无
*
* 作者：Lucio
* 日期：2018/6/9
* 版本信息： 1.1
*/
void CLKIDoc::OnCheckCircleChoose()
{
	if (image_now.IsNull() || mouse_level != CHOOSING) {
		AfxMessageBox(L"请使用选择模式并打开一张图片！");
		return;
	}
	choose_tool = CHOOSE_CIRCLE;
	mouse_level = CHOOSING;
}

/**
* 功能：复选框UI响应
* 参数及返回值说明：无
*
* 作者：Lucio
* 日期：2018/6/9
* 版本信息： 1.1
*/
void CLKIDoc::OnUpdateCheckCircleChoose(CCmdUI *pCmdUI)
{
	if (choose_tool == CHOOSE_CIRCLE && mouse_level == CHOOSING) pCmdUI->SetCheck();
	else pCmdUI->SetCheck(0);
}

/**
* 功能：通过颜色C2填充已选择选择区域
* 参数及返回值说明：无
*
* 作者：Lucio
* 日期：2018/6/9
* 版本信息： 1.1
*/
void CLKIDoc::OnButtonFullFill()
{
	if (image_now.IsNull()) {
		AfxMessageBox(L"未打开图片！");
		return;
	}

	//获得图像数据
	int height = image_now.GetHeight(), width = image_now.GetWidth();
	//行距
	int pitch = image_now.GetPitch();
	//首像素地址
	BYTE * src = (BYTE*)image_now.GetBits();

	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			if (SetofChoosed_Pixel[j][i]) {
				*(src + pitch * i + j * 3 + 2) = GetRValue(C2);
				*(src + pitch * i + j * 3 + 1) = GetGValue(C2);
				*(src + pitch * i + j * 3) = GetBValue(C2);
			}
		}
	}

	POSITION pos = GetFirstViewPosition();
	CLeftView *view_tmp = (CLeftView*)GetNextView(pos);
	view_tmp->Invalidate();
}

/**
* 功能：取消对所有像素点的选择
* 参数及返回值说明：无
*
* 作者：Lucio
* 日期：2018/6/9
* 版本信息： 1.1
*/
void CLKIDoc::OnButtonCancelChoose()
{
	memset(SetofChoosed_Pixel, 0, sizeof(SetofChoosed_Pixel));
	HDC hdc = image_show.GetDC(); //获取目标绘制区的hDC
	image_now.Draw(hdc, 0, 0);
	image_show.ReleaseDC();     //拿到的DC要释放，不释放第二次刷新会崩溃
	POSITION pos = GetFirstViewPosition();
	CLeftView *view_tmp = (CLeftView*)GetNextView(pos);
	view_tmp->Invalidate();
}

/**
* 功能：对左视图图片全图显示
* 参数及返回值说明：无
*
* 作者：Lucio
* 日期：2018/6/9
* 版本信息： 1.1
*/
void CLKIDoc::OnCheckShowComplete()
{
	if (image_now.IsNull() || mouse_level != CHOOSING) {
		AfxMessageBox(L"请使用选择模式并建立选择集！");
		return;
	}

	ShowComplete = true;
	HDC hdc = image_show.GetDC(); //获取目标绘制区的hDC
	image_now.Draw(hdc, 0, 0);
	image_show.ReleaseDC();     //拿到的DC要释放，不释放第二次刷新会崩溃

	//获得图像数据
	int height = image_now.GetHeight(), width = image_now.GetWidth();
	//行距
	int pitch = image_now.GetPitch();
	//首像素地址
	BYTE * src = (BYTE*)image_show.GetBits();

	for (int i = 0; i < image_now.GetWidth(); i++) {
		for (int j = 0; j < image_now.GetHeight(); j++) {
			if (SetofChoosed_Pixel[i][j]) {
				*(src + pitch * j + i * 3 + 2) = GetRValue(C1);
				*(src + pitch * j + i * 3 + 1) = GetGValue(C1);
				*(src + pitch * j + i * 3) = GetBValue(C1);
			}
		}
	}

	POSITION pos = GetFirstViewPosition();
	CLeftView *view_tmp = (CLeftView*)GetNextView(pos);
	view_tmp->Invalidate();
}

/**
* 功能：复选框UI响应
* 参数及返回值说明：无
*
* 作者：Lucio
* 日期：2018/6/9
* 版本信息： 1.1
*/
void CLKIDoc::OnUpdateCheckShowComplete(CCmdUI *pCmdUI)
{
	if (ShowComplete) pCmdUI->SetCheck();
	else pCmdUI->SetCheck(0);
}

/**
* 功能：对左视图仅显示图片的选择集部分
* 参数及返回值说明：无
*
* 作者：Lucio
* 日期：2018/6/9
* 版本信息： 1.1
*/
void CLKIDoc::OnCheckShowOnlyChoosed()
{
	if (image_now.IsNull() || mouse_level != CHOOSING) {
		AfxMessageBox(L"请使用选择模式并建立选择集！");
		return;
	}

	//获得图像数据
	int height = image_now.GetHeight(), width = image_now.GetWidth();
	//行距
	int pitch = image_now.GetPitch();
	//首像素地址
	BYTE * src = (BYTE*)image_show.GetBits();

	ShowComplete = false;
	for (int i = 0; i < image_now.GetWidth(); i++) {
		for (int j = 0; j < image_now.GetHeight(); j++) {
			if (!SetofChoosed_Pixel[i][j]) {
				image_show.SetPixel(i, j, m_ColorBKG);
				*(src + pitch * j + i * 3 + 2) = GetRValue(m_ColorBKG);
				*(src + pitch * j + i * 3 + 1) = GetGValue(m_ColorBKG);
				*(src + pitch * j + i * 3) = GetBValue(m_ColorBKG);
			}
		}
	}

	POSITION pos = GetFirstViewPosition();
	CLeftView *view_tmp = (CLeftView*)GetNextView(pos);
	view_tmp->Invalidate();
}

/**
* 功能：复选框UI响应
* 参数及返回值说明：无
*
* 作者：Lucio
* 日期：2018/6/9
* 版本信息： 1.1
*/
void CLKIDoc::OnUpdateCheckShowOnlyChoosed(CCmdUI *pCmdUI)
{
	if (!ShowComplete) pCmdUI->SetCheck();
	else pCmdUI->SetCheck(0);
}

/**
* 功能：坐标系顺时针旋转，仍为RG平面投影的斜二测显示
* 参数及返回值说明：无
*
* 作者：Lucio
* 日期：2018/6/9
* 版本信息： 1.1
*/
void CLKIDoc::OnButtonrotate1()
{
	POSITION pos = GetFirstViewPosition();
	GetNextView(pos);
	CLKIView *view_tmp = (CLKIView*)GetNextView(pos);
	view_tmp->m_set.coordination.slant -= PI / 25;
	if (view_tmp->m_set.coordination.slant < PI * (-5) / 24) {
		view_tmp->m_set.coordination.slant += PI / 25;
		AfxMessageBox(L"转角不能更大！坐标点将无法显示");
	}
	
	show_level = ON_MOVING;
	view_tmp->Invalidate();
}

/**
* 功能：坐标系逆时针旋转，仍为RG平面投影的斜二测显示
* 参数及返回值说明：无
*
* 作者：Lucio
* 日期：2018/6/9
* 版本信息： 1.1
*/
void CLKIDoc::OnButtonrotate2()
{
	POSITION pos = GetFirstViewPosition();
	GetNextView(pos);
	CLKIView *view_tmp = (CLKIView*)GetNextView(pos);
	view_tmp->m_set.coordination.slant += PI / 25;
	if (view_tmp->m_set.coordination.slant > PI * 17 / 24) {
		view_tmp->m_set.coordination.slant -= PI / 25;
		AfxMessageBox(L"转角不能更大！坐标点将无法显示");
	}
	
	show_level = ON_MOVING;
	view_tmp->Invalidate();
}

/**
* 功能：对坐标系的旋转角度初始化
* 参数及返回值说明：无
*
* 作者：Lucio
* 日期：2018/6/9
* 版本信息： 1.1
*/
void CLKIDoc::OnButtonrotateinit()
{
	POSITION pos = GetFirstViewPosition();
	GetNextView(pos);
	CLKIView *view_tmp = (CLKIView*)GetNextView(pos);
	view_tmp->m_set.coordination.slant = PI / 4;

	show_level = ON_MOVING;

	view_tmp->Invalidate();
}

/**
* 功能：将坐标系所有属性(旋转角、原点坐标、缩放倍数)重设为默认值
* 参数及返回值说明：无
*
* 作者：Lucio
* 日期：2018/6/9
* 版本信息： 1.1
*/
void CLKIDoc::OnButtonInitAll()
{
	POSITION pos = GetFirstViewPosition();
	GetNextView(pos);
	CLKIView *view_tmp = (CLKIView*)GetNextView(pos);
	view_tmp->m_set.coordination.slant = PI / 4;
	view_tmp->GetClientRect(GraphClient);
	view_tmp->SetInitAdaptive(GraphClient);

	show_level = ON_MOVING;
	view_tmp->Invalidate();
}

/**
* 功能：将图像转化为仿灰度图
* 参数及返回值说明：无
*
* 作者：Lucio
* 日期：2018/6/15
* 版本信息： 1.3
*/
void CLKIDoc::OnButtongray()
{
	if (image_now.IsNull()) {
		AfxMessageBox(L"请打开一张图片！");
		return;
	}

	//获得图像数据
	int height = image_now.GetHeight(), width = image_now.GetWidth();
	//行距
	int pitch = image_now.GetPitch();
	//首像素地址
	BYTE * src = (BYTE*)image_now.GetBits();
	BYTE * src2 = (BYTE*)image_show.GetBits();

	bool ifchoosed = false;

	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			if (SetofChoosed_Pixel[i][j]) {				
				*(src + pitch * j + i * 3 + 2) = *(src + pitch * j + i * 3 + 1) = *(src + pitch * j + i * 3) = 
					(*(src + pitch * j + i * 3 + 2) + *(src + pitch * j + i * 3 + 1) + *(src + pitch * j + i * 3)) / 3;
				ifchoosed = true;
				SetofChoosed_Pixel[i][j] = false;
			}
		}
	}

	if (!ifchoosed) {
		for (int i = 0; i < width; i++) {
			for (int j = 0; j < height; j++) {
				*(src + pitch * j + i * 3 + 2) = *(src + pitch * j + i * 3 + 1) = *(src + pitch * j + i * 3) =
					(*(src + pitch * j + i * 3 + 2) + *(src + pitch * j + i * 3 + 1) + *(src + pitch * j + i * 3)) / 3;
			}
		}
	}

	HDC hdc = image_show.GetDC(); //获取目标绘制区的hDC
	image_now.Draw(hdc, 0, 0);
	image_show.ReleaseDC();     //拿到的DC要释放，不释放第二次刷新会崩溃
	POSITION pos = GetFirstViewPosition();
	CLeftView *view_tmp = (CLeftView*)GetNextView(pos);
	view_tmp->Invalidate();
}

/**
* 功能：将图像冷色调化
* 参数及返回值说明：无
*
* 作者：Lucio
* 日期：2018/6/15
* 版本信息： 1.3
*/
void CLKIDoc::OnButtoncold()
{
	if (image_now.IsNull()) {
		AfxMessageBox(L"请打开一张图片！");
		return;
	}

	//获得图像数据
	int height = image_now.GetHeight(), width = image_now.GetWidth();
	//行距
	int pitch = image_now.GetPitch();
	//首像素地址
	BYTE * src = (BYTE*)image_now.GetBits();
	BYTE * src2 = (BYTE*)image_show.GetBits();

	bool ifchoosed = false;

	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			if (SetofChoosed_Pixel[i][j]) {
				if (*(src + pitch * j + i * 3) < 235) *(src + pitch * j + i * 3) = *(src + pitch * j + i * 3) + 20;
				ifchoosed = true;
				SetofChoosed_Pixel[i][j] = false;
			}
		}
	}

	if (!ifchoosed) {
		for (int i = 0; i < width; i++) {
			for (int j = 0; j < height; j++) {
				if (*(src + pitch * j + i * 3) < 235) *(src + pitch * j + i * 3) = *(src + pitch * j + i * 3) + 20;
			}
		}
	}

	HDC hdc = image_show.GetDC(); //获取目标绘制区的hDC
	image_now.Draw(hdc, 0, 0);
	image_show.ReleaseDC();     //拿到的DC要释放，不释放第二次刷新会崩溃
	POSITION pos = GetFirstViewPosition();
	CLeftView *view_tmp = (CLeftView*)GetNextView(pos);
	view_tmp->Invalidate();

}

/**
* 功能：将图像暖色调化
* 参数及返回值说明：无
*
* 作者：Lucio
* 日期：2018/6/15
* 版本信息： 1.3
*/
void CLKIDoc::OnButtonwarm()
{
	if (image_now.IsNull()) {
		AfxMessageBox(L"请打开一张图片！");
		return;
	}

	//获得图像数据
	int height = image_now.GetHeight(), width = image_now.GetWidth();
	//行距
	int pitch = image_now.GetPitch();
	//首像素地址
	BYTE * src = (BYTE*)image_now.GetBits();
	BYTE * src2 = (BYTE*)image_show.GetBits();

	bool ifchoosed = false;

	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			if (SetofChoosed_Pixel[i][j]) {
				if (*(src + pitch * j + i * 3 + 2) < 235) *(src + pitch * j + i * 3 + 2) = *(src + pitch * j + i * 3 + 2) + 20;
				if (*(src + pitch * j + i * 3 + 1) < 235) *(src + pitch * j + i * 3 + 1) = *(src + pitch * j + i * 3 + 1) + 20;
				ifchoosed = true;
				SetofChoosed_Pixel[i][j] = false;
			}
		}
	}

	if (!ifchoosed) {
		for (int i = 0; i < width; i++) {
			for (int j = 0; j < height; j++) {
				if (*(src + pitch * j + i * 3 + 2) < 235) *(src + pitch * j + i * 3 + 2) = *(src + pitch * j + i * 3 + 2) + 20;
				if (*(src + pitch * j + i * 3 + 1) < 235) *(src + pitch * j + i * 3 + 1) = *(src + pitch * j + i * 3 + 1) + 20;
			}
		}
	}

	HDC hdc = image_show.GetDC(); //获取目标绘制区的hDC
	image_now.Draw(hdc, 0, 0);
	image_show.ReleaseDC();     //拿到的DC要释放，不释放第二次刷新会崩溃
	POSITION pos = GetFirstViewPosition();
	CLeftView *view_tmp = (CLeftView*)GetNextView(pos);
	view_tmp->Invalidate();

}

/**
* 功能：将图像亮度增加
* 参数及返回值说明：无
*
* 作者：Lucio
* 日期：2018/6/15
* 版本信息： 1.3
*/
void CLKIDoc::OnButtonhighlight()
{
	if (image_now.IsNull()) {
		AfxMessageBox(L"请打开一张图片！");
		return;
	}

	//获得图像数据
	int height = image_now.GetHeight(), width = image_now.GetWidth();
	//行距
	int pitch = image_now.GetPitch();
	//首像素地址
	BYTE * src = (BYTE*)image_now.GetBits();
	BYTE * src2 = (BYTE*)image_show.GetBits();

	bool ifchoosed = false;

	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			if (SetofChoosed_Pixel[i][j]) {
				if (*(src + pitch * j + i * 3 + 2) < 235) *(src + pitch * j + i * 3 + 2) = *(src + pitch * j + i * 3 + 2) + 20;
				if (*(src + pitch * j + i * 3 + 1) < 235) *(src + pitch * j + i * 3 + 1) = *(src + pitch * j + i * 3 + 1) + 20;
				if (*(src + pitch * j + i * 3) < 235) *(src + pitch * j + i * 3) = *(src + pitch * j + i * 3) + 20;
				ifchoosed = true;
				SetofChoosed_Pixel[i][j] = false;
			}
		}
	}

	if (!ifchoosed) {
		for (int i = 0; i < width; i++) {
			for (int j = 0; j < height; j++) {
				if (*(src + pitch * j + i * 3 + 2) < 235) *(src + pitch * j + i * 3 + 2) = *(src + pitch * j + i * 3 + 2) + 20;
				if (*(src + pitch * j + i * 3 + 1) < 235) *(src + pitch * j + i * 3 + 1) = *(src + pitch * j + i * 3 + 1) + 20;
				if (*(src + pitch * j + i * 3) < 235) *(src + pitch * j + i * 3) = *(src + pitch * j + i * 3) + 20;
			}
		}
	}

	HDC hdc = image_show.GetDC(); //获取目标绘制区的hDC
	image_now.Draw(hdc, 0, 0);
	image_show.ReleaseDC();     //拿到的DC要释放，不释放第二次刷新会崩溃
	POSITION pos = GetFirstViewPosition();
	CLeftView *view_tmp = (CLeftView*)GetNextView(pos);
	view_tmp->Invalidate();

}

/**
* 功能：将图像亮度降低
* 参数及返回值说明：无
*
* 作者：Lucio
* 日期：2018/6/15
* 版本信息： 1.3
*/
void CLKIDoc::OnButtondarkle()
{
	if (image_now.IsNull()) {
		AfxMessageBox(L"请打开一张图片！");
		return;
	}

	//获得图像数据
	int height = image_now.GetHeight(), width = image_now.GetWidth();
	//行距
	int pitch = image_now.GetPitch();
	//首像素地址
	BYTE * src = (BYTE*)image_now.GetBits();
	BYTE * src2 = (BYTE*)image_show.GetBits();

	bool ifchoosed = false;

	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			if (SetofChoosed_Pixel[i][j]) {
				if (*(src + pitch * j + i * 3 + 2) > 20) *(src + pitch * j + i * 3 + 2) = *(src + pitch * j + i * 3 + 2) - 20;
				if (*(src + pitch * j + i * 3 + 1) > 20) *(src + pitch * j + i * 3 + 1) = *(src + pitch * j + i * 3 + 1) - 20;
				if (*(src + pitch * j + i * 3) > 20) *(src + pitch * j + i * 3) = *(src + pitch * j + i * 3) - 20;
				ifchoosed = true;
				SetofChoosed_Pixel[i][j] = false;
			}
		}
	}

	if (!ifchoosed) {
		for (int i = 0; i < width; i++) {
			for (int j = 0; j < height; j++) {
				if (*(src + pitch * j + i * 3 + 2) > 20) *(src + pitch * j + i * 3 + 2) = *(src + pitch * j + i * 3 + 2) - 20;
				if (*(src + pitch * j + i * 3 + 1) > 20) *(src + pitch * j + i * 3 + 1) = *(src + pitch * j + i * 3 + 1) - 20;
				if (*(src + pitch * j + i * 3) > 20) *(src + pitch * j + i * 3) = *(src + pitch * j + i * 3) - 20;
			}
		}
	}

	HDC hdc = image_show.GetDC(); //获取目标绘制区的hDC
	image_now.Draw(hdc, 0, 0);
	image_show.ReleaseDC();     //拿到的DC要释放，不释放第二次刷新会崩溃
	POSITION pos = GetFirstViewPosition();
	CLeftView *view_tmp = (CLeftView*)GetNextView(pos);
	view_tmp->Invalidate();

}
