#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "tbtlib.h"

#define cmp_func ((*compare)(const void *, const void *))

/* ====== Threaded-Tree Types ====== */
/* 0 - Binary Search (BS) */
/* 1 - Splay */
/* 2 - Progressive Splay */
/* 3 - Random */
/* 4 - Treap */
/* 5 - Arne Andersson (AA) */
/* 6 - Red-Black (RB) */
/* 7 - Adelson-Velskii and Landis (AVL) */
/* ================================= */

/* ====== Static Functions declaration ====== */
static int tbtl_root_not(TBTLNode *root, char *function);
static int tbtl_root_null(TBTLNode *root, char *function);
static int tbtl_info_null(struct TBTLInfo *info, char *function);
static int tbtl_compare_null(int cmp_func, char *function);
static int tbtl_node_null(TBTLNode *node, char *function);
static void tbtl_delete_recursive(TBTLNode *current);
static void tbtl_validate_type(char type, char *function);
static void tbtl_check_type(char type, TBTLNode **root, char *function);
static void tbtl_convert_tree(char type, TBTLNode **root);
static TBTLNode *tbtl_new_node(char type, TBTLNode* up, TBTLNode *minor, TBTLNode *major, struct TBTLInfo *info);
static void tbtl_balance(char type, TBTLNode **root, TBTLNode *current, int depth);
static void tbtl_balance_splay(char type, TBTLNode **root, TBTLNode *current);
static void tbtl_balance_random(TBTLNode **root, TBTLNode *current, int depth);
static void tbtl_balance_treap(TBTLNode **root, TBTLNode *current);
static void tbtl_balance_aa(TBTLNode **root, TBTLNode *current);
static void tbtl_balance_rb(TBTLNode **root, TBTLNode *current);
static void tbtl_balance_avl(TBTLNode **root, TBTLNode *current);
static void tbtl_remove_bs(TBTLNode **root, TBTLNode *deleting_node);
static void tbtl_remove_splay(char type, TBTLNode **root, TBTLNode *deleting_node);
static void tbtl_remove_random(TBTLNode **root, TBTLNode *deleting_node);
static void tbtl_remove_treap(TBTLNode **root, TBTLNode *deleting_node);
static void tbtl_remove_aa(TBTLNode **root, TBTLNode *deleting_node);
static void tbtl_remove_rb(TBTLNode **root, TBTLNode *deleting_node);
static void tbtl_remove_avl(TBTLNode **root, TBTLNode *deleting_node);
static TBTLNode *tbtl_rotate_left(TBTLNode *current);
static TBTLNode *tbtl_rotate_right(TBTLNode *current);
static int tbtl_son_side(TBTLNode *up, TBTLNode *current);
static void tbtl_refresh_weight(TBTLNode *current);
static void tbtl_refresh_value(TBTLNode *current);
static void tbtl_refresh_balance(TBTLNode *current);
static void tbtl_cut_node(TBTLNode *cuting_node, TBTLNode *son);
static void tbtl_remove_node(TBTLNode **root, TBTLNode *deleting_node);
static void tbtl_replace_node(TBTLNode **root, TBTLNode *deleting_node, TBTLNode *replacing_node);
/* ========================================== */

/* ====== Static Variables declaration ====== */
static int r_rand = -1;
/* ========================================== */

/* ------ Validation functions ------ */
/* ====== Print warning if node is not the root of the tree ====== */
static int tbtl_root_not(TBTLNode *root, char *function) {
  if(root->up) {
    printf("TBTL Warning! - %s: Given root is not the tree's root! (root->up is not NULL)\n", function);
    return 1;
  }
  return 0;
}
/* =============================================================== */

/* ====== Print warning if root is NULL ====== */
static int tbtl_root_null(TBTLNode *root, char *function) {
  if(!root) {
    printf("TBTL Warning! - %s: Given tree root is NULL!\n", function);
    return 1;
  }
  return 0;
}
/* =========================================== */

/* ====== Print warning if info is NULL ====== */
static int tbtl_info_null(struct TBTLInfo *info, char *function) {
  if(!info) {
    printf("TBTL Warning! - %s: Given info is NULL!\n", function);
    return 1;
  }
  return 0;
}
/* =========================================== */

/* ====== Print warning if compare function is NULL ====== */
static int tbtl_compare_null(int cmp_func, char *function) {
  if(!compare) {
    printf("TBTL Warning! - %s: Given compare function is NULL!\n", function);
    return 1;
  }
  return 0;
}
/* ======================================================= */

/* ====== Print warning if node is NULL ====== */
static int tbtl_node_null(TBTLNode *node, char *function) {
  if(!node) {
    printf("TBTL Warning! - %s: Given node is NULL!\n", function);
    return 1;
  }
  return 0;
}
/* =========================================== */
/* ------ Validation functions ------ */

/* ------ Tree deletion ------ */
/* ====== Manages the deletion of the tree ====== */
void tbtl_delete(TBTLNode **root) {
  if(tbtl_root_null((*root), "(tbtl_delete)")) return;
  if(tbtl_root_not((*root), "(tbtl_delete)")) return;
  tbtl_delete_recursive((*root));
  (*root) = NULL;
}
/* ============================================ */

