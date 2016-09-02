// IdealFilter.cpp : implementation file
//

#include "stdafx.h"
//#include "	\ add additional includes here"
#include "IdealFilter.h"
#include "math.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CIdealFilter dialog


CIdealFilter::CIdealFilter(CWnd* pParent /*=NULL*/)
	: CDialog(CIdealFilter::IDD, pParent)
{
	//{{AFX_DATA_INIT(CIdealFilter)
	m_CutoffFreq = 50;
	//}}AFX_DATA_INIT
}


void CIdealFilter::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CIdealFilter)
	DDX_Text(pDX, IDC_IDEAL_CUTOFF_FREQ, m_CutoffFreq);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CIdealFilter, CDialog)
	//{{AFX_MSG_MAP(CIdealFilter)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIdealFilter message handlers

void CIdealFilter::GetMembers(int nByteWidth,int nHeight,int nBits,int nWidth,BYTE *lpBits)
{
	m_nByteWidth = nByteWidth;
	m_nHeight = nHeight;
	m_nBits = nBits;
	m_nWidth = nWidth;
	m_lpBits = lpBits;
}

BOOL CIdealFilter::FFTDIB(COMPLEX *pComplex)
{
	
	//	HCURSOR hcURSOR = SetCursor(LoadCursor(NULL, IDC_WAIT));	//���Ĺ����״
	
	
	//***********the old image*****************/
	int w=1, h=1, wp=0, hp=0;
	while(w < m_nWidth)
	{
		w *= 2;
		wp++;
	}
	while(h < m_nHeight)
	{
		h *= 2;
		hp++;
	}
	
	COMPLEX *TD   = new COMPLEX[w*h];	//ʱ��
	COMPLEX *FD   = new COMPLEX[w*h];	//Ƶ��
	
	int x,y;
	for(y=0; y<h; y++)
	{
		for(x=0; x<w; x++)
		{
			TD[x+w*y].re = 0;	//����ֵ
			TD[x+w*y].im = 0;
			FD[x+w*y].re = 0;	//����ֵ
			FD[x+w*y].im = 0;
		}
	}
	
	for(y=0; y<m_nHeight; y++)
	{
		for(x=0; x<m_nWidth; x++)
		{
			// 			TD[x + y*w].re = pBits[x + (nHeight-1-y)*nWidthBytes];	//��ô��ֵ����Ŀ��Ϊ�Σ��д����ã���(ע���±�)
			TD[x + y*w].re = m_lpBits[x + y*m_nByteWidth];
		}
	}
	
	CCOMPLEX t;
	for(y=0;y<h;y++)
	{
		t.FFT(&TD[w*y],&FD[w*y],wp);
	}
	for(y=0;y<h;y++)
	{
		for(x=0;x<w;x++)
		{
			TD[y+h*x]=FD[x+w*y];	//����ת��
			//			TD[x+w*y]=FD[x*h+y];
		}
	}
	for(x=0;x<w;x++)
	{
		t.FFT(&TD[x*h],&FD[x*h],hp);
	}
	
	double m;
	
	for(y=0; y<h; y++)
	{
		for(x=0; x<w; x++)
		{
			m = sqrt(FD[x*h+y].re*FD[x*h+y].re+FD[x*h+y].im*FD[x*h+y].im)/100;	//ע���±꣬ʵ�����൱��ת��
			if (m>255) m=255;
			//�����ĸ�if�������ã����У���
			if (y<m_nHeight/2 && x<m_nWidth/2)
				m_lpBits[(x+m_nWidth/2) + (y+m_nHeight/2)*m_nByteWidth] = (unsigned char)(m);
			if (y<m_nHeight/2 && x>=w-m_nWidth/2)
				m_lpBits[(x-(w-m_nWidth/2)) + (y+m_nHeight/2)*m_nByteWidth] = (unsigned char)(m);
			if (y>=h-m_nHeight/2 && x<m_nWidth/2)
				m_lpBits[(x+m_nWidth/2) + (y-(h-m_nHeight/2))*m_nByteWidth] = (unsigned char)(m);
			if (y>=h-m_nHeight/2 && x>=w-m_nWidth/2)
				m_lpBits[(x-(w-m_nWidth/2)) + (y-(h-m_nHeight/2))*m_nByteWidth] = (unsigned char)(m);
		}
	}
	
	for(y=0;y<h;y++)
	{
		for(x=0;x<w;x++)
		{
			pComplex[x+w*y].re=FD[x+w*y].re;
			pComplex[x+w*y].im=FD[x+w*y].im;
		}
	}
	
	delete [] TD;
	delete [] FD;
	
	//	SetCursor(hcURSOR);		//�ָ������״
	return TRUE;
}

