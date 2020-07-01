#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

#include "model.h"
#include "Stack.h"//广度优先搜素辅助栈结构
#include "LoadMapData.h"
#include "SearchBus.h"
#include "SearchStation.h"
#include "SearchRoute.h"

int SearchRoute(BusMap &Map)
{
    system("cls");
    char *startStation=(char *)malloc(sizeof(char)*20);
    char *endStation=(char *)malloc(sizeof(char)*20);
    int nStartStation,nEndStation;
    printf("\n===== 查询路线信息 =====\n");
    printf("提示：1.查询路线信息起始站点和终止站点必须为已存在站点\n");
    printf("      2.查询路线信息会显示从起始站点到终止站点只用换乘一次的路线\n\n");

    printf("请输入要查询的路线起始站点：");
    scanf("%s",startStation);
    nStartStation=FindStation(Map,startStation);
    if(nStartStation==NONE)
    {
        printf("站点[%s]不存在！\n",startStation);
        return NONE;
    }

    printf("请输入要查询的路线目标站点：");
    scanf("%s",endStation);
    nEndStation=FindStation(Map,endStation);
    if(nEndStation==NONE)
    {
        printf("站点[%s]不存在！\n",endStation);
        return NONE;
    }

    if(QueryRoute(Map,nStartStation,nEndStation)==NONE)
        return NONE;

    printf("输入任何键退出[查询路线信息]功能\n");
    getch();
    system("cls");
    return 1;
}

int QueryRoute(BusMap &Map,int nStartStation,int nEndStation)
{
    //初始化path 路径数组
    //path结构为 bus-station-bus-station
    int i;
    int initialPathNum=3,initialPathStationNum=20;
    int **path=(int**)malloc(sizeof(int*)*initialPathNum);
    int *pathStationNum=(int*)malloc(sizeof(int)*initialPathNum);
    for(i=0; i<initialPathNum; i++)
        path[i]=(int*)malloc(sizeof(int)*initialPathStationNum*2);
    path[0][0]=-1;
    path[0][1]=nStartStation;

    //初始化站点栈辅助数组
    int *stationInStack=(int*)malloc(sizeof(int)*Map.station_num);
    for(i=0; i<Map.station_num; i++)
        stationInStack[i]=0;

    //初始化route status
    Station *pStation;
    Route *pRoute;
    for(i=0; i<Map.station_num; i++)
    {
        pStation=&Map.stations[i];
        pRoute=pStation->routes;
        while(pRoute!=NULL)
        {
            pRoute->staus=0;
            pRoute=pRoute->next;
        }
    }

    //初始化栈结构
    Stack stationStack,busStack;
    InitStack(stationStack);
    InitStack(busStack);

    //nStartStation入栈
    PushStack(stationStack,nStartStation);
    PushStack(busStack,-1);
    stationInStack[nStartStation]=1;

    int j,k,nStation;
    int flag=0,pathNum=0;
    while(EmptyStack(stationStack)==false)
    {
        nStation=GetTopStack(stationStack);//栈顶元素出栈
        if(nStation==nEndStation)//找到一条路径
        {
            if(flag==0)
            {
                printf("\n[%s]-[%s]之间存在路线\n",Map.stations[nStartStation].station,Map.stations[nEndStation].station);
                flag=1;
            }

            pathNum++;
            if(pathNum>initialPathNum)//扩容
            {
                initialPathNum+=2;
                path=(int**)realloc(path,sizeof(int*)*initialPathNum);
                for(i=pathNum-1; i<initialPathNum; i++)
                {
                    path[i]=(int*)malloc(sizeof(int)*initialPathStationNum*2);
                    path[i][0]=-1;
                    path[i][1]=nStartStation;
                }
                pathStationNum=(int*)realloc(pathStationNum,sizeof(int)*initialPathNum);
            }
            //将栈内站点依次输入path[pathNum]数组
            pathStationNum[pathNum-1]=GetStackNum(stationStack);
            if(pathStationNum[pathNum-1]>initialPathStationNum)
                path[pathNum-1]=(int*)realloc(path[pathNum-1],sizeof(int)*pathStationNum[pathNum-1]*2);

            i=pathNum-1;
            path[i][0]=-1;
            path[i][1]=nStartStation;
            for(k=1,j=2; k<pathStationNum[pathNum-1]; k++,j+=2)
            {
                path[i][j]=GetStackElement(busStack,k);
                path[i][j+1]=GetStackElement(stationStack,k);
            }
            //nEndStation出栈
            PopStack(stationStack);
            PopStack(busStack);
            stationInStack[nEndStation]=0;
            //更新route status
            updateRouteStatus(Map,nEndStation,stationInStack);
        }
        else
        {
            pStation=&Map.stations[nStation];
            pRoute=pStation->routes;
            while(pRoute!=NULL)
            {
                i=pRoute->station;
                if(stationInStack[i]==0 && pRoute->staus==0)
                {
                    //入栈
                    PushStack(stationStack,i);
                    PushStack(busStack,pRoute->bus);
                    stationInStack[i]=1;
                    pRoute->staus=1;
                    break;
                }
                pRoute=pRoute->next;
            }
            if(pRoute==NULL)//该节点没有符合要求的后续节点
            {
                //nStation出栈
                PopStack(stationStack);
                PopStack(busStack);
                stationInStack[nStation]=0;
                updateRouteStatus(Map,nStation,stationInStack);
            }
        }
    }

    if(flag==1)
        PrintRoute(Map,nStartStation,nEndStation,pathNum,pathStationNum,path);
    else if(flag==0)
    {
        printf("[%s]-[%s]之间不存在路线！\n\n",Map.stations[nStartStation].station,Map.stations[nEndStation].station);
        return NONE;
    }

    return 1;
}

