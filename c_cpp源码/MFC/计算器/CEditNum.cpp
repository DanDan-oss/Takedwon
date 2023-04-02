#include "pch.h"
#include "CEditNum.h"
#include "resource.h"
#include "testMFCDlg.h"
extern CtestMFCDlg* cWinDlg;

BEGIN_MESSAGE_MAP(CEditNum, CEdit)
	ON_WM_CHAR()
	ON_WM_RBUTTONDOWN()
	ON_COMMAND(ID_32781, &CEditNum::OnPopMenuCopy)
	ON_COMMAND(ID_32782, &CEditNum::OnPopMenuPat)
END_MESSAGE_MAP()


void CEditNum::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 键盘按下 输入字符消息

	// 按下删除键
	if (nChar == 8)
	{
		CEdit::OnChar(nChar, nRepCnt, nFlags);
		return;
	}

	// 判断按下是否是字符是否字母开头,强制字母
	//if (!this->IfFirstLetter())
	//{
	//	int nStart = 0, nEnd = 0;		// 当前文本框光标所在位置
	//	this->GetSel(nStart, nEnd);

	//	//当前光标不在首位输入位置
	//	if (nStart != 0)
	//	{
	//		AfxMessageBox(L"首字符必须是字符");
	//		return;
	//	}
	//	
	//	if (nChar >= 'a' && nChar >= 'z' || nChar >= 'A' && nChar >= 'Z')
	//		CEdit::OnChar(nChar, nRepCnt, nFlags);
	//	else
	//		AfxMessageBox(L"首字符输入的必须是字符");
	//	return;	
	//}


	// 按下数字键 0-9 和 小数点.
	if (nChar >= '0' && nChar <= '9' || nChar == '.')
	{
		if (nChar == '.')
		{
			int n = this->GetEditSpot();
			if (n < 1)
				CEdit::OnChar(nChar, nRepCnt, nFlags);
			return;
		}
		CEdit::OnChar(nChar, nRepCnt, nFlags);
		return;
	}


	return;
}


int CEditNum::GetEditSpot()
{
	int iBufSize = 0;	// 控件字符串大小
	WCHAR* pBuf = 0;	// 获取控件字符串文本
	int n = 0;			// 控件内 . 的个数

	iBufSize = this->GetWindowTextLengthW();
	pBuf = new WCHAR[iBufSize * sizeof(WCHAR)];
	GetWindowTextW(pBuf, iBufSize * sizeof(WCHAR));
	for (int i = 0; i < iBufSize; i++)
		if (pBuf[i] == '.')
			n++;

	delete[] pBuf;
	return n;
}

bool  CEditNum::IfFirstLetter()
{
	int iBufer = 0;
	WCHAR* pBuf = NULL;
	bool nRet = 0;

	pBuf = new WCHAR[2];
	GetWindowTextW(pBuf, 2);
	if (pBuf[0] >= 'a' && pBuf[0] <= 'z' || pBuf[0] >= 'A' && pBuf[0] <= 'Z')
		nRet = 1;
	delete[] pBuf;
	return nRet;
}

void CEditNum::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	//CEdit::OnRButtonDown(nFlags, point);
	CMenu popMenu;		// 资源对象
	CPoint cpXY = 0;		// 存放鼠标当前坐标位置信息
	popMenu.LoadMenuW(IDR_MENU_RBTN);
	GetCursorPos(&cpXY);
	popMenu.GetSubMenu(0)->TrackPopupMenu(0, cpXY.x, cpXY.y, this);
}


void CEditNum::OnPopMenuCopy()
{
	// TODO: 编辑框点击右键复制功能

	UpdateData(TRUE);
	int error = 0;
	CString csValue;		// 获取控件文本临时存放
	int editLenth = 0;		// 文本框数据长度
	wchar_t* szChar = NULL;	// 文本框数据
	HGLOBAL hbMem = NULL;	// 全局内存块句柄
	wchar_t* wcChar = NULL;	// 指向系统全局内存块
	error = OpenClipboard();
	if (!error)
	{
		MessageBox(TEXT("剪切板已被其他应用占用"));

		return;
	}

	// 清空剪切板是否成功
	if (!EmptyClipboard())
	{
		CloseClipboard();
		return;
	}
	editLenth = this->GetWindowTextLengthW();
	this->GetWindowTextW(csValue);
	szChar = new wchar_t[editLenth + 1];
	memset(szChar, 0, editLenth * 2 + 2);
	memcpy(szChar, csValue.GetBuffer(), editLenth * 2 + 2);
	csValue.ReleaseBuffer();

	hbMem = GlobalAlloc(GHND, wcslen(szChar) * 2 + 2);
	wcChar = (wchar_t*)GlobalLock(hbMem);
	wcsncpy(wcChar, szChar, wcslen(szChar) + 1);
	SetClipboardData(CF_UNICODETEXT, wcChar);
	CloseClipboard();
	//HGLOBAL a =GlobalFree(hbMem);
	delete[] szChar;
}


void CEditNum::OnPopMenuPat()
{
	// 编辑框点击右键粘贴功能
	HANDLE hdClipb = NULL;		// 全局内存块句柄
	char* szChar = NULL;
	wchar_t* wcChar = NULL;
	void* pClipbBuf = NULL;		// 剪切板数据
	int iSize = 0;				// 剪切板数据大小
	char iType = 0;
	if (!IsClipboardFormatAvailable(CF_TEXT || CF_UNICODETEXT))
	{
		AfxMessageBox(L"剪切板数据的类型无法粘贴到文本框");
		return;
	}
	if (!OpenClipboard())
	{
		AfxMessageBox(L"剪切板正在被其他应用占用");
		return;
	}
	do
	{
		if (IsClipboardFormatAvailable(CF_TEXT))
		{
			hdClipb = GetClipboardData(CF_TEXT);
			iType = 1;
		}
		else
		{
			hdClipb = GetClipboardData(CF_UNICODETEXT);
			iType = 0;
		}
		iSize = GlobalSize(hdClipb);
		pClipbBuf = GlobalLock(hdClipb);
		if (!pClipbBuf)
			break;

		if (iType)
		{
			szChar = new char[iSize + 1];
			memset(szChar, 0, iSize + 1);
			memcpy(szChar, pClipbBuf, iSize + 1);
			cWinDlg->m_edit_TextValue = szChar;
		}
		else
		{
			wcChar = new wchar_t[iSize * 2 + 2];
			memset(wcChar, 0, iSize * 2 + 2);
			memcpy(wcChar, pClipbBuf, iSize * 2 + 2);
			cWinDlg->m_edit_TextValue = wcChar;
		}
		cWinDlg->UpdateData(FALSE);
	} while (false);

	if (pClipbBuf)
		GlobalUnlock(pClipbBuf);
	if (wcChar)
		delete[] wcChar;
	if (szChar)
		delete[] szChar;

	CloseClipboard();

}
