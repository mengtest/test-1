#include "Config.h"
//#include "GObject/Country.h"
#include "BattleField.h"
//#include "GObject/FairyPet.h"
#include "BattleFighter.h"
#include "GData/SkillTable.h"
#include <math.h>

#define SUB(x,y) (x>y)?(x-y):(y-x)
#define Min(x,y) (x)>(y)?(y):(x)
namespace Battle
{

    BattleField::BattleField(UInt8 mapId1, UInt8 mapId2, UInt16 fieldDistance, UInt16 timeActionLimit):_mapId1(mapId1),_mapId2(mapId2),_fieldDistance(fieldDistance),_timeActionLimit(timeActionLimit)
    {
        //memset(_objs, 0, sizeof(_objs));
        _super = NULL;
        _timeExtra = 0;
    }

    BattleField::~BattleField()
    {
        clear();
    }
    void BattleField::clear()
    {

    }

    BattleFighter * BattleField::GetTargetForRide(UInt8 side, UInt16 posX ,UInt16 posY,UInt8 direction)
    { 
        if(side > 1)
            return NULL;
        UInt16 maxy = -1;
        UInt16 maxx = 0;
        UInt8 res = -1;
        for(UInt8 i = 0; i < _fighters[side].size(); ++i)
        { 
            if(!_fighters[side][i] || !_fighters[side][i]->getHP())
                continue;
            if(_fighters[side][i]->getClass() > eFighter)
                return NULL;
            UInt16 x = _fighters[side][i]->getPosX();
            UInt16 y = _fighters[side][i]->getPosY();

            if(direction != 2)
            {
                if(posY != y )
                    continue ;
                if( direction == 1 && posX > x)   //表示右冲锋
                    continue ;
                if(direction == 0 && posX < x)
                    continue ;
            }

            UInt16 advanceY = SUB(y,posY);
            UInt16 advanceX = SUB(x,posX);
            if( advanceY <= maxy)
            {
                if(advanceY == maxy && advanceX >= maxx)
                    continue ;
                { 
                    maxy = advanceY;
                    maxx = advanceX;
                    res = i;
                } 
            }
        } 

        if(res < _fighters[side].size())
        { 
            return _fighters[side][res];
        } 
        return NULL;
    } 
    BattleFighter * BattleField::GetTarget(UInt8 side, UInt16 posX ,UInt16 posY, BattleFighter* cur)
    { 
        if(side > 1)
            return NULL;
        ////COUT << "选择的Side:" << static_cast<UInt32>(side) << std::endl;
        //FieldPrint();
        if(cur!= NULL && cur->GetSideInBS() == side)
        {
            //COUT << "The Same Side "  << std::endl;
        }
        UInt16 max = -1;
        UInt8 res = -1;
        for(UInt8 i = 0; i < _fighters[side].size(); ++i)
        { 
            if(!_fighters[side][i] || !_fighters[side][i]->getHP())
                continue;
            if(_fighters[side][i]->getClass() > eFighter)
                return NULL;
            UInt16 x = _fighters[side][i]->getPosX();
            UInt16 y = _fighters[side][i]->getPosY();

            UInt16 advanceY = SUB(y,posY);
            UInt16 advanceX = SUB(x,posX);
            UInt16 dis = sqrt(advanceY*advanceY + advanceX*advanceX);
            if(dis <= max)
            {
                { 
                    max = dis;
                    res = i;
                } 
            }
        } 

        if(res < _fighters[side].size())
        { 
            return _fighters[side][res];
        } 
        return NULL;
    } 

    UInt16 BattleField::getDistance(UInt16 x1, UInt16 y1, UInt16 x2, UInt16 y2)
    { 
        UInt16 adx = x1>x2?(x1-x2):(x2-x1);
        UInt16 ady = y1>y2?(y1-y2):(y2-y1);
        if(ady > 0 )
            --ady;
        return adx + ady;
    } 

