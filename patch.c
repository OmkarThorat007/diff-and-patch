#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<errno.h>
#include<fcntl.h>
#include<unistd.h>
#include"list.h"
#include"stack.h"
#include"queue.h"
#include<getopt.h>
int check_str(char *string, char key){
	int i = 0, len, count = 0;
	len = strlen(string);
	while(i < len){
		if(string[i] == key){
			count++;
		}
		i++;
	}
	if(count == 1)
		return 1;
	else if(count == 0)
		return 0;
	else
		return -1;
}
void print_from_file1(FILE *fd_new, int file_key, char (*file1)[2000]){
	int k = 1;
	if(file_key != 1){
		k = 1;
		while(k < file_key){
			fprintf(fd_new, "%s\n", file1[k]);
			k++;
		}
	}
}
char *rev_strstr(char *string, char key){
	int len, i = 0;
	char *string_ret;
	string_ret = (char *)malloc(40);
	len = strlen(string);
	strcpy(string_ret, string);
	while(i < len){
		if(string_ret[i] == key)
			string_ret[i] = '\0';
		i++;
	}
	return string_ret;
}
int readline(char (*file)[2000], int fd){
	//reads lines into 2D character array and returns the number of lines read
	int i = 1, j = 0, ret, len;
	char ch;
	while(1){
		ret = read(fd, &ch, 1);
		if(ret == 0){
			break;
		}
		if(ch == '\n'){
			file[i][j] = '\0';
			i++;
			j = 0;
		}
		else{
			file[i][j] = ch;
			j++;
		}
	}
	len = i - 1;
	return len;
}
int main(int argc, char *argv[]){
	int fd1, fd_patch, len_f1, len_f2, x, y, count = 0;
	int len1, i, k, file1_start = 1, file2_start = 1, file1_end, file2_end;
	int file1_counter, file2_counter;
	stack s1, s2;
	init(&s1);
	init(&s2);
	FILE *fd_new;
	char patch_file[1000][2000], filename1[30], filename2[30], *str, line[2000];
	char file1[1000][2000], *ptr;
	if(argc != 3){
		if(argc > 3)
			printf("patch : %s: extra operand\n", argv[3]);
		exit(-1);
	}
	//opening file1
	fd1 = open(argv[1], O_RDONLY);
	if(fd1 == -1){
		perror("open failed");
		return errno;
	}
	//opening file having output of diff
	fd_patch = open(argv[2], O_RDONLY);
	if(fd_patch == -1){
		perror("open failed");
		return errno;
	}
	//reading the files into a 2D character array
	len_f1 = readline(file1, fd1);
	len1 = readline(patch_file, fd_patch);
	fd_new = fopen("patchfile", "w+");
	if(patch_file[1][0] == '*' && patch_file[1][1] == '*' && patch_file[1][2] == '*'){
		//-c output is to be used to patch the file
		//to be deleted	
		str = strtok(patch_file[1], " ");
		str = strtok(NULL, " ");
		strcpy(filename1, str);
		str = strtok(patch_file[2], " ");
		str = strtok(NULL, " ");
		strcpy(filename2, str);
		i = 3;
		while(strcmp(patch_file[i], "***************") == 0){
			strcpy(line, patch_file[i + 1]);
			str = strtok(line, " ");
			str = strtok(NULL, ",");
			file1_start = atoi(str);
			if(!(isempty(&s1))){
				if(file1_start != stop(&s1)){
					k = stop(&s1) + 1;
					while(k < file1_start){
						fprintf(fd_new, "%s\n", file1[k]);
						k++;
					}
				}
			}
			str = strtok(NULL, " ");
			file1_end = atoi(str);
			push(&s1, file1_start);
			push(&s1, file1_end);
			if(patch_file[i + 2][0] == '-' && patch_file[i + 2][1] == '-' && patch_file[i + 2][2] == '-'){
				
				i = i + 2;
			}
			else{
				k = i + 2;
				while(k < i + file1_end - file1_start + 3){
					if(patch_file[k][0] != '!' && patch_file[k][0] != '-' && patch_file[k][0] != ' '){
						printf("patch : **** malformed patch at line %d : %s \n", k, patch_file[k]);
						exit(-1);
					}
					k++;
				}
				i = i + file1_end - file1_start + 3;
			}
			strcpy(line, patch_file[i]);
			//i is the counter where --- x, y ---- is printed
			str = strtok(line, " ");
			str = strtok(NULL, ",");
			file2_start = atoi(str);
			str = strtok(NULL, " ");
			file2_end = atoi(str);
			if(patch_file[i + 1][0] == '*' && patch_file[i + 1][1] == '*' && patch_file[i + 1][2] == '*' && patch_file[i + 1][3] == '*'){
				//if second part of the hunk is not printed
				//we go back and use the stack
				if(patch_file[i - 1][0] == '*'){
					//if the first part of the hunk is also not printed
					x = pop(&s1);
					y = pop(&s1);
					k = y;
					while(k <= x){
						fprintf(fd_new, "%s\n", file1[k]);
						k++;
					}
					push(&s1, y);
					push(&s1, x);
				}
				else{
					x = pop(&s1);
					y = pop(&s2);
					k = i - (x - y + 1);
					//k is at the point where printing is to be started
					while(k < i){
						if(patch_file[k][0] == ' '){
							fprintf(fd_new, "%s\n", file1[y + count]);
							count++;
						}
						k++;
					}
					push(&s1, y);
					push(&s1, x);
				}
				count = 0;
				i = i + 1;
			}
			else if(i == len1){
				//if the hunk is not printed
				x = pop(&s1);
				y = pop(&s1);
				k = i - (x - y + 1);
				//k is the start point from where the printing has to start
				while(k < i){
					if(patch_file[k][0] == ' '){
						fprintf(fd_new, "%s\n", file1[y + count]);
						count++;
					}
					k++;			
				}
				break;
			}
			else{
				k = i + 1;
				while(k <= file2_end - file2_start + i + 1){
					ptr = patch_file[k];
					ptr = ptr + 2;
					fprintf(fd_new, "%s\n", ptr);
					k++;
				}
				k = i + 1;
				while(k <= file2_end - file2_start + i + 1){
					if(patch_file[k][0] != '!' && patch_file[k][0] != '+' && patch_file[k][0] != ' '){
						printf("patch : **** malformed patch at line %d : %s\n", k, patch_file[k]);
						exit(-1);
					}
					k++;
				}
				i = i + file2_end - file2_start + 2;
			}
		}
	}
	else{
		//normal diff output is to be used
		file1_start = 0;
		file1_end = 0;
		file2_start = 0;
		file2_end = 0;
		file1_counter = 0;
		file2_counter = 0;
		char *temp = NULL, *str1, *str2, *temp2 = NULL;
		char *temp3 = NULL, *temp4 = NULL;
		char instr;
		int ret, flag = 0;
		i = 1;
		while(i <= len1){
			//tokenising the line
			file1_counter = 0;
			file2_counter = 0;
			if((temp = strstr(patch_file[i], "c")) != NULL){
				temp2 = rev_strstr(patch_file[i], 'c');
				ret = check_str(temp2, ',');
				if(ret == -1)
					exit(-1);
				if(ret){
					file1_start = atoi(rev_strstr(temp2, ','));
					temp4 = strtok(temp2, ",");
					temp4 = strtok(NULL, "\n");
					file1_end = atoi(temp4);
				}
				else{
					file1_counter = atoi(temp2);
				}
				str1 = strtok(temp, "c");
				ret = check_str(temp, ',');
				if(ret == -1)
					exit(-1);
				if(ret){
					file2_start = atoi(rev_strstr(str1, ','));
					str2 = strtok(str1, ",");
					str2 = strtok(NULL," \n");
					file2_end = atoi(str2);
				}
				else{
					file2_counter = atoi(str1);
				}
				instr = 'c';
			}
			else if((temp = strstr(patch_file[i], "d")) != NULL){
				temp2 = rev_strstr(patch_file[i], 'd');
				ret = check_str(temp2, ',');
				if(ret == -1)
					exit(-1);
				if(ret){
					file1_start = atoi(rev_strstr(temp2, ','));
					temp4 = strtok(temp2, ",");
					temp4 = strtok(NULL, "\n");
					file1_end = atoi(temp4);
				}
				else{
					file1_counter = atoi(temp2);
				}
				str1 = strtok(temp, "d");
				ret = check_str(temp, ',');
				if(ret == -1)
					exit(-1);
				if(ret){
					file2_start = atoi(rev_strstr(str1, ','));
					str2 = strtok(str1, ",");
					str2 = strtok(NULL," \n");
					file2_end = atoi(str2);
				}
				else{
					file2_counter = atoi(str1);
				}
				instr = 'd';
			}
			else if((temp = strstr(patch_file[i], "a")) != NULL){
				temp2 = rev_strstr(patch_file[i], 'a');
				ret = check_str(temp2, ',');
				if(ret == -1)
					exit(-1);
				if(ret){
					file1_start = atoi(rev_strstr(temp2, ','));
					temp4 = strtok(temp2, ",");
					temp4 = strtok(NULL, "\n");
					file1_end = atoi(temp4);
				}
				else{
					file1_counter = atoi(temp2);
				}
				str1 = strtok(temp, "a");
				ret = check_str(temp, ',');
				if(ret == -1)
					exit(-1);
				if(ret){
					file2_start = atoi(rev_strstr(str1, ','));
					str2 = strtok(str1, ",");
					str2 = strtok(NULL," \n");
					file2_end = atoi(str2);
				}
				else{
					file2_counter = atoi(str1);
				}
				instr = 'a';
			}	
			else{

				exit(-1);
			}
			if(instr == 'c'){
				if(file1_counter == 0 && file2_counter == 0){
					//output is of type 1,3c6,7
					if(!(isempty(&s1))){
						if(file1_start != stop(&s1)){
							k = stop(&s1) + 1;
							while(k < file1_start){
								fprintf(fd_new, "%s\n", file1[k]);
								k++;
							}
						}
					}
					if((isempty(&s1))){
						print_from_file1(fd_new, file1_start, file1);
					}
					push(&s1, file1_start);
					push(&s1, file1_end);
					k = i + 3 + file1_end - file1_start;
					while(k <= i + 3 + file1_end - file1_start + file2_end - file2_start){
						ptr = patch_file[k];
						ptr = ptr + 2;
						fprintf(fd_new, "%s\n", ptr);
						k++;
					}
					//increment i
					i = i + 4 + file1_end - file1_start + file2_end - file2_start;	
				}
				else if(file1_counter != 0 && file2_counter == 0){
					//3c5,6
					if(!(isempty(&s1))){
						if(file1_counter != stop(&s1)){
							k = stop(&s1) + 1;
							while(k < file1_counter){
								fprintf(fd_new, "%s\n", file1[k]);
								k++;
							}
						}
					}
					if((isempty(&s1))){
						print_from_file1(fd_new, file1_counter, file1);						
					}
					push(&s1, file1_counter);
					k = i + 3;
					while(k <= file2_end - file2_start + i + 3){
						ptr = patch_file[k];
						ptr = ptr + 2;
						fprintf(fd_new, "%s\n", ptr);
						k++;	
					}
					//increment i
					i = i + 4 + file2_end - file2_start;
				}
				else if(file1_counter == 0 && file2_counter != 0){
					//2,3c5
					if(!(isempty(&s1))){
						if(file1_start != stop(&s1)){
							k = stop(&s1) + 1;
							while(k < file1_start){
								fprintf(fd_new, "%s\n", file1[k]);
								k++;
							}
						}
					}
					if((isempty(&s1))){
						print_from_file1(fd_new, file1_start, file1);						
					}
					push(&s1, file1_start);
					push(&s1, file1_end);
					k = i + 3 + file1_end - file1_start;
					ptr = patch_file[k];
					ptr = ptr + 2;
					fprintf(fd_new, "%s\n", ptr);
					//increment i
					i = i + 4 + file1_end - file1_start;
				}
				else if(file1_counter != 0 && file2_counter != 0){
					//4c6
					if(!(isempty(&s1))){
						if(file1_counter != stop(&s1)){
							k = stop(&s1) + 1;
							while(k < file1_counter){
								fprintf(fd_new, "%s\n", file1[k]);
								k++;
							}
						}
					}
					if((isempty(&s1))){
						print_from_file1(fd_new, file1_counter, file1);
					}
					push(&s1, file1_counter);
					k = i + 3;
					ptr = patch_file[k];
					ptr = ptr + 2;
					fprintf(fd_new, "%s\n", ptr);
					//increment i
					i = k + 1;
				}
			}
			else if(instr == 'd'){
			//1,10d3
				if(file1_counter == 0){
				//output is of type 1,10d3
					if(!(isempty(&s1))){
						if(file1_start != stop(&s1)){
							k = stop(&s1) + 1;
							while(k < file1_start){
								fprintf(fd_new, "%s\n", file1[k]);
								k++;
							}
						}
					}
					if((isempty(&s1))){
						print_from_file1(fd_new, file1_start, file1);
					}
					push(&s1, file1_start);
					push(&s1, file1_end);
					//increment i
					i = i + file1_end - file1_start + 3;
				}
				else{
				//output is of type 3d4
					if(!(isempty(&s1))){
						if(file1_counter != stop(&s1)){
							k = stop(&s1) + 1;
							while(k < file1_counter){
								fprintf(fd_new, "%s\n", file1[k]);
								k++;
							}
						}
					}
					if((isempty(&s1))){
						print_from_file1(fd_new, file1_counter, file1);
					}
					push(&s1,file1_counter);
					//increment i
					i = i + 3;
				}
			}
			else if(instr == 'a'){
			//1a2,5
				if(file2_counter == 0){
				//output is of type 1a2,5
					if(!(isempty(&s1))){
						if(file1_counter != stop(&s1)){
							k = stop(&s1) + 1;
							while(k <= file1_counter){
								fprintf(fd_new, "%s\n", file1[k]);
								k++;
							}
						}
					}
					if((isempty(&s1))){
						print_from_file1(fd_new, file1_counter, file1);
					}
					push(&s1, file1_counter);
					//add lines from file2
					if(file1)
					k = i + 2;
					while(k <= i + 2 + file2_end - file2_start){
						ptr = patch_file[k];
						ptr = ptr + 2;
						fprintf(fd_new, "%s\n", ptr);
						k++;
					}
					//increment i
					i = i + file2_end - file2_start + 3;
				}
				else{
				//output is of type 1a4
					if(!(isempty(&s1))){
						if(file1_counter != stop(&s1)){
							k = stop(&s1) + 1;
							while(k <= file1_counter){
								fprintf(fd_new, "%s\n", file1[k]);
								k++;
							}
						}
					}
					if((isempty(&s1))){
						print_from_file1(fd_new, file1_counter, file1);
					}
					push(&s1, file1_counter);
					//add lines from file2
					k = i + 2;
					ptr = patch_file[k];
					ptr = ptr + 2;
					fprintf(fd_new, "%s\n", ptr);
					//increment i
					i = i + 3;
				}				
				
			}
		}
	}
	if(remove(argv[1]) == 0)
		rename("patchfile", argv[1]);
	else{
		printf("error in deleting the file %s , check file named : 'patchfile'\n", argv[1]);
	}
	close(fd_patch);
	close(fd1);
	fclose(fd_new);
}