/* ====== Delete all nodes in the tree recursively ====== */
static void tbtl_delete_recursive(TBTLNode *current) {
  if(current->left) tbtl_delete_recursive(current->left);
  if(current->right) tbtl_delete_recursive(current->right);
  free(current);
}
/* ==================================================== */
/* ------ Tree deletion ------ */

/* ------ Type management ------ */
/* ====== Check if new type introduced by the user is valid ====== */
static void tbtl_validate_type(char type, char *function) {
  if(type < '0' || type > '7') {
    printf("  TBTL ERROR! - %s: Given tree type is not valid!\n", function);
    exit(-1);
  }
}
/* =============================================================== */

/* ====== Check if the tree mantains the type or if it's convertible ====== */
static void tbtl_check_type(char type, TBTLNode **root, char *function) {
  if((*root)->type == type) return;
  if(type == '0' || type == '1' || type == '2' || ((*root)->type == '3' && type == '4'))
    tbtl_convert_tree(type, root);
  else {
    printf("  TBTL ERROR! - %s: Can not convert Tree from type %c to %c!\n", function, (*root)->type, type);
    exit(-1);
  }
}
/* ======================================================================== */

/* ====== Convert all the tree's nodes ====== */
static void tbtl_convert_tree(char type, TBTLNode **root) {
  TBTLNode *current = NULL;
  printf("    TBTL note - (tbtl_convert): Tree was converted from type %c to %c\n", (*root)->type, type);
  for(current = tbtl_get_leftmost_node((*root)); current; current = current->major)
    current->type = type;
}
/* ========================================== */

/* ====== Convert the tree's type ====== */
void tbtl_convert(char type, TBTLNode **root) {
  if(tbtl_root_null((*root), "(tbtl_convert)")) return;
  if(tbtl_root_not((*root), "(tbtl_convert)")) return;
  tbtl_validate_type(type, "(tbtl_convert)");
  if((*root)->type == type) {
    printf("TBTL Warning! - (tbtl_convert): Given type is the same as the tree!\n");
    return;
  }
  tbtl_check_type(type, root, "(tbtl_convert)");
}
/* ===================================== */
/* ------ Type management ------ */

/* ------ Node finder ------ */
/* ====== Get the leftmost node in the subtree ====== */
TBTLNode *tbtl_get_leftmost_node(TBTLNode *stroot) {
  TBTLNode *current = NULL;
  if(tbtl_root_null(stroot, "(tbtl_get_leftmost_node)")) return NULL;
  for(current = stroot; current->left; current = current->left) {;}
  return current;
}
/* ================================================== */

/* ====== Get the rightmost node in the subtree ====== */
TBTLNode *tbtl_get_rightmost_node(TBTLNode *stroot) {
  TBTLNode *current = NULL;
  if(tbtl_root_null(stroot, "(tbtl_get_rightmost_node)")) return NULL;
  for(current = stroot; current->right; current = current->right) {;}
  return current;
}
/* =================================================== */

/* ====== Find the node with a certain info in the tree ====== */
TBTLNode *tbtl_find(char type, TBTLNode **root, struct TBTLInfo *info, int cmp_func) {

  TBTLNode *current = NULL;

  /* Validation */
  if(tbtl_root_null((*root), "(tbtl_find)")) return NULL;
  if(tbtl_root_not((*root), "(tbtl_find)")) return NULL;
  if(tbtl_info_null(info, "(tbtl_find)")) return NULL;
  if(tbtl_compare_null(compare, "(tbtl_find)")) return NULL;
  tbtl_validate_type(type, "(tbtl_find)");
  tbtl_check_type(type, root, "(tbtl_find)");

  /* Find node */
  for(current = (*root); current; ) {
    if(compare(info, current->info) == -1) current = current->left;
    else if(compare(info, current->info) == 1) current = current->right;
    else {
      if(type == '1' || type == '2') tbtl_balance_splay(type, root, current);
      return current;
    }
  }
  return NULL;
}
/* =========================================================== */
/* ------ Node finder ------ */

/* ------ Node creator ------ */
/* ====== Create and initialize a new node ====== */
static TBTLNode *tbtl_new_node(char type, TBTLNode* up, TBTLNode *minor, TBTLNode *major, struct TBTLInfo *info) {

  TBTLNode *node = (TBTLNode *) calloc(1, sizeof(TBTLNode));

  switch(type) {
  case '3': case '5':	node->weight = 1;									break;
    /*case '4':			node->weight = (r_rand = rand_r(&r_rand));			break;*/
  case '6':			if(up) node->colour = 'r'; else node->colour = 'b';	break;
  case '7':			node->balance = node->n_left = node->n_right = 0;	break;
  }
  node->left = node->right = NULL;
  node->up = up;
  node->minor = minor;
  node->major = major;
  node->type = type;
  node->info = info;

  return node;
}
/* ============================================== */