    void BattleField::setObject(UInt16 x , UInt16 y , BattleObject * bo ,UInt16 isBody)
    { 
        //if(_objs[x][y] != NULL)
        //{   
        //    if(!_objs[x][y])
        //        delete _objs[x][y];
        //    _objs[x][y] = NULL;
        //}   
        //_objs[x][y] = bo;
    } 

    UInt16 BattleField::getDistance(BattleObject * bf , BattleObject* bo)
    { 
        UInt16 x1 = bf->getPosX()*1.0;
        UInt16 y1 = bf->getPosY()*1.0;
        UInt16 x2 = bo->getPosX()*1.0;
        UInt16 y2 = bo->getPosY()*1.0;
        UInt16 adx = x1>x2?(x1-x2):(x2-x1)*1.0;
        UInt16 ady = y1>y2?(y1-y2):(y2-y1)*1.0;
        //if(ady >= 34 + (bf->GetRad()+bo->GetRad()))
        //    return -1;
        if(adx <= (bf->GetRad()+bo->GetRad()))
            return 0;
        return adx - (bf->GetRad()+bo->GetRad());

        if(static_cast<UInt16>(sqrt((adx*adx + ady*ady))) <= (bf->GetRad()+bo->GetRad()))
            return 0;
        return  (static_cast<UInt16>(sqrt(adx*adx + ady*ady)) - bf->GetRad() - bo->GetRad());
    } 

    UInt16 BattleField::getDistance(BattleObject * bf , UInt16 x2 , UInt16 y2 ,UInt16 rad)
    { 
        if( !bf )
            return -1;
        UInt16 x1 = bf->getPosX()*1.0;
        UInt16 y1 = bf->getPosY()*1.0;
        UInt16 adx = x1>x2?(x1-x2):(x2-x1)*1.0;
        UInt16 ady = y1>y2?(y1-y2):(y2-y1)*1.0;

        if(ady >= 34)// + bf->GetRad()+rad)   改
            return -1;
        if(adx <= bf->GetRad()+ rad)
        {
            //COUT << "adx:" << static_cast<UInt32>(adx) << " Rad1:" << static_cast<UInt32>(bf->GetRad()) << " Rad2" << static_cast<UInt32>(rad) <<std::endl;
            return 0;
        }
        return adx - bf->GetRad()- rad;


        if(static_cast<UInt16>(sqrt((adx*adx + ady*ady))) <= (bf->GetRad()+rad))
            return 0;
        return  (static_cast<UInt16>(sqrt(adx*adx + ady*ady)) - bf->GetRad() - rad);
    } 

    bool BattleField::setObjectXY(UInt16 x, UInt16 y, BattleObject * bo, bool flag, UInt16 Body, UInt16 write)
    { 
        for(UInt8 j = 0; j < 2; ++j)
        {
            for(UInt8 i = 0; i < _fighters[j].size() ; ++i)
            { 
                if(bo == _fighters[j][i])
                    continue ;
                if(getDistance(_fighters[j][i],x,y,bo->GetRad()) == 0)
                {
                    //COUT << "位置冲突: " << static_cast<UInt32>(x) << " , " << static_cast<UInt32>(y) << " 和 " << static_cast<UInt32>(_fighters[j][i]->getPosX()) << " , " << static_cast<UInt32>(_fighters[j][i]->getPosY()) <<  std::endl;
                    return false;  
                }
            } 
        }
        bo->SetMinXY(x,y);
        bo->setPos(x,y);
        return true;
    } 

    UInt16 BattleField::getShootDistance(BattleObject * begin , BattleObject * end , BattleObject* target)
    { 
        int x1 =static_cast<int>(begin->getPosX());
        int y1 =static_cast<int>(begin->getPosY());
        int x2 =static_cast<int>(end->getPosX());
        int y2 =static_cast<int>(end->getPosY());
        int x3 =static_cast<int>(target->getPosX());
        int y3 =static_cast<int>(target->getPosY());
        return static_cast<UInt16>(abs((y1-y2)*x3+(x2-x1)*y3-y1*x2+x1*y2)/sqrt(((y1-y2)*(y1-y2) + (x1-x2)*(x1-x2))));
    } 

