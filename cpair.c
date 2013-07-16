#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#define MAX 10000
#define min(a,b) ((a)<(b))?(a):(b)

double points[MAX][2];
double min_d;
int N,D;
int P1,P2;

int comp(double *a, double *b);
double divconq(int s,int f);
double lrmin(int s,int f);
double dist(double a[2],double b[2]);

int main(){
  int i=0;
  scanf("%d %d",&N,&D);
  for(i=0;scanf("%lf %lf",&points[i][0],&points[i][1])!=-1;i++);

  qsort(points,N,sizeof(double[2]),(int(*)(const void*,const void*))comp);

  divconq(0,N);

  printf("%f\n",min_d);
  return 0;
}

double divconq(int s, int f){
  double m,r,l;
  if(s<f-1){
    l=divconq(s,f/2);
    r=divconq(f/2,f);
    
    m=min(l,r);
    
    m=min(m,lrmin(s,f));
  }
  return m;
}

double lrmin(int s,int f){
  int i,j,k,m;
  double tmp;
  m=(f-s)/2;
  for(i=s;i<m;i++){
    k=0;
    for(j=m;j<f;j++){
      if(k==8 || points[j][0]-points[i][0]>=min_d) break;
      if(abs(points[j][1]-points[i][1])>=min_d) continue;
      else{
	k++;
	tmp=dist(points[i],points[j]);
	if(tmp<min_d){
	  min_d=tmp;
	  P1=i;P2=j;
	}
      }
    }
  }
  return 0;
}

int comp(double *a, double *b){
  int i;
  for(i=0;i<D;i++){
    if (a[i] > b[i]) return 1;
    if (a[i] < b[i]) return -1;
  }
  return 0;
}

double dist(double a[2],double b[2]){
  return sqrt(((a[0]-b[0])*(a[0]-b[0]))+((a[1]-b[1])*(a[1]-b[1])));
}

