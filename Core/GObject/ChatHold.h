
#ifndef CHATHOLD_H_
#define CHATHOLD_H_
#include "Config.h"
#include "Common/Stream.h"
#include "Common/TimeUtil.h"
namespace GObject
{
    class Player;

    struct ChatStruct
    {
        Player* pl;
        std::string context;
        UInt32 time;
        ChatStruct(Player* p , std::string text):pl(p),context(text){
            time = TimeUtil::Now();
        }
    };
    class ChatHold
    {
        public:
            ChatHold(UInt8 type,UInt8 max = 0):_type(type),_countMax(max){ _lst.clear();}
            void InsertChat(Player * pl, std::string text);
            void GetStream(Stream& st,UInt8 count = 1);
        private:
            std::list<ChatStruct> _lst;
            UInt8 _type;
            UInt8 _countMax;
    };
}
#endif // CHATHOLD_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

