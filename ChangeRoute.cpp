#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>

#include "model.h"
#include "LoadMapData.h"
#include "SearchBus.h"
#include "AddNewRoute.h"
#include "RemoveRoute.h"
#include "ChangeBus.h"
#include "ChangeRoute.h"

int ChangeRoute(BusMap &Map)
{
    system("cls");
    printf("\n===== 修改路线信息 =====\n");
    printf("提示：1.修改路线信息不可更改选定公交线路路线数目\n");
    printf("      2.修改路线信息可以修改选定公交线路多条路线,逐个修改\n");
    printf("      3.修改路线操作中,系统会提示是否修改相关信息,输入'0'视为不修改\n\n");

    int i,j;
    extern char **BUSES;
    printf("现有公交线路：");
    for(i=0,j=0; i<Map.bus_num-1; i++,j+=3)
        printf("%s、",BUSES[j]);
    printf("%s\n\n",BUSES[j]);

    char *changeBus=(char*)malloc(sizeof(char)*10);
    printf("请输入你要修改的公交线路:");
    scanf("%s",changeBus);
    int nBus=FindBus(Map,changeBus);
    if(nBus==NONE)
    {
        printf("[%s]公交线路不存在！\n",changeBus);
        return NONE;
    }

    printf("\n--------------------------------------\n");
    printf("[%s]公交线路：\n",changeBus);
    QueryBus(Map,changeBus);
    printf("--------------------------------------\n");

    char *changeStart=(char*)malloc(sizeof(char)*20);
    printf("请输入你要修改的路线起始站点：");
    scanf("%s",changeStart);
    int nStart=FindStation(Map,changeStart);
    if(nStart==NONE)
    {
        printf("[%s]站点信息不存！\n",changeStart);
        return NONE;
    }
    if(FindStationInBus(Map,nBus,nStart)==NONE)
    {
        printf("站点[%s]不存在[%s]中\n",changeStart,changeBus);
        return NONE;
    }

    char *changeEnd=(char*)malloc(sizeof(char)*20);
    printf("请输入你要修改的路线目标站点：");
    scanf("%s",changeEnd);
    int nEnd=FindStation(Map,changeEnd);
    if(nEnd==NONE)
    {
        printf("[%s]站点信息不存！\n",changeEnd);
        return NONE;
    }
    if(FindStationInBus(Map,nBus,nEnd)==NONE)
    {
        printf("站点[%s]不存在[%s]中\n",changeEnd,changeBus);
        return NONE;
    }

    if(ChangeRouteToMap(Map,nBus,nStart,nEnd)==NONE)
        return NONE;

    printf("\n--------------------------------------\n");
    printf("成功修改[%s]公交线路 [%s]-[%s]路段\n\n",changeBus,changeStart,changeEnd);
    printf("[%s]公交线路：\n",changeBus);
    QueryBus(Map,changeBus);
    printf("\n--------------------------------------\n");
    printf("自动更新下行路线\n");
    RemoveRouteToMap2(Map,nBus);
    changeBus=Map.buses[nBus+1].name;
    printf("--------------------------------------\n");
    printf("成功修改[%s]公交线路 [%s]-[%s]路段\n\n",changeBus,changeEnd,changeStart);
    printf("[%s]公交线路：\n",changeBus);
    QueryBus(Map,changeBus);
    printf("\n--------------------------------------\n");

    printf("\n输入任何键退出[修改路线信息]功能\n");
    getch();
    system("cls");

    return 1;
}

