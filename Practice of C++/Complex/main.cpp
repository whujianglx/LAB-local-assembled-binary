#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "complex.h"

int main(void){
/*mode
	0 xy 
	1 rad
	2 deg
*/

	char s[100];
	Complex ans[4];
	Complex suan1;
	Complex suan2;

	char suan;
	double u,v;
	char mode=0;

	int i;
	for (i=0;i<4;i++)
		ans[i].v=ans[i].u=0;

	printf("��ӭʹ�ø������㹤��\n");
	while (1){
H1:		
		printf("\n������ʲô?\nhelp����\nexit�˳�\n");
		scanf("%s",s);

		if (strcmp(s,"help")==0){
			printf("\n");
			printf("�����ʽ��ǰ׺��������ɣ�\n\ndeg��rad��xyΪǰ׺���ֱ��ʾ�Ƕ��ƻ����ƺ�ֱ�������ʾ,ǰ׺����ʡ��\n\n");
			printf("����Ϊ����double����ʾģ�ͽǶȻ�ʵ�����鲿\nҲ������ans��ans1��ans2��ans3���棬�ֱ��ʾ������һ����ʽ��������ϸ���ʽ������Դ�����\n\n");
			printf("//**  **  **  **  **  **  **// \n���������\n\ndeg 10 30  (ģ��10 �н�30��)\nnext\nans        (�����ϸ���ʽ�Ľ��)\n�������ͣ�\n+          (��������)\n\n�绹�в�����ֱ��ѯ�ʳ���Գorz\n");//��������file
			goto H1;
		}
		else if (strcmp(s,"exit")==0){
			return 0;
		}
		else if (strcmp(s,"deg")==0){
			mode=2;
			scanf("%lf",&u);
			scanf("%lf",&v);
			suan1.in_degree(u,v);
		}
		else if (strcmp(s,"rad")==0){
			mode=1;
			scanf("%lf",&u);
			scanf("%lf",&v);
			suan1.in_radian(u,v);
		}
		else if (strcmp(s,"xy")==0){
			mode=0;
			scanf("%lf",&u);
			scanf("%lf",&v);
			suan1.in_xy(u,v);
		}
		else if (strcmp(s,"ans")==0 || strcmp(s,"ans0")==0){
			suan1=ans[0];
		}
		else if (strcmp(s,"ans1")==0){
			suan1=ans[1];
		}
		else if (strcmp(s,"ans2")==0){
			suan1=ans[2];
		}
		else if (strcmp(s,"ans3")==0){
			suan1=ans[3];
		}
		else{
			u=atof(s);
			scanf("%lf",&v);
			switch (mode){
			case 0:suan1.in_xy(u,v);break;
			case 1:suan1.in_radian(u,v);break;
			case 2:suan1.in_degree(u,v);break;		
			};
		}
		printf("next\n");

		scanf("%s",s);
		if (strcmp(s,"exit")==0){
			return 0;
		}
		else if (strcmp(s,"deg")==0){
			mode=2;
			scanf("%lf",&u);
			scanf("%lf",&v);
			suan2.in_degree(u,v);
		}
		else if (strcmp(s,"rad")==0){
			mode=1;
			scanf("%lf",&u);
			scanf("%lf",&v);
			suan2.in_radian(u,v);
		}
		else if (strcmp(s,"xy")==0){
			mode=0;
			scanf("%lf",&u);
			scanf("%lf",&v);
			suan2.in_xy(u,v);
		}
		else if (strcmp(s,"ans")==0 || strcmp(s,"ans0")==0){
			suan2=ans[0];
		}
		else if (strcmp(s,"ans1")==0){
			suan2=ans[1];
		}
		else if (strcmp(s,"ans2")==0){
			suan2=ans[2];
		}
		else if (strcmp(s,"ans3")==0){
			suan2=ans[3];
		}
		else{
			u=atof(s);
			scanf("%lf",&v);
			switch (mode){
			case 0:suan2.in_xy(u,v);break;
			case 1:suan2.in_radian(u,v);break;
			case 2:suan2.in_degree(u,v);break;		
			};
		}
		/*
	suan 0 +
	suan 1 -
	suan 2 *
	suan 3 /
	*/


		while(1){

			printf("�������ͣ�\n");
			scanf("%s",s);
			if (s[0]=='+'){
				suan=0;
				break;
			}
			else if (s[0]=='-'){
				suan=1;
				break;
			}
			else if (s[0]=='*'){
				suan=2;
				break;
			}
			else if (s[0]=='/'){
				suan=3;
				break;
			}
			else
				continue;
		}
		for (i=2;i>=0;i--){
			ans[i+1]=ans[i];
		}

		switch (suan){
		case 0:
			ans[0]=suan1+suan2;
			break;
		case 1:
			ans[0]=suan1-suan2;
			break;
		case 2:
			ans[0]=suan1*suan2;
			break;
		case 3:
			ans[0]=suan1/suan2;
			break;
		}
		
		printf("%lf+i%lf\n",ans[0].u,ans[0].v);
		ans[0].out_degree(u,v);
		printf("�Ƕ���: %lf��%lf��\n",u,v);
		ans[0].out_radian(u,v);
		printf("������: %lf��%lf\n",u,v);

	


		
	
	
	
	}
	


}