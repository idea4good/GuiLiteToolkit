#pragma once
#include <afxwin.h>
class BitmapDlg :
	public CDialog
{
public:
	afx_msg void OnBnClickedImport();
	afx_msg void OnBnClickedGenerate();
	DECLARE_MESSAGE_MAP()
private:
	CString m_fileName;
	CString m_filePath;
};

