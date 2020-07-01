#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

#include "model.h"
#include "LoadMapData.h"
#include "SearchBus.h"
#include "AddNewBus.h"
#include "AddNewRoute.h"
#include "RemoveRoute.h"
#include "RemoveBus.h"
#include "ChangeBus.h"
#include "ChangeRoute.h"//ChangeRouteArray

int RemoveRoute(BusMap &Map)
{
    system("cls");
    printf("\n===== 删除路线信息 =====\n");
    printf("提示：1.只需删除上行公交线路路线，下行公交线路将自动更改。\n");
    printf("      2.删除路线起始站点必须位于所选公交线路中目标站点之前\n");
    printf("      3.可删除所选公交线路中多个连续路线\n");
    printf("      4.若删除所选公交线路起点站或终点站相关路线，将更改所选公交线路信息\n\n");

    int i,j;
    extern char **BUSES;
    printf("现有公交线路：");
    for(i=0,j=0; i<Map.bus_num-1; i++,j+=3)
        printf("%s、",BUSES[j]);
    printf("%s\n",BUSES[j]);

    printf("请输入你要删除的路线所在公交线路：");
    char *bus=(char*)malloc(sizeof(char)*10);
    scanf("%s",bus);
    int nBus=FindBus(Map,bus);
    if(nBus==NONE)
    {
        printf("[%s]公交线路不存在！请重新输入！\n",bus);
        return NONE;
    }

    printf("\n--------------------------------------\n");
    printf("[%s]公交线路：\n",bus);
    QueryBus(Map,bus);
    printf("--------------------------------------\n");

    printf("请输入你要删除的路线起始站点：");
    char *start=(char*)malloc(sizeof(char)*20);
    scanf("%s",start);
    int nStart=FindStation(Map,start);
    if(nStart==NONE)
    {
        printf("站点信息不存在！\n");
        return NONE;
    }
    if(FindStationInBus(Map,nBus,nStart)==NONE)
    {
        printf("站点[%s]不存在[%s]中\n",start,bus);
        return NONE;
    }

    printf("请输入你要删除的路线目标站点：");
    char *end=(char*)malloc(sizeof(char)*20);
    scanf("%s",end);
    int nEnd=FindStation(Map,end);
    if(nEnd==NONE)
    {
        printf("站点信息不存在！\n");
        return NONE;
    }
    if(FindStationInBus(Map,nBus,nEnd)==NONE)
    {
        printf("站点[%s]不存在[%s]中\n",end,bus);
        return NONE;
    }

    int flag=RemoveRouteToMap(Map,nBus,nStart,nEnd);
    if(flag==NONE)
        return NONE;
    else if(flag==2)//删除整条公交线路情况
    {
        printf("--------------------------------------\n\n");
        return 1;
    }

    printf("\n--------------------------------------\n");
    printf("成功删除[%s]公交线路 [%s]-[%s]路段\n\n",bus,start,end);
    printf("[%s]公交线路：\n",bus);
    QueryBus(Map,bus);
    printf("\n--------------------------------------\n");

    printf("自动更新下行路线\n");
    RemoveRouteToMap2(Map,nBus);
    printf("\n--------------------------------------\n");
    bus=Map.buses[nBus+1].name;
    printf("成功删除[%s]公交线路 [%s]-[%s]路段\n\n",bus,end,start);
    printf("[%s]公交线路：\n",bus);
    QueryBus(Map,bus);
    printf("\n--------------------------------------\n");

    printf("输入任何键退出[删除路线信息]功能\n");
    getch();
    system("cls");
    return 1;
}