void PrintRoute(BusMap Map,int nStartStation,int nEndStation,int pathNum,int *pathStationNum,int **path)
{
    printf("\n--------------------------------------\n");
    int i,j,k;
    int num=0;//只需换乘一次路线数
    int *transferBusOnceRoutes=(int*)malloc(sizeof(int)*(pathNum));

    int inBus=-1,flag;//flag为换乘标志
    for(i=0; i<pathNum; i++)
    {
        inBus=path[i][2];
        flag=0;
        for(k=1,j=2; k<pathStationNum[i]; k++,j+=2)
        {
            if(path[i][j]!=inBus)//判断换乘次数
            {
                if(flag==1)//已经换乘一次
                {
                    transferBusOnceRoutes[i]=-1;
                    flag=2;
                    break;
                }
                else
                {
                    inBus=path[i][j];
                    flag=1;
                }
            }
        }
        if(flag!=2)//至多换乘一次
        {
            transferBusOnceRoutes[i]=i;
            num++;
        }
    }

    char *start=Map.stations[nStartStation].station;
    char *end=Map.stations[nEndStation].station;
    printf("从[%s]-[%s] 共找到 [%d]条至多换乘一次的线路：\n",start,end,num);
    int traverserNum=1;
    for(i=0; i<pathNum; i++)
    {
        if(transferBusOnceRoutes[i]!=-1)
        {
            printf("\n--------------------------------------\n\n");
            inBus=path[i][2];
            printf("路线[%d]:   %s----[乘坐：%s]----",traverserNum,start,Map.buses[inBus].name);
            traverserNum++;

            for(k=1,j=2; k<pathStationNum[i]; k++,j+=2)
            {
                if(path[i][j]!=inBus)
                {
                    inBus=path[i][j];
                    printf("----[换乘：%s]----",Map.buses[inBus].name);
                }
                printf("->%s",Map.stations[path[i][j+1]].station);
            }
            printf("\n");
        }
    }
    printf("\n--------------------------------------\n\n");
}

void updateRouteStatus(BusMap &Map,int station,int *stationInStack)
{
    Station *pStation=&Map.stations[station];
    Route *pRoute=pStation->routes;
    while(pRoute!=NULL)
    {
        if(stationInStack[pRoute->station]==0)
            pRoute->staus=0;
        pRoute=pRoute->next;
    }

    int i;
    for(i=0; i<Map.station_num && i!=station; i++)
    {
        pStation=&Map.stations[i];
        pRoute=pStation->routes;
        while(pRoute!=NULL)
        {
            if(pRoute->station==station && stationInStack[i]==0)
                pRoute->staus=0;
            pRoute=pRoute->next;
        }
    }
}
