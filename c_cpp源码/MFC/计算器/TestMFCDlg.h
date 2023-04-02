
// testMFCDlg.h: 头文件
//

#pragma once
#include "CEditNum.h"

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框
class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};



// CtestMFCDlg 对话框
class CtestMFCDlg : public CDialogEx
{
	// 构造
public:
	CtestMFCDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TESTMFC_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CAboutDlg cAbout;


	long long m_old_num;			// 之前的数据
	long long m_curr_num;			// 当前文本框的数据
	bool m_iscp;
	int m_operator;			// 存储运算符（ 0空 加1 减2 乘3 除4 ）
	void FourCcat(int Operator);
	void Compute(int Operator);			// 加减乘除运算
	void EventKey(int btnKey);

	CEditNum m_edit_text;
	CString m_edit_TextValue;
	CString m_oldnum_data;
	CButton m_checkBtn_Wintop;		// 窗口置顶单选框
	BOOL m_CheckBtn_Wintop;
	afx_msg void OnBnClickedButtonNum07();
	afx_msg void OnBnClickedButtonNum08();
	afx_msg void OnBnClickedButtonNum09();
	afx_msg void OnBnClickedButtonNum04();
	afx_msg void OnBnClickedButtonNum05();
	afx_msg void OnBnClickedButtonNum06();
	afx_msg void OnBnClickedButtonNum01();
	afx_msg void OnBnClickedButtonNum02();
	afx_msg void OnBnClickedButtonNum03();
	afx_msg void OnBnClickedButtonNum00();
	afx_msg void OnBnClickedBtnBs();
	afx_msg void OnBnClickedBtnPosneg();
	afx_msg void OnBnClickedBtnCe();
	afx_msg void OnBnClickedBtnCpadd();
	afx_msg void OnBnClickedBtnCpsub();
	afx_msg void OnBnClickedBtnCpmup();
	afx_msg void OnBnClickedBtnCpdivd();
	afx_msg void OnBnClickedBtnEmpty();
	afx_msg void OnBnClickedBtnEqual();
	afx_msg void OnMenuAbout();
	afx_msg void OnMenuCopy();
	afx_msg void OnMenuPaste();
	afx_msg void OnEnChangeEditText();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnMenuWintop();
	afx_msg void OnBnClickedCheckWintop();
};



