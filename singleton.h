#pragma once

template<typename T>
class Singleton
{
private:

protected:

	Singleton() = default;

public:

	Singleton(const Singleton&) = delete;


	Singleton& operator = (const Singleton&) = delete;


	static T& GetInstance()
	{
		static T Instance;
		return Instance;
	}
};