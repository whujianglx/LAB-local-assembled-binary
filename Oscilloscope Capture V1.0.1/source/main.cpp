#include "stdio.h"
#include "highgui.h"
#include "cv.h"
#include "math.h"

#define CV_GET_SEQ_ELEMENT(a,b,c)  *((a *)cvGetSeqElem(b,c))

#define WIDTH  920
#define HEIGHT 690

int N;


void Judge(CvSeq * fp,IplImage * img_information,CvPoint  st[]);
void DB(IplImage * img,float contrast,float brightness);
void Inf(IplImage * img,float mean[],float st_deviation[] );
void Cal(IplImage * img,float * contrast,float * brightness,float mean,float st_deviation);

int main(void){

	unsigned char Err=0;

	CvMat * Cha=cvCreateMat(3,3,CV_32FC1);
	CvMat * Cha2=cvCreateMat(2,3,CV_32FC1);
	CvSize cvsz;
	float fx,fy;//�ü�ϵ��
	int index;double MAX;
	CvSeq * contours, * contour,* maxcontour;
	CvPoint st[4];
	CvPoint2D32f  QuS[4],QuT[4];//Source Target
	CvMemStorage* storage=cvCreateMemStorage(0);
	char PATH[10];
	char PATHH[10];
	PATH[1]='.',PATHH[1]='P',PATHH[2]='.';
	PATH[2]=PATHH[3]='j';
	PATH[3]=PATHH[4]='p';
	PATH[4]=PATHH[5]='g';
	PATH[5]=PATHH[6]=0;

	//��ʼ���ü�ϵ��
	fx=0.012;
	fy=0.02;

	//��ӭ����
	printf("��ӭʹ��ʾ����ͼ��ץͼ���\n");
	printf("��ʾ�������ļ����ﰲ���ļ���������:\n");
	printf("1.jpg   2.jpg   3.jpg  ...");
	printf("�ܹ�����1-N?:  ");
	scanf("%d",&N);

	int ii;

	for ( ii=1;ii<N+1;ii++){
	PATH[0]=ii+'0';
	IplImage* img_pre=cvLoadImage(PATH);//����ͼƬ
	
	if (img_pre==NULL){
		printf("Picture %d Is Not Found!\n",ii);
		continue;
	}

	IplImage * img=cvCreateImage(cvSize(960,720),IPL_DEPTH_8U,3);//��ô���ǰͼ��
	
	
	//Ԥ����
	QuS[0].x=0;QuS[0].y=0;
	QuS[1].x=img_pre->width;QuS[1].y=0;
	QuS[2].x=0;QuS[2].y=img_pre->height;

	QuT[0].x=0;QuT[0].y=0;
	QuT[1].x=960;QuT[1].y=0;
	QuT[2].x=0;QuT[2].y=720;

	cvGetAffineTransform(QuS,QuT,Cha2);
	cvWarpAffine(img_pre,img,Cha2);	

	//����img_pre
	cvReleaseImage(& img_pre);
	//������
	cvsz=cvGetSize(img);//��ȡ�ߴ�

	IplImage * img2=cvCreateImage(cvsz,IPL_DEPTH_8U,1);//�����Ҷ�ͼ��
	//Debug

	IplImage * img3=cvCreateImage(cvsz,IPL_DEPTH_8U,1);//������ֵͼ��
	IplImage * img_out=cvCreateImage(cvsz,IPL_DEPTH_8U,1);//������λͼ��
	IplImage * img_ans=cvCreateImage(cvSize(WIDTH,HEIGHT),IPL_DEPTH_8U,3);//�����������
	IplImage * img_ans2=cvCreateImage(cvSize(WIDTH,HEIGHT),IPL_DEPTH_8U,3);//�����������
	//��ʼ���������ͼƬ

	img_ans->origin=img->origin;
	cvZero(img_ans);
	//
	cvCvtColor(img, img2, CV_BGR2GRAY);//ת���Ҷ�ͼ��
	
	//
	cvZero(img_out);
	
	if (Err){//����ʱ����
		Err=2;
		cvErode(img2,img2,NULL,1);
	}
	
	
	cvAdaptiveThreshold(img2,img3,255,CV_ADAPTIVE_THRESH_MEAN_C,CV_THRESH_BINARY,7);//ת��
	//����img2
	cvReleaseImage(&img2);

	//
	cvClearMemStorage(storage);//���ڴ�
	cvFindContours(img3,storage,&contours);//�����������ұ�����contours��
		
	

	index=0;
	MAX=0;
	maxcontour=NULL;
	for (contour=contours;contour!=NULL;contour=contour->h_next,index++) // Error
    {	  
		double Area=cvContourArea(contour);//����������
		if (Area>MAX){//�����������ָ��
			maxcontour=contour;
			MAX=Area;
		}
	}		
	
	//Self_Adapting
	if (MAX<60000){//�Զ�������
		if (Err==2)//������Զ�����
		{
			cvReleaseImage(&img3);
			cvReleaseImage(&img_out);
			cvReleaseImage(&img_ans);
			cvReleaseImage(&img_ans2);
			printf("Picture %d Failed!\n",ii);
			Err=0;
			continue;
		}
		else{
			Err=1;
			cvReleaseImage(&img3);
			cvReleaseImage(&img_out);
			cvReleaseImage(&img_ans);
			cvReleaseImage(&img_ans2);
			printf("Picture %d Retry!\n",ii);
			ii--;
			continue;
		}		
	}
	else
		if (Err==2)//�ɹ����BUG
			Err=0;
	//End Self_Adapting

	Judge(maxcontour,img_out,st);//�Զ����ұ�Ե�㺯��

	//��ʼ������������,���ҽ���͸�ӱ任
	QuS[0].x=st[0].x;QuS[0].y=st[0].y;
	QuS[1].x=st[1].x;QuS[1].y=st[1].y;
	QuS[2].x=st[2].x;QuS[2].y=st[2].y;
	QuS[3].x=st[3].x;QuS[3].y=st[3].y;
	QuT[0].x=QuT[0].y=0;
	QuT[1].x=WIDTH;QuT[1].y=0;
	QuT[2].x=0;QuT[2].y=HEIGHT;
	QuT[3].x=WIDTH,QuT[3].y=HEIGHT;

	cvGetPerspectiveTransform(
			QuS,
			QuT,
			Cha
		);
	cvWarpPerspective(img,img_ans,Cha);
	//��ʼ���з���任
	QuS[0].x=fx*WIDTH;QuS[0].y=fy*HEIGHT;
	QuS[1].x=(1-fx)*WIDTH;QuS[1].y=fy*HEIGHT;
	QuS[2].x=(1-fx)*WIDTH;QuS[2].y=(1-fy)*HEIGHT;
		
	QuT[0].x=0;QuT[0].y=0;
	QuT[1].x=WIDTH;QuT[1].y=0;
	QuT[2].x=WIDTH;QuT[2].y=HEIGHT;
	
	cvGetAffineTransform(QuS,QuT,Cha2);

	cvWarpAffine(img_ans,img_ans2,Cha2);
	cvXorS(img_ans2, cvScalarAll(255), img_ans2);
	
	//INSERT 2
	float contrast;
	float brightness;
	Cal(img_ans2,&contrast,&brightness,220,75);
	DB(img_ans2,contrast,brightness);


	//Save The Picture
	printf("Picture %d Succeed!\n",ii);
	PATHH[0]='0'+ii;
	cvSaveImage(PATHH,img_ans2);
	//Release
	cvReleaseImage(&img3);
	cvReleaseImage(&img_out);
	cvReleaseImage(&img_ans);
	cvReleaseImage(&img_ans2);
	}
	
	cvReleaseMemStorage(&storage);
	cvReleaseMat(&Cha);
	printf("Finish   Press Any Key To Exit\n");
	getchar();
	getchar();
	return 0;

}

