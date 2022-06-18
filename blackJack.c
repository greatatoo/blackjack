#include <stdio.h>
#include <time.h>
#include <windows.h>

#define PLAYER 5	//���a��ƪ��ƶq 

typedef struct{
    char name[11];	//�W��
    int score;		//�w�X
    int win;		//�ӳ���
    int play;		//�`����
    int blackjack;	//���o�³ǧJ����
}Data;				//���a����� 

int suit[]={6,6,6,6,6,6,6,6,6,6,6,6,6,  //�®窺 ascii �X
            3,3,3,3,3,3,3,3,3,3,3,3,3,	//���ߪ� ascii �X
            4,4,4,4,4,4,4,4,4,4,4,4,4,	//����� ascii �X
            5,5,5,5,5,5,5,5,5,5,5,5,5,	//���᪺ ascii �X
            6,6,6,6,6,6,6,6,6,6,6,6,6,
            3,3,3,3,3,3,3,3,3,3,3,3,3,
            4,4,4,4,4,4,4,4,4,4,4,4,4,
            5,5,5,5,5,5,5,5,5,5,5,5,5};	//���P��

int face[]={1,2,3,4,5,6,7,8,9,10,11,12,13,
            1,2,3,4,5,6,7,8,9,10,11,12,13,
            1,2,3,4,5,6,7,8,9,10,11,12,13,
            1,2,3,4,5,6,7,8,9,10,11,12,13,
            1,2,3,4,5,6,7,8,9,10,11,12,13,
            1,2,3,4,5,6,7,8,9,10,11,12,13,
            1,2,3,4,5,6,7,8,9,10,11,12,13,
            1,2,3,4,5,6,7,8,9,10,11,12,13};	//�I�ƵP��

int playerCard[2][10]={{0,0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0,0}};	//���a��P 
int computerCard[2][10]={{0,0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0,0}};	//���a��P 

int playerCardNumber=0,computerCardNumber=0,cardNumber=104;	//���a��P�ơB���a��P�ơB�P��Ѿl�P�� 
int playerPoint=0,computerPoint=0;	//���a�β��a���I�ƩM 

int i,j;	//�p�� 
int choose1=1,choose2=0,choose3,choose4,choose5=0;	//�������a����� 
int status=0;	//�C�����G  

Data playerData[PLAYER];	//���a��ƪ��}�C 

char temp[255];

int main(){
    while(choose1!=5){
        system("cls");
        printf("<BlackJack>\n\n");
        printf("Please select an option:\n\n[1]Start [2]Charts [3]Rules [4]Search [5]Exit......");
        scanf("%d",&choose1);
        switch(choose1){
        case 1:
            readyInterface();
            choose5=0;
            ticket();
            shuffle();
            while(choose5!=9){
                betInterface();
                if(choose5!=9)
                    play();
                resultInterface();
            }
            writeData();
            break;
        case 2:
            recordInterface();
            break;
        case 3:
            ruleInterface();
            break;
        case 4:
            binarySearch();
            break;
        default:
            break;
        }
    }
    return 0;
}

swap(int *a,int *b){
    int temp=*a;
    *a=*b;
    *b=temp;
}	//�洫����Ф��e (��ӫ���) 

randomize(int arr[]){
    srand(time(NULL));
    for(i=51;i>=1;i--){
        int j=rand()%(i+1);
        swap(&arr[i],&arr[j]);
    }
}	//���ð}�C���e (���~�ê��}�C) 

shuffle(){
    randomize(suit);
    randomize(face);
}	//�~�P 

deal(int arr[2][10],int *a,int b){
    for(i=1;i<=b;i++){
        arr[0][(i-1)+*a]=suit[(i-1)+playerCardNumber+computerCardNumber+(104-cardNumber)];
        arr[1][(i-1)+*a]=face[(i-1)+playerCardNumber+computerCardNumber+(104-cardNumber)];
    }
    *a=*a+b;
    cardNumber-=b;
}	//�o�P (�o�P��H����P�}�C�A�ӰƵP���P���ܼơA���o�J���i��) 

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
}	//�p��@�ƵP���̤j�P�I (���p�⪺��P�}�C�A�ӰƵP���P�I�ܼ�) 

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
}	//���m�C�������ܼ� 

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
}	//�P�_�ӭt 	

bubbleSort(){
    for(i=1;i<=PLAYER;i++){
        for(j=PLAYER-1;j>=1;j--){
            if(playerData[j].score>playerData[j-1].score){
                Data temp=playerData[j];
                playerData[j]=playerData[j-1];
                playerData[j-1]=temp;
            }
        }
    }
}	//�N���a����ƨ��w�X�Ƨ�  

