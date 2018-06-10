// 这段 MFC 示例源代码演示如何使用 MFC Microsoft Office Fluent 用户界面
// (“Fluent UI”)。该示例仅供参考，
// 用以补充《Microsoft 基础类参考》和
// MFC C++ 库软件随附的相关电子文档。
// 复制、使用或分发 Fluent UI 的许可条款是单独提供的。
// 若要了解有关 Fluent UI 许可计划的详细信息，请访问
// https://go.microsoft.com/fwlink/?LinkId=238214.
//
// 版权所有(C) Microsoft Corporation
// 保留所有权利。

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
END_MESSAGE_MAP()


bool operator ==(Color_set a, Color_set b) {
	return (a.color == b.color);
}

bool operator <(Color_set a, Color_set b) {
	return (a.color < b.color);
}


// CLKIDoc 构造/析构

CLKIDoc::CLKIDoc()
{
	// TODO: 在此添加一次性构造代码
	m_ColorBKG = RGB(192,192,192);
	show_level = 0;
	mouse_level = 0;
	choose_style = CHOOSE_UNION;
	memset(SetofChoosed_Pixel, 0, sizeof(SetofChoosed_Pixel));
	ZoomSize = 1.0;
	C1 = RGB(106, 90, 205);
	C2 = RGB(255, 69, 0);
}

CLKIDoc::~CLKIDoc()
{
}

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

