#include "stdafx.h"

#include "Manipulate.h"
#include "OpenBitmap.h"

#include "UseDoc.h"
#include "UseView.h"

#include "math.h"

#define D 30

int V;
int N;
Acceleration A;

Manipulate::Manipulate(int amount,Acceleration a,CUseView * fp)
	:state(FALSE)
{
	p_p=fp;
	this->amount=amount;
	this->a=a;
	c_p=new Ball[amount];
	int i;
	int rnd;
	for (i=0;i<amount;i++){//����ͼ��
		rnd=randEx(6);
		c_p[i].map=p_p->cbitmap[rnd];
	}
}

Manipulate::~Manipulate(){
	delete [] c_p;
	p_p->KillTimer(1);

}
void Manipulate::Start(void){
	state=TRUE;
	CRect rc;
	p_p->GetClientRect(&rc);
	
	int i;

	for (i=0;i<amount;i++){//����λ�ã��ٶȵ�
		//���ó�ʼλ��
		while(1){
			//rnd �����������������
			
			c_p[i].Position.x=D/2+randEx(rc.right-D);
			c_p[i].Position.y=D/2+randEx(rc.bottom-D);
			if (Judge_Mutual(i,0))
				break;			
		}
		//���ó�ʼ�ٶ�
		c_p[i].Velocity.vx=randEx(10)+randEx(10)*0.1;
		c_p[i].Velocity.vy=randEx(10)+randEx(10)*0.1;
		//�����ٶȷ���
		if(randEx(2))
			c_p[i].Velocity.vx=-c_p[i].Velocity.vx;
		if(randEx(2))
			c_p[i].Velocity.vy=-c_p[i].Velocity.vy;

		//������ʼλ��
		c_p[i].D_Position=c_p[i].Position;
		//���ó�ʼ����
		c_p[i].M=1;
	}
	p_p->SetTimer(1,60,NULL);
	p_p->Invalidate();


}
void Manipulate::UpdateWindow(void){//������Ч��
	CRect rect;
	int i;
	for (i=0;i<amount;i++){//�����µ�λ��
		
		rect.left=c_p[i].D_Position.x-D/2;
		rect.top=c_p[i].D_Position.y-D/2;
		rect.right=c_p[i].D_Position.x+D/2;
		rect.bottom=c_p[i].D_Position.y+D/2;
		p_p->InvalidateRect(rect,TRUE);
		//���Ǿ�ֵ
	
		
		rect.left=c_p[i].Position.x-D/2;
		rect.top=c_p[i].Position.y-D/2;
		rect.right=c_p[i].Position.x+D/2;
		rect.bottom=c_p[i].Position.y+D/2;
		p_p->InvalidateRect(rect,TRUE);
		
		//���¾ɵ�λ��
		c_p[i].D_Position=c_p[i].Position;
	}
	
	return ;
}
BOOL Manipulate::Judge_Mutual(int n,char module){//module 0 ���� module 1 ��ʽ 
	int i;
	switch (module){
	case 0:
		for(i=0;i<n;i++)
			if (Distance(c_p[i],c_p[n],0)<1.415*D)
				if (Distance(c_p[i],c_p[n],1)<D)
					return FALSE;
		break;
				
	case 1:
		char * st;
		st=Tag;
		for (i=n+1;i<amount;i++){
			if (Distance(c_p[i],c_p[n],0)<1.415*D)
				if (Distance(c_p[i],c_p[n],1)<D){
					*(st++)=i+1;
					*(st++)=0;
				}
		}
		if (st!=Tag){
			*st=0;
			return FALSE;
		}
		break;
	};
	return TRUE;
}
double Manipulate::Distance(Ball & x1,Ball & x2,char module)const{//module 0 ֱ�Ǿ��� module 1 ֱ�߾���
	double delta_x,delta_y;

	switch (module){
	case 0:
		delta_x=x1.Position.x-x2.Position.x;
		if (delta_x<0)
			delta_x=-delta_x;
		delta_y=x1.Position.y-x2.Position.y;
		if (delta_y<0)
			delta_y=-delta_y;
		
		return delta_x+delta_y;
		break;
	case 1:
		delta_x=x1.Position.x-x2.Position.x;
		delta_y=x1.Position.y-x2.Position.y;
		return sqrt(delta_x*delta_x+delta_y*delta_y);
		break;
	};
}

