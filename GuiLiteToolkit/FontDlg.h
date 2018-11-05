#pragma once
#include <algorithm>
#include <map>
#include <fstream>
#include <string>
#include <vector> 

class CFontLattice
{
public:
	CFontLattice();
	~CFontLattice();

	unsigned int	utf_8_code;
	unsigned char	width;
	unsigned char*	p_data;
};

class CFontDlg : public CDialog
{
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedFont();
	afx_msg void OnBnClickedGenerate();
	DECLARE_MESSAGE_MAP()
	int GetCharInfo(wchar_t character, LOGFONT& logFont);
	int GetStringInfo(wchar_t* str, LOGFONT& logFont);
private:
	int WriteLatticeDataInCppFile(std::fstream& file);
	std::map<unsigned int, CFontLattice> mCurrentFontLatticeMap;
	std::string mCurrentFontName;
	int mCurrentFontHeight;
	wchar_t mInput[1024];
};
