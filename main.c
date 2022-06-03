#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>

/*
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

#define DECK_NUMBER 1

//定義花色 (參考課程 ch10 p24)
enum Suit{
    SPADE,  //黑桃
    HEART,  //紅心
    DIAMOND,//方塊
    CLUB    //梅花
};

//定義卡牌資料型態和卡牌屬性 (參考課程 ch10 p15)
typedef struct{
    int deck;       //第N副牌
    enum Suit suit; //花色
    char *face;     //牌點
    int point;      //遊戲點數
    int altPoint;   //可替代的遊戲點數
    int order;      //出牌順序
    int appeared;   //出現次數
} Card;

//定義玩家資料型態和玩家屬性 (參考課程 ch10 p15)
typedef struct{
    char *id;           //玩家ID
    int chip;           //籌碼
    int playCount;      //玩牌次數
    int winCount;       //贏的次數
    int loseCount;      //輸的次數
    int blackjackCount; //拿到黑傑克的次數
} Player;

void mainMenu();                //主功能選單
void showWelcome();             //顯示歡迎詞
void playCardScene();           //玩一場牌局的場景
void clearScreen();             //清除畫面
void pressAnyKeyToContinue();   //按下任一鍵繼續

//XXXXXXX 測試卡牌渲染(測試用，將來會移除) XXXXXXX
void testRenderCard(int gap);
void makeDummyPlayer();
void dumpDummyPlayer();
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

int main(){

    SetConsoleOutputCP(437);//印出特殊ASCII字元 (參考課程 ch04 p26)
    srand(time(NULL));//用時間來決定亂數 (參考課程 ch05 p28)
    mainMenu();

    return 0;
}

//主功能選單
void mainMenu(){

    char choice = '?'; //選單功能選擇

    do{
        switch(choice){
        case '1':
            playCardScene();
            break;
        case '2': //XXX 純測試用，將來移除
            //testRenderCard(1);
            makeDummyPlayer();
            break;
        case '3': //XXX 純測試用，將來移除
            break;
        }
        clearScreen();
        showWelcome();

        printf("%35s%s\n", " ", "3) Test2");
        printf("%35s%s\n", " ", "2) makeDummyPlayer");
        printf("%35s%s\n", " ", "1) Play");
        printf("%35s%s\n", " ", "0) Quit");
        printf("\n%35s%s", " ", "Choice:");
    }while((choice=getchar())!='0');
}

//顯示歡迎畫面
void showWelcome(){
    //歡迎詞
    //https://patorjk.com/software/taag/#p=display&f=Slant&t=BlackJack%2021
    //若字串內容有倒斜線\，則手動將之跳脫(escape) (參考課程 ch09 p24)
    printf("\n");
    printf("            ____  __           __       __           __      ___  ___\n");
    printf("           / __ )/ /___ ______/ /__    / /___ ______/ /__   |__ \\<  /\n");
    printf("          / __  / / __ `/ ___/ //_/_  / / __ `/ ___/ //_/   __/ // / \n");
    printf("         / /_/ / / /_/ / /__/ ,< / /_/ / /_/ / /__/ ,<     / __// /  \n");
    printf("        /_____/_/\\__,_/\\___/_/|_|\\____/\\__,_/\\___/_/|_|   /____/_/\n");
    printf("\n");
}

//玩一場牌局的場景
void playCardScene(){
    /*
     * TODO 將牌局所有要用到的變數在此宣告和初始化
     */
    Player player[999]; //曾經建檔過的玩家
    char choice = '?';

    //TODO 從檔案載入所有Player資料到陣列
    //TODO 要求使用者輸入帳號
    //TODO 從Player陣列裏搜尋帳號資料 (此處要用到二元搜尋法)
    //TODO 如果找到帳號資料則 assign 到變數
    //TODO 如果沒找到則append一個元素到Player陣列

    clearScreen();
    printf("%35s%s choice=%c\n\n",  " ", "User Info", choice);
    //TODO 顯示Player資料

    do{
        switch(choice){
        case '1':
            //TODO 初始牌堆
            //TODO 洗牌
            //TODO 發牌
            //TODO 進入遊戲邏輯
            clearScreen();
            printf("%35s%s\n\n", " ", "PlayCardScene");
            printf("%35s%s\n", " ", "1) Play again");
            printf("%35s%s\n", " ", "0) Quit");
            printf("\n%35s%s", " ", "Choice:");
            break;
        case '?':
            printf("%35s%s\n", " ", "1) GO");
            printf("%35s%s\n", " ", "0) Quit");
            printf("\n%35s%s", " ", "Choice:");
            break;
        }

    }while((choice=getchar())!='0');
}

//清除畫面
void clearScreen(){
    system("cls");
}

//按下任一鍵繼續
void pressAnyKeyToContinue(){
    system("pause");
}

//產生測試用的假玩家
void makeDummyPlayer(){
    Player dummyPlayer[999]={0};
    char* playerId[] = {"david","hank","angus"};
    int playerNum = sizeof(playerId)/sizeof(*playerId);
    int i;

    clearScreen();

    for(i=0; i<playerNum; i++){
        dummyPlayer[i].id = playerId[i];
        dummyPlayer[i].chip = 10000;
        dummyPlayer[i].playCount = 0;
        dummyPlayer[i].winCount = 0;
        dummyPlayer[i].loseCount = 0;
        dummyPlayer[i].blackjackCount = 0;

        printf("Dummy Player%d has been created.\n",i);
    }

    for(i=0; i<999; i++){
        if(!dummyPlayer[i].id)
            continue;
        printf("i=%d id=%s chip=%d play=%d win=%d lose=%d blackjack=%d\n",
               i,
               dummyPlayer[i].id,
               dummyPlayer[i].chip,
               dummyPlayer[i].playCount,
               dummyPlayer[i].winCount,
               dummyPlayer[i].loseCount,
               dummyPlayer[i].blackjackCount
               );
    }

    pressAnyKeyToContinue();
}

//印出玩家陣列
void dumpPlayer(){

}

//---------------以下測試用，將來會移除--------------------
/**
 * 測試卡牌渲染(測試用，將來會移除)
 * gap = 兩張卡牌間的間隔
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
