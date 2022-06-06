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
 * 179 = │
 * 191 = ┐
 * 192 = └
 * 196 = ─
 * 217 = ┘
 * 218 = ┌
 */

#define DECK_NUMBER 1                   //牌堆數量
#define CARD_NUMBER 52                  //一副牌有52張
#define MAX_PLAYERS_NUMBER 999          //最大玩家資料數
#define MAX_PLAYER_NAME_SIZE 20         //玩家名稱最大長度
#define STRING_INDENT 35                //顯示字串縮排
#define FIELD_KEY_WIDTH 10              //欄位Key寛度
#define FIELD_VALUE_WIDTH 20            //欄位Value寛度
#define DEFAULT_CHIP 1000               //預設發放籌碼
#define PLAYERS_DAT_FILE "players.dat"  //玩家資料檔案
#define MAX_SPLIT_NUMBER 4*DECK_NUMBER  //最大分牌數量
#define MAX_DEAL_NUMBER 5               //最大發牌數量
#define DEFAULT_BET 50                  //預設下注額

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
    char face[3];   //牌點
    int point;      //遊戲點數
    int altPoint;   //可替代的遊戲點數
    int order;      //出牌順序
    int appeared;   //出現次數
} Card;

//定義牌局資料型態和牌局屬性 (參考課程 ch10 p15)
typedef struct{
    int game[MAX_DEAL_NUMBER]; //手牌
    boolean banker; //是否為莊家
    boolean insurance; //保險
    boolean doubleDown; //雙倍下注
    boolean surrender; //投降
    boolean stand;     //停牌
    boolean bust;      //爆牌
    int bet; //下注
} Hand;

//定義玩家資料型態和玩家屬性 (參考課程 ch10 p15)
typedef struct{
    char name[MAX_PLAYER_NAME_SIZE];  //玩家名稱
    int chip;           //籌碼
    int playCount;      //玩牌次數
    int winCount;       //贏的次數
    int loseCount;      //輸的次數
    int blackjackCount; //拿到黑傑克的次數
} Player;

// +----------------------------------------+
// |                選單場景
// +----------------------------------------+
void mainMenu(); //主功能選單
void playCardScene(); //玩一場牌局的場景
void blackjackLogic(Card* cards, Hand* bankerHands, Hand* playerHands); //黑傑克邏輯
// +----------------------------------------+
// |                資料處理
// +----------------------------------------+
void inputPlayerName(char* playerName); //輸入玩家名稱
void readPlayerByName(Player* player, char* playerName); //讀取玩家資料
void savePlayer(Player* player); //將玩家資料儲存到檔案
void initHands(Hand* hands, boolean isBanker); //初始化手牌
void initCards(Card* cards); //初始化牌堆
void shuffleCards(Card* cards); //洗牌
void dealCard(int* nextCardIndex, Hand* playerHands, Card* cards); //發牌給玩家或莊家
// +----------------------------------------+
// |                資料渲染
// +----------------------------------------+
void showWelcome(); //顯示歡迎畫面
void printPlayer(Player* player); //印出玩家資料
// +----------------------------------------+
// |                輔助功能
// +----------------------------------------+
void clearScreen(); //清除畫面
void pressAnyKeyToContinue(); //按下任一鍵繼續
void dumpPlayer(Player* player); //傾倒玩家資料
void dumpPlayers(); //列出所有玩家資料
void dumpCards(Card* cards); //列出卡牌
void printHand(Hand* hand, Card* cards); //印出手牌(無清除畫面，無暫停)
void dumpHand(Hand* hand, Card* cards); //列出手牌
void dumpHands(Hand* hands, Card* cards); //列出手牌陣列
// +----------------------------------------+
// |                測試
// +----------------------------------------+
void testRenderCard(int gap); //測試卡牌渲染
void makeDummyPlayer(); //產生測試用的假玩家

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
    int indent = 0; //字串縮排
    char choice = '?'; //選單號碼
    char playerName[MAX_PLAYER_NAME_SIZE] = {0}; //玩家名稱
    Player player = {0}; //玩家資料
    Card cards[DECK_NUMBER*CARD_NUMBER] = {0}; //牌堆
    Hand bankerHands[MAX_SPLIT_NUMBER] = {0}; //莊家手牌
    Hand playerHands[MAX_SPLIT_NUMBER] = {0}; //玩家手牌

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
            //初始手牌
            initHands(bankerHands, TRUE);
            initHands(playerHands, FALSE);
            //初始牌堆
            initCards(cards);
            //洗牌
            shuffleCards(cards);
            //黑傑克邏輯
            blackjackLogic(cards, bankerHands, playerHands);

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

/**
 * @brief 黑傑克遊戲邏輯
 */
