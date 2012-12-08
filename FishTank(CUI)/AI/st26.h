#include "fish.h"

class st26:public fish{

    private:
        int round,point,id,lv,exp,x,y,hp,maxHp,att,sp,status;
        int pic[N+1][M+1];
        int playerPos[2][MAX_PLAYER+1][2];
        int thinglist[3][N*M+1],thing;
        int DEAD,ALIVE;
        int INIT_HP,INIT_SP,LEAST_HP,MOST_HP;
        int Food_Value,Kill_Value;
        int MAX_INT;
        int guessSp[MAX_PLAYER+1];   //注意revive后不能猜，真实速度>=guessSp[did],他还可以加点;
        int getHPs[MAX_PLAYER+1],getHpMaxs[MAX_PLAYER+1];
        int myPoint;
        int a1[5],a2[5];

        void refreshGraph();
        void refreshData();
        void assignLeftPoint();
        void guessSpeed(int t);

        int evalueDanger(int dx,int dy);  //评估时需注意吃完水藻后风险降低
        int evalueProfit(int dx,int dy);
        int evalueDangerRevive(int dx,int dy);  //评估时需注意吃完水藻后风险降低
        int evalueProfitFrom(int p);
        int evalueAttack(int p);

        int dis(int x1,int y1,int x2,int y2);
        int max1(int Inta,int Intb);
        int min1(int Inta,int Intb);
        int abs1(int Inta);

    public:
        void init();
        void play();
        void revive(int &toX,int &toY);
};

int st26::max1(int Inta,int Intb)
{
    if(Inta>Intb) return Inta; else return Intb;
}

int st26::min1(int Inta,int Intb)
{
    if(Inta<Intb) return Inta; else return Intb;
}

int st26::abs1(int Inta)
{
    if(Inta>0) return Inta; else return -Inta;
}

int st26::dis(int x1,int y1,int x2,int y2)
{
     return abs1(x1-x2)+abs1(y1-y2);
}

void st26::refreshGraph()
{
    for(int i=1;i<=N;i++)
        for(int j=1;j<=M;j++)
            pic[i][j]=askWhat(i,j);
    return ;
}

void st26::refreshData()
{
    point=getPoint();
    id=getID();
    lv=getLevel();
    exp=getExp();
    x=getX();
    y=getY();
    hp=getHP();
    maxHp=getMaxHP();
    att=getAtt();
    sp=getSp();
    for(int i=1;i<=MAX_PLAYER;i++) {getHPs[i]=askHP(i);getHpMaxs[i]=max1(getHpMaxs[i],getHPs[i]);}
}

void st26::init()
{
    //const int initialized
    a1[1]=1;a1[2]=0;a1[3]=-1;a1[4]=0;
    a2[1]=0;a2[2]=1;a2[3]=0;a2[4]=-1;
    myPoint=0;
    DEAD=-1,ALIVE=1;
    INIT_HP=4,INIT_SP=5,LEAST_HP=30,MOST_HP=200;
    Food_Value=10,Kill_Value=10;
    MAX_INT=1000000000;
    status=0;
    for(int i=1;i<=MAX_PLAYER;i++) {guessSp[i]=-1;getHPs[i]=getHpMaxs[i]=0;}
    for(int i=1;i<=MAX_PLAYER;i++) playerPos[0][i][0]=playerPos[1][i][0]=playerPos[1][i][1]=playerPos[0][i][1]=0;

    round=0;
    refreshData();
    refreshGraph();
    for(int i=1;i<=MAX_PLAYER;i++) {playerPos[0][i][1]=-1;playerPos[0][i][0]=-1;}
    for(int i=1;i<=N;i++)
        for(int j=1;j<=M;j++)
            if(pic[i][j]>0) {playerPos[0][pic[i][j]][0]=i;playerPos[0][pic[i][j]][1]=j;}

    //initialize the points
    for(int i=1;i<=INIT_HP;i++) increaseHealth();
    for(int i=1;i<=INIT_SP;i++) increaseSpeed();
    for(int i=1;i<=10;i++) increaseStrength();
}

