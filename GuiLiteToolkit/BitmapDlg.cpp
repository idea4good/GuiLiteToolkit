#include "stdafx.h"
#include "BitmapDlg.h"
#include "resource.h"
#include <fstream>

BEGIN_MESSAGE_MAP(BitmapDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, &BitmapDlg::OnBnClickedGenerate)
	ON_BN_CLICKED(IDC_BUTTON2, &BitmapDlg::OnBnClickedImport)
END_MESSAGE_MAP()

void BitmapDlg::OnBnClickedImport()
{
	CFileDialog fileDlg(TRUE);
	fileDlg.DoModal();
	m_fileName = fileDlg.GetFileName();
	m_filePath = fileDlg.GetPathName();
}

void BitmapDlg::OnBnClickedGenerate()
{
	if (m_fileName.IsEmpty() && m_filePath.IsEmpty())
	{
		return;
	}

	std::fstream file(m_filePath, std::ios::in | std::ios::binary);
	int size = file.tellg();
}