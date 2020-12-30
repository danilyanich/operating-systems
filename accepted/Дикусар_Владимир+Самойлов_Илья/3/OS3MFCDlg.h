
// OS3MFCDlg.h: файл заголовка
//

#include <urlmon.h>
#include <urlmon.h>

// Диалоговое окно COS3MFCDlg
class COS3MFCDlg : public CDialogEx
{
// Создание
public:
	COS3MFCDlg(CWnd* pParent = nullptr);	// стандартный конструктор

// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_OS3MFC_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// поддержка DDX/DDV


// Реализация
protected:
	HICON m_hIcon;

	// Созданные функции схемы сообщений
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
private:
	CString URL;
public:
	afx_msg void OnBnClickedOk();
};