BOOL CIdealFilter::IdealLowPassFilter()
{
//	HCURSOR hcURSOR = SetCursor(LoadCursor(NULL, IDC_WAIT));	//���Ĺ����״

	//**********��ȡͼ���������ָ��**********
	 
	//����Ƶ��ʱ���¿ռ�
	int w=1, h=1, wp=0, hp=0;
	while(w < m_nWidth)
	{
		w *= 2;
		wp++;
	}
	while(h < m_nHeight)
	{
		h *= 2;
		hp++;
	}
	COMPLEX *TD = new COMPLEX[w*h]; 
	COMPLEX *FD = new COMPLEX[w*h];

	//����ֵ
	int i,j;
	for(j=0; j<h; j++)
	{
		for(i=0; i<w; i++)
		{
			TD[i+w*j].re = 0;	
			TD[i+w*j].im = 0;
		}
	}
	for(j=0; j<m_nHeight; j++)
	{
		for(i=0; i<m_nWidth; i++)
		{
			TD[i + j*m_nByteWidth].re = m_lpBits[i + j*m_nByteWidth];
		}
	}

	CCOMPLEX t;
	//FFT
	for(j=0;j<h;j++)
	{
		t.FFT(&TD[w*j],&FD[w*j],wp);
	}
	for(j=0;j<h;j++)
	{
		for(i=0;i<w;i++)
		{
			TD[j+h*i]=FD[i+w*j];	//ת��	
//			TD[i+w*j]=FD[i*h+j];
		}
	}
	for(i=0;i<w;i++)
	{
		t.FFT(&TD[i*h],&FD[i*h],hp);
	}

	//LowPass
	COMPLEX* temp = new COMPLEX[w*h];
	for(j=0; j<h; j++)
	{
		for(i=0; i<w; i++)
		{
			temp[(i<w/2 ? i+w/2 : i-w/2) + (j<h/2 ? j+h/2 : j-h/2)*w] = FD[j + i*h];		//�Ⱥ��ұ��±��ʾת��,
// 			temp[(i<w/2 ? i+w/2 : i-w/2) + (j<h/2 ? j+h/2 : j-h/2)*w].im = FD[j + i*h].im;		//�Ⱥ�����±��ʾ���С�
		}
	}	
	for (j=0; j<h; j++)
	{
		for(i=0;i<w;i++)
		{
			if(j < h/2-m_CutoffFreq || j > h/2+m_CutoffFreq || i<w/2-m_CutoffFreq || i > w/2+m_CutoffFreq)
			{
				temp[i + j*w].re = 0; 
				temp[i + j*w].im = 0;
			}
			else if (t.Distance(i,j, w/2,h/2) > m_CutoffFreq)
			{
				temp[i + j*w].re = 0; 
				temp[i + j*w].im = 0;
			}
		}
	}
	for(j=0; j<h; j++)
	{
		for(i=0; i<w; i++)
		{
			FD[(i<w/2 ? i+w/2 : i-w/2) + (j<h/2 ? j+h/2 : j-h/2)*w] = temp[j + i*h];	//��ԭƵ��ͼ��
// 			FD[(i<w/2 ? i+w/2 : i-w/2) + (j<h/2 ? j+h/2 : j-h/2)*w].im = temp[j + i*h].im;
		}
	}	

	
	//IFFT
	for(j=0;j<h;j++)
	{
		t.IFFT(&FD[w*j],&TD[w*j],wp);
	}
	for(j=0;j<h;j++)
	{
		for(i=0;i<w;i++)
		{
			FD[j+h*i]=TD[i+w*j];
		}
	}
	for(i=0;i<w;i++)
	{
		t.IFFT(&FD[i*h],&TD[i*h],hp);
	}

	m_lpOut=new BYTE[m_nByteWidth*m_nHeight];
	//���任���д��ͼ����
	for(j=0; j<h; j++)
	{
		for(i=0; i<w; i++)
		{
			m_lpOut[i + j*m_nByteWidth] = (unsigned char)(TD[i+w*j].re);
		}
	}

	delete [] TD;
	delete [] FD;

//	SetCursor(hcURSOR);		//�ָ������״
	return TRUE;
}

