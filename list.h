typedef struct lnode{
	int num;
	struct lnode *next;
	struct lnode *prev;
}lnode;
typedef struct list{
	lnode *head;
	lnode *tail;
}list;
//position starts from 1
void linit(list *l);
void append(list *l, int val);
void insert_front(list *l, int val, int pos);
void insert_back(list *l, int val, int src_pos, int n);
int remov(list *l, int pos);
int lisempty(list *l);
int listlen(list *l);
void printlist(list *l);
int traverse(list *l, int pos);
