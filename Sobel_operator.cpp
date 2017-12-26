// kingimageView.cpp : implementation of the CKingimageView class
//

#include "stdafx.h"
#include "kingimage.h"

#include "kingimageDoc.h"
#include "kingimageView.h"
#include <ctgmath>
#include <fstream>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CKingimageView

IMPLEMENT_DYNCREATE(CKingimageView, CScrollView)

BEGIN_MESSAGE_MAP(CKingimageView, CScrollView)
	//{{AFX_MSG_MAP(CKingimageView)
	ON_WM_MOUSEMOVE()
	ON_COMMAND(ID_PROCESS, OnProcess)
	ON_COMMAND(ID_any, Onany)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CScrollView::OnFilePrintPreview)
	ON_COMMAND(ID_SOBEL, &CKingimageView::OnSobel)
	ON_COMMAND(ID_UNSHARPMASKING, &CKingimageView::OnUnsharpmasking)
	ON_COMMAND(ID_LOGMASK4, &CKingimageView::OnLogmask4)
	ON_COMMAND(ID_LOGMASK3, &CKingimageView::OnLogmask3)
	ON_COMMAND(ID_LOGMASK, &CKingimageView::OnLogmask)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CKingimageView construction/destruction

CKingimageView::CKingimageView()
{
	// TODO: add construction code here

}

CKingimageView::~CKingimageView()
{
}

BOOL CKingimageView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CScrollView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CKingimageView drawing

void CKingimageView::OnDraw(CDC* pDC)
{
	CKingimageDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
//	pDC->SetStretchBltMode(COLORONCOLOR);
//	int x,y,bytes;
	if (pDoc->imagetype==PCX)
		(pDoc->_pcx)->draw(pDC);
	if (pDoc->imagetype==BMP)
		(pDoc->_bmp)->draw(pDC);
	if (pDoc->imagetype==GIF)
		(pDoc->_gif)->draw(pDC);
	if (pDoc->imagetype==JPG)
		(pDoc->_jpg)->draw(pDC);
/*	if (pDoc->imagetype==BMP)
	{
	
	x=((KINGBMP*)(pDoc->_ppicture))->bmpinfo.bmiHeader.biWidth;
	y=((KINGBMP*)(pDoc->_ppicture))->bmpinfo.bmiHeader.biHeight;
	bytes=(pDoc->_ppicture)->needbyte;
	int place=0;
	for (int j=0;j<y;j++)
		for (int i=0;i<x;i++){
//			int color= (pDoc->_bmp)->point[place++] ;
			pDC->SetPixel(i,j,RGB( (pDoc->_picture)->pixel[place+2], (pDoc->_picture)->pixel[place+1] ,(pDoc->_picture)->pixel[place]));
			place+=3;
		}

  /*	if ( (pDoc->_bmp)->bitsperpixel!=24 ){
		CPalette *palette=new CPalette();
		LOGPALETTE palet;
		palet.palVersion=0x300;
		palet.palNumEntries=(pDoc->_bmp)->usedcolor;
		for (int i=0;i<palet.palNumEntries;i++){
			palet.palPalEntry[i].peRed=( (pDoc->_bmp) -> bmpinfo) .bmiColors[i].rgbRed;
			palet.palPalEntry[i].peBlue=( (pDoc->_bmp) -> bmpinfo) .bmiColors[i].rgbBlue;
			palet.palPalEntry[i].peGreen=( (pDoc->_bmp) -> bmpinfo) .bmiColors[i].rgbGreen;
			palet.palPalEntry[i].peFlags=0;//( (pDoc->_bmp) -> bmpinfo) .bmiColors[0].rgbRed;
		}
		BOOL re=palette->CreatePalette(&palet);
		CPalette *pp;
		pp=pDC->SelectPalette(palette,TRUE);
	}
*/
//	::SetDIBitsToDevice(pDC->m_hDC,0,0,x,y,0,0,0,y,(pDoc->_ppicture)->point,&(((KINGBMP*)(pDoc->_ppicture))->bmpinfo),DIB_RGB_COLORS);
	
//	}
	// TODO: add draw code for native data here
}

