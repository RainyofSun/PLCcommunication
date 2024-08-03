#include "StdAfx.h"
#include "PLCTool.h"


CPLCTool::CPLCTool(void)
{
}


CPLCTool::~CPLCTool(void)
{
}

long CPLCTool::reviseBit( long lWord, int bitNum, bool bValue )
{
	int lRevisedWord = lWord;

	if (bValue)
	{
		lRevisedWord = lWord | (1 << bitNum);

	}else
	{
		// 若原有bit是为1，则减去（bit位为1，其他位为0的值）
		const int nValue = (lWord >> bitNum) & 1;

		if (1 == nValue)
		{
			lRevisedWord = lWord - (1 << bitNum);
		}
	}
	return lRevisedWord;
}

int CPLCTool::getBit( long lWord, int bitNum )
{
	const int nValue = (lWord >> bitNum) & 1;

	return nValue;
}

std::vector<CString> CPLCTool::SplitCString(CString strSource, CString ch /* = */ ) 
{
	std::vector<CString> vecString;
	int iPos = 0;
	CString strTmp;
	strTmp = strSource.Tokenize(ch,iPos);

	while(strTmp.Trim() != TEXT(""))
	{
		vecString.push_back(strTmp);
		strTmp = strSource.Tokenize(ch,iPos);
	}

	return vecString;
}