void st26::guessSpeed(int t)
{
    for(int i=1;i<=MAX_PLAYER;i++)
    {
        //printf("%d,%d->%d,%d\n",playerPos[t][i][0],playerPos[t][i][1],playerPos[(t+1)%2][i][0],playerPos[(t+1)%2][i][1]);
        if(playerPos[t][i][0]>0&&playerPos[(t+1)%2][i][0]>0)
            guessSp[i]=max1(dis(playerPos[t][i][0],playerPos[t][i][1],playerPos[(t+1)%2][i][0],playerPos[(t+1)%2][i][1]),guessSp[i]);
    }
}

int st26::evalueProfitFrom(int p)
{
    int pmhp=getHpMaxs[p];
    int psp=guessSp[p];
    int plv=((pmhp/2+psp)*1.5-10)/3+1;
    if(plv<getLevel()) return 10;
    else if(plv>=4) return plv;
    else return 0;
}

int st26::evalueAttack(int p)
{
    int pmhp=getHpMaxs[p];
    int psp=guessSp[p];
    if(psp<=40)return int((pmhp/2+psp)*0.5);
    else if(psp<0) return pmhp/2;
    else return 0;
}

int st26::evalueDangerRevive(int dx,int dy)  //评估时需注意吃完水藻后风险降低
{
    int sum=0;
    for(int thingl=1;thingl<=thing;++thingl)
        {
            int i=thinglist[0][thingl],j=thinglist[1][thingl];
            int t=pic[i][j];
            if(t>0)
            {
                int thp=getHP();
                int tatt=evalueAttack(t);
                if(guessSp[t]<0)
                {
                    if(dis(i,j,dx,dy)<3&&tatt>getHP()) sum += 10;
                    else if(dis(i,j,dx,dy)<8&&tatt>getHP()) sum+=5;
                }
                else if(guessSp[t]>dis(i,j,dx,dy))
                {
                    //cout<<tatt<<","<<getHP()<<"!!!"<<endl;
                    if(tatt-3>getHP()) sum+=1000;
                    else if(tatt*2>getHP()) sum+=10;
                    else if(tatt*3>getHP()) sum+=5;
                    else sum+=1;
                }
            }
        }
    return sum;
}

int st26::evalueDanger(int dx,int dy)  //评估时需注意吃完水藻后风险降低
{
    int sum=0;
    for(int thingl=1;thingl<=thing;thingl++)
        {
            int i=thinglist[0][thingl],j=thinglist[1][thingl];
            int t=askWhat(i,j);
            if(t>0)
            {
                int thp=getHP();
                int tatt=evalueAttack(t);
                if(guessSp[t]<0)
                {
                    if(dis(i,j,dx,dy)<3&&tatt>getHP()) sum += 10;
                    else if(dis(i,j,dx,dy)<8&&tatt>getHP()) sum+=5;
                }
                else if(guessSp[t]>dis(i,j,dx,dy))
                {
                    //cout<<tatt<<","<<getHP()<<"!!!"<<endl;
                    if(tatt>getHP()) sum+=1000;
                    else if(tatt*2>getHP()) sum+=10;
                    else if(tatt*3>getHP()) sum+=5;
                    else sum+=1;
                }
            }
        }
    return sum;
}

