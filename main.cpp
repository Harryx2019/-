#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

#include "model.h"
#include "HandleFile.h"
#include "LoadMapData.h"
#include "SearchBus.h"
#include "SearchStation.h"
#include "SearchRoute.h"
#include "AddNewBus.h"
#include "AddNewStation.h"
#include "AddNewRoute.h"
#include "RemoveBus.h"
#include "RemoveStation.h"
#include "RemoveRoute.h"
#include "ChangeBus.h"
#include "ChangeStation.h"
#include "ChangeRoute.h"
#include "ResetSystem.h"

#define bRunning 1

//定义三个全局变量，为后续增删减改
int BUS_NUM=0;
int STATION_NUM=0;
int ROUTE_NUM=0;
//定义三个全局数组，存储和修改
char **BUSES;//公交数组
char **STATIONS;//站点数组
int *ROUTES;//路段数组

void Case0();

int main()
{
    int n,flag;
    BusMap Map;
    ReadDataFromFile();//将buses.txt,stations.txt,routes.txt三个文本内容载入数组中
    LoadMapData(Map);//将数组信息载入邻接表中

    while(bRunning)
    {
        flag=0;
        //输出界面
        printf("=============== 公交管理系统 ===============\n");
        printf("-------------1. 查询公交线路----------------\n");
        printf("-------------2. 查询站点信息----------------\n");
        printf("-------------3. 查询两个站点公交线路--------\n");
        printf("-------------4. 新增公交线路----------------\n");
        printf("-------------5. 新增站点信息----------------\n");
        printf("-------------6. 新增路段信息----------------\n");
        printf("-------------7. 删除公交线路----------------\n");
        printf("-------------8. 删除站点信息----------------\n");
        printf("-------------9. 删除路段信息----------------\n");
        printf("-------------10.修改公交线路----------------\n");
        printf("-------------11.修改站点信息----------------\n");
        printf("-------------12.修改路段信息----------------\n");
        printf("-------------13.保存公交管理系统------------\n");
        printf("-------------14.重置公交管理系统------------\n");
        printf("-------------0. 退出公交管理系统------------\n");
        printf("============================================\n");
        printf("\n请输入操作编号（0~13):");

        scanf("%d",&n);
        switch(n)
        {
        case 1:flag=SearchBus(Map);break;
        case 2:flag=SearchStation(Map);break;
        case 3: flag=SearchRoute(Map); break;
        case 4: flag=AddNewBus(Map);break;
        case 5: flag=AddNewStation(Map);break;
        case 6: flag=AddNewRoute(Map);break;
        case 7: flag=RemoveBus(Map);break;
        case 8: flag=RemoveStation(Map);break;
        case 9: flag=RemoveRoute(Map);break;
        case 10: flag=ChangeBus(Map);break;
        case 11: flag=ChangeStation(Map);break;
        case 12: flag=ChangeRoute(Map);break;
        case 13: WriteDataToFile();system("cls");break;
        case 14: ResetSystem(Map);system("cls");break;
        case 0: Case0();
        default:
            flag=NONE;
            printf("输入数字无效！\n");
        }

        if(flag==NONE)
        {
            printf("输入任何键继续\n");
            getch();
            system("cls");
        }
    }
    return 0;
}

void Case0()
{
    int n;
    printf("\n\n--------------------------------------\n");
    printf("即将退出公交管理系统，确认是否已保存数据！\n");
    printf("     已保存（请输入1） 未保存（请输入0）\n");
    scanf("%d",&n);
    if(n==0)
        WriteDataToFile();
    printf("\n感谢您的使用！再见！\n");
    exit(0);
}
