#include <stdio.h>
#include <unistd.h>

int main ()   
{   
    pid_t fpid; //fpid表示fork函数返回的值    
    fpid=fork();   
    if (fpid < 0)   
        printf("error in fork!");   
    else if (fpid == 0) {  
        printf("i am the child process, my process id is %d\n",getpid());     
    }  
    else {  
        printf("i am the parent process, my process id is %d\n",getpid());    
    }  
    return 0;  
}  




/**********************************看看下面创建了几个子进程*****************************************/


#include <stdio.h>
#include <unistd.h>

int main(void)
{
    int i;
	
    for (i = 0; i < 2; i++) {
        fork();
    }

    printf("pid = %d, ppid = %d\n", getpid(), getppid());

    sleep(1);

    return 0;

}


//2+1=3