void Judge(CvSeq * fp,IplImage * img_information,CvPoint  st[]){
	int x=img_information->width;
	int y=img_information->height;
	int M=fp->total;
	int i;
	int factor=-0.7;//����Ӱ������
	CvPoint ic;
	double MI[4]={1e9,1e9,1e9,1e9};
	double r2;

	for (i=0;i<M;i++){
		ic=CV_GET_SEQ_ELEMENT(CvPoint,fp,i);//���CvPoint����
		if(ic.x<x/2 && ic.y<y/2)//��һ��
		{
			r2=fabs(ic.x)+fabs(ic.y)+factor*sqrt(ic.x*ic.x+ic.y*ic.y);
			if(r2<MI[0]){
				st[0]=ic;
				MI[0]=r2;
			}
		}
		else if(ic.y<y/2)//�ڶ���
		{
			r2=fabs(ic.x-x)+fabs(ic.y)+factor*sqrt(pow(x-ic.x,2)+pow(ic.y,2));
			if (r2<MI[1]){
				st[1]=ic;
				MI[1]=r2;			
			}		
		}
		else if (ic.x<x/2)//������
		{
			r2=fabs(ic.x)+fabs(ic.y-y)+factor*sqrt(pow(ic.x,2)+pow(y-ic.y,2));
			if (r2<MI[2]){
				st[2]=ic;
				MI[2]=r2;			
			}
		}
		else//������
		{
			r2=fabs(ic.x-x)+fabs(ic.y-y)+factor*sqrt(pow(x-ic.x,2)+pow(y-ic.y,2));
			if (r2<MI[3]){
				st[3]=ic;
				MI[3]=r2;
			}
		}
	}
}
