// testMFCDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "testMFC.h"
#include "testMFCDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CtestMFCDlg* cWinDlg = NULL;

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框
CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CtestMFCDlg 对话框
//CAboutDlg cAbout;		// 帮助菜单对话框

CtestMFCDlg::CtestMFCDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_TESTMFC_DIALOG, pParent)
	, m_edit_TextValue(_T(""))
	, m_oldnum_data(_T(""))
	, m_CheckBtn_Wintop(FALSE)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

}

void CtestMFCDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_TEXT, m_edit_text);
	DDX_Text(pDX, IDC_EDIT_TEXT, m_edit_TextValue);
	DDX_Text(pDX, IDC_STA_NUMDATA, m_oldnum_data);
	DDX_Control(pDX, IDC_CHECK_WINTOP, m_checkBtn_Wintop);
	DDX_Check(pDX, IDC_CHECK_WINTOP, m_CheckBtn_Wintop);
}

BEGIN_MESSAGE_MAP(CtestMFCDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_NUM07, &CtestMFCDlg::OnBnClickedButtonNum07)
	ON_BN_CLICKED(IDC_BUTTON_NUM08, &CtestMFCDlg::OnBnClickedButtonNum08)
	ON_BN_CLICKED(IDC_BUTTON_NUM09, &CtestMFCDlg::OnBnClickedButtonNum09)
	ON_BN_CLICKED(IDC_BUTTON_NUM04, &CtestMFCDlg::OnBnClickedButtonNum04)
	ON_BN_CLICKED(IDC_BUTTON_NUM05, &CtestMFCDlg::OnBnClickedButtonNum05)
	ON_BN_CLICKED(IDC_BUTTON_NUM06, &CtestMFCDlg::OnBnClickedButtonNum06)
	ON_BN_CLICKED(IDC_BUTTON_NUM01, &CtestMFCDlg::OnBnClickedButtonNum01)
	ON_BN_CLICKED(IDC_BUTTON_NUM02, &CtestMFCDlg::OnBnClickedButtonNum02)
	ON_BN_CLICKED(IDC_BUTTON_NUM03, &CtestMFCDlg::OnBnClickedButtonNum03)
	ON_BN_CLICKED(IDC_BUTTON_NUM00, &CtestMFCDlg::OnBnClickedButtonNum00)
	ON_BN_CLICKED(IDC_BTN_BS, &CtestMFCDlg::OnBnClickedBtnBs)
	ON_BN_CLICKED(IDC_BTN_POSNEG, &CtestMFCDlg::OnBnClickedBtnPosneg)
	ON_BN_CLICKED(IDC_BTN_CE, &CtestMFCDlg::OnBnClickedBtnCe)
	ON_BN_CLICKED(IDC_BTN_CPADD, &CtestMFCDlg::OnBnClickedBtnCpadd)
	ON_BN_CLICKED(IDC_BTN_CPSUB, &CtestMFCDlg::OnBnClickedBtnCpsub)
	ON_BN_CLICKED(IDC_BTN_CPMUP, &CtestMFCDlg::OnBnClickedBtnCpmup)
	ON_BN_CLICKED(IDC_BTN_CPDIVD, &CtestMFCDlg::OnBnClickedBtnCpdivd)
	ON_BN_CLICKED(IDC_BTN_EMPTY, &CtestMFCDlg::OnBnClickedBtnEmpty)
	ON_BN_CLICKED(IDC_BTN_EQUAL, &CtestMFCDlg::OnBnClickedBtnEqual)
	ON_COMMAND(ID_32780, &CtestMFCDlg::OnMenuAbout)
	ON_COMMAND(ID_32774, &CtestMFCDlg::OnMenuCopy)
	ON_COMMAND(ID_32775, &CtestMFCDlg::OnMenuPaste)
	ON_EN_CHANGE(IDC_EDIT_TEXT, &CtestMFCDlg::OnEnChangeEditText)
	ON_BN_CLICKED(IDC_BUTTON4, &CtestMFCDlg::OnBnClickedButton4)
	ON_COMMAND(ID_Menu_WinTop, &CtestMFCDlg::OnMenuWintop)
	ON_BN_CLICKED(IDC_CHECK_WINTOP, &CtestMFCDlg::OnBnClickedCheckWintop)
END_MESSAGE_MAP()


// CtestMFCDlg 消息处理程序

BOOL CtestMFCDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	m_edit_text.SetWindowText(TEXT("0."));
	cAbout.Create(IDD_ABOUTBOX, this);
	cWinDlg = this;


	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}


void CtestMFCDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CtestMFCDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CtestMFCDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CtestMFCDlg::EventKey(int btnKey)
{
	CString csTest;
	if (btnKey == 99)
		csTest += '.';
	else
		csTest.Format(TEXT("%d"), btnKey);
	if (m_iscp)
	{
		m_edit_TextValue = csTest;
		m_iscp = 0;
	}
	else
		m_edit_TextValue += csTest;
	UpdateData(FALSE);
}

void CtestMFCDlg::OnBnClickedButtonNum07()
{
	// TODO: 在此添加控件通知处理程序代码
	EventKey(7);
}

