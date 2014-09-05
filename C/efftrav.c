#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#define MAX 10000 
#define max(a,b) ((a)>(b))?(a):(b)
#define min(a,b) ((a)<(b))?(a):(b)

typedef struct Leaf {
  //coords
  double x;
  double y;

  //dispersion
  int marked;
  int rank;

  int off;
  struct Node *node;

} Leaf;

typedef struct Node{
  struct Node *up;
  struct Node *lft;
  struct Node *rgt;
 
  int c[3];
  int min;
  int max;
  int level;

  int a;
  int b;
  double dist;
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
/* ===================================== */

int N,D,K;

Node * nodes;
Leaf *leaves;

int count;
int n_nodes;

int run;
double max_disp;

double distance(int a, int b){
  if (a==b) return MAX;
  if (a==-1    || b==-1)    return MAX;
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
  printf("[%d-%d]: %f (%d,%d) \n",root->min,root->max,root->dist,root->a,root->b);
  /*
  for(i=0;i<=level;i++) printf("  ");
  printf("%d\n",root->c[0]);
  for(i=0;i<=level;i++) printf("  ");
  printf("%d\n",root->c[1]);
  for(i=0;i<=level;i++) printf("  ");
  printf("%d\n",root->c[2]);
  */
  if(root->rgt)
    printTree(root->rgt,level+1);
  return 0;
}

Node *genTree(Node *par, int l){
  Node *new;

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
    new->min=new->c[0]=count++;
    new->c[1]=count++;
    new->c[2]=count++;
    new->max=min(new->c[2],N-1);
    
    leaves[new->c[0]].node=new;
    
    if(new->c[1]<N) leaves[new->c[1]].node=new;
    else new->c[1]=-1;
    if(new->c[2]<N) leaves[new->c[2]].node=new;
    else new->c[2]=-1;
  }    
  return new;    
}

void insertLeaf(int index){
  Node * it;
  for(it=leaves[index].node;it!=NULL;it=it->up){
    it->dist=MAX;
  }
  leaves[index].node->c[index%3]=index;
  leaves[index].off=0;
  return;
}

void removeLeaf(int index){
  Node * it;
  for(it=leaves[index].node;it!=NULL;it=it->up){
    if(it->a == index || it->b==index)
      it->dist=MAX;
  }
  leaves[index].node->c[index%3]=-1;
  leaves[index].off=1;
  return;
}

double bruteForceK(Node * root){
  double tmp;
  int i,j;
  if (root->dist!=MAX)
    return root->dist;

  root->dist=MAX;

  for(i=0;i<2;i++){
    for(j=i;j<3;j++){
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
      if(root->up!=NULL) printf("HERE\n");
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

  for(i=(N/3)+((N%3)?1:0);i!=1;i=(i/2)+(i%2)){l+=1;n_nodes+=i;}
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
