
// FDTD.h: главный файл заголовка для приложения PROJECT_NAME
//

#pragma once

#ifndef __AFXWIN_H__
	#error "включить stdafx.h до включения этого файла в PCH"
#endif

#include "resource.h"		// основные символы


// CFDTDApp:
// Сведения о реализации этого класса: FDTD.cpp
//

class CFDTDApp : public CWinApp
{
public:
	CFDTDApp();

// Переопределение
public:
	virtual BOOL InitInstance();

// Реализация

	DECLARE_MESSAGE_MAP()
};

extern CFDTDApp theApp;
