// LinerParaDlg.cpp : implementation file
//

#include "stdafx.h"
//#include "	\ add additional includes here"
#include "LinerParaDlg.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLinerParaDlg dialog
BITMAPINFO *pBIH1;
HDC T11;int nWidth,nHeight;int nByteWidth,nBits;
unsigned char* m_pTempBits;
void CLinerParaDlg::getmembers(BITMAPINFO *m_pBIH,HDC t1,int m_nWidth,int m_nHeight,int m_nByteWidth,BYTE *m_lpOut)
{
	nWidth = m_nWidth;
	nHeight = m_nHeight;
	nByteWidth = m_nByteWidth;
	nBits = m_nByteWidth/m_nWidth*8;
	lpBits = m_lpOut;
	T11=t1;
	pBIH1=m_pBIH;
}

CLinerParaDlg::CLinerParaDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLinerParaDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLinerParaDlg)
	m_fA = 0.0f;
	m_fB = 0.0f;
	//}}AFX_DATA_INIT
}


void CLinerParaDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLinerParaDlg)
	DDX_Text(pDX, IDC_EDIT_A, m_fA);
	DDX_Text(pDX, IDC_EDIT_B, m_fB);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLinerParaDlg, CDialog)
	//{{AFX_MSG_MAP(CLinerParaDlg)
	ON_EN_KILLFOCUS(IDC_EDIT_A, OnKillfocusEditA)
	ON_EN_KILLFOCUS(IDC_EDIT_B, OnKillfocusEditB)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLinerParaDlg message handlers

BOOL CLinerParaDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CWnd* pWnd = GetDlgItem(IDC_COORD);	
	pWnd->GetClientRect(m_MouseRect);
	pWnd->ClientToScreen(&m_MouseRect);
	
	CRect rect;
	GetClientRect(rect);
	ClientToScreen(&rect);	
	m_MouseRect.top -= rect.top;
	m_MouseRect.left -= rect.left;
	
	// ���ý�������¼�����Ч����
	m_MouseRect.top += 25;
	m_MouseRect.left += 10;
	m_MouseRect.bottom = m_MouseRect.top + 255;
	m_MouseRect.right = m_MouseRect.left + 256;
	
	m_bIsDraging = FALSE;
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CLinerParaDlg::OnKillfocusEditA() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	InvalidateRect(m_MouseRect, TRUE);
}

void CLinerParaDlg::OnKillfocusEditB() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	InvalidateRect(m_MouseRect, TRUE);
}

void CLinerParaDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	// ���û�������������ʼ�϶�
	if(m_MouseRect.PtInRect(point))
	{
		m_p1 = point;		
		// ת������ϵ
		m_p1.x = m_p1.x - m_MouseRect.left + 10;
		m_p1.y = m_p1.y - m_MouseRect.top + 25;
		
		m_bIsDraging = TRUE;
		m_bDrawed = FALSE;
		::SetCursor(::LoadCursor(NULL, IDC_CROSS));
		
		// ��ʼ��������¼�����֤������ƶ���������ʱҲ���Խ��յ�����ͷ��¼���
		SetCapture();
	}
	CDialog::OnLButtonDown(nFlags, point);
}

void CLinerParaDlg::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	// ���û��ͷ�������ֹͣ�϶�
	if (m_bIsDraging)
	{
		// �жϵ�ǰ����Ƿ��ڻ�������
		if(m_MouseRect.PtInRect(point))
		{
			m_p2 = point;			
			// ת������ϵ
			m_p2.x = m_p2.x - m_MouseRect.left + 10;
			m_p2.y = m_p2.y - m_MouseRect.top + 25;
			
			if ((m_p1 != m_p2) && (m_p1.x != m_p2.x))
			{
				// ת������ϵ
				m_p1.x = m_p1.x - 10;
				m_p1.y = 280 - m_p1.y;
				m_p2.x = m_p2.x - 10;
				m_p2.y = 280 - m_p2.y;				
				// ����б�ʺͽؾ�
				m_fA = (float) (m_p2.y - m_p1.y) / (m_p2.x - m_p1.x);
				m_fB = m_p1.y - m_fA * m_p1.x;
				
				LinerTrans();
                StretchDIBits(T11,0,0,nWidth,nHeight,0,0,nWidth, nHeight,lpOut,pBIH1,DIB_RGB_COLORS,SRCCOPY);
				UpdateData(FALSE);
			}
			
			InvalidateRect(m_MouseRect, TRUE);
		}
		else
		{
			// �û��ڻ���������ſ�������
			
			CWnd* pWnd = GetDlgItem(IDC_COORD);			// ��ȡ��ͼ�ı�ǩ						
			CDC* pDC = pWnd->GetDC();					// ��ȡ�豸������						
			int nOldDrawMode = pDC->SetROP2(R2_XORPEN);	// ���û��Ʒ�ʽΪ���ģʽ
			
			CPen* pPen = new CPen;
			pPen->CreatePen(PS_DOT,1,RGB(0,0,0));
			CGdiObject* pOldPen = pDC->SelectObject(pPen);
			
			// �ж��Ƿ��Ѿ�������Ƥ����
			if (m_bDrawed)
			{
				pDC->MoveTo(m_p1);
				pDC->LineTo(m_p2);
			}
			
			pDC->SelectObject(pOldPen);			// ѡ����ǰ�Ļ���			
			pDC->SetROP2(nOldDrawMode);			// �ָ�����ǰ�Ļ���ģʽ
			
			delete pPen;
			ReleaseDC(pDC);			
		}		
		
		::ReleaseCapture();						// ���������¼��ĸ���		
		m_bIsDraging = FALSE;					// �����϶�״̬
		
	}
	CDialog::OnLButtonUp(nFlags, point);
}