int RemoveRouteToMap(BusMap &Map,int nBus,int nStart,int nEnd)
{
    int busStart=Map.buses[nBus].start;
    int busEnd=Map.buses[nBus].end;

    if(nStart==busStart && nEnd==busEnd)
    {
        printf("--------------------------------------\n");
        printf("删除公交路线[%s]\n",Map.buses[nBus].name);
        RemoveBusToMap(Map,nBus);
        return 2;
    }

    //删除路线数量并记录相应信息
    int routesNum=0;
    int STARTS[10],ENDS[10],DISTANCES[10];
    DISTANCES[0]=0;
    if(RemoveRoutesNum(Map,nBus,nStart,nEnd,STARTS,ENDS,DISTANCES,routesNum)==NONE)
        return NONE;

    printf("\n--------------------------------------\n");
    printf("已删除路线:\n");
    int i;
    char *bus,*start,*end;
    bus=Map.buses[nBus].name;

    if(nStart==busStart)//删除路线起始站点为公交线路起点站
    {
        for(i=1; i<=routesNum; i++)
        {
            start=Map.stations[STARTS[i]].station;
            end=Map.stations[ENDS[i]].station;
            printf("[%s]-[%s](路线距离：%d)\n",start,end,DISTANCES[i]);

            RemoveNodeToMap(Map,nBus,STARTS[i],ENDS[i]);
            RemoveRouteFromArray(nBus,STARTS[i],ENDS[i]);
            //修改公交起点站，确保遍历顺利
            Map.buses[nBus].start=ENDS[i];
        }
        //修改上行公交线路起点站
        STARTS[0]=ENDS[routesNum];
        Map.buses[nBus].start=STARTS[0];
        //修改下行公交线路终点站
        Map.buses[nBus+1].end=STARTS[0];

        start=Map.stations[STARTS[0]].station;
        end=Map.stations[busEnd].station;
        //修改BUSES数组
        ChangeBusArray(bus,start,end);
        bus=Map.buses[nBus+1].name;
        ChangeBusArray(bus,end,start);
    }
    else if(nEnd==busEnd)//删除路线目标站点为公交线路终点站
    {
        for(i=1; i<=routesNum; i++)
        {
            start=Map.stations[STARTS[i]].station;
            end=Map.stations[ENDS[i]].station;
            printf("[%s]-[%s](路线距离：%d)\n",start,end,DISTANCES[i]);
            //修改邻接表，注意修改STARTS[0]-ENDS[i]路线 确保公交路线不断开
            RemoveNodeToMap(Map,nBus,STARTS[i],ENDS[i]);
            ChangeNodeToMap(Map,nBus,STARTS[0],ENDS[i],0);

            RemoveRouteFromArray(nBus,STARTS[i],ENDS[i]);
        }
        //修改最后一条路线
        ChangeNodeToMap(Map,nBus,STARTS[0],STARTS[1],DISTANCES[0]);
        ChangeRouteArray(nBus,STARTS[0],STARTS[1],DISTANCES[0]);

        //修改上行公交线路终点站
        ENDS[0]=STARTS[1];
        Map.buses[nBus].end=ENDS[0];
        //修改下行公交线路起点站
        Map.buses[nBus].start=ENDS[0];

        start=Map.stations[busStart].station;
        end=Map.stations[ENDS[0]].station;
        //修改BUSES数组
        ChangeBusArray(bus,start,end);
        bus=Map.buses[nBus+1].name;
        ChangeBusArray(bus,end,start);
    }
    else
    {
        //删除路线
        for(i=1; i<=routesNum; i++)
        {
            start=Map.stations[STARTS[i]].station;
            end=Map.stations[ENDS[i]].station;
            printf("[%s]-[%s](路线距离：%d)\n",start,end,DISTANCES[i]);
            //修改邻接表，注意修改STARTS[0]-ENDS[i]路线 确保公交路线不断开
            RemoveNodeToMap(Map,nBus,STARTS[i],ENDS[i]);
            ChangeNodeToMap(Map,nBus,STARTS[0],ENDS[i],0);

            RemoveRouteFromArray(nBus,STARTS[i],ENDS[i]);
        }

        //最后调整整个邻接表和ROUTES数组
        start=Map.stations[STARTS[0]].station;
        end=Map.stations[ENDS[0]].station;
        printf("\n请输入[%s]-[%s]之间距离：",start,end);
        scanf("%d",&DISTANCES[0]);
        //删除最后一条多余路线
        RemoveNodeToMap(Map,nBus,ENDS[routesNum],ENDS[0]);
        RemoveRouteFromArray(nBus,ENDS[routesNum],ENDS[0]);
        //连接路线
        ChangeNodeToMap(Map,nBus,STARTS[0],ENDS[0],DISTANCES[0]);
        ChangeRouteArray(nBus,STARTS[0],ENDS[0],DISTANCES[0]);
    }
    return 1;
}

