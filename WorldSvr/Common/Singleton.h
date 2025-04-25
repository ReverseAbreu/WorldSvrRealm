#pragma once

template <typename T> class Singleton
{
protected:
	static T instance;
	Singleton() {}

public:
	Singleton(Singleton const&) = delete;
	void operator=(Singleton const&) = delete;

	static T* GetInstance() { return &instance; }
};

template<typename T> T Singleton<T>::instance;