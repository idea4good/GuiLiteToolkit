#include "stdafx.h"
#include "FontDlg.h"
#include "resource.h"

CFontLattice::CFontLattice()
{
	utf_8_code = width = 0;
	p_data = 0;
}

CFontLattice::~CFontLattice()
{
	if (p_data)
	{
		free(p_data);
	}
	if (p_compressed_data)
	{
		free(p_compressed_data);
	}
}

BEGIN_MESSAGE_MAP(CFontDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, &CFontDlg::OnBnClickedFont)
	ON_BN_CLICKED(IDC_BUTTON2, &CFontDlg::OnBnClickedGenerate)
END_MESSAGE_MAP()

static wchar_t default_input[] = L"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ 0123456789(_+=./-\:;@%!#',?)<>▲▼";
BOOL CFontDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	CWnd* p_edit = GetDlgItem(IDC_EDIT1);
	p_edit->SetWindowTextW(default_input);
	return TRUE;
}

void CFontDlg::OnBnClickedFont()
{
	// TODO: Add your control notification handler code here
	memset(mInput, 0, sizeof(mInput));
	GetDlgItemText(IDC_EDIT1, mInput, sizeof(mInput) / sizeof(wchar_t));
	if (0 == wcslen(mInput))
	{
		MessageBox(L"Input characters✍", L"o(╯□╰)o");
		return;
	}

	CWnd* generate_bt = GetDlgItem(IDC_BUTTON2);
	CWnd* font_bt = GetDlgItem(IDC_BUTTON1);
	generate_bt->EnableWindow(false);
	font_bt->EnableWindow(false);

	LOGFONT logFont;
	CFontDialog fdlg;
	if (IDOK == fdlg.DoModal())
	{
		fdlg.GetCurrentFont(&logFont);
		if (0 != wcslen(logFont.lfFaceName))
		{
			mCurrentFontLatticeMap.clear();
			mCurrentFontName.clear();
			mCurrentFontHeight = 0;
			GetStringInfo(mInput, logFont);
		}
	}
	generate_bt->EnableWindow(true);
	font_bt->EnableWindow(true);
}

int CFontDlg::GetStringInfo(wchar_t* str, LOGFONT& logFont)
{
	CClientDC dc(this);
	int length = wcslen(str);
	for (int i = 0; i < length; i++)
	{
		GetCharInfo(dc, str[i], logFont);
	}
	return 0;
}

int CFontDlg::GetCharInfo(CClientDC& dc, wchar_t character, LOGFONT& logFont)
{
	wchar_t characterBuffer[2] = { character, 0 };
	CFont font;
	font.CreateFontIndirectW(&logFont);
	dc.SelectObject(&font);

	dc.FillRect(&CRect{ 0, 0, mCurrentFontHeight, mCurrentFontHeight }, &CBrush(RGB(255, 255, 255)));
	dc.TextOut(0, 0, characterBuffer, 1);

	SIZE size;
	GetTextExtentPoint(dc, characterBuffer, 1, &size);
	unsigned char* pixel_buffer = (unsigned char*)malloc(size.cx * size.cy);
	if (NULL == pixel_buffer)
	{
		return -1;
	}

	for (size_t y = 0; y < size.cy; y++)
	{
		for (size_t x = 0; x < size.cx; x++)
		{
			int value = 0xff - (GetPixel(dc, x, y) & 0xff);
			pixel_buffer[y * size.cx + x] = value;
		}
	}

	unsigned char utf8_buffer[32];
	memset(utf8_buffer, 0, sizeof(utf8_buffer));
	int len = WideCharToMultiByte(CP_UTF8, 0, characterBuffer, -1, (char*)utf8_buffer, sizeof(utf8_buffer), NULL, NULL);

	unsigned int utf8_code = 0;
	switch (len)
	{
	case 0:
	case 1:
		MessageBox(L"WideCharToMultiByte error!", L"✖✖✖");
		return -1;
	case 2:
		utf8_code = utf8_buffer[0];
		break;
	case 3:
		utf8_code = (utf8_buffer[0] << 8) | (utf8_buffer[1]);
		break;
	case 4:
		utf8_code = (utf8_buffer[0] << 16) | (utf8_buffer[1] << 8) | utf8_buffer[2];
		break;
	case 5:
		utf8_code = (utf8_buffer[0] << 24) | (utf8_buffer[1] << 16) | (utf8_buffer[2] << 8) | utf8_buffer[3];
		break;
	default:
		MessageBox(L"GuiLite could not support unicode over 4 bytes", L"✖✖✖");
		return -1;
	}

	mCurrentFontLatticeMap[utf8_code].utf_8_code = utf8_code;
	mCurrentFontLatticeMap[utf8_code].width = size.cx;
	mCurrentFontLatticeMap[utf8_code].p_data = pixel_buffer;
	mCurrentFontLatticeMap[utf8_code].p_compressed_data = CompressFontLattice(pixel_buffer, size.cx * size.cy, mCurrentFontLatticeMap[utf8_code].p_compressed_data_length);

	char name[64];
	sprintf_s(name, "%ws_%d", logFont.lfFaceName, size.cy);
	mCurrentFontName = name;
	mCurrentFontName += (logFont.lfWeight > 400) ? "B" : "";

	std::replace(mCurrentFontName.begin(), mCurrentFontName.end(), '@', '_');		//replace '@' with '_'
	std::replace(mCurrentFontName.begin(), mCurrentFontName.end(), ' ', '_');		//replace ' ' with '_'

	mCurrentFontHeight = size.cy;
	TRACE(_T("font name = %s, utf8 = 0x%x, width = %d, height = %d\n"), logFont.lfFaceName, utf8_code, size.cx, size.cy);
	return 0;
}

