TITLE OF PROJECT : DIFF AND PATCH
NAME : 	OMKAR RAJENDRA THORAT
MIS : 111708041
BRANCH : INFORMATION TECHNOLOGY
Please note that there are two executable files : project1 corresponding to diff and project2 corresponding to patch
DIFF:
I have done diff of two files , in which the options -y -c -t -i -b -w have been completed
I have also done diff of two directories in which the options -y -c -t -i -b -w have been completed, I have not done the -r option
At the start of the program I have used getopt to set the flags corresponding to options.
Then I check if the two files are same or not, I terminate the program if the two files are same and -y option is not selected.
In case the -y option is selected I do not terminate the program
I have stored the two files in a 2D array of characters. Then I have found the longest common subsequence of these two files using a matrix,following the algorithm for longest common subsequence I have pushed the line number(based on the string compare function that I wrote, depending upon the options selected the flags are set accordingly and mystrcmp gives the appropriate output).
On poping, the line numbers come in their proper order.
The line numbers are arranged in two queues(of the two files).
Depending on the flags set I call different functions to print the output.
In Diff of two directories I first identify if the argument is a directory , then I enter the files and subdirectories into a clist. I sort the Clist and then check if two consecutive elements of clist has same name and then I execute it's diff for other files/subdirectories I print the o/p
PATCH :
In patch I take two arguments one first being the file1 and second one containg the o/p of diff of file1 with file2(also named as patchfile)
I use file 1 to write lines of file1 and patchfile to write lines of file2 into a third file,I then remove the first file and rename the third file as the first file thereby patching file1.I have done patching using the diff o/p as well as context diff o/p
