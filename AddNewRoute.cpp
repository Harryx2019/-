#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>

#include "model.h"
#include "LoadMapData.h"
#include "SearchBus.h"
#include "AddNewRoute.h"
#include "ChangeBus.h"
#include "ChangeRoute.h"//ChangeRouteArray

int AddNewRoute(BusMap &Map)
{
    system("cls");
    char *bus=(char*)malloc(sizeof(char)*10);
    char *start=(char*)malloc(sizeof(char)*20);
    char *end=(char*)malloc(sizeof(char)*20);
    int nBus,nStart1,nEnd1,distance1;

    printf("\n===== 新增路线信息 =====\n");
    printf("提示：1.新增路线信息中所选公交线路必须为已存在公交线路，若需要新增公交线路，请先使用[4. 新增公交线路]\n");
    printf("      2.新增路线信息中两站点必须为公交管理系统中已存在站点，若需要新增站点，请先使用[5. 新增站点信息]\n");
    printf("      3.新增线路信息中两站点不可同时存在或不存在所选公交线路中\n");
    printf("      4.只需增加上行公交线路线路信息，下行公交线路信息会自动更改\n");
    printf("      5.若新增所选公交线路起点站或终点站相关路线，将更改所选公交线路信息\n\n");

    int i,j;
    extern char **BUSES;
    printf("现有公交线路：");
    for(i=0,j=0;i<Map.bus_num-1;i++,j+=3)
        printf("%s、",BUSES[j]);
    printf("%s\n",BUSES[j]);

    printf("请输入新增的路线所在公交线路：");
    scanf("%s",bus);
    nBus=FindBus(Map,bus);
    if(nBus==NONE)
    {
        printf("公交信息不存在！\n");
        return NONE;
    }

    printf("\n--------------------------------------\n");
    printf("[%s]公交线路：\n",bus);
    QueryBus(Map,bus);
    printf("--------------------------------------\n");

    printf("请输入新增路线起始站点：");
    scanf("%s",start);
    nStart1=FindStation(Map,start);
    if(nStart1==NONE)
    {
        printf("站点信息不存在！\n");
        return NONE;
    }

    printf("请输入新增路线目标站点：");
    scanf("%s",end);
    nEnd1=FindStation(Map,end);
    if(nEnd1==NONE)
    {
        printf("站点信息不存在！\n");
        return NONE;
    }

    printf("请输入两站点之间距离：");
    scanf("%d",&distance1);

    if(AddNewRouteToMap(Map,nBus,nStart1,nEnd1,distance1)==NONE)
        return NONE;

    printf("\n--------------------------------------\n\n");
    printf("输入任何键退出[新增路线信息]功能\n");
    getch();
    system("cls");
    return 1;
}

//添加公交系统线路信息公共接口
int AddNewRouteToMap(BusMap &Map,int nBus,int nStart1,int nEnd1,int distance1)
{
    //检测路线是否已存在
    Station *pStation=&Map.stations[nStart1];
    Route *pRoute=pStation->routes;
    while(pRoute!=NULL)
    {
        if(pRoute->bus==nBus && pRoute->station==nEnd1)
        {
            printf("该路线已存在！\n");
            return NONE;
        }
        pRoute=pRoute->next;
    }

    //检测路线类型
    int flag=JudgeRouteType(Map,nBus,nStart1,nEnd1);
    if(flag==NONE)
        return NONE;

    int distance2;
    switch(flag)
    {
    case 0:
        distance2=AddNewRouteFlag0(Map,nBus,nStart1,nEnd1,distance1,0);
        printf("\n--------------------------------------\n");
        printf("自动修改下行路线\n");
        AddNewRouteFlag3(Map,nBus+1,nEnd1,nStart1,distance1,distance2);
        break;
    case 1:
        AddNewRouteFlag12(Map,nBus,nStart1,nEnd1,distance1,flag);
        printf("\n--------------------------------------\n");
        printf("自动修改下行路线\n");
        AddNewRouteFlag12(Map,nBus+1,nEnd1,nStart1,distance1,2);
        break;
    case 2:
        AddNewRouteFlag12(Map,nBus,nStart1,nEnd1,distance1,flag);
        printf("\n--------------------------------------\n");
        printf("自动修改下行路线\n");
        AddNewRouteFlag12(Map,nBus+1,nEnd1,nStart1,distance1,1);
        break;
    case 3:
        distance2=AddNewRouteFlag3(Map,nBus,nStart1,nEnd1,distance1,0);
        printf("\n--------------------------------------\n");
        printf("自动修改下行路线\n");
        AddNewRouteFlag0(Map,nBus+1,nEnd1,nStart1,distance1,distance2);
        break;
    }
    return 1;
}

