#ifndef PLAYER_H_
#define PLAYER_H_
#include "GObjectManager.h"
#include "Common/Stream.h"
#include "Common/TimeUtil.h"
#include "Common/MCached.h"
#include "Var.h"
#include "GVar.h" 
#include "Script/lua_tinker.h"
#include "kingnet_analyzer.h"
#include "Friend.h"
#include "Package.h"
#include "Server/WorldServer.h"
//#include "Player.h"
#include "Battle/BattleGround.h"
#include "ChatHold.h"
#include "Clan.h"
#include "Govern.h"
#include "Exploit.h"
#include "StoreA.h"
#include "Task.h"

//VAR
//Package
//Friend
//SelfInfo
//FriendManager
//Session
//thread
//Online
//DCLoger
//serverNo
//log 
//Fighter
//BattleSimulator
//Function
//Chat

#define IDTYPE UInt64   //LIBO
#define MAIL_LIST_MAX 10
#define ARENA_BATTLE_MAX 5
namespace Battle
{
    class BattleSimulator;
    class ClanBattleRoom;
}
namespace Script
{
    class GameActionLua;
}

namespace GObject
{
    enum 
    {
        e_chat_world,
        e_chat_friend,
        e_chat_clan
    };
    class Fighter;
    struct lineup
    {
        lineup(): fid(0), pos(0) {} //, fighter(NULL) {}
        inline bool available() { return fid != 0; } 
        void updateId();
        UInt32 fid; 
        UInt8 pos; 
        Fighter * fighter;
    };

    struct PlayerData
    {
        UInt16 location;
        UInt32 packSize;  //背包空间
        lineup line[6];
    };

    struct ArenaBattleInfo
    {
        UInt32 battleId;
        UInt8  res;
        std::string name;
        UInt16 index;
        UInt32 power;
        ArenaBattleInfo()
        { 
            battleId = 0;
            res = 0;
            index = 0;
            power = 0;
        } 
        ArenaBattleInfo(UInt32 bId, UInt8 res, std::string n, UInt16 idx, UInt32 p)
        { 
            battleId = bId;
            name = n;
            index = idx;
            power = p;
        } 
    };

    struct ClanBattleFighter
    {
        ClanBattleFighter(UInt8 m,UInt16 f,UInt8 px,UInt8 py) : mapId(m),fighterId(f),posx(px),posy(py) {}
        UInt16 GetFighterId() const { return fighterId;}
        UInt8  GetMapId() const { return mapId;}
        UInt8  GetPosX()  const { return posx;}
        UInt8  GetPosY()  const { return posy;}
        UInt8 mapId;
        UInt16 fighterId;
        UInt8 posx;
        UInt8 posy;
    };

    struct ConstantlyKill   //连杀结构体
    {
        ConstantlyKill(UInt16 id,UInt32 num ) : fighterId(id), killNum(num){}
        UInt16 fighterId;
        UInt32 killNum;   //连杀个数
    };

    struct EndConstantlyKill //终结连杀结构体
    {
        EndConstantlyKill(UInt16 sId, GObject::Player* pl,UInt16 id,UInt32 num) :selfFighterId(sId), player(pl),fighterId(id),endkillNum(num) {}
        UInt16 selfFighterId; //己方的战将
        Player* player;      //终结的那个玩家
        UInt16 fighterId;    //哪个将
        UInt32 endkillNum;   //终结了对方的几连杀
    };


    struct KillInfo
    {
        KillInfo(UInt16 id,UInt8 cId,UInt16 fnum,UInt16 snum) : fighterId(id),cityId(cId),killFighterNum(fnum),killSoldierNum(snum){}
        UInt16 fighterId;
        UInt8 cityId;
        UInt16 killFighterNum;
        UInt16 killSoldierNum;
    };
    

    //自己的哪个战将被敌方战将击败
    struct LoseInfo
    {
        LoseInfo(UInt16 id,Player* pl,UInt16 pId) : fighterId(id),peer(pl),peerId(pId){}
        UInt16 fighterId;//自己的战将
        Player* peer;   //敌方玩家
        UInt16 peerId;  //敌方战将
    };

    class VarSystem;
    class FriendManager ;
    class Fighter;
    class ChatHold;
    class Clan;
    class GovernManager; //治理
    class ExploitManager; //开采
    class StoreA;  //个人商城
    class TaskManager;  //任务系统

