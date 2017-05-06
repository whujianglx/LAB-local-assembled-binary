// MyView.cpp : implementation of the CMyView class
//

#include "stdafx.h"
#include "My.h"
#include "math.h"

#include "MyDoc.h"
#include "MyView.h"
#include "NewP.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define K 500
#define PI 3.14159265359

/////////////////////////////////////////////////////////////////////////////
// CMyView

IMPLEMENT_DYNCREATE(CMyView, CView)

BEGIN_MESSAGE_MAP(CMyView, CView)
	//{{AFX_MSG_MAP(CMyView)
	ON_WM_CONTEXTMENU()
	ON_WM_CANCELMODE()
	ON_COMMAND(ID_NEW_POINT, OnNewPoint)
	ON_COMMAND(ID_NEW_POINT1,OnNewPoint1)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyView construction/destruction

CMyView::CMyView()
{
	cmyview_color=RGB(30,30,30);
	cmyview_position.x=cmyview_position.y=-1;
	cmyview_yuandian.x=cmyview_yuandian.y=0;
	cmyview_Q=1;
	count=0;
	scanwidth=10;
	potential_slice=1;

}

CMyView::~CMyView()
{
}

BOOL CMyView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs


	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CMyView drawing

//static function
static CPoint create_cpoint(double xx,double yy){//��double ת���ɾ�ȷ��x,yֵ
	CPoint cp(0,0);

	
	if (xx<0)
		xx-=0.5;
	else if (xx>0)
		xx+=0.5;

	if (yy<0)
		yy-=0.5;
	else if (yy>0)
		yy+=0.5;

	cp.x=(long)xx;
	cp.y=(long)yy;
	
	return cp;

}
static int Lev(double use ,double  edge){//������edge Ϊ���ֵļ�����
	int answer=0;
	while (1){
		if (use>=0 && use<edge) break;
		else if (use<0){
			use+=edge;
			answer--;
		}
		else{
			use-=edge;
			answer++;		
		}	
	}
	return answer;
}
static double getalpha(double x,double y){//��ýǶ�
	double alpha;
	if (x==0) return PI/2;
	
	if (fabs(y/x)<1){
		alpha=atan(y/x);
		if (x<0)
			alpha+=PI;
	}
	else {
		alpha=PI/2-atan(x/y);
		if (y<0)
			alpha+=PI;
	}

	while (alpha>=2*PI)
		alpha-=2*PI;
	
	return alpha;
}

static bool Distance(double x,double y,double ban){//���ڲ� return true �뾶��ban
	if (fabs(x)+fabs(y)>1.414*ban)
		return false;
	else{
		if(sqrt(x*x+y*y)<=ban)
			return true;
		else
			return false;
	}
}

//end static function


double CMyView::Potential(double ix,double iy){//�������
	double potential,distance;
	double x,y;
	double alpha;
	potential=0;

	vector<point_charge>::iterator zig;
	for (zig=TAT.begin();zig<TAT.end();zig++){
		x=zig->x;
		y=zig->y;
		if (fabs(x-ix)<fabs(y-iy)){
			alpha=atan((x-ix)/(y-iy));
			distance=fabs(y-iy)/cos(alpha);
		}
		else
		{
			alpha=atan((y-iy)/(x-ix));
			distance=fabs(x-ix)/cos(alpha);
		}
		potential+=K*zig->Q/distance;
	}
	return potential;
}


void CMyView::Line(double &ix,double &iy,double &alpha){//����糡��
	double ax,ay,distance;
	double x,y;
	ax=ay=0;

	vector<point_charge>::iterator zig;
	for (zig=TAT.begin();zig<TAT.end();zig++){
		x=zig->x;
		y=zig->y;
		if (fabs(x-ix)<fabs(y-iy)){
			alpha=atan((x-ix)/(y-iy));
			distance=fabs(y-iy)/cos(alpha);
		}
		else
		{
			alpha=atan((y-iy)/(x-ix));
			distance=fabs(x-ix)/cos(alpha);
		}
		ax+=(ix-x)*K*zig->Q/(distance*distance*distance);
		ay+=(iy-y)*K*zig->Q/(distance*distance*distance);	
	}
	alpha=getalpha(ax,ay);
	ix=ax;
	iy=ay;
	return;

}