int st26::evalueProfit(int dx,int dy)
{
    int hplimit=1,xuekanren=1;
    if(hp*100/maxHp<50) {xuekanren=0;hplimit=100;}
    //if(hplimit==100) cout<<"s";
    int sum=0;
    for(int thingl=1;thingl<=thing;thingl++)
        {
            int i=thinglist[0][thingl],j=thinglist[1][thingl];
            if(i==dx&&j==dy) return 0;
            if(dis(i,j,dx,dy)==1)
            {
                if(askWhat(i,j)==FOOD) sum+=Food_Value*500000*hplimit;
                else if(askWhat(i,j)>0&&askHP(askWhat(i,j))<=getAtt()+getPoint()&&round>10)
                {
                    //sum+=max1(evalueProfitFrom(askWhat(i,j)),0)*Kill_Value*1000000;
                    //if(evalueProfitFrom(askWhat(i,j))==0) sum+=Kill_Value*200000;
                    int tmp=1,c=evalueProfitFrom(askWhat(i,j));
                    if(c+3<getLevel()||round>=200) tmp=9;
                    else if(c>=5||round>=200) tmp=min1(c/5,9);
                    else if(c>=5) tmp=2;
                    sum+=tmp*Kill_Value*400000;
                }
                else if(askWhat(i,j)>0&&evalueAttack(askWhat(i,j))*2<getHP()&&round>10)
                    sum+=Kill_Value*1000*xuekanren;
            }
            else if(dis(i,j,dx,dy)<=getSp())
            {
                if(askWhat(i,j)==FOOD) sum+=Food_Value*50000*hplimit;
                else if(askWhat(i,j)>0&&askHP(askWhat(i,j))<=getAtt()+getPoint()&&round>10)
                {
                    //sum+=max1(evalueProfitFrom(askWhat(i,j)),0)*Kill_Value*20000;
                    //if(evalueProfitFrom(askWhat(i,j))==0) sum+=Kill_Value*11000;
                    int tmp=1,c=evalueProfitFrom(askWhat(i,j));
                    if(c+3<getLevel()||round>=200) tmp=9;
                    else if(c>=5||round>=200) tmp=min1(c/5,9);
                    else if(c>=5) tmp=2;
                    sum+=tmp*Kill_Value*40000;
                }
                else if(askWhat(i,j)>0&&evalueAttack(askWhat(i,j))*2<getHP()&&round>10)
                    sum+=Kill_Value*100*xuekanren;
            }
            else if(dis(i,j,dx,dy)<=2*getSp())
            {
                if(askWhat(i,j)==FOOD) sum+=Food_Value*5000*hplimit;
                else if(askWhat(i,j)>0&&askHP(askWhat(i,j))<=getAtt()+getPoint()&&round>10)
                {
                    //sum+=max1(evalueProfitFrom(askWhat(i,j)),0)*Kill_Value*2000;
                    //if(evalueProfitFrom(askWhat(i,j))==0) sum+=Kill_Value*1100;
                    int tmp=1,c=evalueProfitFrom(askWhat(i,j));
                    if(c+3<getLevel()||round>=200) tmp=9;
                    else if(c>=5||round>=200) tmp=min1(c/5,9);
                    else if(c>=5) tmp=2;
                    sum+=tmp*Kill_Value*4000;
                }
                else if(askWhat(i,j)>0&&evalueAttack(askWhat(i,j))*2<getHP()&&round>10)
                    sum+=Kill_Value*10*xuekanren;
            }
            else //if(dis(i,j,dx,dy)<=3*getSp())
            {
                if(askWhat(i,j)==FOOD) sum+=Food_Value*500*hplimit;
                else if(askWhat(i,j)>0&&askHP(askWhat(i,j))<=getAtt()+getPoint()&&round>10)
                {
                    //sum+=max1(evalueProfitFrom(askWhat(i,j)),0)*Kill_Value*200;
                    //if(evalueProfitFrom(askWhat(i,j))==0) sum+=Kill_Value*110;
                    int tmp=1,c=evalueProfitFrom(askWhat(i,j));
                    if(c+3<getLevel()||round>=200) tmp=9;
                    else if(c>=5||round>=200) tmp=min1(c/5,9);
                    else if(c>=5) tmp=2;
                    sum+=tmp*Kill_Value*400;
                }
                else if(askWhat(i,j)>0&&evalueAttack(askWhat(i,j))*2<getHP()&&round>10)
                    sum+=Kill_Value*1*xuekanren;
            }
           /* else
            {
                if(askWhat(i,j)==FOOD) sum+=Food_Value*50;
                else if(askWhat(i,j)>0&&askHP(askWhat(i,j))<=getAtt()+getPoint()&&round>10)
                {
                    //sum+=max1(evalueProfitFrom(askWhat(i,j)),0)*Kill_Value*200;
                    //sum+=max1(evalueProfitFrom(askWhat(i,j)),0)*Kill_Value*200;
                    //if(evalueProfitFrom(askWhat(i,j))==0) sum+=Kill_Value*110;
                    int tmp=1,c=evalueProfitFrom(askWhat(i,j));
                    if(c+3<getLevel()||round>=200) tmp=9;
                    else if(c>=5||round>=200) tmp=min1(c/5,9);
                    sum+=tmp*Kill_Value*40;
                }
                else if(askWhat(i,j)>0&&evalueAttack(askWhat(i,j))*2<getHP()&&round>10)
                    sum+=Kill_Value*1;
            }*/
        }
    return sum;
}

