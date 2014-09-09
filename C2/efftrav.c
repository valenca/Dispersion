#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#define max(a,b) ((a)>(b))?(a):(b)
#define min(a,b) ((a)<(b))?(a):(b)

#define MAX 10000    /* Max Distance, should be larger than the distance between the furthest points */
#define LEAF_SIZE 4  /* Number of Leafs for each Leafed Node*/

typedef struct Leaf {
  //coords
  double x;
  double y;
  
  //dispersion
  int rank;          /* Indicates the rank of the latest kDisperse set that contains this point */

  int marked;        /* Indicates if node is selected and fixed for the kDisperse set in progress */
  int off;           /* Indicates if node is removed from consideration */

  struct Node *node; /* Parent Node for the Leaf*/
} Leaf;

typedef struct Node{
  struct Node *up;   /* Parent Node */
  struct Node *lft;  /* Left Child Node*/
  struct Node *rgt;  /* Right Child Node*/
 
  int c[LEAF_SIZE];  /* Children in case this node is a leafed node */
  int min;           /* Minimum Index in the subtree */
  int max;           /* Maximum Index in the subtree */
  int level;         /* Depth Level for this node (means leafed node) */

  int a;             /* One of the points that make the closest pair in the subtree */
  int b;             /* The other point that makes the closest pair in the subtree */
  double dist;       /* Distance between a and b*/
}Node;

int comp(const void * a, const void * b){
  Leaf *p1=(Leaf*)a;
  Leaf *p2=(Leaf*)b;
  if(p1->x < p2->x) return -1;
  if(p1->x > p2->x) return 1;
  if(p1->y < p2->y) return -1;
  if(p1->y > p2->y) return 1;
  return 0;
}

int N,D,K;
Node * nodes;
Leaf *leaves;

int count;
int n_nodes;

int run;
double max_disp;

double distance(int a, int b){
  if (a==b) return MAX;
  if (a==-1 || b==-1)    return MAX;
  if (leaves[a].off==1 || leaves[b].off==1) return MAX;
  return sqrt(
	      (((leaves[a].x)-(leaves[b].x))*
	       ((leaves[a].x)-(leaves[b].x)))+
	      (((leaves[a].y)-(leaves[b].y))*
	       ((leaves[a].y)-(leaves[b].y)))
	      );
}

int printTree(Node *root,int level){
  int i=0;
  if(root->lft)
    printTree(root->lft,level+1);
  for(i=0;i<level;i++)
    printf("  ");
  //printf("[%d-%d]: %f (%d,%d) \n",root->min,root->max,root->dist,root->a,root->b);
  printf("[%d-%d]\n",root->min,root->max);
  /*
  int j;
  for(i=0;i<LEAF_SIZE;i++){
    for(j=0;j<level+1;j++)
      printf("  ");
    printf("[%d]\n",root->c[i]);
  }
  */
  if(root->rgt)
    printTree(root->rgt,level+1);
  return 0;
}

Node *genTree(Node *par, int l){
  Node *new;
  int i;

  new=&nodes[--n_nodes];

  new->up=par;
  new->dist=MAX;
  new->level=l;

  if(l>0){
    new->lft=genTree(new,l-1);
    new->min=new->lft->min;
    if(n_nodes){
      new->rgt=genTree(new,l-1);
      new->max=new->rgt->max;
    }
    else
      new->max=new->lft->max;
  }
  else{
    for(i=0;i<LEAF_SIZE;i++){
      if(count>=N) break;
      new->c[i]=count++;
      leaves[new->c[i]].node=new;
    }

    new->min=new->c[0];
    new->max=count-1;
    
    for(;i<LEAF_SIZE;i++)
      new->c[i]=-1;
  }    
  return new;    
}

void insertLeaf(int index){
  Node * it;
  for(it=leaves[index].node;it!=NULL;it=it->up){
    it->dist=MAX;
  }
  leaves[index].node->c[index%LEAF_SIZE]=index;
  leaves[index].off=0;
  return;
}

