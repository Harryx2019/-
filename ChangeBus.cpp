#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>

#include "model.h"
#include "LoadMapData.h"
#include "SearchBus.h"
#include "RemoveRoute.h"
#include "ChangeBus.h"
#include "ChangeRoute.h"

int ChangeBus(BusMap &Map)
{
    system("cls");
    printf("\n===== 修改公交线路 =====\n");
    printf("提示：1.修改公交线路时,系统首先会提示修改公交线路起始站点信息,输入‘0’将不做修改\n");
    printf("      2.修改公交线路时,系统会依次提示修改公交线路路线目标站点和站点距离,输入‘0’将不做修改\n\n");

    int i;
    printf("现有公交线路：");
    for(i=0; i<Map.bus_num-1; i++)
        printf("%s、",Map.buses[i].name);
    printf("%s\n\n",Map.buses[i].name);

    char *bus=(char*)malloc(sizeof(char)*10);
    printf("请输入你要修改的公交线路：");
    scanf("%s",bus);
    int nBus=FindBus(Map,bus);
    if(nBus==NONE)
    {
        printf("公交线路[%s]不存在！\n",bus);
        return NONE;
    }

    if(ChangeBusToMap(Map,nBus)==NONE)
        return NONE;

    printf("\n--------------------------------------\n");
    printf("[%s]公交线路已更新：\n",Map.buses[nBus].name);
    QueryBus(Map,Map.buses[nBus].name);
    printf("--------------------------------------\n");
    printf("自动更新下行路线\n");
    RemoveRouteToMap2(Map,nBus);//自动更改下行路线
    printf("--------------------------------------\n");
    printf("[%s]公交线路已更新：\n",Map.buses[nBus+1].name);
    QueryBus(Map,Map.buses[nBus+1].name);
    printf("--------------------------------------\n");

    printf("\n输入任何键退出[修改公交线路]功能\n");
    getch();
    system("cls");
    return 1;
}

