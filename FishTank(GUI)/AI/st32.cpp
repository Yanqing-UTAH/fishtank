#include "fish.h"
#include "st32.h"
#include "cstring"
#include <cstdlib>

struct Vsort{int x,y,tx,ty,hp;};

static int Distance(int x1,int y1,int x2,int y2)
{
    return(abs(x1-x2)+abs(y1-y2));
}
static int max(const int a,int b)
{
    if (a>b) return a;
    else return b;
}
static int min(const int a,int b)
{
    if (a>b) return b;
    else return a;
}
static void add(Vsort s[],int &k,int x,int y,int xx,int yy,int HP)
{
    ++k;
    s[k].x=x;
    s[k].y=y;
    s[k].tx=xx;
    s[k].ty=yy;
    s[k].hp=HP;
}
static void sort(Vsort s[],int &k,int ES[][M+1])
{
    Vsort tmp;
    for (int i=1;i<=k;i++)
     for (int j=i+1;j<=k;j++)
       if (ES[s[i].tx][s[i].ty]>ES[s[j].tx][s[j].ty])
         {
             tmp=s[i];
             s[i]=s[j];
             s[j]=tmp;
         }
}
void st32::init()
{
   for (int i=0;i<4;i++)
     increaseHealth();
   for (int i=0;i<2;i++)
     increaseStrength();
   for (int i=0;i<5;i++)
     increaseSpeed();
}//初始加点

void st32::play()
{
    int sum,Point,My_Level,My_MaxHP,My_ID,My_x,My_y,My_HP,My_SP,My_att,HP,dest_x,dest_y,safety;
    int Permit[N+1][M+1],Thing[N+1][M+1],enemy_sum[N+1][M+1];
    const int xx[4]={0,0,1,-1};
    const int yy[4]={-1,1,0,0};
    const int R=10;
    Vsort enemy[1000],food[1000];
    int enemyk=0,foodk=0,temp;

  memset(Permit,0,sizeof(Permit));
  memset(Permit,0,sizeof(Thing));
  memset(enemy_sum,0,sizeof(enemy_sum));
  memset(enemy,0,sizeof(enemy));
  memset(food,0,sizeof(food));
  Point=getPoint();
  My_Level=getLevel();
  My_ID=getID();
  My_x=getX();
  My_y=getY();
  My_HP=getHP();
  My_MaxHP=getMaxHP();
  My_SP=getSp();
  My_att=getAtt();

  for (int i=1;i<=N;i++)
   for (int j=1;j<=M;j++)
    {
        Permit[i][j]=Distance(i,j,My_x,My_y);
        Thing[i][j]=askWhat(i,j);
    }
  for (int i=1;i<=N;i++)
   for (int j=1;j<=M;j++)
    {
        sum=0;
        if (Permit[i][j]<=My_SP+Point)
          for (int ii=max(1,i-R);ii<=min(N,i+R);ii++)
           for (int jj=max(1,j-R);jj<=min(M,j+R);jj++)
            {
                if (Thing[ii][jj]==My_ID) continue;
                if (Thing[ii][jj]>0) sum++;
            }
       enemy_sum[i][j]=sum;
    }//危险系数扫描
  for (int i=1;i<=N;i++)
   for (int j=1;j<=M;j++)
    {
        if (My_ID==Thing[i][j]) continue;
        if ((Permit[i][j]<=My_SP+Point+1)&&(Thing[i][j]!=EMPTY))
         {
            for (int k=0;k<4;k++)
             {
                 dest_x=i+xx[k];
                 dest_y=j+yy[k];
                 if ((dest_x<1)||(dest_y<1)||(dest_x>N)||(dest_y>M)) continue;
                 if ((Thing[dest_x][dest_y]==EMPTY)||(Thing[dest_x][dest_y]==My_ID))
                    {
                        if (Thing[i][j]>0)
                         {
                             HP=askHP(Thing[i][j]);
                              if (Permit[dest_x][dest_y]<=My_SP)
                                if (HP<=My_att+Point)
                                   {
                                     add(enemy,enemyk,i,j,dest_x,dest_y,HP);
                                     break;
                                   }
                              if (Point+My_SP>=Permit[dest_x][dest_y])
                                   {
                                       temp=Point-(Permit[dest_x][dest_y]-My_SP);
                                       if (HP<=My_att+temp)
                                       {
                                           add(enemy,enemyk,i,j,dest_x,dest_y,HP);
                                           break;
                                       }
                                   }
                         }
                        else
                         if (Point+My_SP>=Permit[dest_x][dest_y])
                            add(food,foodk,i,j,dest_x,dest_y,0);
                     }
                }
           }
    }//预处理可达区域enemy,food;
  sort(enemy,enemyk,enemy_sum);
  sort(food,foodk,enemy_sum);

  if (enemyk>0)
   {
       temp=Permit[enemy[1].tx][enemy[1].ty]-My_SP;
       for (int i=1;i<=temp;i++)
         {
           increaseSpeed();
           Point--;
         }
       temp=enemy[1].hp-My_att;
       if (temp>0)
         for (int i=1;i<=temp;i++)
           {
               increaseStrength();
               Point--;
           }
       while (Point>0)
        {
            increaseHealth();
            Point--;
        }
      if ((enemy[1].tx!=My_x)||(enemy[1].ty!=My_y))
         move(enemy[1].tx,enemy[1].ty);
      attack(enemy[1].x,enemy[1].y);
      return;
   }//有敌人可杀
  if (foodk>0)
   {
      temp=Permit[food[1].tx][food[1].ty]-My_SP;
       for (int i=1;i<=temp;i++)
         {
           increaseSpeed();
           Point--;
         }
      while (Point>0)
        {
            increaseHealth();
            Point--;
        }
      if ((food[1].tx!=My_x)||(food[1].ty!=My_y))
            move(food[1].tx,food[1].ty);
      attack(food[1].x,food[1].y);
      return;
   }
   while (((My_HP*2<My_MaxHP)||(My_MaxHP<(1.5*My_Level)))&&(Point>0))
     {
         increaseHealth();
         Point--;
     }
   while (Point>0)
    {
        increaseStrength();
        Point--;
    }
   safety=100;
   for (int i=1;i<=N;i++)
    for (int j=1;j<=M;j++)
     if ((Permit[i][j]<=My_SP)&&(Thing[i][j]==EMPTY))
      {
          if (enemy_sum[i][j]<safety)
           {
               dest_x=i;
               dest_y=j;
               safety=enemy_sum[i][j];
           }
      }
  move(dest_x,dest_y);
}

void st32::revive(int&,int&)
{

}