void st26::assignLeftPoint()
{
    int hpnum=0;
    for(int i=1;i<=MAX_PLAYER;i++) if(askHP(i)>hp) hpnum++;
    if(hp*100/maxHp<40||hpnum>=15) increaseHealth();
    int maxsp=0;
    for(int i=1;i<=MAX_PLAYER;i++) if(guessSp[i]>maxsp&&i!=id) maxsp=guessSp[i];

    //cout<<maxsp<<" "<<sp<<endl;
    if(lv<=6)
    {
        if(sp<80) increaseSpeed();
        if(sp<80) increaseSpeed();
    }
    else if(lv<=30&&maxsp>sp)
    {
        if(sp<80) increaseSpeed();
    }
    else if(lv>=40) increaseStrength();
    else increaseHealth();
    while(getPoint()>0)
    {
        int tmp=rand()%10;
        if(lv<=10)
        {
            if(tmp<=6) increaseHealth();
            else if(tmp<=9) increaseSpeed();
            else increaseStrength();
        }
        else
        {
            if(tmp<=3) increaseHealth();
            else if(tmp<=6) increaseSpeed();
            else increaseStrength();
        }
    }
   /*if(getPoint()>0)
   {
       //increase health
       if(round<100)
       {
            if(getHP()*10/getMaxHP()<6)
            {
                while(getPoint()>0 && getHP()*10/getMaxHP()<8 && getHP()<12) increaseHealth();
                myPoint=getPoint();
            }
       }
       else
       {
           if(getHP()*10/getMaxHP()<8)
            {
                while(getPoint()>0 && getHP()*10/getMaxHP()<8) increaseHealth();
                myPoint=getPoint();
            }
       }
    }
    if(getPoint()-myPoint>0)
    {
       //increase other point
       increaseHealth();
       if(round<50)
       {
           int time=0;
           while(getPoint()-myPoint>0 && time<2) {increaseSpeed();time++;}
       }
       else if(round<100)
       {
           increaseHealth();
       }
       else
       {
           increaseSpeed();
       }
   }
   if(round<=20||(round<=200&&round%40==1)||(round%100==1)) increaseSpeed();
   myPoint=getPoint();
   if(myPoint>5)
    {
        while(myPoint>5)
        {
            if(getSp()<80) {increaseSpeed();myPoint=getPoint();}
            increaseStrength();myPoint=getPoint();
        }
        myPoint=getPoint();
    }*/
}

