// HistogramMatchingDlg.cpp : implementation file
//

#include "stdafx.h"
//#include "	\ add additional includes here"
#include "HistogramMatchingDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHistogramMatchingDlg dialog

BYTE *lpBits;
int nByteWidth,nHeight,nWidth,m_nBits,Flag,nBits;
HDC T1;BITMAPINFO *pBIH;
void CHistogramMatchingDlg::getmembers(BITMAPINFO *m_pBIH,HDC t1,int m_nWidth,int m_nHeight,int m_nByteWidth,BYTE *m_lpOut)
{
	nWidth = m_nWidth;
	nHeight = m_nHeight;
	nByteWidth = m_nByteWidth;
	nBits = m_nByteWidth/m_nWidth*8;
	lpBits = m_lpOut;
    lpOut = m_lpOut;
	T1=t1;
	pBIH=m_pBIH;
}
CHistogramMatchingDlg::CHistogramMatchingDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CHistogramMatchingDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CHistogramMatchingDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_Point1.x = 60;
	m_Point1.y = 200;
	m_Point2.x = 120;
	m_Point2.y = 50;
	m_Point3.x = 190;
	m_Point3.y = 200;
}


void CHistogramMatchingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CHistogramMatchingDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CHistogramMatchingDlg, CDialog)
	//{{AFX_MSG_MAP(CHistogramMatchingDlg)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHistogramMatchingDlg message handlers

BOOL CHistogramMatchingDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CWnd* pWnd = GetDlgItem(IDC_COORD);
	
	// �����������¼�����Ч����
	pWnd->GetClientRect(m_MouseRect);
	pWnd->ClientToScreen(&m_MouseRect);
	
	CRect rect;
	GetClientRect(rect);
	ClientToScreen(&rect);
	
	m_MouseRect.top -= rect.top;
	m_MouseRect.left -= rect.left;
	
	// ���ý�������¼�����Ч���������Ч��Ҫע����OnPaint�����е��������Ǻϣ���
	m_MouseRect.top += 25;
	m_MouseRect.left += 10;
	m_MouseRect.bottom = m_MouseRect.top + 255;
	m_MouseRect.right = m_MouseRect.left + 256;
	
	// ��ʼ���϶�״̬
	m_IsDragingFlag = 0;

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CHistogramMatchingDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	// ���û�������������ʼ�϶�
	if(m_MouseRect.PtInRect(point)) //Point in rectangle?
	{
		CRect rectTemp;
		
		// �����1�ٽ�����
		rectTemp.left = m_MouseRect.left + m_Point1.x - 2;
		rectTemp.right = m_MouseRect.left + m_Point1.x + 2;
		rectTemp.top = 255 + m_MouseRect.top - m_Point1.y - 2;
		rectTemp.bottom = 255 + m_MouseRect.top - m_Point1.y + 2;
		
		// �ж��û��ǲ������϶���1
		if (rectTemp.PtInRect(point))
		{
			
			// �����϶�״̬1���϶���1
			m_IsDragingFlag = 1;
			
			// ���Ĺ��
			::SetCursor(::LoadCursor(NULL, IDC_SIZEALL));
		}
		else
		{
			// �����2�ٽ�����
			rectTemp.left = m_MouseRect.left + m_Point2.x - 2;
			rectTemp.right = m_MouseRect.left + m_Point2.x + 2;
			rectTemp.top = 255 + m_MouseRect.top - m_Point2.y - 2;
			rectTemp.bottom = 255 + m_MouseRect.top - m_Point2.y + 2;
			
			// �ж��û��ǲ������϶���2
			if (rectTemp.PtInRect(point))
			{
				
				// �����϶�״̬Ϊ2���϶���2
				m_IsDragingFlag = 2;
				
				// ���Ĺ��
				::SetCursor(::LoadCursor(NULL, IDC_SIZEALL));
			}
			else
			{
				// �����3�ٽ�����
				rectTemp.left = m_MouseRect.left + m_Point3.x - 2;
				rectTemp.right = m_MouseRect.left + m_Point3.x + 2;
				rectTemp.top = 255 + m_MouseRect.top - m_Point3.y - 2;
				rectTemp.bottom = 255 + m_MouseRect.top - m_Point3.y + 2;
				
				// �ж��û��ǲ������϶���3
				if (rectTemp.PtInRect(point))
				{
					
					// �����϶�״̬Ϊ2���϶���3
					m_IsDragingFlag = 3;
					
					// ���Ĺ��
					::SetCursor(::LoadCursor(NULL, IDC_SIZEALL));
				}
			}
		}
	}
	CDialog::OnLButtonDown(nFlags, point);
}

