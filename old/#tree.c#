#include<stdio.h>
#include<stdlib.h>
#define max(a,b) ((a)>(b))?(a):(b)

typedef struct node * node_ptr;
typedef struct node{
  int x;
  int y;
  node_ptr lft;
  node_ptr rgt;
  node_ptr prt;
}NODE;

node_ptr createNode();
void insertNode(node_ptr new, node_ptr tree);
void printTree(node_ptr tree,int l);
void deleteTree(node_ptr tree);
int compare(node_ptr a, node_ptr b);

/*
int main(){
  int i,j;
  node_ptr v[30],head;
	scanf("%d %d",&N,&D);
	
	for(i=0;i<N;i++)
		for(j=0;j<D;j++)
			scanf("%lf",&points[i][j]);


  head=createNode();
  head->x=15;
  for(i=0;i<30;i++){
    v[i]=createNode();
    v[i]->x=30-i;
    insertNode(v[i],head);
  }
  printTree(head,0);
	
	
	
  return 0;
}
*/

void printTree(node_ptr tree,int l){
  int i;
  if(tree){
    printTree(tree->lft,l+1);
    for(i=0;i<l;i++) printf("  ");
    printf("%d\n",tree->x);
    printTree(tree->rgt,l+1);
  }
  return;
}

node_ptr createNode(){
  return (node_ptr)malloc(sizeof(struct node));
}

void insertNode(node_ptr new, node_ptr tree){
  if(compare(new,tree)>=0){
    if(tree->rgt){
      insertNode(new,tree->rgt);
    }
    else{
      tree->rgt=new;
      new->prt=tree;
    }
    return;
  }
  else{
    if(tree->lft){
      insertNode(new,tree->lft);
    }
    else{
      tree->lft=new;
      new->prt=tree;
    }
    return;
  }

}

int compare(node_ptr a, node_ptr b){
  return (a->x - b->x);
}


void deleteTree(node_ptr tree){
  if(tree->lft) deleteTree(tree->lft);
  if(tree->rgt) deleteTree(tree->rgt);
  free(tree);
  return ;
}
