#include <stdio.h>
#include <malloc.h>

#define True 1
#define False 0
typedef int bool;

int n;  //进程数
int m;  //资源类数
int *Available; //可使用资源向量
int **Max;  //最大需求矩阵
int **Allocation;    //分配矩阵
int **Need; //需求矩阵
bool safe = False;
typedef struct
{
    int id;
    int *req_src;
}Request;
Request* new_request;

void initial();  
void request();
void process();
bool safe_detect();
bool vector_compare(int *a, int *b, int len);  
void vector_add(int *a, int *b, int len);
void vector_sub(int *a, int *b, int len);
void show(int *a, int len);


int main()
{
    initial();
    request();
    process();
    return 0;
}

void show(int *a, int len)
{
    int i = 0;
    while(i<len)
    {
        printf(" %d",*(a+i));
        i++;
    }
   printf("\n");
}
 

bool vector_compare(int *a, int *b, int len)    // If vector a >= vector b, return True
{
    int i = 0;
    while(i<len)
    {
        if(*(a+i)<*(b+i))
            return False;
        i++;
    }
    return True;
}

void vector_add(int *a, int *b, int len) //vector a += vector b
{
    int i = 0;
    while(i<len)
    {
        *(a+i) += *(b+i);
        i++;
    } 
}

void vector_sub(int *a, int *b, int len) //vector a -= vector b
{
    int i = 0;
    while(i<len)
    {
        *(a+i) -= *(b+i);
        i++;
    } 
}

void initial()
{
    int i;
    int j;

    printf("请输入进程数:\n");
    scanf("%d",&n);

    printf("请输入资源类数:\n");
    scanf("%d",&m);    

    printf("请输入可使用资源向量:\n");
    Available = (int*)malloc(sizeof(int)*m);
    for(i=0; i<m; i++)
        scanf("%d",&Available[i]);

    printf("请输入最大需求矩阵:\n");
    Max = (int**)malloc(sizeof(int*)*n);
    for(i=0; i<n; i++)
    {
        Max[i] = (int*)malloc(sizeof(int)*m);
        for(j=0; j<m; j++)
            scanf("%d",&Max[i][j]);
    }


    printf("请输入分配矩阵:\n");
    Allocation = (int**)malloc(sizeof(int*)*n);
    for(i=0; i<n; i++)
    {
        Allocation[i] = (int*)malloc(sizeof(int)*m);
        for(j=0; j<m; j++)
            scanf("%d",&Allocation[i][j]);
    }

    Need = (int**)malloc(sizeof(int*)*n);
    for(i=0;i<n;i++)
    {
        Need[i] = (int *)malloc(sizeof(int)*m);
        for(j=0;j<m;j++)
            Need[i][j] = Max[i][j] - Allocation[i][j];
    }

}

void request()
{
    int i,id;
    new_request = (Request*)malloc(sizeof(Request));
    new_request->req_src = (int*)malloc(sizeof(int)*m);
    printf("请输入进程的ID\n");
    scanf("%d",&id);
    new_request->id = id - 1;
    printf("请输入进程申请资源向量\n");
    for(i=0; i<m; i++)
        scanf("%d",&new_request->req_src[i]);
}

void process()
{
    int i = new_request->id;
    if(vector_compare(Need[i],new_request->req_src,m))
        {
            if(vector_compare(Available,new_request->req_src,m))
            {
                vector_sub(Available,new_request->req_src,m);
                vector_add(Allocation[i],new_request->req_src,m);
                vector_sub(Need[i],new_request->req_src,m);
                safe_detect();
            }
            else
            {
                printf("程序所申请资源大于系统当前所剩资源，推迟执行!\n");  
                return;
            }
                              
        }
    else
    {
        printf("程序所申请资源大于该程序所需资源，无法执行!\n"); 
        return;
    }    
    if(safe)
    {
        printf("系统安全,进程可以执行!\n");
        return;
    }
    else
    {
        printf("系统不安全,进程无法执行!\n");
        vector_add(Available,new_request->req_src,m);
        vector_sub(Allocation[i],new_request->req_src,m);
        vector_add(Need[i],new_request->req_src,m);
        return;
    }
        
}

bool safe_detect()
{
    int *work = Available;
    bool *finish = (bool*)malloc(sizeof(bool)*n);
    int i;

    //初始化finish
    for(i=0; i<n; i++)
        finish[i] = False;
    

    for(i=0; i<n; i++)
    {
        if(finish[i]==False&&vector_compare(work,Need[i],m))
        {
            printf("尝试执行第%d进程\n",i+1);
            vector_add(work,Allocation[i],m);   //尝试执行该进程，释放资源

            finish[i] = True;
            i = -1;  //尝试分配后，从头查找是否还有可以执行的进程，考虑到i++，故此处为-1
        }
    }
    
    for(i=0; i<n; i++)
        if(finish[i]==False)
            break;
    if(i==n)
        safe = True;
    else
        safe = False;
}
