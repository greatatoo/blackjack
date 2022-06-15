#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
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
#define DEFAULT_CHIP 5000               //預設發放籌碼
#define PLAYERS_DAT_FILE "Players.dat"  //玩家資料檔案
#define MAX_SPLIT_NUMBER 4*DECK_NUMBER  //最大分牌數量
#define MAX_DEAL_NUMBER 5               //最大發牌數量
#define CARD_POINT_POSSIBILITY 2        //卡牌點數的可能數
#define MAX_POINTS_COMBINATION 32       //多張卡牌點數的排列組合數
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
    int card[MAX_DEAL_NUMBER]; //手牌
    boolean banker;     //是否為莊家
    boolean split;      //分牌
    boolean insurance;  //保險
    boolean doubleDown; //雙倍下注
    boolean surrender;  //投降
    boolean stand;      //停牌
    boolean bust;       //爆牌
    int bet;            //下注
    int highPoint;       //最高點
    int safeHighPoint;   //安全最高點
    int lowPoint;       //最低點
} Hand;

//定義玩家資料型態和玩家屬性 (參考課程 ch10 p15)
typedef struct{
    char name[MAX_PLAYER_NAME_SIZE];  //玩家名稱
    int chip;           //籌碼
    int playCount;      //玩牌次數
    int winCount;       //贏的次數
    int drawCount;      //平手次數
    int lossCount;      //輸的次數
    int blackjackCount; //拿到黑傑克的次數
} Player;

// +----------------------------------------+
// |                選單場景
// +----------------------------------------+
void mainMenu(); //主功能選單
void playCardScene(); //玩一場牌局的場景
void blackjackLogic(Card* cards, Hand* bankerHands, Hand* playerHands, Player* player); //黑傑克邏輯
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
void computeHand(Hand* hand, Card* cards, int* highPoint, int* safeHightPoint, int* lowPoint); //計算手牌點數並更新狀態
void getHighLowPoints(int* arr, int* highPoint, int* safeHightPoint, int* lowPoint); //取得手牌組合點數的最大值和最小值
// +----------------------------------------+
// |                資料渲染
// +----------------------------------------+
void showWelcome(); //顯示歡迎畫面
void printPlayer(Player* player); //印出玩家資料
void printDesktop(Card* cards, Hand* bankerHands, Hand* playerHands, Player* player); //印出桌面上的牌
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
        case '2':
            dumpPlayers();
            break;
        }
        clearScreen();
        showWelcome();

        indent = STRING_INDENT- 18/2; //18來自下列字串的最大長度18個字

        printf("%*s%s\n", indent, " ", "1) Play");
        printf("%*s%s\n", indent, " ", "2) Leaderboard");
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
    char playerName[MAX_PLAYER_NAME_SIZE] = {0};//玩家名稱
    Player player = {0}; //玩家資料
    Card cards[DECK_NUMBER*CARD_NUMBER] = {0};  //牌堆
    Hand bankerHands[MAX_SPLIT_NUMBER] = {0};   //莊家手牌
    Hand playerHands[MAX_SPLIT_NUMBER] = {0};   //玩家手牌

    //要求使用者輸入玩家名稱
    inputPlayerName(playerName);
    if(!strlen(playerName))
        return;
    //以玩家名稱讀取玩家資料
    readPlayerByName(&player, playerName);
    //初始化新玩家
    if(!strlen(player.name)){
        strcpy_s(player.name, sizeof(player.name), playerName);
        player.chip = DEFAULT_CHIP;
        player.playCount = 0;
        player.winCount = 0;
        player.drawCount = 0;
        player.lossCount = 0;
        player.blackjackCount = 0;
        savePlayer(&player);
    }

    clearScreen();

    //顯示Player資料
    printPlayer(&player);
    //初始牌堆
    initCards(cards);

    do{
        switch(choice){
        case '1':
            //玩家計次
            player.playCount+=1;
            //初始手牌
            initHands(bankerHands, TRUE);
            initHands(playerHands, FALSE);
            //洗牌
            shuffleCards(cards);
            //黑傑克邏輯
            blackjackLogic(cards, bankerHands, playerHands, &player);

            clearScreen();

            indent = STRING_INDENT-13/2; //13來自下列字串的最大長度13個字
            //顯示Player資料
            printPlayer(&player);
            printf("%*s%s\n", indent, " ", "1) Continue");
            printf("%*s%s\n", indent, " ", "0) Quit");
            printf("\n%*s%s", indent, " ", "Choice:");
            break;
        case '?':
            indent = STRING_INDENT-7/2; //7來自下列字串的最大長度7個字
            printf("%*s%s\n", indent, " ", "1) Begin");
            printf("%*s%s\n", indent, " ", "0) Quit");
            printf("\n%*s%s", indent, " ", "Choice:");
            break;
        }
    }while((choice=getchar())!='0');

    //儲存玩家資料
    savePlayer(&player);
}

