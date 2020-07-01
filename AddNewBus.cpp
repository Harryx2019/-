#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>

#include "model.h"
#include "LoadMapData.h"
#include "SearchBus.h"
#include "AddNewRoute.h"
#include "AddNewBus.h"
#include "RemoveRoute.h"
#include "ChangeRoute.h"//ChangeRouteArray

int AddNewBus(BusMap &Map)
{
    system("cls");
    printf("\n===== 新增公交路线 =====\n");
    printf("提示：1.新增公交路线中相关站点必须为已存在站点，若需要新增站点，请先使用[5. 新增站点信息]\n");
    printf("      2.只需新增上行线路相关信息，下行线路会自动生成！\n");
    printf("      3.新增公交线路相关站点信息时，不可重复出现站点！\n");
    printf("      4.新增公交线路相关站点信息时，请按顺序添加路线！\n\n");

    //输入公交线路名称
    char *newBus=(char*)malloc(sizeof(char)*10);
    printf("请输入新增公交路线名称：(例如34路、539路）");
    scanf("%s",newBus);
    //检测公交线路是否存在
    if(FindBus(Map,newBus)!=NONE)
    {
        printf("该公交路线已存在！\n");
        return NONE;
    }

    //输入公交路线起点站
    char *newStart=(char*)malloc(sizeof(char)*20);
    printf("请输入公交线路起点站：");
    scanf("%s",newStart);
    if(FindStation(Map,newStart)==NONE)
    {
        printf("该站点不存在！\n");
        return NONE;
    }

    //输入公交路线终点站
    char *newEnd=(char*)malloc(sizeof(char)*20);
    printf("请输入公交线路终点站：");
    scanf("%s",newEnd);
    if(FindStation(Map,newEnd)==NONE)
    {
        printf("该站点不存在\n");
        return NONE;
    }

    //修改BUSES数组和邻接表，自动添加下行路线
    AddNewBusToMap(Map,newBus,newStart,newEnd);
    //添加新公交线路中的路线
    AddNewBusRouteToMap(Map,newBus);

    printf("\n输入任何键退出[新增公交路线]功能\n");
    getch();
    system("cls");
    return 1;
}

void AddNewBusToMap(BusMap &Map,char *newBus,char *newStart,char *newEnd)
{
    extern int BUS_NUM;
    int nBus=BUS_NUM;
    int nStart=FindStation(Map,newStart);
    int nEnd=FindStation(Map,newEnd);

    char *newBus1=(char*)malloc(sizeof(char)*10);
    char *newBus2=(char*)malloc(sizeof(char)*10);
    strcpy(newBus1,newBus);
    strcpy(newBus2,newBus);

    Map.buses=(Bus*)realloc(Map.buses,sizeof(Bus)*(BUS_NUM+2));
    //将公交线路信息添加到BUSES数组中并修改图bus结构
    strcat(newBus1,"上行");
    Map.buses[nBus].name=newBus1;
    Map.buses[nBus].start=nStart;
    Map.buses[nBus].end=nEnd;
    AddNewBusToArray(newBus1,newStart,newEnd);

    //初始化上行公交第一条路线：起点站-终点站
    AddNewNodeToMap(Map,nBus,nStart,nEnd,0);
    AddNewRouteToArray(nBus,nStart,nEnd,0);
    nBus++;

    strcat(newBus2,"下行");
    Map.buses[nBus].name=newBus2;
    Map.buses[nBus].start=nEnd;
    Map.buses[nBus].end=nStart;
    AddNewBusToArray(newBus2,newEnd,newStart);

    Map.bus_num=BUS_NUM;

    printf("\n--------------------------------------\n");
    printf("已添加公交线路：%s [%s]-[%s]\n",newBus1,newStart,newEnd);
    printf("已添加公交线路：%s [%s]-[%s]\n",newBus2,newEnd,newStart);
}

void AddNewBusToArray(char *newBus,char *newStart,char *newEnd)
{
    extern char **BUSES;
    extern int BUS_NUM;
    int j=BUS_NUM*3;
    BUS_NUM++;
    BUSES=(char**)realloc(BUSES,sizeof(char*)*BUS_NUM*3);
    BUSES[j]=newBus;
    BUSES[j+1]=newStart;
    BUSES[j+2]=newEnd;
}

