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

#define DECK_NUMBER 1                   //牌堆數量
#define MAX_PLAYERS_NUMBER 999          //最大玩家資料數
#define MAX_PLAYER_NAME_SIZE 20         //玩家名稱最大長度
#define STRING_INDENT 35                //顯示字串縮排
#define FIELD_KEY_WIDTH 10              //欄位Key寛度
#define FIELD_VALUE_WIDTH 20            //欄位Value寛度
#define DEFAULT_CHIP 1000               //預設發放籌碼
#define PLAYERS_DAT_FILE "players.dat"  //玩家資料檔案

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
    char face[2];     //牌點
    int point;      //遊戲點數
    int altPoint;   //可替代的遊戲點數
    int order;      //出牌順序
    int appeared;   //出現次數
} Card;

//定義玩家資料型態和玩家屬性 (參考課程 ch10 p15)
typedef struct{
    char name[MAX_PLAYER_NAME_SIZE];  //玩家名稱
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

void inputPlayerName(char* playerName);
void readPlayerByName(Player* player, char* playerName);
void savePlayer(Player* player);
void printPlayer(Player* player);
void dumpPlayer(Player* player);
void dumpPlayers();
void bubbleSortPlayersByName(Player* players);

//XXXXXXX 測試卡牌渲染(測試用，將來會移除) XXXXXXX
void testRenderCard(int gap);
void makeDummyPlayer();
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

int main(){

    SetConsoleOutputCP(437);//印出特殊ASCII字元 (參考課程 ch04 p26)
    srand(time(NULL));//用時間來決定亂數 (參考課程 ch05 p28)
    mainMenu();

    return 0;
}

// +----------------------------------------+
// |                選單場景
// +----------------------------------------+
/**
 * @brief 主功能選單
 */
void mainMenu(){

    int indent = 0;
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
            dumpPlayers();
            break;
        }
        clearScreen();
        showWelcome();

        indent = STRING_INDENT- 18/2; //18來自下列字串的最大長度18個字
        printf("%*s%s\n", indent, " ", "3) dumpPlayers");
        printf("%*s%s\n", indent, " ", "2) makeDummyPlayer");
        printf("%*s%s\n", indent, " ", "1) Play");
        printf("%*s%s\n", indent, " ", "0) Quit");
        printf("\n%*s%s", indent, " ", "Choice:");
    }while((choice=getchar())!='0');
}

/**
 * @brief 玩一場牌局的場景
 */
void playCardScene(){
    /*
     * 將牌局所有要用到的變數在此宣告和初始化
     */
    int indent = 0;
    char choice = '?';
    char playerName[MAX_PLAYER_NAME_SIZE] = {0};
    Player player = {0};

    //要求使用者輸入玩家名稱
    inputPlayerName(playerName);
    //以玩家名稱讀取玩家資料
    readPlayerByName(&player, playerName);
    //初始化新玩家
    if(!strlen(player.name)){
        strcpy_s(player.name, sizeof(player.name), playerName);
        player.chip = DEFAULT_CHIP;
        player.playCount = 0;
        player.winCount = 0;
        player.loseCount = 0;
        player.blackjackCount = 0;
        savePlayer(&player);
    }

    clearScreen();

    //顯示Player資料
    printPlayer(&player);

    do{
        switch(choice){
        case '1':
            //TODO 初始牌堆
            //TODO 洗牌
            //TODO 發牌
            //TODO 進入遊戲邏輯
            clearScreen();
            indent = STRING_INDENT-13/2; //13來自下列字串的最大長度13個字
            printf("%*s%s\n\n", indent, " ", "PlayCardScene");
            printf("%*s%s\n", indent, " ", "1) Play again");
            printf("%*s%s\n", indent, " ", "0) Quit");
            printf("\n%*s%s", indent, " ", "Choice:");
            break;
        case '?':
            indent = STRING_INDENT-7/2; //7來自下列字串的最大長度7個字
            printf("%*s%s\n", indent, " ", "1) GO");
            printf("%*s%s\n", indent, " ", "0) Quit");
            printf("\n%*s%s", indent, " ", "Choice:");
            break;
        }

    }while((choice=getchar())!='0');
}

// +----------------------------------------+
// |                資料處理
// +----------------------------------------+
/**
 * @brief 輸入玩家名稱
 * @param playerName
 * scanf 參考 ch02 p19
 */
