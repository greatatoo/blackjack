#include <stdio.h>
#include <time.h>
#include <windows.h>

typedef struct{
    char name[11];	//名稱
    int score;	//籌碼
    int win;	//勝場數
    int play;	//遊玩場數
    int blackjack;	//黑傑克次數
}Data;	//玩家的資料

int suit[]={6,6,6,6,6,6,6,6,6,6,6,6,6,
            3,3,3,3,3,3,3,3,3,3,3,3,3,
            4,4,4,4,4,4,4,4,4,4,4,4,4,
            5,5,5,5,5,5,5,5,5,5,5,5,5,
            6,6,6,6,6,6,6,6,6,6,6,6,6,
            3,3,3,3,3,3,3,3,3,3,3,3,3,
            4,4,4,4,4,4,4,4,4,4,4,4,4,
            5,5,5,5,5,5,5,5,5,5,5,5,5,};	//花色牌堆

int face[]={1,2,3,4,5,6,7,8,9,10,11,12,13,
            1,2,3,4,5,6,7,8,9,10,11,12,13,
            1,2,3,4,5,6,7,8,9,10,11,12,13,
            1,2,3,4,5,6,7,8,9,10,11,12,13,
            1,2,3,4,5,6,7,8,9,10,11,12,13,
            1,2,3,4,5,6,7,8,9,10,11,12,13,
            1,2,3,4,5,6,7,8,9,10,11,12,13,
            1,2,3,4,5,6,7,8,9,10,11,12,13};	//點數牌堆

int playerCard[2][10]={{0,0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0,0}};	//閒家手牌
int computerCard[2][10]={{0,0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0,0}};	//莊家手牌

int playerCardNumber=0,computerCardNumber=0,cardNumber=104;	//牌數
int playerPoint=0,computerPoint=0;	//點數和

int i,j;	//計數
int choose1=0,choose2=0,choose3,choose4,choose5=0;	//玩家的選擇
int status=0;	//遊戲結果

Data playerData[5];	//五個玩家資料的陣列

main(){
    while(choose1!=3){
        mainInterface();
        switch(choose1){
            case 1:
                readyInterface();
                shuffle();
                choose5=0;
                while(choose5!=9){
                    if(cardNumber>52){
                        betInterface();
                        if(choose5!=9)
                        play();
                        resultInterface();
                    }
                    else{
                        shuffle();
                        cardNumber=104;
                    }
                }
                break;
            case 2:
                recordInterface();
            default:
                break;
        }
    }
}

swap(int *a,int *b){
    int temp=*a;
    *a=*b;
    *b=temp;
}	//交換指標內容 (兩個指標)

randomize(int arr[]){
    srand(time(NULL));
    for(i=51;i>=1;i--){
        int j=rand()%(i+1);
        swap(&arr[i],&arr[j]);
    }
}	//洗牌演算法 (欲洗亂的陣列)

shuffle(){
    randomize(suit);
    randomize(face);
}	//洗牌

deal(int arr[2][10],int *a,int b){
    for(i=1;i<=b;i++){
        arr[0][(i-1)+*a]=suit[(i-1)+playerCardNumber+computerCardNumber+(104-cardNumber)];
        arr[1][(i-1)+*a]=face[(i-1)+playerCardNumber+computerCardNumber+(104-cardNumber)];
    }
    *a=*a+b;
    cardNumber-=b;
}	//由牌堆發牌到玩家手牌(發牌目的地，該副牌現有a張，發入b張)

counter(int arr[2][10],int *a){
    *a=0;
    j=0;
    for(i=1;i<=10;i++){
        switch(arr[1][i-1]){
            case 1:
                *a+=1;
                j++;
                break;
            case 11:
                *a+=10;
                break;
            case 12:
                *a+=10;
                break;
            case 13:
                *a+=10;
                break;
            default:
                *a+=arr[1][i-1];
                break;
        }
    }
    for(i=0;i<=j;i++){
        if(*a+10*i<=21)
        *a=*a+10*i;
    }
}	//計算一副牌的最大牌點(欲計算的牌組，該副牌的牌點變數)

reset(){
    status=0;
    playerCardNumber= 0;
    computerCardNumber= 0;
    playerPoint= 0;
    computerPoint= 0;
    for(i=1;i<=2;i++){
        for(j=1;j<=10;j++){
            playerCard[i-1][j-1]= 0;
            computerCard[i-1][j-1]= 0;
        }
    }
}	//重置遊戲相關變數

result(){
    if(playerPoint<=21){
        if(computerPoint>21){
            status=5;
        }
        else{
            if(playerPoint>computerPoint){
                status=5;
            }
            if(playerPoint<computerPoint){
                status=1;
            }
            if(playerPoint==computerPoint){
                status=3;
            }
        }
    }
    else{
        status=1;
    }
}	//判斷勝負

bubbleSort(){
    for(i=1;i<=5;i++){
        for(j=4;j>=1;j--){
            if(playerData[j].score>playerData[j-1].score){
                Data temp=playerData[j];
                playerData[j]=playerData[j-1];
                playerData[j-1]=temp;
            }
        }
    }
}	//玩家的資料依籌碼排序

