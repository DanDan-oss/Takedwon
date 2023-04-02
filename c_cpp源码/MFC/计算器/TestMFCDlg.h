
// testMFCDlg.h: ͷ�ļ�
//

#pragma once
#include "CEditNum.h"

// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���
class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};



// CtestMFCDlg �Ի���
class CtestMFCDlg : public CDialogEx
{
	// ����
public:
	CtestMFCDlg(CWnd* pParent = nullptr);	// ��׼���캯��

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TESTMFC_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CAboutDlg cAbout;


	long long m_old_num;			// ֮ǰ������
	long long m_curr_num;			// ��ǰ�ı��������
	bool m_iscp;
	int m_operator;			// �洢������� 0�� ��1 ��2 ��3 ��4 ��
	void FourCcat(int Operator);
	void Compute(int Operator);			// �Ӽ��˳�����
	void EventKey(int btnKey);

	CEditNum m_edit_text;
	CString m_edit_TextValue;
	CString m_oldnum_data;
	CButton m_checkBtn_Wintop;		// �����ö���ѡ��
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



