#include "pool.h"
// struct job  //任务节点  （队列的节点）
// {
// 	void*(*func)(void *arg);  //函数指针，用于存放任务
// 	void *arg;               //用来给任务函数传参使用
// 	struct job *next;      //队列节点的指针域
// };

// struct pthreadpool
// {
// 	int m_threadNum;  //已开启的线程的数量
// 	pthread_t *m_pthreadIds; //保存线程池中线程的ID

// 	struct job *head;  //任务队列的头
// 	struct job *tail;  //任务队列的尾
// 	int m_QueueMaNum;  //任务队列的最大数
// 	int m_QueueCurNum;  //任务队列已经有了多少个任务

// 	pthread_mutex_t mutex;

// 	pthread_cond_t m_queueEmpty;   //任务队列为空的条件
// 	pthread_cond_t m_queueNotEmpty; //任务队列不为空的条件
// 	pthread_cond_t m_queueNotFull;  //任务队列不为满的条件
// 	int pool_close;
// };

void *threadpool_Run(void *arg)
{
	struct pthreadpool *pool = (struct pthreadpool *)arg;
	struct job *pjob = NULL;   //定义一个任务节点，用来获取任务
	//sleep(2);
	while (1)
	{
		pthread_mutex_lock(&pool->mutex);  //所有的线程运行之前，先尝试加锁，为了访问临界资源安全
		while (pool->m_QueueCurNum == 0)   //当任务数量为0的时候，线程挂起等待
		{
			printf("pool->m_QueueCurNum =%d\n", pool->m_QueueCurNum);
			pthread_cond_wait(&(pool->m_queueNotEmpty), &(pool->mutex)); 
			if (pool->pool_close == 1)
			{
				pthread_exit(NULL);
			}
		}
	//向下执行，说明任务队列中有任务
		pjob = pool->head; //将任务队列队头的任务赋值给定义的局部任务节点
		pool->m_QueueCurNum--;  //从任务队列取出任务后，任务队列任务数减一
		if (pool->m_QueueCurNum != pool->m_QueueMaNum)
		{
			pthread_cond_broadcast(&(pool->m_queueNotFull));  //当任务队列不为满的时候，唤醒条件变量m_queueNotFull，唤醒主线程去添加任务
		}
		if (pool->m_QueueCurNum == 0)   //如果取出任务之后，任务队列的任务数为0，唤醒条件变量m_queueEmpty
		{
			pool->head = pool->tail = NULL;
			pthread_cond_broadcast(&pool->m_queueEmpty);
		}
		else
		{
			pool->head = pool->head->next;  //将head指针指向下一个任务节点
		}
		pthread_mutex_unlock(&pool->mutex);
		pjob->func(pjob->arg);   //开始执行任务队列中刚取出的任务
		free(pjob);   //任务执行完毕后，需要将任务队列执行完毕的节点释放
		pjob = NULL;
	}
}
struct pthreadpool *ThreadPool_init(int threadNum, int queueMaxNum)
{
	struct pthreadpool *pool = (struct pthreadpool *)malloc(sizeof(struct pthreadpool));

	pool->m_QueueMaNum = queueMaxNum;  //初始化任务最大值
	pool->m_QueueCurNum = 0;   //当前任务数为0
	pool->head = NULL;  //初始化任务队列
	pool->tail = NULL;
	pool->pool_close = 0;
	pthread_mutex_init(&(pool->mutex), NULL);   //初始化互斥锁
	pthread_cond_init(&(pool->m_queueEmpty), NULL);  //初始化条件变量
	pthread_cond_init(&(pool->m_queueNotEmpty), NULL);//初始化条件变量
	pthread_cond_init(&(pool->m_queueNotFull), NULL);//初始化条件变量

	pool->m_threadNum = threadNum;   //线程池中线程的数量
	pool->m_pthreadIds = (pthread_t *)malloc(sizeof(pthread_t) *threadNum);  //给线程号开辟空间（空间大小为：单个线程号大小*线程的个数）

