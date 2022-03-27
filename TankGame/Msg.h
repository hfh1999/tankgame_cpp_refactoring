/**************************************************
* File name: Msg.h
* Author: flyer<2350827470@qq.com>
* Copyright 2021. All right reserved.
* Description:
	���������ʵ�������ݲ���Ҫ����,���ǽ����ƶ�
**************************************************/
#pragma once
#include <memory>
#include <utility>
/**
 * @brief �����ڶ����д��ݵ���Ϣ�����п�Я���������͵�����.������������
 *
 * @tparam T ��ʾ��Ϣ���̺��������������
 */
template <typename T>
class Msg {
	std::unique_ptr<T> load_data;

public:
	template <typename... Arg>
	Msg(Arg &&...args) : load_data(new T(std::forward<Arg>(args)...)) // ���캯��
	{
	}
	Msg(const Msg &) = delete;     // ���ƹ���
	~Msg() = default;              // ����
	std::unique_ptr<Msg<T>> move() // ��������������ƶ�
	{
		return std::unique_ptr<Msg<T>>(new Msg<T>(std::move(*this))); // �����õ����ƶ����캯��
	}
	T &get_load_data()
	{
		return *load_data;
	}
	Msg(Msg<T> &&) = default; // �ƶ����캯��
private:
	Msg<T> &operator=(Msg<T> &&) = delete;
};
