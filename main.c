#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>

/**
 * 莊家 Banker
 * 閒家 Player
 * 下注 Bet
 * 籌碼 Chip
 * 洗牌 Shuffle
 * 發牌 Deal
 * 補牌 Hit 或 Draw
 * 分牌 Split
 * 雙倍下注 Double Down
 * 超過21點 Bust
 * 投降 Surrender
 * 停牌 Stand
 * 牌戲的一盤 Hand
 */

//定義卡牌資料型態和卡牌屬性
typedef struct{
    int deck;       //第N副牌
    char *suit;     //花色
    char *face;     //牌點
    int point;      //遊戲點數
    int altPoint;   //可替代的遊戲點數
    int order;      //出牌順序
    int appeared;   //出現次數
} Card;

//定義玩家資料型態
typedef struct{
    char *id;       //玩家ID
    int chip;       //籌碼
    int hands;      //玩牌次數
    int win;        //贏的次數
    int lose;       //輸的次數
    int blackjack;  //拿到黑傑克的次數
} Player;

void startGame();

void initDummyPlayer();
void initCard();
void shuffleCard();
void dealCard();

void showWelcome();

void arrayAppend();
void bubbleSort();
void clearScreen();
void pressAnyKeyToContinue();

void testRenderCard(int gap);

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

    int indent = 35;
    char choice = '9';

    do{
        switch(choice){
        case '1':
            testRenderCard(1);
            break;
        }
        clearScreen();
        showWelcome();

        printf("%*s%s\n", indent, " ", "1) Play");
        printf("%*s%s\n", indent, " ", "0) Quit");
        printf("\n%*s%s", indent, " ", "Choice:");
    }while((choice=getchar())!='0');
}

/**
 * @brief 顯示歡迎畫面
 *
 * 預先將歡迎詞放入一個2維陣列，即字串陣列
 * 歡迎詞有多行字串，每個陣列元素存放一行字串
 * 若字串內容有倒斜線\，則手動將之跳脫(escape)
 * 參考課程投影片ch09 p24
 */
void showWelcome(){
    //歡迎詞
    char *welcome[] = {
        "",
        //        "__________.__                 __        ___.              __     ________  ____ ",
        //        "\\______   \\  | _____    ____ |  | __   |   |____    ____ |  | __ \\_____  \\/_   |",
        //        " |    |  _/  | \\__  \\ _/ ___\\|  |/ /   |   \\__  \\ _/ ___\\|  |/ /  /  ____/ |   |",
        //        " |    |   \\  |__/ __ \\\\  \\___|    </\\__|   |/ __ \\\\  \\___|    <  /       \\ |   |",
        //        " |______  /____(____  /\\___  >__|_ \\_______(____  /\\___  >__|_ \\ \\_______ \\|___|",
        //        "        \\/          \\/     \\/     \\/             \\/     \\/     \\/         \\/    ",
        "            ____  __           __       __           __      ___  ___",
        "           / __ )/ /___ ______/ /__    / /___ ______/ /__   |__ \\<  /",
        "          / __  / / __ `/ ___/ //_/_  / / __ `/ ___/ //_/   __/ // / ",
        "         / /_/ / / /_/ / /__/ ,< / /_/ / /_/ / /__/ ,<     / __// /  ",
        "        /_____/_/\\__,_/\\___/_/|_|\\____/\\__,_/\\___/_/|_|   /____/_/   ",
        "",
    };

    //計算陣列字串的個數 (2D陣列所佔bytes/每個陣列元素所佔bytes)
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

