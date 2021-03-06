
#ifndef BATTLEACTION_H_
#define BATTLEACTION_H_

#include "Config.h"
#include <math.h>
#include "Common/Stream.h"
#define FIELD_WIDTH 1720
#define STEP 36
#define FIELD_HIGH  10*STEP

#define MIN(x,y) x>y?y:x
#define MIN_3(x,y,z) MIN((MIN(x,y)),z)

namespace Battle
{
    class BattleFighter;
    class BattleObject;
    //行为动作
    struct BattleActionStream
    {
        UInt16 _curtime;
        float _curtime2;
        BattleObject* _bo;
        UInt32 _prarm ;
        BattleActionStream(UInt16 curtime, BattleObject*  bo, UInt32 prarm):_curtime(curtime),_bo(bo),_prarm(prarm){ }
        BattleActionStream(float curtime, BattleObject*  bo, UInt32 prarm):_curtime2(curtime),_bo(bo),_prarm(prarm){ }
        UInt16 GetCurTime(){return _curtime;}
        float GetCurTime2(){return _curtime2;}
        BattleObject* GetBattleObject(){ return _bo;}
        UInt32 GetParam(){ return _prarm;}
    };

    class BattleAction
    {
        public:
            BattleAction(){};
            virtual  UInt32 GetAttack(){return 0;}
            virtual  UInt32 GetAttackImage(){return 0;}
            virtual  UInt32 GetHit(){return 0;}
            virtual  UInt32 GetWreck() {return 0;}
            virtual  UInt32 GetCritical() {return 0;}
            virtual  BattleFighter * GetBattleFighter(){return NULL;}
            virtual  bool CanCounter() {return true;}
            virtual UInt16 GetHappenTime(){ return 0;}
            virtual float GetHappenTime2(){ return 0;}
            virtual UInt16 GetObjectSize(){return 0;}
            virtual BattleObject* GetObject(UInt16 index){return NULL;}
            virtual UInt16 GetSkillId(){return 0;}
    };

    //攻击包info
    class ActionPackage  : public BattleAction
    {
        public:
            ActionPackage(BattleFighter * bf,UInt16 skillId,UInt16 time/*,BattleObject* bo*/):_bf(bf),_skillId(skillId),_time(time)//,_bo(bo)
        {
            vec_bo.clear();
        }
            ActionPackage(BattleFighter * bf,UInt16 skillId,float time/*,BattleObject* bo*/):_bf(bf),_skillId(skillId),_time2(time)//,_bo(bo)
        {
            vec_bo.clear();
        }
            ActionPackage(){}
            ~ActionPackage(){}
            UInt32 GetAttack(); //{if(!_bf) return 0; return _bf->GetAttack();}
            UInt32 GetAttackImage(); //{if(!_bf) return 0; return _bf->GetAttack();}
            UInt32 GetHit(); //{ if(!_bf) return 0; return _bf->GetHit();}
            UInt32 GetWreck(); //{ if(!_bf) return 0; return _bf->GetWreck();}
            UInt32 GetCritical(); //{ if(!_bf) return 0; return _bf->GetCritical();}
            //UInt32 GetBattleObject() {return _bo;}
            BattleFighter * GetBattleFighter(); // {return _bf;}
            void PushObject(BattleObject* bo); // { vec_bo.push_back(bo);}
            UInt16 GetObjectSize(); //{return vec_bo.size();}
            UInt16 GetHappenTime(); //{return _time;}
            float GetHappenTime2(); //{return _time;}
            UInt16 GetSkillId() { return _skillId;}

            BattleObject* GetObject(UInt16 index) ;//{if(index > vec_bo.size())return NULL; return vec_bo[index];}
        private: 
            //UInt16 skillScopeId;
            std::vector<BattleObject *> vec_bo;
            BattleFighter * _bf; //攻击发起者
            UInt16 _time;
            float _time2;
            UInt16 _skillId;
            //UInt8 _type;   //0 表示根据本包内容组织 1 表示根据攻击发起者进行组织(bf为NULL 或死亡，本次攻击失效)
    };