/**
 * @brief 黑傑克遊戲邏輯
 */
void blackjackLogic(Card* cards, Hand* bankerHands, Hand* playerHands, Player* player){
    int nextCardIndex = 0;
    int i,j;
    Hand* hand;

    clearScreen();

    //開局先對玩家和莊家發牌兩輪
    for(i=0; i<2; i++){
        //向玩家發一張牌
        dealCard(&nextCardIndex, playerHands, cards);
        //向莊家發一張牌
        dealCard(&nextCardIndex, bankerHands, cards);
    }
    //dumpHands(playerHands, cards);
    //dumpHands(bankerHands, cards);
    //dumpCards(cards);
    printDesktop(cards, bankerHands, playerHands, player);

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
    char choice;
    char inputName[MAX_PLAYER_NAME_SIZE]={0};
    int indent = STRING_INDENT-23/2; //23來自下列字串的最大長度23個字

    do{
        clearScreen();

        printf("\n\n\n%*sPlease enter your name:", indent, " ");
        scanf("%s", inputName);
        if(strlen(inputName)>7){
            printf("\n%*sThe length of name must be less than 7.\n", indent, " ");
            system("pause");
        }else{
            break;
        }
    }while(1);
    printf("\n%*sYou're going to play with account '%s'.\n\n", indent, " ", inputName);
    printf("%*s1) Yes\n", indent, "");
    printf("%*s0) No\n\n", indent, "");
    printf("%*sChoice:", indent, "");
    getchar();//消耗上面 scanf 按下的 enter
    choice=getchar();

    switch(choice){
    case '1':
        strcpy_s(playerName, sizeof(playerName), inputName);
        break;
    }
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
        player->drawCount = tmp.drawCount;
        player->lossCount = tmp.lossCount;
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
    boolean isPlayerFound = FALSE;
    int playerIndexInFile = -1;

    err=fopen_s(&fp,PLAYERS_DAT_FILE,"ab+");
    if(err)
        return;

    //尋找玩家
    while(!feof(fp)){
        fread(&tmp, sizeof(Player), 1, fp);
        playerIndexInFile++;

        if(!strlen(tmp.name))
            break;

        if(strcmp(tmp.name, player->name)==0){
            isPlayerFound = TRUE;
            break;
        }
    }
    fclose(fp);

    if(isPlayerFound){//更新玩家資料
        err=fopen_s(&fp,PLAYERS_DAT_FILE,"rb+");
        if(err)
            return;
        fseek(fp, playerIndexInFile*sizeof(Player), SEEK_SET);
        fwrite(player, sizeof(Player), 1, fp);
        fclose(fp);
    }else{//新增玩家資料
        err=fopen_s(&fp,PLAYERS_DAT_FILE,"ab+");
        if(err)
            return;
        fwrite(player, sizeof(Player), 1, fp);
        fclose(fp);
    }
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
            *(hand->card+j)=-1;
        hand->banker = isBanker;
        hand->split = FALSE;
        hand->insurance = FALSE;
        hand->doubleDown = FALSE;
        hand->surrender = FALSE;
        hand->stand = FALSE;
        hand->bust = FALSE;
        hand->bet = i==0? DEFAULT_BET:0;
        hand->highPoint = 0;
        hand->safeHighPoint = 0;
        hand->lowPoint = 0;
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
    printf("%*s|          Player Stats           |\n", ident, " ");
    printf("%*s+---------------------------------+\n", ident, " ");
    printf("%*s| %*s %*s |\n", ident, " ", FIELD_KEY_WIDTH, "Player", FIELD_VALUE_WIDTH, player->name);
    printf("%*s| %*s %*d |\n", ident, " ", FIELD_KEY_WIDTH, "Chips", FIELD_VALUE_WIDTH, player->chip);
    printf("%*s| %*s %*d |\n", ident, " ", FIELD_KEY_WIDTH, "Plays", FIELD_VALUE_WIDTH, player->playCount);
    printf("%*s| %*s %*d |\n", ident, " ", FIELD_KEY_WIDTH, "Wins", FIELD_VALUE_WIDTH, player->winCount);
    printf("%*s| %*s %*d |\n", ident, " ", FIELD_KEY_WIDTH, "Draws", FIELD_VALUE_WIDTH, player->drawCount);
    printf("%*s| %*s %*d |\n", ident, " ", FIELD_KEY_WIDTH, "Losses", FIELD_VALUE_WIDTH, player->lossCount);
    printf("%*s| %*s %*d |\n", ident, " ", FIELD_KEY_WIDTH, "Black Jack", FIELD_VALUE_WIDTH, player->blackjackCount);
    printf("%*s%s\n\n", ident, " ", "+---------------------------------+");
}