/* ====== Insert new node in the tree ====== */
TBTLNode *tbtl_insert(char type, TBTLNode **root, struct TBTLInfo *info, int cmp_func) {

  TBTLNode *current = NULL;
  int depth = 0;
	
  if(r_rand == -1) r_rand = time(NULL);

  /* Validation */
  tbtl_validate_type(type, "(tbtl_insert)");
  if(tbtl_info_null(info, "(tbtl_insert)")) return NULL;

  /* If root is NULL */
  if(!(*root)) return ((*root) = tbtl_new_node(type, NULL, NULL, NULL, info));

  /* Validation */
  if(tbtl_root_not((*root), "(tbtl_insert)")) return NULL;
  if(tbtl_compare_null(compare, "(tbtl_insert)")) return NULL;
  tbtl_check_type(type, root, "(tbtl_insert)");

  current = (*root);
  while(1) {
    /* Update number of rotations on Random Tree */
    if(type == '3') {
      if(depth != 0) depth++;
      else {
	r_rand = 1;
	if(((double)r_rand / (double)RAND_MAX) < ((double)1/((double)current->weight+1)))
	  depth = 1;
      }
    }
    /* If new info is lower than the current one */
    if(compare(info, current->info) == -1) {
      /* Insert new node */
      if(!current->left) {
	current->left = tbtl_new_node(type, current, current->minor, current, info);
	if(current->minor) current->minor->major = current->left;
	current->minor = current->left;
	tbtl_balance(type, root, current->left, depth);
	return current->minor;
      }
      /* Keep going */
      else current = current->left;
    }
    /* If new info is higher than the current one */
    else if(compare(info, current->info) == 1) {
      /* Insert new node */
      if(!current->right) {
	current->right = tbtl_new_node(type, current, current, current->major, info);
	if(current->major) current->major->minor = current->right;
	current->major = current->right;
	tbtl_balance(type, root, current->right, depth);
	return current->major;
      }
      /* Keep going */
      else current = current->right;
    }
    /* If new info already exists in the tree */
    else return NULL;
  }
}
/* ========================================= */

/* ====== Call respective balancing function ====== */
static void tbtl_balance(char type, TBTLNode **root, TBTLNode *current, int depth) {
  switch(type) {
  case '1': case '2':	tbtl_balance_splay(type, root, current);		break;
  case '3':			tbtl_balance_random(root, current, depth);	break;
  case '4':			tbtl_balance_treap(root, current);			break;
  case '5':			tbtl_balance_aa(root, current);				break;
  case '6':			tbtl_balance_rb(root, current);				break;
  case '7':			tbtl_balance_avl(root, current);				break;
  }
}
/* ================================================ */
/* ------ Node creator ------ */

/* ------ Node balancer ------ */
/* ====== Balance a Splay Tree ====== */
static void tbtl_balance_splay(char type, TBTLNode **root, TBTLNode *current) {

  int side = 0;

  /* If it is the root */
  if(current == (*root)) return;

  /* If not */
  while(1) {
    /* Get side to its father */
    side = tbtl_son_side(current->up, current);

    /* Get father's side to its own father */
    switch(tbtl_son_side(current->up->up, current->up)) {
    case 0:	 if(side == -1)	(*root) = tbtl_rotate_left(current->up);
      else			(*root) = tbtl_rotate_right(current->up);
      return;
    case -1: if(side == -1)	current->up->up->left = tbtl_rotate_left(current->up);
      else			current->up->up->left = tbtl_rotate_right(current->up);
      break;
    case 1:	 if(side == -1)	current->up->up->right = tbtl_rotate_left(current->up);
      else			current->up->up->right = tbtl_rotate_right(current->up);
      break;
    }

    /* If it is Progressive Splay */
    if(type == '2') return;
  }
}
/* ================================== */

/* ====== Balance a Random Tree ====== */
static void tbtl_balance_random(TBTLNode **root, TBTLNode *current, int depth) {

  int i = 0, side = 0;

  for(i = 0; i < depth; i++) {
    /* Update weight */
    tbtl_refresh_weight(current);

    /* Get side to its father */
    side = tbtl_son_side(current->up, current);

    /* Get father's side to its own father */
    switch(tbtl_son_side(current->up->up, current->up)) {
    case 0:	 if(side == -1)	(*root) = tbtl_rotate_left(current->up);
      else			(*root) = tbtl_rotate_right(current->up);
      break;
    case -1: if(side == -1)	current->up->up->left = tbtl_rotate_left(current->up);
      else			current->up->up->left = tbtl_rotate_right(current->up);
      break;
    case 1:	 if(side == -1)	current->up->up->right = tbtl_rotate_left(current->up);
      else			current->up->up->right = tbtl_rotate_right(current->up);
      break;
    }

    if(side == -1)	tbtl_refresh_weight(current->right);
    else			tbtl_refresh_weight(current->left);
  }

  /* Update weights of its ascendancy */
  for(; current; current = current->up) tbtl_refresh_weight(current);
}
/* =================================== */