void st26::play()
{
    //cout<<id<<"  PLAY,in"<<endl;
    round++;
    //cout<<round<<"-";
    refreshData();
    refreshGraph();
    for(int i=1;i<=MAX_PLAYER;i++)
    {
        playerPos[round%2][i][0]=-1;playerPos[round%2][i][1]=-1;
    }
    for(int i=1;i<=N;i++)
        for(int j=1;j<=M;j++)
            if(askWhat(i,j)>0) {playerPos[round%2][askWhat(i,j)][0]=i;playerPos[round%2][askWhat(i,j)][1]=j;}
    if(status) guessSpeed(round%2);

    //evaluate where to go
    int danger[N+1][M+1],profit[N+1][M+1];
    for(int i=1;i<=N;i++) for(int j=1;j<=M;j++) danger[i][j]=profit[i][j]=0;
    int listd[N*M+1]={0},listp[N*M+1]={0},listx[N*M+1]={0},listy[N*M+1]={0};
    int t=0;
    //cout<<hp<<"/"<<maxHp<<endl;
    thing=0;
    for(int i=1;i<=N;i++)
        for(int j=1;j<=M;j++)
        {
            int tmp=askWhat(i,j);
            if(tmp!=EMPTY&&tmp!=id)
            {
                ++thing;
                thinglist[0][thing]=i;
                thinglist[1][thing]=j;
                thinglist[2][thing]=tmp;
            }
        }
    for(int i=1;i<=N;i++)
        for(int j=1;j<=M;j++)
        {
            t++;
            danger[i][j]=evalueDanger(i,j);
            profit[i][j]=evalueProfit(i,j);
            listp[t]=profit[i][j];
            listd[t]=danger[i][j];
            listx[t]=i;listy[t]=j;
        }
    /*cout<<"Profit:";
    for(int i=1;i<=N;i++)
        {for(int j=1;j<=M;j++)
            printf("%d ",profit[i][j]);
        cout<<endl;
        }
    cout<<"Danger:";
    for(int i=1;i<=N;i++)
        {for(int j=1;j<=M;j++)
            printf("%4d",danger[i][j]);
        cout<<endl;
        }
    cout<<"guessSpeed:";
    for(int i=1;i<=1;i++) cout<<guessSp[i]<<" ";
    cout<<endl;
    cout<<"guessAttack:";
    for(int i=1;i<=1;i++) cout<<evalueAttack(i)<<" ";
    cout<<endl;
    cout<<"getHP:";
    for(int i=1;i<=1;i++) cout<<getHPs[i]<<" ";
    cout<<endl;
    cout<<"getMAXHP:";
    for(int i=1;i<=1;i++) cout<<getHpMaxs[i]<<" ";
    cout<<endl;*/
    for(int i=1;i<t;i++)
        for(int j=i+1;j<=t;j++)
            if((listp[i]<listp[j])||(listp[i]==listp[j]&&listd[i]>listd[j]))
            {
                int r=listp[i];listp[i]=listp[j];listp[j]=r;
                r=listd[i];listd[i]=listd[j];listd[j]=r;
                r=listx[i];listx[i]=listx[j];listx[j]=r;
                r=listy[i];listy[i]=listy[j];listy[j]=r;
            }

    bool f=false;
    int toX=0,toY=0,maxpro=0,maxi=0,maxj=0;int flag=0;
    for(int i=1;i<=N;i++)
        for(int j=1;j<=M;j++)
            if(askWhat(i,j)>0&&askWhat(i,j)!=id&&askHP(askWhat(i,j))<=getAtt()&&dis(x,y,i,j)<=getSp())
            {
                if(evalueProfitFrom(askWhat(i,j))>maxpro&&evalueDanger(i,j)<1000)
                {
                    maxpro=evalueProfitFrom(askWhat(i,j));
                    maxi=i;maxj=j;
                }
            }
    if(maxpro)
    {
        flag=move(maxi-1,maxj)+move(maxi+1,maxj)+move(maxi,maxj-1)+move(maxi,maxj+1);
        if(flag) attack(maxi,maxj);
    }
    for(int i=1;i<=t&&(!f);i++)
    {
        if(listd[i]<500&&dis(getX(),getY(),listx[i],listy[i])<=getSp())
            f=move(listx[i],listy[i]);
        if(f) {toX=listx[i];toY=listy[i];}
    }
    if(!f)
    {
        for(int i=1;i<t;i++)
            for(int j=i+1;j<=t;j++)
                if((listd[i]>listd[j])||(listd[i]==listd[j]&&listp[i]<listp[j]))
                {
                    int r=listp[i];listp[i]=listp[j];listp[j]=r;
                    r=listd[i];listd[i]=listd[j];listd[j]=r;
                    r=listx[i];listx[i]=listx[j];listx[j]=r;
                    r=listy[i];listy[i]=listy[j];listy[j]=r;
                }
        int toX=0,toY=0;
        for(int i=1;i<=t&&(!f);i++)
        {
            if(dis(getX(),getY(),listx[i],listy[i])<=getSp()) f=move(listx[i],listy[i]);
            if(f) {toX=listx[i];toY=listy[i];}
        }
    }
    //cout<<getX()<<","<<getY()<<endl;
    if(f)
    {
        int tx,ty;int food[5]={0},kill[5]={0};
        for(int i=1;i<=4;i++)
        {
            tx=getX()+a1[i];ty=getY()+a2[i];
            if((tx>=1&&tx<=N)&&(ty>=1&&ty<=M))
            {
                int tmp=askWhat(tx,ty);
                if(tmp>0)
                {
                    kill[i]=1;
                }
                else if(tmp==FOOD)
                {
                    food[i]=1;
                }
            }
        }

        //attack player
        int maxKill=0;
        for(int i=1;i<=4;i++)
            if(kill[i])
            {
                tx=getX()+a1[i];ty=getY()+a2[i];
                if(tx>=1&&tx<=N&&ty>=1&&ty<=M)
                {
                    int tmp=askWhat(tx,ty);
                    if(askHP(tmp)<=getAtt()+getPoint())
                    {
                        int total=0;
                        while(askHP(tmp)>getAtt()) {increaseStrength();++total;}
                        myPoint-=total;
                        attack(tx,ty);
                    }
                    else maxKill=i;
                }
            }

        //attack food
        for(int i=1;i<=4;i++)
            if(food[i])
            {
                tx=getX()+a1[i];ty=getY()+a2[i];
                if(tx>=1&&tx<=N&&ty>=1&&ty<=M){
                    attack(tx,ty);
                }
            }

        if(maxKill)
        {
                int tx=getX()+a1[maxKill],ty=getY()+a2[maxKill];
                if(tx>=1&&tx<=N&&ty>=1&&ty<=M)
                {
                    int tmp=askWhat(tx,ty);
                    for(int i=1;i<=(myPoint+1)/2;i++) increaseStrength();
                    myPoint-=(myPoint+1)/2;
                    attack(getX()+a1[maxKill],getY()+a2[maxKill]);
                }
        }
    }

    //assign leftPoint
    assignLeftPoint();

    refreshData();
    refreshGraph();

    status=1;

    //for(int i=1;i<=TOTAL_PLAYER;i++) cout<<guessSp[i]<<" ";cout<<endl;
    //cout<<getX()<<","<<getY()<<endl;
    //cout<<id<<"   PLAY,out"<<endl;
}

