#pragma once
#include<queue>
#include<string>
#include<pthread.h>
#include<iostream>

#define MSG_POOL_SIZE 1024

class MsgPool
{

    public:
        MsgPool()
        {
            Capcity_ = MSG_POOL_SIZE;
            pthread_mutex_init(&MsgQueLock_,NULL);
            pthread_cond_init(&SynComQue_,NULL);
            pthread_cond_init(&SynProQue_,NULL);
        }

        ~MsgPool()
        {
            pthread_mutex_destroy(&MsgQueLock_);
            pthread_cond_destroy(&SynComQue_);
            pthread_cond_destroy(&SynProQue_);
        }
        
        void PushMsgToPool(std::string& msg)
        {
            pthread_mutex_lock(&MsgQueLock_);
            //判断队列是否满了
            while(IsFull())
            {
                //满了，放到生产者阻塞等待队列中去
                pthread_cond_wait(&SynProQue_,&MsgQueLock_);   
            }
            //进行生产 插入
            MsgQue_.push(msg);
            pthread_mutex_unlock(&MsgQueLock_);

            //通知消费者 可以进行消费
            pthread_cond_signal(&SynComQue_);
        }

        void PopMsgFromPool(std::string* msg)
        {
            pthread_mutex_lock(&MsgQueLock_);
            while(MsgQue_.empty())
            {
                //消费时发现队列为空 进入消费者等待队列进行等待
                pthread_cond_wait(&SynComQue_, &MsgQueLock_);
            }
            //进行消费
            *msg = MsgQue_.front();
            MsgQue_.pop();
            pthread_mutex_unlock(&MsgQueLock_);

            //通知生产者  可以进行生产
            pthread_cond_signal(&SynProQue_);
        }

    private:
        bool IsFull()
        {
            if(MsgQue_.size() == Capcity_)
            {
                return true;
            }
            return false;
        }
    private:
        //数据池队列
        std::queue<std::string> MsgQue_;
        //约束队列大小，防止异常情况下，队列无线扩容，导致内存占用过大或者被操作系统强杀。
        size_t Capcity_;
        //互斥锁   互斥
        pthread_mutex_t MsgQueLock_;
        //条件变量 同步 消费者条件变量 
        pthread_cond_t SynComQue_;
        //条件变量 同步 生产者条件变量
        pthread_cond_t SynProQue_;

};

