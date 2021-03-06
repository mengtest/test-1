
#ifndef FIGHTER_H_
#define FIGHTER_H_
#include "Config.h"
#include "GGlobalObjectManager.h"
#include "FVar.h"
#include "Common/Stream.h"

namespace GObject
{
    class ItemWeapon;
    class ItemArmor;
    class Player;
    class FVarSystem;

#define ENCHANT_MAX 9
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

    enum
    {
        e_fighter = 0,
        e_walk = 1,
        e_ride = 2,
        e_shoot = 3,
        e_advice = 4,
        e_lance = 5,
        e_fighter_max
    };

    class Fighter
    {
        public:
            Fighter(UInt32 id , Player*);
            ~Fighter();
            virtual Fighter * Clone(Player * owner);
            //virtual Fighter * cloneWithEquip(Player * owner); 
            //virtual Fighter * cloneWithOutDirty(Player * player);
            //Set
            inline void SetOwner(Player * p) { _owner = p; }
            inline void SetName(const std::string& s) {_name = s;}
            inline void SetClass(UInt8 c) { _class = c; }
            inline void SetSex(UInt8 s) {_sex = s;}
            inline void SetLevel(UInt8 l, bool boss = false) { _level = l; }// if (boss) worldBoss.SetLevel(l); }
            inline void SetExp(UInt64 e) {_exp = e;}
            void SetLevelAndExp(UInt8 l, UInt64 e);   //TODO
            ItemWeapon * SetWeapon(ItemWeapon * w, bool = true);  //TODO
            ItemArmor * SetArmor(int idx, ItemArmor * a, bool = true);  //TODO
            void SetColor(UInt8 color) { _color = color;}
            void SetTypeId(UInt8 type) { _typeId = type;}
            UInt8 GetTypeId(){return _typeId;}
            void SetChildTypeId(UInt16 type) { _childTypeId = type;}

            UInt16 GetChildTypeId(){return _childTypeId;}
            //Get
            inline UInt32 getId() { return _id; }
            inline Player * GetOwner() { return _owner; }
            inline UInt8 GetClass() {return _typeId;}
            inline UInt8 GetSex(){return _sex;}
            inline ItemWeapon * GetWeapon() { return _weapon; } 
            inline ItemArmor * GetArmor(int idx) { return (idx >= 0 && idx < 5) ? _armor[idx] : NULL; }
            inline UInt8 GetColor(){return _color;}

            UInt8 GetSide();//{ if(_owner) return _owner->GetBattleSide();}
            FVarSystem* GetFVar();

            UInt32 GetVar(UInt32 num);
            void SetVar(UInt32 num, UInt32 val);
            void AddVar(UInt32 num, UInt32 val);

            void SetRad(UInt16 rad) { _bodySize = rad;} 
            UInt16 GetRad() { return _bodySize;}

            UInt8 GetLevel(){return _level;}

            void SetSpeed(UInt16 speed){ _speed = speed;}

            UInt16 GetSpeed(){return _speed;}

            void SetBaseAttr(UInt32 hp,UInt32 attack , UInt32 defend, UInt32 magatk, UInt32 magdef, UInt32 critical,UInt32 criticalDef,UInt32 hit, UInt32 evade) 
            { 
                _hp = hp;
                _attack = attack;
                _defend = defend;
                _magatk = magatk;
                _magdef = magdef;
                _critical = critical;
                _criticalDef = critical;
                _hit = hit;
                _evade = evade;
            } 
            void SetBaseSkill(UInt8 level ,UInt16 skillId); // UInt16 skillCondId, UInt16 skillScopeId, UInt16 skillEffectId);

            void SetSkill(std::string skill);

            void SetArenaPos(UInt16 pos){ _Arena = pos;}
            UInt16 GetArenaPos() { return _Arena;}

