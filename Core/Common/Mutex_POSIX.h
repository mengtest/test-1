#ifndef Foundation_Mutex_POSIX_INCLUDED
#define Foundation_Mutex_POSIX_INCLUDED


#include "Platform.h"
#include "Exception.h"
#include <pthread.h>
#include <errno.h>


class MutexImpl
{
protected:
	MutexImpl();
	MutexImpl(bool fast);
	~MutexImpl();
	void lockImpl();
	bool tryLockImpl();
	bool tryLockImpl(long milliseconds);
	void unlockImpl();

private:
	pthread_mutex_t _mutex;
};


class  FastMutexImpl: public MutexImpl
{
protected:
	FastMutexImpl();
	~FastMutexImpl();
};


//
// inlines
//
inline void MutexImpl::lockImpl()
{
	if (pthread_mutex_lock(&_mutex))
		throw SystemException("cannot lock mutex");
}


inline bool MutexImpl::tryLockImpl()
{
	int rc = pthread_mutex_trylock(&_mutex);
	if (rc == 0)
		return true;
	else if (rc == EBUSY)
		return false;
	else
		throw SystemException("cannot lock mutex");

}


inline void MutexImpl::unlockImpl()
{
	if (pthread_mutex_unlock(&_mutex))
		throw SystemException("cannot unlock mutex");
}


#endif // Foundation_Mutex_POSIX_INCLUDED