void inputPlayerName(char* playerName){
    clearScreen();
    int indent = STRING_INDENT-23/2; //23來自下列字串的最大長度23個字
    printf("%*sPlease enter your name:", indent, " ");
    scanf("%s", playerName);
}

/**
 * @brief 讀取玩家資料
 * @param playerName
 */
void readPlayerByName(Player* player, char* playerName){
    FILE *fp;
    errno_t err;
    Player tmp={0}; //必須初始化struct內的值為 0

    err=fopen_s(&fp,PLAYERS_DAT_FILE,"rb");
    if(err)
        return;

    while(!feof(fp)){
        fread(&tmp, sizeof(Player), 1, fp);
        if(!strlen(tmp.name))
            continue;
        if(strcmp(tmp.name, playerName)!=0)
            continue;
        strcpy_s(player->name, sizeof(player->name), tmp.name);
        player->chip = tmp.chip;
        player->playCount = tmp.playCount;
        player->winCount = tmp.winCount;
        player->loseCount = tmp.loseCount;
        player->blackjackCount = tmp.blackjackCount;
        break;
    }
    fclose(fp);
}

/**
 * @brief 將玩家資料儲存到檔案
 * @param player
 */
void savePlayer(Player* player){
    if(!strlen(player->name))
        return;

    FILE *fp;
    errno_t err;
    Player tmp={0}; //必須初始化struct內的值為 0
    int playerIndexInFile = -1;

    err=fopen_s(&fp,PLAYERS_DAT_FILE,"a+b");
    if(err)
        return;

    while(!feof(fp)){
        fread(&tmp, sizeof(Player), 1, fp);

        playerIndexInFile++;

        if(!strlen(tmp.name))
            continue;
        if(strcmp(tmp.name, player->name)==0)
            break;
    }
    fseek(fp, playerIndexInFile*sizeof(Player), SEEK_SET);
    fwrite(player, sizeof(Player), 1, fp);
    fclose(fp);
}


// +----------------------------------------+
// |                資料渲染
// +----------------------------------------+
/**
 * @brief 顯示歡迎畫面
 */
