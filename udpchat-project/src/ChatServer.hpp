#pragma once
#include<iostream>
#include<sys/socket.h>
#include<sys/types.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<pthread.h>
#include<string>

#include"MsgPool.hpp"
#include"LogSvr.hpp"
#include"ConnectInfo.hpp"
#include"UserManager.hpp"
#include"Message.hpp"


#define  UDP_PORT 17777
#define  TCP_PORT 17778
#define  THREAD_COUNT 2



//当前类实现
//1.接收客户端数据
//2.发送数据消息给客户端
//依赖UDP协议实现

class ChatServer
{
    public:
        ChatServer()
        {
            UdpSock_ = -1;
            UdpPort_ = UDP_PORT;
            MsgPool_ = NULL;
             TcpSock_=  -1;
            TcpPort_ = TCP_PORT;

            UserMana_ = NULL;
        }
        ~ChatServer()
        {
            if(MsgPool_ )
            {
                delete MsgPool_;
                MsgPool_ = NULL;
            }

            if(UserMana_)
            {
                delete UserMana_;
                UserMana_ =NULL;
            }
        }

        //上次调用InitServer函数来初始化UDP服务
        void InitServer()
        {
            //1.创建套接字
            UdpSock_ = socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
            if(UdpSock_ < 0)
            {
                LOG(FATAL,"Creat socket failed")<<std::endl;
                //perror("socket");
                exit(1);
            }
            //2.绑定地址信息
            struct sockaddr_in addr;
            addr.sin_family = AF_INET;
            addr.sin_port = htons(UdpPort_);
            addr.sin_addr.s_addr =inet_addr( "0.0.0.0");

            int ret = bind(UdpSock_, (struct  sockaddr*)&addr,sizeof(addr));
            if(ret < 0)
            {
                LOG(FATAL,"Bind addrinfo failed")<<"0.0.0.0:17777"<<std::endl;
                //perror("bind addrinfo failed");
                exit(2);
            }
                

            LOG(INFO,"Udp bind success")<<"0.0.0.0:17777"<<std::endl;

            //初始化数据池
            MsgPool_ = new MsgPool();
            if(!MsgPool_)
            {
                LOG(FATAL,"Create MsgPool failed")<<std::endl;
                // perror("Create MsgPool failed");
                exit(3);
            }
            LOG(INFO,"Create MsgPool success")<<std::endl;
            
            UserMana_ = new UserManager();
            if(!UserMana_)
            {
                LOG(FATAL,"Create Usermanager failed")<<std::endl;
                exit(8);
            }


            //有关TCP通信 创建 绑定 侦听  初始化   accept接收函数不能放在这里面因为它会阻塞导致后面start无法进行，因此可以把它放在start函数里面
            //创建TCPsocket
            TcpSock_  = socket(AF_INET , SOCK_STREAM ,  IPPROTO_TCP);
            if(TcpSock_ < 0)
            {
                LOG(FATAL,"Create tcp  socket failed")<<std::endl;
                exit(5);
            }
                
            //2MLS  -》 60s
            //地址复用
            int opt = 1;
            setsockopt(TcpSock_,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));

            struct sockaddr_in tcpaddr;
            tcpaddr.sin_family = AF_INET;
            tcpaddr.sin_port = htons(TcpPort_);
            tcpaddr.sin_addr.s_addr = inet_addr("0.0.0.0");
            ret = bind(TcpSock_, (struct sockaddr*)&tcpaddr, sizeof(tcpaddr));
            if(ret < 0)
            {
                LOG(FATAL,"Bind Tcp addrinfo failed")<<std::endl;
                exit(6);
            }

