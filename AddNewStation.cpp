#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>

#include "model.h"
#include "LoadMapData.h"
#include "SearchBus.h"
#include "AddNewRoute.h"
#include "AddNewStation.h"

int AddNewStation(BusMap &Map)
{
    system("cls");
    printf("\n===== 新增站点信息 =====\n");
    printf("提示：1.新增站点信息中公交线路必须为已存在公交线路，若需要新增公交线路，请先使用[4. 新增公交线路]\n");
    printf("      2.设置新增站点相关路线信息，站点要求详见[6. 新增路线信息]\n");

    //输入站点名称
    char *newStation=(char*)malloc(sizeof(char)*20);
    printf("请输入新站点名称：");
    scanf("%s",newStation);
    //检测站点是否已存在
    if(FindStation(Map,newStation)!=NONE)
    {
        printf("此站点已存在！\n");
        return NONE;
    }
    AddNewStationToMap(Map,newStation);

    //输入该站点相关路线信息
    int i=1;
    char *bus=(char*)malloc(sizeof(char)*10);
    char *start=(char*)malloc(sizeof(char)*20);
    char *end=(char*)malloc(sizeof(char)*20);
    int nBus,nStart,nEnd,distance;
    printf("\n--------------------------------------\n");
    printf("请输入[%s]路线信息\n",newStation);
    printf("提示: 输入公交路线为 0 为信息输入完毕\n");
    while(1)
    {
        printf("\n--------------------------------------\n");
        printf("\n请输入[%s]路线信息%d:\n",newStation,i);
        printf("公交线路：");
        scanf("%s",bus);
        if(strcmp(bus,"0")==0)
            break;
        nBus=FindBus(Map,bus);
        if(nBus==NONE)
        {
            printf("该公交路线不存在！请重新输入！\n");
            continue;
        }
        printf("--------------------------------------\n");
        printf("[%s]公交线路：\n",bus);
        QueryBus(Map,bus);
        printf("--------------------------------------\n");

        printf("出发站点：");
        scanf("%s",start);
        nStart=FindStation(Map,start);
        if(nStart==NONE)
        {
            printf("该站点不存在！请重新输入！\n");
            continue;
        }

        printf("目的站点：");
        scanf("%s",end);
        nEnd=FindStation(Map,end);
        if(nEnd==NONE)
        {
            printf("该站点不存在！请重新输入！\n");
            continue;
        }

        printf("请输入两站点之间距离：");
        scanf("%d",&distance);

        if(AddNewRouteToMap(Map,nBus,nStart,nEnd,distance)==NONE)
            continue;
        i++;
    }
    printf("\n--------------------------------------\n\n");
    printf("输入任何键退出[新增站点信息]功能\n");
    getch();
    system("cls");

    return 1;
}

//添加公交系统站点信息公共接口
//注意：此处&Map是因为此处对Map中内容进行了修改
void AddNewStationToMap(BusMap &Map,char *newStation)
{
    extern int STATION_NUM;
    int nStation=STATION_NUM;
    STATION_NUM++;

    extern char **STATIONS;
    STATIONS=(char**)realloc(STATIONS,sizeof(char*)*STATION_NUM);
    //将站点信息加入STATIONS数组
    STATIONS[nStation]=newStation;

    //将站点信息加入到邻接表中
    Map.station_num++;
    Map.stations=(Station*)realloc(Map.stations,sizeof(Station)*STATION_NUM);
    Map.stations[nStation].station=newStation;
    Map.stations[nStation].routes=NULL;
}
