#include <list>
#include <boost/thread/mutex.hpp>
#include <boost/thread/recursive_mutex.hpp>
#include <boost/thread/condition.hpp>
#include <boost/thread/semaphore.hpp>
//#include <boost/thread/atomic.hpp>
#include <boost/thread/tss.hpp>

#define BOOST_INCLUDE_MAIN
#include <boost/test/test_tools.hpp>

#include <windows.h>

template <typename M>
void test_lock(M* dummy=0)
{
	typedef M mutex_type;
	typedef typename M::lock lock_type;

	mutex_type mutex;
	boost::condition condition;

    // Test the lock's constructors.
    {
        lock_type lock(mutex, false);
        BOOST_TEST(!lock);
    }
	lock_type lock(mutex);
	BOOST_TEST(lock);

    // Construct and initialize an xtime for a fast time out.
    boost::xtime xt;
    BOOST_TEST(boost::xtime_get(&xt, boost::TIME_UTC) == boost::TIME_UTC);
    xt.nsec += 100000000;

    // Test the lock and the mutex with condition variables.
    // No one is going to notify this condition variable.  We expect to
    // time out.
	BOOST_TEST(condition.timed_wait(lock, xt) == false);
	BOOST_TEST(lock);

    // Test the lock and unlock methods.
	lock.unlock();
	BOOST_TEST(!lock);
	lock.lock();
	BOOST_TEST(lock);
}

template <typename M>
void test_trylock(M* dummy=0)
{
	typedef M mutex_type;
	typedef typename M::trylock trylock_type;

	mutex_type mutex;
	boost::condition condition;

    // Test the lock's constructors.
    {
        trylock_type lock(mutex);
        BOOST_TEST(lock);
    }
    {
        trylock_type lock(mutex, false);
        BOOST_TEST(!lock);
    }
	trylock_type lock(mutex, true);
	BOOST_TEST(lock);

    // Construct and initialize an xtime for a fast time out.
    boost::xtime xt;
    BOOST_TEST(boost::xtime_get(&xt, boost::TIME_UTC) == boost::TIME_UTC);
    xt.nsec += 100000000;

    // Test the lock and the mutex with condition variables.
    // No one is going to notify this condition variable.  We expect to
    // time out.
	BOOST_TEST(condition.timed_wait(lock, xt) == false);
    BOOST_TEST(lock);

    // Test the lock, unlock and trylock methods.
	lock.unlock();
	BOOST_TEST(!lock);
	lock.lock();
	BOOST_TEST(lock);
	lock.unlock();
	BOOST_TEST(!lock);
	BOOST_TEST(lock.try_lock());
	BOOST_TEST(lock);
}

template <typename M>
void test_timedlock(M* dummy=0)
{
	typedef M mutex_type;
	typedef typename M::timedlock timedlock_type;

	mutex_type mutex;
	boost::condition condition;

    // Test the lock's constructors.
    {
        // Construct and initialize an xtime for a fast time out.
        boost::xtime xt;
        BOOST_TEST(boost::xtime_get(&xt, boost::TIME_UTC) == boost::TIME_UTC);
        xt.nsec += 100000000;

        timedlock_type lock(mutex, xt);
        BOOST_TEST(lock);
    }
    {
        timedlock_type lock(mutex, false);
        BOOST_TEST(!lock);
    }
	timedlock_type lock(mutex, true);
	BOOST_TEST(lock);

    // Construct and initialize an xtime for a fast time out.
    boost::xtime xt;
    BOOST_TEST(boost::xtime_get(&xt, boost::TIME_UTC) == boost::TIME_UTC);
    xt.nsec += 100000000;

    // Test the lock and the mutex with condition variables.
    // No one is going to notify this condition variable.  We expect to
    // time out.
    BOOST_TEST(condition.timed_wait(lock, xt) == false);
	BOOST_TEST(lock);

    // Test the lock, unlock and timedlock methods.
    lock.unlock();
	BOOST_TEST(!lock);
	lock.lock();
	BOOST_TEST(lock);
	lock.unlock();
	BOOST_TEST(!lock);
    BOOST_TEST(boost::xtime_get(&xt, boost::TIME_UTC) == boost::TIME_UTC);
    xt.nsec += 100000000;
    BOOST_TEST(lock.timed_lock(xt));
}