int ChangeBusToMap(BusMap &Map,int nBus)
{
    char *bus=Map.buses[nBus].name;
    int nBusStart=Map.buses[nBus].start;
    char *busStart=Map.stations[Map.buses[nBus].start].station;
    int nBusEnd=Map.buses[nBus].end;
    char *busEnd=Map.stations[Map.buses[nBus].end].station;

    printf("\n--------------------------------------\n");
    printf("[%s]公交线路：\n",Map.buses[nBus].name);
    QueryBus(Map,Map.buses[nBus].name);
    printf("--------------------------------------\n");
    //1.修改公交线路起点站
    char *newBusStart=(char *)malloc(sizeof(char)*20);
    printf("\n--------------------------------------\n");
    printf("修改[%s]公交线路起点站[%s]为:",bus,busStart);
    scanf("%s",newBusStart);
    if(strcmp(newBusStart,"0")!=0)
    {
        int nNewBusStart=FindStation(Map,newBusStart);
        if(nNewBusStart==NONE)
        {
            printf("站点[%s]不存在！\n",newBusStart);
            return NONE;
        }
        if(FindStationInBus(Map,nBus,nNewBusStart)!=NONE)
        {
            printf("站点[%s]已存在公交线路中!\n",newBusStart);
            return NONE;
        }
        //修改邻接表结构
        int routeEnd,routeDistance;
        routeEnd=FindPostStationInBus(Map,nBus,nBusStart,routeDistance);
        //修改newStation为公交线路起点站，需要连接和移除相关节点
        ChangeRouteStartAsBusStart(Map,nBus,nBusStart,nNewBusStart,routeEnd,routeDistance);
        //修改busStart nBusStart
        strcpy(busStart,newBusStart);
        nBusStart=nNewBusStart;
    }
    //2.修改公交线路终点站
    char *newBusEnd=(char *)malloc(sizeof(char)*20);
    printf("\n--------------------------------------\n");
    printf("修改[%s]公交线路终点站[%s]为:",bus,busEnd);
    scanf("%s",newBusEnd);
    if(strcmp(newBusEnd,"0")!=0)
    {
        int nNewBusEnd=FindStation(Map,newBusEnd);
        if(nNewBusEnd==NONE)
        {
            printf("站点[%s]不存在！\n",newBusEnd);
            return NONE;
        }
        if(FindStationInBus(Map,nBus,nNewBusEnd)!=NONE)
        {
            printf("站点[%s]已存在公交线路中!\n",newBusEnd);
            return NONE;
        }
        //修改邻接表结构
        int routeStart,routeDistance;
        routeStart=FindPreStationInBus(Map,nBus,nBusEnd,routeDistance);
        //修改newStation为公交线路终点站，需要连接和移除相关节点
        ChangeRouteEndAsBusEnd(Map,nBus,nBusEnd,routeStart,nNewBusEnd,routeDistance);
        //修改busEnd nBusEnd
        strcpy(busEnd,newBusEnd);
        nBusEnd=nNewBusEnd;
    }
    //3.依次修改公交线路路线信息
    printf("\n--------------------------------------\n");
    printf("修改公交线路[%s]各路线信息\n",bus);
    Station *pStation;
    Route *pRoute;
    int i=nBusStart,j;

    char *start=(char*)malloc(sizeof(char)*20);
    char *end=(char*)malloc(sizeof(char)*20);
    char *newEnd=(char*)malloc(sizeof(char)*20);
    int nNewEnd;
    int distance;
    while(i!=nBusEnd)
    {
        pStation=&Map.stations[i];
        pRoute=pStation->routes;
        while(pRoute!=NULL)
        {
            if(pRoute->bus==nBus)
            {
                j=i;
                i=pRoute->station;
                break;
            }
            pRoute=pRoute->next;
        }
        if(i==nBusEnd)
            break;

        start=Map.stations[j].station;
        end=Map.stations[i].station;
        distance=pRoute->distance;

        printf("\n--------------------------------------\n");
        printf("修改路线 [%s]-[%s] (站点距离：%d)\n\n",start,end,distance);
        printf("修改目标站点[%s]为:",end);
        scanf("%s",newEnd);
        if(strcmp(newEnd,"0")!=0)
        {
            nNewEnd=FindStation(Map,newEnd);
            if(nNewEnd==NONE)
            {
                printf("修改失败！-站点[%s]不存在！请重新输入！\n",newEnd);
                i=j;
                continue;
            }
            if(FindStationInBus(Map,nBus,nNewEnd)!=NONE)
            {
                printf("修改失败！-站点[%s]已存在公交线路[%s]中！请重新输入！\n",newEnd,bus);
                i=j;
                continue;
            }
            int postStation,postDistance;
            postStation=FindPostStationInBus(Map,nBus,i,postDistance);
            ChangeRouteEnd(Map,nBus,postStation,i,postDistance,j,nNewEnd,distance);
            //更新i end
            i=nNewEnd;
            end=Map.stations[i].station;
        }

        int newDistance;
        printf("修改站点距离[%d]为:",distance);
        scanf("%d",&newDistance);
        if(newDistance!=0)
        {
            ChangeNodeToMap(Map,nBus,j,i,newDistance);
            ChangeRouteArray(nBus,j,i,newDistance);
            printf("\n已修改路线[%s]-[%s](站点距离：%d)为[%s]-[%s](站点距离：%d)\n",start,end,distance,start,end,newDistance);
        }
    }
    return 1;
}

//修改BUSES数组
void ChangeBusArray(char *changeBus,char *changeStart,char *changeEnd)
{
    int i,j;
    extern int BUS_NUM;
    extern char **BUSES;

    for(i=0,j=0; i<BUS_NUM; i++,j+=3)
    {
        if(strcmp(BUSES[j],changeBus)==0)
        {
            strcpy(BUSES[j+1],changeStart);
            strcpy(BUSES[j+2],changeEnd);
        }
    }
    printf("\n--------------------------------------\n");
    printf("已修改公交线路： [%s线路]  从:[%s] 开往 [%s]\n",changeBus,changeStart,changeEnd);
}