BOOL CIdealFilter::IdealHighPassFilter()
{
//	HCURSOR hcURSOR = SetCursor(LoadCursor(NULL, IDC_WAIT));	//���Ĺ����״

	//**********��ȡͼ���������ָ��**********
		 
	//����Ƶ��ʱ���¿ռ�
	int w=1, h=1, wp=0, hp=0;
	while(w < m_nWidth)
	{
		w *= 2;
		wp++;
	}
	while(h < m_nHeight)
	{
		h *= 2;
		hp++;
	}
	COMPLEX *TD = new COMPLEX[w*h];
	COMPLEX *FD = new COMPLEX[w*h];

	//����ֵ
	int i,j;
	for(j=0; j<h; j++)
	{
		for(i=0; i<w; i++)
		{
			TD[i+w*j].re = 0;	
			TD[i+w*j].im = 0;
		}
	}
	for(j=0; j<m_nHeight; j++)
	{
		for(i=0; i<m_nWidth; i++)
		{
			TD[i + j*m_nByteWidth].re = m_lpBits[i + j*m_nByteWidth];
		}
	}

	CCOMPLEX t;
	//FFT
	for(j=0;j<h;j++)
	{
		t.FFT(&TD[w*j],&FD[w*j],wp);
	}
	for(j=0;j<h;j++)
	{
		for(i=0;i<w;i++)
		{
			TD[j+h*i]=FD[i+w*j];	//ת��	
//			TD[i+w*j]=FD[i*h+j];
		}
	}
	for(i=0;i<w;i++)
	{
		t.FFT(&TD[i*h],&FD[i*h],hp);
	}

	//HighPass
	COMPLEX* temp = new COMPLEX[w*h];
	for(j=0; j<h; j++)
	{
		for(i=0; i<w; i++)
		{
			temp[(i<w/2 ? i+w/2 : i-w/2) + (j<h/2 ? j+h/2 : j-h/2)*w] = FD[j + i*h];//�Ⱥ��ұ��±��ʾת��,�Ⱥ�����±��ʾ���С�
		}
	}	
	for (j=h/2-m_CutoffFreq; j<h/2+m_CutoffFreq; j++)
	{
		for(i=w/2-m_CutoffFreq; i<w/2+m_CutoffFreq; i++)
		{
			if (t.Distance(i,j, w/2,h/2) < m_CutoffFreq)
			{
				temp[i + j*w].re = 0; 
				temp[i + j*w].im = 0;
			}
		}
	}
	for(j=0; j<h; j++)
	{
		for(i=0; i<w; i++)
		{
			FD[(i<w/2 ? i+w/2 : i-w/2) + (j<h/2 ? j+h/2 : j-h/2)*w] = temp[j + i*h];	//��ԭƵ��ͼ��
		}
	}	

	
	//IFFT
	for(j=0;j<h;j++)
	{
		t.IFFT(&FD[w*j],&TD[w*j],wp);
	}
	for(j=0;j<h;j++)
	{
		for(i=0;i<w;i++)
		{
			FD[j+h*i]=TD[i+w*j];
		}
	}
	for(i=0;i<w;i++)
	{
		t.IFFT(&FD[i*h],&TD[i*h],hp);
	}

	//���任���д��ͼ����
	int len=m_nByteWidth*m_nHeight;
	m_lpOut=new BYTE[len];
	for(j=0; j<h; j++)
	{
		for(i=0; i<w; i++)
		{
			m_lpOut[i + j*m_nByteWidth] = (unsigned char)(TD[i+w*j].re);
		}
	}

	delete [] TD;
	delete [] FD;

//	SetCursor(hcURSOR);		//�ָ������״
	return TRUE;
}
