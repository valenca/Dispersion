#include <stdio.h>
#include <stdlib.h>

typedef struct { double x, y; } point_t, *point;

inline int cmp_dbl(double a, double b)
{
  return a<b?-1:a>b?1:0;
}

int cmp_x(const void *a, const void *b) {
  return cmp_dbl( (*(const point*)a)->x, (*(const point*)b)->x );
}

double closestPair(point * vector,int n,int *ip,int *iq){
  if(n<4){
    bruteforce
  }
  int l1,l2;
  int r1,r2;
  *ip=3;
  *iq=4;
  return 1.2;
}

double kDisperse(point * vector,int N,int *marked,int K){
  int i,ip,iq;
  double c;
  if(1){
    c=closestPair(vector,N,&ip,&iq);
    printf("%d %d %f %f %f %f",ip,iq,vector[ip]->x,vector[ip]->y,vector[iq]->x,vector[iq]->y);
    return c;
  }

  for(i=0;i<N;i++){
    printf("%d: %f - %f\n",marked[i],vector[i]->x,vector[i]->y);
  }
  printf("%d\n",K);
  return 0;
}

int main()
{
  int i,N,D,K;
  point pts;
  point *vector;
  int *marked;

  scanf("%d %d %d",&N,&D,&K);  
  pts  = malloc(sizeof(point_t) * N);
  vector = malloc(sizeof(point) * N);
  marked = malloc(sizeof(int *) * N);
  
  for(i=0;i<N;i++){
    scanf("%lf%lf",&pts[i].x,&pts[i].y);
    vector[i] = pts + i;
    marked[i]=i%2;
  }
  
  qsort(vector, N, sizeof(point), cmp_x);
  printf("\n%g\n",kDisperse(vector,N,marked,K));
  return 0;
}