    //法术包info
    class ImagePackage : public BattleAction
    {
        public:
            ImagePackage(UInt16 skillId,UInt32 attack, UInt32 attackImage, UInt32 critical, UInt32 wreck, UInt32 hit , BattleFighter * bf , UInt16 time):_skillId(skillId),_attack(attack),_attackImage(attackImage),_critical(critical),_wreck(wreck),_hit(hit),_bf(bf),_time(time),_special(0),_sIndex(0){}
            ImagePackage(UInt16 skillId,UInt32 attack, UInt32 attackImage, UInt32 critical, UInt32 wreck, UInt32 hit , BattleFighter * bf , float time):_skillId(skillId),_attack(attack),_attackImage(attackImage),_critical(critical),_wreck(wreck),_hit(hit),_bf(bf),_time2(time){}   // BATTLE2
            UInt32 GetAttack(){return _attack;}
            UInt32 GetAttackImage(){return _attackImage;}
            UInt32 GetHit(){return _hit;}
            UInt32 GetWreck() {return _wreck;}
            UInt32 GetCritical() {return _critical;}
            BattleFighter * GetBattleFighter(){return _bf;}
            bool CanCounter() {return true;}
            UInt16 GetSkillId(){ return _skillId;}
            UInt16 GetHappenTime(){ return _time;}
            float GetHappenTime2(){ return _time2;}
            UInt16 GetObjectSize(){return vec_bo.size();}
            BattleObject* GetObject(UInt16 index = -1){if(index == -1) index = _sIndex++;if(index > vec_bo.size()) return NULL ; return vec_bo[index];}
            void PushObject(BattleObject* bo) { vec_bo.push_back(bo);}

            void SetSpecialType(UInt8 special){ _special = special;}
            UInt8 GetSpecialType(){return _special;}

        public:
            std::vector<BattleObject *> vec_bo;

        private:
            UInt16 _skillId;
            UInt32 _attack ;
            UInt32 _attackImage ;
            UInt32 _critical ;
            UInt32 _wreck ;
            UInt32 _hit;

            UInt16 skillScopeId;
            BattleFighter * _bf; //攻击发起者
            UInt16 _time;
            float _time2;

            UInt8 _special;  //特殊技能处理
            UInt8 _sIndex;
    };

    struct PointOb
    {
        UInt16 _x;
        UInt16 _y;
        PointOb(UInt16 x, UInt16 y):_x(x),_y(y){}
    };

    //粒子型info
    class ObjectPackage : public BattleAction
    { 
        public:
            ObjectPackage(UInt16 skillId,UInt32 attack,UInt32 attackImage, UInt32 critical, UInt32 wreck, UInt32 hit , BattleFighter * bf , UInt16 time):_skillId(skillId),_attack(attack),_attackImage(attackImage),_critical(critical),_wreck(wreck),_hit(hit),_bf(bf),_bo(NULL),_time(time),_xAdd(0),_yAdd(0),_flagX(0),_flagY(0),_count(1),_being(0){}
            ObjectPackage(UInt16 skillId,UInt32 attack, UInt32 attackImage, UInt32 critical, UInt32 wreck, UInt32 hit , BattleFighter * bf , float time):_skillId(skillId),_attack(attack),_attackImage(attackImage),_critical(critical),_wreck(wreck),_hit(hit),_bf(bf),_bo(NULL),_time2(time),_xAdd(0),_yAdd(0),_flagX(0),_flagY(0),_count(1),_being(0){}
            UInt32 GetAttack(){return _attack;}
            UInt32 GetAttackImage(){return _attackImage;}
            UInt32 GetHit(){return _hit;}
            UInt32 GetWreck() {return _wreck;}
            UInt32 GetCritical() {return _critical;}
            BattleFighter * GetBattleFighter(){return _bf;}
            BattleFighter * GetTargetFighter(){return _bo;}
            bool CanCounter() {return true;}
            UInt16 GetSkillId(){ return _skillId;}
            UInt16 GetHappenTime(){ return _time;}
            float GetHappenTime2(){ return _time2;}
            void SetBeing(UInt16 v) { _being = v;}
            UInt16 GetBeing() {return _being;}

            void GoNext()
            { 
                if(_bo != NULL)
                { 
                    GoForTarget();
                    return ;
                } 

                for(UInt8 i = 0; i < _point.size(); ++i)
                {
                    if(_xAdd)
                        _point[i]._x += _xAdd * _flagX - _xAdd * !_flagX;
                    if(_yAdd)
                        _point[i]._y += _yAdd * _flagY - _yAdd * !_flagY;
                    //COUT << " 粒子技能 " << static_cast<UInt32>(i) << " 行动 ：" << static_cast<UInt32>(_point[i]._x) << " , " << static_cast<UInt32>(_point[i]._y) << std::endl;
                }
            } 

            void GoForTarget();

