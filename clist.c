#include"clist.h"
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
void cinit(clist *l){
	(*l) = NULL;
}
void cinsert(clist *l, char *filename, char *filepath, int type, int dir, int pos){
	cnode *temp;
	temp = (cnode *)malloc(sizeof(cnode));
	strcpy(temp->filename, filename);
	strcpy(temp->filepath, filepath);
	temp->type = type;
	temp->dir = dir;
	if(*l == NULL){
		if(pos != 0){
			//l is null and position entered is wrong
			return;
		}
		else{
			*l = temp;
			(*l)->next = NULL;
		}
	}
	
	else{
		cnode *p;
		p = *l;
		int len = 1;
		while(p->next != NULL){
			p = p->next;
			len++;
		}
		if(pos > len + 2){
			printf("wrong position\n");
			//wrong position
			return;
		}
		else{
			int i = 0;
			cnode *q;
			q = *l;
			if(pos == 0){
				//appending at the front
				temp->next = *l;
				*l = temp;
			}
			else if(pos == len){
				//appending at the end
				p->next = temp;
				temp->next = NULL;
				
			}
			else{
				//inserting in between
				while(i < pos - 1)
					q = q->next;
				printf("out of while loop\n");
				temp->next = q->next;
				q->next = temp;
			}
		}
	}
}
int get_type(clist *l, int num){
	cnode *temp;
	temp = *l;
	int i = 0;
	while(i < num){
		temp = temp->next;
		i++;
	}
	return temp->type;
}
int check(clist *l, int num){
	//checks for if next element in the list has same filename
	//returns zero if filename not found, one if filename is found
	cnode *temp;
	temp = *l;
	int i = 0;
	while(i < num){
		temp = temp->next;
		i++;
	}
	if(strcmp(temp->filename, temp->next->filename) == 0){
		return 1;
	}
	else
		return 0;
}
int checkdir(clist *l, int num){
	//returns the directory of the element at a given position
	cnode *temp;
	temp = *l;
	int i = 0;
	while(i < num){
		temp = temp->next;
		i++;
	}
	if(temp->dir == 1){
		return 1;
	}
	else
		return 2;
}
char *getfile(clist *l, int num){
	cnode *temp;
	temp = *l;
	char file[60];
	int i = 0;
	while(i < num){
		temp = temp->next;
		i++;
	}
	strcpy(file, temp->filename);
}
void printfilename(clist *l, int num){
	cnode *temp;
	temp = *l;
	int i = 0;
	while(i < num){
		temp = temp->next;
		i++;
	}
	printf("%s\n", temp->filename);
}
void csort(clist *l){
	//sorts the clist
	cnode *p, *q;
	p = *l;
	char string1[100], string2[100];
	int len = 0, i, j, temp1, temp2;
	while( p->next != NULL){
		p = p->next;
		len++;
	}
	p = *l;
	for(i = 0; i < len; i++){
		q = p->next;
		for(j = i ; j < len; j++){
			if((strcmp(p->filename, q->filename)) >= 0){
				strcpy(string1, p->filename);
				strcpy(string2, p->filepath);
				temp1 = p->type;
				temp2 = p->dir;
				strcpy(p->filename, q->filename);
				strcpy(p->filepath, q->filepath);
				p->type = q->type;
				p->dir = q->dir;
				strcpy(q->filename, string1);
				strcpy(q->filepath, string2);
				q->type = temp1;
				q->dir = temp2;
			}
			q = q->next;
		}
		p = p->next;
	}
}
void creverse(clist *l){
	//reverses the clist
	cnode *p, *q, *r;
	if(*l == NULL)
		return;
	else{
		p = *l;
		if(p->next == NULL)
			return;
		else{
			q = p->next;
			if(q->next == NULL){
				p->next = NULL;
				q->next = p;
				*l = q;
			}
			else{
				r = q->next;
				p->next = NULL;
				while(q != NULL){
					q->next = p;
					p = q;
					q = r;
					if(r != NULL)
						r = r->next;
				}
				*l = p;
			}
		}
	}
}
void printclist(clist *l){
	//prints the clist
	cnode *p;
	p = *l;
	if(p == NULL){
		printf("[ ]\n");
	}
	else{
		printf("[");
		while(p != NULL){
			printf("filename: %s file-type : %d directory: %d\n", p->filename, p->type, p->dir);
			p = p->next;
		}
		printf(" ]\n");
	}
}
int clen(clist *l){
	//gives the length of the clist
	cnode *p;
	p = *l;
	int ans = 0;
	if(p == NULL)
		return 0;
	else{
		while(p != NULL){
			ans++;
			p = p->next;
		}
	}
	return ans;
}
