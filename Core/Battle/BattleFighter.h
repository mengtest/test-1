
#ifndef BATTLEFIGHTER_H_
#define BATTLEFIGHTER_H_
//#include "GObject/Fighter.h"
#include "GData/AttrExtra.h"   
#include <set>
#include "BattleObject.h"
#include "GObject/Fighter.h"
#include "BattleAction.h"
#include "Battle/BattleSimulator.h"
#include "Script/GameActionLua.h"
#include "GObject/Country.h"

namespace Script
{
    class BattleFormula;
}
namespace GObject
{
    class Fighter;
}
namespace Battle
{
#define  ACTION_WAIT 0  //动作延迟起作用
#define  ACTION_HAPPEN 1  //动作立即起作用
#define  MYFIGHTERMAX 10
#define  A_SUB_B(x,y) (x)>(y)?(x)-(y):0


    enum 
    {
        e_none = 0, //待机
        e_run = 1, //行进
        e_attack_near = 2, //近距离攻击
        e_attack_middle = 3, //中距离攻击
        e_attack_distant = 4, //远距离攻击
        e_image_attack = 5,  //魔法攻击
        e_image_therapy = 6, //魔法治疗
        //e_attack_counter = 7, //反击
        e_attack_back = 7, //击退（眩晕处理）
        e_attack_quick = 8 , //快攻
        e_run_attack = 9 ,   //跑攻 (骑兵专用)
        e_object_image = 10,
        e_object_attack = 11, //弓箭（追击型）
        e_image_attack_time_special = 12, // 根据目标数量决定施法时间
        e_parry = 13, // 格挡
        e_be_attacked = 14
    };

    /*
    enum
    {
        e_attr_attack = 0,
        e_attr_magatk = 1,
        e_attr_defend = 2,
        e_attr_magdef = 3,
        e_attr_critical = 4,
        e_attr_criticalDef = 5,
        e_attr_hit = 6,
        e_attr_evade = 7,
        e_attr_hp = 8,
        e_attr_max
    };
    */

    struct BattleBuff
    {
        UInt16 buffId;
        UInt8 count;
        std::vector<UInt16> value;
        BattleBuff(UInt16 buffId1, UInt8 count1):buffId(buffId1),count(count1){}
    };

    struct ActionBase
    {
        ActionBase(UInt16 skillId /*UInt16 condition ,UInt16 scope, UInt16 effect*/):/*_condition(condition),_scpoe(scope),_effect(effect),*/_skillId(skillId),_cd(0),_priority(0) { } 
        ActionBase():/*_condition(0),_scpoe(0),_effect(0),*/_skillId(0),_cd(8),_priority(0){}
        UInt16 _skillId;
        //UInt16 _condition ;  // 触发条件编号
        //UInt16 _scpoe ;      // 触发范围编号
        //UInt16 _effect ;     // 触发效果编号
        UInt16 _cd;           //行动cd  //BATTLE2
        UInt8 _priority ;  //触发优先级
    };
    struct lt_absort
    {  
        bool operator()(ActionBase& a, ActionBase& b) const { return a._priority > b._priority;}
    };

    typedef std::list<ActionBase /*, lt_absort*/> ActionSort;

    class BattleField;

