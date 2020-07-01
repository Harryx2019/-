#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>

#include "model.h"
#include "LoadMapData.h"
#include "SearchBus.h"
#include "SearchStation.h"
#include "AddNewRoute.h"
#include "RemoveRoute.h"
#include "ChangeBus.h"
#include "ChangeRoute.h"
#include "ChangeStation.h"

int ChangeStation(BusMap &Map)
{
    system("cls");
    printf("\n===== 修改站点信息 =====\n");
    printf("提示:1.修改站点信息第一步会提示是否修改站点名称,输入'0'为不修改\n");
    printf("     2.只需修改与待修改站点上行公交线路相关路线,下行公交线路将自动更改\n");
    printf("     3.系统会依次提示与该站点相关路线信息，逐条修改每一路线,输入'0'为不修改\n\n");

    char *changeStation=(char*)malloc(sizeof(char)*20);
    printf("请输入你要修改的站点：");
    scanf("%s",changeStation);
    int nStation=FindStation(Map,changeStation);
    if(nStation==NONE)
    {
        printf("[%s]站点不存在！\n",changeStation);
        return NONE;
    }

    if(ChangeStationToMap(Map,nStation)==NONE)
        return NONE;

    printf("成功修改[%s]站点信息\n",changeStation);
    printf("--------------------------------------\n\n");
    printf("输入任何键退出[修改站点信息]功能\n");
    getch();
    system("cls");
    return 1;
}

int ChangeStationToMap(BusMap &Map,int nStation)
{
    char *changeStation=(char*)malloc(sizeof(char)*20);
    strcpy(changeStation,Map.stations[nStation].station);

    //1.修改站点名称
    char *station=(char*)malloc(sizeof(char)*20);
    printf("修改站点[%s]名称为:",changeStation);
    scanf("%s",station);
    if(strcmp(station,"0")!=0)
    {
        int nStation2=FindStation(Map,station);
        if(nStation2!=NONE)
        {
            printf("修改失败！-不可将站点名称修改为已存在站点！\n");
            return NONE;
        }
        //修改STATIONS数组
        extern char **STATIONS;
        strcpy(STATIONS[nStation],station);
        strcpy(Map.stations[nStation].station,station);
        printf("\n已成功修改站点[%s]名称为[%s]\n",changeStation,station);

        //修改以该站点为公交线路起点站或终点站的公交线路信息
        extern char **BUSES;
        int i,j;
        for(i=0,j=0; i<Map.bus_num; i++,j+=3)
        {
            if(strcmp(BUSES[j+1],changeStation)==0)//该站点为公交线路起点站
                ChangeBusArray(BUSES[j],station,BUSES[j+2]);
            if(strcmp(BUSES[j+2],changeStation)==0)//该站点为公交线路终点站
                ChangeBusArray(BUSES[j],BUSES[j+1],station);
        }
        strcpy(changeStation,station);
    }

    //2.找到所有与修改站点相关路线
    printf("\n--------------------------------------\n");
    printf("[%s]相关路线信息：\n",changeStation);
    QueryStation(Map,changeStation);
    printf("--------------------------------------\n");

    //3.修改所有以该站点为起始站点的路线信息
    if(ChangeStationAsStart(Map,nStation)==NONE)
        return NONE;

    //4.修改所有以该站点为目标站点的路线信息
    if(ChangeStationAsEnd(Map,nStation)==NONE)
        return NONE;

    return 1;
}

