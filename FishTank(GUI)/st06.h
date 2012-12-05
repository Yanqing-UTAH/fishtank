
#pragma once
//#define DEBUG
#include "fish.h"

#include <cstdlib>
#include <cmath>
#include <algorithm>
#include <cstring>
#include <ctime>

#define for1(i,a,b) for(i=a;i<=b;i++)
#define for2(i,a,b) for(i=a;i>=b;i--)

#ifdef DEBUG
#include <fstream>
#include <iomanip>
#include <iostream>
using namespace std;
#endif


class st06:public fish{
private:
	static int X[4],Y[4];
	double RATE;
	int round;

	bool legal(int x,int y);
	bool check(int x,int y);
	void refresh();
	void addPoint();
	void paint(int x,int y,int r,double v);
	void access();
	void moving();
	void attacking();

public:
	struct data{
		int HP,MaxHP,Speed,Att,Point,Level,Exp,X,Y;
	}my;
	data PA[MAX_PLAYER+3];
	int ID;
	int map[N+3][M+3];
	double value[N+3][M+3];
	int q[N*M+3][2];
	bool f[N+3][M+3];

    void init();
    void play();
    void revive(int &x,int &y);
};

int st06::X[4]={0,0,1,-1};
int st06::Y[4]={1,-1,0,0};

bool st06::legal(int x,int y){
	return (x>0 && y>0 && x<=N && y<=M);
}

bool st06::check(int x,int y){
	int i;
	if (map[x][y]!=EMPTY)return false;
	for1(i,0,3){
		if (legal(x+X[i],y+Y[i]) && map[x+X[i]][y+Y[i]]!=EMPTY && map[x+X[i]][y+Y[i]]!=ID)return true;
	}
	return false;
}

void st06::refresh(){
	int i,j;
	for1(i,1,N){
		for1(j,1,M){
			map[i][j]=fish::askWhat(i,j);
		}
	}
	my.HP=getHP();
	my.MaxHP=getMaxHP();
	my.Speed=getSp();
	my.Att=getAtt();
	my.Point=getPoint();
	my.Level=getLevel();
	my.Exp=getExp();
	my.X=getX();
	my.Y=getY();
}

void st06::addPoint(){
	while (fish::getPoint()){
		int k=rand()%3;
		switch (k){
			case 0:{
				fish::increaseHealth();
				break;
			}
			case 1:{
				fish::increaseSpeed();
				break;
			}
			case 2:{
				fish::increaseStrength();
				break;
			}
		}
	}
}

void st06::paint(int x,int y,int R,double v){
	int i,h=0,r=1;
	int Tot[N*M+3];
	double tmp[N+3][M+3];
	memset(f,0,sizeof(f));
	memset(tmp,0,sizeof(tmp));
	f[x][y]=true;
	tmp[x][y]=v;
	value[x][y]+=v;
	q[h][0]=x;
	q[h][1]=y;
	Tot[h]=R;
	while (h<r){
		for1(i,0,3){
			if (Tot[h]==0)continue;
			x=q[h][0]+X[i];
			y=q[h][1]+Y[i];
			if (legal(x,y) && !f[x][y]){
				f[x][y]=true;
				tmp[x][y]=tmp[q[h][0]][q[h][1]]*RATE;
				value[x][y]+=tmp[x][y];
				q[r][0]=x;
				q[r][1]=y;
				Tot[r]=Tot[h]-1;
				r++;
			}
		}
		h++;
	}
}

void st06::access(){
	memset(value,0,sizeof(value));
	int i,j;
	for1(i,1,N){
		for1(j,1,M){
			if (map[i][j]==FOOD){
				paint(i,j,my.Speed,-double(my.MaxHP)/(my.HP+1));
			}
			if (map[i][j]!=EMPTY && map[i][j]!=FOOD && map[i][j]!=ID){
				paint(i,j,my.Speed,askHP(map[i][j])-my.Att-1);
			}
		}
	}
}

void st06::moving(){
	int i,h=0,r=1,x=my.X,y=my.Y,p=0;
	int Tot[N*M+3];
	memset(f,0,sizeof(f));
	f[x][y]=true;
	q[h][0]=x;
	q[h][1]=y;
	Tot[h]=my.Speed;
	while (h<r){
		for1(i,0,3){
			if (Tot[h]==0)continue;
			x=q[h][0]+X[i];
			y=q[h][1]+Y[i];
			if (legal(x,y) && !f[x][y]){
				f[x][y]=true;
				q[r][0]=x;
				q[r][1]=y;
				Tot[r]=Tot[h]-1;
				r++;
			}
		}
		h++;
	}
	for1(i,0,r-1){
		x=q[i][0];
		y=q[i][1];
		if (check(x,y) && (p==0 || value[x][y]<value[q[p][0]][q[p][1]]))p=i;
	}
	if (p==0){
		for1(i,0,r-1){
			x=q[i][0];
			y=q[i][1];
			if (p==0 || value[x][y]<value[q[p][0]][q[p][1]])p=i;
		}
	}
	if (p==0)p=rand()%r;
	fish::move(q[p][0],q[p][1]);
}

void st06::attacking(){
	int i,x,y;
	for1(i,0,3){
		x=my.X+X[i];
		y=my.Y+Y[i];
		if (x>0 && y>0 && x<=N && y<=M && map[x][y]!=EMPTY){
			fish::attack(x,y);
		}
	}
}

void st06::init(){
	ID=fish::getID();
	srand(time(NULL));
	RATE=0.8;
	round=0;

	fish::increaseHealth();
	fish::increaseSpeed();
	fish::increaseStrength();

	addPoint();
}

void st06::play(){
	round++;
	addPoint();
	refresh();
	access();
	moving();
	refresh();
	attacking();
	addPoint();
}

void st06::revive(int &x,int &y){
	round++;
	x=rand()%N+1;
	y=rand()%M+1;
}

