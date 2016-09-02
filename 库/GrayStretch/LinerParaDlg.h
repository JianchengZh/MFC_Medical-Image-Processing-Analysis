#if !defined(AFX_LINERPARADLG_H__039BB21D_3CE3_4618_8B96_D664E92E20A3__INCLUDED_)
#define AFX_LINERPARADLG_H__039BB21D_3CE3_4618_8B96_D664E92E20A3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LinerParaDlg.h : header file
//
#include "resource.h"
/////////////////////////////////////////////////////////////////////////////
// CLinerParaDlg dialog

class AFX_EXT_CLASS CLinerParaDlg : public CDialog
{
// Construction
public:
	CLinerParaDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
public:	
	BOOL	m_bDrawed;			// ��ʶ�Ƿ��Ѿ�������Ƥ����	
	CPoint	m_p1;				// ��������������ʱ��λ��	
	CPoint	m_p2;				// ��������϶�ʱ��λ��	
	BOOL	m_bIsDraging;		// ��ǰ����϶�״̬��TRUE��ʾ�����϶���	
	CRect	m_MouseRect;		// ��Ӧ����¼��ľ�������
//	BYTE *lpOut;
	void LinerTrans();
	BYTE *lpOut;BYTE *lpBits;
	void getmembers(BITMAPINFO *m_pBIH,HDC t1,int m_nWidth,int m_nHeight,int m_nByteWidth,BYTE *m_lpOut);

	//{{AFX_DATA(CLinerParaDlg)
	enum { IDD = IDD_DLG_LINERPARA };
	float	m_fA;
	float	m_fB;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLinerParaDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CLinerParaDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnKillfocusEditA();
	afx_msg void OnKillfocusEditB();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LINERPARADLG_H__039BB21D_3CE3_4618_8B96_D664E92E20A3__INCLUDED_)
