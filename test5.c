#include<stdio.h>
#include<unistd.h>
#include<queue>
#include<stack>
#include<time.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/wait.h>

using namespace std;
#define frame_num 20                //定义页面个数
#define total_instruction 12       //随机访问序列个数
#define min_block 7                //驻留集大小
#define N 10


typedef struct one_frame
{
	int page_no;              //页面号
	char flag;                //是否在内存
	long ti;                //最后一次使用时间
}one_frame;

struct cmp
{
	bool operator()(one_frame a,one_frame b)
	{
		if(a.ti == b.ti)
		{
			return a.page_no>b.page_no;
		}
		return a.ti>b.ti;
	}
};

int count_FIFO=0,count_LRU=0;
one_frame M_Frame[frame_num];
int Acess_Series1[total_instruction];    //子进程1的随机序列

queue<int> memory_page;     //FIFO队列
priority_queue<one_frame,vector<one_frame>,cmp> memory_LRU;  //LRU算法所使用的驻留集

void FIFO(int i);            //先进先出算法
void LRU(int i);             //最近最久未使用算法

int main()
{
	int i;
	for(int i=0;i<total_instruction;i++)
	{
		Acess_Series1[i]=rand()%frame_num;           //随机产生total_instruction个内存访问页面序列
	}
	
	for(int i=0;i<total_instruction;i++)
	{
		M_Frame[i].page_no=i;                       //初始化所有的页面并给页面编号，flag=0表示不在内存中
		M_Frame[i].flag=0;
		M_Frame[i].ti=0;
	}
	
	pid_t pid1,pid2; 
	
    pid1=fork();   
	
	if(pid1==0)   //子进程1
	{
		int j;
		for(j=0;j<N;j++)
		{
			FIFO(j);
		}
		sleep(1);
		printf("\n子进程1，pid=：%d   :先进先出算法：缺页次数：%d   缺页率：%f\n",getpid(),count_FIFO,(float)count_FIFO/N);
		//以上为先进先出算法的模拟，之后对所有页面的flag进行重置，准备模拟最近最久未使用算法
/*		for(j=0;j<frame_num;j++)
		{
			M_Frame[j].flag=0;
		}
		
		for(j=0;j<N;j++)
		{
			LRU(j);
			sleep(2);
		}
		printf("\n子进程1，pid=：%d   :最近最久未使用算法：缺页次数：%d   缺页率：%f\n\n",getpid(),count_LRU,(float)count_LRU/N);*/
		exit(0);
	}
	else
	{
		pid2=fork();
		if(pid2==0)//子进程2
		{
			int j;
/*    		for(j=0;j<N;j++)
    		{
	    		FIFO(j);
	    	}
	    	sleep(1);
			printf("\n子进程2，pid=：%d   :先进先出算法：缺页次数：%d   缺页率：%f\n",getpid(),count_FIFO,(float)count_FIFO/N);
		//以上为先进先出算法的模拟，之后对所有页面的flag进行重置，准备模拟最近最久未使用算法
	     	for(j=0;j<frame_num;j++)
	    	{
	    		M_Frame[j].flag=0;
	    	}*/
		
	    	for(j=0;j<N;j++)
	    	{
	    		LRU(j);
	    		sleep(2);
	    	}
			printf("\n子进程2，pid=：%d   :最近最久未使用算法：缺页次数：%d   缺页率：%f\n\n",getpid(),count_LRU,(float)count_LRU/N);
	    	exit(0);
		}
	}
	
	wait(NULL);
	wait(NULL);
	/*printf("\n\n\n");
	printf("先进先出算法：缺页次数：%d   缺页率：%d\n",count_FIFO,count_FIFO/80);
	printf("最近最久未使用算法：缺页次数：%d   缺页率：%d\n",count_LRU,count_LRU/80);*/
	return 0;
}

void FIFO(int i)
{
	if(M_Frame[Acess_Series1[i]].flag==0)  //不在内存中，准备调入
	{
		count_FIFO++;
		if(memory_page.size()==min_block)  //当前驻留集已经满了，需要将页面置换出去
		{
			int k;
			k=memory_page.front();
			memory_page.pop();
			memory_page.push(Acess_Series1[i]);
			printf("我是子进程1，发生缺页中断，并且内存已满需要置换，被置换的页面编号为：%d，进入内存的页面编号为：%d\n",k,Acess_Series1[i]);
			M_Frame[k].flag=0;
		}
		else
		{
			memory_page.push(Acess_Series1[i]);
			printf("我是子进程1，发生缺页中断，进入内存的页面编号为：%d\n",Acess_Series1[i]);
		}
		M_Frame[Acess_Series1[i]].flag=1;
	}
	else
	{
		printf("我是子进程1，所需页面在内存中，页面编号为：%d\n",Acess_Series1[i]);
	}
	return;
}

void LRU(int i)
{
	
	if(M_Frame[Acess_Series1[i]].flag==0)  //不在内存中，准备调入
	{
		count_LRU++;
		clock_t t=clock();
		M_Frame[Acess_Series1[i]].ti=t;
		
		if(memory_LRU.size()==min_block)  //当前驻留集已经满了，需要将页面置换出去
		{
			one_frame k;
			k=memory_LRU.top();
			memory_LRU.pop();
			memory_LRU.push(M_Frame[Acess_Series1[i]]);
			printf("我是子进程2，发生缺页中断，并且内存已满需要置换，被置换的页面编号为：%d，进入内存的页面编号为：%d\n",k.page_no,Acess_Series1[i]);
			k.flag=0;
		}
		else
		{
			memory_LRU.push(M_Frame[Acess_Series1[i]]);
			printf("我是子进程2，发生缺页中断，进入内存的页面编号为：%d\n",Acess_Series1[i]);
		}
		M_Frame[Acess_Series1[i]].flag=1;
	}
	else
	{
		printf("我是子进程2，所需页面在内存中，页面编号为：%d\n",Acess_Series1[i]);
	}
	
	return;
}
