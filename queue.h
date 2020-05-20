typedef struct qnode{
	int val;
	struct qnode *next;
}qnode;
typedef qnode *queue;
void qinit(queue *q);
void enq(queue *q, int num);
int deq(queue *q);
int qisfull(queue *q);
int qisempty(queue *q);
int lastq(queue *q);
int qlen(queue *q);