int ChangeRouteToMap(BusMap &Map,int nBus,int nStart,int nEnd)
{
    int busStart=Map.buses[nBus].start;
    int busEnd=Map.buses[nBus].end;
    //辅助数组
    int routes[20*3];
    int changeNum=ChangeRouteNum(Map,nBus,nStart,nEnd,routes);

    //修改路线
    char *start,*end;
    char *station=(char*)malloc(sizeof(char)*20);
    int i,k,nStation,distance;
    int flag;//站点是否在公交路线中标志
    for(i=1,k=3; i<=changeNum; i++,k+=3)
    {
        start=Map.stations[routes[k]].station;
        end=Map.stations[routes[k+1]].station;
        distance=routes[k+2];
        printf("\n--------------------------------------\n");
        printf("修改路线： [%s]-[%s] (站点距离：%d)\n",start,end,distance);
        printf("\n修改起始站点[%s]为:",start);
        scanf("%s",station);
        if(strcmp(station,"0")!=0)
        {
            nStation=FindStation(Map,station);
            if(nStation==NONE)
            {
                printf("修改失败！站点[%s]不存在!请重新输入！\n",station);
                i--;
                k-=3;
                continue;
            }
            flag=FindStationInBus(Map,nBus,nStation);
            if(flag!=NONE)
            {
                printf("修改失败！-不可将站点修改为已存在于公交线路中的站点！请重新输入！\n");
                i--;
                k-=3;
                continue;
            }
            //如果修改路线起始站点为公交线路起点站
            if(routes[k]==busStart)
            {
                ChangeRouteStartAsBusStart(Map,nBus,routes[k],nStation,routes[k+1],routes[k+2]);
                routes[k]=nStation;
            }
            else
            {
                ChangeRouteStart(Map,nBus,routes[k-3],routes[k],routes[k-1],nStation,routes[k+1],routes[k+2]);
                routes[k-3]=nStation;
                routes[k]=nStation;
            }
            start=Map.stations[nStation].station;
        }

        printf("\n--------------------------------------\n");
        printf("修改目标站点[%s]为:",end);
        scanf("%s",station);
        if(strcmp(station,"0")!=0)
        {
            nStation=FindStation(Map,station);
            if(nStation==NONE)
            {
                printf("修改失败！站点[%s]不存在!请重新输入！\n",station);
                i--;
                k-=3;
                continue;
            }
            flag=FindStationInBus(Map,nBus,nStation);
            if(flag!=NONE)
            {
                printf("修改失败！-不可将站点修改为已存在于公交线路中的站点！请重新输入！\n");
                i--;
                k-=3;
                continue;
            }
            //如果修改路线目标站点为公交线路终点站
            if(routes[k+1]==busEnd)
            {
                ChangeRouteEndAsBusEnd(Map,nBus,routes[k+1],routes[k],nStation,routes[k+2]);
                routes[k+1]=nStation;
            }
            else//修改邻接表结构
            {
                ChangeRouteEnd(Map,nBus,routes[k+4],routes[k+1],routes[k+5],routes[k],nStation,routes[k+2]);
                routes[k+1]=nStation;
                routes[k+3]=nStation;
            }
            end=Map.stations[nStation].station;
        }

        printf("\n--------------------------------------\n");
        printf("修改站点距离[%d]为:",routes[k+2]);
        scanf("%d",&distance);
        if(distance!=0)
        {
            ChangeNodeToMap(Map,nBus,routes[k],routes[k+1],distance);
            ChangeRouteArray(nBus,routes[k],routes[k+1],distance);
            printf("\n已修改路线[%s]-[%s](站点距离：%d)为[%s]-[%s](站点距离：%d)\n",start,end,routes[k+2],start,end,distance);
            routes[k+2]=distance;
        }
    }
    return 1;
}

int ChangeRouteNum(BusMap &Map,int nBus,int nStart,int nEnd,int *routes)
{
    int busStart=Map.buses[nBus].start;
    int busEnd=Map.buses[nBus].end;
    int changeNum=0,k=0,initialNum=20;//记录修改路线
    //遍历公交线路,记录需要修改的路线
    int i=busStart,j;
    Station *pStation;
    Route *pRoute;
    int flag=0;//开始计数标志
    if(nStart==busStart)//修改路线为公交线路起点站
    {
        routes[0]=routes[1]=routes[2]=-1;
        k+=3;
        flag=1;
    }
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
        if(flag==1)
        {
            changeNum++;
            if(changeNum+1==initialNum)//保证多出一个数组空间
            {
                initialNum+=10;
                routes=(int*)realloc(routes,sizeof(int)*initialNum*3);
            }
            routes[k]=j;
            routes[k+1]=i;
            routes[k+2]=pRoute->distance;
            k+=3;
        }
        if(i==nStart && flag==0)
        {
            routes[0]=j;
            routes[1]=i;
            routes[2]=pRoute->distance;
            k+=3;
            flag=1;//开始计数
        }
        if(i==nEnd)
        {
            if(nEnd==busEnd)
            {
                routes[k]=routes[k+1]=routes[k+2]=-1;
                break;
            }
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
            routes[k]=j;
            routes[k+1]=i;
            routes[k+2]=pRoute->distance;
            break;
        }
    }
    return changeNum;
}

//修改路线起始站点并且该站点为公交线路起点站 公共接口
void ChangeRouteStartAsBusStart(BusMap &Map,int nBus,int changeStart,int routeStart,int routeEnd,int routeDistance)
{
    char *bus=Map.buses[nBus].name;
    char *BusEnd=Map.stations[Map.buses[nBus].end].station;

    char *station=Map.stations[routeStart].station;
    char *start=Map.stations[changeStart].station;
    char *end=Map.stations[routeEnd].station;
    //修改上行公交线路
    Map.buses[nBus].start=routeStart;
    ChangeBusArray(bus,station,BusEnd);
    //修改下行公交线路
    bus=Map.buses[nBus+1].name;
    Map.buses[nBus+1].end=routeStart;
    ChangeBusArray(bus,BusEnd,station);

    //修改邻接表结构
    RemoveNodeToMap(Map,nBus,changeStart,routeEnd);
    RemoveRouteFromArray(nBus,changeStart,routeEnd);
    printf("\n已移除路线[%s]-[%s](站点距离：%d)\n",start,end,routeDistance);
    AddNewNodeToMap(Map,nBus,routeStart,routeEnd,routeDistance);
    AddNewRouteToArray(nBus,routeStart,routeEnd,routeDistance);
    printf("已增加路线[%s]-[%s](站点距离：%d)\n",station,end,routeDistance);
}

