/**************************************************
* File name: Msg.h
* Author: flyer<2350827470@qq.com>
* Copyright 2021. All right reserved.
* Description:
	利用这个类实现了数据不需要复制,光是进行移动
**************************************************/
#pragma once
#include <memory>
#include <utility>
/**
 * @brief 用来在队列中传递的消息，其中可携带任意类型的数据.不可用作基类
 *
 * @tparam T 表示消息中蕴含的任意类的类型
 */
template <typename T>
class Msg {
	std::unique_ptr<T> load_data;

public:
	template <typename... Arg>
	Msg(Arg &&...args) : load_data(new T(std::forward<Arg>(args)...)) // 构造函数
	{
	}
	Msg(const Msg &) = delete;     // 复制构造
	~Msg() = default;              // 析构
	std::unique_ptr<Msg<T>> move() // 利用这个函数来移动
	{
		return std::unique_ptr<Msg<T>>(new Msg<T>(std::move(*this))); // 这里用到了移动构造函数
	}
	T &get_load_data()
	{
		return *load_data;
	}
	Msg(Msg<T> &&) = default; // 移动构造函数
private:
	Msg<T> &operator=(Msg<T> &&) = delete;
};
