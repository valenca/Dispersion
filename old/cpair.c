#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include"tree.c"

#define DIM 2
#define MAX 100000

#define min(a,b) ((a)<(b))?(a):(b)

double dist(int a,int b);
int comp(double *a, double *b);
double divc(int s,int f,int l);
void calcK();
void rec(node_ptr h);

void removePoint(int p);

double points[MAX][DIM];
double min_d=10000000;
int N,D,K,P;
int P1,P2;


int main(){
  int i,j;
	
	scanf("%d %d %d",&N,&D,&P);
	
	for(i=0;i<N;i++)
		for(j=0;j<D;j++)
			scanf("%lf",&points[i][j]);
	calcK();
	qsort(points,N,sizeof(double[2]),(int(*)(const void*,const void*))comp);
	
	for(i=N-P;i>0;i--){
		min_d=100000;
		divc(0,N,0);	
		removePoint(P1);
	}
	
	for(i=0;i<N;i++){
		printf("%f %f\n",points[i][0],points[i][1]);
	}
	min_d=1000000;
	divc(0,P,0);
	
	printf("%f %d %d\n",min_d,P1,P2);
	return 0;
}

void removePoint(int p){
	int i,j;
	for(i=p;i<N;i++)
		for(j=0;j<DIM;j++)
			points[i][j]=points[i+1][j];
	N--;
}

void calcK(){
	K=pow(3,DIM)-pow(3,DIM-1);
	return;
}

void rec(node_ptr h){
	
}

double divc(int s,int f,int l){
	int i,j,k,d;
	double m=100000;
	if (f==s+3){
		if(min_d>dist(s,s+2)){
			min_d=dist(s,s+2);
			P1=s;
			P2=s+2;
		}
		if(min_d>dist(s,s+1)){
			min_d=dist(s,s+1);
			P1=s;
			P2=s+1;
		}
		if(min_d>dist(s+1,s+2)){
			min_d=dist(s+1,s+2);
			P1=s+1;
			P2=s+2;
		}
		return min_d;
	}
	else if(f==s+2){
		if(min_d>dist(s,s+1)){
			min_d=dist(s,s+1);
			P1=s;
			P2=s+1;
		}
		return min_d;
	}
	else{
		d=s+(f-s)/2;
		divc(s,d,l);
		divc(d,f,l+1);

		for(i=d-1;i>=s;i--){
			k=0;
			for(j=d;j<f;j++){
				if(k>K || points[j][0]-points[i][0]>=m) break;
				if(abs(points[j][1]-points[i][1])>=m) continue;
				else{
					/*m=min(m,dist(i,j));*/
					if(min_d>dist(i,j)){
						min_d=dist(i,j);
						P1=i;
						P2=j;
					}
					k++;
				}
			}
		}
		return min_d;
	}
}

int comp(double *a, double *b){
	int i;
	for(i=0;i<D;i++){
		if (a[i] > b[i]) return 1;
		if (a[i] < b[i]) return -1;
	}
	return 0;
}

double dist(int a,int b){
	int i;
	double pre=0;
	for(i=0;i<DIM;i++){
		pre+=(points[a][i]-points[b][i])*(points[a][i]-points[b][i]);
	}
	return sqrt(pre);
}
