#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "model.h"
#include "LoadMapData.h"

extern char **STATIONS;
extern char **BUSES;
extern int *ROUTES;

extern int STATION_NUM,BUS_NUM,ROUTE_NUM;

void LoadMapData(BusMap &Map)
{
    //将三个数组中数据载入邻接表中
    int i,j;

    //加载站点信息
    Map.station_num=STATION_NUM;
    Map.stations=(Station*)malloc(sizeof(Station)*STATION_NUM);
    for(i=0;i<STATION_NUM;i++)
    {
        Map.stations[i].station=(char*)malloc(sizeof(char)*20);
        strcpy(Map.stations[i].station,STATIONS[i]);//初始化站点名
        Map.stations[i].routes=NULL;//下行线路暂时设置为空
    }

    //加载路段信息
    for(i=0,j=0;i<ROUTE_NUM;i++,j+=4)
        AddNewNodeToMap(Map,ROUTES[j],ROUTES[j+1],ROUTES[j+2],ROUTES[j+3]);

    //加载公交线路信息
    int busStart,busEnd;
    Map.bus_num=BUS_NUM;
    Map.buses=(Bus*)malloc(sizeof(Bus)*BUS_NUM);
    for(i=0,j=0;i<BUS_NUM;i++,j+=3)
    {
        Map.buses[i].name=(char*)malloc(sizeof(char)*10);
        strcpy(Map.buses[i].name,BUSES[j]);
        busStart=FindStation(Map,BUSES[j+1]);
        busEnd=FindStation(Map,BUSES[j+2]);
        Map.buses[i].start=busStart;
        Map.buses[i].end=busEnd;
    }
}

void AddNewNodeToMap(BusMap Map,int nBus,int nStart,int nEnd,int distance)
{
    //插入起点的出边链域
    Station *pStation=&Map.stations[nStart];
    Route *pRoute=pStation->routes;
    //创建新节点
    Route *pNewRoute=(Route*)malloc(sizeof(Route));
    pNewRoute->bus=nBus;
    pNewRoute->station=nEnd;
    pNewRoute->distance=distance;

    pNewRoute->next=pRoute;
    pStation->routes=pNewRoute;
}

int FindBus(BusMap Map,char *bus)
{
    int i;
    for(i=0;i<Map.bus_num;i++)
    {
        if(strcmp(Map.buses[i].name,bus)==0)
            return i;
    }
    return NONE;
}

int FindStation(BusMap Map,char *station)
{
    int i;
    for(i=0;i<Map.station_num;i++)
    {
        if(strcmp(Map.stations[i].station,station)==0)
            return i;
    }
    return NONE;
}

int FindStationInBus(BusMap Map,int nBus,int nStation)
{
    char *station=Map.stations[nStation].station;
    int start=Map.buses[nBus].start;
    int end=Map.buses[nBus].end;

    //判断nStation是否为公交线路终点站
    Station *pStation=&Map.stations[end];
    if(strcmp(pStation->station,station)==0)
        return 3;

    //判断nStation是否为公交线路起点站
    pStation=&Map.stations[start];
    if(strcmp(pStation->station,station)==0)
        return 2;

    Route *pRoute;
    int i=start;
    while(i!=end)
    {
        pStation=&Map.stations[i];
        pRoute=pStation->routes;
        while(pRoute!=NULL)
        {
            if(pRoute->bus==nBus)
            {
                i=pRoute->station;
                break;
            }
            pRoute=pRoute->next;
        }
        if(strcmp(pStation->station,station)==0)
            return 1;//站点存在当前公交线路
    }
    return NONE;//站点不存在当前公交线路
}

int FindPreStationInBus(BusMap Map,int nBus,int nStation,int &distance)//寻找在公交线路中指定站点前一站点，注意distance参数
{
    int preStation;//返回参数

    int busStart=Map.buses[nBus].start;
    int busEnd=Map.buses[nBus].end;

    int k=busStart;
    Station *pStation;
    Route *pRoute;
    while(k!=busEnd)
    {
        pStation=&Map.stations[k];
        pRoute=pStation->routes;
        while(pRoute!=NULL)
        {
            if(pRoute->bus == nBus)
            {
                preStation=k;
                k=pRoute->station;
                break;
            }
            pRoute=pRoute->next;
        }
        if(k==nStation)
        {
            distance=pRoute->distance;
            return preStation;
        }
    }
    return NONE;//基本不会用到 因为调用此函数可以确保 站点在该公交线路中
}

int FindPostStationInBus(BusMap Map,int nBus,int nStation,int &distance)
{
    int busStart=Map.buses[nBus].start;
    int busEnd=Map.buses[nBus].end;

    int i=busStart,j;
    Station *pStation;
    Route *pRoute;
    while(i!=busEnd)
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
        if(j==nStation)
        {
            distance=pRoute->distance;
            return i;
        }
    }
    return NONE;
}

void PrintMap(BusMap Map)
{
    //打印邻接表
    int i;
    Route *pRoute=(Route*)malloc(sizeof(Route));
    for(i=0;i<Map.station_num;i++)
    {
        printf("%d %s ",i,Map.stations[i].station);
        pRoute=Map.stations[i].routes;
        while(pRoute!=NULL)
        {
            printf("->(%d)%s[%s][%d] ",pRoute->station,Map.stations[pRoute->station].station,Map.buses[pRoute->bus].name,pRoute->distance);
            pRoute=pRoute->next;
        }
        printf("\n");
    }

    extern int BUS_NUM,STATION_NUM,ROUTE_NUM;
    printf("%d %d %d\n",BUS_NUM,STATION_NUM,ROUTE_NUM);
}