readData(){
    FILE *fp;
    if((fp=fopen("data.txt","r"))!=NULL){
        for(i=0;i<=4;i++){
            fscanf(fp,"%s%d%d%d%d",playerData[i].name,&(playerData[i].score),&(playerData[i].win),&(playerData[i].play),&(playerData[i].blackjack));
        }
        fclose(fp);
    }
    else{
        for(i=0;i<=4;i++){
            strcpy(playerData[i].name,"none");
            playerData[i].score=10000;
            playerData[i].win=0;
            playerData[i].play=0;
            playerData[i].blackjack=0;
        }
    }
}	//讀取玩家資料

writeData(){
    FILE *fp;
    fp=fopen("data.txt","w");
    for(i=0;i<=4;i++){
        fprintf(fp,"%s %d %d %d %d\n",playerData[i].name,playerData[i].score,playerData[i].win,playerData[i].play,playerData[i].blackjack);
    }
    fclose(fp);
}	//輸出玩家資料

printCard(int arr[2][10],int n){
    int faceAscii[]={65,50,51,52,53,54,55,56,57,84,74,81,75};

    SetConsoleOutputCP(437);
    for(i=1;i<=n;i++)
    printf("%c%c%c%c%c%c%c%c\t",218,196,196,196,196,196,196,191);
    printf("\n");
    for(i=1;i<=n;i++)
    printf("%c      %c\t",179,179);
    printf("\n");
    for(i=1;i<=n;i++)
    printf("%c %c    %c\t",179,arr[0][i-1],179);
    printf("\n");
    for(i=1;i<=n;i++)
    printf("%c      %c\t",179,179);
    printf("\n");
    for(i=1;i<=n;i++){
        int k=arr[1][i-1];
        printf("%c    %c %c\t",179,faceAscii[k-1],179);
    }
    printf("\n");
    for(i=1;i<=n;i++)
    printf("%c      %c\t",179,179);
    printf("\n");
    for(i=1;i<=n;i++)
    printf("%c%c%c%c%c%c%c%c\t",192,196,196,196,196,196,196,217);
    printf("\n");
    SetConsoleOutputCP(950);
}	//印出卡牌(欲印出的牌組，印出n張)

mainInterface(){
    system("cls");
    printf("【BlackJack】\n\n");
    printf("【Rule】\n");
    printf("這是一個閒家(你)跟莊家(電腦)比大小的遊戲\n");
    printf("手牌的點數和較大者贏，但不能超過21點\n");
    printf("2~9對應著2~9點\n");
    printf("10,J,Q,K皆為10點\n");
    printf("A可視為1點或11點\n");
    printf("開局莊家會發給閒家和自己兩張牌，且莊家的第二張牌為暗牌\n");
    printf("閒家可以視手上的牌點決定是否加牌(不限加牌次數)\n");
    printf("閒家完成加牌後，莊家若點數和小於17點則必須加牌\n");
    printf("在閒家和莊家加牌完成後進行勝負結算\n");
    printf("備註:本遊戲以2副撲克牌進行遊玩。\n\n");
    printf("【Menu】\n");
    printf("請輸入:[1]開始遊戲 [2]查看排行 [3]關閉遊戲......");
    scanf("%d",&choose1);
}	//主畫面

readyInterface(){
    readData();
    bubbleSort();
    system("cls");
    printf("No.     name     score   win_rate\n");
    for(i=0;i<=4;i++){
        printf("%2d%10s%10d%10.2f%%\n",i+1,playerData[i].name,playerData[i].score,playerData[i].play==0? 0:((float)playerData[i].win/(float)playerData[i].play)*100);
    }
    printf("請選擇人物(輸入編號):");
    do{
        scanf("%d",&choose4);
    }while(choose4<1||choose4>5);
    printf("請選擇動作:[1]開始遊戲 [2]重置資料......");
    scanf("%d",&choose3);
    if(choose3==2){
        strcpy(playerData[choose4-1].name,"none");
        playerData[choose4-1].score=10000;
        playerData[choose4-1].win=0;
        playerData[choose4-1].play=0;
        playerData[choose4-1].blackjack=0;
    }
    if(strcmp(playerData[choose4-1].name,"none")==0){
        printf("請命名:");
        scanf("%s",playerData[choose4-1].name);
    }
}	//遊戲準備畫面

playInterface(int a,int b){
    system("cls");
    printf("玩家:%s  現有籌碼:%d  此次下注金額:%d\t牌堆尚餘 %d 張牌\n\n",playerData[choose4-1].name,playerData[choose4-1].score,1000*choose5,cardNumber);
    printf("莊家的牌:\n");
    printCard(computerCard,a);
    printf("你的手牌:\n");
    printCard(playerCard,b);
    printf("請選擇:[1]要牌 [2]pass ......");
}	//遊玩時的畫面 (顯示莊家a張牌，閒家b張牌)