	for (int i = 0; i < pool->m_threadNum; i++)    //循环创建m_threadNum个线程
	{
		pthread_create(&pool->m_pthreadIds[i], NULL, threadpool_Run, (void *)pool);
	}
	return pool;
}
void thread_add_job(struct pthreadpool *pool, void *(*func)(void *), void *arg)
{
	pthread_mutex_lock(&pool->mutex);
	while (pool->m_QueueCurNum == pool->m_QueueMaNum)   //当当前任务数等于任务队列最大任务数的时候，主线程挂起等待
	{
		printf("任务队列已满,等待线程赶紧做完任务。。。\n");
		pthread_cond_wait((&pool->m_queueNotFull), &pool->mutex);
	}

	struct job *pjob = (struct job *)malloc(sizeof(struct job));  //申请一个新的任务节点
  	pjob->func = func;     //初始化任务节点
	pjob->arg = arg;
	//pjob->func(pjob->arg);
	pjob->next = NULL;
	//对于无头链队列，队列是否为空插入方式不一样，需要分情况讨论
	if (pool->head == NULL)  //任务队列为空 ，插入任务节点
	{
		printf("pool->head == NULL\n");
		pool->head = pool->tail = pjob;
		pthread_cond_broadcast(&(pool->m_queueNotEmpty));
	}
	else   //任务队列不为空，插入任务节点
	{
		pool->tail->next = pjob;
		pool->tail = pjob;
	}

	pool->m_QueueCurNum++;
	pthread_mutex_unlock(&pool->mutex);
}
// void *work(void *arg)
// {
// 	char *p = (char *)arg;
// 	printf("hello world!,%s\n", p);
// 	printf("welcome to jsetc!,%s\n", p);
// 	sleep(1);
// }

// void *work2(void *arg)
// {
// 	char *p = (char *)arg;
// 	printf("你好南京!,%s\n", p);
// 	printf("welcome to nanjing!,%s\n", p);
// 	sleep(1);
// }
//等待任务队列为空，再去销毁线程资源
void thread_destroy(struct pthreadpool *pool)
{
	pthread_mutex_lock(&pool->mutex);
	while (pool->m_QueueCurNum != 0)
	{
		printf("阻塞等待销毁线程中...！\n");
		pthread_cond_wait(&pool->m_queueEmpty, &pool->mutex);
	}
	printf("任务结束，线程%ld被销毁\n", pthread_self());
	pthread_mutex_unlock(&pool->mutex);
	pthread_cond_broadcast(&pool->m_queueNotEmpty);
	pthread_cond_broadcast(&pool->m_queueNotFull);
	pool->pool_close = 1;
	for (int i = 0; i < pool->m_threadNum; i++)
	{
		//pthread_cancel(pool->m_pthreadIds[i]);
		pthread_join(pool->m_pthreadIds[i], NULL);
	}
	pthread_mutex_destroy(&pool->mutex);
	pthread_cond_destroy(&pool->m_queueNotEmpty);
	pthread_cond_destroy(&pool->m_queueNotFull);
	pthread_cond_destroy(&pool->m_queueEmpty);
	free(pool->m_pthreadIds);
	struct job *tmp;
	while (pool->head != NULL) //以防还有未执行的任务，因为此时所有线程已销毁，所以任务必须销毁掉
	{
		tmp = pool->head;
		pool->head = tmp->next;
		free(tmp);
	}
	free(pool);
}
// int main(int argc, char const *argv[])
// {
// 	struct pthreadpool *pool = ThreadPool_init(5, 10);  //初始化线程池，线程池中线程的数量为5，任务最大数为10
// 	thread_add_job(pool, work, "1");
// 	thread_add_job(pool, work, "2");
// 	thread_add_job(pool, work, "3");
// 	thread_add_job(pool, work, "4");
// 	thread_add_job(pool, work2, "5");
// 	thread_add_job(pool, work2, "6");
// 	thread_add_job(pool, work, "7");
// 	thread_add_job(pool, work2, "8");
// 	thread_add_job(pool, work2, "9");

// 	thread_add_job(pool, work, "11");
// 	thread_add_job(pool, work, "22");
// 	thread_add_job(pool, work, "33");
// 	thread_add_job(pool, work, "44");
// 	thread_add_job(pool, work2, "55");
// 	thread_add_job(pool, work2, "66");
// 	thread_add_job(pool, work, "77");
// 	thread_add_job(pool, work2, "88");
// 	thread_add_job(pool, work2, "99");


// 	sleep(20);
// 	thread_destroy(pool);
// 	return 0;
// }