/* ====== Balance a Treap Tree ====== */
static void tbtl_balance_treap(TBTLNode **root, TBTLNode *current) {

  int side = 0;

  /* If its weight his higher than its father's */
  while(current->up && current->weight > current->up->weight) {
    /* Get side to its father */
    side = tbtl_son_side(current->up, current);

    /* Get father's side to its own father */
    switch(tbtl_son_side(current->up->up, current->up)) {
    case 0:	 if(side == -1)	(*root) = tbtl_rotate_left(current->up);
      else			(*root) = tbtl_rotate_right(current->up);
      return;
    case -1: if(side == -1)	current->up->up->left = tbtl_rotate_left(current->up);
      else			current->up->up->left = tbtl_rotate_right(current->up);
      break;
    case 1:	 if(side == -1)	current->up->up->right = tbtl_rotate_left(current->up);
      else			current->up->up->right = tbtl_rotate_right(current->up);
      break;
    }
  }
}
/* ================================== */

/* ====== Balance a AA Tree ====== */
static void tbtl_balance_aa(TBTLNode **root, TBTLNode *current) {

  int side = 0;

  for(; current; current = current->up) {

    /* Get side to its father */
    side = tbtl_son_side(current->up, current);

    if(current->left && current->left->weight == current->weight) {
      /* If has two sons with equal weight -> increase weight */
      if(current->right && current->right->weight == current->weight) current->weight++;
      /* If has only the left son -> rotate with its left son */
      else {
	switch(side) {
	case 0:	 (*root) = tbtl_rotate_left(current);			return;
	case -1: current->up->left = tbtl_rotate_left(current);	break;
	case 1:	 current->up->right = tbtl_rotate_left(current);	break;
	}
	current = current->up;
      }
    }
    /* If three straigt right descendancy with same weight -> rotate with right son */
    else if(current->right && current->right->weight == current->weight &&
	    current->right->right && current->right->right->weight == current->weight) {
      current->right->weight++;
      switch(side) {
      case 0:	 (*root) = tbtl_rotate_right(current);			 return;
      case -1: current->up->left = tbtl_rotate_right(current);	 break;
      case 1:	 current->up->right = tbtl_rotate_right(current); break;
      }
      current = current->up;
    }
  }
}
/* =============================== */

/* ====== Balance a RB Tree ====== */
static void tbtl_balance_rb(TBTLNode **root, TBTLNode *current) {

  int rotate = 0, side = 0, straight = 0;

  for(rotate = 0; current; current = current->up, rotate = 0) {

    /* Get side to its father */
    side = tbtl_son_side(current->up, current);

    /* If there are two straight red nodes on the left side */
    if(straight == -1) {
      straight = 0;

      /* If current node is black and both sons are red */
      if(current->colour == 'b' && current->right && current->right->colour == 'r') {
	current->colour = 'r';
	current->right->colour = current->left->colour = 'b';
      } else {
	rotate = 1;
	/* If zig-zag layout */
	if(current->left->right && current->left->right->colour == 'r')
	  current->left = tbtl_rotate_right(current->left);
	switch(side) {
	case 0:	 (*root) = tbtl_rotate_left(current);			break;
	case -1: current->up->left = tbtl_rotate_left(current);	break;
	case 1:	 current->up->right = tbtl_rotate_left(current);	break;
	}
      }
    }
    /* If there are two straight red nodes on the right side */
    else if(straight == 1) {
      straight = 0;

      /* If current node is black and both sons are red */
      if(current->colour == 'b' && current->left && current->left->colour == 'r') {
	current->colour = 'r';
	current->right->colour = current->left->colour = 'b';
      } else {
	rotate = 1;
	/* If zig-zag layout */
	if(current->right->left != NULL && current->right->left->colour == 'r')
	  current->right = tbtl_rotate_left(current->right);
	switch(side) {
	case 0:	 (*root) = tbtl_rotate_right(current);			 break;
	case -1: current->up->left = tbtl_rotate_right(current);	 break;
	case 1:	 current->up->right = tbtl_rotate_right(current); break;
	}
      }
    }
    /* If there are currently two straight red nodes */
    else if(current->colour == 'r' &&
	    ((current->left && current->left->colour == 'r') || 
	     (current->right && current->right->colour == 'r'))) {
      straight = side;
    }

    /* If there was a rotation -> refresh colours and current node */
    if(rotate) {
      current->up->colour = 'b';
      current->colour = 'r';
      switch(side) {
      case 0:			 (*root)->colour = 'b';	return;
      case -1: case 1: current = current->up;	break;
      }
    }
  }

  /* Root must always be a black node */
  (*root)->colour = 'b';
}
/* =============================== */

