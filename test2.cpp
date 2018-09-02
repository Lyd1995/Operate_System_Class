#include<stdio.h>
#include<stdlib.h>
#include<string>
#include<iostream>
#include<time.h>
#include<queue>

using namespace std;

#define maxn 5
#define slice 3

typedef struct Process
{
	string name;     //程序的名字
	int priority;      //优先级
	int arrive_time;     //到达时间
	int need_time;       //需要运行的时间
	int run_time;        //已经运行的时间
	int tag;           //tag=1表示处于运行态,tag=0表示处于就绪态
	Process() {}
	Process(int a)
	{
		string temp = "Process ";
		name = temp + to_string(a);
		priority = rand() % 100;
		arrive_time = rand() % 20;
		need_time = rand() % 100;
		run_time = 0;
		tag = 0;
	}
}Process;

typedef struct cmp
{
	bool operator() (Process a,Process b)
	{
		if (a.arrive_time != b.arrive_time)return a.arrive_time > b.arrive_time;
		else return a.priority < b.priority;
	}
};

Process p[maxn];
priority_queue<Process, vector<Process>, cmp> pq;
queue<Process> run;
int now_time = 0;

void init()
{
	for (int i = 0; i < maxn; i++)
	{
		p[i] = Process(i);
		pq.push(p[i]);
	}
}

void print()
{
	printf("以下是随机生成的一些进程信息:\n");
	for (int i = 0; i < maxn; i++) {
		cout << p[i].name << endl;
		printf("priority=%d\narrive_time=%d\nneed_time=%d\nrun_time=%d\n\n",p[i].priority,p[i].arrive_time,p[i].need_time,p[i].run_time);
	}
}

void System_Run()
{
	now_time = pq.top().arrive_time;
	int top_time = now_time;
	while (!pq.empty() && pq.top().arrive_time == now_time)
	{
		run.push(pq.top());
		pq.pop();
	}
	if (!pq.empty())top_time = pq.top().arrive_time;
	while (!run.empty())
	{
		bool tag;
		Process temp = run.front();
		run.pop();
		if (temp.need_time <= slice) {
			temp.run_time += temp.need_time;
			printf("%s运行%d个时间,已经运行%d个时间，%s运行结束\n\n", temp.name.c_str(), temp.need_time, temp.run_time, temp.name.c_str());
			now_time += temp.need_time;
			temp.need_time = 0;
			tag = false;
		}
		else {
			temp.run_time += slice;
			printf("%s运行3个时间，已经运行%d个时间，运行未结束，加入等待队列\n\n",temp.name.c_str(),temp.run_time);
			temp.need_time -= slice;
			now_time += slice;
			tag = true;
		}
		while (!pq.empty() && pq.top().arrive_time <= now_time) {
			run.push(pq.top());
			pq.pop();
		}
		if (tag)run.push(temp);
	}
}

int main()
{
	srand(time(0));
	printf("\t\t使用时间片加优先级的调度算法调度程序\n");
	init();
	print();
	System_Run();
	system("pause");
	return 0;
}