static char* include = "#include \"GuiLite.h\"\n\n";
void CFontDlg::OnBnClickedGenerate()
{
	// TODO: Add your control notification handler code here
	if (0 == mCurrentFontLatticeMap.size())
	{
		return;
	}

	CWnd* generate_bt = GetDlgItem(IDC_BUTTON2);
	generate_bt->EnableWindow(false);

	char font_major_name[LF_FACESIZE];
	memset(font_major_name, 0, sizeof(font_major_name));

	std::fstream file(std::string("./") + mCurrentFontName + ".cpp", std::ios::trunc | std::ios::out | std::ios::binary);
	file.write(include, strlen(include));
	WriteLatticeDataInCppFile(file);
	file.close();

	generate_bt->EnableWindow(true);
	std::wstring result = L"Generate lattice cpp file success✌\n" + std::to_wstring(GetCompressionRatio()) + L"% of lattice size";
	MessageBox(result.c_str(), L"\\^o^/");
}

int CFontDlg::WriteLatticeDataInCppFile(std::fstream& file)
{
	if (0 == mCurrentFontLatticeMap.size())
	{
		return -1;
	}

	for (auto& elem : mCurrentFontLatticeMap)
	{
		std::string data_type = "static const unsigned char ";
		std::string data_name = "_" + (std::to_string(elem.second.utf_8_code));

		std::string define_lattice_data = data_type + data_name + "[] = {\n";
		for (int i = 0; i < elem.second.p_compressed_data_length; i++)
		{
			define_lattice_data += std::to_string(elem.second.p_compressed_data[i]);
			define_lattice_data += ", ";
		}
		define_lattice_data += "};\n";
		file.write(define_lattice_data.c_str(), define_lattice_data.length());
	}

	std::string define_lattice_array = "static  LATTICE lattice_array[] = {\n";
	for (auto& elem : mCurrentFontLatticeMap)
	{
		define_lattice_array += "        {" + std::to_string(elem.second.utf_8_code) + ", " +
			std::to_string(elem.second.width) + ", " +
			"_" + (std::to_string(elem.second.utf_8_code)) + "},\n";
	}
	define_lattice_array += "};\n";
	file.write(define_lattice_array.c_str(), define_lattice_array.length());

	std::string data_type = "const FONT_INFO ";
	std::string define_font_info = "extern " + data_type + mCurrentFontName + ";\n";
	define_font_info += data_type + mCurrentFontName + " ={\n";
	define_font_info += "    " + std::to_string(mCurrentFontHeight) + ",\n";
	define_font_info += "    " + std::to_string(mCurrentFontLatticeMap.size()) + ",\n";
	define_font_info += "    lattice_array\n};\n";

	file.write(define_font_info.c_str(), define_font_info.length());
	return 0;
}

unsigned char* CFontDlg::CompressFontLattice(unsigned char* p_data, int length, int& out_length)
{
	class CLatticeBlock
	{
	public:
		CLatticeBlock()
		{
			m_value = m_count = 0;
		}
		unsigned char m_value;
		unsigned char m_count;
	};

	std::vector<CLatticeBlock> blocks;
	CLatticeBlock cur_blk;
	cur_blk.m_value = p_data[0];
	for (int i = 0; i < length; i++)
	{
		if (cur_blk.m_value == p_data[i])
		{
			cur_blk.m_count++;
			if (0xFF == cur_blk.m_count && ((i + 1) < length) && (cur_blk.m_value == p_data[i + 1]))
			{//avoiding cur_blk.m_count overflow, add new block
				blocks.push_back(cur_blk);//truncate current block
				cur_blk.m_count = 0;//new block
			}
		}
		else
		{
			blocks.push_back(cur_blk);
			cur_blk.m_value = p_data[i];
			cur_blk.m_count = 1;
		}
	}
	if (cur_blk.m_count)
	{
		blocks.push_back(cur_blk);
	}
	
	out_length = blocks.size() * 2;
	unsigned char* ret = (unsigned char*)malloc(out_length);
	int index = 0;
	for (auto& elem : blocks)
	{
		ret[index++] = elem.m_value;
		ret[index++] = elem.m_count;
	}
	return ret;
}

int CFontDlg::GetCompressionRatio()
{
	if (0 == mCurrentFontLatticeMap.size())
	{
		return 0;
	}

	int raw_data_cnt = 0;
	int compressed_data_cnt = 0;
	for (auto& elem : mCurrentFontLatticeMap)
	{
		raw_data_cnt += elem.second.width * mCurrentFontHeight;
		compressed_data_cnt += elem.second.p_compressed_data_length;
	}
	TRACE(_T("raw_data_cnt = %d, compressed_data_cnt = %d\n"), raw_data_cnt, compressed_data_cnt);
	return (100 * compressed_data_cnt / raw_data_cnt);
}