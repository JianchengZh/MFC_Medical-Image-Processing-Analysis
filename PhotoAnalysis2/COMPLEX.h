// COMPLEX.h: interface for the CCOMPLEX class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_COMPLEX_H__B885CAED_805A_4D29_9869_CAA904EAEEA0__INCLUDED_)
#define AFX_COMPLEX_H__B885CAED_805A_4D29_9869_CAA904EAEEA0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define PI (double)3.14159265359
#define BWLOWPASS    1
#define BWHIGHPASS   2
#define ATTURBDEGMOL 3	//�����Ŷ��˻�
#define MOTIONDEGMOL 4	//�˶��˻�

typedef struct
{
	double re;
	double im;
}COMPLEX;

COMPLEX Mul(COMPLEX c1, COMPLEX c2);
COMPLEX Div(COMPLEX c1, COMPLEX c2);

class CCOMPLEX  
{
public:
	CCOMPLEX();
	virtual ~CCOMPLEX();

	void FFT(COMPLEX * TD, COMPLEX * FD, int power);
	void IFFT(COMPLEX * FD, COMPLEX * TD, int power);
	double Distance(int x1, int y1, int x2, int y2);
	void Calculate(COMPLEX *&FILTER, int nCutoffFreq, int nOrder, int w, int h, int TYPE);
	void Calculate(COMPLEX *&FILTER, float fGammaH, float fGammaL, int nCutoffFreq, int w, int h);
	void AtmosphTurbuDegeModel(COMPLEX* &Model, int w, int h, double Para_k);//�������ΪPara_k�Ĵ��������˻�ģ��
	void MotionDegenerateModel(COMPLEX* &Model, int w, int h, double Para_a, double Para_b, double Para_T);
};

#endif // !defined(AFX_COMPLEX_H__B885CAED_805A_4D29_9869_CAA904EAEEA0__INCLUDED_)