void CtestMFCDlg::OnBnClickedButtonNum08()
{
	// TODO: 在此添加控件通知处理程序代码
	EventKey(8);
}

void CtestMFCDlg::OnBnClickedButtonNum09()
{
	// TODO: 在此添加控件通知处理程序代码
	EventKey(9);
}

void CtestMFCDlg::OnBnClickedButtonNum04()
{
	// TODO: 在此添加控件通知处理程序代码
	EventKey(4);
}

void CtestMFCDlg::OnBnClickedButtonNum05()
{
	// TODO: 在此添加控件通知处理程序代码
	EventKey(5);
}

void CtestMFCDlg::OnBnClickedButtonNum06()
{
	// TODO: 在此添加控件通知处理程序代码
	EventKey(6);
}

void CtestMFCDlg::OnBnClickedButtonNum01()
{
	// TODO: 在此添加控件通知处理程序代码
	EventKey(1);
}

void CtestMFCDlg::OnBnClickedButtonNum02()
{
	// TODO: 在此添加控件通知处理程序代码
	EventKey(2);
}

void CtestMFCDlg::OnBnClickedButtonNum03()
{
	// TODO: 在此添加控件通知处理程序代码
	EventKey(3);
}

void CtestMFCDlg::OnBnClickedButtonNum00()
{
	// TODO: 在此添加控件通知处理程序代码
	EventKey(0);
}

void CtestMFCDlg::OnBnClickedButton4()
{
	// TODO: 在此添加控件通知处理程序代码
	// 小数点按钮被点击时
	int iCode = 0;
	int iSize = m_edit_TextValue.GetLength();
	wchar_t* cc = m_edit_TextValue.GetBuffer();
	for (size_t i = 0; i < iSize; i++)
	{
		if (cc[i] == '.')
		{
			m_edit_TextValue.ReleaseBuffer();
			return;
		}
	}

	m_edit_TextValue.ReleaseBuffer();
	EventKey(99);
}

void CtestMFCDlg::OnBnClickedBtnCpadd()
{
	// TODO: 在此添加控件通知处理程序代码
	FourCcat(1);
}

void CtestMFCDlg::OnBnClickedBtnCpsub()
{
	// TODO: 在此添加控件通知处理程序代码
	FourCcat(2);
}

void CtestMFCDlg::OnBnClickedBtnCpmup()
{
	// TODO: 在此添加控件通知处理程序代码
	FourCcat(3);
}

void CtestMFCDlg::OnBnClickedBtnCpdivd()
{
	// TODO: 在此添加控件通知处理程序代码
	FourCcat(4);
}

void CtestMFCDlg::OnBnClickedBtnEqual()
{
	// TODO: 在此添加控件通知处理程序代码
	FourCcat(5);
}

void  CtestMFCDlg::FourCcat(int Operator)
{
	if (m_iscp)
		return;
	m_iscp = TRUE;
	Compute(m_operator);
	m_oldnum_data.Format(TEXT("%d"), m_old_num);
	switch (Operator)
	{
	case 1:
		m_oldnum_data += TEXT(" +");
		break;
	case 2:
		m_oldnum_data += TEXT(" -");
		break;
	case 3:
		m_oldnum_data += TEXT(" *");
		break;
	case 4:
		m_oldnum_data += TEXT(" /");
		break;
	case 5:
	{
		m_oldnum_data = TEXT("");
		m_edit_TextValue.Format(TEXT("%d\n"), m_old_num);
		UpdateData(FALSE);
		m_operator = 0;
		m_iscp = FALSE;
		return;
	}
	default:
		break;
	}
	UpdateData(FALSE);
	m_operator = Operator;
}

void CtestMFCDlg::Compute(int Operator)
{
	// 取缓冲区内容判断又没小数点,兼容小数

	// ----- 数值太大无法转换
	long long iNum = _wtoi(m_edit_TextValue);
	switch (Operator)
	{
	case 0:		// 第一次点击加减乘除,不需要算出前面表达式的值
		m_old_num = iNum;
		break;
	case 1:		// +
		m_old_num += iNum;
		break;
	case 2:		// -
		m_old_num -= iNum;
		break;
	case 3:		// *
		m_old_num *= iNum;
		break;
	case 4:		// /
		m_old_num /= iNum;
		break;
	default:
		break;
	}
}

void CtestMFCDlg::OnBnClickedBtnBs()
{
	// TODO: 在此添加控件通知处理程序代码
	m_edit_TextValue.GetBuffer()[m_edit_TextValue.GetLength() - 1] = 0;
	m_edit_TextValue.ReleaseBuffer();
	UpdateData(FALSE);
}

void CtestMFCDlg::OnBnClickedBtnEmpty()
{
	// TODO: 在此添加控件通知处理程序代码
	m_curr_num = 0;
	m_edit_TextValue = TEXT("");
	UpdateData(FALSE);
}

