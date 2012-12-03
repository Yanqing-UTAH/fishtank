#ifndef _ST20_H_
#define _ST20_H_

#include "fish.h"
#include <map>
#include <vector>

class st20:virtual public fish{

public:
    void init();

    void play();

    void revive(int&,int&);

    st20():idTable(), ETMaxHP(), ETSpeed(), ifDead(), PosX(), PosY(),
           CurRound(-1), DataValid(false), playerCount(0),
           iEmergencyPercentage(30),
           iEmergencyAbsoluteValue(50),
           iEmergencyPercentageWhenHPSmallerThanAbsoluteValue(50),
           iRadicalAbsoluteValue(170),
           iRadicalPercentageWhenHPBiggerThanAbsoluteValue(80),
           iEmergencyPropertyCostForHPCoefficient(-1000),
           iEmergencyPropertyCostForSPCoefficient(-1000),
           iEmergencyPropertyCostForSTCoefficient(-100000),
           iEmergencyETInRangeEnemyCoefficient(-1000),
           iEmergencyETNotInRangeEnemyCoefficient(-800),
           iEmergencyLevelOneEnemyMultiplier(10),
           iEmergencyLevelTwoEnemyMultiplier(8),
           iEmergencyLevelThreeEnemyMultiplier(5),
           iEmergencyFoodBenefitCoefficient(50000),
           iEmergencyPerHPAddedCoefficient(600),
           iEmergencyOneHitKillBenefitCoefficient(38000),
           iEmergencyAttackBenefitDecreaseCoefficient(-50000),
           iEmergencyMaxHPAdded(4),
           iEmergencyMaxSTAdded(0),
           iEmergencyMaxSPAdded(4),
           iNormalPropertyCostForHPCoefficient(-1000),
           iNormalPropertyCostForSPCoefficient(-1000),
           iNormalPropertyCostForSTCoefficient(-1000),
           iNormalETInRangeEnemyCoefficient(-1000),
           iNormalETNotInRangeEnemyCoefficient(-500),
           iNormalLevelOneEnemyMultiplier(10),
           iNormalLevelTwoEnemyMultiplier(5),
           iNormalLevelThreeEnemyMultiplier(1),
           iNormalFoodBenefitCoefficient(46000),
           iNormalPerHPAddedCoefficient(500),
           iNormalOneHitKillBenefitCoefficient(50000),
           iNormalAttackBenefitDecreaseCoefficient(-1500),
           iNormalMaxHPAdded(2),
           iNormalMaxSTAdded(2),
           iNormalMaxSPAdded(2),
           iRadicalPropertyCostForHPCoefficient(-100000),
           iRadicalPropertyCostForSPCoefficient(-1000),
           iRadicalPropertyCostForSTCoefficient(-1000),
           iRadicalETInRangeEnemyCoefficient(-1000),
           iRadicalETNotInRangeEnemyCoefficient(-300),
           iRadicalLevelOneEnemyMultiplier(10),
           iRadicalLevelTwoEnemyMultiplier(3),
           iRadicalLevelThreeEnemyMultiplier(1),
           iRadicalFoodBenefitCoefficient(35000),
           iRadicalPerHPAddedCoefficient(400),
           iRadicalOneHitKillBenefitCoefficient(50000),
           iRadicalAttackBenefitDecreaseCoefficient(-1000),
           iRadicalMaxHPAdded(0),
           iRadicalMaxSTAdded(4),
           iRadicalMaxSPAdded(4),
           iAttackMaxHPRatio(3),
           iPointsToSave(4)
           {}
private:
    int mapc[N+1][M+1];
    typedef std::map<int, int>::iterator mapIterator;
    typedef std::map<int, int>::const_iterator mapConstIterator;
    typedef std::vector<int>::iterator vectorIterator;
    typedef std::vector<int>::const_iterator vectorConstIterator;

    std::map<int, int> idTable;
    std::vector<int> ETMaxHP;
    std::vector<int> ETSpeed;
    std::vector<bool> ifDead;
    std::vector<int> PosX;
    std::vector<int> PosY;
    int CurRound;
    bool DataValid;
    int myId;
    int myHP;
    int mySP;
    int myST;
    int myMaxHP;
    int myRemainingPoint;
    int playerCount;
    int averageETMaxHP;
    int averageETSpeed;

    int iEmergencyPercentage;
    int iEmergencyAbsoluteValue;
    int iEmergencyPercentageWhenHPSmallerThanAbsoluteValue;
    int iRadicalAbsoluteValue;
    int iRadicalPercentageWhenHPBiggerThanAbsoluteValue;

    static const int dx[4];
    static const int dy[4];