BOOL Manipulate::Judge_Wall(int n){//1 ��ǽ 2 ��ǽ 3 ��ǽ 4 ��ǽ 
	CRect rect;
	p_p->GetClientRect(&rect);
	
	char * ty=Tag;
	
	//����Ҫ���Ǳ߽绹Ҫ�����ٶȷ���
	if (c_p[n].Position.x<D/2 && c_p[n].Velocity.vx<0){
		*(ty++)=1;
		*(ty++)=0;
	}
	else if (c_p[n].Position.x>rect.right-D/2 && c_p[n].Velocity.vx>0){
		*(ty++)=3;
		*(ty++)=0;
	}
	if (c_p[n].Position.y<D/2 && c_p[n].Velocity.vy<0){
		*(ty++)=2;
		*(ty++)=0;
	}
	else if (c_p[n].Position.y>rect.bottom-D/2 && c_p[n].Velocity.vy>0){
		*(ty++)=4;
		*(ty++)=0;
	}

	if (ty!=Tag){
		*ty=0;
		return FALSE;
	}
	else
		return TRUE;
}
static double Value(double v1,double v2){//���㳤��
	double sum;

	sum=v1*v1+v2*v2;

	return sqrt(sum);
}

static int Di(double x){//���Ƶ������׺���
	if (x>=0)
		return 1;
	else 
		return -1;
} 
void Manipulate::Touch(int main,int ano){//����������ײ��ĵ���
	struct Direction{
		double i;
		double j;
		double value;
	};

	Direction p1,p2;

	//��mainָ��ano����������ײ����
	p1.i=c_p[ano].Position.x-c_p[main].Position.x;
	p1.j=c_p[ano].Position.y-c_p[main].Position.y;
	p1.value=Value(p1.i,p1.j);
	//��ֱp1����������ײ����
	p2.i=1;
	p2.j=0-p2.i*p1.i/p1.j;
	p2.value=Value(p2.i,p2.j);

	Direction vmI,vm_,vaI,va_;

	//main����������
	vm_.value=(c_p[main].Velocity.vx*p2.i+c_p[main].Velocity.vy*p2.j)/p2.value;
	vm_.i=p2.i*vm_.value/p2.value;
	vm_.j=p2.j*vm_.value/p2.value;

	vmI.i=c_p[main].Velocity.vx-vm_.i;
	vmI.j=c_p[main].Velocity.vy-vm_.j;
	vmI.value=Value(vmI.i,vmI.j);

	//ano����������
	va_.value=(c_p[ano].Velocity.vx*p2.i+c_p[ano].Velocity.vy*p2.j)/p2.value;
	va_.i=p2.i*va_.value/p2.value;
	va_.j=p2.j*va_.value/p2.value;

	vaI.i=c_p[ano].Velocity.vx-va_.i;
	vaI.j=c_p[ano].Velocity.vy-va_.j;
	vaI.value=Value(vaI.i,vaI.j);

	//
	Direction cmp;
	cmp.i=vaI.i-vmI.i;
	cmp.j=vaI.j-vmI.j;
	if (Di(cmp.j)==Di(p1.j)){
		;//δ��ײ
	}
	else{

		
		//���߷��򱣳ֲ���
		c_p[main].Velocity.vx=vm_.i;
		c_p[main].Velocity.vy=vm_.j;

		c_p[ano].Velocity.vx=va_.i;
		c_p[ano].Velocity.vy=va_.j;
		//���Ϸ��߷����ཻ��

		c_p[main].Velocity.vx+=vaI.i;
		c_p[main].Velocity.vy+=vaI.j;

		c_p[ano].Velocity.vx+=vmI.i;
		c_p[ano].Velocity.vy+=vmI.j;
	
	}
}
void Manipulate::Move(double t){
	int i;
	char * ma;
	int ano;
	
	for (i=0;i<amount;i++){//ȫ����λ
		c_p[i].Position.x+=c_p[i].Velocity.vx*t;
		c_p[i].Position.y+=c_p[i].Velocity.vy*t;
		//ȫ�����
		c_p[i].Velocity.vx+=a.ax*t;
		c_p[i].Velocity.vy+=a.ay*t;
	}
		
	for (i=0;i<amount;i++){//ײ����
		if (!Judge_Mutual(i,1)){
			ma=Tag;
			while(1){
				ano=(int )*ma-1;
				Touch(i,ano);
				ma+=2;
				if (*ma==0)
					break;	
			}
		}
	}	
	for (i=0;i<amount;i++){//ײǽ����
		if (!Judge_Wall(i)){
			ma=Tag;
			while (1){
				switch (*ma){
				case 1://��ǽ
				case 3://��ǽ
					c_p[i].Velocity.vx=-c_p[i].Velocity.vx;
					break;
				
					
				case 2://��ǽ
				case 4://��ǽ
					c_p[i].Velocity.vy=-c_p[i].Velocity.vy;				
				}			
				ma+=2;
				if (*ma==0)
					break;
			}
		}
	}
}