void test_mutex()
{
    typedef boost::mutex mutex;
    test_lock<mutex>();
}

void test_try_mutex()
{
    typedef boost::try_mutex mutex;
    test_lock<mutex>();
    test_trylock<mutex>();
}

void test_timed_mutex()
{
    typedef boost::timed_mutex mutex;
    test_lock<mutex>();
    test_trylock<mutex>();
    test_timedlock<mutex>();
}

void test_recursive_mutex()
{
    typedef boost::recursive_mutex mutex;
    test_lock<mutex>();
    mutex mx;
    mutex::lock lock1(mx);
    mutex::lock lock2(mx);
}

void test_recursive_try_mutex()
{
    typedef boost::recursive_try_mutex mutex;
    test_lock<mutex>();
    test_trylock<mutex>();
    mutex mx;
    mutex::lock lock1(mx);
    mutex::lock lock2(mx);
}

void test_recursive_timed_mutex()
{
    typedef boost::recursive_timed_mutex mutex;
    test_lock<mutex>();
    test_trylock<mutex>();
    test_timedlock<mutex>();
    mutex mx;
    mutex::lock lock1(mx);
    mutex::lock lock2(mx);
}

struct condition_test_data
{
	condition_test_data() : notified(0), awoken(0) { }

	boost::mutex mutex;
	boost::condition condition;
	int notified;
	int awoken;
};

void condition_test_thread(void* param)
{
    condition_test_data* data = static_cast<condition_test_data*>(param);
	boost::mutex::lock lock(data->mutex);
	BOOST_TEST(lock);
	while (!(data->notified > 0))
		data->condition.wait(lock);
	BOOST_TEST(lock);
	data->awoken++;
}

class thread_adapter
{
public:
    thread_adapter(void (*func)(void*), void* param) : _func(func), _param(param) { }
    void operator()() const { _func(_param); }
private:
    void (*_func)(void*);
    void* _param;
};

void test_condition_notify_one()
{
	condition_test_data data;

    boost::thread::create(thread_adapter(&condition_test_thread, &data));

	{
		boost::mutex::lock lock(data.mutex);
		BOOST_TEST(lock);
		data.notified++;
		data.condition.notify_one();
	}

    boost::thread::join_all();
	BOOST_TEST(data.awoken == 1);
}

void test_condition_notify_all()
{
	const int NUMTHREADS = 5;
	condition_test_data data;

	for (int i = 0; i < NUMTHREADS; ++i)
        boost::thread::create(thread_adapter(&condition_test_thread, &data));

	{
		boost::mutex::lock lock(data.mutex);
		BOOST_TEST(lock);
		data.notified++;
		data.condition.notify_all();
	}

    boost::thread::join_all();
	BOOST_TEST(data.awoken == NUMTHREADS);
}

struct cond_predicate
{
    cond_predicate(int& var, int val) : _var(var), _val(val) { }

    bool operator()() { return _var == _val; }

    int& _var;
    int _val;
};

void condition_test_waits(void* param)
{
    condition_test_data* data = static_cast<condition_test_data*>(param);

    boost::mutex::lock lock(data->mutex);
    BOOST_TEST(lock);

    // Test wait.
    while (data->notified != 1)
        data->condition.wait(lock);
    BOOST_TEST(lock);
    BOOST_TEST(data->notified == 1);
    data->awoken++;
    data->condition.notify_one();

    // Test predicate wait.
    data->condition.wait(lock, cond_predicate(data->notified, 2));
    BOOST_TEST(lock);
    BOOST_TEST(data->notified == 2);
    data->awoken++;
    data->condition.notify_one();

    // Test timed_wait.
    boost::xtime xt;
    BOOST_TEST(boost::xtime_get(&xt, boost::TIME_UTC) == boost::TIME_UTC);
    xt.nsec += 100000000;
    while (data->notified != 3)
        data->condition.timed_wait(lock, xt);
    BOOST_TEST(lock);
    BOOST_TEST(data->notified == 3);
    data->awoken++;
    data->condition.notify_one();

    // Test predicate timed_wait.
    BOOST_TEST(boost::xtime_get(&xt, boost::TIME_UTC) == boost::TIME_UTC);
    xt.sec += 2;
    BOOST_TEST(data->condition.timed_wait(lock, xt, cond_predicate(data->notified, 4)));
    BOOST_TEST(lock);
    BOOST_TEST(data->notified == 4);
    data->awoken++;
}