            UInt32 GetFighterAttr(UInt8 index)
            { 
                static UInt32 baseUp[]= {0,100,200,300,400,500,600,700,800,900,1000,1100,1200,1300,1400,1500,1600,1700,1800,1900};
                UInt32 up = GetVar(27); // 星级（品质）编号
                if(up >= 20)
                    return 0;
                if(index >= e_attr_max)
                    return 0;
                
                // 缺少了装备的属性加成
                UInt32 attr = 0;// = GetFighterEquipUpAttr(index);

                switch(index)
                { 
                    case e_attr_attack:
                        attr = _attack * (baseUp[up] + 100)/100 ;
                        break;
                    case e_attr_magatk:
                        attr =  _magatk * (baseUp[up] + 100)/100 ;
                        break;
                    case e_attr_defend:
                        attr =  _defend * (baseUp[up] + 100)/100 ;
                        break;
                    case e_attr_magdef:
                        attr =  _magdef * (baseUp[up] + 100)/100 ;
                        break;
                    case e_attr_critical:
                        attr = _critical * (baseUp[up] + 100)/100;
                        break;
                    case e_attr_criticalDef:
                        attr =  _criticalDef * (baseUp[up] + 100)/100;
                        break;
                    case e_attr_hit:
                        attr =  _hit * (baseUp[up] + 100)/100;
                        break;
                    case e_attr_evade:
                        attr =  _evade * (baseUp[up] + 100)/100;
                        break;
                    case e_attr_hp:
                        attr =  _hp * (baseUp[up] + 100)/100;
                        break;
                } 

                attr += GetFighterEquipUpAttr(index);

                return attr;
            } 

            UInt32 GetFighterEquipUpAttr(UInt8 index)
            { 
               UInt8 cls = GetTypeId();
               UInt16 arena = 0;
               UInt16 arenaLevel = 0;
               if(!_owner && GetArenaPos() < 3001)
                   arenaLevel = (3001 - GetArenaPos())*15/3000 + 15;

               switch(index)
               { 
                   //攻击
                   case e_attr_attack:
                       {
                           //步兵，骑兵，弓手，枪兵，谋士
                           UInt32 AttackForWeapon[e_fighter_max] = {40,50,30,0,40};
                           UInt32 value = GetVar(FVAR_WEAPON_ENCHANT);
                           if(arenaLevel)
                               value = arenaLevel;
                           return AttackForWeapon[cls] * (value/(ENCHANT_MAX+1)+1) +AttackForWeapon[cls] * (20 + value%(ENCHANT_MAX+1))/20;
                           //return AttackForWeapon[cls] * (2 << (value/(ENCHANT_MAX+1))) * (100 + value%(ENCHANT_MAX+1))/20;
                       }
                       //魔法攻击
                   case e_attr_magatk:
                       {
                           UInt32 AttackForWeapon[e_fighter_max] = {0,0,0,30,0};
                           UInt32 value = GetVar(FVAR_WEAPON_ENCHANT);
                           if(arenaLevel)
                               value = arenaLevel;
                           return AttackForWeapon[cls] * (value/(ENCHANT_MAX+1)+1) +AttackForWeapon[cls] * (20 + value%(ENCHANT_MAX+1))/20;
                           //return AttackForWeapon[cls] * (2 << (value/(ENCHANT_MAX+1))) * (100 + value%(ENCHANT_MAX+1))/20;
                       }
                       break;
                       //防御
                   case e_attr_defend:
                       {
                           UInt32 AttackForArmor1[e_fighter_max] = {50,40,25,25,40};
                           UInt32 value = GetVar(FVAR_ARMOR1_ENCHANT);
                           if(arenaLevel)
                               value = arenaLevel;
                           return AttackForArmor1[cls] * (value/(ENCHANT_MAX+1)+1) +AttackForArmor1[cls] * (20 + value%(ENCHANT_MAX+1))/20;
                           //return AttackForArmor1[cls] * (2 << (value/(ENCHANT_MAX+1))) * (100 + value%(ENCHANT_MAX+1))/20;
                       }
                       break;
                       //魔法防御
                   case e_attr_magdef:
                       {
                           UInt32 AttackForArmor2[e_fighter_max] = {30,30,35,40,30};
                           UInt32 value = GetVar(FVAR_ARMOR2_ENCHANT);
                           if(arenaLevel)
                               value = arenaLevel;
                           return AttackForArmor2[cls] * (value/(ENCHANT_MAX+1)+1) +AttackForArmor2[cls] * (20 + value%(ENCHANT_MAX+1))/20;
                           //return AttackForArmor2[cls] * (2 << (value/(ENCHANT_MAX+1))) * (100 + value%(ENCHANT_MAX+1))/20;
                       }
                       break;
                       //暴击
                   case e_attr_critical:
                       {
                           UInt32 AttackForArmor4[e_fighter_max] = {9,12,10,10,10};
                           UInt32 value = GetVar(FVAR_ARMOR4_ENCHANT);
                           if(arenaLevel)
                               value = arenaLevel;
                           return AttackForArmor4[cls] * (value/(ENCHANT_MAX+1)+1) +AttackForArmor4[cls] * (20 + value%(ENCHANT_MAX+1))/20;
                           //return AttackForArmor4[cls] * (2 << (value/(ENCHANT_MAX+1))) * (100 + value%(ENCHANT_MAX+1))/20;
                       }
                       break;
                       //抗爆
                   case e_attr_criticalDef:
                       {
                           UInt32 AttackForArmor4[e_fighter_max] = {6,3,5,5,5};
                           UInt32 value = GetVar(FVAR_ARMOR4_ENCHANT);
                           if(arenaLevel)
                               value = arenaLevel;
                           return AttackForArmor4[cls] * (value/(ENCHANT_MAX+1)+1) +AttackForArmor4[cls] * (20 + value%(ENCHANT_MAX+1))/20;
                           //return AttackForArmor4[cls] * (2 << (value/(ENCHANT_MAX+1))) * (100 + value%(ENCHANT_MAX+1))/20;
                       }
                       break;
                       //命中
                   case e_attr_hit:
                       {
                           UInt32 AttackForArmor5[e_fighter_max] = {10,10,10,10,10};
                           UInt32 value = GetVar(FVAR_ARMOR5_ENCHANT);
                           if(arenaLevel)
                               value = arenaLevel;
                           return AttackForArmor5[cls] * (value/(ENCHANT_MAX+1)+1) +AttackForArmor5[cls] * (20 + value%(ENCHANT_MAX+1))/20;
                           //return AttackForArmor5[cls] * (2 << (value/(ENCHANT_MAX+1))) * (100 + value%(ENCHANT_MAX+1))/20;
                       }
                       break;
                       //闪避
                   case e_attr_evade:
                       {
                           UInt32 AttackForArmor5[e_fighter_max] = {2,2,2,2,2};
                           UInt32 value = GetVar(FVAR_ARMOR5_ENCHANT);
                           if(arenaLevel)
                               value = arenaLevel;
                           return AttackForArmor5[cls] * (value/(ENCHANT_MAX+1)+1) +AttackForArmor5[cls] * (20 + value%(ENCHANT_MAX+1))/20;
                           //return AttackForArmor5[cls] * (2 << (value/(ENCHANT_MAX+1))) * (100 + value%(ENCHANT_MAX+1))/20;
                       }
                       break;
                       //血量
                   case e_attr_hp:
                       {
                           UInt32 AttackForArmor3[e_fighter_max] = {40,40,20,20,50};
                           UInt32 value = GetVar(FVAR_ARMOR3_ENCHANT);
                           if(arenaLevel)
                               value = arenaLevel;
                           return AttackForArmor3[cls] * (value/(ENCHANT_MAX+1)+1) +AttackForArmor3[cls] * (20 + value%(ENCHANT_MAX+1))/20;
                           //return AttackForArmor3[cls] * (2 << (value/(ENCHANT_MAX+1))) * (100 + value%(ENCHANT_MAX+1))/20;
                       }
                       break;
               } 
            } 

