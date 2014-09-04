#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include"TBTLib/tbtlib.h"
#include"TBTLib/tbtlib.c"

#define TREE_TYPE 3
#define THRESHOLD 4
#define min(a,b) ((a)<(b))?(a):(b)


/* ================================ */

/* ====== TBTLInfo initialization ====== */

TBTLInfo *info_pool;
int info_id;
double max;
int N;
int run;
int count;

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
  if(root->right)
    printTree(root->right,l+1);
  for(i=0;i<l;i++) printf("  ");
  if(root->info->dist!=MAX){
    printf("%d(%.2f,%.1f) --> %.2f ",
	   root->info->id,
	   root->info->x,
	   root->info->y,
	   root->info->dist);
    printf(" (%d %d)\n",
	   root->info->a->id,
	   root->info->b->id);
  }
  else
    printf("%d(%.2f,%.1f) --> %.2f\n",
	   root->info->id,
	   root->info->x,
	   root->info->y,
	   root->info->dist);
    
  if(root->left)
    printTree(root->left,l+1);
  return 0;
}

double bruteForceK(TBTLNode *root, int k);
double closestPair(TBTLNode *root);
double kDispersePoints(TBTLNode *root,int n,int K);

double bruteForceK(TBTLNode *root, int k){
  TBTLNode *it1,*it2;
  int i,j;
  double tmp;
  it1=tbtl_get_leftmost_node(root);
  if(it1->minor) it1=it1->minor;
  
  root->info->dist=MAX;
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

double closestPair(TBTLNode *root){
  TBTLNode *it1,*it2,*st,*fn;
  int i=1,k;
  double tmp1,tmp2,tmp3;
  
  tmp1=tmp2=tmp3=0;
  
  if(root->info->dist!=MAX)
    return root->info->dist;

  st=it1=tbtl_get_leftmost_node(root);
  fn=it2=tbtl_get_rightmost_node(root); 
  
  if(it1->minor) it1=it1->minor;
  for(;i<=THRESHOLD && it1!=it2;it1=it1->major){
    i++;
  }

  if(i<=THRESHOLD){
    return bruteForceK(root,i);
  }
  
  tmp1=closestPair(root->right);
  tmp2=closestPair(root->left);

  if(tmp1<tmp2){
    root->info->dist=tmp1;
    root->info->a=root->right->info->a;
    root->info->b=root->right->info->b;
  }
  else{
    root->info->dist=tmp2;
    root->info->a=root->left->info->a; 
    root->info->b=root->left->info->b;
  }

  /*Calculate BORDER region*/  
  for(it1=root->minor;it1!=st->minor;it1=it1->minor){
    k=0;
    for(it2=root;it2!=fn->major;it2=it2->major){
      if(k==8 || (it2->info->x)-(it1->info->x) >= root->info->dist) break;
      if(abs(it2->info->y-it1->info->y) >= root->info->dist) continue;
      else{
	k++;
	tmp3=distance(it1->info,it2->info);
	if(tmp3<root->info->dist){
	  root->info->dist=tmp3;
	  root->info->a=it1->info;root->info->b=it2->info;
	}
      }
    }
  }
  return root->info->dist;
}

double kDispersePoints(TBTLNode *root,int n,int K){
  TBTLNode *it;
  TBTLInfo *p,*q;
  double rgt=0,lft=0;
  
  if (K==n){
    rgt=closestPair(root);
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
    closestPair(root);  
    printf("New Level %d:\n",N-n);
    printTree(root,N-n);
    p=root->info->a;
    q=root->info->b;


    if(p->marked>0 && q->marked>0)
      return 0;
    
    if(p->marked==0){
      printf("Level %d P\n",N-n);
      for(it=p->node;it!=NULL;it=it->up)
	if(it->info->a==p || it->info->b==p)
	  it->info->dist=MAX;	  

      for(it=p->node->major;it!=NULL;it=it->up)
	if(it->info->a==p || it->info->b==p)
	  it->info->dist=MAX;	  
      
      tbtl_remove(TREE_TYPE,&root,p->node);	
      lft=kDispersePoints(root,n-1,K);

      for(;root->up!=NULL;root=root->up);
      tbtl_insert(TREE_TYPE,&root,p,(int(*)(const void*,const void*))comp);
    }
    for(it=p->node;it!=NULL;it=it->up) it->info->dist=MAX;
    for(it=p->node->major;it!=NULL;it=it->up) it->info->dist=MAX;
    
    if(q->marked==0){     
      closestPair(root); 
      printf("Level %d Q\n",N-n);
      for(it=q->node;it!=NULL;it=it->up)
	if(it->info->a==q || it->info->b==q)
	  it->info->dist=MAX;	  
      
      for(it=q->node->major;it!=NULL;it=it->up)
	if(it->info->a==q || it->info->b==q)
	  it->info->dist=MAX;	  
      printTree(root,N-n);
      printf("id: %d %d\n",q->id,p->id);
      tbtl_remove(TREE_TYPE,&root,q->node);
      printTree(root,N-n);
      printf("------------\n");
      p->marked++;
      rgt=kDispersePoints(root,n-1,K);
      p->marked--;

      for(;root->up!=NULL;root=root->up);
      tbtl_insert(TREE_TYPE,&root,q, (int(*)(const void*,const void*))comp);
    }
    for(it=q->node;it!=NULL;it=it->up) it->info->dist=MAX;
    for(it=q->node->major;it!=NULL;it=it->up) it->info->dist=MAX;
    
    if(lft>rgt)
      return lft;
    else 
      return rgt; 
  }
}
int main(int argc, char *argv[]) {
  int D,K,i;
  TBTLNode *root = NULL,*it;
  TBTLInfo *tmp =NULL;

  /*read input head*/
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

  printTree(root,0);
  /*
  for(it=tbtl_get_leftmost_node(root);it!=NULL;it=it->major){
    if(it->info->rank==run){
      printf("%d:(%f,%f)\n",it->info->id,it->info->x,it->info->y);
    }
  }
  */
  /*printf("%d\n",count);*/
  return 0;
}
