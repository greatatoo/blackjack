#include <stdio.h>
#include <time.h>
#include <windows.h>

typedef struct{
    char name[11];	//�W��
    int score;	//�w�X
    int win;	//�ӳ���
    int play;	//�C������
    int blackjack;	//�³ǧJ����
}Data;	//���a�����

int suit[]={6,6,6,6,6,6,6,6,6,6,6,6,6,
            3,3,3,3,3,3,3,3,3,3,3,3,3,
            4,4,4,4,4,4,4,4,4,4,4,4,4,
            5,5,5,5,5,5,5,5,5,5,5,5,5,
            6,6,6,6,6,6,6,6,6,6,6,6,6,
            3,3,3,3,3,3,3,3,3,3,3,3,3,
            4,4,4,4,4,4,4,4,4,4,4,4,4,
            5,5,5,5,5,5,5,5,5,5,5,5,5,};	//���P��

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

int playerCardNumber=0,computerCardNumber=0,cardNumber=104;	//�P��
int playerPoint=0,computerPoint=0;	//�I�ƩM

int i,j;	//�p��
int choose1=0,choose2=0,choose3,choose4,choose5=0;	//���a�����
int status=0;	//�C�����G

Data playerData[5];	//���Ӫ��a��ƪ��}�C

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
}	//�洫���Ф��e (��ӫ���)

randomize(int arr[]){
    srand(time(NULL));
    for(i=51;i>=1;i--){
        int j=rand()%(i+1);
        swap(&arr[i],&arr[j]);
    }
}	//�~�P�t��k (���~�ê��}�C)

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
}	//�ѵP��o�P�쪱�a��P(�o�P�ت��a�A�ӰƵP�{��a�i�A�o�Jb�i)

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
}	//�p��@�ƵP���̤j�P�I(���p�⪺�P�աA�ӰƵP���P�I�ܼ�)

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
    for(i=1;i<=5;i++){
        for(j=4;j>=1;j--){
            if(playerData[j].score>playerData[j-1].score){
                Data temp=playerData[j];
                playerData[j]=playerData[j-1];
                playerData[j-1]=temp;
            }
        }
    }
}	//���a����ƨ��w�X�Ƨ�

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
}	//Ū�����a���

writeData(){
    FILE *fp;
    fp=fopen("data.txt","w");
    for(i=0;i<=4;i++){
        fprintf(fp,"%s %d %d %d %d\n",playerData[i].name,playerData[i].score,playerData[i].win,playerData[i].play,playerData[i].blackjack);
    }
    fclose(fp);
}	//��X���a���

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
}	//�L�X�d�P(���L�X���P�աA�L�Xn�i)

mainInterface(){
    system("cls");
    printf("�iBlackJack�j\n\n");
    printf("�iRule�j\n");
    printf("�o�O�@�Ӷ��a(�A)����a(�q��)��j�p���C��\n");
    printf("��P���I�ƩM���j��Ĺ�A������W�L21�I\n");
    printf("2~9������2~9�I\n");
    printf("10,J,Q,K�Ҭ�10�I\n");
    printf("A�i����1�I��11�I\n");
    printf("�}�����a�|�o�����a�M�ۤv��i�P�A�B���a���ĤG�i�P���t�P\n");
    printf("���a�i�H����W���P�I�M�w�O�_�[�P(�����[�P����)\n");
    printf("���a�����[�P��A���a�Y�I�ƩM�p��17�I�h�����[�P\n");
    printf("�b���a�M���a�[�P������i��ӭt����\n");
    printf("�Ƶ�:���C���H2�Ƽ��J�P�i��C���C\n\n");
    printf("�iMenu�j\n");
    printf("�п�J:[1]�}�l�C�� [2]�d�ݱƦ� [3]�����C��......");
    scanf("%d",&choose1);
}	//�D�e��

readyInterface(){
    readData();
    bubbleSort();
    system("cls");
    printf("No.     name     score   win_rate\n");
    for(i=0;i<=4;i++){
        printf("%2d%10s%10d%10.2f%%\n",i+1,playerData[i].name,playerData[i].score,playerData[i].play==0? 0:((float)playerData[i].win/(float)playerData[i].play)*100);
    }
    printf("�п�ܤH��(��J�s��):");
    do{
        scanf("%d",&choose4);
    }while(choose4<1||choose4>5);
    printf("�п�ܰʧ@:[1]�}�l�C�� [2]���m���......");
    scanf("%d",&choose3);
    if(choose3==2){
        strcpy(playerData[choose4-1].name,"none");
        playerData[choose4-1].score=10000;
        playerData[choose4-1].win=0;
        playerData[choose4-1].play=0;
        playerData[choose4-1].blackjack=0;
    }
    if(strcmp(playerData[choose4-1].name,"none")==0){
        printf("�ЩR�W:");
        scanf("%s",playerData[choose4-1].name);
    }
}	//�C���ǳƵe��

