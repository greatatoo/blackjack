#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>

void startGame();

void initDummyPlayer();
void initCard();
void shuffleCard();
void dealCard();

void showWelcome();
void bubbleSort();

void clearScreen();
void pressAnyKeyToContinue();

void testRenderCard(int gap);

//定義卡牌資料型態和卡牌屬性
typedef struct{
    int deck;   //牌堆
    char *suit; //花色
    char *face; //牌點
    int point;  //遊戲點數
    int altPoint;//遊戲點數
    int order;  //出牌順序
    int showCount; //出現次數
} Card;

int main(){

    SetConsoleOutputCP(437);
    srand(time(NULL));
    startGame();

    return 0;
}

/**
 * @brief 開始遊戲
 */
void startGame(){

    char choice = '9';

    do{
        switch(choice){
        case '1':
            testRenderCard(1);
            break;
        }
        clearScreen();
        showWelcome();
        printf("1) Play\n");
        printf("0) Quit\n");
        printf("Choice:");
    }while((choice=getchar())!='0');
}

/**
 * @brief 顯示歡迎畫面
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

    //逐行印出
    for(i=0; i<lineCount; i++)
        printf("%s\n",*(welcome+i));
}

/**
 * @brief 測試卡牌渲染
 * @param gap 兩張卡牌間的間隔
 */
void testRenderCard(int gap){
    clearScreen();

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

    pressAnyKeyToContinue();
}

/**
 * @brief 清除畫面
 */
void clearScreen(){
    system("cls");
}

/**
 * @brief 按下任一鍵繼續
 */
void pressAnyKeyToContinue(){
    system("pause");
}