int JudgeRouteType(BusMap Map,int nBus,int nStart1,int nEnd1)
{
    //检测路线类型
    //flag=0: 起始站点存在公交线路中，目标站点不存在
    //flag=1: 目标站点为当前公交线路起点站
    //flag=2: 起始站点为当前公交线路终点站
    //flag=3: 起始站点不存在公交线路中，目标站点存在
    int flag=0;
    int flag1=FindStationInBus(Map,nBus,nStart1);
    int flag2=FindStationInBus(Map,nBus,nEnd1);
    char *bus=Map.buses[nBus].name;
    char *start=Map.stations[nStart1].station;
    char *end=Map.stations[nEnd1].station;
    if(flag1==1 && flag2 ==1)
    {
        printf("路线不合法！-[%s][%s]均已存在[%s]\n",start,end,bus);
        return NONE;
    }
    if(flag1==NONE && flag2 ==NONE)
    {
        printf("路线不合法！-[%s][%s]均不存在[%s]\n",start,end,bus);
        return NONE;
    }
    if((flag1==2 && flag2==3) || (flag1==3 && flag2==2))//起始站点和目标站点均为公交线路起点站或终点站
    {
        printf("路线不合法！-[%s][%s]均已存在[%s]\n",start,end,bus);
        return NONE;
    }
    if((flag1==1&&(flag2==2||flag2==3)) || (flag2==1&&(flag1==2||flag1==3)))//起始站点或目标站点为公交线路起点站或终点站，另一站点已存在
    {
        printf("路线不合法！-[%s][%s]均已存在[%s]\n",start,end,bus);
        return NONE;
    }
    if(flag1==3)
    {
        printf("修改[%s]为[%s]终点站\n",end,bus);
        flag=2;
    }
    if(flag2==2)
    {
        printf("修改[%s]为[%s]起点站\n",start,bus);
        flag=1;
    }
    if(flag==0 && flag1==NONE)
        flag=3;

    return flag;
}

int AddNewRouteFlag0(BusMap Map,int nBus,int nStart1,int nEnd1,int distance1,int distance2)
{
    char *bus=Map.buses[nBus].name;
    char *start=Map.stations[nStart1].station;
    char *end=Map.stations[nEnd1].station;
    //修改nStart1的邻接表节点
    int nStart2,nEnd2;
    Station *pStation=&Map.stations[nStart1];
    Route *pRoute=pStation->routes;
    while(pRoute!=NULL)
    {
        if(pRoute->bus==nBus)
        {
            nStart2=nEnd1;
            nEnd2=pRoute->station;
            pRoute->station=nEnd1;
            pRoute->distance=distance1;
            break;
        }
        pRoute=pRoute->next;
    }

    //增加nStart2的邻接表节点
    if(distance2==0)//修改上行路线判断 0：上行路线
    {
        printf("请输入 [%s] 到 [%s] 之间距离：",Map.stations[nStart2].station,Map.stations[nEnd2].station);
        scanf("%d",&distance2);
    }

    AddNewNodeToMap(Map,nBus,nStart2,nEnd2,distance2);

    //修改ROUTES数组
    ChangeRouteArray(nBus,nStart1,nEnd1,distance1);
    AddNewRouteToArray(nBus,nStart2,nEnd2,distance2);

    printf("\n--------------------------------------\n");
    printf("已添加路线：%s [%s]-[%s] (站点距离：%d)\n",bus,start,end,distance1);
    printf("               [%s]-[%s] (站点距离：%d)\n",end,Map.stations[nEnd2].station,distance2);
    return distance2;
}

