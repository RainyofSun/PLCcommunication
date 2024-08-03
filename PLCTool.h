#pragma once
#include <vector>

class CPLCTool
{
public:
	CPLCTool(void);
	~CPLCTool(void);

	static long reviseBit(long lWord, int bitNum, bool bValue);
	static int getBit(long lWord, int bitNum);
	static std::vector<CString> SplitCString(CString strSource, CString ch = "$");
};