/* ====== Balance a AVL Tree ====== */
static void tbtl_balance_avl(TBTLNode **root, TBTLNode *current) {

  int rotate = 0, side = 0;

  for(rotate = 0; current != NULL; current = current->up, rotate = 0) {

    /* Refresh current node's balance */
    tbtl_refresh_balance(current);

    /* Get side to its father */
    side = tbtl_son_side(current->up, current);

    /* If subtree is unbalanced to the left side */
    if(current->balance < -1) {
      rotate = 1;
      /* If zig-zag layout */
      if(current->left->balance > 0) current->left = tbtl_rotate_right(current->left);
      switch(side) {
      case 0:	 (*root) = tbtl_rotate_left(current);			break;
      case -1: current->up->left = tbtl_rotate_left(current);	break;
      case 1:	 current->up->right = tbtl_rotate_left(current);	break;
      }
    }
    /* If it is to the right side */
    else if(current->balance > 1) {
      rotate = 1;
      /* If zig-zag layout */
      if(current->right->balance < 0) current->right = tbtl_rotate_left(current->right);
      switch(side) {
      case 0:	 (*root) = tbtl_rotate_right(current);			 break;
      case -1: current->up->left = tbtl_rotate_right(current);	 break;
      case 1:	 current->up->right = tbtl_rotate_right(current); break;
      }
    }

    /* If there was a rotation -> refresh balances and current node */
    if(rotate) {
      tbtl_refresh_balance(current->up->left);
      tbtl_refresh_balance(current->up->right);
      tbtl_refresh_balance(current->up);
      switch(side) {
      case 0:				return;
      case -1: case 1:	current = current->up;	break;
      }
    }
  }
}
/* ================================ */
/* ------ Node balancer ------ */

/* ------ Node remover ------ */
/* ====== Manages the removal of a node in the tree ====== */
void tbtl_remove(char type, TBTLNode **root, TBTLNode *deleting_node) {

  /* Validation */
  if(tbtl_root_null((*root), "(tbtl_remove)")) return;
  if(tbtl_root_not((*root), "(tbtl_remove)")) return;
  if(tbtl_node_null(deleting_node, "(tbtl_remove)")) return;
  tbtl_validate_type(type, "(tbtl_remove)");
  tbtl_check_type(type, root, "(tbtl_remove)");

  switch(type) {
  case '0':			tbtl_remove_bs(root, deleting_node);			 break;
  case '1': case '2':	tbtl_remove_splay(type, root, deleting_node); break;
  case '3': 			tbtl_remove_random(root, deleting_node);		 break;
  case '4':			tbtl_remove_treap(root, deleting_node);		 break;
  case '5':			tbtl_remove_aa(root, deleting_node);			 break;
  case '6':			tbtl_remove_rb(root, deleting_node);			 break;
  case '7':			tbtl_remove_avl(root, deleting_node);		 break;
  }
}
/* ======================================================= */

/* ====== Remove a node in a BS Tree ====== */
static void tbtl_remove_bs(TBTLNode **root, TBTLNode *deleting_node) {

  TBTLNode *current = NULL;

  /* If it has no sons */
  if(!deleting_node->right && !deleting_node->left)
    tbtl_remove_node(root, deleting_node);
  /* If it has both sons */
  else if(deleting_node->right && deleting_node->left) {
    current = deleting_node->major;
    tbtl_cut_node(current, current->right);
    tbtl_replace_node(root, deleting_node, current);
  }
  /* If it has only one son */
  else {
    if(deleting_node->right) current = deleting_node->right;
    else 					 current = deleting_node->left;
    switch(tbtl_son_side(deleting_node->up, deleting_node)) {
    case 0:		(*root) = current;					break;
    case -1:	deleting_node->up->left = current;	break;
    case 1:		deleting_node->up->right = current;	break;
    }
    current->up = deleting_node->up;
    if(deleting_node->major) deleting_node->major->minor = deleting_node->minor;
    if(deleting_node->minor) deleting_node->minor->major = deleting_node->major;
    free(deleting_node);
  }
}
/* ======================================== */

/* ====== Remove a node in a Splay Tree ====== */
static void tbtl_remove_splay(char type, TBTLNode **root, TBTLNode *deleting_node) {

  /* Splay deleting node */
  tbtl_balance_splay(type, root, deleting_node);

  /* Remove node as in a BS Tree */
  tbtl_remove_bs(root, deleting_node);
}
/* =========================================== */

/* ====== Remove a node in a Random Tree ====== */
static void tbtl_remove_random(TBTLNode **root, TBTLNode *deleting_node) {

  TBTLNode *temp = NULL;
  int side = 0;

  while(1) {

    /* Get side to its father */
    side = tbtl_son_side(deleting_node->up, deleting_node);

    /* If the right side has more descendants than the left -> rotate with right son */
    if(deleting_node->right && ((deleting_node->left &&
				 deleting_node->right->weight >= deleting_node->left->weight) || !deleting_node->left)) {
      switch(side) {
      case 0:	 (*root) = tbtl_rotate_right(deleting_node);					 break;
      case -1: deleting_node->up->left = tbtl_rotate_right(deleting_node);	 break;
      case 1:	 deleting_node->up->right = tbtl_rotate_right(deleting_node); break;
      }
      tbtl_refresh_weight(deleting_node);
      tbtl_refresh_weight(deleting_node->up);
    }
    /* If the left side has more descendants than the right -> rotate with left son */
    /*else if(deleting_node->left && ((deleting_node->right &&
      deleting_node->left->weight > deleting_node->right->weight) || !deleting_node->right)) {*/
    else if(deleting_node->left) {
      switch(side) {
      case 0:	 (*root) = tbtl_rotate_left(deleting_node);					break;
      case -1: deleting_node->up->left = tbtl_rotate_left(deleting_node);	break;
      case 1:	 deleting_node->up->right = tbtl_rotate_left(deleting_node);	break;
      }
      tbtl_refresh_weight(deleting_node);
      tbtl_refresh_weight(deleting_node->up);
    }
    /* Remove node (who is now a leaf) */
    else {
      temp = deleting_node->up;
      tbtl_remove_node(root, deleting_node);
      /* Balance the Random Tree */
      tbtl_balance_random(root, temp, 0);
      return;
    }
  }
}
/* ============================================ */

