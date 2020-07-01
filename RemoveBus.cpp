#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>

#include "model.h"
#include "LoadMapData.h"
#include "SearchBus.h"
#include "RemoveBus.h"
#include "RemoveRoute.h"

int RemoveBus(BusMap &Map)
{
    system("cls");
    printf("\n===== 删除公交线路 =====\n");
    printf("提示： 只需删除上行公交线路，下行公交线路将自动移除\n\n");

    int i,j;
    extern char **BUSES;
    printf("现有公交线路：");
    for(i=0,j=0; i<Map.bus_num-1; i++,j+=3)
        printf("%s、",BUSES[j]);
    printf("%s\n\n",BUSES[j]);

    char *removeBus=(char*)malloc(sizeof(char)*10);
    printf("请输入你要删除的公交线路：");
    scanf("%s",removeBus);
    int nBus=FindBus(Map,removeBus);
    if(nBus==NONE)
    {
        printf("[%s]公交线路不存在！\n",removeBus);
        return NONE;
    }

    RemoveBusToMap(Map,nBus);
    printf("\n--------------------------------------\n");
    printf("输入任何键退出[删除公交线路]功能\n");
    getch();
    system("cls");
    return 1;
}

void RemoveBusToMap(BusMap &Map,int nBus)
{
    int busStart=Map.buses[nBus].start;
    int busEnd=Map.buses[nBus].end;

    //删除公交线路的所有路线
    int initialNum=20,num=0;
    int *routes=(int*)malloc(sizeof(int)*initialNum*2);//记录所有路线

    printf("\n--------------------------------------\n");
    printf("删除[%s]所有路线：\n",Map.buses[nBus].name);
    Station *pStation;
    Route *pRoute;
    int i=busStart,j,k=0;
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
        num++;
        //记录路线
        if(num-1>initialNum)
        {
            initialNum+=20;
            routes=(int*)realloc(routes,sizeof(int)*initialNum*2);
        }
        routes[k]=j;
        routes[k+1]=i;
        k+=2;

        printf("[%s]-[%s](站点距离：%d)\n",Map.stations[j].station,Map.stations[i].station,pRoute->distance);
        RemoveNodeToMap(Map,nBus,j,i);
        RemoveRouteFromArray(nBus,j,i);
        Map.buses[nBus].start=i;
    }

    printf("\n--------------------------------------\n");
    printf("自动删除[%s]所有路线\n",Map.buses[nBus+1].name);
    printf("\n--------------------------------------\n");
    for(i=num-1,j=i*2;i>=0;i--,j-=2)
    {
        RemoveNodeToMap(Map,nBus+1,routes[j+1],routes[j]);
        RemoveRouteFromArray(nBus+1,routes[j+1],routes[j]);
        Map.buses[nBus+1].start=routes[j];
    }
    //修改BUSES数组
    char *bus=Map.buses[nBus].name;
    RemoveBusFromArray(Map,Map.buses[nBus].name);
    printf("已成功删除[%s]所有信息！\n",bus);

    //删除下行路线BUSES数组信息
    bus=Map.buses[nBus].name;
    RemoveBusFromArray(Map,Map.buses[nBus].name);
    printf("已成功删除[%s]所有信息！\n",bus);
}

void RemoveBusFromArray(BusMap &Map,char *removeBus)
{
    int nBus=FindBus(Map,removeBus);

    //修改BUSES数组
    int i,j;
    extern char **BUSES;
    extern int BUS_NUM;
    for(i=nBus,j=i*3;i<BUS_NUM-1;i++,j+=3)
    {
        strcpy(BUSES[j],BUSES[j+3]);
        strcpy(BUSES[j+1],BUSES[j+4]);
        strcpy(BUSES[j+2],BUSES[j+5]);
    }
    BUS_NUM--;
    Map.bus_num=BUS_NUM;
    BUSES=(char**)realloc(BUSES,sizeof(char*)*BUS_NUM*3);

    //修改ROUTES数组下标
    extern int ROUTE_NUM;
    extern int *ROUTES;
    for(i=0,j=0;i<ROUTE_NUM;i++,j+=4)
    {
        if(ROUTES[j]>nBus)
            ROUTES[j]--;
    }

    //更新邻接表
    LoadMapData(Map);
}
