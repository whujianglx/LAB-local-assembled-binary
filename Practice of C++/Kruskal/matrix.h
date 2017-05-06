#pragma once

#include <iostream.h>
#include <iomanip.h>
#include <typeinfo.h>


//���� CLASS

template <class Type>
class Matrix{
public:    //����
	/* ����д��  */
	enum Method{M_Line,M_List};//�����ȣ�������
	enum Sequence{Geometric,Arismetic};//�ȱȷ������Ȳ��
	/* End ����д�� */

	int line; //����
	int list; //����
	Type * _begin;
	bool Est;
	bool String;
	      //��Ա����
	Matrix(){
		line=0;
		list=0;
		Est=0;
		String=0;
	    if(strcmp(typeid(Type).name(),"char *")==0)
			String=1;

		_begin=NULL;
	}

	
	void CleM(){
		if (Est==1){
			Est=0;
			delete[] this->_begin;
								
		}
		return ;
		
	}
	int NewM(int x,int y,Type c=0){ //����1 ��ʾ��ȷ�½�//error
		int i,j;

		if (Est==1)
			CleM();
		if (x>0 && y>0 ){
			
			line=x;
			list=y;
			_begin=new Type[line*list];

			/*��ʼ��*/
			for (i=0;i<line;i++)
				for (j=0;j<list;j++)
					_begin[list*i+j]=c;	//error
				
			Est=1;
			return 1;
						
			
		}
		else 
			return 0; 
		
	}
    
    Type Value(int x,int y){ //��������ʽ��ĳԪ��
		

		if (Est==1){
			if (x>0 && x<=line)
				if (y>0 && y<=list){
					return _begin[list*(x-1)+y-1];
					
				}
		}
		return (Type) 0;

		
		
	}
	/* ����д�� */
	

	int Write_All(Type Beg=0,Method method=M_Line,Sequence line_sequence=Arismetic,Type line_step=0,Sequence list_sequence=Arismetic,Type list_step=0){
		//��ʼֵ��������/�����ȣ��з������оࣻ�з������о���
		// 1 ��ȷ����
		/* (x,Matrix<T>::M_Line,Matrix<T>::Arismetic,(T)C.list,Matrix<T>::Arismetic,(T)1) 
		       | 1  2  3  4 |
			   | 5  6  7  8 |
			   | 9 10 11 12 |
		                                                    */
		int i,j;
		

		if (Est==1){
			_begin[0]=Beg;


			if (method==M_Line){
				
				for (i=1;i<line;i++){
				
					if (line_sequence==Arismetic)
						_begin[i*list]=_begin[(i-1)*list]+line_step;
					else
						_begin[i*list]=_begin[(i-1)*list]*line_step;
						
				}
				for (j=1;j<list;j++)
					for (i=0;i<line;i++){
						if(list_sequence==Arismetic)
							_begin[i*list+j]=_begin[i*list+j-1]+list_step;
						else
							_begin[i*list+j]=_begin[i*list+j-1]*list_step;
					}
			}
			else if(method==M_List){

				for (i=1;i<list;i++){
					if (list_sequence==Arismetic)
						_begin[i]=_begin[i-1]+list_step;
					else
						_begin[i]=_begin[i-1]*list_step;
				}
				for (j=1;j<line;j++)
					for (i=0;i<list;i++){
						if (line_sequence==Arismetic)
							_begin[j*list+i]=_begin[(j-1)*list+i]+line_step;
						else
							_begin[j*list+i]=_begin[(j-1)*list+i]*line_step;
					}
			}
			return 1;

		
		}	
		return 0;
	}
	/* End ����д�� */
	
	int Write_Array(Type * & con,int num){//����д�� ����1��ʾ��ȷд��
		int i;
		if (Est==1){
			num> line*list ? num=line*list : num=num ;
			for (i=0;i<num;i++)
				_begin[i]=con[i];
			return 1;
		}
		return 0;
	
	
	}

	int Write_In(int x,int y,Type content){  //���� д�� ����1��ʾ��ȷд��
		if (Est==1){
			if (x>0 && x<=line)
				if (y>0 && y<=list){
					_begin[list*(x-1)+y-1]=content;
					return 1;
									
				}
		}
		return 0;
	}