void showWelcome(){
    //歡迎詞
    char *welcome[] = {
        "",
        "            ____  __           __       __           __      ___  ___",
        "           / __ )/ /___ ______/ /__    / /___ ______/ /__   |__ \\<  /",
        "          / __  / / __ `/ ___/ //_/_  / / __ `/ ___/ //_/   __/ // / ",
        "         / /_/ / / /_/ / /__/ ,< / /_/ / /_/ / /__/ ,<     / __// /  ",
        "        /_____/_/\\__,_/\\___/_/|_|\\____/\\__,_/\\___/_/|_|   /____/_/",
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
 * @brief 印出玩家資料
 * @param player
 */
void printPlayer(Player* player){
    if(!strlen(player->name))
        return;

    int ident = STRING_INDENT-(FIELD_KEY_WIDTH+FIELD_VALUE_WIDTH)/2;

    printf("\n%*s+---------------------------------+\n", ident, " ");
    printf("%*s|           Player Info           |\n", ident, " ");
    printf("%*s+---------------------------------+\n", ident, " ");
    printf("%*s| %*s %*s |\n", ident, " ", FIELD_KEY_WIDTH, "Player", FIELD_VALUE_WIDTH, player->name);
    printf("%*s| %*s %*d |\n", ident, " ", FIELD_KEY_WIDTH, "Chip", FIELD_VALUE_WIDTH, player->chip);
    printf("%*s| %*s %*d |\n", ident, " ", FIELD_KEY_WIDTH, "Play", FIELD_VALUE_WIDTH, player->playCount);
    printf("%*s| %*s %*d |\n", ident, " ", FIELD_KEY_WIDTH, "Win", FIELD_VALUE_WIDTH, player->winCount);
    printf("%*s| %*s %*d |\n", ident, " ", FIELD_KEY_WIDTH, "Lose", FIELD_VALUE_WIDTH, player->loseCount);
    printf("%*s| %*s %*d |\n", ident, " ", FIELD_KEY_WIDTH, "Black Jack", FIELD_VALUE_WIDTH, player->blackjackCount);
    printf("%*s%s\n\n", ident, " ", "+---------------------------------+");
}

// +----------------------------------------+
// |                輔助功能
// +----------------------------------------+
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

/**
 * @brief 傾倒玩家資料
 */
void dumpPlayer(Player* player){
    if(!strlen(player->name))
        return;

    printf("Player name=%s chip=%d play=%d win=%d lose=%d blackjack=%d\n",
           player->name,
           player->chip,
           player->playCount,
           player->winCount,
           player->loseCount,
           player->blackjackCount
           );
}

/**
 * @brief 列出所有玩家資料
 */
void dumpPlayers(){
    FILE *fp;
    errno_t err;
    Player tmp={0}; //必須初始化struct內的值為 0
    int playerIndexInFile = -1;

    err=fopen_s(&fp,PLAYERS_DAT_FILE,"rb");
    if(err)
        return;

    clearScreen();

    fseek(fp, 0*sizeof(Player), SEEK_SET);
    while(!feof(fp)){
        fread(&tmp, sizeof(Player), 1, fp);

        if(!strlen(tmp.name))
            continue;

        playerIndexInFile++;
        printf("[%d] ", playerIndexInFile);
        dumpPlayer(&tmp);
        memset(tmp.name, 0, sizeof(tmp.name));
    }
    fclose(fp);

    pressAnyKeyToContinue();
}

/**
 * @brief 將玩家陣列以汽泡排序法排列
 * @param players
 * (參考課程 ch06 p42)
 */
void bubbleSortPlayersByName(Player *players){
    int loop;
    int ptrIdx;
    int playersSize=0;
    int i;
    Player hold;

    for(i=0;i<MAX_PLAYERS_NUMBER;i++){
        if(!strlen((players+i)->name))
            break;
        playersSize++;
    }

    for(loop=1; loop<playersSize; loop++){
        for(ptrIdx=0; ptrIdx<playersSize-1; ptrIdx++){
            //如果前者的name > 後者的name，則兩者互相交換
            Player *former = players+ptrIdx;
            Player *latter = players+ptrIdx+1;
            if(strcmp(former->name, latter->name)>0){ //字串比較 (參考課程 ch08 p40)
                //暫存前者資料
                strcpy_s(hold.name, sizeof(hold.name), former->name);
                hold.chip = former->chip;
                hold.playCount = former->playCount;
                hold.winCount = former->winCount;
                hold.loseCount = former->loseCount;
                hold.blackjackCount = former->blackjackCount;
                //後者資料assign給前者
                strcpy_s(former->name, sizeof(former->name), latter->name);
                former->chip = latter->chip;
                former->playCount = latter->playCount;
                former->winCount = latter->winCount;
                former->loseCount = latter->loseCount;
                former->blackjackCount = latter->blackjackCount;
                //暫存資料assign給後者
                strcpy_s(latter->name, sizeof(latter->name), hold.name);
                latter->chip = hold.chip;
                latter->playCount = hold.playCount;
                latter->winCount = hold.winCount;
                latter->loseCount = hold.loseCount;
                latter->blackjackCount = hold.blackjackCount;
            }
        }
    }
}

// +----------------------------------------+
// |                測試
// +----------------------------------------+
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

/**
 * @brief 產生測試用的假玩家
 */
void makeDummyPlayer(){
    Player dummyPlayers[MAX_PLAYERS_NUMBER]={0};
    char* playerName[] = {"david","hank","angus"};
    int playerNum = sizeof(playerName)/sizeof(*playerName);
    int i;

    clearScreen();

    for(i=0; i<playerNum; i++){
        strcpy_s(dummyPlayers[i].name, sizeof(dummyPlayers[i].name), playerName[i]);
        dummyPlayers[i].chip = 10000;
        dummyPlayers[i].playCount = 0;
        dummyPlayers[i].winCount = 0;
        dummyPlayers[i].loseCount = 0;
        dummyPlayers[i].blackjackCount = 0;

        printf("Dummy Player%d %s has been created.\n",i, dummyPlayers[i].name);
    }

    bubbleSortPlayersByName(dummyPlayers);

    for(i=0; i<MAX_PLAYERS_NUMBER; i++){
        if(!strlen(dummyPlayers[i].name))
            continue;
        printf("name=%s chip=%d play=%d win=%d lose=%d blackjack=%d\n",
               dummyPlayers[i].name,
               dummyPlayers[i].chip,
               dummyPlayers[i].playCount,
               dummyPlayers[i].winCount,
               dummyPlayers[i].loseCount,
               dummyPlayers[i].blackjackCount
               );
    }

    pressAnyKeyToContinue();
}
