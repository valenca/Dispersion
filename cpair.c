#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#define MAX 100000
#define min(a,b) ((a)<(b))?(a):(b)

double dist(double a[2],double b[2]);
int comp(double *a, double *b);
double divc(double v[MAX][2],int s,int f,int l);

double points[MAX][2];
double min_d;
int N,D;
int P1,P2;

int main(){
  int i=0,j;
  double m=1000000;
  scanf("%d %d",&N,&D);
  
  for(i=0;i<N;i++)
    for(j=0;j<D;j++)
      scanf("%lf",&points[i][j]);
  

  qsort(points,N,sizeof(double[2]),(int(*)(const void*,const void*))comp);
  
  for(i=0;i<N;i++){
    for(j=i+1;j<N;j++){
      m=min(m,dist(points[i],points[j]));
    }
  }

  printf("%f\n",m);
  
  printf("%f\n",divc(points,0,N,0));

  return 0;
}

double divc(double v[MAX][2],int s,int f,int l){
  int i,j,k,d;
  double m=100000;
  if (f==s+3){
    return min(dist(v[s],v[s+2]),min(dist(v[s],v[s+1]),dist(v[s+1],v[s+2])));
  }
  else if(f==s+2){
    return dist(v[s],v[s+1]);
  }
  else{
    d=s+(f-s)/2;
    m=min(divc(v,s,d,l),divc(v,d,f,l+1));
    
    for(i=d-1;i>=s;i--){
      k=0;
      for(j=d;j<f;j++){
	if(k==7  || v[j][0]-v[i][0]>=m) break;      
	if(abs(v[j][1]-v[i][1])>=m) continue;
	else{
	  m=min(m,dist(v[i],v[j]));
	  k++;
	}
      }
    }
    return m;
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

double dist(double a[2],double b[2]){
  return sqrt(((a[0]-b[0])*(a[0]-b[0]))+((a[1]-b[1])*(a[1]-b[1])));
}