//修改路线目标站点并且该站点为公交线路终点站 公共接口
void ChangeRouteEndAsBusEnd(BusMap &Map,int nBus,int changeEnd,int routeStart,int routeEnd,int routeDistance)
{
    char *bus=Map.buses[nBus].name;
    char *BusStart=Map.stations[Map.buses[nBus].start].station;

    char *station=Map.stations[routeEnd].station;
    char *start=Map.stations[routeStart].station;
    char *end=Map.stations[changeEnd].station;
    //修改上行公交线路
    Map.buses[nBus].end=routeEnd;
    ChangeBusArray(bus,BusStart,station);
    //修改下行公交线路
    bus=Map.buses[nBus+1].name;
    Map.buses[nBus+1].start=routeEnd;
    ChangeBusArray(bus,station,BusStart);

    //修改邻接表结构
    RemoveNodeToMap(Map,nBus,routeStart,changeEnd);
    RemoveRouteFromArray(nBus,routeStart,changeEnd);
    printf("\n已移除路线[%s]-[%s](站点距离：%d)\n",start,end,routeDistance);
    AddNewNodeToMap(Map,nBus,routeStart,routeEnd,routeDistance);
    AddNewRouteToArray(nBus,routeStart,routeEnd,routeDistance);
    printf("已增加路线[%s]-[%s](站点距离：%d)\n",start,station,routeDistance);
}

//修改路线起始站点公共接口
void ChangeRouteStart(BusMap &Map,int nBus,int preStation,int changeStart,int preDistance,int newStart,int routeEnd,int routeDistance)
{
    char *station=Map.stations[newStart].station;
    char *start=Map.stations[changeStart].station;
    char *end=Map.stations[routeEnd].station;
    int distance;
    printf("请输入[%s]-[%s]之间距离：",Map.stations[preStation].station,station);
    scanf("%d",&distance);
    ChangeNodeToMap(Map,nBus,preStation,newStart,distance);
    ChangeRouteArray(nBus,preStation,newStart,distance);
    printf("\n已修改路线[%s]-[%s](站点距离：%d)为[%s]-[%s](站点距离：%d)\n",Map.stations[preStation].station,start,preDistance,Map.stations[preStation].station,station,distance);
    //移除routes[k]-routes[k+1]
    RemoveNodeToMap(Map,nBus,changeStart,routeEnd);
    RemoveRouteFromArray(nBus,changeStart,routeEnd);
    printf("已移除路线[%s]-[%s](站点距离：%d)\n",start,end,routeDistance);
    //增加nStation-routes[k+1]
    AddNewNodeToMap(Map,nBus,newStart,routeEnd,routeDistance);
    AddNewRouteToArray(nBus,newStart,routeEnd,routeDistance);
    printf("已增加路线[%s]-[%s](站点距离：%d)\n",station,end,routeDistance);
}

//修改路线目标站点公共接口
void ChangeRouteEnd(BusMap &Map,int nBus,int postStation,int changeEnd,int postDistance,int routeStart,int newEnd,int routeDistance)
{
    char *station=Map.stations[newEnd].station;
    char *start=Map.stations[routeStart].station;
    char *end=Map.stations[changeEnd].station;
    ChangeNodeToMap(Map,nBus,routeStart,newEnd,routeDistance);
    ChangeRouteArray(nBus,routeStart,newEnd,routeDistance);
    printf("\n已修改路线[%s]-[%s](站点距离：%d)为[%s]-[%s](站点距离：%d)\n",start,end,routeDistance,start,station,routeDistance);
    //移除routes[k+1]-routes[k+4]
    RemoveNodeToMap(Map,nBus,changeEnd,postStation);
    RemoveRouteFromArray(nBus,changeEnd,postStation);
    printf("已移除路线[%s]-[%s](站点距离：%d)\n",end,Map.stations[postStation].station,postDistance);
    //增加nStation-routes[k+4]
    int distance;
    printf("请输入[%s]-[%s]之间距离：",station,Map.stations[postStation].station);
    scanf("%d",&distance);
    AddNewNodeToMap(Map,nBus,newEnd,postStation,distance);
    AddNewRouteToArray(nBus,newEnd,postStation,distance);
    printf("已增加路线[%s]-[%s](站点距离：%d)\n",station,Map.stations[postStation].station,distance);
}

void ChangeNodeToMap(BusMap &Map,int nBus,int nStart,int changeEnd,int changeDistance)
{
    //以nStart为线索修改路线
    Station *pStation=&Map.stations[nStart];
    Route *pRoute=pStation->routes;
    while(pRoute!=NULL)
    {
        if(pRoute->bus==nBus)
        {
            pRoute->station=changeEnd;
            pRoute->distance=changeDistance;
            break;
        }
        pRoute=pRoute->next;
    }
}

//修改ROUTES数组
void ChangeRouteArray(int nBus,int nStart,int changeEnd,int changeDistance)
{
    //以nStart为线索修改
    extern int *ROUTES;
    extern int ROUTE_NUM;
    int i,j;
    for(i=0,j=0; i<ROUTE_NUM; i++,j+=4)
    {
        if(ROUTES[j]==nBus && ROUTES[j+1]==nStart)
        {
            ROUTES[j+2]=changeEnd;
            ROUTES[j+3]=changeDistance;
            break;
        }
    }
}