/**
 * @brief 印出桌面上的牌
 * @param cards
 * @param bankerHands
 * @param playerHands
 */
void printDesktop(Card* cards, Hand* bankerHands, Hand* playerHands, Player* player){
    int h,c,i,j,cardCount;
    char cardsCell[40]={0};
    char tmp[40]={0};
    Hand* tmpHand;
    int ascii;

    char* singleLine =  "+----------+----------------------------------------+------+-------------------+";

    printf("%s\n", singleLine);
    //玩家手牌
    for(h=0; h<MAX_SPLIT_NUMBER; h++){
        tmpHand = playerHands+h;
        //判斷這一手牌是否有下注開始玩
        if(!tmpHand->bet)
            continue;
        //計算這一手牌目前發了幾張牌
        for(c=0,cardCount=0; c<MAX_DEAL_NUMBER; c++){
            if(*(tmpHand->card+c)>=0)
                cardCount++;
        }
        for(i=0; i<4; i++){//每張卡牌的高佔4列
            switch(i){
            case 0://第一列
                cardsCell[0]='\0';
                //顯示每張手牌的第一列
                for(c=0; c<cardCount; c++){
                    sprintf(tmp,"%c%c%c%c%c%c",32,218,196,196,196,191);
                    strcat_s(cardsCell, sizeof(cardsCell)/sizeof(cardsCell[0]), tmp);
                }
                printf("|%10s|%-40s|%6s|%19s|\n"," ", cardsCell," "," "," ");
                break;
            case 1://第二列
                cardsCell[0]='\0';
                //顯示每張手牌的第二列
                for(c=0; c<cardCount; c++){
                    //轉換 suit 為 ascii
                    switch((cards+*(tmpHand->card+c))->suit){
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
                    sprintf(tmp,"%c%c%c%c%c%c",32,179,ascii,32,32,179);
                    strcat_s(cardsCell, sizeof(cardsCell)/sizeof(cardsCell[0]), tmp);
                }
                printf("| %8s |%-40s|  %2d  |%19s|\n",player->name,cardsCell,tmpHand->safeHighPoint," "," ");
                break;
            case 2://第三列
                cardsCell[0]='\0';
                //顯示每張手牌的第三列
                for(c=0; c<cardCount; c++){
                    sprintf(tmp,"%c%c%c%2s%c",32,179,32,(cards+*(tmpHand->card+c))->face,179,32);
                    strcat_s(cardsCell, sizeof(cardsCell)/sizeof(cardsCell[0]), tmp);
                }
                printf("|%10s|%-40s|%6s|%19s|\n"," ",cardsCell," "," "," ");
                break;
            case 3://第四列
                cardsCell[0]='\0';
                //顯示每張手牌的第一列
                for(c=0; c<cardCount; c++){
                    sprintf(tmp,"%c%c%c%c%c%c",32,192,196,196,196,217);
                    strcat_s(cardsCell, sizeof(cardsCell)/sizeof(cardsCell[0]), tmp);
                }
                printf("|%10s|%-40s|%6s|%19s|\n"," ", cardsCell," "," "," ");
                break;
            }
        }
        printf("%s\n", singleLine);
    }

    //莊家手牌

    tmpHand = bankerHands+0;//莊家只會有一手牌，所以 +0
    //判斷是否該莊家出牌

    //計算這一手牌目前發了幾張牌
    for(c=0,cardCount=0; c<MAX_DEAL_NUMBER; c++){
        if(*(tmpHand->card+c)>=0)
            cardCount++;
    }
    for(i=0; i<4; i++){//每張卡牌的高佔4列
        switch(i){
        case 0://第一列
            cardsCell[0]='\0';
            //顯示每張手牌的第一列
            for(c=0; c<cardCount; c++){
                sprintf(tmp,"%c%c%c%c%c%c",32,218,196,196,196,191);
                strcat_s(cardsCell, sizeof(cardsCell)/sizeof(cardsCell[0]), tmp);
            }
            printf("|%10s|%-40s|%6s|%19s|\n"," ", cardsCell," "," "," ");
            break;
        case 1://第二列
            cardsCell[0]='\0';
            //顯示每張手牌的第二列
            for(c=0; c<cardCount; c++){
                //轉換 suit 為 ascii
                switch((cards+*(tmpHand->card+c))->suit){
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
                sprintf(tmp,"%c%c%c%c%c%c",32,179,ascii,32,32,179);
                strcat_s(cardsCell, sizeof(cardsCell)/sizeof(cardsCell[0]), tmp);
            }
            printf("| %8s |%-40s|  %2d  |%19s|\n","Banker",cardsCell,tmpHand->safeHighPoint," "," ");
            break;
        case 2://第三列
            cardsCell[0]='\0';
            //顯示每張手牌的第三列
            for(c=0; c<cardCount; c++){
                sprintf(tmp,"%c%c%c%2s%c",32,179,32,(cards+*(tmpHand->card+c))->face,179,32);
                strcat_s(cardsCell, sizeof(cardsCell)/sizeof(cardsCell[0]), tmp);
            }
            printf("|%10s|%-40s|%6s|%19s|\n"," ",cardsCell," "," "," ");
            break;
        case 3://第四列
            cardsCell[0]='\0';
            //顯示每張手牌的第一列
            for(c=0; c<cardCount; c++){
                sprintf(tmp,"%c%c%c%c%c%c",32,192,196,196,196,217);
                strcat_s(cardsCell, sizeof(cardsCell)/sizeof(cardsCell[0]), tmp);
            }
            printf("|%10s|%-40s|%6s|%19s|\n"," ", cardsCell," "," "," ");
            break;
        }
    }
    printf("%s\n", singleLine);


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

    printf("Player name=%s chip=%d play=%d win=%d draw=%d loss=%d blackjack=%d\n",
           player->name,
           player->chip,
           player->playCount,
           player->winCount,
           player->drawCount,
           player->lossCount,
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
    int highPoint, safeHightPoint, lowPoint;

    printf("%s hand bet=%d split=%s insurance=%s double=%s surrender=%s stand=%s bust=%s point=%d,%d,%d",
           (hand->banker?"Banker":"Player"),
           hand->bet,
           hand->split?"O":"X",
           hand->insurance?"O":"X",
           hand->doubleDown?"O":"X",
           hand->surrender?"O":"X",
           hand->stand?"O":"X",
           hand->bust?"O":"X",
           hand->highPoint,
           hand->safeHighPoint,
           hand->lowPoint
           );

    printf(" card=[");
    for(j=0; j<MAX_DEAL_NUMBER; j++){
        if(j==0)
            printf("%d",*(hand->card+j));
        else
            printf(",%d",*(hand->card+j));
    }
    printf("]");

    printf(" card=[");
    for(j=0; j<MAX_DEAL_NUMBER; j++){
        k = *(hand->card+j);
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

    computeHand(hand, cards, &highPoint, &safeHightPoint, &lowPoint);
    printf("] high=%d safe=%d low=%d\n",highPoint, safeHightPoint, lowPoint);
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
    int i,j;
    Card* card;

    //重新給予order值
    for(i=0; i<DECK_NUMBER; i++){
        for(j=0; j<CARD_NUMBER; j++){
            card = cards+i*CARD_NUMBER+j;
            card->order = rand();
        }
    }

    //依order值排序
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
    int safeHightPoint;
    int highPoint;
    int lowPoint;
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
            if(*(hand->card+j)==-1){ // -1代表尚未被發牌
                *(hand->card+j) = *nextCardIndex;
                //卡牌計次
                (cards+(*nextCardIndex))->appeared+=1;
                //發牌後，將下一張牌的index++
                (*nextCardIndex)++;
                isDeal = TRUE;
                break;
            }
        }

        if(isDeal){
            highPoint=0;
            safeHightPoint=0;
            lowPoint=0;
            computeHand(hand, cards, &highPoint, &safeHightPoint, &lowPoint);
            break;
        }
    }
}

/**
 * @brief 計算手牌點數並更新狀態
 * @param hand
 * @param cards
 */
void computeHand(Hand* hand, Card* cards, int* highPoint, int* safeHightPoint, int* lowPoint){
    int i,cardIdx;
    Card* card;
    int points[MAX_DEAL_NUMBER][CARD_POINT_POSSIBILITY]={0};

    //將手牌點數整理成二維陣列，後續將進一步取排列組合之最大最小值
    for(i=0; i<MAX_DEAL_NUMBER; i++){
        cardIdx=*(hand->card+i);
        if(cardIdx<0) // 代表尚未被發牌
            break;
        card = cards+cardIdx;
        points[i][0]=card->point;
        points[i][1]=card->altPoint;
    }
    getHighLowPoints(*points, highPoint, safeHightPoint, lowPoint);

    hand->highPoint = *highPoint;
    hand->safeHighPoint = *safeHightPoint;
    hand->lowPoint = *lowPoint;

    if(*lowPoint > 21)
        hand->bust = TRUE;
}

/**
 * @brief 從手牌各張的可能點數做排列組合取最大值和最小值
 * @param arr 二維陣列倒退一維的指標
 * @see https://www.geeksforgeeks.org/combinations-from-n-arrays-picking-one-element-from-each-array/
 */
void getHighLowPoints(int* arr, int* highPoint, int* safeHightPoint, int* lowPoint){
    int i,point,sum,next;
    int indices[MAX_DEAL_NUMBER] ={0};
    int candidate[MAX_POINTS_COMBINATION];
    int candidatePush=0;
    int high=0;
    int low=0;
    int safeHigh=0;

    //初始化陣列
    for(i=0; i<MAX_POINTS_COMBINATION; i++)
        candidate[i]=0;

    while (1) {
        sum=0;
        for (i = 0; i < MAX_DEAL_NUMBER; i++){
            point = *(arr+i*CARD_POINT_POSSIBILITY+indices[i]);
            sum+=point;
//            if(i)
//                printf("+%d",point);
//            else
//                printf("%d",point);
        }
//        printf("=%d  ",sum);
        candidate[candidatePush++]=sum;

        next = MAX_DEAL_NUMBER - 1;
        while (next >= 0 && (indices[next] + 1 >= CARD_POINT_POSSIBILITY))
            next--;

        if (next < 0)
            break;

        indices[next]++;

        for (i = next + 1; i < MAX_DEAL_NUMBER; i++)
            indices[i] = 0;
    }

    //找最高分、最低分
    for(i=0,high=candidate[0],low=candidate[0]; i<MAX_POINTS_COMBINATION; i++){
        if(candidate[i]<low)
            low = candidate[i];
        if(candidate[i]>high)
            high = candidate[i];
    }
    //找安全最高分
    for(i=0,safeHigh=low; i<MAX_POINTS_COMBINATION; i++){
        if(candidate[i]<=21 && candidate[i]>safeHigh)
            safeHigh = candidate[i];
    }

    *highPoint=high;
    *safeHightPoint=safeHigh;
    *lowPoint=low;

    //列出點數排列組合可能值
//    printf("\n[");
//    for(i=0; i<MAX_POINTS_COMBINATION; i++){
//        if(i==0)
//            printf("%d",candidate[i]);
//        else
//            printf(",%d",candidate[i]);
//    }
//    printf("] high=%d safe=%d low=%d\n", high, safeHigh, low);
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
        dummyPlayers[i].drawCount = 0;
        dummyPlayers[i].lossCount = 0;
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
               dummyPlayers[i].drawCount,
               dummyPlayers[i].lossCount,
               dummyPlayers[i].blackjackCount
               );
    }

    pressAnyKeyToContinue();
}