    class BattleFighter:
        public BattleObject
    {
        friend class BattleSimulator;
        public:
            BattleFighter(UInt8 Class ,Script::BattleFormula * bf,GObject::Fighter * = NULL,UInt8 pointX = 0, UInt8 pointY = 0);
            virtual ~BattleFighter();

            void setFighter( GObject::Fighter * f );

            void StartAction(UInt8 actionType);
            inline UInt8 GetActionLast(){ return _actionLast;}  //获得当前状态
            inline UInt8 GetActionBackLast(){ return _actionBackLast;}  //获得当前状态

            UInt8 GetSide() { if(!_isChild) return _fighter->GetSide(); if(m_mainFighter) m_mainFighter->GetSide();return 0;}
            void SetGroundX(UInt8 x){_groundX = x;}
            void SetGroundY(UInt8 y){_groundY = y;}
            UInt8 GetGroundX(){ return _groundX;}
            UInt8 GetGroundY(){ return _groundY;}
            UInt8 GetTypeId() { if(_fighter) return _fighter->GetTypeId(); return 0;}

            virtual void Action();  //行动
            //移动
             void GoForward(UInt8 flag = 0 ,UInt16 count = 0);
             ActionPackage MakeActionEffect();   //实现动作效果  伤害 法术等

            //被击
            UInt32 BeActed(BattleAction *  bAction ); //是否延迟

            //添加本身数据包

            //战斗时期
            //从战斗中或许自己该有的行为
            void GetActionFromField();
            //updateAction
            void UpdateActionList();

            ActionBase GetActionCurrent(UInt16 advance, UInt8 flag = 0);
            void SetField(BattleField* bfield){ _field = bfield;}

            BattleField * GetField();

            UInt8 GetMovePower();
            UInt8 GetClass(){ if(_fighter) return _fighter->GetClass();return 0;}
            UInt8 GetAttackRange();
            virtual UInt8 GetDistance(){ return 1;}   //
            virtual UInt16 GetId(){ if(_fighter) return _fighter->getId(); return 0;}

            void setNumber(UInt8 num){ _number = num;}
            UInt8 getNumber(){ return _number; }

            void setMainFighter(BattleFighter * bf){
                m_mainFighter = bf;
                //setHP(100);//m_mainFighter->getHP());
                //for(UInt8 i = e_attr_attack ; i < e_attr_max; ++i)
                //{ 
                //    attrBase[i] = m_mainFighter->GetBattleAttr(i);
                //} 
                //AddSkill();

                _isChild = true;
            }

            void PutBattleFighters(BattleSimulator& bsim);
            BattleFighter* getMyFighters(UInt8 index);

            virtual UInt16 GetRad(){if(_fighter) return _fighter->GetRad(); return 0;}
            void SetEnterPos(UInt8 x1 , UInt8  y1){ if(!_isChild)return ; EnterX = x1; EnterY = y1;}
            inline UInt8 GetEnterPosX(){ return EnterX;}
            inline UInt8 GetEnterPosY(){ return EnterY;}

            virtual void InsertFighterInfo(Stream& st ,UInt8 flag = 0);

            void SetBattleIndex(UInt8 x) { _battleIndex = x;}
            UInt16 GetBattleIndex() { return _battleIndex;}

            virtual void SetMinXY(UInt16 x,UInt16 y) { _minX = x; _minY = y; resetBattleStatue();} //入场
            UInt16 GetMinX(){return _minX;}
            UInt16 GetMinY(){return _minY;}
            
            void SetSideInBS(UInt8 side) { _sideInBS = side;}
            UInt8 GetSideInBS() { if(m_mainFighter&&m_mainFighter!= this) return m_mainFighter->GetSideInBS(); return _sideInBS;}

            void SetBattleTargetPos(UInt16 x,UInt16 y){ _battleTargetX = x; _battleTargetY = y;}

            virtual UInt8 GetBSNumber() { return _number + GetSideInBS()* (MYFIGHTERMAX +1);}//GetField()->GetFirstSize();}

            void SetNowTime(UInt16 time ){ /*if(_nowTime != time ) SetGone(false);*/ _nowTime = time;}
            UInt16  GetNowTime() { return _nowTime;}

            void SetNowTime(float time ){ /*if(_nowTime2 != time ) SetGone(false);*/ _nowTime2 = time;}
            float  GetNowTime2() { return _nowTime2;}
            //Virtual 
            virtual bool PreGetObject(){ return false;}  //设定攻击对象，以及战斗
            virtual void BuildLocalStream(UInt8 wait = 0 , UInt8 param = 0);
            virtual UInt16 GetTargetDistance(){ return -1;}

            UInt16 GetBaseSpeed() 
            {
                if(_fighter) 
                    return _fighter->GetSpeed();
                return 200; 
            } 
            std::vector<UInt16> GetBaseSkills()
            {
                std::vector<UInt16> vec;
                if(_fighter)
                    vec = _fighter->GetBaseSkills();
                else if(m_mainFighter)
                    vec = m_mainFighter->GetBaseSkills();
                return vec;
            }
            virtual UInt16 GetSpeed() { return GetBaseSpeed();}

            virtual void resetBattleStatue();

            virtual void SetGone(bool v){ }
            virtual bool GetGone(){return true;}

            virtual UInt8 GetRideCount() {return 1;}

            virtual UInt16 GetBaseActionNum() = 0 ; //(基础攻击技能)弃用
            
            UInt32 GetAttack()
            { 
                return GetTotalAttr(e_attr_attack)*(100 + GetField()->GetAttackUp(GetSideInBS()))/100; 
            }
            UInt32 GetAttackImage(){ return GetTotalAttr(e_attr_magatk);}

            UInt32 GetDefend(){ return GetTotalAttr(e_attr_defend);}
            UInt32 GetImageDefend(){ return GetTotalAttr(e_attr_magdef);}
            UInt32 GetHit() { return GetTotalAttr(e_attr_hit);}
            UInt32 GetEvade() {return GetTotalAttr(e_attr_evade);}
            UInt32 GetCritical() { return GetTotalAttr(e_attr_critical);}
            UInt32 GetCriticalDef() { return GetTotalAttr(e_attr_criticalDef);}

            UInt32 GetWreck() { return _wreck;}
            UInt32 GetParry(){ return _parry;}

            ImagePackage MakeImageEffect();

            UInt32 GetBattleAttr(UInt8 index, UInt8 type =0)
            { 
                if(index >= e_attr_max)
                    return 0;
                switch(type)
                { 
                    case 0:
                        return attrBase[index];
                    case 1:
                        return attrAdd[index];
                    case 2:
                        return attrSub[index];
                } 
                return 0;
            } 

            void SetBattleAttr(UInt8 index , UInt32 val, UInt8 type)
            { 
                if(index >= e_attr_max)
                    return; 
                switch(type)
                { 
                    case 0:
                        attrBase[index] = val;
                        break;
                    case 1:
                        attrAdd[index] = val;
                        break;
                    case 2:
                        attrSub[index] = val;
                        break;
                } 
            } 

            UInt32 GetTotalAttr(UInt8 index)
            { 
                if(index >= e_attr_max)
                    return 0;
                UInt8 multiKill = GetMultiKill();
                //连杀导致战斗属性下降
                if(multiKill > 19)
                    multiKill = 19;
                return (A_SUB_B(attrBase[index]+attrAdd[index] , attrSub[index]))* (20 - multiKill )* 5 / 100;
            } 

            void AddBuff(UInt16 buffId);
            void CheckBuff();
            void AddSkill() ;
            
            //BattleAction
            virtual UInt8 NormolAttack();
            virtual UInt8 NormolImage();
            virtual UInt8 NormolObject();

            BattleObject * GetTarget(){ return _target;};

            void SetBattleDirection(UInt8 direct){ if(_direction == direct) return; _direction = direct;}
            virtual UInt8 GetBattleDirection(){return _direction;}  //0左1右2全部
            virtual UInt8 BeForAction() { PreGetObject(); return 0;};

            void BattlePrintf();

            virtual bool IsStoped(){return false;}

            void AddKillCount1(){ ++_killCount1;}
            UInt16 GetKillCount1(){ return _killCount1;}
            void AddKillCount2(){ ++_killCount2;}
            UInt16 GetKillCount2(){ return _killCount2;}

            GObject::Player* GetOwner() const { return _fighter->GetOwner();}   //by qsj
            //设置某一小兵的血量
            void SetSoldierHp(UInt8 index,UInt32 hp);
            //获得某一小兵的血量
            UInt32 GetSoldierHp(UInt8 index);
            //设置主将血量
            //void SetMainFighterHP(UInt32 hp);
            //获得主将血量
            //UInt32 GetMainFighterHP() const { return m_mainFighter->getHP();}
            virtual void SetHighSpeed(bool v){ }//_isHighSpeed = v;}

            void SetBeginTime(UInt16 time) { _beginTime = time ;}
            float GetBeginTime(){return _beginTime;}

            UInt8 GetFighterNum() 
            {
                UInt8 count = 0;
                for( UInt8 i = 0; i < MYFIGHTERMAX ; ++i )
                {
                    if( m_fighters[i] !=  NULL && m_fighters[i]->getHP() > 0  )
                    {
                          ++count;
                    }
                }
                return count;
            }
            void SetCachePx(UInt16 cache) { _cachePx = cache ;}
            UInt16 GetCachePx(){ return _cachePx;}

            //如果可以移动返回false ， 不可以移动返回true
            virtual bool CanMove() { return true; } // return _canMove;}
            virtual void SetMove(bool v) {}// _canMove = v;}

            void SetActed(bool v){ _acted = v;}
            bool GetActed(){ return _acted;}

            void SetAvoidHurt(bool v){ _avoidhurt = v;}
            bool GetAvoidHurt(){ return _avoidhurt;}

            bool IsMainFighter(){return !_isChild;}
            
            void ResetSkill();//{}

            UInt32 GetTotalPower() { return _fighter->GetTotalPower();}

            void AddEnergy(UInt32 energy){ if(!_isChild) _energy += energy;}
            UInt32 GetEnergy(){ return _energy;}

            bool GetAttackedSkill(UInt8& flag);

            void SetMultiKill(UInt8 kills){ _multiKill = kills;}
            UInt8 GetMultiKill(){ if(!_isChild || !m_mainFighter) return _multiKill; return m_mainFighter->GetMultiKill();}
        protected:

            UInt8 _crick;  //硬直
            UInt16 _actionLast ;   //动作持续
            UInt16 _actionBackLast ;   //动作收招持续
            //std::list<BattleObject*>  targetList; //对象列表 (待解)
            UInt8 _actionType;  // 动作类型

            Script::BattleFormula * _formula;
            BattleFighter * m_fighters[MYFIGHTERMAX];  //小兵指针数组
            BattleFighter * m_mainFighter;   //主将指针
            UInt8 _number;   //所在阵营

            ActionBase _ab;  //当前选择的行为

            ActionSort  preActionList;   //动作行为列表O
            ActionSort  preActionCD;     //待CD动作行为列表

            GObject::Fighter * _fighter;  //本体fighter指针
            BattleField * _field;        //场地指针

            //战场属性
            UInt8 EnterX;
            UInt8 EnterY;

            //属性
            UInt8 _crickSum;  //硬直累计
            UInt8 _crickLev; //硬直等级

            //霸体 (暂时未使用)
            UInt8 _stoic;
            UInt8 _stoicLev;

            UInt32 _wreck; //破击
            UInt32 _parry;  //格挡

            UInt8 _groundX; //战场位置
            UInt8 _groundY;

            UInt16 _battleIndex; 

            UInt16 _nowTime; //行动时间
            float _nowTime2; //行动时间(备用)

            UInt16 _beginTime;  //起始动作时间

            UInt16 _minX;  //入场X坐标  (用于计算骑兵的行径路线)
            UInt16 _minY;  //入场Y坐标  

            UInt16 _battleTargetX;        //战斗对象的坐标
            UInt16 _battleTargetY;

            UInt8 _sideInBS;              //进攻方0 防守方1

            UInt32 attrBase[e_attr_max];  //基础属性
            UInt32 attrAdd[e_attr_max];   //buff增加属性
            UInt32 attrSub[e_attr_max];  //buff减少属性值

            std::list<BattleBuff> bufflst;  //bufflist
            UInt8 _direction;    //方向（步兵，骑兵有方向区别）
            UInt16 _killCount1;  // 击杀主将
            UInt16 _killCount2;  // 击杀小兵

            UInt16 _cachePx;   //曾经使用于缓冲x累加值

            bool _acted;  //XXX

            bool _avoidhurt;  //是否无敌

            bool _isChild;   //与IsMainFighter 效果相同

            UInt32 _energy;    //能量

            UInt8 _multiKill;  //连杀
        public:
            BattleFighter * _target;  //攻击对象（步兵固定）
    };

}
#endif // BATTLEFIGHTER_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

