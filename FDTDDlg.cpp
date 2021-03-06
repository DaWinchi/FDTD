
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

CRITICAL_SECTION cs;



CFDTDDlg::CFDTDDlg(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_FDTD_DIALOG, pParent)
	, stepTime(0.5)
	, x1(0.33)
	, x2(0.66)
	, y1(0.33)
	, y2(0.66)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CFDTDDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_STEPTIME, stepTime);
	DDX_Text(pDX, IDC_EDIT1, x1);
	DDV_MinMaxDouble(pDX, x1, 0.05, 0.95);
	DDX_Text(pDX, IDC_EDIT3, x2);
	DDV_MinMaxDouble(pDX, x2, 0.05, 0.95);
	DDX_Text(pDX, IDC_EDIT2, y1);
	DDV_MinMaxDouble(pDX, y1, 0.05, 0.95);
	DDX_Text(pDX, IDC_EDIT4, y2);
	DDV_MinMaxDouble(pDX, y2, 0.05, 0.95);
}

BEGIN_MESSAGE_MAP(CFDTDDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CFDTDDlg::OnBnClickedOk)
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_APPLY, &CFDTDDlg::OnBnClickedApply)
	ON_BN_CLICKED(IDC_STOP, &CFDTDDlg::OnBnClickedStop)
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
	InitializeCriticalSection(&cs);
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

	fdtd = new fdtd2d(1, 300, 1, 300, 10, &x1, &x2, &y1, &y2);
	points.resize(fdtd->yee.size());
	for (int i = 0; i < points.size(); ++i)
	{
		points[i].resize(points.size());
	}
	
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
	delete fdtd;
	fdtd = new fdtd2d(1, 300, 1, 300, 10, &x1, &x2, &y1, &y2);
	SetTimer(1, 30, NULL);
	//hThreadCalculate = CreateThread(
	//	NULL,		// дескриптор защиты
	//	0,			// начальный размер стека ( Если это значение нулевое, новый поток использует по умолчанию размер стека исполняемой программы)
	//	(LPTHREAD_START_ROUTINE)ThreadCalculate,	 // функция потока
	//	this,		// параметр потока 
	//	0,			//oпции создания(здесь можно отложить запуск выполнения потока. Для запуска потока сразу же, передаём 0.)
	//	&pdwThreadCalculate);// идентификатор потока (указатель на переменную, куда будет сохранён идентификатор потока)
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
	KillTimer(1);
	TerminateThread(hThreadCalculate, pdwThreadCalculate);
	drawer.~CDrawer();
}


void CFDTDDlg::OnTimer(UINT_PTR nIDEvent)
{
	EnterCriticalSection(&cs);
	fdtd->GenNextStep(stepTime);

	for (int i = 0; i< 300; i++)
	{
		for (int j = 0; j < 300; j++)
		{
			double x = i*0.02 - 3;
			double y = j*0.02 - 3;
			double z1 = fdtd->yee[i][j].ez;
			points[i][j].x = x;
			points[i][j].y = y;
			points[i][j].z = z1;
		}
	}
	drawer.Draw();
	LeaveCriticalSection(&cs);
	
	CDialog::OnTimer(nIDEvent);
}

DWORD WINAPI ThreadCalculate(PVOID param)
{
	CFDTDDlg *dlg = (CFDTDDlg*)param;
	while (true)
	{
		EnterCriticalSection(&cs);
		dlg->fdtd->GenNextStep(0.1);

		float max = 0.1, min = 1000.;
		for (int i = 0; i< 200; i++)
		{
			for (int j = 0; j < 200; j++)
			{
				double x = i*0.02 - 2;
				double y = j*0.02 - 2;
				double z1 = dlg->fdtd->yee[i][j].ez;
				if (max < z1) max = z1;
				if (min > z1) min = z1;
				dlg->points[i][j].x = x;
				dlg->points[i][j].y = y;
				dlg->points[i][j].z = z1;
			}
		}
		LeaveCriticalSection(&cs);
	}
	return 0;
}


void CFDTDDlg::OnBnClickedApply()
{
	UpdateData(TRUE);
}


void CFDTDDlg::OnBnClickedStop()
{
	KillTimer(1);
}