void AddNewBusRouteToMap(BusMap &Map,char *newBus)
{
    printf("\n--------------------------------------\n");
    printf("添加公交线路中各路线\n");

    extern int BUS_NUM;
    int STATION[BUS_NUM],DISTANCE[BUS_NUM];
    //添加上行路线
    char *bus1=(char*)malloc(sizeof(char)*10);
    strcpy(bus1,newBus);
    strcat(bus1,"上行");

    char *newStart=(char*)malloc(sizeof(char)*20);
    char *newEnd=(char*)malloc(sizeof(char)*20);
    int nBus=FindBus(Map,bus1);
    int nStart,nEnd,distance;
    int i=1;
    STATION[0]=Map.buses[nBus].start;
    while(1)
    {
        printf("请输入路线%d:\n",i);
        printf("请输入起始站点：");
        scanf("%s",newStart);
        nStart=FindStation(Map,newStart);
        if(nStart==NONE)
        {
            printf("该站点不存在！请重新输入！\n");
            continue;
        }

        printf("请输入目的站点：");
        scanf("%s",newEnd);
        nEnd=FindStation(Map,newEnd);
        if(nEnd==NONE)
        {
            printf("该站点不存在！请重新输入！\n");
            continue;
        }
        STATION[i]=nEnd;//记录站点

        printf("请输入两站点之间距离：");
        scanf("%d",&distance);
        DISTANCE[i]=distance;

        //检测是否到达终点站
        if(FindStationInBus(Map,nBus,nEnd)==3)
        {
            //检测路线起始站点是否已存在当前公交线路
            int flag=0;
            Station *pStation=&Map.stations[nStart];
            Route *pRoute=pStation->routes;
            while(pRoute!=NULL)
            {
                if(pRoute->bus==nBus && pRoute->station==nEnd)
                {
                    flag=1;//以nStart为起始站点的路线在公交线路中仅仅只有一条nStart-busEnd
                    break;
                }
                pRoute=pRoute->next;
            }
            if(flag==0)
            {
                printf("路线不合法！请重新输入！\n");
                continue;
            }

            ChangeNodeToMap(Map,nBus,nStart,nEnd,distance);//仅仅修改distance
            ChangeRouteArray(nBus,nStart,nEnd,distance);
            printf("已添加路线：%s [%s]-[%s] (站点距离：%d)\n",bus1,newStart,newEnd,distance);
            printf("\n--------------------------------------\n");
            break;
        }
        //检测新路线是否合法
        int flag=JudgeRouteType(Map,nBus,nStart,nEnd);
        if(flag==NONE)
        {
            printf("\n--------------------------------------\n\n");
            continue;
        }
        else if(flag==1||flag==2)
        {
            printf("此功能下不可修改！如需修改请使用功能[10.修改公交线路]\n");
            printf("\n--------------------------------------\n\n");
            continue;
        }
        else if(flag==3)
        {
            printf("请按顺序添加路线！\n");
            printf("\n--------------------------------------\n\n");
            continue;
        }

        //在Map邻接表中添加新路线
        AddNewNodeToMap(Map,nBus,nStart,nEnd,distance);
        //在Map邻接表中修改路线
        RemoveNodeToMap(Map,nBus,nStart,Map.buses[nBus].end);//前提此路线存在！
        AddNewNodeToMap(Map,nBus,nEnd,Map.buses[nBus].end,0);

        //在ROUTES中添加新路线
        AddNewRouteToArray(nBus,nStart,nEnd,distance);
        //在ROUTES中修改路线
        RemoveRouteFromArray(nBus,nStart,Map.buses[nBus].end);
        AddNewRouteToArray(nBus,nEnd,Map.buses[nBus].end,0);

        //PrintMap(Map);
        printf("已添加路线：%s [%s]-[%s] (站点距离：%d)\n",bus1,newStart,newEnd,distance);
        printf("\n--------------------------------------\n");
        i++;
    }

    //自动添加下行路线
    printf("\n--------------------------------------\n");
    printf("自动添加下行路线\n");
    char *bus2=(char*)malloc(sizeof(char)*10);
    strcpy(bus2,newBus);
    strcat(bus2,"下行");
    nBus=FindBus(Map,bus2);
    int j;
    for(j=i; j>0; j--)
    {
        //在Map邻接表中添加新路线
        AddNewNodeToMap(Map,nBus,STATION[j],STATION[j-1],DISTANCE[j]);
        //在ROUTES中添加新路线
        AddNewRouteToArray(nBus,STATION[j],STATION[j-1],DISTANCE[j]);
        printf("已添加路线：%s [%s]-[%s] (站点距离：%d)\n",bus2,Map.stations[STATION[j]].station,Map.stations[STATION[j-1]].station,DISTANCE[j]);
    }
    printf("--------------------------------------\n\n");

    nBus=FindBus(Map,bus1);
    printf("成功添加[%s]公交线路\n\n",bus1);
    printf("[%s]公交线路：\n",bus1);
    QueryBus(Map,bus1);
    printf("\n--------------------------------------\n");

    nBus=FindBus(Map,bus2);
    printf("成功添加[%s]公交线路\n\n",bus2);
    printf("[%s]公交线路：\n",bus2);
    QueryBus(Map,bus2);
    printf("\n--------------------------------------\n");
}
