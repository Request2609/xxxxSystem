#include <stdio.h>  
#include <stdlib.h>  
#include <string.h>  
#define SIZE 10  
/* 
多处最优服务次序，输入n个顾客的服务时间和s处提供此项服务。输出最小平均等待时间。 
*/  
int s,n;  
int B[SIZE];  //存储服务点  
int C[SIZE];  //存储服务等待时间  
double  greedy(int A[]);  
void sort(int A[]);  
void swap(int *a,int *b);  
  
int main()  
{  
    int i,j,temp;  
    printf("请输入顾客数目和服务点个数：\n");  
    scanf("%d %d",&n,&s);  
    int A[n];
    printf("请输入每个顾客的服务时间：\n");  
    for(i = 0;i < n;i++)  
    {  
        scanf("%d",&A[i]);  
    } 
    
	for(i=0;i<n;i++){ //按服务时间由小到大排列  
       for(j=i+1;j<n;j++){
            if(A[i]>A[j]){
                temp=A[i];A[i]=A[j];A[j]=temp;
            }
        }
    } 
    printf("最小平均等待时间：%lf",greedy(A));  
    return 0;  
}  
  
double greedy(int A[])  
{  
    int i= 0 ,j = 0;  
    while(i < n)  
    {  
        B[j] += A[i];  
        C[j] += B[j];      //C[i] 存储每个顾客的等待时间  
        i++;  
        j++;  
        if(j == s)         //安排s个服务点的活动  
        {  
            j = 0;  
        }  
    }  
    double t=0;
    for(i = 0;i < s;i++)  
        t += C[i];      //每个服务点的等待时间累加  
    return  t/n;  
} 