            bool CanExit()
            { 
                if(!_count)
                    return true;
                for(UInt8 i = 0; i < _point.size(); ++i)
                {
                    if(_point[i]._x > FIELD_WIDTH || _point[i]._y > FIELD_HIGH)
                        return true;
                }
                if(!_xAdd && !_yAdd)
                    return true;
                return false;
            } 

            UInt16 getDistance(UInt16 x, UInt16 y)  //获得飞行轨迹到目标的距离
            { 
                UInt16 result = -1;
                for(UInt8 i = 0; i<_point.size(); ++i) 
                {
                    UInt16 result_bak = 0;
                    UInt16 _x = _point[i]._x;
                    UInt16 _y = _point[i]._y;
                    int x1 =static_cast<int>(_x);
                    int y1 =static_cast<int>(_y);
                    int x2 =static_cast<int>(_x + _xAdd);
                    int y2 =static_cast<int>(_y + _yAdd);
                    int x3 =static_cast<int>(x);
                    int y3 =static_cast<int>(y);
                    int res = (abs((y1-y2)*x3+(x2-x1)*y3-y1*x2+x1*y2)/sqrt(((y1-y2)*(y1-y2) + (x1-x2)*(x1-x2))));

                    int res1 = sqrt((x1-x3)*(x1-x3) + (y1-y3)*(y1-y3));
                    int res2 = sqrt((x2-x3)*(x2-x3) + (y2-y3)*(y2-y3));

                    if(!CanBeCounted(x,y,_x,_y))
                        result_bak =  static_cast<UInt16>(MIN(res1,res2));
                    else
                        result_bak = static_cast<UInt16>(res);
                    if(result_bak < result)
                        result = result_bak;
                }
                return result;
            } 

            bool CanBeCounted(UInt16 x, UInt16 y,UInt16 _x, UInt16 _y)  //判断斜率是否相同
            { 
                //return false;
                int x1 =static_cast<int>(_x);
                int y1 =static_cast<int>(_y);
                int x2 =static_cast<int>(_x + _xAdd);
                int y2 =static_cast<int>(_y + _yAdd);
                int x3 =static_cast<int>(x);
                int y3 =static_cast<int>(y);

                int res1  = (x3-x1)*(x2-x1) + (y3 - y1)*(y2 - y1) ;
                int res2  = (x3-x2)*(x1-x2) + (y3 - y2)*(y1 - y2) ;

                if(res1 < 0 || res2 < 0)
                    return false;

                return true;
            }

            void pushObjectPoint(UInt16 x,UInt16 y)
            { 
                _point.push_back(PointOb(x,y));
            } 

            void setObjectDirection(UInt8 flagX,UInt8 flagY, UInt16 xAdd, UInt16 yAdd ,UInt16 rad, BattleFighter* bo = NULL)  //飞行系
            { 
                _xAdd = xAdd;
                _yAdd = yAdd;
                _flagX = flagX;
                _flagY = flagY;
                _rad = rad;
                _bo = bo;
            } 

            void setObjectCount(UInt8 count ) { _count = count;}

            bool CheckFighterInSCope(BattleObject* bo);  //非指向性
            void InsertIntoPackage(UInt16 curtime , BattleObject* bo , UInt32 param)
            { 
                if(_count)
                { 
                    --_count;
                    vec_struct.push_back(BattleActionStream(curtime, bo, param));
                }
                //return ;
            } 
            UInt8 BuildStream(Stream& st);

            bool CheckFighterAttacked(BattleObject * bo);
            
            void SetEffectType(UInt8 effect){ _effectType = effect;}
            UInt8 GetEffectType() { return _effectType;}
        public:
            std::vector< BattleActionStream >  vec_struct;
        private:

            UInt16 _skillId;
            UInt32 _attack ;
            UInt32 _attackImage ;
            UInt32 _critical ;
            UInt32 _wreck ;
            UInt32 _hit;

            //            UInt16 skillScopeId;
            BattleFighter * _bf; //攻击发起者
            BattleFighter * _bo; //受击者
            UInt16 _time;
            float _time2;
            
            std::vector<struct PointOb> _point;  //粒子坐标

            UInt16 _xAdd;   //x 步长
            UInt16 _yAdd;   //y 步长

            UInt8  _flagX;  //x方向  0--向左  1--向右
            UInt8  _flagY;  //y方向  0--向下  1--向上
            UInt16 _rad;   //物体距离

            UInt8 _count;

            UInt8 _effectType;
            UInt16 _being; // 发射时间

    };
}
#endif // BATTLEACTION_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

