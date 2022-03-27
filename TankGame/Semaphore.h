/**************************************************
* File name: semaphore.h
* Author: flyer<2350827470@qq.com>
* Copyright 2021. All right reserved.
* Description:
	�Ա���ź���
**************************************************/
#pragma once
#include <mutex>
#include <condition_variable>
class Semaphore
{
	int avail_num; // ��ʾ��Դ��,��Ϊ����
	std::mutex mutex_;
	std::condition_variable cv;

public:
	/*��ʽ��default constructor*/
	explicit Semaphore(int source_num = 0) :avail_num(source_num), mutex_(), cv() {}
	/*û��copy constructor*/
	Semaphore(const Semaphore&) = delete;
	/*p ����*/
	void wait() {
		std::unique_lock<std::mutex> lock(mutex_);
		cv.wait(lock, [this] {return avail_num > 0; });
		avail_num--;
	}
	/*v ����*/
	void signal() {
		std::unique_lock<std::mutex> lock(mutex_);
		avail_num++;
		cv.notify_one();
	}

	/* ���ص�ǰ�ж�����Դ���� */
	bool is_waitable() {
		std::unique_lock<std::mutex> lock(mutex_);
		return avail_num > 0?true:false;
	}
};