void CLinerParaDlg::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	// �жϵ�ǰ����Ƿ��ڻ�������
	if(m_MouseRect.PtInRect(point))
	{
		::SetCursor(::LoadCursor(NULL, IDC_CROSS));
		
		if (m_bIsDraging)
		{
			CWnd* pWnd = GetDlgItem(IDC_COORD);
			CDC* pDC = pWnd->GetDC();			
			// ���û��Ʒ�ʽΪ���ģʽ
			int nOldDrawMode = pDC->SetROP2(R2_XORPEN);
			
			CPen* pPen = new CPen;
			pPen->CreatePen(PS_DOT,1,RGB(0,0,0));
			CGdiObject* pOldPen = pDC->SelectObject(pPen);
			
			// �ж��Ƿ��Ѿ�������Ƥ����
			if (m_bDrawed)
			{
				pDC->MoveTo(m_p1);
				pDC->LineTo(m_p2);
			}
			
			m_p2 = point;		
			
			// ת������ϵ
			m_p2.x = m_p2.x - m_MouseRect.left + 10;
			m_p2.y = m_p2.y - m_MouseRect.top + 25;	
			
			// ����һ������Ƥ����
			pDC->MoveTo(m_p1);
			pDC->LineTo(m_p2);
			
			m_bDrawed = TRUE;			
			pDC->SelectObject(pOldPen);	// ѡ����ǰ�Ļ���		
			pDC->SetROP2(nOldDrawMode);	// �ָ�����ǰ�Ļ���ģʽ
			
			delete pPen;
			ReleaseDC(pDC);
		}
	}
	else
	{
		if (m_bIsDraging)
		{
			::SetCursor(::LoadCursor(NULL, IDC_NO));
		}
	}
	CDialog::OnMouseMove(nFlags, point);
}

void CLinerParaDlg::OnPaint() 
{
	CString str;	
	int x1, y1, x2, y2;			// ֱ�ߺ��������������
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
		CWnd* pWnd = GetDlgItem(IDC_COORD);
	CDC* pDC = pWnd->GetDC();
	pWnd->Invalidate();
	pWnd->UpdateWindow();
	pDC->Rectangle(0,0,330,300);
	
	CPen* pPenRed = new CPen;		// ��ɫ����
	pPenRed->CreatePen(PS_SOLID,2,RGB(255,0,0));
	CPen* pPenBlue = new CPen;		// ��ɫ����
	pPenBlue->CreatePen(PS_SOLID,2,RGB(0,0, 255));
	
	// ѡ�е�ǰ��ɫ���ʣ���������ǰ�Ļ���
	CGdiObject* pOldPen = pDC->SelectObject(pPenRed);
		
	pDC->MoveTo(10,10);				// ����������	
	pDC->LineTo(10,280);			// ��ֱ��	
	pDC->LineTo(320,280);			// ˮƽ��
	str.Format("0");
	pDC->TextOut(10, 281, str);		// д����	
	str.Format("255");
	pDC->TextOut(265, 281, str);
	pDC->TextOut(11, 25, str);
	
	// ����X���ͷ
	pDC->LineTo(315,275);
	pDC->MoveTo(320,280);
	pDC->LineTo(315,285);
	
	// ����X���ͷ
	pDC->MoveTo(10,10);
	pDC->LineTo(5,15);
	pDC->MoveTo(10,10);
	pDC->LineTo(15,15);
	
	// ���ĳ���ɫ����
	pDC->SelectObject(pPenBlue);	
	
	// ����ֱ�ߺ������������������
	if (m_fA >= 0)
	{
		if (((m_fA * 255 + m_fB) >= 0) && (m_fB < 255))
		{
			if (m_fB < 0)
			{
				x1 = (int) (- m_fB/m_fA + 0.5);
				y1 = 0;
			}
			else
			{
				x1 = 0;
				y1 = (int) (m_fB + 0.5);
			}
			
			if ((m_fA * 255 + m_fB) > 255)
			{
				x2 = (int) ((255- m_fB)/m_fA + 0.5);
				y2 = 255;
			}
			else
			{
				x2 = 255;
				y2 = (int) (255* m_fA + m_fB + 0.5);
			}
		}
		else if(((m_fA * 255 + m_fB) < 0))
		{
			x1 = 0;
			y1 = 0;
			x2 = 255;
			y2 = 0;
		}
		else
		{
			x1 = 0;
			y1 = 255;
			x2 = 255;
			y2 = 255;
		}
	}
	else  // б��С��0
	{
		if ((m_fB > 0) && (255* m_fA + m_fB < 255))
		{
			if (m_fB > 255)
			{
				x1 = (int) ((255- m_fB)/m_fA + 0.5);
				y1 = 255;
			}
			else
			{
				x1 = 0;
				y1 = (int) (m_fB + 0.5);
			}
			
			if ((m_fA * 255 + m_fB) < 0)
			{
				x2 = (int) (- m_fB/m_fA + 0.5);
				y2 = 0;
			}
			else
			{
				x2 = 255;
				y2 = (int) (255* m_fA + m_fB + 0.5);
			}
		}
		else if (m_fB <=0)
		{
			x1 = 0;
			y1 = 0;
			x2 = 255;
			y2 = 0;
		}
		else
		{
			x1 = 0;
			y1 = 255;
			x2 = 255;
			y2 = 255;
		}
	}
	
	// ��������ֵ
	str.Format("(%d, %d)", x1, y1);
	pDC->TextOut(x1 + 10, 280 - y1 + 1, str);
	str.Format("(%d, %d)", x2, y2);
	pDC->TextOut(x2 + 10, 280 - y2 + 1, str);
	
	// �����û�ָ�������Ա任ֱ�ߣ�ע��ת������ϵ��
	pDC->MoveTo(x1 + 10, 280 - y1);
	pDC->LineTo(x2 + 10, 280 - y2);
	
	// �ָ���ǰ�Ļ���
	pDC->SelectObject(pOldPen);	
	
	// ���Ʊ�Ե
	pDC->MoveTo(10,25);
	pDC->LineTo(265,25);
	pDC->LineTo(265,280);
	
	delete pPenRed;
	delete pPenBlue;
	// Do not call CDialog::OnPaint() for painting messages
}