void test_condition_waits()
{
    condition_test_data data;

    boost::thread::create(thread_adapter(&condition_test_waits, &data));

    boost::xtime xt;

    {
        boost::mutex::lock lock(data.mutex);
        BOOST_TEST(lock);

        BOOST_TEST(boost::xtime_get(&xt, boost::TIME_UTC) == boost::TIME_UTC);
        xt.sec += 1;
        boost::thread::sleep(xt);
        data.notified++;
        data.condition.notify_one();
        while (data.awoken != 1)
            data.condition.wait(lock);
        BOOST_TEST(data.awoken == 1);

        BOOST_TEST(boost::xtime_get(&xt, boost::TIME_UTC) == boost::TIME_UTC);
        xt.sec += 1;
        boost::thread::sleep(xt);
        data.notified++;
        data.condition.notify_one();
        while (data.awoken != 2)
            data.condition.wait(lock);
        BOOST_TEST(data.awoken == 2);

        BOOST_TEST(boost::xtime_get(&xt, boost::TIME_UTC) == boost::TIME_UTC);
        xt.sec += 1;
        boost::thread::sleep(xt);
        data.notified++;
        data.condition.notify_one();
        while (data.awoken != 3)
            data.condition.wait(lock);
        BOOST_TEST(data.awoken == 3);
    }

    BOOST_TEST(boost::xtime_get(&xt, boost::TIME_UTC) == boost::TIME_UTC);
    xt.sec += 1;
    boost::thread::sleep(xt);
    data.notified++;
    data.condition.notify_one();
    BOOST_TEST(boost::xtime_get(&xt, boost::TIME_UTC) == boost::TIME_UTC);
    xt.sec += 1;
    boost::thread::sleep(xt);
    boost::thread::join_all();
    BOOST_TEST(data.awoken == 4);
}

void test_condition()
{
    test_condition_notify_one();
    test_condition_notify_all();
    test_condition_waits();
}

void test_semaphore()
{
    boost::xtime xt;
    unsigned val;
    boost::semaphore sema(0, 1);

    BOOST_TEST(sema.up(1, &val));
    BOOST_TEST(val == 0);
    BOOST_TEST(!sema.up());

    sema.down();
    BOOST_TEST(sema.up());

    BOOST_TEST(boost::xtime_get(&xt, boost::TIME_UTC) == boost::TIME_UTC);
    xt.nsec += 100000000;
    BOOST_TEST(sema.down(xt));
    BOOST_TEST(boost::xtime_get(&xt, boost::TIME_UTC) == boost::TIME_UTC);
    xt.nsec += 100000000;
    BOOST_TEST(!sema.down(xt));
}

/*void test_atomic_t()
{
    boost::atomic_t atomic;
    BOOST_TEST(boost::increment(atomic) > 0);
    BOOST_TEST(boost::decrement(atomic) == 0);
    BOOST_TEST(boost::swap(atomic, 10) == 0);
    BOOST_TEST(boost::swap(atomic, 0) == 10);
    BOOST_TEST(boost::compare_swap(atomic, 20, 10) == 0);
    BOOST_TEST(boost::compare_swap(atomic, 20, 0) == 0);
    BOOST_TEST(boost::read(atomic) == 20);
}*/

void test_tss_thread()
{
    static boost::tss value;
    value.set(new int(0));

    for (int i=0; i<1000; ++i)
    {
        int* pn = static_cast<int*>(value.get());
        BOOST_TEST(*pn == i);
        ++*pn;
        boost::thread::yield();
    }
}

void test_tss()
{
    for (int i=0; i<5; ++i)
        boost::thread::create(&test_tss_thread);
    boost::thread::join_all();
}

int test_main(int, char*[])
{
    test_mutex();
    test_try_mutex();
    test_timed_mutex();
    test_recursive_mutex();
    test_recursive_try_mutex();
    test_recursive_timed_mutex();
    test_condition();
    test_semaphore();
    test_tss();
    return 0;
}
