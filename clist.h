#define MAX 128
typedef struct cnode{
	char filename[MAX];
	char filepath[MAX];
	int type;
	int dir;
	struct cnode *next;
}cnode;
typedef cnode *clist;
void cinit(clist *l);
void cinsert(clist *l, char *filename, char *filepath, int type, int dir, int pos);
int check(clist *l, int num);
int checkdir(clist *l, int num);
void printfilename(clist *l, int num);
void csort(clist *l);
void creverse(clist *l);
void printclist(clist *l);
int clen(clist *l);
char *getfile(clist *l, int num);
int get_type(clist *l, int num);
