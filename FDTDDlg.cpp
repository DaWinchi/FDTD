
// FDTDDlg.cpp: файл реализации
//

#include "stdafx.h"
#include "FDTD.h"
#include "FDTDDlg.h"
#include "afxdialogex.h"

#include <gl/GLU.h>
#include <gl/GL.h>
#include "glut.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Диалоговое окно CFDTDDlg



CFDTDDlg::CFDTDDlg(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_FDTD_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CFDTDDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CFDTDDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CFDTDDlg::OnBnClickedOk)
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	ON_WM_DESTROY()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// Обработчики сообщений CFDTDDlg

BOOL CFDTDDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Задает значок для этого диалогового окна.  Среда делает это автоматически,
	//  если главное окно приложения не является диалоговым
	SetIcon(m_hIcon, TRUE);			// Крупный значок
	SetIcon(m_hIcon, FALSE);		// Мелкий значок

	// TODO: добавьте дополнительную инициализацию
	CDC* dc = GetDlgItem(IDC_GRAPHICS)->GetDC();
	GetDlgItem(IDC_GRAPHICS)->GetClientRect(&rect);
	drawer.InitiateOPGL(rect, dc);

	angleX = 0;
	angleY = 0;
	zoom = 1;
	drawer.angleX = &angleX;
	drawer.angleY = &angleY;
	drawer.zoom = &zoom;
	drawer.points = &points;
	oldPoint.x = 0;
	oldPoint.y = 0;

	SetTimer(1, 30, NULL);
	return TRUE;  // возврат значения TRUE, если фокус не передан элементу управления
}

// При добавлении кнопки свертывания в диалоговое окно нужно воспользоваться приведенным ниже кодом,
//  чтобы нарисовать значок.  Для приложений MFC, использующих модель документов или представлений,
//  это автоматически выполняется рабочей областью.

void CFDTDDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // контекст устройства для рисования

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Выравнивание значка по центру клиентского прямоугольника
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Нарисуйте значок
		dc.DrawIcon(x, y, m_hIcon);


	}
	else
	{

		CDialog::OnPaint();
	}
}

// Система вызывает эту функцию для получения отображения курсора при перемещении
//  свернутого окна.
HCURSOR CFDTDDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}






void CFDTDDlg::OnBnClickedOk()
{

	Calculate();
	drawer.Draw();

}


void CFDTDDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	int dX = (point.x - oldPoint.x);
	int dY = (point.y - oldPoint.y);
	bool IsInRect = (point.x > rect.left) && (point.x < rect.right) && (point.y > rect.top) && (point.y < rect.bottom);

	oldPoint.x = point.x;
	oldPoint.y = point.y;
	if (IsInRect)
	{
		if (nFlags & MK_LBUTTON)
		{
			angleX += 0.5*dY;
			angleY += 0.5*dX;
		}
	}
	CDialog::OnMouseMove(nFlags, point);
}


BOOL CFDTDDlg::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	bool IsInRect = (pt.x > rect.left) && (pt.x < rect.right) && (pt.y > rect.top) && (pt.y < rect.bottom);

	if (IsInRect)
	{
		zoom += zDelta / 1000.;
	}
	return CDialog::OnMouseWheel(nFlags, zDelta, pt);
}

void CFDTDDlg::Calculate()
{
	//Гиперболический параболоид
	points.clear();
	for (double i = -14; i < 14; i += 0.1)
	{
		Point3D pt;
		pt.x = i;
		std::vector <Point3D> pts;
		for (double j = -14; j < 14; j += 0.1)
		{
			pt.y = j;
			pt.z = 1*cos(i*j);
			pts.push_back(pt);
		}
		points.push_back(pts);
	}
}

void CFDTDDlg::OnDestroy()
{
	CDialog::OnDestroy();

	drawer.~CDrawer();
}


void CFDTDDlg::OnTimer(UINT_PTR nIDEvent)
{
	drawer.Draw();
	CDialog::OnTimer(nIDEvent);
}
