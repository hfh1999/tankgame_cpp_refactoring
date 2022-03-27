/**************************************************
* File name: MsgQueue.h
* Author: flyer<2350827470@qq.com>
* Copyright 2021. All right reserved.
* Description:
	MsgQueue's generic version,the "T" need to be copyable.
**************************************************/
#pragma once
#include <deque>
#include <mutex>
#include "Semaphore.h"
#include "Msg.h"
using std::deque;
using std::mutex;
template<typename T>
class MsgQueue
{
	deque<Msg<T>> queue_;
	mutex mutex_;
	Semaphore semaphore_;// 信号量
public:
	MsgQueue() : queue_(), mutex_(), semaphore_(0) {}
	/**
	 * @brief blocking recv
	 *
	 * @return T
	 */
	std::unique_ptr<Msg<T>> recv();
	/**
	 * @brief
	 *
	 * @param in_msg
	 */
	void send(Msg<T>&& in_msg);
	std::unique_ptr<Msg<T>> recv_unblock();

};
template <typename T>
std::unique_ptr<Msg<T>> MsgQueue<T>::recv() {
	semaphore_.wait(); //若消息为空,阻塞在这里

	//此时队列里至少有一个
	std::lock_guard<std::mutex> locker(mutex_);
	//使用移动语义,这样可以不复制
	std::unique_ptr<Msg<T>> ret_msg = queue_.at(0).move();
	queue_.pop_front();
	return ret_msg;
}
template <typename T>
void MsgQueue<T>::send(Msg<T>&& in_msg) {
	std::lock_guard<std::mutex> locker(mutex_);
	queue_.push_back(std::move(in_msg));
	//::printf("len = %d\n", queue_.size());
	semaphore_.signal(); //表示有新的消息进入了

}

template<typename T>
std::unique_ptr<Msg<T>> MsgQueue<T>::recv_unblock()
{
	if (semaphore_.is_waitable())//no block.
	{
		semaphore_.wait();// wait here to reduce semaphore's count.
		std::lock_guard<std::mutex> locker(mutex_);
		auto ret_msg = queue_.at(0).move();
		queue_.pop_front();
		return ret_msg;
	}
	else
	{
		//return null ptr if num <= 0
		return std::unique_ptr<Msg<T>>(nullptr);
	}
}
