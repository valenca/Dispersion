#include<stdio.h>
#include<stdlib.h>

typedef struct node * node_ptr;
typedef struct node{
  int x;
  int y;
  node_ptr lft;
  node_ptr rgt;
}NODE;

int main(){
  node_ptr N;
  N=(node_ptr)malloc(sizeof(NODE));
  N->x=1;
  printf("%d",N->x);
  return 0;
}