void blackjackLogic(Card* cards, Hand* bankerHands, Hand* playerHands){
    int nextCardIndex = 0;
    int i,j;
    Hand* hand;

    clearScreen();

    //向莊家發一張牌
    dealCard(&nextCardIndex, bankerHands, cards);
    dealCard(&nextCardIndex, bankerHands, cards);
    dealCard(&nextCardIndex, bankerHands, cards);
    dumpHands(bankerHands, cards);
    //向玩家發一張牌
    dealCard(&nextCardIndex, playerHands, cards);
    dealCard(&nextCardIndex, playerHands, cards);
    dealCard(&nextCardIndex, playerHands, cards);
    dumpHands(playerHands, cards);

    dumpCards(cards);

    pressAnyKeyToContinue();
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

/**
 * @brief 初始化手牌
 * @param hands
 */
void initHands(Hand* hands, boolean isBanker){
    int i,j;
    Hand* hand;

    for(i=0; i<MAX_SPLIT_NUMBER; i++){
        hand = hands+i;
        for(j=0; j<MAX_DEAL_NUMBER; j++)
            *(hand->game+j)=-1;
        hand->banker = isBanker;
        hand->insurance = FALSE;
        hand->doubleDown = FALSE;
        hand->surrender = FALSE;
        hand->stand = FALSE;
        hand->bust = FALSE;
        hand->bet = i==0? DEFAULT_BET:0;
    }
}

/**
 * @brief 初始化牌堆
 */
void initCards(Card* cards){
    int i,j;
    Card* card;

    for(i=0; i<DECK_NUMBER; i++){
        for(j=0; j<CARD_NUMBER; j++){
            card = cards+i*CARD_NUMBER+j;
            card->deck = i;
            card->suit = j/13;
            switch(j%13){
            case 0:
                strcpy_s(card->face, sizeof(card->face), "A");
                card->point = 1;
                card->altPoint = 11;
                card->order = rand();
                card->appeared = 0;
                break;
            case 1:
                strcpy_s(card->face, sizeof(card->face), "2");
                card->point = 2;
                card->altPoint = 2;
                card->order = rand();
                card->appeared = 0;
                break;
            case 2:
                strcpy_s(card->face, sizeof(card->face), "3");
                card->point = 3;
                card->altPoint = 3;
                card->order = rand();
                card->appeared = 0;
                break;
            case 3:
                strcpy_s(card->face, sizeof(card->face), "4");
                card->point = 4;
                card->altPoint = 4;
                card->order = rand();
                card->appeared = 0;
                break;
            case 4:
                strcpy_s(card->face, sizeof(card->face), "5");
                card->point = 5;
                card->altPoint = 5;
                card->order = rand();
                card->appeared = 0;
                break;
            case 5:
                strcpy_s(card->face, sizeof(card->face), "6");
                card->point = 6;
                card->altPoint = 6;
                card->order = rand();
                card->appeared = 0;
                break;
            case 6:
                strcpy_s(card->face, sizeof(card->face), "7");
                card->point = 7;
                card->altPoint = 7;
                card->order = rand();
                card->appeared = 0;
                break;
            case 7:
                strcpy_s(card->face, sizeof(card->face), "8");
                card->point = 8;
                card->altPoint = 8;
                card->order = rand();
                card->appeared = 0;
                break;
            case 8:
                strcpy_s(card->face, sizeof(card->face), "9");
                card->point = 9;
                card->altPoint = 9;
                card->order = rand();
                card->appeared = 0;
                break;
            case 9:
                strcpy_s(card->face, sizeof(card->face), "10");
                card->point = 10;
                card->altPoint = 10;
                card->order = rand();
                card->appeared = 0;
                break;
            case 10:
                strcpy_s(card->face, sizeof(card->face), "J");
                card->point = 10;
                card->altPoint = 10;
                card->order = rand();
                card->appeared = 0;
                break;
            case 11:
                strcpy_s(card->face, sizeof(card->face), "Q");
                card->point = 10;
                card->altPoint = 10;
                card->order = rand();
                card->appeared = 0;
                break;
            case 12:
                strcpy_s(card->face, sizeof(card->face), "K");
                card->point = 10;
                card->altPoint = 10;
                card->order = rand();
                card->appeared = 0;
                break;
            }
        }
    }
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
 * @brief 傾倒牌堆卡牌
 * @param cards
 */
void dumpCards(Card* cards){
    int i,j,ascii;
    Card* card;

    clearScreen();

    for(i=0; i<DECK_NUMBER; i++){
        for(j=0; j<CARD_NUMBER; j++){
            card = cards+i*CARD_NUMBER+j;
            switch(card->suit){
            case SPADE:
                ascii = 6;
                break;
            case HEART:
                ascii = 3;
                break;
            case DIAMOND:
                ascii = 4;
                break;
            case CLUB:
                ascii = 5;
                break;
            }

            printf("card#%03d deck=%2d suit=%c face=%2s point=%2d,%2d order=%5d appeared=%3d\n",
                   i*CARD_NUMBER+j,
                   card->deck,
                   ascii,
                   card->face,
                   card->point,
                   card->altPoint,
                   card->order,
                   card->appeared
                   );
        }
    }

    pressAnyKeyToContinue();
}

void printHand(Hand* hand, Card* cards){
    int j,k,ascii;

    printf("%s hand bet=%d insurance=%s double=%s surrender=%s stand=%s bust=%s",
           (hand->banker?"Banker":"Player"),
           hand->bet,
           hand->insurance?"O":"X",
           hand->doubleDown?"O":"X",
           hand->surrender?"O":"X",
           hand->stand?"O":"X",
           hand->bust?"O":"X"
           );

    printf(" card=[");
    for(j=0; j<MAX_DEAL_NUMBER; j++){
        if(j==0)
            printf("%d",*(hand->game+j));
        else
            printf(",%d",*(hand->game+j));
    }
    printf("]");

    printf(" card=[");
    for(j=0; j<MAX_DEAL_NUMBER; j++){
        k = *(hand->game+j);
        if(k<0)
            continue;
        switch((cards+k)->suit){
        case SPADE:
            ascii = 6;
            break;
        case HEART:
            ascii = 3;
            break;
        case DIAMOND:
            ascii = 4;
            break;
        case CLUB:
            ascii = 5;
            break;
        }
        if(j==0)
            printf("%c %s",ascii, (cards+k)->face);
        else
            printf(",%c %s",ascii, (cards+k)->face);
    }
    printf("]\n");
}

void dumpHand(Hand* hand, Card* cards){
    clearScreen();
    printHand(hand, cards);
    pressAnyKeyToContinue();
}

/**
 * @brief 列出手牌
 * @param hands
 */
void dumpHands(Hand* hands, Card* cards){
    int i;
    clearScreen();
    for(i=0; i<MAX_SPLIT_NUMBER; i++)
        printHand(hands+i, cards);
    pressAnyKeyToContinue();
}

/**
 * @brief 洗牌
 * @param cards
 */
void shuffleCards(Card* cards){
    int loop, ptrIdx;
    int total = DECK_NUMBER*CARD_NUMBER;
    Card hold;

    for(loop=1; loop<total; loop++){
        for(ptrIdx=0; ptrIdx<total-1; ptrIdx++){
            //如果前者的order > 後者的order，則兩者互相交換
            Card *former = cards+ptrIdx;
            Card *latter = cards+ptrIdx+1;
            if(former->order > latter->order){
                //暫存前者資料
                hold.deck = former->deck;
                hold.suit = former->suit;
                strcpy_s(hold.face, sizeof(hold.face), former->face);
                hold.point = former->point;
                hold.altPoint = former->altPoint;
                hold.order = former->order;
                hold.appeared = former->appeared;
                //後者資料assign給前者
                former->deck = latter->deck;
                former->suit = latter->suit;
                strcpy_s(former->face, sizeof(former->face), latter->face);
                former->point = latter->point;
                former->altPoint = latter->altPoint;
                former->order = latter->order;
                former->appeared = latter->appeared;
                //暫存資料assign給後者
                latter->deck = hold.deck;
                latter->suit = hold.suit;
                strcpy_s(latter->face, sizeof(latter->face), hold.face);
                latter->point = hold.point;
                latter->altPoint = hold.altPoint;
                latter->order = hold.order;
                latter->appeared = hold.appeared;
            }
        }
    }
}

/**
 * @brief 發牌
 * @param nextCardIndex 目前已發牌的index
 * @param playerHands 玩家手牌
 */
void dealCard(int* nextCardIndex, Hand* playerHands, Card* cards){
    int i,j;
    boolean isDeal = FALSE;
    Hand* hand;

    for(i=0; i<MAX_SPLIT_NUMBER; i++){
        hand = playerHands+i;

        //跳過沒下注的
        if(!hand->bet)
            continue;
        //已停牌的
        if(hand->stand)
            continue;
        //已爆牌的
        if(hand->bust)
            continue;

        for(j=0; j<MAX_DEAL_NUMBER; j++){
            if(*(hand->game+j)==-1){ // -1代表尚未被發牌
                *(hand->game+j) = *nextCardIndex;
                //發牌後，將下一張牌的index++
                (*nextCardIndex)++;
                isDeal = TRUE;
                break;
            }
        }

        if(isDeal){
            //TODO 計算點數，依點數更新其它狀態如 bust
            dumpHand(hand, cards);

            break;
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
