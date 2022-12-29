#ifndef _POOL_H_
#define _POOL_H_
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

struct job  //任务节点  （队列的节点）
{
	void*(*func)(void *arg);  //函数指针，用于存放任务
	void *arg;               //用来给任务函数传参使用
	struct job *next;      //队列节点的指针域
};

struct pthreadpool
{
	int m_threadNum;  //已开启的线程的数量
	pthread_t *m_pthreadIds; //保存线程池中线程的ID

	struct job *head;  //任务队列的头
	struct job *tail;  //任务队列的尾
	int m_QueueMaNum;  //任务队列的最大数
	int m_QueueCurNum;  //任务队列已经有了多少个任务

	pthread_mutex_t mutex;

	pthread_cond_t m_queueEmpty;   //任务队列为空的条件
	pthread_cond_t m_queueNotEmpty; //任务队列不为空的条件
	pthread_cond_t m_queueNotFull;  //任务队列不为满的条件
	int pool_close;
};

void *threadpool_Run(void *arg);
struct pthreadpool *ThreadPool_init(int threadNum, int queueMaxNum);
void thread_add_job(struct pthreadpool *pool, void *(*func)(void *), void *arg);
void thread_destroy(struct pthreadpool *pool);








#endif