void CLKIDoc::Set_chooseset(CRect chooserect)
{
	POSITION pos = GetFirstViewPosition();
	CLeftView *view_tmp = (CLeftView*)GetNextView(pos);
	switch (choose_tool) {
	case CHOOSE_PIXEL:
	case CHOOSE_RECTANGLE: {
		if (choose_style == CHOOSE_INTERSECT) {
			for (int i = 0; i < image_now.GetWidth(); i++) {
				for (int j = 0; j < image_now.GetHeight(); j++) {
					if (i <= chooserect.right && i >= chooserect.left && j >= chooserect.top && j <= chooserect.bottom) continue;
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
						if (i >= image_now.GetWidth() || j >= image_now.GetHeight() || i < 0 || j < 0) continue;
						image_show.SetPixel(i, j, C1);
						SetofChoosed_Pixel[i][j] = true;
					}
					else if (choose_style == CHOOSE_DEDUCE) {
						if (i >= image_now.GetWidth() || j >= image_now.GetHeight() || i < 0 || j < 0) continue;
						if (SetofChoosed_Pixel[i][j]) {
							image_show.SetPixel(i, j, image_now.GetPixel(i, j));
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
						image_show.SetPixel(i, j, C1);
						SetofChoosed_Pixel[i][j] = true;
					}
					else if (choose_style == CHOOSE_DEDUCE) {
						if (i >= image_now.GetWidth() || j >= image_now.GetHeight() || i < 0 || j < 0) continue;
						if (((i - O.x)*(i - O.x) + (j - O.y)*(j - O.y)) >= R * R) continue;
						if (SetofChoosed_Pixel[i][j]) {
							image_show.SetPixel(i, j, image_now.GetPixel(i, j));
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

void CLKIDoc::Set_choosesetByCoordination(CRect chooserect)
{
	POSITION pos = GetFirstViewPosition();
	GetNextView(pos);
	CLKIView *view_tmp = (CLKIView*)GetNextView(pos);

	switch (choose_tool) {
	case CHOOSE_PIXEL:
	case CHOOSE_RECTANGLE: {
		if (choose_style == CHOOSE_INTERSECT) {
			for (int i = -181; i < 256; i++) {
				for (int j = -181; j < 256; j++) {
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
			for (int i = -181; i < 256; i++) {
				for (int j = -181; j < 256; j++) {
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

	// TODO: 在此添加重新初始化代码
	// (SDI 文档将重用该文档)
	SetTitle(L"图像处理");

	return TRUE;
}

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


void CLKIDoc::OnButton3d()
{
	// TODO: 在此添加命令处理程序代码
	if (!image_now.IsNull()) {
		show_level = ON_3D_COORDINATION;
		UpdateAllViews(NULL);
	}
	else {
		AfxMessageBox(L"未打开图片!");
	}
}


void CLKIDoc::OnButtonZoomImageInit()
{
	// TODO: 在此添加命令处理程序代码
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


void CLKIDoc::OnButtonRGBGraph_ZoomInit()
{
	// TODO: 在此添加命令处理程序代码
	if (image_show.IsNull()) {
		AfxMessageBox(L"未打开文件");
		return;
	}
	CRect rect;
	POSITION pos = GetFirstViewPosition();
	GetNextView(pos);
	CLKIView *view_tmp = (CLKIView*)GetNextView(pos);
	view_tmp->GetClientRect(rect);
	if (rect.Height() > rect.Width()) {
		view_tmp->m_set.coordination.scale = (rect.Width() * 14 / 19.0 - 40) / 255.0;
	}
	else {
		view_tmp->m_set.coordination.scale = (rect.Height() * 14 / 19.0 - 40) /255.0;
	}
	view_tmp->m_set.coordination.O_x = rect.Width() * 5 / 19.0;
	view_tmp->m_set.coordination.O_y = rect.Height() * 14 / 19.0;
	view_tmp->Invalidate();
}


void CLKIDoc::OnButtonZoomOut_Image()
{
	// TODO: 在此添加命令处理程序代码
	if (image_show.IsNull()) {
		return;
	}
	ZoomSize += 0.1;

	POSITION pos = GetFirstViewPosition();
	CView *view_tmp = (CView*)GetNextView(pos);
	show_level = ON_ZOOM_INOUT;
	view_tmp->Invalidate();
}


void CLKIDoc::OnButtonZoomOut_Graph()
{
	// TODO: 在此添加命令处理程序代码
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


void CLKIDoc::OnButtonZoomIn_Image()
{
	// TODO: 在此添加命令处理程序代码
	if (image_show.IsNull()) {
		return;
	}
	ZoomSize -= 0.1;

	POSITION pos = GetFirstViewPosition();
	CView *view_tmp = (CView*)GetNextView(pos);
	show_level = ON_ZOOM_INOUT;
	view_tmp->Invalidate();
}



void CLKIDoc::OnButtonZoomIn_Graph()
{
	// TODO: 在此添加命令处理程序代码
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


void CLKIDoc::OnFileSave()
{
	// TODO: 在此添加命令处理程序代码
	if (image_now.IsNull()) {
		AfxMessageBox(L"未打开文件");
		return;
	}
	image_now.Save(file_path);
}



void CLKIDoc::OnFileSaveAs()
{
	// TODO: 在此添加命令处理程序代码
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




void CLKIDoc::OnButtonInitcoordination_Image()
{
	// TODO: 在此添加命令处理程序代码
	if (image_now.IsNull()) {
		AfxMessageBox(L"未打开文件");
		return;
	}

	POSITION pos = GetFirstViewPosition();
	CLeftView *view_tmp = (CLeftView*)GetNextView(pos);
	view_tmp->draw_LT = (0, 0);
	view_tmp->Invalidate();
}


void CLKIDoc::OnButtonInitcoordination_RGBGraph()
{
	// TODO: 在此添加命令处理程序代码
	if (image_now.IsNull()) {
		AfxMessageBox(L"未打开文件");
		return;
	}
	//自适应显示
	POSITION pos = GetFirstViewPosition();
	GetNextView(pos);
	CLKIView *view_tmp = (CLKIView*)GetNextView(pos);
	view_tmp->GetClientRect(GraphClient);
	view_tmp->m_set.coordination.O_x = GraphClient.Width() * 5 / 19.0;
	view_tmp->m_set.coordination.O_y = GraphClient.Height() * 14 / 19.0;
	view_tmp->Invalidate();
}


void CLKIDoc::OnCheck0()
{
	// TODO: 在此添加命令处理程序代码
	mouse_level = VIEWING;
}


void CLKIDoc::OnCheck1()
{
	// TODO: 在此添加命令处理程序代码
	if (image_now.IsNull()) {
		AfxMessageBox(L"请先加载图片！");
		return;
	}
	mouse_level = MOVING;
}


void CLKIDoc::OnCheck2()
{
	// TODO: 在此添加命令处理程序代码
	if (image_now.IsNull()) {
		AfxMessageBox(L"请先加载图片！");
		return;
	}
	mouse_level = CHOOSING;
	choose_tool = CHOOSE_RECTANGLE;
	choose_style = CHOOSE_UNION;
}


void CLKIDoc::OnUpdateCheck0(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	if (mouse_level == VIEWING) pCmdUI->SetCheck();
	else pCmdUI->SetCheck(0);
}


void CLKIDoc::OnUpdateCheck1(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	if (mouse_level == MOVING) pCmdUI->SetCheck();
	else pCmdUI->SetCheck(0);
}


void CLKIDoc::OnUpdateCheck2(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	if (mouse_level == CHOOSING) pCmdUI->SetCheck();
	else pCmdUI->SetCheck(0);
}


void CLKIDoc::OnButtonSetC1()
{
	// TODO: 在此添加命令处理程序代码
	CColorDialog dlg;
	dlg.m_cc.Flags |= CC_RGBINIT | CC_FULLOPEN;
	if (IDOK == dlg.DoModal())
	{
		C1 = dlg.m_cc.rgbResult;    //将dlg.m_cc.rgbResult获取到的颜色对话框中的颜色保存到变量m_clr中
	}
}


void CLKIDoc::OnButtonSetC2()
{
	// TODO: 在此添加命令处理程序代码
	CColorDialog dlg;
	dlg.m_cc.Flags |= CC_RGBINIT | CC_FULLOPEN;
	if (IDOK == dlg.DoModal())
	{
		C2 = dlg.m_cc.rgbResult;    //将dlg.m_cc.rgbResult获取到的颜色对话框中的颜色保存到变量m_clr中
	}
}


void CLKIDoc::OnCheckUnion()
{
	// TODO: 在此添加命令处理程序代码
	choose_style = CHOOSE_UNION;
	mouse_level = CHOOSING;
}


void CLKIDoc::OnUpdateCheckUnion(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	if (choose_style == CHOOSE_UNION && mouse_level == CHOOSING) pCmdUI->SetCheck();
	else pCmdUI->SetCheck(0);
}

void CLKIDoc::OnCheckDeduce()
{
	// TODO: 在此添加命令处理程序代码
	choose_style = CHOOSE_DEDUCE;
	mouse_level = CHOOSING;
}


void CLKIDoc::OnUpdateCheckDeduce(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	if (choose_style == CHOOSE_DEDUCE && mouse_level == CHOOSING) pCmdUI->SetCheck();
	else pCmdUI->SetCheck(0);
}


void CLKIDoc::OnCheckIntersect()
{
	// TODO: 在此添加命令处理程序代码
	choose_style = CHOOSE_INTERSECT;
	mouse_level = CHOOSING;
}


void CLKIDoc::OnUpdateCheckIntersect(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	if (choose_style == CHOOSE_INTERSECT && mouse_level == CHOOSING) pCmdUI->SetCheck();
	else pCmdUI->SetCheck(0);
}


void CLKIDoc::OnCheckRectChoose()
{
	// TODO: 在此添加命令处理程序代码
	choose_tool = CHOOSE_RECTANGLE;
	mouse_level = CHOOSING;
}


void CLKIDoc::OnUpdateCheckRectChoose(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	if (choose_tool == CHOOSE_RECTANGLE && mouse_level == CHOOSING) pCmdUI->SetCheck();
	else pCmdUI->SetCheck(0);
}


void CLKIDoc::OnCheckPixelChoose()
{
	// TODO: 在此添加命令处理程序代码
	choose_tool = CHOOSE_PIXEL;
	mouse_level = CHOOSING;
}


void CLKIDoc::OnUpdateCheckPixelChoose(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	if (choose_tool == CHOOSE_PIXEL && mouse_level == CHOOSING) pCmdUI->SetCheck();
	else pCmdUI->SetCheck(0);
}


void CLKIDoc::OnCheckCircleChoose()
{
	// TODO: 在此添加命令处理程序代码
	choose_tool = CHOOSE_CIRCLE;
	mouse_level = CHOOSING;
}


void CLKIDoc::OnUpdateCheckCircleChoose(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	if (choose_tool == CHOOSE_CIRCLE && mouse_level == CHOOSING) pCmdUI->SetCheck();
	else pCmdUI->SetCheck(0);
}


void CLKIDoc::OnButtonFullFill()
{
	// TODO: 在此添加命令处理程序代码
	if (image_now.IsNull()) {
		AfxMessageBox(L"未打开图片！");
		return;
	}
	for (int i = 0; i < image_now.GetHeight(); i++) {
		for (int j = 0; j < image_now.GetWidth(); j++) {
			if (SetofChoosed_Pixel[j][i]) image_now.SetPixel(j, i, C2);
		}
	}

	POSITION pos = GetFirstViewPosition();
	CLeftView *view_tmp = (CLeftView*)GetNextView(pos);
	view_tmp->Invalidate();
}


void CLKIDoc::OnButtonCancelChoose()
{
	// TODO: 在此添加命令处理程序代码
	memset(SetofChoosed_Pixel, 0, sizeof(SetofChoosed_Pixel));
	HDC hdc = image_show.GetDC(); //获取目标绘制区的hDC
	image_now.Draw(hdc, 0, 0);
	image_show.ReleaseDC();     //拿到的DC要释放，不释放第二次刷新会崩溃
	POSITION pos = GetFirstViewPosition();
	CLeftView *view_tmp = (CLeftView*)GetNextView(pos);
	view_tmp->Invalidate();
}

