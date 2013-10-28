#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#define DIM 3
#define MAX 100000
#define min(a,b) ((a)<(b))?(a):(b)

double dist(int a,int b);
int comp(double *a, double *b);
double divc(int s,int f,int l);

double points[MAX][2];
double min_d=10000000;
int N,D;
int P1,P2;

int main(){
	int i=0,j;
	/*double m=10000000;*/
	scanf("%d %d",&N,&D);

	
	for(i=0;i<N;i++)
		for(j=0;j<D;j++)
			scanf("%lf",&points[i][j]);

	qsort(points,N,sizeof(double[2]),(int(*)(const void*,const void*))comp);
	
	/*
	for(i=0;i<N;i++){
		for(j=i+1;j<N;j++){
			if (dist(i,j)<m){
				m=dist(i,j);
				P1=i;
				P2=j;
			}
		}
	}
	
	printf("%f %d %d\n",m,P1,P2);
	*/
	divc(0,N,0);
	printf("%f %d %d\n",min_d,P1,P2);
	return 0;
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
				if(k==19 || points[j][0]-points[i][0]>=m) break;
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