	int Transposition(void){  //����ת�� ����1 ת�óɹ�
		Type *_kn;

		
		int i,j;
		int exchange;

		if (Est==1){
			_kn=new Type[list*line];
				
				for (i=0;i<line;i++)
					for (j=0;j<list;j++)
						_kn[j*line+i]=_begin[i*list+j];

				delete [] _begin;
				
				_begin=_kn;

				exchange=line;
				line=list;
				list=exchange;
				return 1;


				
		
			
		}

		return 0;
	}
	
	
	int Print(int digit=5){//����1 ��ʾ��ȷ��� |����C++����̨
		int i,j;
		
	
		
		if (Est==1){
			cout<<"\n";

			for (i=0;i<line;i++){
				for (j=0;j<list;j++)
					cout<<setw(digit)<<_begin[i*list+j];
				cout<<"\n";
			}
			return 1;
		}
		return 0;
	}

	int Equal_Fac_Normal(Type st,int *  &Dir){// Ѱ����ͬ��ֵ������������
		int * dr=new int[2*line*list];
		int i,j;
		int sp=0;
		
		
		if (Est==1){


			
		for (i=0;i<line;i++)
			for (j=0;j<list;j++)
				if (_begin[i*list+j]==st){
					dr[sp++]=i;
					dr[sp++]=j;
						
				}
			
		
		Dir=new int[sp];
		for (i=0;i<sp;i++)
			Dir[i]=dr[i]+1; //��1�����������ѧ�ϵ�������
		delete[] dr;
		return sp/2;

		}



	    return 0;
	}
	/* char * ���͵Ĳ��Һ��� */
	int Equal_Fac_String(Type st,int * &Dir){
		int * dr=new int[2*line*list];
		int i,j;
		int sp=0;
		
		
		if (Est==1){


			
		for (i=0;i<line;i++)
			for (j=0;j<list;j++)
				if (strcmp(_begin[i*list+j],st)==0){
					dr[sp++]=i;
					dr[sp++]=j;
						
				}
			
		
		Dir=new int[sp];
		for (i=0;i<sp;i++)
			Dir[i]=dr[i]+1; //��1�����������ѧ�ϵ�������
		delete[] dr;
		return sp/2;
		}



	    return 0;
	}
	/* End char * */

	int Se_Max_Min(int * &Dir,bool i=0){//0 �����ֵ��!0 ����Сֵ
		int num=0;
		int j;

		if (Est==1){
			if (!i){//���
				for (j=1 ;j<line*list;j++)
					_begin[num]<_begin[j] ? num=j :num=num ;		
			}
			else
			{
				for (j=1;j<line*list;j++)
					_begin[num]>_begin[j] ? num=j: num=num   ;
		
			}
			return Equal_Fac_Normal(_begin[num],Dir);
		}
		return 0;	
	
	}


    Type Sum_Line(int li){ //���к�
		int i;
		Type sum=(Type) 0;

		if (Est==1){
			if (li>0 && li<=line){
				for (i=0;i<list;i++)
					sum+=_begin[(li-1)*list+i];
				return sum;
			}
		}
		return (Type) 0;
	}

	Type Sum_List(int li){ //���к�
		int i;
		Type sum=(Type) 0;

		if (Est==1){
			if (li>0 && li<=list){
				for (i=0;i<line;i++)
					sum+=_begin[i*list+li-1];
				return sum;
			}
		}
		return (Type) 0;
	}

	Type Sum_All(void){  //��ȫ���Ӻ�
		int i,j;
		Type sum=(Type) 0;

		if (Est==1){
			for (i=0;i<line;i++)
				for (j=0;j<list;j++)
					sum+=_begin[i*list+j];
			return sum;
		}
		return (Type ) 0;
	}
	/*    ���� operator     */
	
	void operator =(Matrix<Type> &ma ){//A=B
		if (&ma==this) return;
		int i;
		
		if (ma.Est==1){
			this->NewM(ma.line,ma.list);
			for (i=0;i<ma.line*ma.list;i++)
				this->_begin[i]=ma._begin[i];
		}
		else
			this->CleM();

		return ;	
	}

