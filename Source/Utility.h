#pragma once

#include <easylogging.h>
#include <rapidjson/document.h>
#include <atomic>

// Semi-safe pointer deletion
#define SafeDelete(pPtr) { delete pPtr; pPtr = nullptr; }

namespace util
{
	bool ParseIntFromJsonString( const std::string &jsonStringIn, const std::string &keyIn, int &valueOut );

	class SpinLock
	{
	public:
		SpinLock(std::atomic<bool>& lock) : m_lock(lock)
		{
			while(m_lock.exchange(true)){/*spin*/}
		}

		~SpinLock()
		{
			m_lock = false;
		}

		SpinLock(const SpinLock&) = delete;
		SpinLock& operator=(const SpinLock&) = delete;
	private:
		std::atomic<bool>& m_lock;
	};
}
