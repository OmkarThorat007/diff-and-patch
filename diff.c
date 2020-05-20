#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<time.h>
#include<sys/sysmacros.h>
#include<errno.h>
#include<fcntl.h>
#include<unistd.h>
#include"list.h"
#include"stack.h"
#include"queue.h"
#include"clist.h"
#include<getopt.h>
#include<dirent.h>
int flag_i = 0, flag_b = 0,flag_w = 0;
int flag_r = 0, flag_p = 0, flag_s = 0;
//flags for output display
int flag_t = 0, flag_y = 0,flag_c = 0;
//flags for checking if the argument is a file or folder
int flag1_dir = 0, flag2_dir = 0;
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
void check_file_directory(int fd1, int fd2){
	struct stat fc1;
	struct stat fc2;
	fstat(fd1, &fc1);
	if(S_ISDIR(fc1.st_mode)){
		flag1_dir = 1;
	}
	fstat(fd2, &fc2);
	if(S_ISDIR(fc2.st_mode)){
		flag2_dir = 1;
	}
}
void print_mod_time(char *filename){
	struct stat file;
	stat(filename, &file);
	printf("  %s", ctime(&file.st_mtime));
}
int readline(char (*file)[2000], int fd){
	//reads lines into 2D character array (starting from line number 1) and returns the number of lines read
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
void changefile(char (*file1)[2000], char (*file2)[2000], int len1, int len2){
	//changes tab to spaces
	int i = 1, j;
	char temp[2000];
	char *val = NULL;
	while(i <= len1){
		while(val = strstr(file1[i], "\t")){
			strcpy(temp, val + 1);
			for(j = 0; j < 8; j++){
				*val = ' ';
				val = val + 1;
			}
			*val = '\0';
			strcat(file1[i], temp);
		}
		i++;
	}
	i = 1;
	val = NULL;
	while(i <= len2){
		while(val = strstr(file2[i], "\t")){
			strcpy(temp, val + 1);
			for(j = 0; j < 8; j++){
				*val = ' ';
				val = val + 1;
			}
			*val = '\0';
			strcat(file2[i], temp);
		}
		i++;
	}
}
void strcmpbb(char *string1, char *string2, int len1, int len2){
	//removes existing white spaces
	int i = 0, m, internal_flag = 1;
	while(i < len1){
		internal_flag = 1;
		if(string1[i] == ' ' && string1[i + 1] == ' '){
			internal_flag = 0;
			for(m = i; m < len1; m++)
				string1[m] = string1[m + 1];
			len1--;
		}
		if(internal_flag)
			i++;
	}
	i = 0;
	internal_flag = 1;
	while(i < len2){
		internal_flag = 1;
		if(string2[i] == ' ' && string2[i + 1] == ' '){
			internal_flag = 0;
			for(m = i; m < len2; m++)
				string2[m] = string2[m + 1];
			len2--;
		}
		if(internal_flag)
			i++;
	}
}
void strcmpww(char *string1, char *string2, int len1, int len2){
	//eliminates all spaces from the strings
	int i = 0, m, internal_flag = 1;
	while(i < len1){
		internal_flag = 1;
		if(string1[i] == ' '){
			internal_flag = 0;
			for(m = i; m < len1; m++)
				string1[m] = string1[m + 1];
			len1--;
		}
		if(internal_flag)
			i++;
	}
	i = 0;
	while(i < len2){
		internal_flag = 1;
		if(string2[i] == ' '){
			internal_flag = 0;
			for(m = i; m < len2; m++)
				string2[m] = string2[m + 1];
			len2--;
		}
		if(internal_flag)
			i++;
	}
}
int strcmpii(char *string1, char *string2){
	//compares the strings without taking into account higher or lower case
	int len1 = strlen(string1);
	int len2 = strlen(string2);
	int i = 0;
	if(len1 != len2)
		return 1;
	else{
		while(i <= len1){
			if(string1[i] == string2[i] || ((string1[i] - string2[i]) == 'A' - 'a') ||((string1[i] - string2[i]) == 'a' - 'A')){
				i++;
			}
			else
				break;
		}
		if(i == len1 + 1)
			return 0;
		else
			return 1;
	}
}
int mystrcmp(char *line1, char *line2){
	//depending on the flags set the function calls other functions
	char *string1, *string2;
	int len1, len2;
	len1 = strlen(line1);
	len2 = strlen(line2);
	string1 = (char *)malloc(len1 + 1);
	string2 = (char *)malloc(len2 + 1);
	strcpy(string1, line1);
	strcpy(string2, line2);
	if(flag_i == 0 && flag_b == 0 && flag_w == 0){
	//no option
		return strcmp(string1, string2);
	}	
	else if(flag_i == 1 && flag_b == 0 && flag_w == 0){
	//i only
		return strcmpii(string1, string2);
	}
	else if(flag_i == 0 && flag_b == 1 && flag_w == 0){
	//b only
		strcmpbb(string1, string2, len1, len2);
		return strcmp(string1, string2);
	}
	else if((flag_i == 0 && flag_b == 0 && flag_w == 1) || (flag_i == 0 && flag_b == 1 && flag_w == 1)){
	//w only or (b and w)
		strcmpww(string1, string2, len1, len2);
		return strcmp(string1, string2);
	}
	else if(flag_i == 1 && flag_b == 1 && flag_w == 0){
	//i and b option
		strcmpbb(string1, string2, len1, len2);
		return strcmpii(string1, string2);
	}
	else if( (flag_i == 1 && flag_b == 0 && flag_w == 1)  ||  (flag_i == 1 && flag_b == 1 && flag_w == 1)){
	//i and w option or all comparison option
		strcmpww(string1, string2, len1, len2);
		return strcmpii(string1, string2);
	}
}
void print_output(queue *f1, queue *f2, char (*file1)[2000], char (*file2)[2000], int len1, int len2){
	/*printing the output*/
	int f1_index, f2_index, f1_end, f2_end, f1_len = 1, f2_len = 1, i = 1, j = 1, p1, p2, a, b;
	if(flag_p == 1){
	//if there is no longest common subsequence
	//or if one of the files is blank
	//we print that entire file has to be changed
		if(len1 == 0 || len2 == 0){
			if(len1 == 0 && len2 == 0)
				exit(-1);
			else if(len1 == 0){
				printf("0a1,%d\n", len2);
				printf("---\n");
				while(i <= len2){
					printf("> %s\n", file2[i]);
					i++;
				}
				return;
			}
			else if(len2 == 0){
				printf("1,%dd0\n", len1);
				while(i <= len1){
					printf("< %s\n", file1[i]);
					i++;
				}
				printf("---\n");
				return;
			}
		}
		else{
			if(len1 != 1 && len2 != 1)
				printf("1,%dc1,%d\n",len1, len2);
			else if(len1 == 1 && len2 != 1)
				printf("1c1,%d\n", len2);
			else if(len1 != 1 && len2 == 1)
				printf("1,%dc1\n", len1);
			else
				printf("1c1\n");
			while(i <=len1){
				printf("< %s\n", file1[i]);
				i++;
			}
			printf("---\n");
			i = 1;
			while(i <=len2){
				printf("> %s\n", file2[i]);
				i++;
			}
			return;
		}
	}
	f1_index = deq(f1);
	f2_index = deq(f2);
	f1_end = f1_index;
	f2_end = f2_index;
	if(!(qisempty(f1))){
		f1_end = lastq(f1);
	}
	if(!(qisempty(f2))){
		f2_end = lastq(f2);
	}
	if(!(qisempty(f1)))
		f1_len = qlen(f1);
	if(!(qisempty(f2)))
		f2_len = qlen(f2);
	//printing lines before first line of lcs
	if(f1_index == 1 && f2_index != 1){
		//printing statement/changes
		if(f2_index - 1 != 1)
			printf("0a1,%d\n", f2_index - 1);
		else
			printf("0a1\n");
		//printing lines
		printf("---\n");
		while(i <= f2_index - 1){
			printf("> %s\n", file2[i]);
			i++;
		}
	}
	else if(f1_index != 1 && f2_index == 1){
		//printing statement/changes
		if(f1_index - 1 != 1)
			printf("1,%dd0\n", f1_index - 1);
		else
			printf("1d0\n");
		//printing lines
		while(i <= f1_index - 1){
			printf("< %s\n", file1[i]);
			i++;
		}
		printf("---\n");
	}
	else if(f1_index != 1 && f2_index != 1){
		//printing statement/changes
		if(f1_index - 1 != 1 && f2_index - 1 != 1)
			printf("1,%dc1,%d\n", f1_index - 1, f2_index - 1);
		else if(f1_index - 1 == 1 && f2_index - 1 != 1)
			printf("1c1,%d\n", f2_index - 1);
		else if(f1_index - 1 != 1 && f2_index - 1 == 1)
			printf("1,%dc1\n", f1_index - 1);
		else
			printf("1c1\n");
		//printing lines
		while(i <= f1_index - 1){
			printf("< %s\n", file1[i]);
			i++;
		}
		printf("---\n");
		i = 1;
		while(i <= f2_index - 1){
			printf("> %s\n", file2[i]);
			i++;
		}
	}
	//printing the lines in between lcs
	i = f1_index;
	a = f2_index;
	while(!(qisempty(f1))){
		j = deq(f1) - 1;
		b = deq(f2) - 1;
		if(i == j && a != b){
			if(a + 1 != b)
				printf("%da%d,%d\n", j, a + 1, b);
			else
				printf("%da%d\n", j, b);
			printf("---\n");
			p1 = a + 1;
			while(p1 <= b){
				printf("> %s\n", file2[p1]);
				p1++;
			}
		}
		else if(a == b && i != j){
			if(i + 1 != j)
				printf("%d,%dd%d\n", i + 1, j , b);
			else
				printf("%dd%d\n", j, b);
			p1 = i + 1;
			while(p1 <= j){
				printf("< %s\n", file1[p1]);
				p1++;
			}
			printf("---\n");
		}
		else if(a != b && i != j){
			if(i + 1 != j && a + 1 != b)
				printf("%d,%dc%d,%d\n", i + 1, j, a + 1, b);
			else if(i + 1 == j && a + 1 != b)
				printf("%dc%d,%d\n", j, a + 1, b);
			else if(i + 1 != j && a + 1 == b)
				printf("%d,%dc%d\n", i + 1, j, b);
			else
				printf("%dc%d\n", j, b);
			p1 = i + 1;
			while(p1 <= j){
				printf("< %s\n", file1[p1]);
				p1++;
			}
			printf("---\n");
			p2 = a + 1;
			while(p2 <= b){
				printf("> %s\n", file2[p2]);
				p2++;
			}
		}
		i = j + 1;
		a = b + 1;
	}
	//printing after last line of lcs	
	if(f1_end == len1 && f2_end != len2){
		//printing statement/changes
		if(f2_end + 1 != len2)
			printf("%da%d,%d\n", len1, f2_end + 1, len2);
		else
			printf("%da%d\n", len1, len2);
		printf("---\n");
		i = f2_end + 1;
		//printing lines
		while(i <= len2){
			printf("> %s\n", file2[i]);
			i++;
		}
	}
	else if(f1_end != len1 && f2_end == len2){
		if(f1_end + 1 != len1)
			printf("%d,%dd%d\n", f1_end + 1, len1, len2);
		else
			printf("%dd%d\n", f1_end + 1, len2);
		i = f1_end + 1;
		while(i <= len1){
			printf("< %s\n", file1[i]);
			i++;
		}
		printf("---\n");
	}
	else if(f1_end != len1 && f2_end != len2){
		//printing statement/changes
		if(f1_end + 1 != len1 && f2_end != len2)
			printf("%d,%dc%d,%d\n", f1_end + 1, len1, f2_end + 1, len2);
		else if(f1_end + 1 == len1 && f2_end + 1 != len2)
			printf("%dc%d,%d\n", len1, f2_end + 1, len2);
		else if(f1_end + 1 != len1 && f2_end + 1 == len2)
			printf("%d,%dc%d\n", len1, f1_end + 1, len2);
		else
			printf("%dc%d\n", len1, len2);
		//printing lines
		i = f1_end + 1;
		while(i <= len1){
			printf("< %s\n", file1[i]);
			i++;
		}
		printf("---\n");
		i = f2_end + 1;
		while(i <= len2){
			printf("> %s\n", file2[i]);
			i++;
		}
	}
	
}
void print_outputc(queue *f1, queue *f2, char (*file1)[2000], char (*file2)[2000], int len1, int len2, char *filename1, char *filename2){
	/*This function prints the output when -c option is selected*/
	int f1_index, f2_index, f1_end, f2_end, f1_len = 1, f2_len = 1, i = 1, j = 1, p1, p2, a, b;
	int count = 0, arr1[len1], arr2[len2], arr_index = 0, hunk[1000], lcs_len = 0, check1, check2, checkprev1, checkprev2;
	char f1_instr[len1 + 2], f2_instr[len2 + 2];
	list printlist1, printlist2;
	linit(&printlist2);
	linit(&printlist1);
	if(flag_p == 1){
	//if there is no longest common subsequence or one or both of the file is blank file 
		if(len1 == 0 || len2 == 0){
			if(len1 == 0 && len2 == 0)
				exit(-1);
			else if(len1 == 0){
				while(i <= len2){
					f2_instr[i] = '+';
					i++;
				}
				i = 1;
				printf("*** %s", filename1);
				print_mod_time(filename1);
				printf("--- %s", filename2);
				print_mod_time(filename2);
				printf("***************\n");
				printf("*** 0 ****\n");
				printf("--- 1,%d ----\n", len2);
				while(i <= len2){
					printf("%c ", f2_instr[i]);
					printf("%s\n", file2[i]);
					i++;
				}
				return;
			}
			else if(len2 == 0){
				while(i <= len1){
					f1_instr[i] = '-';
					i++;
				}
				i = 1;
				printf("*** %s", filename1);
				print_mod_time(filename1);
				printf("--- %s", filename2);
				print_mod_time(filename2);
				printf("***************\n");
				printf("*** 1,%d ****\n", len2);
				while(i <= len1){
					printf("%c ", f1_instr[i]);
					printf("%s\n", file1[i]);
					i++;
				}
				printf("--- 0 ----\n");
				return;			
			}
		}
		else{
			printf("*** %s", filename1);
			print_mod_time(filename1);
			printf("--- %s", filename2);
			print_mod_time(filename2);
			printf("*** 1,%d ****\n", len1);
			while(i <=len1){
				printf("! %s\n", file1[i]);
				i++;
			}
			printf("--- 1,%d ----\n", len2);
			i = 1;
			while(i <=len2){
				printf("! %s\n", file2[i]);
				i++;
			}
			return;
		}
	}
	f1_index = deq(f1);
	f2_index = deq(f2);
	if(!(qisempty(f1))){
		f1_end = lastq(f1);
	}
	if(!(qisempty(f2))){
		f2_end = lastq(f2);
	}
	if(!(qisempty(f1)))
		f1_len = qlen(f1);
	if(!(qisempty(f2)))
		f2_len = qlen(f2);
	
	for(i = 0;i <= len1 + 2; i++)
		f1_instr[i] = ' ';
	for(i = 0;i <= len2 + 2; i++)
		f2_instr[i] = ' ';
	//lines before first line of lcs
	if(f1_index == 1 && f2_index != 1){
		i = 1;
		while(i <= f2_index - 1){
			f2_instr[i] = '+';
			i++;
		}
	}
	else if(f1_index != 1 && f2_index == 1){
		i = 1;
		while(i <= f1_index - 1){
			f1_instr[i] = '-';
			i++;
		}
	}
	else if(f1_index != 1 && f2_index != 1){
		i = 1;
		while(i <= f1_index - 1){
			f1_instr[i] = '!';
			i++;
		}
		i = 1;
		while(i <= f2_index - 1){
			f2_instr[i] = '!';
			i++;
		}
	}
	//lines between lcs
	i = f1_index;
	a = f2_index;
	while(!(qisempty(f1))){
		j = deq(f1) - 1;
		b = deq(f2) - 1;
		if(i == j && a != b){
			p2 = a + 1;
			while(p2 <= b){
				f2_instr[p2] = '+';
				p2++;
			}
		}
		else if(i != j && a == b){
			p1 = i + 1;
			while(p1 <= j){
				f1_instr[p1] = '-';
				p1++;
			}
		}
		else if(i != j && a != b){
			p1 = i + 1;
			while(p1 <= j){
				f1_instr[p1] = '!';
				p1++;
			}
			p2 = a + 1;
			while(p2 <= b){
				f2_instr[p2] = '!';
				p2++;
			}
		}
		i = j + 1;
		a = b + 1;
	}
	//lines after lcs
	if(f1_end == len1 && f2_end != len2){
		i = f2_end + 1;
		while(i <= len2){
			f2_instr[i] = '+';
			i++;
		}
	}
	else if(f1_end != len1 && f2_end == len2){
		i = f1_end + 1;
		while(i <= len1){
			f1_instr[i] = '-';
			i++;
		}
	}
	else if(f1_end != len1 && f2_end != len2){
		i = f1_end + 1;
		while(i <= len1){
			f1_instr[i] = '!';
			i++;
		}
		i = f2_end + 1;
		while(i <= len2){
			f2_instr[i] = '!';
			i++;
		}
	}
	i = 1;
	j = 1;
	while(i <= len1){
		if(f1_instr[i] == ' '){
			//lines are in lcs
			lcs_len++;
			append(&printlist1, i);

		}
		i++;
	}
	while(j <= len2){
		if(f2_instr[j] == ' '){
			append(&printlist2, j);
		}
		j++;
	}
	//lines numbers are enqueued in 2 queues
	//dequeue the queues and check for 7 or more continous line numbers
	if(lcs_len < 7){
		i = 1;
		printf("*** %s", filename1);
		print_mod_time(filename1);
		printf("--- %s", filename2);
		print_mod_time(filename2);
		printf("***************\n");
		printf("*** 1,%d ****\n", len1);
		while(i <= len1){
			printf("%c ", f1_instr[i]);
			printf("%s\n", file1[i]);
			i++;
		}
		i = 1;
		printf("--- 1,%d ----\n", len2);
		while(i <= len2){
			printf("%c ", f2_instr[i]);
			printf("%s\n", file2[i]);
			i++;
		}
	}
	//printlist1 and printlist2 are two lists
	//we print in lists till -1 and again start from -2
	else{
		i = 1;
		check1 = 0;
		check2 = 0;
		count = 1;
		int k = 1, len, result1, copy, result2;
		while(i <= (len = listlen(&printlist1))){
			checkprev1 = check1;
			checkprev2 = check2;
			check1 = traverse(&printlist1, i);
			check2 = traverse(&printlist2, i);
			result1 = check1 - checkprev1;
			result2 = check2 - checkprev2;
			if(result1 == 1 && result2 == 1){
				count++;
				if(count >= 7 && i == len){
					insert_front(&printlist1, -2, i - 2);
					insert_back(&printlist1, -1, i + 1, count - 3);
					insert_front(&printlist2, -2, i - 2);
					insert_back(&printlist2, -1, i + 1, count - 3);
					count = 1;					
					i = i + 2;
				}
			}
			else{
				copy = count;
				count = 1;
				if(copy >= 7 && (result1 != 1 || result2 != 1)){
					insert_front(&printlist1, -2, i - 3);
					insert_back(&printlist1, -1, i + 1, copy - 2);
					insert_front(&printlist2, -2, i - 3);
					insert_back(&printlist2, -1, i + 1, copy - 2);
					count = 1;	
					i = i + 2;
				}
			}	
			i++;
		}
		len = listlen(&printlist1);
		i = 1;
		j = 1;
		p1 = 1;
		p2 = 1;
		k = 1;
		check1 = 0;
		check2 = 0;
		int print_end1 = 0, print_end2 = 0, print_start1 = 1, print_start2 = 1, flag = 1;
		printf("*** %s", filename1);
		print_mod_time(filename1);
		printf("--- %s", filename2);
		print_mod_time(filename2);
		while(k <= len){
			checkprev1 = check1;
			checkprev2 = check2;
			check1 = traverse(&printlist1,i);
			check2 = traverse(&printlist2, j);
			if(check1 == -1)
				print_end1 = checkprev1;
			if(check2 == -1)
				print_end2 = checkprev2;
			if(check1 == -2)
				print_start1 = traverse(&printlist1, i + 1);
			if(check2 == -2)
				print_start2 = traverse(&printlist2, j + 1);
			if(print_end1 != 0 && print_end2 != 0){
				printf("***************\n");
				printf("*** %d,%d ****\n", print_start1, print_end1);
				copy = print_start1;
				while(copy <= print_end1){
					if(f1_instr[copy] ==  ' ')
						copy++;
					else
						break;
				}
				if(copy == print_end1 + 1)
					flag = 0;
				if(flag == 1){
					while(print_start1 <= print_end1){
						printf("%c ", f1_instr[print_start1]);
						printf("%s\n", file1[print_start1]);
						print_start1++;
					}
				}
				printf("--- %d,%d ----\n", print_start2, print_end2);
				flag = 1;
				copy = print_start2;
				while(copy <= print_end2){
					if(f2_instr[copy] ==  ' ')
						copy++;
					else
						break;
				}
				if(copy == print_end2 + 1)
					flag = 0;
				if(flag == 1){
					while(print_start2 <= print_end2){
						printf("%c ", f2_instr[print_start2]);
						printf("%s\n", file2[print_start2]);
						print_start2++;
					}
				}
				print_end1 = 0;
				print_end2 = 0;
			}
			if(k == len){
				printf("***************\n");
				printf("*** %d,%d ****\n", print_start1, len1);
				copy = print_start1;
				while(copy <= len1){
					if(f1_instr[copy] ==  ' ')
						copy++;
					else
						break;
				}
				if(copy == len1 + 1)
					flag = 0;
				if(flag == 1){
					while(print_start1 <= len1){
						printf("%c ", f1_instr[print_start1]);
						printf("%s\n", file1[print_start1]);
						print_start1++;
					}
				}
				printf("--- %d,%d ----\n", print_start2, len2);
				flag = 1;
				copy = print_start2;
				while(copy <= len2){
					if(f2_instr[copy] ==  ' ')
						copy++;
					else
						break;
				}
				if(copy == len2 + 1)
					flag = 0;
				if(flag == 1){
					while(print_start2 <= len2){
						printf("%c ", f2_instr[print_start2]);
						printf("%s\n", file2[print_start2]);
						print_start2++;
					}
				}
			}
			i++;
			j++;
			k++;
		}
	}
}
int mystrlent(char *file){
	//returns the actual length when tabs are present in lines
	int j = 0, count = 0;;
	while(file[j] != '\0'){
		if(file[j] == '\t'){
			count = count + 8;
			j++;
		}
		else{
			count++;
			j++;
		}
	}
	return count;
}
void print_outputy(queue *f1, queue *f2, char (*file1)[2000], char (*file2)[2000], int len1, int len2){
	/*printing output when -y option is selected*/
	int f1_index, f2_index, f1_end, f2_end, f1_len = 1, f2_len = 1, i = 1, j = 1, p1, p2, a, b;
	char str[62];
	int max, num_spaces, flen, set;
	if(flag_t == 1){
		set = 4;
		max = 63;
		for(a = 0; a < 63; a++)
			str[a] = ' ';
		str[63] = '\0';
	}
	else{
		set = 3;
		max = 61;
		//str is the string having spaces
		for(a = 0; a < 61; a++)
			str[a] = ' ';
		str[61] = '\0';
	}
	i = 1;
	if(flag_s == 1){
		while(i <= len1){
			flen = mystrlent(file1[i]);
			if(flen > max){
				write(1, file1[i], max);
			}
			else{
				write(1, file1[i], flen);
				num_spaces = max - flen;
				while(num_spaces){
					write(1, " ", 1);
					num_spaces--;
				}
			}
			write(1, "    ", set);
			write(1, file2[i], max);
			printf("\n");
			i++;
		}
		return;
	}
	else if(flag_p == 1){
	//if there is no longest common subsequence
		if(len1 == 0 || len2 == 0){
			if(len1 == 0 && len2 == 0)
				return;
			else if(len1 == 0){
				i = 1;
				while(i <= len2){
					num_spaces = max;
					while(num_spaces){
						write(1, " ", 1);
						num_spaces--;
					}
					write(1, " >  ", set);
					write(1, file2[i], max);
					printf("\n");
					i++;
				}
			}
			else if(len2 == 0){
				i = 1;
				while(i <= len1){
					flen = mystrlent(file1[i]);
					if(flen > max)
						write(1, file1[i], max);
					else{
						write(1, file1[i], flen);
						num_spaces = max - flen;
						while(num_spaces){
							write(1, " ", 1);
							num_spaces--;
						}
					}
					write(1, " <  ", set);
					printf("\n");
					i++;
				}
			}
			return;
		}
		if(len1 == len2){
			while(i <= len1){
				flen = mystrlent(file1[i]);
				if(flen > max)
					write(1, file1[i], max);
				else{
					write(1, file1[i], flen);
					num_spaces = max - flen;
					while(num_spaces){
						write(1, " " , 1);
						num_spaces--;
					}
				}	
				write(1, " |  ", set);
				write(1, file2[i], max);
				printf("\n");
				i++;
			}
		}
		else if(len1 > len2){
			while(i <= len2){
				flen = mystrlent(file1[i]);
				if(flen > max)
					write(1, file1[i], max);
				else{
					write(1, file1[i], flen);
					num_spaces = max - flen;
					while(num_spaces){
						write(1, " " , 1);
						num_spaces--;
					}
				}
				write(1, " |  ", set);
				write(1, file2[i], max);
				printf("\n");
				i++;
			}
			while(i <= len1){
				flen = mystrlent(file1[i]);
				if(flen > max)
					write(1, file1[i], max);
				else{
					write(1, file1[i], flen);
					num_spaces = max - flen;
					while(num_spaces){
						write(1, " " , 1);
						num_spaces--;
					}
				}
				write(1, " <  ", set);
				printf("\n");
				i++;
			}
		}
		else if(len1 < len2){
			while(i <= len1){
				flen = mystrlent(file1[i]);
				if(flen > max)
					write(1, file1[i], max);
				else{
					write(1, file1[i], flen);
					num_spaces = max - flen;
					while(num_spaces){
						write(1, " " , 1);
						num_spaces--;
					}
				}
				write(1, " |  ", set);
				write(1, file2[i], max);
				printf("\n");
				i++;
			}
			while(i <= len2){
				write(1, str, max);
				write(1, " >  ", set);
				write(1, file2[i], max);
				printf("\n");
				i++;
			}
		}
		return;
	}
	else{
	//there exists a longest common subsequence
		f1_index = deq(f1);
		f2_index = deq(f2);
		if(!(qisempty(f1))){
			f1_end = lastq(f1);
		}
		if(!(qisempty(f2))){
			f2_end = lastq(f2);
		}
		if(!(qisempty(f1)))
			f1_len = qlen(f1);
		if(!(qisempty(f2)))
			f2_len = qlen(f2);
		/*printing lines before first line of lcs*/
		if(f1_index == 1 && f2_index != 1){
		//add
		//as file1 starts with 1st line of lcs while there are some lines before the 1st line of lcs in file2 diff suggest to insert these lines in file1
			i = 1;
			while(i <= f2_index - 1){
				write(1, str, max);
				write(1, " >  ", set);
				write(1, file2[i], max);
				printf("\n");
				i++;
			}
		}
		else if(f1_index != 1 && f2_index == 1){
		//delete
		//as file2 starts with 1st line of lcs while there are some lines before the 1st line of lcs in file1 diff suggest to delete these lines from file1
			i = 1;
			while(i <= f1_index - 1){
				flen = mystrlent(file1[i]);
				if(flen > max)
					write(1, file1[i], max);
				else{
					write(1, file1[i], flen);
					num_spaces = max - flen;
					while(num_spaces){
						write(1, " " , 1);
						num_spaces--;
					}
				}
				write(1, " <  ", set);
				write(1, str, max);
				printf("\n");
				i++;
			}
		}
		else if(f1_index != 1 && f2_index != 1){
		//change
			if(f1_index > f2_index){
			//change and delete
				i = 1;
				while(i <= f2_index - 1){
					flen = mystrlent(file1[i]);
					if(flen > max)
						write(1, file1[i], max);
					else{
						write(1, file1[i], flen);
						num_spaces = max - flen;
						while(num_spaces){
							write(1, " " , 1);
							num_spaces--;
						}
					}
					write(1, " |  ", set);
					write(1, file2[i], max);
					printf("\n");
					i++;
				}
				while(i <= f1_index - 1){
					flen = mystrlent(file1[i]);
					if(flen > max)
						write(1, file1[i], max);
					else{
						write(1, file1[i], flen);
						num_spaces = max - flen;
						while(num_spaces){
							write(1, " " , 1);
							num_spaces--;
						}
					}
					write(1, " <  ", set);
					printf("\n");
					i++;
				}
			}
			else if(f2_index > f1_index){
			//change and add
				i = 1;
				while(i <= f1_index - 1){
					flen = mystrlent(file1[i]);
					if(flen > max)
						write(1, file1[i], max);
					else{
						write(1, file1[i], flen);
						num_spaces = max - flen;
						while(num_spaces){
							write(1, " " , 1);
							num_spaces--;
						}
					}
					write(1, " |  ", set);
					write(1, file2[i], max);
					printf("\n");
					i++;
				}
				while(i <= f2_index - 1){
					write(1, str, max);
					write(1, " >  ", set);
					write(1, file2[i], max);
					printf("\n");
					i++;
				}
			}
		}
		/*printing lines n between two lines of lcs*/
		i = f1_index;
		a = f2_index;
		while(!(qisempty(f1))){
			j = deq(f1) - 1;
			b = deq(f2) - 1;
			flen = mystrlent(file1[i]);
			if(flen > max)
				write(1, file1[i], max);
			else{
				write(1, file1[i], flen);
				num_spaces = max - flen;
				while(num_spaces){
					write(1, " " , 1);
					num_spaces--;
				}
			}
			write(1 , "    ", set);
			write(1, file2[a], max);
			printf("\n");
			if(i == j && a != b){
			//insert
				p2 = a + 1;
				while(p2 <= b){
					write(1, str, max);
					write(1, " >  ", set);
					write(1, file2[p2], max);
					printf("\n");
					p2++;
				}
			}
			else if(i != j && a == b){
			//delete
				p1 = i + 1;
				while(p1 <= j){
					flen = mystrlent(file1[p1]);
					if(flen > max)
						write(1, file1[p1], max);
					else{
						write(1, file1[p1], flen);
						num_spaces = max - flen;
						while(num_spaces){
							write(1, " " , 1);
							num_spaces--;
						}
					}
					write(1, " <  ", set);
					printf("\n");
					p1++;
				}
			}
			else if(i != j && a!= b){
			//change
				if(j - i > b - a){
				//change and delete
					p1 = i + 1;
					p2 = a + 1;
					while(p2 <=b){
						flen = mystrlent(file1[p1]);
						if(flen > max)
							write(1, file1[p1], max);
						else{
							write(1, file1[p1], flen);
							num_spaces = max - flen;
							while(num_spaces){
								write(1, " " , 1);
								num_spaces--;
							}
						}
						write(1, " |  ", set);
						write(1, file2[p2], max);
						printf("\n");
						p2++;
						p1++;
					}
					while(p1 <= j ){
						flen = mystrlent(file1[p1]);
						if(flen > max)
							write(1, file1[p1], max);
						else{
							write(1, file1[p1], flen);
							num_spaces = max - flen;
							while(num_spaces){
								write(1, " " , 1);
								num_spaces--;
							}
						}
						write(1, " <  ", set);
						printf("\n");
						p1++;
					}
					
				}
				if(j - i < b - a){
				//change and insert
					p1 = i + 1;
					p2 = a + 1;
					while(p1 <= j){
						flen = mystrlent(file1[p1]);
						if(flen > max)
							write(1, file1[p1], max);
						else{
							write(1, file1[p1], flen);
							num_spaces = max - flen;
							while(num_spaces){
								write(1, " " , 1);
								num_spaces--;
							}
						}
						write(1, " |  ", set);
						write(1, file2[p2], max);
						printf("\n"); 
						p1++;
						p2++;
					}
					while(p2 <= b){
						write(1, str, max);
						write(1, " >  ", set);
						write(1, file2[p2], max);
						printf("\n");
						p2++;
					}
				}
				else{
					p1 = i + 1;
					p2 = a + 1;
					while(p1 <= j){
						flen = mystrlent(file1[p1]);
						if(flen > max)
							write(1, file1[p1], max);
						else{
							write(1, file1[p1], flen);
							num_spaces = max - flen;
							while(num_spaces){
								write(1, " " , 1);
								num_spaces--;
							}
						}
						write(1, " |  ", set);
						write(1, file2[p2], max);
						printf("\n");
						p2++;
						p1++;
					}
				}
			}
			i = j + 1;
			a = b + 1;
		}
		/*first line of lcs will be printed here*/
		/*last line of lcs will be printed here*/
		flen = mystrlent(file1[f1_end]);
		if(flen > max)
			write(1, file1[f1_end], max);
		else{
			write(1, file1[f1_end], flen);
			num_spaces = max - flen;
			while(num_spaces){
				write(1, " " , 1);
				num_spaces--;
			}
		}
		write(1 , "    ", set);
		write(1, file2[f2_end], max);
		printf("\n");
		/*printing lines after the last line of lcs*/
		if(f1_end == len1 && f2_end != len2){
		//add
		//as there are some lines in file 2 after the last line of lcs the diff suggests to those lines of file2 into file 1
			i = f2_end + 1;
			while(i <= len2){
				write(1, str, max);
				write(1, " >  ", set);
				write(1, file2[i], max);
				printf("\n");
				i++;
			}
		}
		else if(f1_end != len1 && f2_end == len2){
		//delete
		//as there are still some lines in file 1 after the last line of lcs in file 2 , diff suggests to delete those lines of file 1
			i = f1_end + 1;
			while(i <= len1){
				flen = mystrlent(file1[i]);
				if(flen > max)
					write(1, file1[i], max);
				else{
					write(1, file1[i], flen);
					num_spaces = max - flen;
					while(num_spaces){
						write(1, " " , 1);
						num_spaces--;
					}
				}
				write(1, " <  ", set);
				printf("\n");
				i++;
			}
		}
		else if(f1_end != len1 && f2_end != len2){
		//change
			if(len1 - f1_end > len2 - f2_end){
				i = f1_end + 1;
				j = f2_end + 1;
				while(j <= len2){
					flen = mystrlent(file1[i]);
					if(flen > max)
						write(1, file1[i], max);
					else{
						write(1, file1[i], flen);
						num_spaces = max - flen;
						while(num_spaces){
							write(1, " " , 1);
							num_spaces--;
						}
					}
					write(1, " |  ", set);
					write(1, file2[j], max);
					printf("\n");
					i++;
					j++;
				}
				while(i <= len1){
					flen = mystrlent(file1[i]);
					if(flen > max)
						write(1, file1[i], max);
					else{
						write(1, file1[i], flen);
						num_spaces = max - flen;
						while(num_spaces){
							write(1, " " , 1);
							num_spaces--;
						}
					}
					write(1, " <  ", set);
					printf("\n");
					i++;
				}
			}
			else if(len1 - f1_end < len2 - f2_end){
				i = f1_end + 1;
				j = f2_end + 1;
				while(i <= len1){
					flen = mystrlent(file1[i]);
					if(flen > max)
						write(1, file1[i], max);
					else{
						write(1, file1[i], flen);
						num_spaces = max - flen;
						while(num_spaces){
							write(1, " " , 1);
							num_spaces--;
						}
					}
					write(1, " |  ", set);
					write(1, file2[j], max);
					printf("\n");
					i++;
					j++;
				}
				while(j <= len2){
					write(1, str, max);
					write(1, " >  ", set);
					write(1, file2[j], max);
					printf("\n");
					j++;
				}
			}
			else{
				i = f1_end + 1;
				j = f2_end + 1;
				while(i <= len1){
					flen = mystrlent(file1[i]);
					if(flen > max)
						write(1, file1[i], max);
					else{
						write(1, file1[i], flen);
						num_spaces = max - flen;
						while(num_spaces){
							write(1, " " , 1);
							num_spaces--;
						}
					}
					write(1, " |  ", set);
					write(1, file2[j], max);
					printf("\n");
					i++;
					j++;
				}
			}
		}
	}
}
/*returns the maximum among two numbers*/
int max(int num1,int num2){
	if(num1 == num2)
		return num1;
	else if(num1 > num2)
		return num1;
	else
		return num2;
}
void print_filediff(int fdk1, int fdk2, char *dir_name1, char *dir_name2, clist *cl){
	stack s, y;
	queue f1, f2, f1_copy, f2_copy, print_f1, print_f2;
	init(&s);
	init(&y);
	qinit(&f1);
	qinit(&f2);
	int option;
	int i, j, m = 1, n, k = 1, ret, deq_result, result;
	int len1, len2;//length of files
	char file1[1000][2000], file2[1000][2000];//2D array corresponding to files
	char string1[1000], string2[1000], lcs[100][1000], ch;
	DIR *dir1, *dir2;
	int len, x = 0, fd1_pass, fd2_pass, flag_1 = 0, flag_2 = 0;
	char *filename1, *filename2, *file1_pass, *file2_pass;
	int fd1, fd2, count1 = 0, count2 = 0;
	struct dirent *fdir1;//pointer for entry
	struct dirent *fdir2;//pointer for entry
	dir1 = fdopendir(fdk1);
	dir2 = fdopendir(fdk2);
	while((fdir1 = readdir(dir1)) != NULL){
		if(fdir1->d_type == 4 && strcmp(fdir1->d_name, ".") != 0 && strcmp(fdir1->d_name, "..") != 0){
			//it is a folder
			cinsert(cl, fdir1->d_name, fdir1->d_name, 4, 1, 0);
		}
		else if(fdir1->d_type == 8){
			//it is a file
			cinsert(cl, fdir1->d_name, fdir1->d_name, 8, 1, 0);
		}
	}
	while((fdir2 = readdir(dir2)) != NULL){
		if(fdir2->d_type == 4 && strcmp(fdir2->d_name, ".") != 0 && strcmp(fdir2->d_name, "..") != 0){
			//it is a folder
			cinsert(cl, fdir2->d_name, fdir2->d_name, 4, 2, 0);
		}
		else if(fdir2->d_type == 8){
			//it is a file
			cinsert(cl, fdir2->d_name, fdir2->d_name, 8, 2, 0);
		}
	}
	if(clen(cl) == 0)
		exit(-1);
	csort(cl);
	len = clen(cl);
	while(x < len){
		if(checkdir(cl, x) == 1)
			count1++;
		else if(checkdir(cl, x) == 2)
			count2++;
		x++;
	}
	x = 0;
	if(count1 == 0 && count2 != 0){
		while(x < len){
			printf("Only in %s: ", dir_name2);
			printfilename(cl, x);
			x++;
		}
		exit(-1);
	}
	else if(count1 != 0 && count2 == 0){
		while(x < len){
			printf("Only in %s: ", dir_name1);
			printfilename(cl, x);
			x++;
		}
		exit(-1);
	}
	while(x < len){
		if(x == len - 1){
			if(checkdir(cl, x) == 1){
				printf("Only in %s: ", dir_name1);
				printfilename(cl, x);
			}
			else if(checkdir(cl, x) == 2){
				printf("Only in %s: ", dir_name2);
				printfilename(cl, x);
			}
			break;
		}
		if(check(cl, x) == 0){
			//file is not common
			if(checkdir(cl, x) == 1){
				printf("Only in %s: ", dir_name1);
				printfilename(cl, x);
			}
			else if(checkdir(cl, x) == 2){
				printf("Only in %s: ", dir_name2);
				printfilename(cl, x);
			}
		}
		else if( check(cl,x) == 1 && get_type(cl, x) == 4 && flag_r != 1){
			//folder is common
			if(checkdir(cl, x) == 1){
				filename1 = getfile(cl, x);
				filename2 = getfile(cl, x + 1);
			}
			else if(checkdir(cl, x) == 2){
				filename2 = getfile(cl, x);
				filename1 = getfile(cl, x + 1);
			}
			printf("Common subdirectories: %s/%s and %s/%s\n", dir_name1, filename1, dir_name2, filename2);
			x++;
		}
	/*	else if(check(cl, x) == 1 && get_type(cl, x) == 4 && flag_r == 1){
			//folder is common and -r option is selected
			if(checkdir(cl, x) == 1){
				file1_pass = strcat(dir_name1, "/");
				file2_pass = strcat(dir_name2, "/");
				filename1 = getfile(cl, x);
				filename2 = getfile(cl, x + 1);
			}
			else if(checkdir(cl, x) == 2){
				file1_pass = strcat(dir_name1, "/");
				file2_pass = strcat(dir_name2, "/");
				filename2 = getfile(cl, x);
				filename1 = getfile(cl, x + 1);
			}
			file1_pass = strcat(dir_name1, filename1);
			file2_pass = strcat(dir_name2, filename2);
			fd1_pass = open(file1_pass, O_RDONLY);
			fd2_pass = open(file2_pass, O_RDONLY);
			dir_name1 =  rev_strstr(dir_name1, '/');
			dir_name2 =  rev_strstr(dir_name2, '/');
			if(fd1_pass == -1){
				perror("open failed");
			}
			if(fd2_pass == -1){
				perror("open failed");
			}
			clist cl1;
			cinit(&cl1);
			print_filediff(fd1_pass, fd2_pass, file1_pass, file2_pass, &cl1);
		}*/
		else if(check(cl, x) == 1 && get_type(cl, x) == 8){
			//file is common
			if(checkdir(cl, x) == 1){
				filename1 = getfile(cl, x);
				filename2 = getfile(cl, x + 1);
			}
			else{
				filename2 = getfile(cl, x);
				filename1 = getfile(cl, x + 1);
			}
			dir_name1 = strcat(dir_name1, "/");
			filename1 = strcat(dir_name1, filename1);
			dir_name2 = strcat(dir_name2, "/");
			filename2 = strcat(dir_name2, filename2);
			fd1 = open(filename1, O_RDONLY);
			fd2 = open(filename2, O_RDONLY);
			dir_name1 =  rev_strstr(dir_name1, '/');
			dir_name2 =  rev_strstr(dir_name2, '/');
			printf("diff %s %s\n", filename1, filename2);
			if(fd1 == -1){
				printf("open failed\n");
			}
			if(fd2 == -1){
				printf("open failed\n");
			}
			//reading lines from first file into a 2d character array
			len1 = readline(file1, fd1);
			//reading lines from second file into a 2d character array
			len2 = readline(file2, fd2);
			if(flag_t == 1)
				changefile(file1, file2, len1, len2);
			//finding largest common subsequence
			int arr[len1 + 1][len2 + 1];
			/*setting all elements of array as zero*/
			for(i = 0; i <= len1 ; i++)
				for(j = 0; j <= len2 ; j++)
					arr[i][j] = 0;

			m = 1;
			for(i = 1; i <= len1; i++){
				n = 1;
				for(j = 1; j <= len2 ; j++){
					//comparing lines
					if(mystrcmp(file1[m], file2[n]) == 0){
						//match found
						arr[i][j] = arr[i-1][j-1] + 1;
					}
					else
						arr[i][j] = max(arr[i-1][j], arr[i][j-1]);
					n++;
				}
				m++;
			}
			k = 0;
			m = len1;
			n = len2;
			//if there is no longest common subsequence
			if(arr[m][n] == 0){
				flag_p = 1;
				if(flag_c == 1){
					print_outputc(&f1, &f2, file1, file2, len1, len2, filename1, filename2);
				}
				else if(flag_y == 1){
					print_outputy(&f1, &f2, file1, file2, len1, len2);
				}
				else
					print_output(&f1, &f2, file1, file2, len1, len2);
			}
			else{
				while(arr[m][n] != 0){
					if(arr[m][n] != max(arr[m-1][n], arr[m][n-1])){
						//putting the lcs in a string
						strcpy(lcs[k++], file2[m - 1]);
						push(&s, n);
						push(&y, m);
						m = m - 1;
						n = n - 1;
					}
					else{
						if(arr[m][n] == arr[m][n - 1]){
							n = n - 1;
						}
						else
							m = m - 1;
					}
				}
				//the line numbers corresponding to longest common subsequence is pushed in the stack
				//on popping the numbers and enqueueing them, the numbers are in proper order in the queue
				while(!(isempty(&s))){
					i = pop(&s);
					enq(&f2, i);
				}
				while(!(isempty(&y))){
					i = pop(&y);
					enq(&f1, i);
				}
				if(flag_c == 1){
					print_outputc(&f1, &f2, file1, file2, len1, len2, filename1, filename2);
				}
				else if(flag_y == 1){
					print_outputy(&f1, &f2, file1, file2, len1, len2);
				}
				else
					print_output(&f1, &f2, file1, file2, len1, len2);
			}
			x++;
		}
		x++;
	}
	close(fd1);
	close(fd2);
	exit(-1);
}
int main(int argc, char *argv[]){
	stack s, y;
	clist cl;
	cinit(&cl);
	queue f1, f2, f1_copy, f2_copy, print_f1, print_f2;
	init(&s);
	init(&y);
	qinit(&f1);
	qinit(&f2);
	int option;
	int i, j, m = 1, n, k = 1, ret, deq_result, check, result;
	int len1, len2;//length of files
	char file1[1000][2000], file2[1000][2000];//2D array corresponding to files
	char string1[1000], string2[1000], lcs[100][1000], ch, filename1[100], filename2[100];
	while((option = getopt(argc, argv, "ibwytcr")) != -1){
		switch(option){
			case 'i':
				flag_i = 1;
				break;
			case 'b':
				flag_b = 1;
				break;
			case 'w':
				flag_w = 1;
				break;
			case 'y':
				flag_y = 1;
				break;
			case 't':
				flag_t = 1;
				break;
			case 'c':
				flag_c = 1;
				break;
			case 'r':
				flag_r = 1;
				break;		
			default: 
				printf("wrong option\n");
				break;
		}
	}
	if(flag_y == 1 && flag_c == 1){
		printf("conflicting output style options\n");
		exit(-1);
	}
	int fd1, fd2;
	if(argv[optind] == NULL){
		printf("missing operand\n");
		exit(-1);
	}
	else
		fd1 = open(argv[optind],O_RDONLY,S_IWUSR|S_IRUSR);
	strcpy(filename1, argv[optind]);
	optind++;
	if(argv[optind] == NULL){
		printf("missing operand after '%s'\n", argv[optind - 1]);
		exit(-1);
	}
	else
		fd2 = open(argv[optind],O_RDONLY,S_IWUSR|S_IRUSR);
	strcpy(filename2, argv[optind]);\
	if(fd1 == -1){
		perror("open failed");
		return errno;
	}
	if(fd2 == -1){
		perror("open failed");
		return errno;
	}
	//this function checks if both the arguments are directories or not
	check_file_directory(fd1, fd2);
	if(flag1_dir == 1 && flag2_dir == 1){
		//both the arguements are directories
		print_filediff(fd1, fd2, filename1, filename2, &cl);
	}
	else if(flag1_dir != 1 && flag2_dir != 1){
		//both the arguements are files
		//reading lines from first file into a 2d character array
		len1 = readline(file1, fd1);
		//reading lines from second file into a 2d character array
		len2 = readline(file2, fd2);
		//exiting if both the files are same
		check = optind - 1;
		if(strcmp(argv[optind],argv[check]) == 0){
			//in -y the files are displayed side by side if both the filenames are same
			if(flag_y == 1){
				flag_s = 1;
				if(flag_t == 1)
					changefile(file1, file2, len1, len2);
				print_outputy(&f1, &f2, file1, file2, len1, len2);
			}
			else
				exit(-1);
		}
		if(flag_t == 1)
			changefile(file1, file2, len1, len2);
		//finding largest common subsequence
		int arr[len1 + 1][len2 + 1];
		/*setting all elements of array as zero*/
		for(i = 0; i <= len1 ; i++)
			for(j = 0; j <= len2 ; j++)
				arr[i][j] = 0;

		m = 1;
		for(i = 1; i <= len1; i++){
			n = 1;
			for(j = 1; j <= len2 ; j++){
				//comparing lines
				if(mystrcmp(file1[m], file2[n]) == 0){
					//match found
					arr[i][j] = arr[i-1][j-1] + 1;
				}
				else
					arr[i][j] = max(arr[i-1][j], arr[i][j-1]);
				n++;
			}
			m++;
		}
		k = 0;
		m = len1;
		n = len2;
		//if there is no longest common subsequence
		if(arr[m][n] == 0){
			flag_p = 1;
			if(flag_c == 1){
				print_outputc(&f1, &f2, file1, file2, len1, len2, filename1, filename2);
			}
			else if(flag_y == 1){
				print_outputy(&f1, &f2, file1, file2, len1, len2);
			}
			else
				print_output(&f1, &f2, file1, file2, len1, len2);
		}
		else{
			while(arr[m][n] != 0){
				if(arr[m][n] != max(arr[m-1][n], arr[m][n-1])){
					//putting the lcs in a string
					strcpy(lcs[k++], file2[m - 1]);
					push(&s, n);
					push(&y, m);
					m = m - 1;
					n = n - 1;
				}
				else{
					if(arr[m][n] == arr[m][n - 1]){
						n = n - 1;
					}
					else
						m = m - 1;
				}
			}
			//the line numbers corresponding to longest common subsequence is pushed in the stack
			//on popping the numbers and enqueueing them, the numbers are in proper order in the queue
			while(!(isempty(&s))){
				i = pop(&s);
				enq(&f2, i);
			}
			while(!(isempty(&y))){
				i = pop(&y);
				enq(&f1, i);
			}
			if(flag_c == 1){
				print_outputc(&f1, &f2, file1, file2, len1, len2, filename1, filename2);
			}
			else if(flag_y == 1){
				print_outputy(&f1, &f2, file1, file2, len1, len2);
			}
			else
				print_output(&f1, &f2, file1, file2, len1, len2);
		}
	}
	close(fd1);
	close(fd2);
}