	Matrix<Type> operator~(void){// A^T ת��
		Matrix<Type> mAdd;
		mAdd=*this;
		mAdd.Transposition();
		return mAdd;
	}
	Matrix<Type> operator-(void){//-A
		Matrix<Type> mAdd;
		int i;

		if (Est==1){
			mAdd.NewM(line,list);
			for (i=0;i<line*list;i++){
				mAdd._begin[i]=-this->_begin[i];
			}
		}
		return mAdd;	
	}
	Matrix<Type> operator*(Type ma){// A*b
		Matrix<Type> mAdd;
		int i;

		if (Est==1){
			mAdd.NewM(line,list);
			for (i=0;i<line*list;i++)
				mAdd._begin[i]=this->_begin[i]*ma;
		}

		return mAdd;
	}
	Matrix<Type> operator*(Matrix<Type> &ma){// A*B
		Matrix<Type> mAdd;
		int i,j;

		if ((Est & ma.Est)==1){
			if (list==ma.line){
				mAdd.NewM(line,ma.list);
				for (i=0;i<line;i++)
					for (j=0;j<ma.list;j++)
						mAdd._begin[i*ma.list+j]=Matrix_Mul(*this,ma,list,ma.list,i,j);
			}
		}

		return mAdd;
	}
	Matrix<Type> operator-(Type ma){//A-b
		Matrix<Type> mAdd;
		int i;

		if (Est==1){
			mAdd.NewM(line,list);
			for (i=0;i<line*list;i++)
				mAdd._begin[i]=this->_begin[i]-ma;
		}
		return mAdd;
	}
	Matrix<Type> operator-(Matrix<Type> &ma){//A-B
		Matrix<Type> mAdd;
		int i,j;

		if (Est==1){
			if (ma.Est==1){
				if (line==ma.line && list==ma.list){
					mAdd.NewM(line,list);
					for (i=0;i<line*list;i++)
						mAdd._begin[i]=this->_begin[i]-ma._begin[i];
					return mAdd;
				}
			}
			mAdd=*this;
		}
		else if (ma.Est==1)
			mAdd=ma;

		return mAdd;	
	
	}
	Matrix<Type> operator/(Type ma){// A/b
		Matrix<Type> mAdd;
		int i;

        if (Est==1){
			mAdd.NewM(line,list);
			if (ma==(Type)0)
				mAdd=*this;
			else
				for (i=0;i<line*list;i++)
					mAdd._begin[i]=this->_begin[i]/ma;
		}
		return mAdd;
	}
	Matrix<Type> operator+(Type ma){//A+b
		Matrix<Type> mAdd;
		int i;
		
		if (Est==1){
			mAdd.NewM(line,list);
			for (i=0;i<line*list;i++)
				mAdd._begin[i]=this->_begin[i]+ma;
		}
		return mAdd;
	}

	Matrix<Type> operator+(Matrix<Type> &ma){ //A+B
		Matrix<Type> mAdd;
		int i,j;

		if (Est==1){
			if (ma.Est==1){
				if (line==ma.line && list==ma.list){
					mAdd.NewM(line,list);
					for (i=0;i<line*list;i++)
						mAdd._begin[i]=this->_begin[i]+ma._begin[i];
					return mAdd;
				}
			}
			mAdd=*this;
		}
		else if (ma.Est==1)
			mAdd=ma;

		return mAdd;	
	}
	/* End ���� operator  */



	//����ʽ���ֹ��� 
	
	Type Trace(void){ //��
		int i;
		Type sum=(Type) 0;

		if (Est==1){
			if (line==list){
				for(i=0;i<line;i++)
					sum+=_begin[i*list+i];
				return sum;
								
			}
		}
		return (Type) 0;
	}
	double Determinant_Value(void){//double ���� |������ʽ��ֵ
		double sum=(Type) 0;
		
		
		if (Est==1){
			if (line==list){
				int *i=new int[line];
				int j;
				for (j=0;j<line;j++)
					i[j]=j;
				sum=Seq(i,0);
				delete[] i;
				return sum;
				
			}
		}
		return -404;//����
	}

	//���󲿷ֹ���
    
	int E_Exchange(int i,int j,Method met=M_Line){//���л��� ����1�ɹ�
		int k;
		Type exc;

		i--;
		j--;

		if (Est==1){
			if (met==M_Line){
				if (i>=0 && i<line)
					if(j>=0 && j<line){
						for (k=0;k<list;k++){
							exc=_begin[i*list+k];
							_begin[i*list+k]=_begin[j*list+k];
							_begin[j*list+k]=exc;
						}
						return 1;
					}
			}
			else if (met==M_List){
				if (i>=0 && i<list)
					if(j>=0 && j<list){
						for (k=0;k<line;k++){
							exc=_begin[k*list+i];
							_begin[k*list+i]=_begin[k*list+j];
							_begin[k*list+j]=exc;
						}
						return 1;
					}
			}
		}
		return 0;	
	}

