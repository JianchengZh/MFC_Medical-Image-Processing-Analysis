// HistogramView1.cpp : implementation file
//

#include "stdafx.h"
#include "HistogramView.h"
#include "HistogramView1.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHistogramView dialog
int nWidth;
int nHeight;
int nByteWidth;
BYTE *lpBits;
int nBits;
BYTE *lpOut;
BOOL Flag;

CHistogramView::CHistogramView(CWnd* pParent /*=NULL*/)
	: CDialog(CHistogramView::IDD, pParent)
{
	//{{AFX_DATA_INIT(CHistogramView)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CHistogramView::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CHistogramView)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CHistogramView, CDialog)
	//{{AFX_MSG_MAP(CHistogramView)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHistogramView message handlers
void CHistogramView::getmember(int m_nWidth,int m_nHeight,int m_nByteWidth,BYTE *m_lpBits,int m_nBits,BYTE *m_lpOut,BOOL m_Flag)
{
	nWidth = m_nWidth;
	nHeight = m_nHeight;
	nByteWidth = m_nByteWidth;
	lpBits = m_lpBits;
	nBits = m_nBits;
	Flag = m_Flag;
	lpOut = m_lpOut;

}
BOOL CHistogramView::OnInitDialog() 
{
CDialog::OnInitDialog();

	int nSize = 1<<nBits;
	long* plCount = NULL;
	int x,y,k;
	switch (nBits)
	{
	case 8:
		//����ͼ���λ�÷�����Ӧ���ڴ�
		plCount = new long[nSize];
		m_pfDistr = new float[nSize];
		
		if ((m_pfDistr == NULL)||(plCount == NULL))
		{
			return FALSE;
		}
		//������
		for(k=0; k<nSize; k++) 
		{
			plCount[k] = 0;
			m_pfDistr[k] = 0;
		}
		break;
	case 16:
		break;
	case 24:
		break;
	default:
		break;
	}
	//
	if(Flag == 0)
	{
		for(y=0;y<nHeight;y++)				
		{
			for(x=0;x<nWidth;x++)
			{
				plCount[lpBits[(nHeight-1-y)*nByteWidth+x]]++;
			}
		}
	}
	else
	{
		for(y=0;y<nHeight;y++)				
		{
			for(x=0;x<nWidth;x++)
			{
				plCount[lpOut[(nHeight-1-y)*nByteWidth+x]]++;
			}
		}
	}
	long lTotalCount = (long)nHeight*nWidth;
	for (k=0;k<nSize;k++)
	{
		
		m_pfDistr[k] = plCount[k]/(float)lTotalCount;
		
	}
	
	delete[] plCount;	//���շ����һ�������ڴ�ռ�
	plCount = NULL;
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CHistogramView::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	// ��ȡ����������ı���
	CWnd* pWnd = GetDlgItem(IDC_COORD);
	CDC* pDC = pWnd->GetDC();
	pWnd->Invalidate();
	pWnd->UpdateWindow();
	
	RECT rect;
	pWnd->GetClientRect(&rect);
	pDC->Rectangle(rect.left,rect.top,rect.right,rect.bottom);	//������
	int rcWidth = rect.right-rect.left;
	int rcHeight = rect.bottom-rect.top;
	
	//�����ʻ�����
	CPen* pPenRed = new CPen;	//���
	pPenRed->CreatePen(PS_SOLID,1,RGB(255,0,0));
	CPen* pPenBlue = new CPen;	//����
	pPenBlue->CreatePen(PS_SOLID,1,RGB(0,0,255));
	CPen* pPenGreen = new CPen;	//�̱�
	pPenGreen->CreatePen(PS_DOT,1,RGB(0,255,0));
	
	//ѡ�е�ǰ��ɫ���ʣ���������ǰ�Ļ���
	CGdiObject* pOldPen = pDC->SelectObject(pPenRed);
	
	//����������
	pDC->MoveTo(15,15);
	pDC->LineTo(15,rcHeight-15);
	pDC->LineTo(rcWidth-15,rcHeight-15);
	
	//���������ͷ
	pDC->MoveTo(15,15);
	pDC->LineTo(15,5);
	pDC->LineTo(13,10);
	pDC->MoveTo(15,5);
	pDC->LineTo(17,10);
	
	//���ƺ����ͷ
	pDC->MoveTo(rcWidth-15,rcHeight-15);
	pDC->LineTo(rcWidth-5,rcHeight-15);
	pDC->LineTo(rcWidth-10,rcHeight-13);
	pDC->MoveTo(rcWidth-5,rcHeight-15);
	pDC->LineTo(rcWidth-10,rcHeight-17);
	
	//дX��̶�ֵ
	pDC->SetBkMode(TRANSPARENT);
	CString str = _T("");
	str.Format("0");
	pDC->TextOut(5,rcHeight-17,str);
	str.Format("Gray level");
	pDC->TextOut((int)(rcWidth/2-20),rcHeight-15,str);
	pDC->MoveTo(rcWidth-15,rcHeight-15);
	pDC->LineTo(rcWidth-15,rcHeight-20);
	str.Format("255");
	pDC->TextOut(rcWidth-25,rcHeight-15,str);
	
	//�����������ܶ�ֵ
	float fMaxDistr = 0;
	for (int i=0; i<256; i++)
	{
		//�ж��Ƿ���ڵ�ǰ���ֵ
		if (m_pfDistr[i] > fMaxDistr)
		{
			fMaxDistr = m_pfDistr[i];
		}
	}
	
	//дY��̶�
	pDC->MoveTo(15,15);
    pDC->LineTo(20,15);
	str.Format("%f",fMaxDistr);
	pDC->TextOut(20,8,str);
	
	//�ָ���ǰ�Ļ���
	pDC->SelectObject(pOldPen);
	
	//����ֱ��ͼ
	for (i=0; i<256; i++)
	{
		pDC->MoveTo((int)((rcWidth-30)/255.0*i)+15,rcHeight-15);
		pDC->LineTo((int)((rcWidth-30)/255.0*i)+15,rcHeight-15-(int)(m_pfDistr[i]/fMaxDistr*(rcHeight-30)));		
	}
	
	//ɾ������
	delete pPenRed;
	delete pPenBlue;
	delete pPenGreen;
	// Do not call CDialog::OnPaint() for painting messages
}
extern "C" __declspec(dllexport) void Show()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CHistogramView test;
	test.DoModal ();
}