    void BattleField::GetBSEnterInfo(Stream& st)
    { 
        st << static_cast<UInt8>(_mapId1);
        st << static_cast<UInt8>(_mapId2);
        for(UInt8 i= 0; i < 2; ++i)
        { 
            st << static_cast<UInt16>(_fighters[i][0]->GetId());
            st << static_cast<UInt8>(_fighters[i].size());
            for(UInt8 j = 0; j < _fighters[i].size(); ++ j)
            { 
                st << static_cast<UInt8>( _fighters[i][j]->GetBSNumber()); 
                //st << static_cast<UInt16>(_fighters[i][j]->GetId());
                st << static_cast<UInt16>(_fighters[i][j]->getPosX());
                st << static_cast<UInt16>(_fighters[i][j]->getPosY());
                st << static_cast<UInt16>(_fighters[i][j]->getHP());
                ////COUT << " 入场战将分部" << std::endl;
                ////COUT << " 战将编号"<<  static_cast<UInt32>(_fighters[i][j]->GetBSNumber());
                ////COUT << " X坐标："<< static_cast<UInt32>(_fighters[i][j]->getPosX());
                ////COUT << " Y坐标：" <<static_cast<UInt32>(_fighters[i][j]->getPosY()) << std::endl;
            } 
        } 
    } 

    void BattleField::GetTargetList(UInt8 side, BattleFighter * bf ,std::vector<BattleObject *>& vec , UInt16 skillId, UInt8 flag)   //flag == 2 ==> 
    { 
        if(!_fighters[side].size())
            return ;

        const GData::SkillEffect * se = GData::skillManager[skillId]->GetSkillEffect();
        if(se && se->skillType == e_image_therapy)
            side = !side;

        const GData::SkillScope* ss =GData::skillManager[skillId]->GetSkillScope();
        if(!ss)
            return ;

        switch(ss->area)
        {
            case 1:  //随机N个敌人 ss->x 为数量 
                { 
                    //XXX
                    std::vector<UInt8> vec1 = getAliveCount(side);
                    if(!ss->x)
                        return ;
                    UInt8 div = vec1.size()/ss->x; 
                    UInt8 rand = uRand(div);
                    for(UInt8 i = 0; i < ss->x ; ++i)
                    { 
                        if(static_cast<UInt32>(i*div +rand) >= _fighters[side].size())
                            continue ;
                        if(!_fighters[side][i] || !_fighters[side][i]->getHP())
                            continue ;
                        vec.push_back(_fighters[side][i]);
                    } 
                    break;
                } 
            case 2: //所有敌人
                { 
                    for(UInt8 i = 0; i < _fighters[side].size(); ++i)
                    { 
                        if(!_fighters[side][i] || !_fighters[side][i]->getHP())
                            continue ;
                        vec.push_back(_fighters[side][i]);
                    } 
                    break;
                } 
            case 3:  //目标扩散型 先找到目标，然后以x为半径攻击
                { 
                    BattleObject * bo = GetTarget(side , bf->getPosX() , bf->getPosY());
                    if(!bo)
                        return ;
                    for(UInt8 i = 0; i < _fighters[side].size(); ++i)
                    { 
                        BattleObject* fgt1 = _fighters[side][i] ;
                        if(!fgt1 || !fgt1->getHP())
                            continue;
                        if(getDistance(bo,fgt1) <= ss->x)
                            vec.push_back(fgt1);
                    } 
                    break;
                } 
            case 4:  
                { 
                    BattleObject * bo = GetTarget(side , bf->getPosX() , bf->getPosY());
                    if(!bo)
                        return ;
                    for(UInt8 i = 0; i < _fighters[side].size(); ++i)
                    { 
                        BattleObject* fgt1 = _fighters[side][i] ;
                        if(!fgt1 || !fgt1->getHP())
                            continue;
                        if(getDistance(bo,fgt1) <= ss->x)
                            vec.push_back(fgt1);
                    } 
                    break;
                } 
            case 5:  //y轴攻击型  //半月斩
                { 
                    UInt16 myY = bf->getPosY();
                    UInt8  minNumber = ss->radx / 2 ;  //radx 表示数量(一般情况为奇数) rady表示间隔 x,y作为上下闭合区间
                    UInt16 width = ss->x + ss->y;
                    UInt16 minY  =  0;
                    if(myY > ((width * ss->rady + width)*minNumber + ss->y))
                        minY = myY - ((width * ss->rady + width)*minNumber + ss->y);

                    for(UInt8 i = 0; i < _fighters[side].size(); ++i)
                    { 
                        BattleObject* fgt1 = _fighters[side][i] ;
                        if(!fgt1 || !fgt1->getHP())
                            continue;
                        if(flag == 2 && bf->getPosX() >= fgt1->getPosX())  
                            continue;
                        if(flag == 1 && bf->getPosX() <= fgt1->getPosX())  
                            continue;
                        UInt16 disy = fgt1->getPosY();  //目标y坐标
                        for(UInt8 i = 0; i < ss->radx; ++i)
                        { 
                            if(minY + (ss->rady+1)*i*width < disy && disy < minY + (ss->rady + 1)*i*width +width)
                            {
                                vec.push_back(fgt1);
                                break;  
                            }
                        } 
                    } 
                    break;
                } 
            case 6:    //弓箭手的攻击范围
                {
                    
                    BattleObject * bo = GetTarget(side , bf->getPosX() , bf->getPosY());
                    if(!bo)
                        return ;
                    vec.push_back(bo) ;
                    for(UInt8 i = 0; i < _fighters[side].size(); ++i)
                    { 
                        BattleObject* fgt1 = _fighters[side][i] ;
                        if(bo == fgt1)
                            continue;
                        if(!fgt1 || !fgt1->getHP())
                            continue;
                        if(getShootDistance(bf,bo,fgt1) < fgt1->GetRad())
                            vec.push_back(fgt1);
                    } 
                    break;
                }
        }
    } 