void st26::revive(int &rx,int &ry)
{
    //cout<<id<<"   REVIVE,in"<<endl;
    round++;

    //add hp
    while(getPoint()>0&&getHP()*10/getMaxHP()<=7) increaseHealth();
    myPoint=getPoint();

    int max,maxi,maxj;
    int foodden[N+1][M+1]={0},foodlx[N*M+1]={0},foodly[N*M+1]={0},foodlistnum=0;
    for(int i=1;i<=N;i++)
        for(int j=1;j<=M;j++)
            if(pic[i][j]==FOOD)
            {
                foodlistnum++;
                foodlx[foodlistnum]=i;
                foodly[foodlistnum]=j;
            }
    int maxfood=0;
    for(int i=1;i<=N;i++)
        for(int j=1;j<=M;j++)
        {
            for(int k=1;k<=foodlistnum;k++)
            {
                if(dis(foodlx[k],foodly[k],i,j)<=sp) foodden[i][j]++;
            }
            if(foodden[i][j]>maxfood&&pic[i][j]==EMPTY){maxfood=foodden[i][j];rx=i;ry=j;}
        }
    //cout<<maxfood<<endl;
    //cout<<id<<"   REVIVE,out"<<endl;
}

//可自行增加所需函数所需函数及变量，但需保证上面每个函数的完整与可调用

//不要使用全局变量！！