void CMyView::Draw_potential(CDC * pDC)//��������
{

	double check;
	double check1;//�����

	bool reverse;

	int scan;
	double x;
	double y;

	double dianshi;	//����

	
	int level,level1;
	int block,block1;
	int n,npp;
	
	bool bo;
	vector<point_charge>::iterator j;


	
	CPen rt;
	rt.CreatePen(PS_SOLID,1,(unsigned long)0);
	pDC->SelectObject(&rt);

	
	n=(kehuqu.right-kehuqu.left)/scanwidth+1;
	npp=0;//���㿪ʼ

	vector<CPoint> * array_y =new vector<CPoint>[n]; //���õ�   �ڴ����
	
	
	for (scan=kehuqu.left-cmyview_yuandian.x;scan<kehuqu.right-cmyview_yuandian.x;scan+=scanwidth){//ע��x,y��ת��
		y=cmyview_yuandian.y-kehuqu.top;//ע������ת��
		bo=false;

		while (1){
			for (j=TAT.begin();j<TAT.end();j++)	//�ж�λ�� ���ܿ���̫��
				bo|=Distance(j->x-scan,j->y-y,7);//��İ뾶Ĭ��Ϊ7,����Ϊ��ֹ���ܣ�������12
			if (!bo){
				//level=Lev(Potential((double)scan,y),potential_slice);
				break;
			}
			y-=5;
			if (y<cmyview_yuandian.y-kehuqu.bottom) break;
		}

		//�Ѿ��ҵ��˵�һ������Ҫ��ĵ�

		while (y>=cmyview_yuandian.y-kehuqu.bottom){//��������
			check1=Potential((double)scan,y);
			level=Lev(check1,potential_slice);
			
			y-=5;
			bo=false;
			for (j=TAT.begin();j<TAT.end();j++)//λ���ж� ���ܿ���̫��
				bo|=Distance(j->x-scan,j->y-y,7);
			if (!bo){
				check=Potential((double)scan,y);
				level1=Lev(check,potential_slice);
				if (level!=level1){//�������������
					
					if (level1>level)
					{
						y=Line_Dich((double)scan,y+5,y,dianshi=(++level)*potential_slice);
					}
					else
					{
						y=Line_Dich((double)scan,y+5,y,dianshi=level*potential_slice);
					}
					//������µ�yֵ�����Ӹõ㿪ʼ��������

					//level=level1;//����level

					//������Ҫ�ж��Ƿ����ظ���
					vector<CPoint>::iterator cp;
					for (cp=array_y[npp].begin();cp<array_y[npp].end();cp++){
						if (Distance(cp->x-scan,cp->y-y,4))//�ж��������Ϊ6
							break;
					}
					if (cp!=array_y[npp].end()) continue;
					//End Judge

					

   					pDC->MoveTo(create_cpoint(cmyview_yuandian.x+scan,cmyview_yuandian.y-y));//�滭����ʼ��

					double prep_y;prep_y=y;//����yֵ
					double temp_diff;
					double ax,ay,alpha;
					vector<CPoint>::iterator sc;

					CPoint stts;
					
					//Add
					reverse=false;
					x=scan;
					block=-1;
					while(1){
						ax=x;ay=y;
						Line(ax,ay,alpha);
						alpha=alpha+PI/2;
						x=x+cos(alpha);
						y=y+sin(alpha);
						block1=Lev(x+cmyview_yuandian.x-kehuqu.left,scanwidth);//��������
						if (block1!=block && block>=0 ){//����ɨ���ߵ�ʱ���ж��Ƿ����֪���غ� ������δ֪����
							if (block1>block){
								stts=create_cpoint(block1*scanwidth-cmyview_yuandian.x,y-(x+cmyview_yuandian.x-block1*scanwidth)*tan(alpha));
								for (sc=array_y[block1].begin();sc<array_y[block1].end();sc++)
									if (Distance(sc->x-x,sc->y-y,1))//�뾶��1��break
										break;
								if (sc==array_y[block1].end())
									array_y[block1].push_back(stts);
								else
									break;
							}								
							else
							{
								stts=create_cpoint(block*scanwidth-cmyview_yuandian.x,y+(block*scanwidth-x-cmyview_yuandian.x)*tan(alpha));
								for (sc=array_y[block].begin();sc<array_y[block].end();sc++)
									if (Distance(sc->x-x,sc->y-y,1))
										break;
								if (sc==array_y[block].end())
									array_y[block].push_back(stts);
								else
									break;
							}
						}
						block=block1;
						
						
						
						//����
						temp_diff=Potential(x,y)-dianshi;
						if (fabs(temp_diff)>0.002*potential_slice){//���ھ�ȷ��Ҫ��
							Reclined_Dich(x,y,alpha-PI/2,dianshi);						
						}
						pDC->LineTo(create_cpoint(cmyview_yuandian.x+x,cmyview_yuandian.y-y));//����
						

						//�ɳ���Ļ��
						if (x>kehuqu.right-cmyview_yuandian.x ||
							x<kehuqu.left-cmyview_yuandian.x)
						{reverse=true;break;}
						else if(y>cmyview_yuandian.y-kehuqu.top ||
							y<cmyview_yuandian.y-kehuqu.bottom)
						{reverse=true;break;}
						//�ɳ���Ļ�� End

						
						
						//ײ����ĵ�
						bo=false;
						for (j=TAT.begin();j<TAT.end();j++)	//�ж�λ�� ���ܿ���̫��
							bo|=Distance(j->x-x,j->y-y,7);//��İ뾶Ĭ��Ϊ7��Ϊ�˷�ֹ���ܣ����õ�12
						if (bo) {reverse=true;break;}
						//ײ����ĵ� End

						//������ѭ���������˳�����


					}
					y=prep_y;//��ԭy
					
					//Another
					if (!reverse) continue;//�ɻ�������ֱ������


					//Another-------------------!!!!!!!!--------
					//�˳���ȫ������
					
					x=scan;
					block=-1;

					//new line
					pDC->MoveTo(create_cpoint(cmyview_yuandian.x+scan,cmyview_yuandian.y-y));//�滭����ʼ��
					while(1){
						ax=x;ay=y;
						Line(ax,ay,alpha);
						alpha=alpha-PI/2;//---****** changed !!!!!!
						x=x+cos(alpha);
						y=y+sin(alpha);
						block1=Lev(x+cmyview_yuandian.x-kehuqu.left,scanwidth);//��������
						if (block1!=block && block>=0 ){//����ɨ���ߵ�ʱ���ж��Ƿ����֪���غ� ������δ֪����
							if (block1>block){
								stts=create_cpoint(block1*scanwidth-cmyview_yuandian.x,y-(x+cmyview_yuandian.x-block1*scanwidth)*tan(alpha));
								for (sc=array_y[block1].begin();sc<array_y[block1].end();sc++)
									if (Distance(sc->x-x,sc->y-y,1))//�뾶��1��break
										break;
								if (sc==array_y[block1].end())
									array_y[block1].push_back(stts);
								else
									break;
							}								
							else
							{
								stts=create_cpoint(block*scanwidth-cmyview_yuandian.x,y+(block*scanwidth-x-cmyview_yuandian.x)*tan(alpha));
								for (sc=array_y[block].begin();sc<array_y[block].end();sc++)
									if (Distance(sc->x-x,sc->y-y,1))
										break;
								if (sc==array_y[block].end())
									array_y[block].push_back(stts);
								else
									break;
							}
						}
						block=block1;
						
						
						
						//����
						temp_diff=Potential(x,y)-dianshi;
						if (fabs(temp_diff)>0.002*potential_slice){//���ھ�ȷ��Ҫ��
							Reclined_Dich(x,y,alpha+PI/2,dianshi);		//---**** changed!!!!!				
						}
						pDC->LineTo(create_cpoint(cmyview_yuandian.x+x,cmyview_yuandian.y-y));//����
						

						//�ɳ���Ļ��
						if (x>kehuqu.right-cmyview_yuandian.x ||
							x<kehuqu.left-cmyview_yuandian.x)
							break;
						else if(y>cmyview_yuandian.y-kehuqu.top ||
							y<cmyview_yuandian.y-kehuqu.bottom)
							break;////
						//�ɳ���Ļ�� End

						
						
						//ײ����ĵ�
						bo=false;
						for (j=TAT.begin();j<TAT.end();j++)	//�ж�λ�� ���ܿ���̫��
							bo|=Distance(j->x-x,j->y-y,7);//��İ뾶Ĭ��Ϊ7��Ϊ�˷�ֹ���ܣ����õ�12
						if (bo) break;
						//ײ����ĵ� End

						//������ѭ���������˳�����

						//�Ѿ���ȫ�����򣬿��Իص��̵�
					}
					y=prep_y;//��ԭy
				}
			}
		}//�������whileѭ��
		npp++;

	}//�������forѭ��

	delete [] array_y;//�����ڴ�

}
void CMyView::Reclined_Dich(double &x,double &y,double alpha,double trigger){//������� ��Ȼ������Bug��Դ��trigger����ȷ,����ԭ�򣺴���ĳ�ʼtrigger
	
	double xx,yy;
	double figure[10];
	double temp;
	
	int index;
	double step=0.1;

	int i;
	int state=-1;
	xx=x;
	yy=y;
	//�ȱ���
	
	if ((temp=Potential(xx,yy)-trigger)>0)
		state=1;//�ԴӸߵ�λ���͵�λΪ��
	
	index=0;
	figure[0]=fabs(temp);
	for (i=1;i<10;i++){		
		temp=Potential(xx+step*i*state*cos(alpha),yy+step*i*state*sin(alpha));
		figure[i]=fabs(temp-trigger);
	}
	for (i=1;i<10;i++){
		if (figure[i]<figure[index])
			index=i;	
	}
	x=xx+step*index*state*cos(alpha);
	y=yy+step*index*state*sin(alpha);

 
}
double CMyView::Line_Dich(double line,double x,double y,double trigger){
	double z;
	double temp;

	temp=(Potential(line,x)-trigger)*(Potential(line,y)-trigger);//���ǻᱨ�����պ���������
	if (temp>0) 
		return 0;
	
	while (1){
	if (fabs(x-y)<=0.01)break;//����һ����ȷ��Ҫ��

	z=(x+y)/2;
	temp=(Potential(line,x)-trigger)*(Potential(line,z)-trigger);
	if (temp<0)
		y=z;
	else
		x=z;
	}
	return z;
}
void CMyView::OnDraw(CDC* pDC)
{
	CMyDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	
	//
	CBitmap * cbitmap=new CBitmap;
	CBitmap * old_cbitmap;
	old_cbitmap=pDC->SelectObject(cbitmap);
	
	CPen * cpen=new CPen;
	CPen * old_cpen;
	cpen->CreatePen(PS_DASH,1,(unsigned long)0);
	old_cpen=pDC->SelectObject(cpen);


	pDC->MoveTo(CPoint(0,cmyview_yuandian.y));
	pDC->LineTo(CPoint(kehuqu.right,cmyview_yuandian.y));

	pDC->MoveTo(CPoint(cmyview_yuandian.x,0));
	pDC->LineTo(CPoint(cmyview_yuandian.x,kehuqu.bottom));

	cpen->DeleteObject();//ɾ�����߱�
	cpen->CreatePen(PS_NULL,1,(unsigned long)0);
	pDC->SelectObject(cpen);

	

	vector<point_charge>::iterator j;//���õ�����
	CBrush * cbrush=new CBrush;
	CBrush * old_cbrush;

	cbrush->CreateSolidBrush(0);
	old_cbrush=pDC->SelectObject(cbrush);//����

	int huan_x,huan_y;

	for (j=TAT.begin();j<TAT.end();j++){//����
		cbrush->DeleteObject();
		cbrush->CreateSolidBrush(j->color);
		pDC->SelectObject(cbrush);
		huan_x=j->x+cmyview_yuandian.x;
		huan_y=-j->y+cmyview_yuandian.y;
		pDC->Ellipse(huan_x-7,huan_y-7,huan_x+7,huan_y+7);
	}

	//��������
	pDC->Rectangle(10,10,20,20);
	Draw_potential(pDC);
		

	//��ԭ
	pDC->SelectObject(old_cbitmap);
	pDC->SelectObject(old_cpen);//��ԭ���߻���
	pDC->SelectObject(old_cbrush);
	delete cpen;
	delete cbitmap;
	delete cbrush;
	

}

