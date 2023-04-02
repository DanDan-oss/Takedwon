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
	// TODO: ���̰��� �����ַ���Ϣ

	// ����ɾ����
	if (nChar == 8)
	{
		CEdit::OnChar(nChar, nRepCnt, nFlags);
		return;
	}

	// �жϰ����Ƿ����ַ��Ƿ���ĸ��ͷ,ǿ����ĸ
	//if (!this->IfFirstLetter())
	//{
	//	int nStart = 0, nEnd = 0;		// ��ǰ�ı���������λ��
	//	this->GetSel(nStart, nEnd);

	//	//��ǰ��겻����λ����λ��
	//	if (nStart != 0)
	//	{
	//		AfxMessageBox(L"���ַ��������ַ�");
	//		return;
	//	}
	//	
	//	if (nChar >= 'a' && nChar >= 'z' || nChar >= 'A' && nChar >= 'Z')
	//		CEdit::OnChar(nChar, nRepCnt, nFlags);
	//	else
	//		AfxMessageBox(L"���ַ�����ı������ַ�");
	//	return;	
	//}


	// �������ּ� 0-9 �� С����.
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
	int iBufSize = 0;	// �ؼ��ַ�����С
	WCHAR* pBuf = 0;	// ��ȡ�ؼ��ַ����ı�
	int n = 0;			// �ؼ��� . �ĸ���

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
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	//CEdit::OnRButtonDown(nFlags, point);
	CMenu popMenu;		// ��Դ����
	CPoint cpXY = 0;		// �����굱ǰ����λ����Ϣ
	popMenu.LoadMenuW(IDR_MENU_RBTN);
	GetCursorPos(&cpXY);
	popMenu.GetSubMenu(0)->TrackPopupMenu(0, cpXY.x, cpXY.y, this);
}


void CEditNum::OnPopMenuCopy()
{
	// TODO: �༭�����Ҽ����ƹ���

	UpdateData(TRUE);
	int error = 0;
	CString csValue;		// ��ȡ�ؼ��ı���ʱ���
	int editLenth = 0;		// �ı������ݳ���
	wchar_t* szChar = NULL;	// �ı�������
	HGLOBAL hbMem = NULL;	// ȫ���ڴ����
	wchar_t* wcChar = NULL;	// ָ��ϵͳȫ���ڴ��
	error = OpenClipboard();
	if (!error)
	{
		MessageBox(TEXT("���а��ѱ�����Ӧ��ռ��"));

		return;
	}

	// ��ռ��а��Ƿ�ɹ�
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
	// �༭�����Ҽ�ճ������
	HANDLE hdClipb = NULL;		// ȫ���ڴ����
	char* szChar = NULL;
	wchar_t* wcChar = NULL;
	void* pClipbBuf = NULL;		// ���а�����
	int iSize = 0;				// ���а����ݴ�С
	char iType = 0;
	if (!IsClipboardFormatAvailable(CF_TEXT || CF_UNICODETEXT))
	{
		AfxMessageBox(L"���а����ݵ������޷�ճ�����ı���");
		return;
	}
	if (!OpenClipboard())
	{
		AfxMessageBox(L"���а����ڱ�����Ӧ��ռ��");
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
