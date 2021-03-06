#include"ClanBattleCityStatus.h"
#include"Battle/ClanBattleRoom.h"
#include"Battle/ClanBattleDistribute.h"
#include "GObject/GVar.h"
#include "Common/TimeUtil.h"


namespace Battle
{
   RoomAllCityStatusManager roomAllCityStatusManager;

    void RoomAllCityStatus::SetStage(UInt32 t)
    {
        //报名之后的第二天10点为战术推演阶段 
        //由报名时间得到
        if( GObject::GVAR.GetVar(3/*GVAR_BATTLE_STATUS*/) < 3 )
        {
            stage = GObject::GVAR.GetVar(3/*GVAR_BATTLE_STATUS*/);
            return;
        }
        if( IsStop() )
        {
            stage = 2;
            return;
        }
        ClanBattleRoom* room = Battle::clanBattleRoomManager.GetBattleRoom(roomId);
        if( room == NULL )
        {
            return;
        }
        UInt32 buildTime = room->GetBuildTime();


        time_t time = t;
        tm* tt=localtime(&time);
        UInt8 hour = tt->tm_hour;
        //UInt8 min = tt->tm_min;


        UInt32 endTime = TimeUtil::SharpDay(7,buildTime);
        UInt8 newStage = 0;
        if( t >= endTime )
        {
            newStage = 2;
        }
        else
        {
            //处于5-8点是战役演算阶段
            if( hour >= 5 && hour < 8 )
            {
                newStage = 1;
            }
            else
            {
                newStage = 0;
            }
        }

        /*
        UInt32 sday =buildTime+((24-hour)*60-min)*60+36000;   //第二天十点的时间的戳
        //UInt32 sday =buildTime+((24-hour)*60-min)*60+3600*10+1200*2;   //第二天11点的时间的戳
        UInt8 newStage = 0;
        if( t > buildTime && t <= sday )
        {
            newStage = 0;
        }
        else if ( t > sday && t <= sday + 1*86400)
        {
            newStage = 1;
        }
        else if(  t > sday+1*86400 && t <= sday+2*86400)
        {
            newStage = 0;
        }
        else if(  t > sday+2*86400 && t <= sday+3*86400 )
        {
            newStage = 1;
        }
        else if( t >  sday+3*86400 && t <= sday+4*86400 )
        {
            newStage = 0;
        }
        else if(  t > sday+4*86400 && t <= sday+5*86400 )
        {
            newStage = 1;
        }
        else if(  t > sday+5*86400 && t <= sday+6*86400 )
        {
            newStage = 2;
        }
        else
        {
            newStage = 0;
        }
        */
        if( newStage != stage )
        {
            stage = newStage;
        }

    }

   void RoomAllCityStatus::SetCityOwnForce(UInt8 cityId,UInt8 force)
   {
       for( auto it = vecCityStatus.begin(); it != vecCityStatus.end(); ++it )
       {
           if( (*it)->GetCityId() == cityId )
           {
               (*it)->SetOwnForce(force);
               break;
           }
       }
   }

   //判断是不是到结束了
   bool RoomAllCityStatus::IsStop()
   {
      std::set<UInt8> CaptureForceSet;
      for( auto it = vecCityStatus.begin(); it != vecCityStatus.end(); ++it )
      {
          UInt8 ownForce = (*it)->GetOwnForce();
          //并且该地图有
          if( (*it)->GetOwnForce() != 0 )
          {
              CaptureForceSet.insert(ownForce);
          }
      }
      if( CaptureForceSet.size() == 1 )
      {
          return true;
      }
      else
      {
          return false;
      }

   }



   void RoomAllCityStatus::Init()
   {
       //根据战役Id 设置每一座城的拥有者
       GData::BattleMapInfo* info = GData::battleMapTable.GetBattleMapInfo(battleId);
       std::vector<GData::SingleMapInfo*> vecSingleMapInfo = info->GetSingleMapInfos();
       for( auto it = vecSingleMapInfo.begin(); it != vecSingleMapInfo.end(); ++it )
       {
           vecCityStatus.push_back(new CityStatus((*it)->GetMapId(),(*it)->GetForce()));
           /*
           //如果拥有者的势力不是0 则写入数据库
           if( (*it)->GetForce() != 0 )
           {
               DB7().PushUpdateData("INSERT INTO  `clan_battle_citystatus`(`roomId`,`battleId`,`cityId`,`ownforce`) VALUES(%u,%u,%u,%u)",roomId,battleId,(*it)->GetMapId(),(*it)->GetForce());
           }
           */
       }
   }