/* ====== Remove a node in a Treap Tree ====== */
static void tbtl_remove_treap(TBTLNode **root, TBTLNode *deleting_node) {

  int side = 0;

  while(1) {

    /* Get side to its father */
    side = tbtl_son_side(deleting_node->up, deleting_node);

    /* If the right son has higher weight than the left -> rotate with right son */
    if(deleting_node->right && ((deleting_node->left &&
				 deleting_node->right->weight >= deleting_node->left->weight) || !deleting_node->left)) {
      switch(side) {
      case 0:	 (*root) = tbtl_rotate_right(deleting_node);					 break;
      case -1: deleting_node->up->left = tbtl_rotate_right(deleting_node);	 break;
      case 1:	 deleting_node->up->right = tbtl_rotate_right(deleting_node); break;
      }
    }
    /* If the right son has higher weight than the left -> rotate with left son */
    /*else if(deleting_node->left && ((deleting_node->right &&
      deleting_node->left->weight > deleting_node->right->weight) || !deleting_node->right)) {*/
    else if(deleting_node->left) {
      switch(side) {
      case 0:	 (*root) = tbtl_rotate_left(deleting_node);					break;
      case -1: deleting_node->up->left = tbtl_rotate_left(deleting_node);	break;
      case 1:	 deleting_node->up->right = tbtl_rotate_left(deleting_node);	break;
      }
    }
    /* Remove node (who is now a leaf) */
    else {
      tbtl_remove_node(root, deleting_node);
      return;
    }
  }
}
/* =========================================== */

/* ====== Remove a node in a AA Tree ====== */
static void tbtl_remove_aa(TBTLNode **root, TBTLNode *deleting_node) {

  TBTLNode *current = NULL, *temp = NULL;
  int side = 0;

  /* If it has no sons */
  if(!deleting_node->right && !deleting_node->left) {
    current = deleting_node->up;
    tbtl_remove_node(root, deleting_node);
  }
  /* If it has at least one son */
  else {
    current = deleting_node->major;
    if(deleting_node == current->up) temp = NULL;
    else							 temp = current->up;
    tbtl_cut_node(current, current->right);
    tbtl_replace_node(root, deleting_node, current);
    if(temp) current = temp;
  }

  /* If tree is empty */
  if(!(*root)) return;

  for(; current; current = current->up) {

    /* Get side to its father */
    side = tbtl_son_side(current->up, current);

    /* Refresh current node's value */
    tbtl_refresh_value(current);

    if(current->left && current->left->weight == current->weight) {
      /* If has two sons with equal weight -> increase weight */
      if(current->right && current->right->weight == current->weight) current->weight++;
      /* If has only the left son -> rotate with its left son */
      else {
	switch(side) {
	case 0:	 (*root) = tbtl_rotate_left(current);			break;
	case -1: current->up->left = tbtl_rotate_left(current);	break;
	case 1:	 current->up->right = tbtl_rotate_left(current);	break;
	}
	if(current->left) current = current->left;
      }
    }
    /* If its right son has a higher value */
    else if(current->right && current->right->weight > current->weight) {
      current->right->weight--;
      current->right = tbtl_rotate_left(current->right);
      current->right->weight++;
      switch(side) {
      case 0:	 (*root) = tbtl_rotate_right(current);			 break;
      case -1: current->up->left = tbtl_rotate_right(current);	 break;
      case 1:	 current->up->right = tbtl_rotate_right(current); break;
      }
      current = current->up->right->right;
    }
    /* If three straigt right descendancy with same weight -> rotate with right son */
    else if(current->right && current->right->weight == current->weight &&
	    current->right->right && current->right->right->weight == current->weight) {
      current->right->weight++;
      switch(side) {
      case 0:	 (*root) = tbtl_rotate_right(current);			 break;
      case -1: current->up->left = tbtl_rotate_right(current);	 break;
      case 1:	 current->up->right = tbtl_rotate_right(current); break;
      }
      current = current->up;
    }
  }
}
/* ======================================== */