playInterface(int a,int b){
    system("cls");
    printf("���a:%s  �{���w�X:%d  �����U�`���B:%d\t�P��|�l %d �i�P\n\n",playerData[choose4-1].name,playerData[choose4-1].score,1000*choose5,cardNumber);
    printf("���a���P:\n");
    printCard(computerCard,a);
    printf("�A����P:\n");
    printCard(playerCard,b);
    printf("�п��:[1]�n�P [2]pass ......");
}	//�C���ɪ��e�� (��ܲ��aa�i�P�A���ab�i�P)

betInterface(){
    system("cls");
    printf("���a: %s �{���w�X: %d �P��|�l %d �i�P\n\n",playerData[choose4-1].name,playerData[choose4-1].score,cardNumber);
    printf("���a���P:\n\n\n\n\n\n\n\n�A����P:\n\n\n\n\n\n\n\n");
    do{
        printf("�п�ܤU�`���B:[1] 1000 [3] 3000 [9]���}......");
        scanf("%d",&choose5);
    }while(choose5!=1&&choose5!=3&&choose5!=9);
    if(choose5==1||choose5==3)
    playerData[choose4-1].score-=1000*choose5;
}

resultInterface(){
    if(choose5!=9){
        system("cls");
        playerData[choose4-1].play+=1;
        printf("�����C�����G: ");
        switch(status){
            case 1:
                printf("���a���!! �{���w�X: %d\n",playerData[choose4-1].score);
                break;
            case 2:
                printf("���a���P���³ǧJ!! ���a���!! �{���w�X: %d\n",playerData[choose4-1].score);
                break;
            case 3:
                printf("�����I�ƬۦP!! +%d �{���w�X: %d\n",1000*choose5,playerData[choose4-1].score);
                playerData[choose4-1].score+=1000*choose5;
                break;
            case 4:
                playerData[choose4-1].blackjack+=1;
                printf("����P���³ǧJ!! +%d �{���w�X: %d\n",1000*choose5,playerData[choose4-1].score);
                playerData[choose4-1].score+=1000*choose5;
                break;
            case 5:
                playerData[choose4-1].score+=1000*choose5*2;
                playerData[choose4-1].win+=1;
                printf("�A���!! +%d �{���w�X: %d\n",1000*choose5*2,playerData[choose4-1].score);
                break;
            case 6:
                playerData[choose4-1].score+=1000*choose5*3;
                playerData[choose4-1].win+=1;
                playerData[choose4-1].blackjack+=1;
                printf("�A���!! +%d �{���w�X: %d\n",1000*choose5*3,playerData[choose4-1].score);
                break;
        }
        printf("\n���a���P:\n");
        printCard(computerCard,computerCardNumber);
        printf("�A����P:\n");
        printCard(playerCard,playerCardNumber);
        system("pause");
    }
    if(cardNumber<=52){
        cardNumber==104;
        system("cls");
        printf("�P��Ѿl�i�Ƥ����@�b!! ���s�~�P!!\n");
        printf("�ثe�C���έp:\n");
        printf("���a:%s\n",&playerData[choose4-1].name);
        printf("�{���w�X:%d\n",playerData[choose4-1].score);
        printf("�ӳ���:%d\n",playerData[choose4-1].win);
        printf("�C������:%d\n",playerData[choose4-1].play);
        printf("\n");
        system("pause");
    }
    if(choose5==9){
        writeData();
        system("cls");
        printf("�C���ƾڲέp:\n");
        printf("���a:%s\n",&playerData[choose4-1].name);
        printf("�{���w�X:%d\n",playerData[choose4-1].score);
        printf("�ӳ���:%d\n",playerData[choose4-1].win);
        printf("�C������:%d\n",playerData[choose4-1].play);
        printf("\n");
        system("pause");
    }
}	//����e��

recordInterface(){
    readData();
    bubbleSort();
    system("cls");
    printf("�{�ɱƦ�:\nNo.     name    score   win   play   win_rate   blackjack\n");
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
        printf("�ӭt�w��!! 3���۰ʤ���...");
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
                    printf("�A�w�z�P! 3���N�۰ʤ���...");
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
                printf("�п��:[1]�n�P [2]pass ......");
                break;
        }
    }
}	//�C���禡