binarySearch(){
    system("cls");
    readData();
    for(i=1;i<=PLAYER;i++){
        for(j=PLAYER-1;j>=1;j--){
            if(strcmp(playerData[j].name,playerData[j-1].name)==-1){
                Data temp=playerData[j];
                playerData[j]=playerData[j-1];
                playerData[j-1]=temp;
            }
        }
    }

    do{
        printf("Please enter the name......");
        scanf("%s",temp);
    }while(strcmp(temp,"none")==0);

    int low=0,high=PLAYER-1,middle=(PLAYER-1)/2;

    while(strcmp(temp,playerData[middle].name)!=0){
        middle=(low+high)/2;
        if(strcmp(temp,playerData[middle].name)==1){
            low=middle+1;
        }
        if(strcmp(temp,playerData[middle].name)==-1){
            high=middle-1;
        }
        if(low>high){
            break;
        }
    }

    if(low>high){
        printf("\nNot found\n\n");
    }
    else{
        printf("\n      name    chips   wins  plays  win rate   blackjacks\n");
        printf("%10s%9d%7d%7d%9.2f%%%13d\n\n",
               playerData[middle].name,
               playerData[middle].score,
               playerData[middle].win,
               playerData[middle].play,
               playerData[middle].play==0? 0:((float)playerData[middle].win/(float)playerData[middle].play)*100,
               playerData[middle].blackjack);
    }
    system("pause");
}	//�H�W�ٷj�M�Ӫ��a����� 

readData(){
    FILE *fp;
    if((fp=fopen("data.txt","r"))!=NULL){
        for(i=0;i<=PLAYER-1;i++){
            fscanf(fp,"%s%d%d%d%d",playerData[i].name,&(playerData[i].score),&(playerData[i].win),&(playerData[i].play),&(playerData[i].blackjack));
        }
        fclose(fp);
    }
    else{
        for(i=0;i<=PLAYER-1;i++){
            strcpy(playerData[i].name,"none");
            playerData[i].score=0;
            playerData[i].win=0;
            playerData[i].play=0;
            playerData[i].blackjack=0;
        }
    }
}	//Ū�����a��� 

writeData(){
    FILE *fp;
    fp=fopen("data.txt","w");
    for(i=0;i<=PLAYER-1;i++){
        fprintf(fp,"%s %d %d %d %d\n",playerData[i].name,playerData[i].score,playerData[i].win,playerData[i].play,playerData[i].blackjack);
    }
    fclose(fp);
}	//��X���a��� 

ticket(){
    system("cls");
    if(playerData[choose4-1].score<1000){
        choose5=9;
        printf("You have no chips!!\n\n");
        system("pause");
    }
}	//�ˬd�w�X�O�_���� 

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
    //	SetConsoleOutputCP(950);
}	//�L�X�d�P (���L�X����P�}�C�A�L�Xn�i) 

readyInterface(){
    readData();
    bubbleSort();
    system("cls");
    printf("No.     name     chips   win rate\n");
    for(i=0;i<=PLAYER-1;i++){
        printf("%2d%10s%10d%10.2f%%\n",i+1,playerData[i].name,playerData[i].score,playerData[i].play==0? 0:((float)playerData[i].win/(float)playerData[i].play)*100);
    }
    do{
        printf("\nPlease select a character (Enter number):");
        scanf("%d",&choose4);
    }while(choose4<1||choose4>PLAYER);
    printf("\nPlease select an option:[1]Start [2]Reset ......");
    scanf("%d",&choose3);
    if(choose3==2){
        strcpy(playerData[choose4-1].name,"none");
        playerData[choose4-1].win=0;
        playerData[choose4-1].play=0;
        playerData[choose4-1].blackjack=0;
    }
    if(strcmp(playerData[choose4-1].name,"none")==0){
        playerData[choose4-1].score=10000;
        do{
            printf("\nPlease enter your name(No more than 10 characters):");
            scanf("%s",temp);
        }while(strlen(temp)>10);
        strcpy(playerData[choose4-1].name,temp);
    }
}	//�C���ǳƤ��� 

playInterface(int a,int b){
    system("cls");
    printf("Player:%s\tChips:%d\tBet:%d\t%d cards left in deck\n\n",playerData[choose4-1].name,playerData[choose4-1].score,1000*choose5,cardNumber);
    printf("Dealer's card:\n");
    printCard(computerCard,a);
    printf("your card:\n");
    printCard(playerCard,b);
    printf("Please select an option:[1]Hit [2]Stand ......");
}	//�C���ɤ��� (��ܲ��aa�i�P�A���ab�i�P) 

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
        printf("Game over!!\n\n");
        system("pause");
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
                printf("Bust!\n\n");
                system("pause");
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
            Sleep(1500);
            result();
            break;
        default:
            printf("Please select an option:[1]Hit [2]Stand ......");
            break;
        }
    }
}	//�C���禡 

