#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<semaphore.h>
#include<errno.h>
#include<pthread.h>
#include<sys/stat.h>
#include<fcntl.h>

#define N 32

int buffer[N];

void thread_producer(void);
void thread_consumer(void);

int main()
{
    pthread_t id_consumer,id_producer;
    int i,ret_con,ret_pro;
    memset(buffer,0,N*sizeof(int));

    ret_pro=pthread_create(&id_producer,NULL,(void *)thread_producer,NULL);
    ret_con=pthread_create(&id_consumer,NULL,(void *)thread_consumer,NULL);
    

    if(ret_con!=0 || ret_pro!=0)
    {
        printf("Create thread error\n");
        exit(1);
    }
  /*  for(i=0;i<4;i++)
    {
        printf("this is main process\n");
    }*/
    pthread_join(id_producer,NULL);
    pthread_join(id_consumer,NULL);

    return 0;
}

void thread_producer(void)
{
//   printf("p\n\n");
   sem_t *full,*empty,*mutex;
   int i=0;

//   empty=sem_open("em",O_CREAT,0644,32);
//   mutex=sem_open("mu",O_CREAT,0644,1);
//   full=sem_open("fu",O_CREAT,0644,0);

   full=sem_open("fu",0);
   empty=sem_open("em",32);
   mutex=sem_open("mu",1);

   if(full<0 || empty<0 || mutex<0)
   {
       printf("producer process create fault!\n");
   }

   while(1)
   {
       
       sem_wait(empty);
       sem_wait(mutex);
       printf("producing......\n");
       sleep(5);
       while(1)
       {
           if(i<N && buffer[i]==0)
           {
               buffer[i++]=(rand()%100)+1;
               break;
           }
           i++;
           if(N<=i)
               i=0;
           
       }
       printf("producer : %d\n\n",buffer[i-1]);
       sem_post(mutex);
       sem_post(full);
   }
}


void thread_consumer(void)
{
//   printf("c\n\n");
   sem_t *full,*empty,*mutex;
   int i=0,output;

 //  empty=sem_open("em",O_CREAT,0644,32);
 //  full=sem_open("fu",O_CREAT,0644,0);

   empty=sem_open("em",O_CREAT,0644,N);
   mutex=sem_open("mu",O_CREAT,0644,1);
   full=sem_open("fu",O_CREAT,0644,0);

//   full=sem_open("fu",0);
//   empty=sem_open("em",32); 
//   mutex=sem_open("mu",1);

   if(full<0 || empty<0 || mutex<0)
   {
       printf("consumer process create fault!\n");
   }

   while(1)
   {
      // printf("c2\n\n");
       sem_wait(full);
       sem_wait(mutex);
       printf("consuming......\n");
       sleep(5);
       while(1)
       {
           if(i<N && buffer[i]!=0)
           {
                output=buffer[i];
                buffer[i]=0;
                break;
           }
            i++;
            if(N<=i)
               i=0;
        }
        printf("consumer : %d\n\n",output) ;

       sem_post(mutex);
       sem_post(empty);
   }
}
