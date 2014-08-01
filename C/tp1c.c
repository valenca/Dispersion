#include<stdio.h>
#include<stdlib.h>
#include<math.h>


typedef struct point_st{
  int x;
  int y;
}point;

point *quicksort(point *v,int n);
float distance(point a, point b);
point *ccat(point *lft,point mid,point * rgt,int cl,int cr);
void brutef(point *v,int n);
void minimum_distance(point* v,int n);
void lrmin(point *l,point *r,int n);

float min_d=-1;

int main(void){
  int n,i;
  point *v;
  scanf("%d",&n);
  
  if(n<1)
    return 0;
  if(n==1){
    printf("0.00\n");return 0;}

  v=(point*)malloc(n*sizeof(point));
  
  for(i=0;i<n;i++)
    scanf("%d %d", &v[i].x,&v[i].y);
  
  v=quicksort(v,n);

  minimum_distance(v,n);
  
  printf("%#.2f\n",min_d);
  free(v);
  return 0;
}

point *quicksort(point *v,int n){
  int i,cl=0,cr=0,k=0;
  point pivot;
  point *list_l, *list_r,*fin;

  if (n<2) return v;

  list_l=(point*)malloc(n*sizeof(point));
  list_r=(point*)malloc(n*sizeof(point));
  
  pivot=v[n/2+n%2];
  for(i=0;i<n;i++){
    if(k==0 && i==n/2+n%2)
      k++;
    else if(v[i].x>pivot.x)
      list_r[cr++]=v[i];
    else if(v[i].x<=pivot.x)
      list_l[cl++]=v[i];
  }

  list_l=(point*)realloc(list_l,cl*sizeof(point));
  list_r=(point*)realloc(list_r,cr*sizeof(point));

  list_l=quicksort(list_l,cl);
  list_r=quicksort(list_r,cr);

  fin=ccat(list_l,pivot,list_r,cl,cr);
  free(list_r);
  free(list_l);  
  free(v);
  return fin;
}
 
point* ccat(point *lft,point mid,point * rgt,int cl,int cr){
  point *fin;
  int i=0;

  fin=(point*)malloc((cr+cl+1)*sizeof(point));
  for(i=0;i<cl;i++)
    fin[i]=lft[i];
  fin[i++]=mid;
  for(;i<cl+cr+1;i++)
    fin[i]=rgt[i-(cl+1)];
  return fin;
}

void minimum_distance(point* v,int n){
  point *left, *right;
  int i;

  if(n==2 || n==3){
    brutef(v,n);
    return;
  }

  left  = (point*)malloc((n/2)*sizeof(point));
  right = (point*)malloc((n/2+n%2)*sizeof(point));
  
  for(i=0;i<n/2;i++){
    left[i]=v[i];
    right[i]=v[i+n/2];
  }
  if(n%2) right[i]=v[n-1];
  
  minimum_distance(left,n/2);
  minimum_distance(right,n/2+n%2);  
  
  lrmin(left,right,n);

  free(left);
  free(right);
}

void lrmin(point *l,point *r,int n){
  int i,j,k;
  float tmp;
  for(i=0;i<n/2;i++){
    k=0;
    for(j=0;j<n/2+n%2;j++){
      if(k==8 || r[j].x-l[i].x>=min_d) break;
      if(abs(r[j].y-l[i].y)>=min_d) continue;
      else{
	k++;
	tmp=distance(l[i],r[j]);
	if(tmp<min_d) min_d=tmp;
      }
    }
  }
}


void brutef(point *v,int n){
  int i,j;
  float tmp;
  for(i=0;i<n-1;i++){
    for(j=i+1;j<n;j++){
      tmp=distance(v[i],v[j]);
      if(tmp<min_d || min_d ==-1) min_d=tmp;
    }
  }
}

float distance(point a, point b){
  return sqrt(pow(a.x-b.x,2)+pow(a.y-b.y,2));
}