            void MakeFighterInfo(Stream& st);

            //强化更新
            void updateEuipmentLoad(UInt8 index);
            UInt32 GetEquipmentUpgradeLoad(UInt8 index);
            UInt32 GetTotalPower() ;  //获得总战力
            std::string GetName() const { return _name;}
            UInt32 GetHP() const { return _hp;}

            std::vector<UInt16> GetBaseSkills(){return m_baseSkills;}

            std::vector<UInt16> m_baseSkills;
    private:
            UInt32 _id;
            Player * _owner;
            std::string _name;
            UInt8 _class ;
            UInt8 _sex;
            UInt64 _exp;
            UInt8 _level;
            ItemWeapon * _weapon;
            ItemArmor * _armor[5];
            FVarSystem* fVarSystem;

            UInt16 _speed;  //行动速度
            UInt16 _bodySize;  //模型体积半径

            UInt32 _hp;
            UInt32 _attack;
            UInt32 _defend;
            UInt32 _magatk;
            UInt32 _magdef;
            UInt32 _critical;
            UInt32 _criticalDef;
            UInt32 _hit;
            UInt32 _evade;

            UInt8 _typeId;
            UInt16 _childTypeId;
            UInt8 _color; 
            std::map<UInt8,UInt16> m_skills;  //仅global使用
            UInt16 _Arena;
};
typedef GGlobalObjectManagerT<Fighter, UInt32> GlobalFighters;
extern GlobalFighters globalFighters;
}
#endif // FIGHTER_H_