void CLinerParaDlg::LinerTrans()
{
	//lpOut=lpOut;
	lpOut=new BYTE[nByteWidth*nHeight];
	float fTemp,fTemp1,fTemp2;
	int i,j;

			switch(nBits)
			{
			case 8:	
				for(i = 0; i < nHeight; i++)			// ÿ��
				{		
					for(j = 0; j < nWidth; j++)			// ÿ��
					{
						fTemp = m_fA * (lpBits[i*nByteWidth+j]) + m_fB;		// ���Ա任
			        	if (fTemp > 255)				// �ж��Ƿ񳬳���Χ
						{
					 lpOut[i*nByteWidth+j] = 255;
						}
			    	else if (fTemp < 0)
					{
					lpOut[i*nByteWidth+j] = 0;
					}
				else
				{				
					lpOut[i*nByteWidth+j] = (unsigned char) (fTemp + 0.5);	// ��������
				}
				}
				}
				break;
			case 24:
				for(i = 0; i < nHeight; i++)			// ÿ��
				{		
					for(int j = 0; j < nWidth; j++)			// ÿ��
		{fTemp = m_fA * (lpBits[i*nByteWidth+3*j]) + m_fB;		// ���Ա任
				fTemp1 = m_fA * (lpBits[i*nByteWidth+3*j+1]) + m_fB;
				fTemp2 = m_fA * (lpBits[i*nByteWidth+3*j]+2) + m_fB;
				if (fTemp > 255)				// �ж��Ƿ񳬳���Χ
				{
					lpOut[i*nByteWidth+3*j] = 255;
				}
				else if (fTemp < 0)
				{
					lpOut[i*nByteWidth+3*j] = 0;
				}
				else 
				{				
					lpOut[i*nByteWidth+3*j] = (unsigned char) (fTemp + 0.5);	// ��������
				}
				 if (fTemp1 > 255)				// �ж��Ƿ񳬳���Χ
				{
					lpOut[i*nByteWidth+3*j+1] = 255;
				}
				else if (fTemp1 < 0)
				{
					lpOut[i*nByteWidth+3*j+1] = 0;
				}
				else 
				{				
					lpOut[i*nByteWidth+3*j+1] = (unsigned char) (fTemp + 0.5);	// ��������
				}
				 if (fTemp2 > 255)				// �ж��Ƿ񳬳���Χ
				{
					lpOut[i*nByteWidth+j*3+2] = 255;
				}
				else if (fTemp2 < 0)
				{
					lpOut[i*nByteWidth+3*j+2] = 0;
				}
				else
				{				
					lpOut[i*nByteWidth+3*j+2] = (unsigned char) (fTemp + 0.5);	// ��������
				}

			}

		}
				break;
	}
}
