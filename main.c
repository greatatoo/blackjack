#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <Windows.h>

void showWelcome();
void initCard();
void shuffleCard();
void dealCard();
void bubbleSort();

void testRenderCard(int gap);

//定義卡牌資料型態和卡牌屬性
typedef struct{
    int deck;   //牌堆
    char *suit; //花色
    char *face; //牌點
    int order;  //出牌順序
    int showCount; //出現次數
} Card;

int main(){

    srand(time(NULL));
    SetConsoleOutputCP(437);

    testRenderCard(1);
    //showWelcome();

    return 0;
}

/*
 * 顯示歡迎畫面
 *
 * 預先將歡迎詞放入一個2維陣列，即字串陣列
 * 歡迎詞有多行字串，每個陣列欄位存放一行字串
 * 若字串內容有倒斜線\，則手動將之跳脫(escape)
 * 參考課程投影片ch09 p24
 */
void showWelcome(){
    //歡迎詞
    char *welcome[] = {
        "",
        "__________.__                 __          ____.              __    ",
        "\\______   \\  | _____    ____ |  | __     |    |____    ____ |  | __",
        " |    |  _/  | \\__  \\ _/ ___\\|  |/ /     |    \\__  \\ _/ ___\\|  |/ /",
        " |    |   \\  |__/ __ \\\\  \\___|    <  /\\__|    |/ __ \\\\  \\___|    < ",
        " |______  /____(____  /\\___  >__|_ \\ \\________(____  /\\___  >__|_ \\",
        "        \\/          \\/     \\/     \\/               \\/     \\/     \\/",
        "  _________                    .__           __      __.__         ",
        " /   _____/__ _________   ____ |  | ___.__. /  \\    /  \\__| ____   ",
        " \\_____  \\|  |  \\_  __ \\_/ __ \\|  |<   |  | \\   \\/\\/   /  |/    \\  ",
        " /        \\  |  /|  | \\/\\  ___/|  |_\\___  |  \\        /|  |   |  \\ ",
        "/_______  /____/ |__|    \\___  >____/ ____|   \\__/\\  / |__|___|  / ",
        "        \\/                   \\/     \\/             \\/          \\/  ",
        "",
    };
    //計算陣列字串的個數 (2D陣列所佔bytes/每個陣列欄位所佔bytes)
    int lineCount = sizeof(welcome)/sizeof(*welcome);
    int i;

    //清除畫面
    system("cls");

    //逐行印出
    for(i=0; i<lineCount; i++)
        printf("%s\n",*(welcome+i));

    //暫停畫面
    system("pause");
}

/*
 * 測試卡牌渲染
 *
 * 參數 int gap 兩張卡牌間的間隔
 */
void testRenderCard(int gap){
    system("cls");

    printf("%c%c%c%c%c",218,196,196,196,191);
    printf("%*c",gap,32);
    printf("%c%c%c%c%c",218,196,196,196,191);
    printf("\n");
    printf("%c%c%c%c%c",179,6,32,32,179);
    printf("%*c",gap,32);
    printf("%c%c%c%c%c",179,6,32,32,179);
    printf("\n");
    printf("%c%c%c%c%c",179,32,32,65,179);
    printf("%*c",gap,32);
    printf("%c%c%c%c%c",179,32,32,65,179);
    printf("\n");
    printf("%c%c%c%c%c",192,196,196,196,217);
    printf("%*c",gap,32);
    printf("%c%c%c%c%c",192,196,196,196,217);
    printf("\n");

    system("pause");
}