/* ====== Remove a node in a RB Tree ====== */
static void tbtl_remove_rb(TBTLNode **root, TBTLNode *deleting_node) {

  TBTLNode *current = NULL, *temp = NULL;
  int side = 0;

  /* If it has no sons */
  if(!deleting_node->right && !deleting_node->left) {
    /* If deleting node is a red one */
    if(deleting_node->colour == 'r') {
      tbtl_remove_node(root, deleting_node);
      return;
    }
    /* If it is a black one */
    else {
      side = tbtl_son_side(deleting_node->up, deleting_node);
      current = deleting_node->up;
      tbtl_remove_node(root, deleting_node);
    }
  }
  /* If it has both sons */
  else if(deleting_node->right && deleting_node->left) {
    current = deleting_node->major;
    /* If major node is a red one */
    if(current->colour == 'r') {
      tbtl_cut_node(current, current->right);
      current->colour = deleting_node->colour;
      tbtl_replace_node(root, deleting_node, current);
      return;
    }
    /* If it is a black one */
    else {
      /* If major node as a son */
      if(current->right) {
	current->right->colour = 'b';
	tbtl_cut_node(current, current->right);
	current->colour = deleting_node->colour;
	tbtl_replace_node(root, deleting_node, current);
	return;
      }
      /* If not */
      else {
	side = tbtl_son_side(current->up, current);
	if(deleting_node == current->up) temp = NULL;
	else							 temp = current->up;
	tbtl_cut_node(current, current->right);
	current->colour = deleting_node->colour;
	tbtl_replace_node(root, deleting_node, current);
	if(temp) current = temp;
      }
    }
  }
  /* If it has only one son */
  else {
    /* If son is on the right side */
    if(deleting_node->right)
      switch(tbtl_son_side(deleting_node->up, deleting_node)) {
      case 0:	 (*root) = tbtl_rotate_right(deleting_node);					 break;
      case -1: deleting_node->up->left = tbtl_rotate_right(deleting_node);	 break;
      case 1:	 deleting_node->up->right = tbtl_rotate_right(deleting_node); break;
      }
    /* If son is on the left side */
    else
      switch(tbtl_son_side(deleting_node->up, deleting_node)) {
      case 0:	 (*root) = tbtl_rotate_left(deleting_node);					break;
      case -1: deleting_node->up->left = tbtl_rotate_left(deleting_node);	break;
      case 1:	 deleting_node->up->right = tbtl_rotate_left(deleting_node);	break;
      }
    deleting_node->up->colour = 'b';
    tbtl_remove_node(root, deleting_node);
    return;
  }

  /* If tree is empty */
  if(!(*root)) return;

  while(current) {
    /* If tree climb was from the left */
    if(side == -1) {
      /* If brother is a red node */
      if(current->right->colour == 'r') {
	switch(tbtl_son_side(current->up, current)) {
	case 0:	 (*root) = tbtl_rotate_right(current);			 break;
	case -1: current->up->left = tbtl_rotate_right(current);	 break;
	case 1:	 current->up->right = tbtl_rotate_right(current); break;
	}
	current->up->colour = 'b';
	current->colour = 'r';
      }
      /* If farther nephew is a red node */
      else if(current->right->right && current->right->right->colour == 'r') {
	switch(tbtl_son_side(current->up, current)) {
	case 0:	 (*root) = tbtl_rotate_right(current);			 break;
	case -1: current->up->left = tbtl_rotate_right(current);	 break;
	case 1:	 current->up->right = tbtl_rotate_right(current); break;
	}
	current->up->colour = current->colour;
	current->colour = 'b';
	current->up->right->colour = 'b';
	return;
      }
      /* If closer nephew is a red node and farther one it is not */
      else if(current->right->left && current->right->left->colour == 'r') {
	current->right->left->colour = 'b';
	current->right->colour = 'r';
	current->right = tbtl_rotate_left(current->right);
      }
      /* If brother has no sons or two black node sons */
      else {
	current->right->colour = 'r';
	/* If father is a red node */
	if(current->colour == 'r') {
	  current->colour = 'b';
	  return;
	} else {
	  side = tbtl_son_side(current->up, current);
	  current = current->up;
	}
      }
    }
    /* If it was from the right */
    else if(side == 1) {
      /* If brother is a red node */
      if(current->left->colour == 'r') {
	switch(tbtl_son_side(current->up, current)) {
	case 0:	 (*root) = tbtl_rotate_left(current);			break;
	case -1: current->up->left = tbtl_rotate_left(current);	break;
	case 1:	 current->up->right = tbtl_rotate_left(current);	break;
	}
	current->up->colour = 'b';
	current->colour = 'r';
      }
      /* If farther nephew is a red node */
      else if(current->left->left && current->left->left->colour == 'r') {
	switch(tbtl_son_side(current->up, current)) {
	case 0:	 (*root) = tbtl_rotate_left(current);			break;
	case -1: current->up->left = tbtl_rotate_left(current);	break;
	case 1:	 current->up->right = tbtl_rotate_left(current);	break;
	}
	current->up->colour = current->colour;
	current->colour = 'b';
	current->up->left->colour = 'b';
	return;
      }
      /* If closer nephew is a red node and farther one it is not */
      else if(current->left->right && current->left->right->colour == 'r') {
	current->left->right->colour = 'b';
	current->left->colour = 'r';
	current->left = tbtl_rotate_right(current->left);
      }
      /* If brother has no sons or two black node sons */
      else {
	current->left->colour = 'r';
	if(current->colour == 'r') {
	  current->colour = 'b';
	  return;
	} else {
	  side = tbtl_son_side(current->up, current);
	  current = current->up;
	}
      }
    }
  }
}
/* ======================================== */

