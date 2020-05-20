#include<stdlib.h>
#include"queue.h"
void qinit(queue *q){
	*q = NULL;
}
void enq(queue *q, int num){
	qnode *tmp, *p;
	tmp = (qnode *)malloc(sizeof(qnode));
	tmp->next = NULL;
	tmp->val = num;
	if(*q == NULL){
		*q = tmp;
	}
	else{
		p = *q;
		while(p->next != NULL){
			p = p->next;
		}
		p->next = tmp;
	}
}
int deq(queue *q){
	qnode *tmp;
	tmp = *q;
	int ans;
	ans = tmp->val;
	if((*q)->next != NULL)
		*q = (*q)->next;
	else
		*q = NULL;
	free(tmp);
	return ans;
}
int lastq(queue *q){
	int ans;
	qnode *p;
	p = *q;
	while(p->next != NULL){
		p = p->next;
	}
	ans = p->val;
	return ans;
}
int qlen(queue *q){
	int ans = 1;
	qnode *p;
	p = *q;
	while(p->next != NULL){
		p = p->next;
		ans++;
	}
	return ans;
}
int qisfull(queue *q){
	return 0;
}
int qisempty(queue *q){
	return *q == NULL;
}
