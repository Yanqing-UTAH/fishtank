#include "st20.h"
#include <cstdlib>

static inline int max(int a, int b){
    return (a>b)?a : b;
}

static inline int min(int a, int b){
    return (a<b)?a : b;
}

static inline int ManDis(int x1, int y1, int x2, int y2){
    return abs(x1-x2) + abs(y1-y2);
}

const int st20::dx[4] = {1,0,-1,0};
const int st20::dy[4] = {0,1,0,-1};

int st20::GridEvaluator::iPropertyCostForHPCoefficient = 0;
int st20::GridEvaluator::iPropertyCostForSPCoefficient = 0;
int st20::GridEvaluator::iPropertyCostForSTCoefficient = 0;
int st20::GridEvaluator::iETInRangeEnemyCoefficient = 0;
int st20::GridEvaluator::iETNotInRangeEnemyCoefficient = 0;
int st20::GridEvaluator::iLevelOneEnemyMultiplier = 0;
int st20::GridEvaluator::iLevelTwoEnemyMultiplier = 0;
int st20::GridEvaluator::iLevelThreeEnemyMultiplier = 0;
int st20::GridEvaluator::iFoodBenefitCoefficient = 0;
int st20::GridEvaluator::iPerHPAddedCoefficient = 0;
int st20::GridEvaluator::iOneHitKillBenefitCoefficient = 0;
int st20::GridEvaluator::iAttackBenefitDecreaseCoefficient = 0;
int st20::GridEvaluator::iMaxHPAdded = 0;
int st20::GridEvaluator::iMaxSTAdded = 0;

void st20::GridEvaluator::Evaluate(st20& mySelf){
    iEvaluationResult = iAddSpeedBeforeAction * iPropertyCostForSPCoefficient;
    int iPropertyPointLeft = mySelf.myRemainingPoint - iAddSpeedBeforeAction;
    iAddHPBeforeAction = iAddStrengthBeforeAction = 0;
    for (int i = 1; i<=N; ++i)
    for (int j = 1; j<=M; ++j)
    if ((mySelf.mapc[i][j]!=EMPTY) && (mySelf.mapc[i][j]!=FOOD) && (mySelf.mapc[i][j]!=mySelf.myId)){
        int base =((ManDis(iMoveX, iMoveY, i, j) <= mySelf.ETSpeed[mySelf.idTable[mySelf.mapc[i][j]]])
            ? iETInRangeEnemyCoefficient : iETNotInRangeEnemyCoefficient);
        int multiplier = mySelf.ETMaxHP[mySelf.mapc[i][j]] / mySelf.iAttackMaxHPRatio;
        multiplier = (multiplier) ? (mySelf.myHP / multiplier) : 3;
        multiplier = (multiplier <= 1) ? iLevelOneEnemyMultiplier : ((multiplier <=2) ? iLevelTwoEnemyMultiplier : iLevelThreeEnemyMultiplier);
        iEvaluationResult += base * multiplier;
    }
    {
        int maxV = -2000000000;
        iTargetX = -1;
        iTargetY = -1;
        for (int k=0; k<4; ++k){
            int tx = iMoveX + mySelf.dx[k];
            int ty = iMoveY + mySelf.dy[k];
            if (tx>=1 && tx<=N && ty>=1 && ty<=M){
                int target = mySelf.mapc[tx][ty];
                if (target!=EMPTY){
                    if (target == FOOD){
                        int l =0, r = min(iPropertyPointLeft, iMaxHPAdded);
                        int ihp = 0;
                        while (l<=r){
                            int mid = (l+r) >> 1;
                            if (2*mid + mySelf.myHP + max(2, (mySelf.myMaxHP+2*mid)/10) <= HPGoal){
                                l = mid+1;
                            } else {
                                ihp = mid;
                                r = mid-1;
                            }
                        }
                        int V = ihp * iPropertyCostForHPCoefficient + iFoodBenefitCoefficient
                                + iPerHPAddedCoefficient * ( 2* ihp + max(2 ,(mySelf.myMaxHP + 2*ihp) / 10));
                        if (V>maxV){
                            iAddHPBeforeAction = ihp;
                            iAddStrengthBeforeAction = 0;
                            iTargetX = tx;
                            iTargetY = ty;
                            maxV = V;
                        }
                        V = iFoodBenefitCoefficient + iPerHPAddedCoefficient * max(2, mySelf.myMaxHP / 10);
                        if (V>maxV){
                            iAddHPBeforeAction = 0;
                            iAddStrengthBeforeAction = 0;
                            iTargetX = tx;
                            iTargetY = ty;
                            maxV = V;
                        }
                    }
                    else if (target!=mySelf.myId){
                        int ist = max(0, mySelf.askHP(target) - mySelf.myST);
                        ist = (ist>min(iMaxSTAdded, iPropertyPointLeft)) ? 0 : ist;
                        int V = 0;
                        if (mySelf.askHP(target) > mySelf.myST + ist){
                            V = iOneHitKillBenefitCoefficient +
                                ((mySelf.askHP(target) - mySelf.myST) / mySelf.myST + 1) * iAttackBenefitDecreaseCoefficient ;
                        } else {
                            V = iOneHitKillBenefitCoefficient + ist * iPropertyCostForSTCoefficient;
                            int multiplier = (mySelf.ETMaxHP[target] / mySelf.iAttackMaxHPRatio);
                            multiplier = (multiplier) ? (mySelf.myHP / multiplier) : 3;
                            multiplier = (multiplier <= 1) ? iLevelOneEnemyMultiplier : ((multiplier <=2) ? iLevelTwoEnemyMultiplier : iLevelThreeEnemyMultiplier);
                            V -= iETInRangeEnemyCoefficient * multiplier;
                        }
                        if (V>maxV){
                            iAddHPBeforeAction = 0;
                            iAddStrengthBeforeAction = ist;
                            iTargetX = tx;
                            iTargetY = ty;
                            maxV = V;
                        }
                    }
                }
            }
        }
        if (iTargetX!=-1) iEvaluationResult += maxV;
        else {
            int l =0, r = min(iPropertyPointLeft, iMaxHPAdded);
            int ihp = 0;
            while (l<=r){
                int mid = (l+r) >> 1;
                if (2*mid + mySelf.myHP + max(2, (mySelf.myMaxHP+2*mid)/10) <= HPGoal){
                    l = mid+1;
                } else {
                    ihp = mid;
                    r = mid-1;
                }
            }
            iEvaluationResult += iPropertyCostForHPCoefficient * ihp + iPerHPAddedCoefficient * 2 * ihp;
            iAddHPBeforeAction = ihp;
            iAddStrengthBeforeAction = 0;
        }
    }
}

