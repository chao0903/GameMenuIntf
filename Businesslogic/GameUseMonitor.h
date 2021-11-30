#pragma once
#include <string>
#include <tchar.h>
#include <memory>
#include <stdint.h>
#include <vector>
#include <mutex>
#include <map>
#include <functional>
#include "WyGameServerMsgDef.h"
#include <safestl.hpp>
using namespace WyGameServer;
using namespace std;
typedef struct processMonitorTask
{
	int       pid;
	bool      bCreate;
	char      runExe[MAX_PATH];
	uint64_t  createTime;
}processMonitorTask;

typedef struct processRunInfo
{
	UINT64     resId;
	uint64_t   start_time;
	uint64_t   end_time;
}processRunInfo;

template <class T>
class TaskQueue
{
public:
	using MutexLockGuard = std::lock_guard<std::mutex>;
	TaskQueue() : _mutex(), _notEmpty(), _queue() {}
	TaskQueue(const TaskQueue &) = delete;
	TaskQueue& operator=(const TaskQueue &) = delete;
	void put(const T &x)
	{
		{
			MutexLockGuard lock(_mutex);
			_queue.push_back(x);
		}
		_notEmpty.notify_all();
	}

	void put(T &&x)
	{
		{
			MutexLockGuard lock(_mutex);
			_queue.push_back(std::move(x));
		}
		_notEmpty.notify_all();
	}

	T take()
	{
		std::unique_lock<std::mutex> lock(_mutex);
		_notEmpty.wait(lock, [this] {  return !this->_queue.empty(); });
		assert(!_queue.empty());

		T front(std::move(_queue.front()));
		_queue.pop_front();

		return  front;
	}
	std::vector<T> takeAll()
	{
		std::vector<T>  vecT;
		std::unique_lock<std::mutex> lock(_mutex);
		_notEmpty.wait(lock, [this] {  return !this->_queue.empty(); });
		while (!_queue.empty())
		{
			vecT.push_back(std::move(_queue.front()));
			_queue.pop_front();
		}
		return  vecT;
	}
	size_t size() const
	{
		MutexLockGuard lock(_mutex);
		return _queue.size();
	}
private:
	mutable std::mutex _mutex;
	std::condition_variable _notEmpty;
	std::deque<T> _queue;
};

class CGameUseMonitor
{
public:
	static std::shared_ptr<CGameUseMonitor>& instance();
	~CGameUseMonitor();
	bool initializer();
	bool isGameOver(UINT64 resId);
	void setAccount(string account);
	bool isGameDriver(TCHAR driver);
	void addReportTask(processMonitorTask& task);
	void updateDomain();
	void loadAllResouce();
protected:
	CGameUseMonitor();
private:
	bool getResInfoByRunExe(string strRunExe, WyClientGetResSimpleInfo& runInfo);
	void procReportAsync();
	BOOL get_current_programme_path(PTCHAR strPath);
private:
	std::mutex                                               m_cLock;
	std::map<int, processRunInfo>                            m_cMapMonitorGame; //int->资源进程，UINT64 进程ID
	std::map<TCHAR, TCHAR>                                   m_cGameDriverMap;
	CSafeMap<UINT64, WyClientGetResSimpleInfo>               m_cMapResRunExe;
	TaskQueue<processMonitorTask>                            m_cProcessMonitorTask;
	string                                                   m_strAccount;
};


