#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>

#include "model.h"
#include "LoadMapData.h"
#include "SearchBus.h"
#include "SearchStation.h"
#include "AddNewBus.h"//ChangeNodeToMap
#include "RemoveRoute.h"
#include "RemoveStation.h"
#include "ChangeRoute.h"//ChangeRouteArray

int RemoveStation(BusMap &Map)
{
    system("cls");
    printf("\n===== 删除站点信息 =====\n\n");

    char *removeStation=(char*)malloc(sizeof(char)*20);
    printf("请输入你要删除的站点：");
    scanf("%s",removeStation);
    int nStation=FindStation(Map,removeStation);
    if(nStation==NONE)
    {
        printf("[%s]站点不存在！\n",removeStation);
        return NONE;
    }

    RemoveStationToMap(Map,nStation);

    printf("成功删除[%s]站点信息\n",removeStation);
    printf("--------------------------------------\n\n");
    printf("\n输入任何键退出[删除站点信息]功能\n");
    getch();
    system("cls");
    return 1;
}

void RemoveStationToMap(BusMap &Map,int nStation)
{
    printf("\n--------------------------------------\n");
    //1.找到所有与删除站点相关路线
    char *station=Map.stations[nStation].station;
    printf("[%s]相关路线信息：\n",station);
    QueryStation(Map,station);
    printf("--------------------------------------\n");

    //2.删除所有以该站点为公交线路起点站或终点站的路线
    extern char **BUSES;
    int i,j,k,preStation,postStation;
    int nBus;
    Station *pStation;
    Route *pRoute;
    for(i=0,j=0; i<Map.bus_num; i++,j+=3)
    {
        if(strcmp(BUSES[j+1],station)==0)//该站点为公交线路起点站
        {
            nBus=FindBus(Map,BUSES[j]);

            pStation=&Map.stations[nStation];
            pRoute=pStation->routes;
            while(pRoute!=NULL)
            {
                if(pRoute->bus == nBus)
                {
                    postStation=pRoute->station;
                    break;
                }
                pRoute=pRoute->next;
            }
            RemoveRouteToMap(Map,nBus,nStation,postStation);//删除路线公共接口
            RemoveRouteToMap2(Map,nBus);//自动更改下行路线
            printf("\n--------------------------------------\n");
            printf("[%s]公交线路已更新：\n",Map.buses[nBus].name);
            QueryBus(Map,Map.buses[nBus].name);
            printf("--------------------------------------\n");
            printf("--------------------------------------\n");
            printf("[%s]公交线路已更新：\n",Map.buses[nBus+1].name);
            QueryBus(Map,Map.buses[nBus+1].name);
            printf("--------------------------------------\n");
        }
        if(strcmp(BUSES[j+2],station)==0)//该站点为公交线路终点站
        {
            nBus=FindBus(Map,BUSES[j]);
            preStation=FindPreStationInBus(Map,nBus,nStation,k);//k为辅助参数

            RemoveRouteToMap(Map,nBus,preStation,nStation);//删除路线公共接口
            RemoveRouteToMap2(Map,nBus);//自动更改下行路线
            printf("\n--------------------------------------\n");
            printf("[%s]公交线路已更新：\n",Map.buses[nBus].name);
            QueryBus(Map,Map.buses[nBus].name);
            printf("--------------------------------------\n");
            printf("自动更新下行路线\n");
            printf("--------------------------------------\n");
            printf("[%s]公交线路已更新：\n",Map.buses[nBus+1].name);
            QueryBus(Map,Map.buses[nBus+1].name);
            printf("--------------------------------------\n");
        }
    }

    //3.删除所有上行路线以该站点为起始站点的路线
    pStation=&Map.stations[nStation];
    pRoute=pStation->routes;
    Route *preRoute;
    int flag;//小细节 删除路线后，邻接表更改
    int distance;//辅助变量  存储距离
    while(pRoute!=NULL)
    {
        flag=0;
        preRoute=pRoute;
        if(pRoute->bus==0 || (pRoute->bus)%2==0)//筛选上行路线
        {
            if(pRoute == pStation->routes)//邻接表第一条出边链域改变
                flag=2;
            else
                flag=1;

            nBus=pRoute->bus;
            preStation=FindPreStationInBus(Map,nBus,nStation,distance);
            postStation=pRoute->station;

            printf("\n--------------------------------------\n");
            printf("已删除路线：[%s] [%s]-[%s]\n",Map.buses[nBus].name,station,Map.stations[postStation].station);
            printf("            [%s] [%s]-[%s]\n",Map.buses[nBus+1].name,Map.stations[postStation].station,station);
            printf("--------------------------------------\n");

            RemoveNodeToMap(Map,pRoute->bus,nStation,pRoute->station);
            RemoveRouteFromArray(pRoute->bus,nStation,pRoute->station);
            printf("请输入[%s]-[%s]之间距离：",Map.stations[preStation].station,Map.stations[postStation].station);
            scanf("%d",&distance);
            ChangeNodeToMap(Map,nBus,preStation,postStation,distance);
            ChangeRouteArray(nBus,preStation,postStation,distance);

            RemoveRouteToMap2(Map,nBus);//自动更改下行路线

            printf("\n--------------------------------------\n");
            printf("[%s]公交线路已更新：\n",Map.buses[nBus].name);
            QueryBus(Map,Map.buses[nBus].name);
            printf("--------------------------------------\n");
            printf("自动更新下行路线\n");
            printf("--------------------------------------\n");
            printf("[%s]公交线路已更新：\n",Map.buses[nBus+1].name);
            QueryBus(Map,Map.buses[nBus+1].name);
            printf("--------------------------------------\n");
        }
        if(flag==1)
            pRoute=preRoute->next;
        else if(flag==2)
            pRoute=pStation->routes;
        else
            pRoute=pRoute->next;
    }

    //4.最后从邻接表和STATIONS数组中移除nStation
    RemoveStationFromArray(Map,nStation);
}

void RemoveStationFromArray(BusMap &Map,int nStation)
{
    //删除STATIONS数组
    int i;
    extern char **STATIONS;
    extern int STATION_NUM;
    for(i=nStation; i<STATION_NUM-1; i++)
        strcpy(STATIONS[i],STATIONS[i+1]);
    STATION_NUM--;
    Map.station_num=STATION_NUM;
    STATIONS=(char**)realloc(STATIONS,sizeof(char*)*STATION_NUM);

    //修改ROUTESS数组所有站点编号
    int j;
    extern int *ROUTES;
    extern int ROUTE_NUM;
    for(i=0,j=0; i<ROUTE_NUM; i++,j+=4)
    {
        if(ROUTES[j+1]>nStation)
            ROUTES[j+1]--;
        if(ROUTES[j+2]>nStation)
            ROUTES[j+2]--;
    }
    //更新邻接表
    LoadMapData(Map);
}