	int E_Multiply(int i,Type fac,Method met=M_Line){//�г� ����1�ɹ�
		int k;

		i--;
		if (Est==1){
			if(met==M_Line){
				if (i>=0 && i<line){
					for (k=0;k<list;k++)
						_begin[i*line+k]*=fac;
					return 1;
				}
			}
			else if (met==M_List){
				if (i>=0 && i<list){
					for (k=0;k<line;k++)
						_begin[k*line+i]*=fac;
					return 1;				
				}			
			}
		}
		return 0;	
	}

	int E_Divide(int i,Type fac,Method met=M_Line){//�г� ����1�ɹ�
	    i--;

		if (fac==(Type)0) return 0;//��ֹ��0

		if (Est==1){
			if(met==M_Line){
				if (i>=0 && i<line){
					for (k=0;k<list;k++)
						_begin[i*line+k]/=fac;
					return 1;
				}
			}
			else if (met==M_List){
				if (i>=0 && i<list){
					for (k=0;k<line;k++)
						_begin[k*line+i]/=fac;
					return 1;				
				}			
			}
		}
		return 0;	
	}

	int E_Add_Mul(int i,int j,Type fac=1,Method met=M_Line){// i=i+j*fac ����1�ɹ�
        int k;
	
		i--;
		j--;

		if (Est==1){
			if (met==M_Line){
				if (i>=0 && i<line)
					if(j>=0 && j<line){
						for (k=0;k<list;k++)
							_begin[i*list+k]+=_begin[j*list+k]*fac;
						return 1;
					}
			}
			else if (met==M_List){
				if (i>=0 && i<list)
					if(j>=0 && j<list){
						for (k=0;k<line;k++)
							_begin[k*list+i]+=_begin[k*list+j]*fac;
						return 1;
					}
			}
		}
		return 0;
	}
	int E_Add_Div(int i,int j,Type fac=1,Method met=M_Line){// i=i+j/fac ����1�ɹ�
        int k;
	
		i--;
		j--;

		if (fac==(Type)0) return 0 //���� ��0 ����

		if (Est==1){
			if (met==M_Line){
				if (i>=0 && i<line)
					if(j>=0 && j<line){
						for (k=0;k<list;k++)
							_begin[i*list+k]+=_begin[j*list+k]/fac;
						return 1;
					}
			}
			else if (met==M_List){
				if (i>=0 && i<list)
					if(j>=0 && j<list){
						for (k=0;k<line;k++)
							_begin[k*list+i]+=_begin[k*list+j]/fac;
						return 1;
					}
			}
		}
		return 0;
	}
	Matrix<double>  Echelon_M(char step=0){//���ɽ��ݾ��� ����һ��double�;���
		Matrix<double> mAdd;
		int i;

		if(Est==1){
			mAdd.NewM(line,list);
			for (i=0;i<line*list;i++)
				mAdd._begin[i]=(double)this->_begin[i];
			H(mAdd);
			if (step==0) return mAdd;
			H1(mAdd);
			if (step==1) return mAdd;
			H2(mAdd);
		}
		return mAdd;
	}
	int Rank_M(void){ //����
		Matrix<double> C;
		bool sum;
		int i,j;

		C=this->Echelon_M();
		i=0;
		while (sum!=0){
			if (i==line) {i++;break;}
			sum=0;
			for (j=0;j<list;j++)
				if(C._begin[i*list+j]!=0)
					sum=1;
			i++;
		}
		return i-1;
	}
private:
	/* ������ݻ� */

