#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include"TBTLib/tbtlib.h"
#include"TBTLib/tbtlib.c"

#define MAX 10000 

typedef struct TBTLInfo {
  int mark;
  double x;
  double y;

  double dist;
  
  struct TBTLInfo *a;
  struct TBTLInfo *b;

  struct TBTLNode *node;
} TBTLInfo;
/* ================================ */

/* ====== TBTLInfo initialization ====== */
TBTLInfo *new_treeinfo() {
  TBTLInfo *treeinfo = (TBTLInfo *) calloc(1, sizeof(TBTLInfo));
  treeinfo->mark=0;
  treeinfo->dist=-1;
  treeinfo->a=NULL;
  treeinfo->b=NULL;
  return treeinfo;
}
/* ===================================== */

int comp(TBTLInfo *p1, TBTLInfo *p2) {
  if(p1->x < p2->x) return -1;
  if(p1->x > p2->x) return 1;
  if(p1->y < p2->y) return -1;
  if(p1->y > p2->y) return 1;
  return 0;
}

double distance(TBTLInfo *a, TBTLInfo *b){
  return sqrt((((a->x)-(b->x))*((a->x)-(b->x))) + (((a->y)-(b->y))*((a->y)-(b->y))));
}

int closestPairR(TBTLNode *root);
int bruteForce2(TBTLNode *root);
int bruteForce3(TBTLNode *root);

int bruteForceK(TBTLNode *root, int k){
  TBTLNode *it1,*it2;
  int i,j;
  double tmp;

  root->info->dist=MAX;
  
  it1=tbtl_get_leftmost_node(root)->minor;
  
  for(i=0;i<k-1;it1=it1->major,i++){
    for(it2=it1->major,j=i+1;j<k-1;it2=it2->major,j++){
      tmp=distance(it1->info,it2->info);
      if(tmp<root->info->dist){
	root->info->dist=tmp;
	root->info->a=it1->info;
	root->info->b=it2->info;
      }
    }
  }
  return 0;
}

int printTree(TBTLNode *root,int l){
  int i=0;
  if(root->left)
    printTree(root->left,l+1);
  for(i=0;i<l;i++)
    printf("  ");
  if(root->info->dist!=-1)
    printf("%.1f,%.1f --> %.1f\n",root->info->x,root->info->y,root->info->dist);
  else
    printf("%.1f,%.1f\n",root->info->x,root->info->y);
  if(root->right)
    printTree(root->right,l+1);
  return 0;
}

int main(int argc, char *argv[]) {
  int N,D,K,i;
  TBTLNode *root = NULL, *temp;
  TBTLInfo *tmp =NULL;

  scanf("%d %d %d",&N,&D,&K);  
  
  for(i = 0; i < N; i++) {
    tmp=new_treeinfo();
    scanf("%lf%lf",&tmp->x,&tmp->y);
    temp = tbtl_insert('6', &root, tmp, (int(*)(const void*,const void*))comp);
    if(!temp) {
      i--;
    }
  }

  //printTree(root,0);

  temp=tbtl_get_leftmost_node(root);
  temp->minor=temp;

  bruteForceK(root,N);
  printf("(%f,%f)<>(%f,%f) = %f = %f\n",
	 root->info->a->x,
	 root->info->a->y,
	 root->info->b->x,
	 root->info->b->y,
	 root->info->dist,
	 distance(root->info->a,root->info->b));
  return 0;
}