int RemoveRoutesNum(BusMap Map,int nBus,int nStart,int nEnd,int *STARTS,int *ENDS,int *DISTANCES,int &routesNum)
{
    int ArrayNum=10;
    int busStart=Map.buses[nBus].start;
    int busEnd=Map.buses[nBus].end;

    int i=busStart,j;
    Station *pStation;
    Route *pRoute;
    int flag=0;
    if(nStart==busStart)
        flag=1;

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

        if(flag==1)//计数过程
        {
            routesNum++;
            if(routesNum+1>ArrayNum)
            {
                ArrayNum+=10;
                STARTS=(int*)realloc(STARTS,sizeof(int)*ArrayNum);
                ENDS=(int*)realloc(ENDS,sizeof(int)*ArrayNum);
                DISTANCES=(int*)realloc(DISTANCES,sizeof(int)*ArrayNum);
            }
            STARTS[routesNum]=j;
            ENDS[routesNum]=i;
            DISTANCES[routesNum]=pRoute->distance;
        }
        if(flag==0 && i==nStart)
        {
            flag=1;//开始计数
            STARTS[0]=j;//相接起始站点
            DISTANCES[0]=pRoute->distance;
        }
        if(flag==0 && i==nEnd)
        {
            printf("路线不合法！目标站点位于起始站点之前\n");
            return NONE;
        }
        if(flag==1 && i==nEnd)
        {
            if(nEnd!=busEnd)
            {
                pStation=&Map.stations[i];
                pRoute=pStation->routes;
                while(pRoute!=NULL)
                {
                    if(pRoute->bus==nBus)
                    {
                        ENDS[0]=pRoute->station;//寻找相接目标站点
                        break;
                    }
                    pRoute=pRoute->next;
                }
            }
            break;//计数完成
        }
    }
    return 1;
}

void RemoveRouteToMap2(BusMap &Map,int nBus)
{
    extern int *ROUTES;
    extern int ROUTE_NUM;
    int i,j,k,num=0;
    int initialNum=20;
    int *routes=(int*)malloc(sizeof(int)*initialNum*4);//辅助数组 记录下行路线
    for(i=0,j=0,k=0; i<ROUTE_NUM; i++,j+=4)
    {
        if(ROUTES[j]==nBus)
        {
            num++;
            if(num>initialNum)
            {
                initialNum+=10;
                routes=(int*)realloc(routes,sizeof(int)*initialNum*4);
            }
            routes[k]=nBus+1;
            routes[k+1]=ROUTES[j+2];
            routes[k+2]=ROUTES[j+1];
            routes[k+3]=ROUTES[j+3];
            k+=4;
        }
        if(ROUTES[j]==nBus+1)
            break;
    }

    k-=4;
    int num2=0;
    for(i=0,j=0; i<ROUTE_NUM; i++,j+=4)
    {
        if(ROUTES[j]==nBus+1)
        {
            if(num>0)//粘贴routes数组
            {
                ROUTES[j]=routes[k];
                ROUTES[j+1]=routes[k+1];
                ROUTES[j+2]=routes[k+2];
                ROUTES[j+3]=routes[k+3];
                k-=4;
                num--;
            }
            else//记录待删除路线
            {
                num2++;
                k+=4;
                routes[k]=ROUTES[j];
                routes[k+1]=ROUTES[j+1];
                routes[k+2]=ROUTES[j+2];
                routes[k+3]=ROUTES[j+3];
            }
        }
        if(ROUTES[j]==nBus+2)
            break;
    }
    for(i=0,j=0;i<num2;i++,j+=4)
        RemoveRouteFromArray(nBus+1,routes[j+1],routes[j+2]);
    //更新邻接表
    LoadMapData(Map);
}

void RemoveNodeToMap(BusMap Map,int nBus,int nStart,int nEnd)
{
    //以nStart为线索修改路线
    Station *pStation=&Map.stations[nStart];
    Route *pRoute=pStation->routes;
    Route *preRoure=pRoute;
    while(pRoute!=NULL)
    {
        if(pRoute->bus==nBus && pRoute->station==nEnd)
            break;
        preRoure=pRoute;
        pRoute=pRoute->next;
    }
    if(preRoure==pRoute)
        pStation->routes=pRoute->next;
    else
        preRoure->next=pRoute->next;
    free(pRoute);
}

void RemoveRouteFromArray(int nBus,int nStart,int nEnd)
{
    extern int ROUTE_NUM;
    extern int *ROUTES;
    int i,j;
    for(i=0,j=0; i<ROUTE_NUM; i++,j+=4)
    {
        if(ROUTES[j]==nBus && ROUTES[j+1]==nStart && ROUTES[j+2]==nEnd)
            break;
    }
    for(; i<ROUTE_NUM; i++,j+=4)
    {
        ROUTES[j]=ROUTES[j+4];
        ROUTES[j+1]=ROUTES[j+5];
        ROUTES[j+2]=ROUTES[j+6];
        ROUTES[j+3]=ROUTES[j+7];
    }
    ROUTE_NUM--;
    ROUTES=(int*)realloc(ROUTES,sizeof(int)*ROUTE_NUM*4);
}
