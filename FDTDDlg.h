
// FDTDDlg.h: файл заголовка
//

#pragma once
#include "fdtd2d.h"
#include "CDrawer.h"
#include "vector"


// Диалоговое окно CFDTDDlg
class CFDTDDlg : public CDialog
{
// Создание
public:
	CFDTDDlg(CWnd* pParent = NULL);	// стандартный конструктор

// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FDTD_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// поддержка DDX/DDV

	CWnd * PicWnd;
	CDC * PicDc;
	CRect Pic;
	HGLRC glRC;
// Реализация
protected:
	HICON m_hIcon;

	// Созданные функции схемы сообщений
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

	


	CRect rect;
	CDC* pDC;
	

	
	double angleX, angleY, zoom;
	CPoint oldPoint;
	
public:
	void Calculate();
	afx_msg void OnBnClickedOk();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	fdtd2d *fdtd;
	CDrawer drawer;
	std::vector<std::vector<Point3D>> points;
	HANDLE hThreadCalculate;
	DWORD pdwThreadCalculate;
	double stepTime;
	afx_msg void OnBnClickedApply();
	double x1;
	double x2;
	double y1;
	double y2;
	afx_msg void OnBnClickedStop();
};
DWORD WINAPI ThreadCalculate(PVOID param);