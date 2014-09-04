#ifndef _TBTLIB_
#define _TBTLIB_

#define cmp_func ((*compare)(const void *, const void *))
#define MAX 10000


/* ====== TBTLNode structure ====== */
typedef struct TBTLNode {
	int weight, balance, n_left, n_right;
	char colour, type;
	struct TBTLNode *left, *right, *up, *minor, *major;
	struct TBTLInfo *info;
} TBTLNode;
/* ============================== */

typedef struct TBTLInfo {
  /*coords*/
  double x;
  double y;

  /*closest pair*/
  double dist;
  struct TBTLInfo *a;
  struct TBTLInfo *b;

  /*dispersion*/
  int marked;
  int rank;
  int id;

  struct TBTLNode *node;
} TBTLInfo;

/* ====== Functions declaration ====== */
/* ------ Validation functions ------ */
/*static int tbtl_root_not(TBTLNode *root, char *function);*/
/*static int tbtl_root_null(TBTLNode *root, char *function);*/
/*static int tbtl_info_null(TBTLInfo *info, char *function);*/
/*static int tbtl_compare_null(int cmp_func, char *function);*/
/*static int tbtl_node_null(TBTLNode *node, char *function);*/

/* ------ Tree deletion ------ */
void tbtl_delete(TBTLNode **root);
/*static void tbtl_delete_recursive(TBTLNode *current);*/

/* ------ Type management ------ */
/*static void tbtl_validate_type(char type, char *function);*/
/*static void tbtl_check_type(char type, TBTLNode **root, char *function);*/
/*static void tbtl_convert_tree(char type, TBTLNode **root);*/
void tbtl_convert(char type, TBTLNode **root);

/* ------ Node finder ------ */
TBTLNode *tbtl_get_leftmost_node(TBTLNode *stroot);
TBTLNode *tbtl_get_rightmost_node(TBTLNode *stroot);
TBTLNode *tbtl_find(char type, TBTLNode **root, struct TBTLInfo *info, int cmp_func);

/* ------ Node creator ------ */
/*static TBTLNode *tbtl_new_node(char type, TBTLNode* up, TBTLNode *minor, TBTLNode *major, struct TBTLInfo *info);*/
TBTLNode *tbtl_insert(char type, TBTLNode **root, struct TBTLInfo *info, int cmp_func);

/* ------ Node balancer ------ */
/*static void tbtl_balance(char type, TBTLNode **root, TBTLNode *current, int depth);*/
/*static void tbtl_balance_splay(char type, TBTLNode **root, TBTLNode *current);*/
/*static void tbtl_balance_random(TBTLNode **root, TBTLNode *current, int depth);*/
/*static void tbtl_balance_treap(TBTLNode **root, TBTLNode *current);*/
/*static void tbtl_balance_aa(TBTLNode **root, TBTLNode *current);*/
/*static void tbtl_balance_rb(TBTLNode **root, TBTLNode *current);*/
/*static void tbtl_balance_avl(TBTLNode **root, TBTLNode *current);*/

/* ------ Node remover ------ */
void tbtl_remove(char type, TBTLNode **root, TBTLNode *deleting_node);
/*static void tbtl_remove_bs(TBTLNode **root, TBTLNode *deleting_node);*/
/*static void tbtl_remove_splay(char type, TBTLNode **root, TBTLNode *deleting_node);*/
/*static void tbtl_remove_random(TBTLNode **root, TBTLNode *deleting_node);*/
/*static void tbtl_remove_treap(TBTLNode **root, TBTLNode *deleting_node);*/
/*static void tbtl_remove_aa(TBTLNode **root, TBTLNode *deleting_node);*/
/*static void tbtl_remove_rb(TBTLNode **root, TBTLNode *deleting_node);*/
/*static void tbtl_remove_avl(TBTLNode **root, TBTLNode *deleting_node);*/

/* ------ Auxiliary functions ------ */
/*static TreeNode *tbtl_rotate_left(TreeNode *current);*/
/*static TreeNode *tbtl_rotate_right(TreeNode *current);*/
/*static int tbtl_son_side(TBTLNode *up, TBTLNode *current);*/
/*static void tbtl_refresh_weight(TBTLNode *current);*/
/*static void tbtl_refresh_value(TBTLNode *current);*/
/*static void tbtl_refresh_balance(TBTLNode *current)*/
/*static void tbtl_cut_node(TBTLNode *cuting_node, TBTLNode *son);*/
/*static void tbtl_remove_node(TBTLNode **root, TBTLNode *deleting_node);*/
/*static void tbtl_replace_node(TBTLNode **root, TBTLNode *deleting_node, TBTLNode *replacing_node);*/
/* =================================== */

#endif