/* ====== Remove a node in a AVL Tree ====== */
static void tbtl_remove_avl(TBTLNode **root, TBTLNode *deleting_node) {

  TBTLNode *current = NULL, *temp = NULL;

  /* If it has no sons */
  if(!deleting_node->left && !deleting_node->right) {
    temp = deleting_node->up;
    tbtl_remove_node(root, deleting_node);
    if(temp) tbtl_balance_avl(root, temp);
  }
  /* If it has at least one son */
  else {
    /* Choose the side with more descendancy */
    /* Right side */
    if(deleting_node->n_right > deleting_node->n_left) {
      current = deleting_node->major;
      tbtl_cut_node(current, current->right);
    }
    /* Left side */
    else {
      current = deleting_node->minor;
      tbtl_cut_node(current, current->left);
    }

    if(deleting_node == current->up) temp = NULL;
    else							 temp = current->up;
    tbtl_replace_node(root, deleting_node, current);
    if(temp) tbtl_balance_avl(root, temp);
    else 	 tbtl_balance_avl(root, current);
  }
}
/* ========================================= */
/* ------ Node remover ------ */

/* ------ Auxiliary functions ------ */
/* ====== Rotate a node with its left son ====== */
static TBTLNode *tbtl_rotate_left(TBTLNode *current) {
  TBTLNode* left = current->left;
  current->left = left->right;
  left->right = current;
  left->up = current->up;
  current->up = left;
  if(current->left) current->left->up = current;
  return left;
}
/* ============================================= */

/* ====== Rotate a node with its right son ====== */
static TBTLNode *tbtl_rotate_right(TBTLNode *current) {
  TBTLNode* right = current->right;
  current->right = right->left;
  right->left = current;
  right->up = current->up;
  current->up = right;
  if(current->right) current->right->up = current;
  return right;
}
/* ============================================== */

/* ====== Check the side of a son relatively to its father ====== */
static int tbtl_son_side(TBTLNode *up, TBTLNode *current) {
  if(!up) return 0;
  if(up->left && up->left == current) return -1;
  else return 1;
}
/* ============================================================== */

/* ====== Update weight of a node (number of nodes in its descendancy) ====== */
static void tbtl_refresh_weight(TBTLNode *current) {
  current->weight = 1;
  if(current->left)	current->weight += current->left->weight;
  if(current->right)	current->weight += current->right->weight;
}
/* ========================================================================== */

/* ====== Update value of a node (number of levels in its descendancy) ====== */
static void tbtl_refresh_value(TBTLNode *current) {
  if(!current->left || !current->right) {
    current->weight = 1;
    return;
  }
  if(current->left->weight < current->right->weight)
    current->weight = current->left->weight + 1;
  else current->weight = current->right->weight + 1;
}
/* ========================================================================== */

/* ====== Update balance of a node (difference in number of levels of each side) ====== */
static void tbtl_refresh_balance(TBTLNode *current) {
  if(current->left) current->n_left = (current->left->n_left > current->left->n_right) ?
		      current->left->n_left + 1 : current->left->n_right + 1;
  else current->n_left = 0;
  if(current->right) current->n_right = (current->right->n_left > current->right->n_right) ? 
		       current->right->n_left + 1 : current->right->n_right + 1;
  else current->n_right = 0;	
  current->balance = current->n_right - current->n_left;
}
/* ==================================================================================== */

/* ====== Cut a leaf or a leaf's father from the tree (not remove) ====== */
static void tbtl_cut_node(TBTLNode *cuting_node, TBTLNode *son) {
  switch(tbtl_son_side(cuting_node->up, cuting_node)) {
  case -1: cuting_node->up->left = son;	break;
  case 1:	 cuting_node->up->right = son;	break;
  }
  if(son) son->up = cuting_node->up;
}
/* ====================================================================== */

/* ====== Remove a leaf from the tree ====== */
static void tbtl_remove_node(TBTLNode **root, TBTLNode *deleting_node) {
  switch(tbtl_son_side(deleting_node->up, deleting_node)) {
  case 0:	 free(deleting_node); (*root) = NULL;	return;
  case -1: deleting_node->up->left = NULL;		break;
  case 1:	 deleting_node->up->right = NULL;		break;
  }
  if(deleting_node->minor) deleting_node->minor->major = deleting_node->major;
  if(deleting_node->major) deleting_node->major->minor = deleting_node->minor;
  free(deleting_node);
}
/* ========================================= */

/* ====== Replace a node by another one ====== */
static void tbtl_replace_node(TBTLNode **root, TBTLNode *deleting_node, TBTLNode *replacing_node) {
  replacing_node->left = deleting_node->left;
  replacing_node->right = deleting_node->right;
  replacing_node->up = deleting_node->up;
  if(deleting_node->left) deleting_node->left->up = replacing_node;
  if(deleting_node->right) deleting_node->right->up = replacing_node;
  switch(tbtl_son_side(deleting_node->up, deleting_node)) {
  case 0:	 (*root) = replacing_node;					break;
  case -1: deleting_node->up->left = replacing_node;	break;
  case 1:	 deleting_node->up->right = replacing_node;	break;
  }
  if(deleting_node->major) deleting_node->major->minor = deleting_node->minor;
  if(deleting_node->minor) deleting_node->minor->major = deleting_node->major;
  free(deleting_node);
}
/* =========================================== */
/* ------ Auxiliary functions ------ */
