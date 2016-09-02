// HisEqlize.cpp: implementation of the CHisEqlize class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "HisEqlize.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CHisEqlize::CHisEqlize()
{

}

CHisEqlize::~CHisEqlize()
{

}
void CHisEqlize::HistEq(int m_nBits,int nWidth,int nHeight,int nByteWidth,BYTE *lpBits)
{
	lpOut=new BYTE[nByteWidth*nHeight];	//�����������
	int len = nHeight*nWidth;
	int x,y,p,p1;
	BYTE *red_Points=new BYTE[len];		//ͼ���ɫ����
	BYTE *green_Points=new BYTE[len];	//ͼ����ɫ����
	BYTE *blue_Points=new BYTE[len];	//ͼ����ɫ����
	
	switch(m_nBits)
	{
	case 8:
		for (y=0;y<nHeight;y++)
		{
			for(x=0;x<nWidth;x++)
			{
				p=x+y*nByteWidth;
				lpOut[p]=lpBits[p];
			}
		}
		HistFunc(len,lpOut);
		break;
	case 24:
		for(y=0;y<nHeight;y++)
		{
			for(x=0;x<nWidth;x++)
			{
				p=x*3+y*nByteWidth;
				red_Points[x+y*nWidth]=lpBits[p];		//��ɫ������ȡ
				green_Points[x+y*nWidth]=lpBits[p+1];	//��ɫ������ȡ
				blue_Points[x+y*nWidth]=lpBits[p+2];	//��ɫ������ȡ
			}
		}
		
		HistFunc(len,red_Points);			//�ֱ��3ɫͨ����ֱ��ͼ���⻯����
		HistFunc(len,green_Points);
		HistFunc(len,blue_Points);
		
		for(y=0;y<nHeight;y++)				//���������3ɫͨ�����ݴ����������
		{
			for(x=0;x<nWidth;x++)
			{
				p=x*3+y*nByteWidth;
				p1=x+y*nWidth;
				lpOut[p]=red_Points[p1];
				lpOut[p+1]=green_Points[p1];
				lpOut[p+2]=blue_Points[p1];
			}
		}
		delete []red_Points;
		delete []green_Points;
		delete []blue_Points;
		break;
	}
}

void CHisEqlize::HistFunc(int len,BYTE *lpBits)
{
	int i;
	int GP[256],S[256];			//GPΪֱ��ͼ���飬SΪ�ֲ��������飬����256ɫ
	ZeroMemory(GP,1024);			//��ʼ������Ϊ0;ps:ע�ⳤ�Ȳ���256������256*sizeof(int)
	ZeroMemory(S,1024);
	for(i=0;i<len;i++)
		GP[lpBits[i]]++;	//#define Point(x,y) lpPoints[(x)+(y)*nWidth]	//ֱ��ͼ����//ͳ��ÿ���Ҷ�ֵ�����ظ���
	
	S[0]=GP[0];
	for(i=1;i<256;i++)
	{
		S[i]=S[i-1];
		S[i]+=GP[i];
	}
	for(i=0;i<len;i++)
		lpBits[i]=S[lpBits[i]]*255/len;	//#define Point(x,y) lpPoints[(x)+(y)*nWidth]
}