void CHistogramMatchingDlg::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if (m_IsDragingFlag != 0)
	{
		m_IsDragingFlag = 0;
		HistogramMatching(m_Point1,m_Point2,m_Point3);
		StretchDIBits(T1,0,0,nWidth,nHeight,0,0,nWidth, nHeight,lpOut,pBIH,DIB_RGB_COLORS,SRCCOPY);
	}

	CDialog::OnLButtonUp(nFlags, point);
}

void CHistogramMatchingDlg::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
		if(m_MouseRect.PtInRect(point))		//��ǰ����ڻ�������
	{
		// �ж��Ƿ������϶�
		if (m_IsDragingFlag != 0)
		{
			// �ж������϶���1���ǵ�2���ǵ�3
			if (m_IsDragingFlag == 1)
			{
				// �ж�����϶��ĵ�1�Ƿ��ڵ�2���
				if (point.x - m_MouseRect.left < m_Point2.x)
				{
					// ��������
					m_Point1.x = point.x - m_MouseRect.left;
				}
				else
				{
					// ��1�Ϲ���2���ޣ�����Ϊ��2����-1
					m_Point1.x = m_Point2.x - 1;
					
					// �������λ��
					point.x = m_MouseRect.left + m_Point2.x - 1;
				}
				
				// ����Y����
				m_Point1.y = 255 + m_MouseRect.top - point.y;
			}
			else if (m_IsDragingFlag == 2)
			{
				// �����϶���2
				
				//�ж�����϶��ĵ�2�Ƿ��ڵ�1���3֮��
				if (point.x - m_MouseRect.left > m_Point1.x && point.x - m_MouseRect.left < m_Point3.x)
				{
					// ��������
					m_Point2.x = point.x - m_MouseRect.left;
				}
				else if(point.x - m_MouseRect.left <= m_Point1.x)
				{
					// ��2�Ϲ���1�����ޣ���2����Ϊ��1�����ޣ�1
					m_Point2.x = m_Point1.x + 1;
					
					// �������λ��
					point.x = m_MouseRect.left + m_Point1.x + 1;
				}
				else
				{
					// ��2�Ϲ���3�����ޣ���2����Ϊ��3������-1
					m_Point2.x = m_Point3.x - 1;
					
					// �������λ��
					point.x = m_MouseRect.left + m_Point3.x - 1;
				}
				
				// ����Y����
				m_Point2.y = 255 + m_MouseRect.top - point.y;
			}
			else
			{
				//�����϶���3
				
				//�ж�����϶��ĵ�3�Ƿ��ڵ�2�ұ�
				if (point.x - m_MouseRect.left > m_Point2.x)
				{
					// ��������
					m_Point3.x = point.x - m_MouseRect.left;
				}
				else
				{
					// ��3�Ϲ���2�����ޣ���3����Ϊ��2������+1
					m_Point3.x = m_Point2.x + 1;
					
					// �������λ��
					point.x = m_MouseRect.left + m_Point2.x + 1;
				}
				
				// ����Y����
				m_Point3.y = 255 + m_MouseRect.top - point.y;
			}
			
			// ���Ĺ��
			::SetCursor(::LoadCursor(NULL, IDC_SIZEALL));
			
			// ����
			UpdateData(FALSE);
			
			// �ػ�
			InvalidateRect(m_MouseRect, TRUE);
		}
		else
		{
			CRect	rectTemp1;
			CRect	rectTemp2;
			CRect   rectTemp3;
			
			// �����1�ٽ�����
			rectTemp1.left = m_MouseRect.left + m_Point1.x - 2;
			rectTemp1.right = m_MouseRect.left + m_Point1.x + 2;
			rectTemp1.top = 255 + m_MouseRect.top - m_Point1.y - 2;
			rectTemp1.bottom = 255 + m_MouseRect.top - m_Point1.y + 2;
			
			// �����2�ٽ�����
			rectTemp2.left = m_MouseRect.left + m_Point2.x - 2;
			rectTemp2.right = m_MouseRect.left + m_Point2.x + 2;
			rectTemp2.top = 255 + m_MouseRect.top - m_Point2.y - 2;
			rectTemp2.bottom = 255 + m_MouseRect.top - m_Point2.y + 2;

			// �����3�ٽ�����
			rectTemp3.left = m_MouseRect.left + m_Point3.x - 2;
			rectTemp3.right = m_MouseRect.left + m_Point3.x + 2;
			rectTemp3.top = 255 + m_MouseRect.top - m_Point3.y - 2;
			rectTemp3.bottom = 255 + m_MouseRect.top - m_Point3.y + 2;
			
			// �ж��û��ڵ�1���2���3�Ա�
			if ((rectTemp1.PtInRect(point)) || (rectTemp2.PtInRect(point)) || (rectTemp3.PtInRect(point)))
			{
				// ���Ĺ��
				::SetCursor(::LoadCursor(NULL, IDC_SIZEALL));
			}
		}
	}
	CDialog::OnMouseMove(nFlags, point);
}

void CHistogramMatchingDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	CWnd* pWnd = GetDlgItem(IDC_COORD);
	CDC* pDC = pWnd->GetDC();
	
	pWnd->Invalidate();
	pWnd->UpdateWindow();
	
	pDC->Rectangle(0,0,330,300);
	
	//�����ʻ�����
	CPen* pPenRed = new CPen;	//���
	pPenRed->CreatePen(PS_SOLID,2,RGB(255,0,0));
	CPen* pPenBlue = new CPen;	//����
	pPenBlue->CreatePen(PS_SOLID,1,RGB(0,0,255));
	CPen* pPenGreen = new CPen;	//�̱�
	pPenGreen->CreatePen(PS_DOT,1,RGB(0,255,0));
	
	// ѡ�е�ǰ��ɫ���ʣ���������ǰ�Ļ���
	CGdiObject* pOldPen = pDC->SelectObject(pPenRed);
	
	// ����������
	pDC->MoveTo(10,10);
	pDC->LineTo(10,280);
	pDC->LineTo(320,280);
	
	CString str;
	// д����
	str.Format("0");
	pDC->TextOut(10, 281, str);
	str.Format("255");
	pDC->TextOut(265, 281, str);
	pDC->TextOut(11, 25, str);
	
	// ����X���ͷ
	pDC->LineTo(315,275);
	pDC->MoveTo(320,280);
	pDC->LineTo(315,285);
	pDC->MoveTo(10,10);
	pDC->LineTo(5,15);
	pDC->MoveTo(10,10);
	pDC->LineTo(15,15);
	
	// ���ĳ���ɫ����
	pDC->SelectObject(pPenBlue);	
	
	// ��������ֵ
	str.Format("(%d, %d)", m_Point1.x, m_Point1.y);
	pDC->TextOut(m_Point1.x + 10, 281 - m_Point1.y, str);
	str.Format("(%d, %d)", m_Point2.x, m_Point2.y);
	pDC->TextOut(m_Point2.x + 10, 281 - m_Point2.y, str);
	str.Format("(%d, %d)", m_Point3.x, m_Point3.y);
	pDC->TextOut(m_Point3.x + 10, 281 - m_Point3.y, str);
	
	// �����û�ָ���ı任ֱ��
	pDC->MoveTo(10, 280);
	pDC->LineTo(m_Point1.x + 10, 280 - m_Point1.y);
	pDC->LineTo(m_Point2.x + 10, 280 - m_Point2.y);
	pDC->LineTo(m_Point3.x + 10, 280 - m_Point3.y);
	pDC->LineTo(265, 280);
	
	// ���Ƶ��Ե��С����
	CBrush  brush;
	brush.CreateSolidBrush(RGB(0,255,0));
	
	// ѡ��ˢ��
	CGdiObject* pOldBrush = pDC->SelectObject(&brush);
	
	// ����С����
	pDC->Rectangle(m_Point1.x + 10 - 2, 280 - m_Point1.y - 2, m_Point1.x + 12, 280 - m_Point1.y + 2);
	pDC->Rectangle(m_Point2.x + 10 - 2, 280 - m_Point2.y - 2, m_Point2.x + 12, 280 - m_Point2.y + 2);
	pDC->Rectangle(m_Point3.x + 10 - 2, 280 - m_Point3.y - 2, m_Point3.x + 12, 280 - m_Point3.y + 2);
	
	// �ָ���ǰ�Ļ���
	pDC->SelectObject(pOldPen);
	
	// ���Ʊ�Ե
	pDC->MoveTo(10,25);
	pDC->LineTo(265,25);
	pDC->LineTo(265,280);
	
	// ɾ���µĻ���
	delete pPenRed;
	delete pPenBlue;
	delete pPenGreen;
	// Do not call CDialog::OnPaint() for painting messages
}
BOOL CHistogramMatchingDlg::HistogramMatching(CPoint point1, CPoint point2, CPoint point3)
{
	//**********�����ڴ�************
	lpOut=new BYTE[nByteWidth*nHeight];
	int nSize = 1<<nBits;
	int k = 0;
	long * plOldGrayNum = NULL;
	BYTE* pbOldGrayMap = NULL;	//������ͼ�񵽾��⻯�ĻҶ�ӳ���

	BYTE* pbDigitizedFunction = NULL;	//���涨���������ֻ�,������涨������

	BYTE* pbNewGrayMap = NULL;	//���涨�Ҷ�ֱ��ͼ���о��⻯��ӳ���

	BYTE* pbFinalMap = NULL;	//����������Ĵ�Դͼ�񵽹涨ֱ��ͼ��ӳ���
	
	switch (nBits)
	{
		case 8:
			//����ͼ���λ��������Ӧ���ڴ�
			plOldGrayNum = new long[nSize];
			pbOldGrayMap = new BYTE[nSize];
			pbDigitizedFunction = new BYTE[nSize];
			pbNewGrayMap = new BYTE[nSize];
			pbFinalMap = new BYTE[nSize];
			
			if ((plOldGrayNum == NULL)||(pbOldGrayMap == NULL)||(pbNewGrayMap == NULL)||(pbDigitizedFunction == NULL)||(pbFinalMap == NULL))
				return FALSE;

			//����
			for(k=0; k<nSize; k++) 
			{
				plOldGrayNum[k] = 0;
				pbOldGrayMap[k] = 0;
				pbDigitizedFunction[k] = 0;
				pbNewGrayMap[k] = 0;
				pbFinalMap[k] = 0;
			}
			break;
		case 16:
			break;
		case 24:
			break;
		default:
			break;
	}
	
	int i,j;
	//**********����ԭʼͼ�񣬼���������⻯��ӳ���***********	
	for (j=0; j<nHeight; j++)	//������Ҷ�ֵ�ж��ٸ����ص㡣
	{
		for (i=0; i<nWidth; i++)
		{
			plOldGrayNum[lpBits[(nHeight-1-j)*nByteWidth+i]]++;
					// ע��ȡͼ���У�i,j�������صķ�ʽ������
		}
	}
	long lTempVar;
	for (i = 0; i < nSize; i++)
	{
		lTempVar = 0;
		for (j = 0; j <= i; j++)
		{
			lTempVar += plOldGrayNum[j];	//�����ۼ�
		}
		pbOldGrayMap[i] = (BYTE)(lTempVar*(nSize-1)/nHeight/nWidth);	//ӳ��ͼ(����)
	}


	//**********���涨�ĺ������ֻ�������涨�������ı��ˣ���Ҫ������ط�����������***********
	for (i = 0; i <= point1.x; i++)
	{
		// �ж�point1.x�Ƿ����0����ֹ��ĸΪ0��
		if (point1.x > 0)
			pbDigitizedFunction[i] = (BYTE)(point1.y*i/point1.x);
		else
			pbDigitizedFunction[i] = (BYTE)point1.y;
	}
	for (; i <= point2.x; i++)
	{
		if (point2.x != point1.x)
			pbDigitizedFunction[i] = (BYTE)(point1.y + (point2.y-point1.y) * (i-point1.x) / (point2.x-point1.x));
		else
			pbDigitizedFunction[i] = (BYTE)point1.y;
	}
	for (; i <= point3.x; i++)
	{
		if (point3.x != point2.x)
			pbDigitizedFunction[i] = (BYTE)(point2.y + (point3.y-point2.y) * (i-point2.x) / (point3.x-point2.x));
		else
			pbDigitizedFunction[i] = (BYTE)point2.y;
	}
	for (; i < 256; i++)
	{
		// �ж�bX2�Ƿ����255����ֹ��ĸΪ0��
		if (point3.x != 255)
			pbDigitizedFunction[i] = (BYTE)(point3.y + (-point3.y) * (i-point3.x) / (255-point3.x));
		else
			pbDigitizedFunction[i] = (BYTE)point3.y;
	}

	//����Թ涨��������ֱ��ͼ���⻯��Ҫ��ӳ���
	long lTotalTemp = 0;
	for (i = 0; i < 256; i++)
	{
		lTotalTemp += pbDigitizedFunction[i];
	}
	for (i = 0; i < 256; i++)
	{
		lTempVar = 0;	//ǰ���ж����һ����ʱ����
		for (j = 0; j <= i; j++)
		{
			lTempVar += pbDigitizedFunction[j];
		}
		pbNewGrayMap[i] = (BYTE)(lTempVar * 255 * 1.0 / lTotalTemp);	//����
	}


	//**********����ö�ٵ����ķ���ȷ��ԭͼ�����񻯺���֮���ӳ���ϵ******
	float min_value;	//��С��ֵ
	float now_value;	//��ǰ��ֵ
	int IndexTemp = 0;	//�±���ʱֵ
	for (i = 0; i < 256; i++)
	{
		min_value = 1.0;
		for (j = 0; j < 256; j++)	//ö�ٵ�������ʵ���㷨�������������ԸĽ�������ȽϵĴ���̫��
		{
			now_value = 0;
			if (pbOldGrayMap[i] - pbNewGrayMap[j] >= now_value)
				now_value = (float)pbOldGrayMap[i] - pbNewGrayMap[j];
			else
				now_value = (float)pbNewGrayMap[j] - pbOldGrayMap[i];
			if (now_value < min_value)
			{
				IndexTemp = j;
				min_value = now_value;
			}
		}
		pbFinalMap[i] = IndexTemp;
	}
	for (j=0; j<nHeight; j++)	//ͨ��ӳ����ͼ����д���
	{
		for (i=0; i<nWidth; i++)
			lpOut[(nHeight-1-i)*nWidth+j] = pbFinalMap[lpBits[(nHeight-1-i)*nWidth+j]];
	}

	delete[] plOldGrayNum;	//���շ����һ�������ڴ�ռ�
	delete[] pbOldGrayMap;
	delete[] pbDigitizedFunction;
	delete[] pbNewGrayMap;
	delete[] pbFinalMap;
	plOldGrayNum = NULL;
	pbOldGrayMap = NULL;
	pbDigitizedFunction = NULL;
	pbNewGrayMap = NULL;
	pbFinalMap = NULL;

	return TRUE;
}
