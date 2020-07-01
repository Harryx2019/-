#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

#include "model.h"
#include "LoadMapData.h"
#include "SearchBus.h"

int SearchBus(BusMap Map)
{
    system("cls");
    char *bus=(char *)malloc(sizeof(char));
    printf("\n===== 查询公交信息 =====\n");
    int i,j;
    extern char **BUSES;
    printf("可查询公交线路：");
    for(i=0,j=0;i<Map.bus_num-1;i++,j+=3)
        printf("%s、",BUSES[j]);
    printf("%s\n",BUSES[j]);

    printf("请输入要查询的公交名：");
    scanf("%s",bus);
    printf("\n--------------------------------------\n");
    if(QueryBus(Map,bus)==NONE)
        return NONE;

    printf("\n--------------------------------------\n");
    printf("输入任何键退出[查询公交信息]功能\n");
    getch();
    system("cls");
    return 0;
}

int QueryBus(BusMap Map,char *pBus)
{
    //1.查找公交
    int nBus=FindBus(Map,pBus);
    if(nBus==NONE)
    {
        printf("未找到[%s]相关公交信息！\n",pBus);
        return NONE;
    }
    //2.找到公交及其信息
    int nStart=Map.buses[nBus].start;
    int nEnd=Map.buses[nBus].end;
    //3.计算站点数并输出各站点
    int i=nStart,num=0;
    Station *pStation=&Map.stations[i];
    Route *pStRoute=pStation->routes;
    printf("(%d)%s",i,pStation->station);//输出起点站
    while(pStRoute!=NULL)
    {
        if(pStRoute->bus==nBus)
        {
            num++;
            i=pStRoute->station;
        }
        pStRoute=pStRoute->next;
    }
    while(i!=nEnd)
    {
        pStation=&Map.stations[i];
        pStRoute=pStation->routes;
        printf("->(%d)%s",i,pStation->station);
        while(pStRoute!=NULL)
        {
            if(pStRoute->bus==nBus)
            {
                num++;
                i=pStRoute->station;
                break;
            }
            pStRoute=pStRoute->next;
        }
    }
    //终点站
    pStation=&Map.stations[i];
    printf("->(%d)%s",i,pStation->station);
    //4.输出起始站点
    printf("\n\n[%s线路]  从:[%s] 开往 [%s],  共经过%d个站点。\n\n",pBus,Map.stations[nStart].station,Map.stations[nEnd].station,num+1);

    return 0;
}
