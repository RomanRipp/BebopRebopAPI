#pragma once

#include <easylogging.h>
#include <rapidjson/document.h>
#include <atomic>

#include "BebopCommandsAndData.h"

// Semi-safe pointer deletion
#define SafeDelete(pPtr) { delete pPtr; pPtr = nullptr; }

namespace util
{
	bool ParseIntFromJsonString( const std::string &jsonStringIn, const std::string &keyIn, int &valueOut );

	bool ReadFramesFromFile(const std::string& fileName, std::vector<rebop::commands::bebop::video::TRawFrame>& frames);
	bool WriteFramesToFile(const std::string& fileName, const std::vector<rebop::commands::bebop::video::TRawFrame>& frames);

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