int ChangeStationAsStart(BusMap &Map,int nStation)
{
    char *changeStation=Map.stations[nStation].station;
    int nBus,routeEnd,routeDistance;
    char *bus;
    char *station=(char*)malloc(sizeof(char)*20);
    char *routeStation=(char*)malloc(sizeof(char)*20);//对应routeEnd
    int newStation,flag;
    Station *pStation=&Map.stations[nStation];
    Route *pRoute=pStation->routes;
    while(pRoute!=NULL)
    {
        flag=0;

        nBus=pRoute->bus;
        bus=Map.buses[nBus].name;
        if(nBus==0 || nBus%2==0)//筛选上行路线
        {
            routeEnd=pRoute->station;
            routeStation=Map.stations[routeEnd].station;
            routeDistance=pRoute->distance;

            printf("\n--------------------------------------\n");
            printf("修改路线[%s] [%s]-[%s](站点距离:%d)\n",bus,changeStation,routeStation,routeDistance);
            printf("修改目标站点[%s]为：",routeStation);
            scanf("%s",station);
            if(strcmp(station,"0")!=0)
            {
                flag=1;//修改标记
                newStation=FindStation(Map,station);
                if(newStation==NONE)
                {
                    printf("修改失败！站点[%s]不存在!请重新输入！\n",station);
                    continue;
                }
                if(FindStationInBus(Map,nBus,newStation)!=NONE)
                {
                    printf("修改失败！-不可将站点修改为公交线路中已存在站点！请重新输入！\n");
                    continue;
                }
                //如果修改路线目标站点为公交线路终点站
                if(routeEnd==Map.buses[nBus].end)
                    ChangeRouteEndAsBusEnd(Map,nBus,routeEnd,nStation,newStation,routeDistance);
                else
                {
                    int postStation,postDistance;
                    postStation=FindPostStationInBus(Map,nBus,routeEnd,postDistance);
                    ChangeRouteEnd(Map,nBus,postStation,routeEnd,postDistance,nStation,newStation,routeDistance);
                }
                //更新pRoute routeEnd
                pRoute=pStation->routes;
                while(pRoute!=NULL)
                {
                    if(pRoute->bus==nBus)
                        break;
                    pRoute=pRoute->next;
                }
                routeEnd=pRoute->station;
            }


            int distance;
            printf("\n修改站点距离[%d]为:",pRoute->distance);
            scanf("%d",&distance);
            if(distance!=0)
            {
                int oldDistance=pRoute->distance;
                ChangeNodeToMap(Map,nBus,nStation,routeEnd,distance);
                ChangeRouteArray(nBus,nStation,routeEnd,distance);
                printf("\n已修改路线[%s]-[%s](站点距离：%d)为[%s]-[%s](站点距离：%d)\n",changeStation,Map.stations[routeEnd].station,oldDistance,changeStation,Map.stations[routeEnd].station,distance);
            }

            if(flag==1)
            {
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
            }
        }
        pRoute=pRoute->next;
    }
    return 1;
}

int ChangeStationAsEnd(BusMap &Map,int nStation)
{
    char *changeStation=Map.stations[nStation].station;
    int i,nBus,routeStart,routeDistance;
    char *bus;
    char *station=(char*)malloc(sizeof(char)*20);
    char *routeStation=(char*)malloc(sizeof(char)*20);//对应routeStart
    int newStation,flag;
    Station *pStation;
    Route *pRoute;
    int busVisit[Map.bus_num]= {0};
    for(i=0; i<Map.station_num; i++)
    {
        pStation=&Map.stations[i];
        pRoute=pStation->routes;
        while(pRoute!=NULL)
        {
            flag=0;
            nBus=pRoute->bus;
            bus=Map.buses[nBus].name;
            if(busVisit[nBus]==0 && (nBus==0 || nBus%2==0))
            {
                if(pRoute->station==nStation)
                {
                    routeStart=i;
                    routeStation=Map.stations[routeStart].station;
                    routeDistance=pRoute->distance;

                    printf("\n--------------------------------------\n");
                    printf("修改路线[%s] [%s]-[%s](站点距离:%d)\n",bus,routeStation,changeStation,routeDistance);
                    printf("修改起始站点[%s]为：",routeStation);
                    scanf("%s",station);
                    if(strcmp(station,"0")!=0)
                    {
                        flag=1;
                        newStation=FindStation(Map,station);
                        if(newStation==NONE)
                        {
                            printf("修改失败！站点[%s]不存在！请重新输入！\n",station);
                            continue;
                        }
                        if(FindStationInBus(Map,nBus,newStation)!=NONE)
                        {
                            printf("修改失败！-不可将站点修改为公交线路中已存在站点！请重新输入！\n");
                            continue;
                        }
                        //如果修改路线目标站点为公交线路起点站
                        if(Map.buses[nBus].start==routeStart)
                            ChangeRouteStartAsBusStart(Map,nBus,routeStart,newStation,nStation,routeDistance);
                        else
                        {
                            int preStation,preDistance;
                            preStation=FindPreStationInBus(Map,nBus,routeStart,preDistance);
                            ChangeRouteStart(Map,nBus,preStation,routeStart,preDistance,newStation,nStation,routeDistance);
                        }
                        //更新pStation pRoute routeStart
                        pStation=&Map.stations[newStation];
                        pRoute=pStation->routes;
                        while(pRoute!=NULL)
                        {
                            if(pRoute->bus==nBus)
                                break;
                            pRoute=pRoute->next;
                        }
                        routeStart=newStation;
                    }

                    int distance;
                    printf("\n修改站点距离[%d]为:",pRoute->distance);
                    scanf("%d",&distance);
                    if(distance!=0)
                    {
                        int oldDistance=pRoute->distance;
                        ChangeNodeToMap(Map,nBus,routeStart,nStation,distance);
                        ChangeRouteArray(nBus,routeStart,nStation,distance);
                        printf("\n已修改路线[%s]-[%s](站点距离：%d)为[%s]-[%s](站点距离：%d)\n",Map.stations[routeStart].station,changeStation,oldDistance,Map.stations[routeStart].station,changeStation,distance);
                    }

                    if(flag==1)
                    {
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
                    }

                    busVisit[nBus]=1;//标记已修改过在一条公交线路中相关路线
                }
            }
            pRoute=pRoute->next;
        }
    }
    return 1;
}
