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

// LeftView.cpp: CLeftView 类的实现
//

#include "stdafx.h"
#include "LKI.h"

#include "LKIDoc.h"
#include "LeftView.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CLeftView

IMPLEMENT_DYNCREATE(CLeftView, CView)

BEGIN_MESSAGE_MAP(CLeftView, CView)

	ON_WM_ERASEBKGND()
	ON_COMMAND(ID_FILE_OPEN, &CLeftView::OnFileOpen)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()


// CLeftView 构造/析构

CLeftView::CLeftView()
{
	// TODO: 在此处添加构造代码
	draw_LT = (0, 0);
}

CLeftView::~CLeftView()
{
}

void CLeftView::OnDraw(CDC * pDC)
{
	// 获取文档
	CLKIDoc* pDoc = GetDocument();

	switch (pDoc->show_level)
	{
	case ON_OPEN_FILE_DRAW:
	case ON_3D_COORDINATION:
	{
		GetClientRect(&pDoc->ImageClient);
		//自适应显示
		if (pDoc->ImageClient.bottom > 250) pDoc->ImageClient.bottom -= 25;
		if (pDoc->ImageClient.right > 250) pDoc->ImageClient.right -= 25;
		if (pDoc->ImageClient.Height() / (float)pDoc->ImageClient.Width() <
			pDoc->image_show.GetHeight() / (float)pDoc->image_show.GetWidth()) {
			pDoc->ZoomSize = pDoc->ImageClient.Height() / (float)pDoc->image_show.GetHeight();
		}
		else {
			pDoc->ZoomSize = pDoc->ImageClient.Width() / (float)pDoc->image_show.GetWidth();
		}
		if (!pDoc->image_show.IsNull()) {
			pDoc->image_show.Draw(pDC->m_hDC, draw_LT.x, draw_LT.y, int(pDoc->image_show.GetWidth() * pDoc->ZoomSize),
				int(pDoc->image_show.GetHeight() * pDoc->ZoomSize));
		}
		break;
	}
	case ON_MOVE:
	case ON_MOVING:
	case ON_ZOOM_INOUT: 
	{
		if (!pDoc->image_show.IsNull()) {
			pDoc->image_show.Draw(pDC->m_hDC, draw_LT.x, draw_LT.y, int(pDoc->image_show.GetWidth() * pDoc->ZoomSize),
				int(pDoc->image_show.GetHeight() * pDoc->ZoomSize));
		}
		break;
	}
	case ON_CHOOSING: {
		if (!pDoc->image_show.IsNull()) {
			pDoc->image_show.Draw(pDC->m_hDC, draw_LT.x, draw_LT.y, int(pDoc->image_show.GetWidth() * pDoc->ZoomSize),
				int(pDoc->image_show.GetHeight() * pDoc->ZoomSize));
		}

		CClientDC dc(this); //获取设备句柄  
		dc.SelectStockObject(NULL_BRUSH);//不使用画刷 
		if (pDoc->choose_tool == CHOOSE_RECTANGLE) {
			CPoint tmp1,tmp2;
			if (begin_move.x < end_move.x) { tmp1.x = begin_move.x;  tmp2.x = end_move.x; }
			else { tmp2.x = begin_move.x;  tmp1.x = end_move.x; }
			if (begin_move.y < end_move.y) { tmp1.y = begin_move.y;  tmp2.y = end_move.y; }
			else { tmp2.y = begin_move.y;  tmp1.y = end_move.y; }
			dc.Rectangle(CRect(tmp1, tmp2));
		}
		else if (pDoc->choose_tool == CHOOSE_CIRCLE) {
			CPoint tmp1, tmp2;
			if (begin_move.x < end_move.x) { tmp1.x = begin_move.x;  tmp2.x = end_move.x; }
			else { tmp2.x = begin_move.x;  tmp1.x = end_move.x; }
			if (begin_move.y < end_move.y) { tmp1.y = begin_move.y;  tmp2.y = end_move.y; }
			else { tmp2.y = begin_move.y;  tmp1.y = end_move.y; }
			if (tmp2.x - tmp1.x != tmp2.y - tmp1.y) {
				tmp2.y = tmp1.y + tmp2.x - tmp1.x;
			}
			dc.Ellipse(CRect(tmp1, tmp2));
		}
		break;
	}
	case ON_CHOOSED: {
		if (!pDoc->image_show.IsNull()) {
			pDoc->image_show.Draw(pDC->m_hDC, draw_LT.x, draw_LT.y, int(pDoc->image_show.GetWidth() * pDoc->ZoomSize),
				int(pDoc->image_show.GetHeight() * pDoc->ZoomSize));
		}
	}
	default:
		break;
	}
	
}

BOOL CLeftView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改 CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CLeftView 诊断

#ifdef _DEBUG
void CLeftView::AssertValid() const
{
	CView::AssertValid();
}

void CLeftView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CLKIDoc* CLeftView::GetDocument() // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CLKIDoc)));
	return (CLKIDoc*)m_pDocument;
}
#endif //_DEBUG


// CLeftView 消息处理程序

