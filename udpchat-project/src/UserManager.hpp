#pragma once 

#include<stdio.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<string>
#include<cstring>
#include<unordered_map>
#include<vector>
#include<iostream>
#include"LogSvr.hpp"

#define OFFLINE    0
#define REGISTERED 1
#define USERLOGINED    2
#define ONLINE     3

//单个用户
class UserInfo
{
    public:
        //注册和登录阶段使用的是TCP,所以不能保存TCP地址信息，而是要等待当登录上来的这个用户第一次使用UDP协议发送消息的时候，将UDP的地址信息保存下来，
        //保存下来，在群发时，就可以找到有效的UDP的地址信息
        UserInfo(const std::string& NickName, const std::string& School,   uint32_t UserId, const std::string& Passwd)
        {
           NickName_ = NickName;
           School_ =School;
           UserId_ = UserId;
           Passwd_ = Passwd;
           memset(&CliAddr_, '0', sizeof(struct sockaddr_in));
           CliAddrLen_ = -1;
           UserStatus_ =  OFFLINE ; 
        }

        void SetUserStatus(int Status)
        {
            UserStatus_ = Status;
        }

        std::string& GetPasswd()
        {
            return Passwd_;
        }

        int& GetUserStatus()
        {
            return UserStatus_;
        }
        
        void SetCliAddrInfo(const struct sockaddr_in& CliAddr)
        {
            memcpy(&CliAddr_, &CliAddr,  sizeof(CliAddr));
        }
        
        void SetCliAddrLen(const socklen_t& CliAddrLen)
        {
            CliAddrLen_ = CliAddrLen;
        }

        struct sockaddr_in& GetCliAddrInfo()
        {
            return CliAddr_;
        }
        
        socklen_t& GetCliAddrLen()
        {
            return CliAddrLen_;
        }
    private:
        std::string NickName_;
        std::string School_;
        //用户ID
        uint32_t UserId_;
        std::string Passwd_;
        //保存udp客户端的地址信息
        struct sockaddr_in  CliAddr_;
        socklen_t CliAddrLen_;

        //保存状态  状态机
        int UserStatus_;
};

class UserManager
{
    public:
        UserManager()
        {
            UserMap_.clear();
            OnlineUserVec_.clear();
            pthread_mutex_init(&Lock_,NULL);

            PrepareUserId_ =0;
        }
        ~UserManager()
        {
            pthread_mutex_destroy(&Lock_);
        }

        //当一个用户需要注册时， NickName , School , Passwd  ===》UserId
        int Register(const std::string& NickName, const std::string& School, const std::string& Passwd,uint32_t* UserId)
        {
            if(NickName.size() == 0 || School.size() == 0 || Passwd.size() == 0) 
            {
                return -1;
            }
            
            pthread_mutex_lock(&Lock_);
            UserInfo  userinfo(NickName, School, PrepareUserId_, Passwd);
            //更改当前用户的状态,改为已注册状态
            userinfo.SetUserStatus(REGISTERED);
            //插入到Map当中去
            UserMap_.insert(std::make_pair(PrepareUserId_, userinfo));
            *UserId = PrepareUserId_;
            PrepareUserId_++;
            pthread_mutex_unlock(&Lock_);
            return 0;
        }

        //使用用户ID还有密码进行登录
        int Login(const uint32_t& UserId,const std::string& Passwd)
        {
            if(Passwd.size() < 0)
            {
                return -1;
            }

            int LoginStatus = -1;


            //返回登录状态
            //1.先在Map 中查找是否存在这样的ID
            //不存在
            //存在
            //  密码正确
            //  密码错误
            
            std::unordered_map<uint32_t,UserInfo>::iterator iter;
            pthread_mutex_lock(&Lock_);
            //加锁防止迭代器失效
            iter = UserMap_.find(UserId);
            if(iter != UserMap_.end())
            { 
                if(Passwd == iter->second.GetPasswd())
                {
                    //密码正确
                    iter->second.GetUserStatus() = USERLOGINED;
                    LoginStatus = 0;
                }
                else
                {
                    //密码不正确
                    LOG(ERROR,"User Passwd is not  true ,Passwd is: ")<<Passwd<<std::endl;
                    LoginStatus = -1;
                }
            }
            else 
            {
                //没找到
                LOG(ERROR,"UserId not found , UserId is")<< UserId<<std::endl;
                LoginStatus = -1;
            }
            pthread_mutex_unlock(&Lock_);
            return LoginStatus;
        }

        int LoginOut();

        bool IsLogin(uint32_t UserId,const struct sockaddr_in& CliAddr, const socklen_t& CliAddrLen)
        {
            if(sizeof(CliAddr)<0 || CliAddrLen < 0)
            {
                return false;
            }

            //1.校验当前用户是否存在
            std::unordered_map<uint32_t,UserInfo>::iterator iter;
            pthread_mutex_lock(&Lock_);
            iter = UserMap_.find(UserId);
            if(iter == UserMap_.end())
            {
                pthread_mutex_unlock(&Lock_);
                LOG(ERROR,"User not exist")<<std::endl;
                return false;
            }
            
            //2.判断当前用户状态，来判断是否完成登录
            if(iter->second.GetUserStatus() == OFFLINE || iter->second.GetUserStatus() == REGISTERED)
            {
                pthread_mutex_unlock(&Lock_);
                LOG(ERROR,"User status error")<<std::endl;
                return false;
            }
            //3、判断当前用户是否是第一次发生消息
            if(iter->second.GetUserStatus() == ONLINE)
            {
                pthread_mutex_unlock(&Lock_);
                return true;
            }
            //第一次发送消息
            if(iter->second.GetUserStatus() == USERLOGINED)
            {
                //增加地址信息，地址信息长度，改变状态为ONLINE
                //printf("%s:%d\n",inet_ntoa(CliAddr.sin_addr),ntohs(CliAddr.sin_port));
                iter->second.SetCliAddrInfo(CliAddr);
                iter->second.SetCliAddrLen(CliAddrLen);
                iter->second.SetUserStatus(ONLINE);
 
                //保存进Vector
                OnlineUserVec_.push_back(iter->second);
            }
            pthread_mutex_unlock(&Lock_);
            return true;
        }
        void GetOnlineUserInfo(std::vector<UserInfo>* vec)
        {
            *vec = OnlineUserVec_;
        }
        
    private:
        //map  unordered_map
        //在这里使用 unordered_map 查找效率高
        // UserMap_ 保存所以注册用户的信息 ---TCP
        std::unordered_map<uint32_t, UserInfo> UserMap_;
        //保存所以在线用户的信息 ---Udp  判断在线的标准，是否使用udp协议发送消息 
        pthread_mutex_t Lock_;//加锁，保护线程不安全

        std::vector<UserInfo> OnlineUserVec_;
        
        //预分配的用户ID
        uint32_t PrepareUserId_;
};




