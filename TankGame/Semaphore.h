/**************************************************
* File name: semaphore.h
* Author: flyer<2350827470@qq.com>
* Copyright 2021. All right reserved.
* Description:
	自编的信号量
**************************************************/
#pragma once
#include <mutex>
#include <condition_variable>
class Semaphore
{
	int avail_num; // 表示资源量,可为负数
	std::mutex mutex_;
	std::condition_variable cv;

public:
	/*显式的default constructor*/
	explicit Semaphore(int source_num = 0) :avail_num(source_num), mutex_(), cv() {}
	/*没有copy constructor*/
	Semaphore(const Semaphore&) = delete;
	/*p 操作*/
	void wait() {
		std::unique_lock<std::mutex> lock(mutex_);
		cv.wait(lock, [this] {return avail_num > 0; });
		avail_num--;
	}
	/*v 操作*/
	void signal() {
		std::unique_lock<std::mutex> lock(mutex_);
		avail_num++;
		cv.notify_one();
	}

	/* 返回当前有多少资源可用 */
	bool is_waitable() {
		std::unique_lock<std::mutex> lock(mutex_);
		return avail_num > 0?true:false;
	}
};