            ret = listen(TcpSock_, 5);
            if(ret < 0)
            {
                LOG(FATAL,"Tcp listen failed")<<std::endl;
                exit(7);
            }
            LOG(INFO,"Tcp listen 0.0.0.0 : 17778")<<std::endl;
        }


        //初始化程序当中生产和消费线程
        void Start()
        {
            pthread_t tid;
            for(int i = 0; i < THREAD_COUNT; i++)
            { 
                int ret = pthread_create(&tid, NULL,ProductMsgStart, (void*)this);
                if(ret  < 0)
                {
                    LOG(FATAL,"pthread_create new thread failed")<<std::endl;
                    //perror("pthread_creat  new thread failed");
                    exit(4);
                }
                
                ret = pthread_create(&tid, NULL, ConsumeMsgStart, (void*)this);
                if(ret < 0)
                {
                    LOG(FATAL,"pthread_create new thread failed")<<std::endl;
                    //perror("pthread_creat  new thread failed");
                    exit(4);
                }
            }
            LOG(INFO,"UdpChat Server start success")<<std::endl;


            //tcp accept接收函数
            while(1)
            {
                struct sockaddr_in cliaddr;
                socklen_t cliaddrlen = sizeof(cliaddr);
                int newsock = accept(TcpSock_, (struct sockaddr*)&cliaddr, &cliaddrlen);
                if(newsock < 0)
                {
                    LOG(ERROR,"Accept new connect failed")<<std::endl;//不是致命连接
                    continue;//继续还有可能接收其他连接
                }

                LoginConnect* lc = new LoginConnect(newsock, (void*)this);
                if(!lc)
                {
                    LOG(ERROR,"Create LoginConnect failed")<<std::endl;
                    continue;
                }
                //创建线程，处理登录&&注册请求
                pthread_t tid;
                int ret = pthread_create(&tid,NULL,LoginRegStart,(void*)lc);
                if(ret < 0)
                {
                    //ERROR : 一个失败，不能影响新的连接创建线程  && 不能影响Udp正常发送数据的客户端
                    LOG(ERROR,"Create user loginconnect failed")<<std::endl;
                    continue;
                }
                 LOG(INFO,"Create TcpConnect thread  success")<<std::endl;
            }
        }
    private:
        //生产线程：负责生产线程 从接收缓冲区接收数据放到后端数据池当中去
        //static 作用：入口函数默认只有一个参数，加上static避免this指针成为第一个参数。
        static void* ProductMsgStart(void* arg)
        {
            //默认创建出来线程都是join_able 属性，因此进行线程分离
            pthread_detach(pthread_self());
            ChatServer* cs  = (ChatServer*)arg;
            while(1)
            {
                //recvfrom udp 数据
                cs->RecvMsg();
            }
            
            return NULL;
        }
        //消费线程：负责消费线程  从后端的消息缓冲池获取数据进行发送
        static void* ConsumeMsgStart(void* arg)
        {
            pthread_detach(pthread_self());
            ChatServer* cs  = (ChatServer*)arg;
            while(1)
            {
                //sendto  udp消息
                cs->BroadcastMsg();
            }
            return NULL;
        }

        static void* LoginRegStart(void*arg)
        {
            pthread_detach(pthread_self());
            LoginConnect* lc = (LoginConnect*)arg;
            ChatServer* cs = (ChatServer*)lc->GetServer();
            //注册（需要和用户管理模块打交道） 登录请求
                //请求从cli端来， recv(sock,buf,size,0);
                //响应 send(sock,buf,size,0)
            
            //先接受一个字节的 判断是登录还是注册
            char RequestType;
            ssize_t recvsize =  recv(lc->GetTcpSock(),&RequestType,1,0);
            if(recvsize < 0)
            {
                LOG(ERROR,"Recv TagType failed")<<std::endl;
                return NULL;
            }
            else if(recvsize == 0)//表示对端关闭连接
            {
                LOG(ERROR,"Client shutdown connect")<<std::endl;
                return NULL;
            }
            
            uint32_t UserId = -1;
            int UserStatus = -1;
            //正常接收到一个标识请求
            switch(RequestType)
            {
                case REGISTER:
                    //使用用户管理模块的注册
                        UserStatus  =  cs->DealRegister(lc->GetTcpSock(), &UserId);
                        break;

                case LOGIN:
                    //使用用户管理模块的登录
                    {
                        UserStatus =  cs->DealLogin(lc->GetTcpSock());
                        printf("UserStatus:%d\n",UserStatus);
                        break;
                    }
                case LOGINOUT:
                    //使用用户管理模块的退出登录
                    cs->DealLoginOut();
                    break;
                default:
                    UserStatus = REGFAILED;
                    LOG(ERROR,"Recv Request type not a effective value ")<<std::endl;
                    break;
            }
            //响应send(sock,buf,size,0);
            //需要和用户管理模块打交道
            ReplyInfo ri;
            ri.Status = UserStatus;
            ri.UserId_ = UserId;
            ssize_t sendsize = send(lc->GetTcpSock(),&ri,sizeof(ri),0);
            if(sendsize < 0)
            {
                //TODO
                //如果发送失败是否考虑应用层重新发一个
                LOG(ERROR,"SendMsg Failed ")<<std::endl;
            }
            LOG(INFO,"SendMsg success")<<std::endl;

            //将TCP连接释放
            close(lc->GetTcpSock());
            delete lc;
            return NULL;
        }
        
        int DealRegister(int Sock, uint32_t* UseId )
        {
            //接收注册请求
            RegInfo ri;
            ssize_t recvsize =  recv(Sock, &ri, sizeof(ri),0);
            if(recvsize < 0)
            {
                LOG(ERROR,"Recv TagType failed")<<std::endl;
                return OFFLINE;
            }
            else if(recvsize == 0)//表示对端关闭连接
            {
                LOG(ERROR,"Client shutdown connect")<<std::endl;
                //特殊处理 对端关闭情况
            }
            //调用用户管理模块进行注册请求的处理
            

            //返回注册成功之后给用户的UserId
            int ret = UserMana_ ->Register(ri.NickName_, ri.School_, ri.Passwd_, UseId);
            //返回当前状态
            if(ret == -1)
            {
                return REGFAILED;
            }
            else 
                return REGISTERED;
        }

        int DealLogin(int Sock)
        {
            struct LoginInfo li;
            ssize_t  recvsize=recv(Sock,&li,sizeof(li),0);
            if(recvsize < 0)
            {
                LOG(ERROR,"Recv TagType failed")<<std::endl;
                return OFFLINE;
            }
            else if(recvsize  == 0)
            {
                LOG(ERROR,"Client shutdown connect")<<std::endl;
                //需要进处理下
            }

            LOG(DEBUG,"UserId:Passwd")<<li.UserId_<<":"<<li.Passwd_<<std::endl;

            int ret  = UserMana_->Login(li.UserId_, li.Passwd_);
             if(ret == -1)
             {
                    LOG(ERROR,"User Login failed")<<std::endl;
                    return LOGINFAILED;
             }
             return LOGINED;   
        }


        int DealLoginOut()
        {

        }

    private:
        void RecvMsg()
        {
            //接收客户端发送的数据
            char buf[10240] = {0};
            struct sockaddr_in cliaddr;
            socklen_t cliaddrlen = sizeof(struct sockaddr_in );
            int recvsize = recvfrom(UdpSock_,buf,sizeof(buf)-1 ,0 , (struct sockaddr*)&cliaddr,&cliaddrlen);
            if(recvsize < 0)
            {
                //接收失败i
                LOG(ERROR,"recvfrom msg failed")<<std::endl;
                //perror("recvfrom msg failed");
            }
            else 
            {   //正常逻辑 接收数据成功
                //将数据放到数据池
                std::string msg ;
                msg.assign(buf,recvsize);
                LOG(INFO,msg)<<std::endl;
                //需要将发送的数据从JSON转化为我们可以识别的数据
                
                Message jsonmsg;
                jsonmsg.Deserialize(msg);

                //需要增加用户管理，只有进行注册登录的人，才可以向服务器发送消息                 //1.校验当前的消息是否属于注册用户或者老用户发送的
                //  1.1不是 则认为是非法消息
                //  1.2是 区分一下是第一次发送消息的
                //      是：保存UDP地址信息，并且更新状态为在线,将数据放到数据池当中
                //      是老用户：直接将数据放到数据池当中
                //2.需要校验，需要和用户管理模块打交道
                
                
                // UserMana_ ->判断当前的用户是否是注册用户的函数
                bool ret =UserMana_->IsLogin(jsonmsg.GetUserId() ,cliaddr,cliaddrlen) ;
               if(ret != true) 
               {
                    LOG(ERROR,"diacarded the msg")<<msg<<std::endl;
               }
               LOG(INFO,"Push msg to MsgPool")<<std::endl; 
               MsgPool_->PushMsgToPool(msg);
            }
        }

        //组播接口   1.从保存在线用户的数据结构遍历进行循环发送  2.从后端数据池获取要发送的内容，将发送内容传送给SendMsg进行调用发送
        void BroadcastMsg()
        {
            //1.获取要给哪个用户发送
            //2.获取发送内容
            std::string msg;
            MsgPool_->PopMsgFromPool(&msg);
            //调用SendMsg();进行群发
            // SendMsg();
           //用户管理系统提供结构获取在线的用户列表
            std::vector<UserInfo> OnlineUserVec;
            UserMana_ ->GetOnlineUserInfo(&OnlineUserVec);
            std::vector<UserInfo>::iterator iter = OnlineUserVec.begin();
            for(;iter != OnlineUserVec.end();iter++)
            {
                //SendMsg()
               //SendMsg(msg,udp地址信息，udp地址信息长度) -》UserInfo 
                SendMsg(msg,iter->GetCliAddrInfo(),iter->GetCliAddrLen());
            }
        }

        //给一个客户端发送单个消息接口
        void SendMsg(const std::string&msg, struct sockaddr_in& cliaddr , socklen_t& len)
        {
            ssize_t sendsize = sendto(UdpSock_, msg.c_str(), msg.size(), 0, (struct sockaddr*)&cliaddr , len );
            if(sendsize < 0)
            {
                LOG(ERROR,"sendto msg failed")<<std::endl;
                //perror("sendto msg failed");
                //没有发送成功我们是否需要缓存没有发送成功的信息还有客户端的地
            }
            else
            {
                //成功发送 
                LOG(INFO,"Sendto Msg success")<<"["<<inet_ntoa(cliaddr.sin_addr)<<":"<<ntohs(cliaddr.sin_port)<<"]"<<msg<<std::endl;
            }
        }
    private:
        int UdpSock_;
        int UdpPort_;
        //数据池
        MsgPool* MsgPool_;

        //TCP 处理注册登录请求
        int TcpSock_;
        int TcpPort_;

        //用户管理
        UserManager* UserMana_;
};