    std::vector<UInt8> BattleField::getAliveCount(UInt8 side)
    { 
        std::vector<UInt8> vec;
        for(UInt8 i = 0; i < _fighters[side].size(); ++i)
        { 
            if(!_fighters[side][i] || _fighters[side][i]->getHP())
                continue ;
            vec.push_back(i);
        } 
        return vec;
    } 

    BattleFighter * BattleField::getMyFighters(UInt8 side, UInt8 index)
    { 
        UInt8 count = 0;
        for(UInt8 i = 0; i < _fighters[side].size(); ++i)
        {
            if(count++ < index) 
                continue;
            if(_fighters[side][i] && _fighters[side][i]->getHP() == 0)
                continue ;
            return static_cast<BattleFighter*>(_fighters[side][i]);
        }
        return NULL;
    } 

    void BattleField::InsertTimeBattleAction(UInt16 time , ActionPackage ba)
    { 
        FieldAttack[time].push_back(ba);
    } 

    std::vector<ActionPackage> BattleField::GetTimeBattleAction(UInt16& time)
    { 
        std::vector<ActionPackage> vec;
        if(FieldAttack.begin() != FieldAttack.end())
        {
            time = FieldAttack.begin()->first;
            vec = FieldAttack.begin()->second;
        }
        return vec;
    } 

    void BattleField::InsertTimeBattleAction(UInt16 time , ImagePackage ip)
    { 
        FieldImage[time].push_back(ip);
    } 
    std::vector<ImagePackage> BattleField::GetTimeBattleImage(UInt16& time)
    { 
        std::vector<ImagePackage> vec;
        if(FieldImage.begin() != FieldImage.end())
        {
            time = FieldImage.begin()->first;
            vec = FieldImage.begin()->second;
        }
        return vec;
    } 