int AddNewRouteFlag3(BusMap Map,int nBus,int nStart1,int nEnd1,int distance1,int distance2)
{
    char *bus=Map.buses[nBus].name;
    char *start=Map.stations[nStart1].station;
    char *end=Map.stations[nEnd1].station;
    Station *pStation;
    Route *pRoute;

    //寻找公交线路中指向nEnd1的站点nStatr2
    int i=Map.buses[nBus].start,j,k=Map.buses[nBus].end;
    int nStart2,nEnd2;
    while(i!=k)
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
        if(i==nEnd1)
            break;
    }
    nStart2=j;
    nEnd2=nStart1;

    //修改nStart2的邻接表节点
    if(distance2==0)
    {
        printf("请输入 [%s] 到 [%s] 之间距离：",Map.stations[nStart2].station,Map.stations[nEnd2].station);
        scanf("%d",&distance2);
    }

    pStation=&Map.stations[nStart2];
    pRoute=pStation->routes;
    while(pRoute!=NULL)
    {
        if(pRoute->bus==nBus)
        {
            pRoute->station=nEnd2;
            pRoute->distance=distance2;
            break;
        }
        pRoute=pRoute->next;
    }

    //增加nStart1的邻接表节点
    AddNewNodeToMap(Map,nBus,nStart1,nEnd1,distance1);

    //修改ROUTES数组
    ChangeRouteArray(nBus,nStart2,nEnd2,distance2);
    AddNewRouteToArray(nBus,nStart1,nEnd1,distance1);


    printf("\n--------------------------------------\n");
    printf("已添加路线：%s [%s]-[%s] (站点距离：%d)\n",bus,Map.stations[nStart2].station,start,distance2);
    printf("               [%s]-[%s] (站点距离：%d)\n",start,end,distance1);
    return distance2;
}

void AddNewRouteFlag12(BusMap &Map,int nBus,int nStart,int nEnd,int distance,int flag)
{
    char *Bus=Map.buses[nBus].name;
    char *Start=Map.stations[nStart].station;
    char *End=Map.stations[nEnd].station;

    char *busStart=Map.stations[Map.buses[nBus].start].station;
    char *busEnd=Map.stations[Map.buses[nBus].end].station;

    //修改当前公交线路起点站
    if(flag==1)
    {
        Map.buses[nBus].start=nStart;
        ChangeBusArray(Bus,Start,busEnd);//修改buses.txt文件
    }
    //修改当前公交线路终点站
    else if(flag==2)
    {
        Map.buses[nBus].end=nEnd;
        ChangeBusArray(Bus,busStart,End);//修改buses.txt文件
    }

    //新增邻接表节点
    AddNewNodeToMap(Map,nBus,nStart,nEnd,distance);

    //添加routes.txt文件信息
    AddNewRouteToArray(nBus,nStart,nEnd,distance);

    printf("\n--------------------------------------\n");
    printf("已添加路线：%s [%s]-[%s] (站点距离：%d)\n",Bus,Start,End,distance);
}

//添加路段到相关位置
void AddNewRouteToArray(int nBus,int nStart,int nEnd,int nDistance)
{
    extern int *ROUTES;
    extern int ROUTE_NUM;
    ROUTES=(int*)realloc(ROUTES,sizeof(int)*(ROUTE_NUM+1)*4);
    int i,j;
    //寻找插入位置
    for(i=0,j=0; i<ROUTE_NUM; i++,j+=4)
    {
        if(ROUTES[j]==nBus && ROUTES[j+1]==nEnd)
        {
            //目标站点为当前公交线路起点站
            i--;
            break;
        }
        if(ROUTES[j]==nBus && ROUTES[j+2]==nStart)
            break;
    }
    //反向连接数组
    int n=ROUTE_NUM*4,k;
    for(k=ROUTE_NUM-1,j=k*4; k>i; k--,j-=4)
    {
        ROUTES[n]=ROUTES[j];
        ROUTES[n+1]=ROUTES[j+1];
        ROUTES[n+2]=ROUTES[j+2];
        ROUTES[n+3]=ROUTES[j+3];
        n-=4;
    }
    ROUTES[n]=nBus;
    ROUTES[n+1]=nStart;
    ROUTES[n+2]=nEnd;
    ROUTES[n+3]=nDistance;
    ROUTE_NUM++;
}
