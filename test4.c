#include<stdio.h>
#include<signal.h>
#include<sys/types.h>
#include<sys/wait.h>
#include <stdlib.h>
#include <unistd.h>
#include<string.h>


int main()
{
	pid_t ch1_id,ch2_id;
    int fd_rw[2],n,pid_ch1,pid_ch2; 
    char chr1[]="The first child process's pid is  ",chr2[]="The second child process's pid is  ";
	char chr;
	
	int len1=strlen(chr1),len2=strlen(chr2);
    pipe(fd_rw);         // 创建一个无名管道fd包含2个文件描述符的数组，fd[0]用于读，fd[1]用于写若成功返回0,否则返回-1
	                     //一般某个进程用于读，另一个进程用于写，用于读的进程需要close(fd_rw[1]),用于写的进程需要close(fd_rw[0]);
	
    ch1_id=fork();
	
	if(ch1_id==0)          //子进程1----------------------------------------------------------------------
    {
		int i;
		close(fd_rw[0]);
		lockf(fd_rw[1],1,0);
		
   		pid_ch1=getpid();
		int a=pid_ch1;
		char c[5]="0000";
		for(i=0;i<4;i++)
		{
			c[3-i]+=a%10;
			a=a/10;
		}
		
        write(fd_rw[1],chr1,strlen(chr1));
		write(fd_rw[1],c,strlen(c)+1);
        printf("I am first child, my pid=%d\nI have send message to parent:   %s%s\n\n",getpid(),chr1,c);
				
		lockf(fd_rw[1],0,0);
		close(fd_rw[1]);
        exit(1);
    }
	else 
	{ 
		ch2_id=fork();
		if(ch2_id==0)          //子进程2--------------------------------------------------
        {
			int i;
			close(fd_rw[0]);
			lockf(fd_rw[1],1,0);   
			
    		pid_ch2=getpid();
			int a=pid_ch2;
	     	char c[5]="0000";
    		for(i=0;i<4;i++)
	    	{
	     		c[3-i]+=a%10;
	     		a=a/10;
	    	}			
			
            write(fd_rw[1],chr2,strlen(chr2));
			write(fd_rw[1],c,strlen(c)+1);
            printf("I am second child, my pid=%d\nI have send message to parent:   %s%s\n\n",getpid(),chr2,c);
				
			lockf(fd_rw[1],0,0);
            close(fd_rw[1]);
            exit(2);
        }
		
		else                    //父进程------------------------------------------------------------------
		{
			int i,len;
			pid_t wait_pid;
			wait_pid=wait(NULL);
			
	/*		printf("%d  %d  %d\n",wait_pid,pid_ch1,pid_ch2);
			
			if(wait_pid==pid_ch1)
			{
				len=len1+4;
			}
			else
			{
				len=len2+4;
			}
	
*/          printf("I am parent ,pid=%d\n",getpid());
			for(i=0;i<50;i++)
			{
				
				read(fd_rw[0],&chr,1);
				if(chr=='\0')
				{
					break;
				}
				printf("%c",chr);
			}
			printf("\n\n");
			
			wait_pid=wait(NULL);
//			printf("%d\n",sta);
			
/*			if(wait_pid==pid_ch1)
			{
				len=len1+4;
			}
			else
			{
				len=len2+4;
			}
			*/
			printf("I am parent ,pid=%d\n",getpid());
			for(i=0;i<50;i++)
			{
				
				read(fd_rw[0],&chr,1);
				if(chr=='\0')
				{
					break;
				}
				printf("%c",chr);
			}
            printf("\n\n");			
		}
    }
	exit(0);

    return 0;
}