betInterface(){
    system("cls");
    printf("Player:%s\tCurrent chips:%d\t%d cards left in deck\n\n",playerData[choose4-1].name,playerData[choose4-1].score,cardNumber);
    printf("Dealer's card:\n\n\n\n\n\n\n\nYour card:\n\n\n\n\n\n\n\n");
    do{
        printf("Wager:[1]1000 [3]3000 [9]Exit......");
        scanf("%d",&choose5);
    }while((choose5!=1&&choose5!=3&&choose5!=9)||(choose5==3&&playerData[choose4-1].score<3000));

    if(choose5==1||choose5==3)
        playerData[choose4-1].score-=1000*choose5;
}	//�U�`���� 

resultInterface(){
    if(choose5!=9){
        system("cls");
        playerData[choose4-1].play+=1;
        printf("Game result: ");
        switch(status){
        case 1:
            printf("Dealer wins!!\tCurrent chips: %d\n",playerData[choose4-1].score);
            break;
        case 2:
            printf("Dealer gets BLACKJACK!! Dealer wins!!\tCurrent chips: %d\n",playerData[choose4-1].score);
            break;
        case 3:
            playerData[choose4-1].score+=1000*choose5;
            printf("Draw!! +%d\tCurrent chips: %d\n",1000*choose5,playerData[choose4-1].score);
            break;
        case 4:
            playerData[choose4-1].blackjack+=1;
            playerData[choose4-1].score+=1000*choose5;
            printf("Draw!! +%d\tCurrent chips: %d\n",1000*choose5,playerData[choose4-1].score);
            break;
        case 5:
            playerData[choose4-1].score+=1000*choose5*2;
            playerData[choose4-1].win+=1;
            printf("You win!! +%d\tCurrent chips: %d\n",1000*choose5*2,playerData[choose4-1].score);
            break;
        case 6:
            playerData[choose4-1].score+=1000*choose5*3;
            playerData[choose4-1].win+=1;
            playerData[choose4-1].blackjack+=1;
            printf("You win!! +%d\tCurrent chips: %d\n",1000*choose5*3,playerData[choose4-1].score);
            break;
        }
        printf("\nDealer's card:\n");
        printCard(computerCard,computerCardNumber);
        printf("Your card:\n");
        printCard(playerCard,playerCardNumber);
        system("pause");
    }
    if(cardNumber<=52){
        cardNumber=104;
        shuffle();
        system("cls");
        printf("Less than half of the cards remaining in the deck!!\nReshuffle!!\n\n");
        printf("<Play history>\n");
        printf("Player:%s\n",&playerData[choose4-1].name);
        printf("Current chips:%d\n",playerData[choose4-1].score);
        printf("Wins:%d\n",playerData[choose4-1].win);
        printf("Plays:%d\n",playerData[choose4-1].play);
        printf("\n");
        system("pause");
    }
    ticket();
}	//���⤶�� 

recordInterface(){
    readData();
    bubbleSort();
    system("cls");
    printf("Current ranking: (According to chips)\n\nNo.     name    chips   wins  plays  win rate   blackjacks\n");
    for(i=0;i<=PLAYER-1;i++){
        printf("%2d%10s%9d%7d%7d%9.2f%%%13d\n",
               i+1,playerData[i].name,
               playerData[i].score,
               playerData[i].win,
               playerData[i].play,
               playerData[i].play==0? 0:((float)playerData[i].win/(float)playerData[i].play)*100,
               playerData[i].blackjack);
    }
    printf("\n");
    system("pause");
}	//���a�Ʀ椶��  

ruleInterface(){
    system("cls");
    printf("This is a game that player and the dealer compare the hand points.\n\n");
    printf("The hand with the higher value wins, but cannot exceed 21.\n\n");
    printf("2~9 corresponds to 2~9 points.\n\n");
    printf("10,J,Q,K are all 10 points.\n\n");
    printf("A can be regarded as 1 point or 11 points.\n\n");
    printf("First, the dealer will deal two cards to the player and himself.\n\n");
    printf("The dealer's second card is invisible.\n\n");
    printf("Player can decide whether to HIT or not.\n\n");
    printf("The dealer must HIT if the hand points is less than 17.\n\n");
    printf("Note: This game is played with 2 decks of cards.\n\n");
    system("pause");
}	//�W�h�������� 
