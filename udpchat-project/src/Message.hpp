#pragma once  

#include <iostream>
#include <string>
#include "json/json.h"

class Message
{
    public:
        //反序列化
        //这个是在反序列化客户端发给我们的json数据串
        void Deserialize(std::string Messagee)
        {
            Json::Reader reader;
            Json::Value val;
            reader.parse(Messagee,val,false);
            
            NickName_  = val["NickName_"].asString();
            School_ = val["School_"].asString();
            Msg_ = val["Msg_"].asString();
            UserId_ = val["UserId_"].asInt();
        }
       
        //序列化
        void  serialize(std::string* msg)
        {
            Json::Value val;
            val["NickName_"] = NickName_;
            val["School_"] = School_;
            val["Msg_"]=Msg_;
            val["UserId_"] = UserId_; 

            Json::FastWriter writer;
            *msg = writer.write(val);
        }

        
        void SetNickName(std::string& NickName)
        {
            NickName_ = NickName;
        }

        void SetSchool(std::string& School)
        {
            School_ = School;
        }
        
        void SetMsg(std::string& Msg)
        {
            Msg_ = Msg;
        }
        
        void SetUserId(uint32_t& UserId)
        {
            UserId_ = UserId;
        }

        uint32_t& GetUserId()
        {
            return UserId_;
        }

        std::string& GetNickName()
        {
            return NickName_;
        }
        
        std::string& GetSchool()
        {
            return School_;
        }

        std::string& GetMsg()
        {
            return Msg_;
        }
    private:
        std::string NickName_;
        std::string School_;
        std::string Msg_;
        uint32_t UserId_;
};