   std::set<UInt8> RoomAllCityStatus::GetCanConfigure(UInt8 forceId)
   {
       std::vector<UInt8> vecCitys;
       for( auto it = vecCityStatus.begin(); it != vecCityStatus.end(); ++it)
       {
           UInt8 cityId = (*it)->GetCityId();
           UInt8 ownforce = (*it)->GetOwnForce();
           Battle::MapDistributeInfo* info = Battle::battleDistribute.GetMapDistributionInfo(roomId,cityId);
           if( (ownforce == 0 && info != NULL && info->HasBody()) || (!ownforce  && forceId == ownforce))
           {
               vecCitys.push_back(cityId);
           }
       }

       std::set<UInt8> ConfigureSet;
       GData::BattleMapInfo* info = GData::battleMapTable.GetBattleMapInfo(battleId);
       for( auto it = vecCitys.begin(); it != vecCitys.end(); ++it )
       {
            GData::SingleMapInfo* singleInfo = info->GetSingleMapInfo((*it));
            std::vector<UInt8> linkCitys = singleInfo->GetLinks();
            for( auto iter = linkCitys.begin(); iter != linkCitys.end(); ++iter)
            {
                ConfigureSet.insert((*iter));
            }
            ConfigureSet.insert(*it);
       }
       return ConfigureSet;
   }


   UInt8 RoomAllCityStatus::GetCityOwnForce(UInt8 cityId)
   {
       for( auto it = vecCityStatus.begin(); it != vecCityStatus.end(); ++it )
       {
           if( (*it)->GetCityId() == cityId )
           {
               return (*it)->GetOwnForce();
           }
       }
       return 0;
   }
   
   CityStatus* RoomAllCityStatus::GetCityStatus(UInt8 cityId)
   {
       for( auto it = vecCityStatus.begin(); it != vecCityStatus.end(); ++it )
       {
           if( (*it)->GetCityId() == cityId )
           {
               return (*it);
           }
       }
       return NULL;
   }


   std::vector<UInt8> RoomAllCityStatus::GetCaptureCitys(UInt8 forceId)
   {
       std::vector<UInt8> vecCitys;
       for( auto it = vecCityStatus.begin(); it != vecCityStatus.end(); ++it )
       {
           if( (*it)->GetOwnForce() == forceId )
           {
               vecCitys.push_back((*it)->GetCityId());
           }
       }
       return vecCitys;
   }

   UInt8 RoomAllCityStatus::GetCaptureCityNum()
   {
       UInt8 totalNum = 0;
       for( auto it = vecCityStatus.begin(); it != vecCityStatus.end(); ++it )
       {
           if( (*it)->GetOwnForce() != 0 )
           {
               ++totalNum;
           }
       }
       return totalNum;
   }

   void RoomAllCityStatusManager::InsertRoomAllCityStatus(UInt32 roomId,UInt8 battleId)
   {
       RoomAllCityStatus* roomAllStatus = GetRoomAllCityStatus(roomId);
       if( roomAllStatus != NULL )
           return;
       roomAllStatus = new(std::nothrow) RoomAllCityStatus(roomId,battleId);
       if( roomAllStatus == NULL )
           return;
       roomAllStatus->Init();
       _vecRoomAllCityStatus.push_back(roomAllStatus);
       //
   }


   RoomAllCityStatus* RoomAllCityStatusManager::GetRoomAllCityStatus(UInt32 roomId)
   {
       for( auto it = _vecRoomAllCityStatus.begin(); it != _vecRoomAllCityStatus.end(); ++it )
       {
           if( (*it)->GetRoomId() == roomId )
           {
               return (*it);
           }
       }
       return NULL;
   }


   void RoomAllCityStatusManager::loadCityStatus(UInt32 roomId,UInt8 battleId,UInt8 cityId,UInt8 ownforce)
   {
       RoomAllCityStatus* status = GetRoomAllCityStatus(roomId);
       if( status == NULL )
       {
            status = new RoomAllCityStatus(roomId,battleId);
            status->InsertCityStatus(cityId,ownforce);
            _vecRoomAllCityStatus.push_back(status);
       }
       else
       {
           CityStatus* cityStatus = status->GetCityStatus(cityId);
           if( cityStatus != NULL )
           {
               cityStatus->SetOwnForce(ownforce);
           }
           else
           {
               status->InsertCityStatus(cityId,ownforce);
           }
       }


   }

   void RoomAllCityStatusManager::SetOwnForce(UInt32 roomId,UInt8 cityId,UInt8 ownForce)
   {
       RoomAllCityStatus* status = GetRoomAllCityStatus(roomId);
       if( status == NULL )
           return;
       status->SetCityOwnForce(cityId,ownForce);
   }


   void RoomAllCityStatusManager::RemoveRoomAllCityStatus(UInt32 roomId)
   {
       for( auto it =  _vecRoomAllCityStatus.begin(); it != _vecRoomAllCityStatus.end();)
       {
           if( (*it)->GetRoomId() == roomId )
           {
               RoomAllCityStatus* p = (*it);
               it = _vecRoomAllCityStatus.erase(it);
               delete p;
           }
           else
           {
               ++it;
           }
       }
   }

}