void CtestMFCDlg::OnBnClickedBtnPosneg()
{
	// TODO: 在此添加控件通知处理程序代码
	const wchar_t* p = m_edit_TextValue.GetBuffer();
	if (p[0] == '-')
	{
		p++;
		m_edit_TextValue = p;
	}
	else
	{
		CString csBuffer = TEXT("-");
		csBuffer += m_edit_TextValue;
		m_edit_TextValue = csBuffer;
	}
	m_edit_TextValue.ReleaseBuffer();
	UpdateData(FALSE);
}

void CtestMFCDlg::OnBnClickedBtnCe()
{
	// TODO: 在此添加控件通知处理程序代码
	m_old_num = 0;
	m_curr_num = 0;
	m_operator = 0;
	m_edit_TextValue = TEXT("");
	m_oldnum_data = TEXT("");
	UpdateData(FALSE);
}

void CtestMFCDlg::OnMenuAbout()
{
	// TODO: 在此添加命令处理程序代码
	cAbout.ShowWindow(SW_SHOW);
}

void CtestMFCDlg::OnMenuCopy()
{
	// TODO: 点击 菜单复制
	UpdateData(TRUE);
	int error = 0;
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
	if (!EmptyClipboard())
	{
		CloseClipboard();
		return;
	}
	editLenth = m_edit_TextValue.GetLength();
	szChar = new wchar_t[editLenth + 1];
	memset(szChar, 0, editLenth * 2 + 2);
	memcpy(szChar, m_edit_TextValue.GetBuffer(), editLenth * 2 + 2);
	m_edit_TextValue.ReleaseBuffer();

	hbMem = GlobalAlloc(GHND, wcslen(szChar) * 2 + 2);
	wcChar = (wchar_t*)GlobalLock(hbMem);
	wcsncpy(wcChar, szChar, wcslen(szChar) + 1);
	SetClipboardData(CF_UNICODETEXT, wcChar);
	CloseClipboard();
	//HGLOBAL a =GlobalFree(hbMem);
	delete[] szChar;

}

void CtestMFCDlg::OnMenuPaste()
{
	// TODO: 点击 菜单粘贴
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
			m_edit_TextValue = szChar;
		}
		else
		{
			wcChar = new wchar_t[iSize * 2 + 2];
			memset(wcChar, 0, iSize * 2 + 2);
			memcpy(wcChar, pClipbBuf, iSize * 2 + 2);
			m_edit_TextValue = wcChar;
		}
		UpdateData(FALSE);
	} while (false);

	if (pClipbBuf)
		GlobalUnlock(pClipbBuf);
	if (wcChar)
		delete[] wcChar;
	if (szChar)
		delete[] szChar;

	CloseClipboard();

}

void CtestMFCDlg::OnEnChangeEditText()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码

	//int nSelStart = 0, nSelEnd = 0;			// 文本框光标位置
	//WCHAR wcValueBuf[256] = { 0 };
	//WCHAR ch = { 0 };
	//m_edit_text.GetSel(nSelStart, nSelEnd);
	//m_edit_text.GetWindowTextW(wcValueBuf, 256);
	//ch = wcValueBuf[nSelStart-1];

	//if (nSelStart-1 < 0)
	//	return;
	//// 输入的是数字1-9
	//if (ch >= '0' && ch <= '9' || ch == '.')
	//{
	//	if (ch == '.')
	//	{
	//		int iSise = m_edit_TextValue.GetLength();
	//		wchar_t* pBuf = m_edit_TextValue.GetBuffer();
	//		for (int i = 0; i < iSise; i++)
	//		{
	//			if (pBuf[i] == '.')
	//			{
	//				m_edit_TextValue.ReleaseBuffer();
	//				UpdateData(FALSE);
	//				m_edit_text.SetSel(nSelStart - 1, nSelEnd - 1);
	//				return;
	//			}
	//		}
	//		m_edit_TextValue.ReleaseBuffer();
	//	}
	//	UpdateData(TRUE);
	//	return;
	//}
	//else
	//{
	//	UpdateData(FALSE);
	//	m_edit_text.SetSel(nSelStart-1, nSelEnd-1);
	//	return;
	//}


}




void CtestMFCDlg::OnMenuWintop()
{
	// TODO: 菜单项窗口置顶项被点击
	UINT uStatu = this->GetMenu()->GetMenuState(ID_Menu_WinTop, MF_CHECKED);

	if (uStatu == MF_CHECKED)
	{
		// 置顶项 当前为选中状态, 设置为不指定
		this->GetMenu()->CheckMenuItem(ID_Menu_WinTop, MF_UNCHECKED);
		this->SetWindowPos(&wndNoTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	}
	else
	{
		this->GetMenu()->CheckMenuItem(ID_Menu_WinTop, MF_CHECKED);
		this->SetWindowPos(&wndTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	}
	return;
}


void CtestMFCDlg::OnBnClickedCheckWintop()
{
	// TODO: 窗口置顶复选框点击事件,设置窗口置顶

	int iCheckStat = m_checkBtn_Wintop.GetCheck();
	if (iCheckStat)
		this->SetWindowPos(&wndTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	else
		this->SetWindowPos(&wndNoTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);

	//UpdateData(TRUE);
	//if(m_CheckBtn_Wintop)
	//	this->SetWindowPos(&wndTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	//else
	//	this->SetWindowPos(&wndNoTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	//

}