    //BATTLE@
    void BattleField::InsertObjectPackage(ObjectPackage ba)
    { 
        FieldObject.push_back(ba);
    } 
    std::list<ObjectPackage>& BattleField::GetObjectpackage()
    { 
        return FieldObject;
    } 

    UInt16 BattleField::GetMinTime()
    { 
        UInt16 min1 = 10000;
        UInt16 min2 = 10000;
        UInt16 min3 = 10000;
        if(FieldAttack.begin() != FieldAttack.end())
            min1 = FieldAttack.begin()->first;
        if(FieldImage.begin() != FieldImage.end())
            min2 = FieldImage.begin()->first;
        if(BattlePre.begin() != BattlePre.end())
            min3 = BattlePre.begin()->first;
        return Min(Min(min1,min2),min3);
    } 
    void BattleField::DelBattleAction()
    { 
        if(FieldAttack.begin() == FieldAttack.end())
            return ;
        FieldAttack.erase(FieldAttack.begin());

    } 
    void BattleField::DelBattleImage()
    { 
        if(FieldImage.begin() == FieldImage.end())
            return ;
        FieldImage.erase(FieldImage.begin());
    } 

    std::vector<BattleFighter* > BattleField::GetBattlePre(UInt16& time)
    { 
        std::vector<BattleFighter*> vec;
        if(BattlePre.begin() != BattlePre.end())
        {
            time = BattlePre.begin()->first;
            vec = BattlePre.begin()->second;
        }
        return vec;
    } 

    void BattleField::DelBattlePre()
    { 
       if(BattlePre.begin() == BattlePre.end()) 
           return ;
       BattlePre.erase(BattlePre.begin());
    } 

    void BattleField::InsertBattlePre(UInt16 time, BattleFighter* fgt)
    { 
        //if(BattlePre.begin() != BattlePre.end() && time < BattlePre.begin()->first)
        //    return ;

        //COUT << "-----战将编号：" << static_cast<UInt32>(fgt->GetBSNumber()) << " 准备于时间点 " <<  static_cast<UInt32>(time+4) << std::endl;
        //无论如何最多放在下一个时间片段
        BattlePre[time + 4].push_back(fgt);
    } 

    void BattleField::FieldPrint()
    { 
        for(UInt8 i = 0; i < 2; ++i)
        { 
            //COUT << "Side" << static_cast<UInt32>(i) << std::endl;
            for(UInt8 j = 0; j < _fighters[i].size(); ++j)
            {
                //COUT << "战将编号:" << static_cast<UInt32>(_fighters[i][j]->GetBSNumber()) << "血量：" << static_cast<UInt32>(_fighters[i][j]->getHP()) << std::endl;
            }
        } 
    } 

    void BattleField::BattleActionPrintf(UInt8 index)
    { 
        switch(index)    
        { 
            case 0:
                { 
                    //COUT << "准备攻击者 ";
                    if(FieldAttack.begin() == FieldAttack.end())
                        return ;
                    std::vector<ActionPackage> vec = FieldAttack.begin()->second;
                    //COUT << "time：" << FieldAttack.begin()->first << std::endl;
                    for(UInt8 i = 0; i < vec.size(); ++i)
                    { 
                        //COUT << "战将编号" << static_cast<UInt32>(vec[i].GetBattleFighter()->GetBSNumber()) << std::endl;
                    } 
                } 
                break;
            case 1:
                { 
                    //COUT << "魔法行动列表:";
                    if(FieldImage.begin() == FieldImage.end())
                        return ;
                    std::vector<ImagePackage> vec = FieldImage.begin()->second;
                    //COUT << "time：" << FieldImage.begin()->first << std::endl;
                    for(UInt8 i = 0; i < vec.size(); ++i)
                    { 
                        //COUT << " 战将编号: " << static_cast<UInt32>(vec[i].GetBattleFighter()->GetBSNumber()) << std::endl;
                    } 
                } 
                break;
        } 
    } 
}
