#include"list.h"
#include<stdio.h>
#include<stdlib.h>
//position starts from 1
void linit(list *l){
	l->head = NULL;
	l->tail = NULL;
}
void append(list *l, int val){
	lnode *tmp;
	tmp = (lnode *)malloc(sizeof(lnode));
	tmp->next = NULL;
	tmp->prev = NULL;
	tmp->num = val;
	if(l->head == NULL){
		l->head = tmp;
		l->tail = tmp;
	}
	else{
		l->tail->next = tmp;
		tmp->prev = l->tail;
		l->tail = tmp;
	}
}
void insert_front(list *l, int val, int pos){
	lnode *p;
	p = l->head;
	lnode *tmp;
	int i;
	tmp = (lnode *)malloc(sizeof(lnode));
	tmp->next = NULL;
	tmp->prev = NULL;
	tmp->num = val;
	if(p == NULL){
		if(pos != 1)
			return;
		else{
			l->head = tmp;
			l->tail = tmp;
		}
	}
	else{
		int len = listlen(l);
		if(pos <= 0 || pos > len + 1){
			return;
		}
		else{
			p = l->head;
			if(pos == 1){
				tmp->next = l->head;
				l->head->prev = tmp;
				l->head = tmp;
			}
			else if(pos == len + 1){
				l->tail->next = tmp;
				tmp->prev = l->tail;
				l->tail = l->tail->next;
			}
			else{
				i = 1;
				while(i < pos - 1){
					p = p->next;
					i++;
				}
				tmp->next = p->next;
				tmp->prev = p;
				p->next = tmp;
				tmp->next->prev = tmp;
			}
		}
	}
}
void insert_back(list *l, int val, int src_pos, int n){
	//goes from source position in a list BACK n steps and inserts a value at that position
	lnode *tmp, *p;
	tmp = (lnode *)malloc(sizeof(lnode));
	tmp->next = NULL;
	tmp->prev = NULL;
	tmp->num = val;
	int i = 1, len, diff;
	diff = src_pos - n;
	if(diff <= 0)
		return;
	len = listlen(l);
	if(src_pos > len || src_pos <= 0)
		return;
	p = l->head;
	while(i < src_pos){
		p = p->next;
		i++;
	}
	i = 0;
	while(i <= n){
		p = p->prev;
		i++;
	}
	tmp->next = p->next;
	tmp->prev = p;
	p->next->prev = tmp;
	p->next = tmp;
}
int remov(list *l, int pos){
	int ans;
	if(l->head != NULL){
		lnode *p, *q;
		int i = 1;
		p = l->head;
		int len = listlen(l);
		if(pos > 0 && pos <= len){
			if(pos == 1){
				ans = l->head->num;
				l->head = l->head->next;
				l->head->prev = NULL;
				free(p);
				return ans;
			}
			else if(pos == len){
				p = l->tail;
				ans = p->num;
				l->tail = l->tail->prev;
				l->tail->next = NULL;
				free(p);
				return ans;
			}
			else{
				while(i < pos - 1){
					p = p->next;
					i++;
				}
				q = p->next;
				p->next = q->next;
				p->next->prev = p;
				ans = q->num;
				free(q);
				return ans;
			}
		}
	}
}
int lisempty(list *l){
	return (l->head == NULL && l->tail == NULL);
}
void printlist(list *l){
	lnode *p;
	p = l->head;
	if(p == NULL)
		printf("[ ]\n");
	else{
		printf("[");
		while(p != NULL){
			printf(" %d", p->num);
			p = p->next;
		}
		printf("]\n");
	}
}
int listlen(list *l){
	lnode *p;
	int count = 0;
	p = l->head;
	if(p == NULL)
		return 0;
	else{
		while(p != NULL){
			count++;
			p = p->next;
		}
		return count;
	}
}
int traverse(list *l, int pos){
	if(l->head != NULL){
		int len = listlen(l);
		int ans, i = 1;
		if(pos > 0 && pos <= len){
			lnode *p;
			p = l->head;
			if(pos == 1)
				return l->head->num;
			else if(pos == len)
				return l->tail->num;
			else{
				while(i < pos){
					p = p->next;
					i++;
				}
				return p->num;
			}
		}
	}
}
