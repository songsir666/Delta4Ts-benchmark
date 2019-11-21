/*
对locate.sh的C语言改写
1.根据out.list和x.gcov生成fault.list
2.对fault.list进行排序并计算effectiveness
*/

#include  <stdio.h>
#include <pthread.h>
#include <sys/time.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define MAX 8 
#define LINES 20000

#define MAX_TEST_CASE 20000

pthread_t thread[MAX]; //记录进程号
pthread_mutex_t mut;  //互斥锁
int linenum;	//源代码总行数
int failnum;	//失败用例数
int passnum;	//成功用例数
int casenum ;	//总用例数
int fail[LINES];	//失败时某行执行次数
int pass[LINES];	//成功时某行执行次数	
int size;		//每个线程要处理的gcov文件数
struct susp		//每行“可疑度”
{	
	int line;	//记录行数
	float p;	//这一行“可疑度”
};
struct susp ranked[LINES];

int *coverage_information_metric[MAX_TEST_CASE];

int test_result[MAX_TEST_CASE];


int read_test_case_result(){

	FILE * fp;
	if((fp=fopen("out.list","r")) != NULL)
	{
		int flag=0;
		for(int i=0;i<casenum;i++){
			fscanf(fp,"%d",&flag);
			fscanf(fp,"%d",&test_result[flag-1]);
			//printf("%d   %d\n",flag,test_case[flag-1]);

		}
		fclose(fp);
		return 1;
	}
	else
	{
		printf("文件out.list不存在！\n");
		return 0;
	}

}

int ispassed(int j)		//表明第几个测试用例的结果
{
	return test_result[j-1];
}


void inner(void * arg)		//线程内部调用函数
{
	int pth = *(int *)arg;
	printf("%d\n",pth);
	for (int j=pth*size+1;j<pth*size+size+1 && j<=casenum;j++)
	{
		char numname[10];
		snprintf(numname, 10,"%d",j);		
		char * gcovname=strcat(numname,".gcov");
		char pre[30] ="../traces.alt/";
		gcovname = strcat(pre,gcovname);

		if( (access(gcovname, 0 )) != -1 )
		{
			if(ispassed(j) == 1)
			{
				//	printf("%d:passed!\n",j);
				FILE * fp;
				fp = fopen(gcovname,"r");
				if(fp == NULL)
					return ;
				char * buf = malloc(1024*sizeof(char));
				int l = 0;
				while(fgets(buf,1024,fp) != NULL)
				{
					if(buf[8]>=48 && buf[8]<=57)	//数字的ascii码
					{
						pthread_mutex_lock(&mut);
						coverage_information_metric[j-1][l]=1;
						pass[l]++;//printf("%d\t%d\t%s",pass[l],l,buf);}
					pthread_mutex_unlock(&mut);
				}
				l++;
			}
			fclose(fp);
		}
		else
		{
			//	printf("%d:failed!\n",j);
			FILE * fp;
			fp = fopen(gcovname,"r");
			if(fp == NULL)
				return;
			char * buf = malloc(1024*sizeof(char));
			int l=0;
			while(fgets(buf,1024,fp) != NULL)
			{
				if(buf[8]>=48 && buf[8]<=57)
				{
					pthread_mutex_lock(&mut);
					coverage_information_metric[j-1][l]=1;
					fail[l]++;//printf("fail%d\t%s\n",fail[l],buf);}
				pthread_mutex_unlock(&mut);
			}
			l++;
		}
		fclose(fp);
	}
}
else
printf("%d.gcov文件不存在！\n",j);
}		
}

void create()		//创建线程
{	
	int temp;
	int pth = 0;
	memset(&thread,0,sizeof(thread));
	for (pth=0;pth<=MAX;pth++)
	{
		int * temppth = (int *)malloc(sizeof(int));
		*temppth = pth;
		if((temp = pthread_create(&thread[pth],NULL,(void *)inner,temppth)) != 0)
			printf("thread%d failed!\n",pth);
		else
			printf("thread%d running---\n",pth);
		//		sleep(1);
	}
}

void wait()		//等待线程全部结束
{
	for(int pth=0;pth<MAX+1;pth++)
	{
		if(thread[pth] != 0)
			pthread_join(thread[pth],NULL);
	}
}

void outcoverage()
{
	FILE * fp;	//写入文件
	char faultlistname[60];
	sprintf(faultlistname,"coverage");
	char * fault_list_name=strcat(faultlistname,"formula.fault.list");
	if((fp = fopen(fault_list_name,"w")) != NULL)
	{
		for(int i=0;i<casenum;i++){
			fprintf(fp,"%d	",test_result[i]);
			for(int j=0;j<linenum;j++){
				fprintf(fp,"%d	",coverage_information_metric[i][j]);

			}
			fprintf(fp,"\n");
		}
		fclose(fp);
	}
}

//main从shell接受三个参数：1.被测源程序总行数 2.failed用例数 3.passed用例数
int main(int argc,char * argv[])
{
	linenum = atoi(argv[1]);
	failnum = atoi(argv[2]);
	passnum = atoi(argv[3]);
	casenum = passnum + failnum;
	size = casenum / MAX;

//	if(failnum==0)
//	  return 1;
	memset(fail,0,sizeof(int)*LINES);
	memset(pass,0,sizeof(int)*LINES);
	memset(test_result,0,sizeof(int)*casenum);
	for(int i=0;i<casenum;i++){
		coverage_information_metric[i]=(int*)malloc(sizeof(int)*linenum);
		memset(coverage_information_metric[i],0,sizeof(int)*linenum);
	}
	read_test_case_result();
	create();
	wait();
	outcoverage();
}