void st20::init(){
    //set property
    for (int i=0; i<4; ++i) increaseHealth();
    for (int i=0; i<3; ++i) increaseStrength();
    for (int i=0; i<3; ++i) increaseSpeed();
    myId = getID();
    //get map
    {
        for (int i = 1; i<=N; ++i)
        for (int j = 1; j<=M; ++j){
            mapc[i][j] = askWhat(i, j);
            if (mapc[i][j]!=FOOD && mapc[i][j]!=EMPTY){
                idTable[mapc[i][j]] = playerCount++;
                ETMaxHP.push_back(max(2,askHP(mapc[i][j])));
                ETSpeed.push_back(1);
                ifDead.push_back(false);
                PosX.push_back(i);
                PosY.push_back(j);
            }
        }
    }
    if (ETMaxHP.size()){
        averageETMaxHP = 0;
        averageETSpeed = 0;
        for (unsigned i = 0 ;i<ETMaxHP.size(); ++i){
            averageETMaxHP += ETMaxHP[i];
            averageETSpeed += ETSpeed[i];
        }
        averageETMaxHP /= ETMaxHP.size();
        averageETSpeed /= ETSpeed.size();
    } else {
        averageETMaxHP = 2;
        averageETSpeed = 1;
    }
}

void st20::play(){
    //Update info
    ++CurRound;
    if (DataValid){
        for (int i = 0; i<playerCount; ++i){
            ifDead[i] = true;
        }
        for (int i = 1; i<=N; ++i)
        for (int j = 1; j<=M; ++j){
            int now = askWhat(i, j);
            if (now!=FOOD && now!=EMPTY){
                if (idTable.find(now)==idTable.end()){
                    idTable[now] = playerCount++;
                    ETMaxHP.push_back(max(2, askHP(now)));
                    ETSpeed.push_back(1);
                    ifDead.push_back(false);
                    PosX.push_back(i);
                    PosY.push_back(j);
                } else{
                    int id = idTable[now];
                    ETMaxHP[id] = max(ETMaxHP[id], askHP(now));
                    ETSpeed[id] = max(ETSpeed[id], ManDis(PosX[id], PosY[id], i, j));
                    ifDead[id] = false;
                    PosX[id] = i;
                    PosY[id] = j;
                }
            }
            mapc[i][j] = now;
        }
    }
    else{
        for (int i = 0; i<playerCount; ++i){
            ifDead[i] = true;
        }
        for (int i = 1; i<=N; ++i)
        for (int j = 1; j<=M; ++j){
            int now = askWhat(i, j);
            if (now!=FOOD && now!=EMPTY){
                if (idTable.find(now)==idTable.end()){
                    idTable[now] = playerCount++;
                    ETMaxHP.push_back(max(2, askHP(now)));
                    ETSpeed.push_back(1);
                    ifDead.push_back(false);
                    PosX.push_back(i);
                    PosY.push_back(j);
                } else{
                    int id = idTable[now];
                    ETMaxHP[id] = max(ETMaxHP[id], askHP(id));
                    ifDead[id] = false;
                    PosX[id] = i;
                    PosY[id] = j;
                }
            }
            mapc[i][j] = now;
        }
        DataValid = true;
    }
    if (ETMaxHP.size()){
        averageETMaxHP = 0;
        averageETSpeed = 0;
        for (unsigned i = 0 ;i<ETMaxHP.size(); ++i){
            averageETMaxHP += ETMaxHP[i];
            averageETSpeed += ETSpeed[i];
        }
        averageETMaxHP /= ETMaxHP.size();
        averageETSpeed /= ETSpeed.size();
    } else {
        averageETMaxHP = 2;
        averageETSpeed = 1;
    }

    //Decide where to move and attack
    {
        mySP = getSp();
        myST = getAtt();
        myHP = getHP();
        myMaxHP = getMaxHP();
        myRemainingPoint = getPoint();
        int iMaxSPAdded = 0;
        int myLevel = getLevel();
        if ( (myHP <= iEmergencyAbsoluteValue && myHP <= myMaxHP * iEmergencyPercentageWhenHPSmallerThanAbsoluteValue / 100)
             || (myHP <= myMaxHP * iEmergencyPercentage / 100) ){
            GridEvaluator::SetCoefficients(iEmergencyPropertyCostForHPCoefficient,
                                          iEmergencyPropertyCostForSPCoefficient,
                                          iEmergencyPropertyCostForSTCoefficient,
                                          iEmergencyETInRangeEnemyCoefficient,
                                          iEmergencyETNotInRangeEnemyCoefficient,
                                          iEmergencyLevelOneEnemyMultiplier,
                                          iEmergencyLevelTwoEnemyMultiplier,
                                          iEmergencyLevelThreeEnemyMultiplier,
                                          iEmergencyFoodBenefitCoefficient,
                                          iEmergencyPerHPAddedCoefficient,
                                          iEmergencyOneHitKillBenefitCoefficient,
                                          iEmergencyAttackBenefitDecreaseCoefficient,
                                          iEmergencyMaxHPAdded,
                                          iEmergencyMaxSTAdded);
            iMaxSPAdded = iEmergencyMaxSPAdded;
        }   //emergency. priority is food.
        else if (myHP >= iRadicalAbsoluteValue && myHP<= iRadicalPercentageWhenHPBiggerThanAbsoluteValue) {
            GridEvaluator::SetCoefficients(iRadicalPropertyCostForHPCoefficient,
                                          iRadicalPropertyCostForSPCoefficient,
                                          iRadicalPropertyCostForSTCoefficient,
                                          iRadicalETInRangeEnemyCoefficient,
                                          iRadicalETNotInRangeEnemyCoefficient,
                                          iRadicalLevelOneEnemyMultiplier,
                                          iRadicalLevelTwoEnemyMultiplier,
                                          iRadicalLevelThreeEnemyMultiplier,
                                          iRadicalFoodBenefitCoefficient,
                                          iRadicalPerHPAddedCoefficient,
                                          iRadicalOneHitKillBenefitCoefficient,
                                          iRadicalAttackBenefitDecreaseCoefficient,
                                          iRadicalMaxHPAdded,
                                          iRadicalMaxSTAdded);
            iMaxSPAdded = iRadicalMaxSPAdded;
        }   //radical, priority is fish.
        else {
            GridEvaluator::SetCoefficients(iNormalPropertyCostForHPCoefficient,
                                          iNormalPropertyCostForSPCoefficient,
                                          iNormalPropertyCostForSTCoefficient,
                                          iNormalETInRangeEnemyCoefficient,
                                          iNormalETNotInRangeEnemyCoefficient,
                                          iNormalLevelOneEnemyMultiplier,
                                          iNormalLevelTwoEnemyMultiplier,
                                          iNormalLevelThreeEnemyMultiplier,
                                          iNormalFoodBenefitCoefficient,
                                          iNormalPerHPAddedCoefficient,
                                          iNormalOneHitKillBenefitCoefficient,
                                          iNormalAttackBenefitDecreaseCoefficient,
                                          iNormalMaxHPAdded,
                                          iNormalMaxSTAdded);
            iMaxSPAdded = iNormalMaxSPAdded;
        }   //normal, priority is one-hit-kill fish.

        int iInitialX = getX(), iInitialY = getY();
        GridEvaluator BestChoice;
        BestChoice.HPGoal = (myHP <= iEmergencyAbsoluteValue && myHP<= myMaxHP * iEmergencyPercentageWhenHPSmallerThanAbsoluteValue / 100)
                            ? (myMaxHP * iEmergencyPercentageWhenHPSmallerThanAbsoluteValue / 100) :
                            ( (myHP<=myMaxHP * iEmergencyPercentage / 100) ? (myMaxHP * iEmergencyAbsoluteValue / 100)
                             : (averageETMaxHP) );
        BestChoice.iMoveX = iInitialX;
        BestChoice.iMoveY = iInitialY;
        BestChoice.iAddSpeedBeforeAction = 0;
        BestChoice.Evaluate(*this);
        GridEvaluator NextChoice;
        NextChoice.HPGoal = BestChoice.HPGoal;
        NextChoice.iAddSpeedBeforeAction = 0;
        for (int dis = 1; dis<=mySP; ++dis)
        for (int x = iInitialX - dis; x<=iInitialX+dis; ++x)
        if (x>=1 && x<=N){
            int y = iInitialY + dis - abs(x-iInitialX);
            if (y>=1 && y<=M && mapc[x][y] == EMPTY){
                NextChoice.iMoveX = x;
                NextChoice.iMoveY = y;
                NextChoice.Evaluate(*this);
                if (NextChoice.iEvaluationResult > BestChoice.iEvaluationResult)
                    BestChoice = NextChoice;
            }
            if (abs(x-iInitialX)!=dis){
                y = iInitialY - dis + abs(x-iInitialX);
                if (y>=1 && y<=M && mapc[x][y] == EMPTY){
                    NextChoice.iMoveX = x;
                    NextChoice.iMoveY = y;
                    NextChoice.Evaluate(*this);
                    if (NextChoice.iEvaluationResult > BestChoice.iEvaluationResult)
                        BestChoice = NextChoice;
                }
            }
        }
        for (int spAdded = 1; spAdded<= min(iMaxSPAdded, myRemainingPoint); ++spAdded){
            NextChoice.iAddSpeedBeforeAction = spAdded;
            int curSP = mySP + spAdded;
            for (int x = iInitialX - curSP; x<=iInitialX + curSP; ++x)
            if (x>=1 && x<=N){
                int y = iInitialY + curSP - abs(x - iInitialX);
                if (y>=1 && y<=M && mapc[x][y] == EMPTY){
                    NextChoice.iMoveX = x;
                    NextChoice.iMoveY = y;
                    NextChoice.Evaluate(*this);
                    if (NextChoice.iEvaluationResult > BestChoice.iEvaluationResult)
                        BestChoice = NextChoice;
                }
                if (abs(x - iInitialX)!= curSP){
                    y = iInitialY - curSP + abs(x-iInitialX);
                    if (y>=1 && y<=M && mapc[x][y] == EMPTY){
                        NextChoice.iMoveX = x;
                        NextChoice.iMoveY = y;
                        NextChoice.Evaluate(*this);
                        if (NextChoice.iEvaluationResult > BestChoice.iEvaluationResult)
                            BestChoice = NextChoice;
                    }
                }
            }
        }

        for (int i = 0 ;i<BestChoice.iAddSpeedBeforeAction; ++i) increaseSpeed();
        for (int i = 0; i<BestChoice.iAddHPBeforeAction; ++i) increaseHealth();
        for (int i = 0; i<BestChoice.iAddStrengthBeforeAction; ++i) increaseStrength();
        if (BestChoice.iMoveX != iInitialX || BestChoice.iMoveY != iInitialY)
            move(BestChoice.iMoveX, BestChoice.iMoveY);
        if (BestChoice.iTargetX!=-1)
            attack(BestChoice.iTargetX, BestChoice.iTargetY);

        if (getLevel() - myLevel){
            for (int i = 0; i<getLevel() - myLevel; ++i){
                if (getSp() < min(N+M-1, averageETSpeed)) increaseSpeed(); else
                if (getAtt() < getMaxHP() / iAttackMaxHPRatio) increaseStrength(); else increaseHealth();
                if (getAtt() < getMaxHP() / iAttackMaxHPRatio) increaseStrength(); else increaseHealth();
            }
        }
        if (getPoint() > iPointsToSave){
            int left = getPoint() - iPointsToSave;
            //while (getSp() < min(N+M-2, averageETSpeed) && (--left)>=0) increaseSpeed();
            while ((--left)>=0)
                if (getAtt() < getMaxHP() / iAttackMaxHPRatio) increaseStrength(); else increaseHealth();
        }
    }
}

void st20::revive(int& iPosX, int& iPosY){
    ++CurRound;
    DataValid = false;
    iPosX = iPosY = -1;
}
