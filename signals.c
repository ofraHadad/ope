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
pid_t pid_child = -1;
pid_t pid_child_prev = -1;
pid_t pid[5];

void sigCathcher(int sig)
{
	 printf("PID %d caught once\n",getpid());
	 if(pid_child_prev > 0){
		kill(pid_child_prev,SIGINT);
	 }
	 exit(1);
}

int main() 
{ 
signal(SIGINT,sigCathcher);

//	int pid_child = -1;
	 for(int i=0;i<5;i++) // loop will run n times (n=5) 
    { 
		 
	//	printf("pid_child %d \n", pid_child);
		pid_child = fork();
		pid[i]= pid_child;
        if(pid_child == 0) 
        { 
			
			//printf("pid_child_prev %d \n", pid_child_prev);
			//pid[i]= getpid();
            printf("PID %d ready\n",getpid());
			
			//sleep(5);
			while(1);
			//exit(0); 
		} 
		else {
			
			pid_child_prev = pid_child;
			
			
		}
    } 
	sleep(2);
	//printf("prev %d\n",pid_child_prev);
	kill(pid_child_prev,SIGINT);
	sleep(3);
    for(int i=0;i<5;i++){ // loop will run n times (n=5) 
		wait(NULL); 
		printf("Process %d is dead\n", pid[i]);
	}
} 