void removeLeaf(int index){
  Node * it;
  for(it=leaves[index].node;it!=NULL;it=it->up){
    if(it->a == index || it->b==index)
      it->dist=MAX;
  }
  leaves[index].node->c[index%LEAF_SIZE]=-1;
  leaves[index].off=1;
  return;
}

double bruteForceK(Node * root){
  double tmp;
  int i,j;
  if (root->dist!=MAX)
    return root->dist;

  root->dist=MAX;

  for(i=0;i<LEAF_SIZE-1;i++){
    for(j=i;j<LEAF_SIZE;j++){
      tmp=distance(root->c[i],root->c[j]);
      if (tmp<root->dist){
	root->dist=tmp;
	root->a=root->c[i];
	root->b=root->c[j];
      }
    }
  }
  return root->dist;
}

double closestPair(Node * root){
  double ld,rd,md;
  int i,j,k;

  if (!root) 
    return MAX;
  if (root->dist!=MAX)
    return root->dist;
  if(root->level==0)
    return bruteForceK(root);

  ld=closestPair(root->lft);
  if(ld<root->dist){
    root->dist=ld;
    root->a=root->lft->a;
    root->b=root->lft->b;
  }
  
  if (root->rgt==NULL)
    return root->dist;

  rd=closestPair(root->rgt);
  if(rd<root->dist){
    root->dist=rd;
    root->a=root->rgt->a;
    root->b=root->rgt->b;
  }
  
  md=MAX;

  for(i=root->lft->max;i>=root->min;i--){
    k=0;
    if (leaves[i].off==1) continue;
    for(j=root->rgt->min;j<=root->max;j++){
      if(k==8 || leaves[j].x-leaves[i].x >= root->dist) break;
      if(abs(leaves[j].y-leaves[i].y) >= root->dist) continue;
      else{
	k++;
	md=distance(i,j);
	if(md<root->dist){
	  root->dist=md;
	  root->a=i;root->b=j;
	}
      }
    }
  }
  return root->dist;
}

double kDispersePoints(Node * root,int n, int K){
  int i,p,q;
  double rgt=0,lft=0;
  if(K==n){
    rgt=closestPair(root);
    if(rgt>max_disp){
      max_disp=rgt;
      run++;
      for(i=0;i<N;i++)
	if(!(leaves[i].off))
	  leaves[i].rank=run;
    }
    return rgt;
  }
  else{
    closestPair(root);
    p=root->a;
    q=root->b;
    if(leaves[p].marked>0 && leaves[q].marked>0)
      return 0;

    if(leaves[p].marked==0){
      removeLeaf(p);
      lft=kDispersePoints(root,n-1,K);
      insertLeaf(p);
    }
    if(leaves[q].marked==0){
      removeLeaf(q);
      leaves[p].marked++;
      rgt=kDispersePoints(root,n-1,K);
      leaves[p].marked--;
      insertLeaf(q);
    }
    
    return max(lft,rgt); 
  }
}

int main(int argc, char *argv[]) {
  int i,l=0;
  Node *root = NULL;
  //read input head
  scanf("%d %d %d",&N,&D,&K);  

  for(i=(N/LEAF_SIZE)+((N%LEAF_SIZE)?1:0);i!=1;i=(i/2)+(i%2)){l+=1;n_nodes+=i;}
  n_nodes++;

  leaves = (Leaf *)malloc( sizeof(Leaf)*N);
  nodes  = (Node *)malloc( sizeof(Node)*n_nodes);  

  
  for(i=0;i<N;i++) scanf("%lf%lf",&leaves[i].x,&leaves[i].y);

  qsort(leaves, N, sizeof(Leaf), comp);
  
  root=genTree(NULL,l);
    
  printf("%.5f\n",kDispersePoints(root,N,K));
  /*
  for(i=0;i<N;i++){
    if(leaves[i].rank==run){
    //printf("%d:(%lf,%lf)\n",i,leaves[i].x,leaves[i].y);  
    }
  }
  */
  return 0;
}
