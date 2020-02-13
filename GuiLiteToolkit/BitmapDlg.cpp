#include "stdafx.h"
#include "BitmapDlg.h"
#include "resource.h"
#include <fstream>
#include <string>
#include <algorithm>

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

	HBITMAP hb = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), m_filePath, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	if (NULL == hb)
	{
		MessageBox(L"24 bits bitmap file only", L"✖✖✖");
		m_fileName.Empty();
		m_filePath.Empty();
		return;
	}

	CStatic* p_picture = (CStatic*)GetDlgItem(IDC_STATIC);
	p_picture->ModifyStyle(0xF, SS_BITMAP, SWP_NOSIZE);
	p_picture->SetBitmap(hb);
}

void BitmapDlg::OnBnClickedGenerate()
{
	if (m_fileName.IsEmpty() || m_filePath.IsEmpty())
	{
		return;
	}

	std::ifstream file(m_filePath, std::ios::in | std::ios::binary | std::ios::ate);
	if (!file.is_open())
	{
		return;
	}

	file.seekg(0, std::ios::beg);
	file.read((char*)&m_bm_file_header, sizeof(m_bm_file_header));
	file.read((char*)&m_bm_info_header, sizeof(m_bm_info_header));

	char* pixel_data = new char[m_bm_info_header.biSizeImage];
	file.read(pixel_data, m_bm_info_header.biSizeImage);
	if (saveFile(pixel_data)) 
	{
		MessageBox(L"Generate bitmap cpp file success✌", L"\\^o^/");
	}
	
	file.close();
	delete[] pixel_data;
}

static char* include = "#include \"GuiLite.h\"\n\n";
bool BitmapDlg::saveFile(char* pixel_data)
{
	int width = m_bm_info_header.biWidth;
	int height = m_bm_info_header.biHeight;
	int byteCount = m_bm_info_header.biBitCount / 8;
	if (byteCount != 3)
	{
		MessageBox(L"24 bits bitmap file only", L"✖✖✖");
		return FALSE;
	}

	std::string strFileName = CT2A(m_fileName);
	std::replace(strFileName.begin(), strFileName.end(), '.', '_');		//replace '.' with '_'
	std::replace(strFileName.begin(), strFileName.end(), ' ', '_');		//replace ' ' with '_'
	std::fstream file(std::string("./") + strFileName + ".cpp", std::ios::trunc | std::ios::out | std::ios::binary);
	file.write(include, strlen(include));

	std::string data_type = "static const unsigned short ";
	std::string data_name = "raw_data";
	std::string define_bitmap_data = data_type + data_name + "[] = {\n";

	unsigned int bytesPerLine = ((width * byteCount + 3) & (~3));
	for (int y = (height - 1); y >= 0; y--)
	{
		define_bitmap_data += "    ";
		char* data = pixel_data + (y * bytesPerLine);
		for (int x = 0; x < width; x++)
		{		
			unsigned int rgb = *(unsigned int*)data;
			define_bitmap_data += std::to_string((((((unsigned int)(rgb)) & 0xFF) >> 3) | ((((unsigned int)(rgb)) & 0xFC00) >> 5) | ((((unsigned int)(rgb)) & 0xF80000) >> 8)));
			define_bitmap_data += ", ";
			data += byteCount;
		}
		define_bitmap_data += "\n";
	}
	define_bitmap_data += "};\n";
	file.write(define_bitmap_data.c_str(), define_bitmap_data.length());

	data_type = "const BITMAP_INFO ";
	std::string define_font_info = "extern " + data_type + strFileName + ";\n";
	define_font_info += data_type + strFileName + " ={\n";
	define_font_info += "    " + std::to_string(width) + ",\n";
	define_font_info += "    " + std::to_string(height) + ",\n";
	define_font_info += "    " + std::to_string(16) + ",\n";
	define_font_info += "    (unsigned short*)raw_data\n};\n";
	file.write(define_font_info.c_str(), define_font_info.length());

	file.close();
	return TRUE;
}