    struct GridEvaluator{
        int iMoveX, iMoveY;
        int HPGoal;
        int iAddHPBeforeAction;
        int iAddStrengthBeforeAction;
        int iAddSpeedBeforeAction;
        int iTargetX, iTargetY;
        int iEnvironmentDanger;
        int iEvaluationResult;
        void Evaluate(st20& mySelf);
        static int iPropertyCostForHPCoefficient;
        static int iPropertyCostForSPCoefficient;
        static int iPropertyCostForSTCoefficient;
        static int iETInRangeEnemyCoefficient;
        static int iETNotInRangeEnemyCoefficient;
        static int iLevelOneEnemyMultiplier;
        static int iLevelTwoEnemyMultiplier;
        static int iLevelThreeEnemyMultiplier;  //The enemies of higher(one) level are more dangerous.
        static int iFoodBenefitCoefficient;
        static int iPerHPAddedCoefficient;
        static int iOneHitKillBenefitCoefficient;
        static int iAttackBenefitDecreaseCoefficient;
        static int iMaxHPAdded;
        static int iMaxSTAdded;
        static void SetCoefficients(int pcfhpc, int pcfspc, int pcfstc, int eirec, int enirec, int loem, int ltem,
                                    int lthem, int fbc, int phpac, int ohkbc, int abdc, int mhpa, int msta){
            iPropertyCostForHPCoefficient = pcfhpc;
            iPropertyCostForSPCoefficient = pcfspc;
            iPropertyCostForSTCoefficient = pcfstc;
            iETInRangeEnemyCoefficient = eirec;
            iETNotInRangeEnemyCoefficient = enirec;
            iLevelOneEnemyMultiplier = loem;
            iLevelTwoEnemyMultiplier = ltem;
            iLevelThreeEnemyMultiplier = lthem;
            iFoodBenefitCoefficient = fbc;
            iPerHPAddedCoefficient = phpac;
            iOneHitKillBenefitCoefficient = ohkbc;
            iAttackBenefitDecreaseCoefficient = abdc;
            iMaxHPAdded = mhpa;
            iMaxSTAdded = msta;
        }
    };

    friend class GridEvaluator;

    int iEmergencyPropertyCostForHPCoefficient;
    int iEmergencyPropertyCostForSPCoefficient;
    int iEmergencyPropertyCostForSTCoefficient;
    int iEmergencyETInRangeEnemyCoefficient;
    int iEmergencyETNotInRangeEnemyCoefficient;
    int iEmergencyLevelOneEnemyMultiplier;
    int iEmergencyLevelTwoEnemyMultiplier;
    int iEmergencyLevelThreeEnemyMultiplier;
    int iEmergencyFoodBenefitCoefficient;
    int iEmergencyPerHPAddedCoefficient;
    int iEmergencyOneHitKillBenefitCoefficient;
    int iEmergencyAttackBenefitDecreaseCoefficient;
    int iEmergencyMaxHPAdded;
    int iEmergencyMaxSTAdded;
    int iEmergencyMaxSPAdded;

    int iNormalPropertyCostForHPCoefficient;
    int iNormalPropertyCostForSPCoefficient;
    int iNormalPropertyCostForSTCoefficient;
    int iNormalETInRangeEnemyCoefficient;
    int iNormalETNotInRangeEnemyCoefficient;
    int iNormalLevelOneEnemyMultiplier;
    int iNormalLevelTwoEnemyMultiplier;
    int iNormalLevelThreeEnemyMultiplier;
    int iNormalFoodBenefitCoefficient;
    int iNormalPerHPAddedCoefficient;
    int iNormalOneHitKillBenefitCoefficient;
    int iNormalAttackBenefitDecreaseCoefficient;
    int iNormalMaxHPAdded;
    int iNormalMaxSTAdded;
    int iNormalMaxSPAdded;

    int iRadicalPropertyCostForHPCoefficient;
    int iRadicalPropertyCostForSPCoefficient;
    int iRadicalPropertyCostForSTCoefficient;
    int iRadicalETInRangeEnemyCoefficient;
    int iRadicalETNotInRangeEnemyCoefficient;
    int iRadicalLevelOneEnemyMultiplier;
    int iRadicalLevelTwoEnemyMultiplier;
    int iRadicalLevelThreeEnemyMultiplier;
    int iRadicalFoodBenefitCoefficient;
    int iRadicalPerHPAddedCoefficient;
    int iRadicalOneHitKillBenefitCoefficient;
    int iRadicalAttackBenefitDecreaseCoefficient;
    int iRadicalMaxHPAdded;
    int iRadicalMaxSTAdded;
    int iRadicalMaxSPAdded;

    int iAttackMaxHPRatio;
    int iPointsToSave;
};

#endif  //_ST20_H_

//可自行增加所需函数所需函数及变量，但需保证上面每个函数的完整与可调用

//不要使用全局变量！！