betInterface(){
    system("cls");
    printf("玩家: %s 現有籌碼: %d 牌堆尚餘 %d 張牌\n\n",playerData[choose4-1].name,playerData[choose4-1].score,cardNumber);
    printf("莊家的牌:\n\n\n\n\n\n\n\n你的手牌:\n\n\n\n\n\n\n\n");
    do{
        printf("請選擇下注金額:[1] 1000 [3] 3000 [9]離開......");
        scanf("%d",&choose5);
    }while(choose5!=1&&choose5!=3&&choose5!=9);
    if(choose5==1||choose5==3)
    playerData[choose4-1].score-=1000*choose5;
}

resultInterface(){
    if(choose5!=9){
        system("cls");
        playerData[choose4-1].play+=1;
        printf("本次遊戲結果: ");
        switch(status){
            case 1:
                printf("莊家獲勝!! 現有籌碼: %d\n",playerData[choose4-1].score);
                break;
            case 2:
                printf("莊家的牌為黑傑克!! 莊家獲勝!! 現有籌碼: %d\n",playerData[choose4-1].score);
                break;
            case 3:
                printf("雙方點數相同!! +%d 現有籌碼: %d\n",1000*choose5,playerData[choose4-1].score);
                playerData[choose4-1].score+=1000*choose5;
                break;
            case 4:
                playerData[choose4-1].blackjack+=1;
                printf("雙方同為黑傑克!! +%d 現有籌碼: %d\n",1000*choose5,playerData[choose4-1].score);
                playerData[choose4-1].score+=1000*choose5;
                break;
            case 5:
                playerData[choose4-1].score+=1000*choose5*2;
                playerData[choose4-1].win+=1;
                printf("你獲勝!! +%d 現有籌碼: %d\n",1000*choose5*2,playerData[choose4-1].score);
                break;
            case 6:
                playerData[choose4-1].score+=1000*choose5*3;
                playerData[choose4-1].win+=1;
                playerData[choose4-1].blackjack+=1;
                printf("你獲勝!! +%d 現有籌碼: %d\n",1000*choose5*3,playerData[choose4-1].score);
                break;
        }
        printf("\n莊家的牌:\n");
        printCard(computerCard,computerCardNumber);
        printf("你的手牌:\n");
        printCard(playerCard,playerCardNumber);
        system("pause");
    }
    if(cardNumber<=52){
        cardNumber==104;
        system("cls");
        printf("牌堆剩餘張數不足一半!! 重新洗牌!!\n");
        printf("目前遊玩統計:\n");
        printf("玩家:%s\n",&playerData[choose4-1].name);
        printf("現有籌碼:%d\n",playerData[choose4-1].score);
        printf("勝場數:%d\n",playerData[choose4-1].win);
        printf("遊玩場數:%d\n",playerData[choose4-1].play);
        printf("\n");
        system("pause");
    }
    if(choose5==9){
        writeData();
        system("cls");
        printf("遊玩數據統計:\n");
        printf("玩家:%s\n",&playerData[choose4-1].name);
        printf("現有籌碼:%d\n",playerData[choose4-1].score);
        printf("勝場數:%d\n",playerData[choose4-1].win);
        printf("遊玩場數:%d\n",playerData[choose4-1].play);
        printf("\n");
        system("pause");
    }
}	//結算畫面

recordInterface(){
    readData();
    bubbleSort();
    system("cls");
    printf("現時排行:\nNo.     name    score   win   play   win_rate   blackjack\n");
    for(i=0;i<=4;i++){
        printf("%2d%10s%9d%6d%7d%10.2f%%%12d\n",
        i+1,playerData[i].name,
        playerData[i].score,
        playerData[i].win,
        playerData[i].play,
        playerData[i].play==0? 0:((float)playerData[i].win/(float)playerData[i].play)*100,
        playerData[i].blackjack);
    }
    printf("\n");
    system("pause");
}

play(){
    reset();
    deal(computerCard,&computerCardNumber,2);
    deal(playerCard,&playerCardNumber,2);
    counter(computerCard,&computerPoint);
    counter(playerCard,&playerPoint);
    playInterface(1,playerCardNumber);
    if(computerPoint==21||playerPoint==21){
        result();
        status++;
        printf("勝負已分!! 3秒後自動切換...");
        Sleep(3000);
    }
    while(status==0){
        scanf("%d",&choose2);
        Sleep(1000);
        switch(choose2){
            case 1:
                deal(playerCard,&playerCardNumber,1);
                counter(playerCard,&playerPoint);
                playInterface(1,playerCardNumber);
                if(playerPoint>21){
                    result();
                    printf("你已爆牌! 3秒後將自動切換...");
                    Sleep(3000);
                }
                break;
            case 2:
                playInterface(computerCardNumber,playerCardNumber);
                while(computerPoint<17){
                    deal(computerCard,&computerCardNumber,1);
                    counter(computerCard,&computerPoint);
                    Sleep(1000);
                    playInterface(computerCardNumber,playerCardNumber);
                }
                Sleep(1000);
                result();
                break;
            default:
                printf("請選擇:[1]要牌 [2]pass ......");
                break;
        }
    }
}	//遊玩函式