BOOL CLeftView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	// 主要是为了设置子窗体默认的背景色
	// 背景色由文档成员变量m_refColorBKG指定

	// 获取文档
	CLKIDoc* pDoc = GetDocument();

	// 创建一个Brush
	CBrush brush(pDoc->m_ColorBKG);

	// 保存以前的Brush
	CBrush* pOldBrush = pDC->SelectObject(&brush);

	// 获取重绘区域
	CRect rectClip;
	pDC->GetClipBox(&rectClip);

	// 重绘
	pDC->PatBlt(rectClip.left, rectClip.top, rectClip.Width(), rectClip.Height(), PATCOPY);

	// 恢复以前的Brush
	pDC->SelectObject(pOldBrush);

	// 返回
	return TRUE;
}


void CLeftView::OnFileOpen()
{
	// TODO: 在此添加命令处理程序代码
	
	// 获取文档
	CLKIDoc* pDoc = GetDocument();
	CString strFilter; 
	CSimpleArray<GUID> aguidFileTypes; 
	HRESULT hResult;
	hResult = pDoc->image_now.GetExporterFilterString(strFilter, aguidFileTypes, _T("All Image Files")); 
	if (FAILED(hResult)) { MessageBox(_T("GetExporterFilter调用失败！")); return; }
	CFileDialog dlg(TRUE, NULL, NULL, OFN_FILEMUSTEXIST, strFilter); 
	if (IDOK != dlg.DoModal()) return;
	else 	{
		if (GetDocument()->image_now.IsNull()) {
			pDoc->show_level = ON_OPEN_FILE_DRAW;
			pDoc->file_path = dlg.GetPathName();
			GetDocument()->OnOpenDocument(pDoc->file_path);
		}		
		else {
			// 判断当前图像是否已经被改动
			if (GetDocument()->IsModified())
			{
				// 提示用户该操作将丢失所有当前的修改
				if (MessageBox(NULL, L"重新打开图像将丢失所有改动！是否继续？", MB_ICONQUESTION | MB_YESNO) == IDNO)
				{
					// 用户取消操作，直接返回
					return;
				}
			}
			pDoc->show_level = ON_OPEN_FILE_DRAW;
			pDoc->file_path = dlg.GetPathName();
			GetDocument()->OnOpenDocument(pDoc->file_path);
		}
	}
}


void CLeftView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	end_move = point;
	mouse_L_down = true;
}


void CLeftView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CLKIDoc* pDoc = GetDocument();
	mouse_L_down = false;
	if (pDoc->image_now.IsNull()) {
		return;
	}

	switch (pDoc->mouse_level)
	{
	case MOVING: {
		pDoc->show_level = ON_MOVE;
		end_move = point;
		draw_LT.x += (end_move.x - begin_move.x) / pDoc->ZoomSize;
		draw_LT.y += (end_move.y - begin_move.y) / pDoc->ZoomSize;
		//pDoc->mouse_level = VIEWING;
		Invalidate();
		break;
	}
	case CHOOSING: {
		begin_move = point;
		CPoint tmp1, tmp2;
		begin_move.x = (begin_move.x - draw_LT.x) / pDoc->ZoomSize;
		begin_move.y = (begin_move.y - draw_LT.y) / pDoc->ZoomSize;
		end_move.x = (end_move.x - draw_LT.x) / pDoc->ZoomSize;
		end_move.y = (end_move.y - draw_LT.y) / pDoc->ZoomSize;
		if (pDoc->choose_tool == CHOOSE_PIXEL) {
			tmp1 = begin_move;
			tmp2 = begin_move;
		}
		else {
			if (begin_move.x < end_move.x) { tmp1.x = begin_move.x;  tmp2.x = end_move.x; }
			else { tmp2.x = begin_move.x;  tmp1.x = end_move.x; }
			if (begin_move.y < end_move.y) { tmp1.y = begin_move.y;  tmp2.y = end_move.y; }
			else { tmp2.y = begin_move.y;  tmp1.y = end_move.y; }
		}
		if (pDoc->choose_tool == CHOOSE_CIRCLE && tmp2.x - tmp1.x != tmp2.y - tmp1.y) {
			tmp2.y = tmp1.y + tmp2.x - tmp1.x;
		}
		
		CRect chooseRect(tmp1, tmp2);
		//设置选择集
		pDoc->Set_chooseset(chooseRect);

		pDoc->show_level = ON_CHOOSED;
		Invalidate();

		break;
	}
	default:
		break;
	}

}


void CLeftView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CString str;
	CMFCRibbonBaseElement * pElement;
	str.Format(_T("鼠标位置：x:%d, y:%d"), point.x, point.y);
	pElement = ((CMainFrame*)AfxGetMainWnd())->m_wndStatusBar.FindElement(ID_STATUSBAR_PANE1);
	pElement->SetText(str);
	pElement->Redraw();
	
	CLKIDoc* pDoc = GetDocument();

	if (pDoc->image_now.IsNull()) {
		return;
	}
	switch (pDoc->mouse_level)
	{
	case MOVING: {
		if (mouse_L_down) {
			begin_move = end_move;
			pDoc->show_level = ON_MOVING;
			end_move = point;
			draw_LT.x += (end_move.x - begin_move.x) / pDoc->ZoomSize;
			draw_LT.y += (end_move.y - begin_move.y) / pDoc->ZoomSize;
			Invalidate();
		}
		break;
	}
	case CHOOSING: {
		if (mouse_L_down && pDoc->choose_tool != CHOOSE_PIXEL) {
			begin_move = point;
			pDoc->show_level = ON_CHOOSING;
			Invalidate();
		}
		break;
	}
	default:
		break;
	}

}
