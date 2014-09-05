#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include"TBTLib/tbtlib.h"
#include"TBTLib/tbtlib.c"

#define MAX 3000 
#define TREE_TYPE '7'

typedef struct TBTLInfo {
  //coords
  double x;
  double y;

  //closest pair
  double dist;
  struct TBTLInfo *a;
  struct TBTLInfo *b;

  //dispersion
  int marked;
  int rank;
  int id;

  struct TBTLNode *node;
} TBTLInfo;
/* ================================ */

/* ====== TBTLInfo initialization ====== */

TBTLInfo *info_pool;
int info_id;
double max;
int run;

TBTLInfo *new_treeinfo() {
  TBTLInfo *treeinfo = &info_pool[info_id];
  info_id++;
  treeinfo->marked=0;
  treeinfo->dist=MAX;
  treeinfo->rank=0;
  treeinfo->a=NULL;
  treeinfo->b=NULL;
  return treeinfo;
}

int comp(TBTLInfo *p1, TBTLInfo *p2) {
  if(p1->x < p2->x) return -1;
  if(p1->x > p2->x) return 1;
  if(p1->y < p2->y) return -1;
  if(p1->y > p2->y) return 1;
  return 0;
}

/* ===================================== */

double distance(TBTLInfo *a, TBTLInfo *b){
  return sqrt((((a->x)-(b->x))*((a->x)-(b->x))) + (((a->y)-(b->y))*((a->y)-(b->y))));
}

int printTree(TBTLNode *root,int l){
  int i=0;
  if(root->left)
    printTree(root->left,l+1);
  for(i=0;i<l;i++)
    printf("  ");
  printf("%d(%.2f,%.1f) --> %.2f\n",root->info->rank,root->info->x,root->info->y,root->info->dist);
  if(root->right)
    printTree(root->right,l+1);
  return 0;
}

double bruteForceK(TBTLNode *root, int k);
double closestPair(TBTLNode *root,double dmin);
double kDispersePoints(TBTLNode *root,int n,int K);

double bruteForceK(TBTLNode *root, int k){
  TBTLNode *it1,*it2;
  int i,j;
  double tmp;

  root->info->dist=MAX;
  it1=tbtl_get_leftmost_node(root);
  if(it1->minor) it1=it1->minor;
  
  for(i=0;i<k-1;it1=it1->major,i++){
    for(it2=it1->major,j=i+1;j<k;it2=it2->major,j++){
      tmp=distance(it1->info,it2->info);
      if(tmp<root->info->dist){
	root->info->dist=tmp;
	root->info->a=it1->info;
	root->info->b=it2->info;
      }
    }
  }
  return root->info->dist;
}

double closestPair(TBTLNode *root,double dmin){
  TBTLNode *it1,*it2;
  int i=1,k;
  double tmp;
  it1=tbtl_get_leftmost_node(root);
  it2=tbtl_get_rightmost_node(root); 
  
  if(it1->minor) it1=it1->minor;
  for(;it1!=it2;it1=it1->major){
    i++;
  }

  if(i<=5){
    return bruteForceK(root,i);
  }

  tmp=closestPair(root->right,dmin);
  if(tmp<dmin){
    dmin=root->info->dist=tmp;
    root->info->a=root->right->info->a;root->info->b=root->right->info->b;
  }
  
  tmp=closestPair(root->left,dmin);
  if(tmp<dmin){
    dmin=root->info->dist=tmp;
    root->info->a=root->left->info->a;root->info->b=root->left->info->b;
  }
  
  for(it1=root->minor;it1!=NULL;it1=it1->minor){
    k=0;
    for(it2=root;it2!=NULL;it2=it2->major){
      if(k==8 || it2->info->x-it1->info->x >= dmin) break;
      if(abs(it2->info->y-it1->info->y) >= dmin) continue;
      else{
	k++;
	tmp=distance(it1->info,it2->info);
	if(tmp<dmin){
	  dmin=root->info->dist=tmp;
	  root->info->a=it1->info;root->info->b=it2->info;
	}
      }
    }
  }
  return dmin;
}

double kDispersePoints(TBTLNode *root,int n,int K){
  TBTLNode *it;
  TBTLInfo *p,*q;
  double rgt=0,lft=0;

  if (K==n){
    rgt=closestPair(root,MAX);
    if(rgt>max){
      max=rgt;
      run++;
      for(it=tbtl_get_leftmost_node(root);it!=NULL;it=it->major){
	it->info->rank=run;
      }
    }
    return rgt;
  }
  else{
    closestPair(root,MAX);
    p=root->info->a;
    q=root->info->b;
    
    if(p->marked>0 && q->marked>0)
      return 0;
    
    if(p->marked==0){
      tbtl_remove(TREE_TYPE,&root,p->node);	
      lft=kDispersePoints(root,n-1,K);
      for(;root->up!=NULL;root=root->up);
      tbtl_insert(TREE_TYPE,&root,p,(int(*)(const void*,const void*))comp);
    }
    
    if(q->marked==0){
      tbtl_remove(TREE_TYPE,&root,q->node);
      p->marked++;
      rgt=kDispersePoints(root,n-1,K);
      for(;root->up!=NULL;root=root->up);
      p->marked--;
      tbtl_insert(TREE_TYPE,&root,q, (int(*)(const void*,const void*))comp);      
    }
    
    if(lft>rgt)
      return lft;
    else 
      return rgt; 
  }
}

int main(int argc, char *argv[]) {
  int N,D,K,i;
  TBTLNode *root = NULL,*it;
  TBTLInfo *tmp =NULL;

  //read input head
  scanf("%d %d %d",&N,&D,&K);  

  info_pool = (TBTLInfo *)malloc( sizeof(TBTLInfo)*N);  

  for(i=0;i<N;i++){
    tmp=new_treeinfo();
    scanf("%lf%lf",&tmp->x,&tmp->y);
    tmp->node = tbtl_insert(TREE_TYPE, &root, tmp, (int(*)(const void*,const void*))comp);
    if(!tmp) i--;    
  }
  
  for(i=0,it=tbtl_get_leftmost_node(root);it!=NULL;it=it->major){
    it->info->id=i++;
  }
  
  printf("%.5f\n",kDispersePoints(root,N,K));
  for(;root->up!=NULL;root=root->up);
  /*
  for(it=tbtl_get_leftmost_node(root);it!=NULL;it=it->major){
    if(it->info->rank==run){
      printf("%d:(%f,%f)\n",it->info->id,it->info->x,it->info->y);
    }
  }
  */
  return 0;
}