/////////////////////////////////////////////////////////////////////////////
// CKingimageView printing

BOOL CKingimageView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CKingimageView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CKingimageView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CKingimageView diagnostics

#ifdef _DEBUG
void CKingimageView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CKingimageView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CKingimageDoc* CKingimageView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CKingimageDoc)));
	return (CKingimageDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CKingimageView message handlers



void CKingimageView::OnInitialUpdate() 
{
	CScrollView::OnInitialUpdate();
	
	// TODO: Add your specialized code here and/or call the base class
	CSize totalSize=CSize(::GetSystemMetrics(SM_CXSCREEN),
		                  ::GetSystemMetrics(SM_CYSCREEN));
	CSize pageSize=CSize(totalSize.cx/2,totalSize.cy/2);
	CSize lineSize=CSize(totalSize.cx/10,totalSize.cy/10);

	SetScrollSizes(MM_TEXT,totalSize,pageSize,lineSize);
}

void CKingimageView::OnMouseMove(UINT nFlags, CPoint point) 
{
	CScrollView::OnMouseMove(nFlags, point);
}


void CKingimageView::OnSobel()
{
	CKingimageDoc* pDoc = GetDocument();

	int iBitPerPixel = pDoc->_bmp->bitsperpixel;
	int iWidth = pDoc->_bmp->width;
	int iHeight = pDoc->_bmp->height;
	BYTE *pImg = pDoc->_bmp->point;

	int Wp = iWidth;
	if (iBitPerPixel == 8)  ////Grey scale 8 bits image
	{
		int r = iWidth % 4;
		int p = (4 - r) % 4;
		Wp = iWidth + p;
	}
	else if (iBitPerPixel == 24)	// RGB image
	{
		int r = (3 * iWidth) % 4;
		int p = (4 - r) % 4;
		Wp = 3 * iWidth + p;
	}

	double *Sobelx = new double[Wp *iHeight];
	double *Sobely = new double[Wp*iHeight];
	double *Sobel = new double[Wp*iHeight];
	int Sx[] = { -1,0,1,-2,0,2,-1,0,1 };
	int Sy[] = { -1,-2,-1,0,0,0,1,2,1 };

	for (int i = 1; i<iHeight - 1; i++)
		for (int j = 1; j<iWidth - 1; j++)
		{
			Sobelx[i*Wp + j] = (pImg[(i - 1)*Wp + (j - 1)] * Sx[0] + pImg[(i - 1)*Wp + j] * Sx[1] + pImg[(i - 1)*Wp + (j + 1)] * Sx[2] + pImg[i*Wp + (j - 1)] * Sx[3] + pImg[i*Wp + j] * Sx[4] + pImg[i*Wp + (j + 1)] * Sx[5] + pImg[(i + 1)*Wp + (j - 1)] * Sx[6] + pImg[(i + 1)*Wp + j] * Sx[7] + pImg[(i + 1)*Wp + (j + 1)] * Sx[8]); //Sobelx	
			Sobely[i*Wp + j] = (pImg[(i - 1)*Wp + (j - 1)] * Sy[0] + pImg[(i - 1)*Wp + j] * Sy[1] + pImg[(i - 1)*Wp + (j + 1)] * Sy[2] + pImg[i*Wp + (j - 1)] * Sy[3] + pImg[i*Wp + j] * Sy[4] + pImg[i*Wp + (j + 1)] * Sy[5] + pImg[(i + 1)*Wp + (j - 1)] * Sy[6] + pImg[(i + 1)*Wp + j] * Sy[7] + pImg[(i + 1)*Wp + (j + 1)] * Sy[8]); //Sobely
			Sobel[i*Wp + j] = (sqrt(Sobelx[i*Wp + j] * Sobelx[i*Wp + j] + Sobely[i*Wp + j] * Sobely[i*Wp + j]));
		}

	for (int i = 1; i<iHeight - 1; i++)
		for (int j = 1; j < iWidth - 1; j++)
		{
			if (Sobel[i*Wp + j] > 255)
				pImg[i*Wp + j] = 255;
			else
				pImg[i*Wp + j] = Sobel[i*Wp + j];
		}
	OnDraw(GetDC());

}