/////////////////////////////////////////////////////////////////////////////
// CMyView printing

BOOL CMyView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CMyView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CMyView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CMyView diagnostics

#ifdef _DEBUG
void CMyView::AssertValid() const
{
	CView::AssertValid();
}

void CMyView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMyDoc* CMyView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMyDoc)));
	return (CMyDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMyView message handlers

void CMyView::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	CMenu menuPopup;
	POINT pp;
	pp.x=point.x;
	pp.y=point.y;
//��Ļ �ͻ���ת��	
	ScreenToClient(&pp);
	cmyview_position.x=pp.x;
	cmyview_position.y=pp.y;

	
	menuPopup.CreatePopupMenu();
	menuPopup.AppendMenu(0,ID_NEW_POINT,"�½����\tCtrl+Z");
	menuPopup.TrackPopupMenu(TPM_LEFTALIGN,point.x,point.y,this);
	
	
}

void CMyView::OnCancelMode() 
{
	CView::OnCancelMode();
	
	// TODO: Add your message handler code here
	
}

void CMyView::OnNewPoint() 
{
	NewP Dialog1;
	CString temp2;

	Dialog1.who_do_it=1;
	Dialog1.newp_color=this->cmyview_color;
	temp2.Format("%d",count+1);
	Dialog1.newp_name=CString("P")+temp2;
	
	//ȷ������λ��
	Dialog1.newp_position.x=cmyview_position.x-cmyview_yuandian.x;
	Dialog1.newp_position.y=-cmyview_position.y+cmyview_yuandian.y;
	cmyview_position.x=cmyview_position.y=-1;//��λ
	Dialog1.newp_Q=cmyview_Q;


	if(Dialog1.DoModal()==IDOK){
		point_charge S;
		S.x=Dialog1.newp_position.x;
		S.y=Dialog1.newp_position.y;
		cmyview_Q=S.Q=Dialog1.newp_Q;//��ȡQ
		cmyview_color=S.color=Dialog1.newp_color;//��ȡ��ɫ
		S.name=Dialog1.newp_name;
		TAT.push_back(S);
		count++;
		//��������
		

		//�ػ���

		int huan_x,huan_y;

		huan_x=S.x+cmyview_yuandian.x;
		huan_y=-S.y+cmyview_yuandian.y;
		//InvalidateRect(CRect(huan_x-7,huan_y-7,huan_x+7,huan_y+7));
		Invalidate();
	
	}
}
void CMyView::OnNewPoint1()
{
	NewP Dialog1;
	CString temp2;

	Dialog1.who_do_it=2;
	Dialog1.newp_color=this->cmyview_color;
	Dialog1.newp_Q=cmyview_Q;
	temp2.Format("%d",count+1);
	Dialog1.newp_name=CString("P")+temp2;

	
	if(Dialog1.DoModal()==IDOK){
		point_charge S;
		S.x=Dialog1.newp_position.x;
		S.y=Dialog1.newp_position.y;
		cmyview_Q=S.Q=Dialog1.newp_Q;//��ȡQ
		cmyview_color=S.color=Dialog1.newp_color;//��ȡ��ɫ
		TAT.push_back(S);
		count++;
		//�ػ���

		int huan_x,huan_y;

		huan_x=S.x+cmyview_yuandian.x;
		huan_y=-S.y+cmyview_yuandian.y;
		//InvalidateRect(CRect(huan_x-7,huan_y-7,huan_x+7,huan_y+7));
		Invalidate();
		
	}
}

void CMyView::OnSize(UINT nType, int cx, int cy) 
{
	CView::OnSize(nType, cx, cy);
	

	GetClientRect(&kehuqu);

	cmyview_yuandian.x=(kehuqu.right-kehuqu.left)/2;
	cmyview_yuandian.y=(kehuqu.bottom-kehuqu.top)/2;
	//����λ�ò�redraw

}