    class Player :
        public GObjectBaseT<Player,IDTYPE>
    {
        public:
            Player(IDTYPE id);
            ~Player();
            //VAR
            UInt32 GetVar(UInt32 id);
            Int32 GetVarS(Int32 id);
            void LoadVar(UInt32 id, UInt32 val, UInt32 overTime);
            void SetVar(UInt32 id, UInt32 val,UInt8 flag = 0);
            void DelVar(UInt32 id);
            void AddVar(UInt32 id, UInt32 val);
            void AddVarS(UInt32 id, Int32 val);
            UInt32 GetVarNow(UInt32 id, UInt32 now);
            void SetVarNow(UInt32 id,  UInt32 val, UInt32 now);
            void AddVarNow(UInt32 id , UInt32 val, UInt32 now);
            void SetVarOffset(UInt32 offset);

            //Package
            Package * GetPackage() { return m_Package; } 

            //Friend
            bool _hasFriend( UInt8 type, Player * pl ) const;
            Player * _findFriend( UInt8 type, std::string& name );
            inline bool _hasFriend(Player *pl) const { return _hasFriend(0, pl); }

            //SelfInfo
            void GetSelfInfoStream(Stream &st);

            //FriendManager
            FriendManager * GetFriendManager(){ return m_friendMgr;}

            //Session
            inline void SetSessionID(int session) { _session = session; }
            inline int GetSessionID() const { return _session; }
            void send( const void * buf, int size);
            inline void send(Stream& st)
            {send(&st[0], st.size());}

            //thread
            UInt8 getThreadId(){ return 0;}

            template <typename MsgType>
                inline void send(MsgType& msg)
                { if(_isOnline) NETWORK()->SendMsgToClient(_session, msg);  }

            //Online
            inline bool isOnline() const { return _isOnline; }  
            inline void setOnline(bool o) { _isOnline = o; }


            //DCLoger
            UInt32 getClientAddress();
            inline const char* getDomain() const { return m_domain; } 
            inline const char* getOpenId() const { return m_openid; }
            inline const char* getOpenKey() const { return m_openkey; }
            inline const char* getClientIp() const { return m_clientIp; } 
            void setOpenId(const std::string& openid, bool load = false);
            inline void setOpenKey(const std::string& openkey) { strncpy(m_openkey, openkey.c_str(), 256); m_openkey[256] = '\0';} 
            inline void setClientIp(const std::string& clientIp) 
            {                                           
                if (inet_addr(clientIp.c_str()) == INADDR_NONE)
                {
                    strncpy(m_clientIp, "0.0.0.0", 16);
                }
                else
                {   
                    strncpy(m_clientIp, clientIp.c_str(), 256);
                    m_clientIp[255] = '\0';
                }
            }

            //serverNo
            inline UInt16 GetServerNo() { return 0;}//static_cast<UInt16>(getId() >> 48); }
            inline UInt64 GetId() {return getId();}

            //log 
            void udpLog(const char* str1, const char* str2, const char* str3, const char* str4, 
                    const char* str5, const char* str6, const char* type, UInt32 count = 1);
            void udpLog(UInt8 platform, const char* str1, const char* str2, const char* str3, const char* str4,
                    const char* str5, const char* str6, const char* type, UInt32 count = 1);
            void udpLog(UInt32 type, UInt32 id, UInt32 num, UInt32 price, const char* op); 
            //
            inline const std::string& GetName(){return name; }
            inline void SetName(std::string name1){name = name1; }

            //Fighter
            void addFighter(Fighter * fgt, bool = true, bool = false); 
            void addFighter(UInt16 fgtId, bool = true, bool = false); 

            Fighter * findFighter(UInt32);
            Fighter * getMainFighter();

            //BattleSimulator
            void PutFighters( Battle::BattleGround& bsim, int side, bool fullhp ,UInt16 fighterId);

            //Function
            void Login();
            void makePlayerInfo(Stream& st);



            //login
            //inline void setId(IDTYPE id){ _id = id;}
            inline IDTYPE getId(){ return _id;}

            inline void SetAccounts(std::string accounts) { _accounts = accounts;}
            inline std::string GetAccounts(){return _accounts;}
            inline void SetPassword(std::string password) {_password = password;}

            //Battle
            //设置战斗ID，0表示未在战斗中
            void SetBattleId(UInt32 bid) { _battleId = bid;}
            UInt32 GetBattleId(){ return _battleId;}
            void SetBattleSide(UInt8 side) { _battleSide = side; }
            UInt8 GetBattleSide() { return _battleSide;}

            //Chat
            void SetRecChat(UInt8 type, UInt8 v){ recChat |= (!!v << type);}
            UInt8 GetRecChat(UInt8 type){ return recChat&(1<<type);}


            //业务逻辑
            UInt8 Enchant(UInt8 number);

            //void LoadFriend(Player* friendOne){ _friends.push_back(friendOne);}

            ChatHold* GetChatHold();

            //Mutex& GetMutex() { return mutex;}
            std::string getSource() { return NULL;}
            UInt8 getVipLevel() const { return 1;}
            UInt8 GetSex() const { return 1;}
            UInt8 GetLevel() ;// const { return getMainFighter()->GetLevel();}
            
            Clan* GetClan() const { return clan;}
            void SetClan(Clan* cl) { clan = cl;}
            UInt8 GetClanPos(){ return _clanPos;}
            void SetClanPos(UInt8 clanPos){ _clanPos = clanPos;}
            UInt8 GetFriendMax() const { return _friendMax;}
            void SetFriendMax(UInt8 num) { _friendMax = num;}

            void ChatForWorld(std::string text);
            void ChatForClan(std::string text);
            void ChatForFriend(std::string name, std::string text);

            void AddMail(UInt32 id, UInt8 update = 1);
            UInt8 ReciveMail(UInt32 id,UInt8 flag = 0);
            UInt8 DeleteMail(UInt32 id,UInt8 flag = 0);

            UInt8 DeleteMail();
            UInt8 ReciveMail();

            UInt8 ListMail(Stream& st, UInt16 index = 0);

            GovernManager* GetGovernManager() const { return m_governManager;}
           
            //Clan
            void SendClanListinfo(Stream &st);
            UInt8 CreateClan(std::string name, UInt8 picIndex/*,std::string announcement*/);

            //FindFighter
            void SearchFighter(UInt8 count);
            void VisitFighter(UInt16 fighterId, UInt8 count);

            //addmoney
            void AddGold(UInt32 num);
            UInt8 UseGold(UInt32 num);
            void AddMoney(UInt8 type, UInt32 num);
            UInt8 GetGovernLevel() { return 1;}

            UInt8 GetFreeSearch();

            void GiveBattleAward(UInt8 mapId);

            //军团战相关
            UInt8 SignUpClanBattle();   //报名军团战
            UInt8 GetClanBattleStatue();  //0未报名   1已报名
            void  InsertClanBattleFighter(UInt8 mapId,UInt16 fighterId,UInt8 posx,UInt8 posy); //加载军团战参战战将信息
            void  DelClanBattleFighter(UInt8 mapId,UInt16 fighterId,UInt8 posx,UInt8 posy);  //删除军团战某一参战战将信息
            ClanBattleFighter* GetClanBattleFighter(UInt16 fighterId);
            void SetJoinClanBattle(UInt8 status) { _isClanBattle = status ; }
            UInt8 GetJoinClanBattle() const { return _isClanBattle;}

            UInt8 UpFighter(UInt16 fighterId);

            UInt32 GetKillFighterNum() const { return killFighterNum;}
            void AddKillFighterNum(UInt32 killCount) { killFighterNum += killCount;}
            UInt32 GetKillSoldiersNum() const { return killSoldiersNum;}
            void AddKillSoldiersNum(UInt32 killCount) { killSoldiersNum+=killCount;}

            void AddConstantlyKill(UInt16 fighterId,UInt32 killCount,bool flag = false);   //增加连续击杀的数量
            UInt32 GetConstantlyKill(UInt16 fighterId);

            void AddEndConstantlyKill(UInt16 id, Player* pl,UInt16 fighterId,UInt32 killCount,bool flag = false);
            void GiveEndConstantlyKillAward();
            UInt16 GetMaxEndConstantlyKill();
            UInt16 GetMaxConstantlyKill();
            UInt16 GetTotalKill(); // 杀士兵和战将总数  
            void AddBeKilledFighterNum() { beKilledFighterNum += 1;}
            UInt32 GetBeKilledFighterNum() const { return beKilledFighterNum;}
           
            //某一战将杀敌将
            void AddKillFighter(UInt16 fighterId,UInt8 cityId,UInt16 killCount);
            //某一战将杀敌方士兵
            void AddKillSoldier(UInt16 fighterId,UInt8 cityId,UInt16 killCount);
            UInt16 GetKillFighter(UInt16 fighterId);
            UInt16 GetKillSoldier(UInt16 fighterId);

            //被敌方的哪个玩家的哪个将击败 
            void AddLoseInfo(UInt16 fighterId,Player* peer,UInt16 peerId);
            LoseInfo * GetLoseInfo(UInt16 fighterId);

            void GiveConstantlyKillAward();

            UInt8 SignForMouth(UInt8 opt);

            void GetSignInfo(UInt16 number);

            ExploitManager* GetExploitManager() const { return m_exploitManager;}

            //Arena 竞技场排布
            void LoadArenaDefendLayout(UInt8 index , UInt16 fighterId);
            std::map<UInt8,UInt16> GetArenaLayout();
            void SetArenaLayout(UInt8 index, UInt16 fighterId);
            void GetArenaDefendStream(Stream& st);

            std::map<UInt8,UInt16> GetArenaDefendLayout();
            void SetArenaDefendLayout(UInt8 index, UInt16 fighterId);
            void GetSelfBattleInfo(Stream& st);

            void GetArenaInfo();
            void AttackArenaPos(UInt16 targetPos);
            UInt8 ClearArenaCD();
            UInt8 AddArenaCount();
            bool CanAttackArena();
            void ClearAllBattleFighter() { _vecClanBattleFighter.clear();}
            StoreA* GetStoreA() const { return m_storeA;}
            void FreshStoreItems(UInt8 pageId);
            void BuyItem(UInt8 pageId,UInt8 index,UInt8 num);
            bool UseItem(UInt32 itemId,UInt16 num);
            void ResetClanBattleData();

            //ARENA Battle
            void InsertArenaBattleReport(ArenaBattleInfo abi, bool update = true);
            void GetArenaBattleReport(Stream& st);
            UInt8 GetVipLevel() const { return vipLevel;}
            void SetVipLevel(UInt8 level) { vipLevel = level;}

            TaskManager* GetTaskManager() const { return m_taskManager;}

            void Recharge(UInt32 val);
    private:
            //IDTYPE _id;
            std::string _accounts;
            std::string _password;
            bool _isOnline;  
            AtomicVal<int> _session;
            char m_domain[256+1]; 
            char m_openid[256+1];
            char m_openkey[256+1];
            char m_clientIp[256+1];

            //Clan
            UInt8 _clanPos;  //职位
            UInt8 _isClanBattle; //是否参加军团战


            //功能类
            VarSystem* m_pVars;
            Package* m_Package;
            FriendManager * m_friendMgr;  //好友
            UInt8 _friendMax;    //好友上限
            PlayerData _playerData;
            std::string name;
            ChatHold * chatHold;
            Clan * clan;

            //聊天 Chat
            UInt8 recChat;    //是否接收信息

            //战斗相关
            UInt32 _battleId ; //战斗ID
            UInt8  _battleSide; //战斗所属

            //散仙相关
            std::map<UInt32, Fighter *> _fighters;

            std::list<UInt32> _mailList;

            //治理相关
            GovernManager* m_governManager;

            std::vector<ClanBattleFighter*> _vecClanBattleFighter;

            UInt32 killSoldiersNum;     //军团战击杀敌军人数(小兵)
            UInt32 killFighterNum; //军团战击杀敌将数量(主将)

            UInt32 beKilledFighterNum; //被杀的战将数量

            std::vector<ConstantlyKill> vecConstantlyKill;
            std::vector<EndConstantlyKill> vecEndConstantlyKill;

            ExploitManager* m_exploitManager;

            std::map<UInt8 , UInt16> _ArenaLayout;
            std::map<UInt8 , UInt16> _ArenaDefendLayout;

            std::vector<KillInfo> vecKillInfo;
            std::vector<LoseInfo> vecLoseInfo;

            StoreA* m_storeA;

            //ARENABATTLE
            std::list<ArenaBattleInfo > _arenaBattleReport;

            UInt8 vipLevel;

            TaskManager* m_taskManager;
};

typedef GGlobalObjectManagerT<Player, UInt64> GlobalPlayers;
extern GlobalPlayers globalPlayers;
extern GlobalPlayers globalOnlinePlayers;

typedef GGlobalObjectManagerIStringT<Player> GlobalNamedPlayers;  
extern GlobalNamedPlayers globalNamedPlayers;

typedef GGlobalObjectManagerIStringT<Clan> GlobalNamedClans;  
extern GlobalNamedClans globalNamedClans;

typedef std::vector<Player* > GlobalPlayerVec;  
extern GlobalPlayerVec globalPlayerVec;
//extern GlobalNamedPlayers globalAccountsPlayers;

typedef GGlobalObjectManagerT<Clan, UInt32> GlobalClans;
extern GlobalClans globalClan;

typedef std::set<Player*> GlobalOnlinePlayerSet;
extern GlobalOnlinePlayerSet globalOnlinePlayerSet;
}
#endif // PLAYER_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */
