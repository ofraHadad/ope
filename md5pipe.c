#include <string.h>

#include <stdio.h>

#include <stdlib.h>

#include <fcntl.h>

#include <sys/uio.h>

#include <sys/types.h>

#include <unistd.h>

#include <signal.h>

#include <sys/fcntl.h>

#include<sys/wait.h> 

// לשלוח סיגנל .. להוסיף בדיקה של 20? לא מקבל רווחים//////

	char str[33];
	int flag=1;
	void is32(int sig)
	{
		//printf("is32 start \n");
		if(	strlen(str)!=32)
		{
			printf("failed, len diffrent then 32. len(%ld)\n", strlen(str));
			exit(1);
		} else {
			flag=0;
			//printf("The string len is 32\n");
		}
		
	}
int main() //I got help from the following link : https://www.geeksforgeeks.org/c-program-demonstrate-fork-and-pipe/
{ 
	// We use two pipes 
	// First pipe to send input string from parent 
	// Second pipe to send concatenated string from child 
	
	int fd1[2]; // Used to store two ends of first pipe 
	int fd2[2]; // Used to store two ends of second pipe 


	 
	pid_t p; 
	
	if (pipe(fd1)==-1)//returns -1 in failure
	{ 
		fprintf(stderr, "Pipe Failed" ); 
		return 1; 
	} 
	if (pipe(fd2)==-1) //returns -1 in failure
	{ 
		fprintf(stderr, "Pipe Failed" ); 
		return 1; 
	} 
	
	
	p = fork(); 

	if (p < 0) 
	{ 
		fprintf(stderr, "fork Failed" ); 
		return 1; 
	} 

	// Parent process 
	else if (p > 0) 
	{ 
		signal(SIGINT, is32); 
		//להוסיף בדיקה של 20 אותיות?
		printf("Parent: Please enter a string: \n");
		scanf("%s", str); 
		
		
		close(fd1[0]); // Close reading end of first pipe 
		
		// Write input string and close writing end of first 
		// pipe. 
		write(fd1[1], str, strlen(str)+1); 
		close(fd1[1]); 
	//	printf("Parent: send the string \n");
		// Wait for child to send a string 
		
		
		close(fd2[1]); // Close writing end of second pipe 
		
		// Read string from child, print it and close 
		// reading end. 
		read(fd2[0], str, 100); 
		while(flag);
		printf("coded string %s\n", str); 
		close(fd2[0]);
		kill(p,SIGKILL);
	} 

	// child process 
	else
	{ 
		close(fd1[1]); // Close writing end of first pipe 
		
		// Read a string using first pipe 
		char  concat_str[100]; 
		int len = read(fd1[0], concat_str, 100); 
		if(len == -1){
			printf("Parent failed to read from pipe");
			return 1;
		}
		
		
		//printf("Child: read string from pipe (%s)\n", concat_str);
		
		char c=concat_str[0]; 
	   	for (int i=0; i<32; i++) {
			int add=i+c;
			if(i+c>127)
			{
				add=c-i;
			}
			concat_str[i] = add; 
		}
		concat_str[32]= '\0';
		//printf("Child: After concat a string (%s)\n", concat_str);
		
		
		// Close both reading ends 
		close(fd1[0]);


		//close for reading		
		close(fd2[0]); 
		
		// Write concatenated string and close writing end 
		 
		write(fd2[1], concat_str, strlen(concat_str)+1); 
		close(fd2[1]); 
		
		pid_t pid = getppid();
		
       // printf("Child: sending to Parent SIGINT\n\n"); 
        kill(pid, SIGINT); 
		while(1);
		exit(0); 
	}

	
} 
