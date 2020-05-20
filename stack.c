#include<stdlib.h>
#include"stack.h"
void init(stack *s){
	*s = NULL;
}
void push(stack *s, int num){
	node *tmp;
	tmp = (node *)malloc(sizeof(node));
	tmp->next = NULL;
	tmp->val = num;
	if(*s == NULL){
		*s = tmp;
	}
	else{
		tmp->next = *s;
		*s = tmp;
	}
}
int pop(stack *s){
	int ans;
	node *tmp;
	tmp = *s;
	ans = tmp->val;
	*s = (*s)->next;
	tmp->next = NULL;
	free(tmp);
	return ans;
}
int isempty(stack *s){
	return *s == NULL;
}
int isfull(stack *s){
	return 0;
}
int stop(stack *s){
	int ans;
	ans = pop(s);
	push(s, ans);
	return ans;
}
