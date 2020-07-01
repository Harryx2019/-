#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

#include "model.h"
#include "SearchStation.h"
#include "LoadMapData.h"


int SearchStation(BusMap Map)
{
    system("cls");
    char *station=(char *)malloc(sizeof(char)*20);
    int num=0;
    printf("\n===== 查询站点信息 =====\n");
    printf("请输入要查询的站点名：");
    scanf("%s",station);
    printf("\n--------------------------------------\n");
    num=QueryStation(Map,station);
    if(num==NONE)
        return NONE;
    printf("\n[%s站]  共%d辆车经过。\n\n",station,num);

    printf("--------------------------------------\n");
    printf("\n输入任何键退出[查询站点信息]功能\n");
    getch();
    system("cls");
    return 1;
}

//查询站点信息，输出该站点所经线路信息
int QueryStation(BusMap Map,char *pStation)
{
    int nStation=FindStation(Map,pStation);
    if(nStation==NONE)
    {
        printf("未找到[%s]相关信息！\n",pStation);
        return NONE;
    }
    int i,k=0;
    //1.遍历该站点邻接表找到所有从该站点驶出的车
    Station *pStStation=&Map.stations[nStation];
    Route *pStRoute=pStStation->routes;
    while(pStRoute!=NULL)
    {
        k++;
        printf("%s  [%s]-[%s] (站点距离：%d)\n",Map.buses[pStRoute->bus].name,pStation,Map.stations[pStRoute->station].station,pStRoute->distance);
        pStRoute=pStRoute->next;
    }
    //2.遍历该站点邻接表找到所有驶入该站点的车
    for(i=0;i<Map.station_num;i++)
    {
        pStStation=&Map.stations[i];
        pStRoute=pStStation->routes;
        while(pStRoute!=NULL)
        {
            if(pStRoute->station==nStation)
            {
                k++;
                printf("%s  [%s]-[%s] (站点距离：%d)\n",Map.buses[pStRoute->bus].name,pStStation->station,pStation,pStRoute->distance);
            }
            pStRoute=pStRoute->next;
        }
    }
    return k;
}
