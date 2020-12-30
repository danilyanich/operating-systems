
// OS3MFC.h: главный файл заголовка для приложения PROJECT_NAME
//

#pragma once

#ifndef __AFXWIN_H__
	#error "включить pch.h до включения этого файла в PCH"
#endif

#include "resource.h"		// основные символы


// COS3MFCApp:
// Сведения о реализации этого класса: OS3MFC.cpp
//

class COS3MFCApp : public CWinApp
{
public:
	COS3MFCApp();

// Переопределение
public:
	virtual BOOL InitInstance();

// Реализация

	DECLARE_MESSAGE_MAP()
};

extern COS3MFCApp theApp;
