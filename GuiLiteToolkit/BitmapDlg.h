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
	bool saveFile(char* pixel_data);
	CString m_fileName;
	CString m_filePath;
	BITMAPFILEHEADER m_bm_file_header;
	BITMAPINFOHEADER m_bm_info_header;
};

