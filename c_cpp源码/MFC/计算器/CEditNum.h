#pragma once
#include <afxwin.h>
class CEditNum :
    public CEdit
{


public:
    DECLARE_MESSAGE_MAP()
    afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
    int GetEditSpot();      // ��ȡ�ؼ��ַ������ж��ٸ��ַ���
    bool IfFirstLetter();   // �ж��Ƿ���ĸ��ͷ
    afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnPopMenuCopy();
    afx_msg void OnPopMenuPat();
};

