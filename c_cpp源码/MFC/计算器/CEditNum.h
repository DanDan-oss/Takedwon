#pragma once
#include <afxwin.h>
class CEditNum :
    public CEdit
{


public:
    DECLARE_MESSAGE_MAP()
    afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
    int GetEditSpot();      // 获取控件字符里面有多少个字符串
    bool IfFirstLetter();   // 判断是否字母开头
    afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnPopMenuCopy();
    afx_msg void OnPopMenuPat();
};