	void H(Matrix<double> & ma){//���󻯽�����
		int i,j,k;

		double K;
		int min;
		int min_index;
		int step=0;//�̶�λ�õĸ���

        while(1){
			i=step;
			j=0;
			while(j<ma.list  ){
				if(ma._begin[i*ma.list+j]!=0) break;
					j++;
			}
			min=j;
			min_index=i;
			for (i=step+1;i<ma.line;i++){
				j=0;
				while(j<ma.list){
					if(ma._begin[i*ma.list+j]!=0) break; 
					j++;
				}
				if (j<min){
					min=j;
					min_index=i;
				}
			}

			if (min==ma.list) break;

			ma.E_Exchange(step,min_index);//ȷ����׼��
			for (i=step+1;i<ma.line;i++){
				K=ma._begin[i*ma.list+min]/ma._begin[step*ma.list+min]*(-1.0);
				//�мӼ��任
				for (k=0;k<ma.list;k++)
					ma._begin[i*ma.list+k]+=ma._begin[step*ma.list+k]*K;

				if (ma._begin[i*ma.list+min]!=0){
					cout<<"�г˾�ȷ�Ⱦ���"<<endl;
					ma._begin[i*ma.list+min]=0;//��Ϊ��ȷ�����⣬��Ҫ�ֶ�����
				}
			}
			step++;
			if (step==ma.line) break;
		}	
		return ;
	}
	void H1(Matrix<double> & ma){//���󻯽����� ����� ��Ϊ��λһ
		double K;
		int i,j;
		int step;//��ĩһ��
		
		for (i=0;i<line;i++){
			for (j=i;j<list;j++){
				if (ma._begin[i*ma.list+j]!=0) break;
			}
			if (j==list) break;
			step=j;
			K=1.0/ma._begin[i*ma.list+step];
			for (j=step;j<list;j++){
				ma._begin[i*ma.list+j]*=K;
			}
			if (ma._begin[i*ma.list+step]!=1){
				cout<<"��λһ���龫ȷ�Ⱦ���"<<endl;
				ma._begin[i*ma.list+step]=1;			
			}
			//�����ϵ�һ��Ӧ������
		}
		return ;
	}
	void H2(Matrix<double> & ma){//���󻯽����� ������ �õ���������
		double K;
		int i,j,k;
		int step;
		for (i=0;i<line ;i++){
			for (j=i;j<list;j++){
				if (ma._begin[i*ma.list+j]!=0) break;
			}
			if (j==list)
				break;
		}
		j=i-1;//������ĩλ
		while(j>0){
			for (k=0;k<list;k++)
				if (ma._begin[j*ma.list+k]!=0){
					step=k;
					break;
				}
			for (i=0;i<j;i++){
				K=ma._begin[i*ma.list+step];
				for (k=step;k<list;k++)
					ma._begin[i*ma.list+k]-=K*ma._begin[j*ma.list+k];
				if (ma._begin[i*ma.list+step]!=0){//�����ϸñ�������ĵط�Ӧ�ñ�����0
					cout<<"�����������;�ȷ�Ⱦ���"<<endl;
					ma._begin[i*ma.list+step]=0;
				}
			}
			j--;
		}
		return ;
	}
/*  End ������ݻ�  */
	Type Matrix_Mul(Matrix & A,Matrix & B,int step,int step_r,int a,int b){
		Type sum=(Type) 0;
		int i;
		for (i=0;i<step;i++){
			sum+=A._begin[a*step+i]*B._begin[i*step_r+b];
		}
		return sum;
	}
/* Determinant_Value     function */
	int S_Flag(int * &pu){//����
		int sum=0,i,j;

		for(i=0;i<line-1;i++)
			for (j=i+1;j<line;j++)
				if (pu[i]>pu[j])
					sum++;
		if (sum%2==1)
			return -1;
		else
			return 1;
	}
	double S_Out(int * &pu){//Seq ���������
		int i;
		double mul= 1;

		for (i=0;i<line;i++)
			mul*=_begin[i*list+pu[i]];
		mul*=S_Flag(pu);

		return mul;		
	}
		
	double Seq(int * &pu,int stat){
		int i;
		double sum= 0;
		
		int exc;

        //����		
		for (i=stat;i<line;i++){
			exc=pu[stat];
			pu[stat]=pu[i];
			pu[i]=exc;

            if (stat==line-1){
				sum+=S_Out(pu);
				
			}
			else
				sum+=Seq(pu,stat+1);
			
			exc=pu[stat];
			pu[stat]=pu[i];
			pu[i]=exc;		
		
		}
		return sum;
	}
/* End  Determinant_Value     function */

/* strcmp    function     */
	int strcmp(const char * put1,char * put2){
		int i=0;
		while (put1[i]==put2[i]){
			if (put1[i]==0)
				return 0;
			i++;
		}
		return put1[i]-put2[i];
	
	
	
	}
	

	int strcmp(Type put1,Type put2){
		int i=0;
		while (put1[i]==put2[i]){
			if (put1[i]==0)
				return 0;
			i++;
		}
		return put1[i]-put2[i];
/* End strcmp     function */
	
	
	
	}
	

	

	


};