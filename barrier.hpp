
#include <mutex>
#include <condition_variable>
#include <cstddef>


#ifndef BARRIER_INC
#define BARRIER_INC

namespace ari {

class barrier {
	size_t mThreads;
	size_t mWaiting;
	bool mCycle; // Boost makes this a size_t, however a bool will suffice and
	// potentially uses less memory.
	
	std::mutex mMtx;
	std::condition_variable mCondVar;
	
public:
	explicit barrier(ptrdiff_t num_threads)
	    : mThreads{ num_threads }, mWaiting{ num_threads }, mCycle{ false } {};
	barrier(const barrier&) = delete;
	
	barrier& operator=(const barrier&) = delete;
	~barrier() = default;
	
	void arrive_and_wait() {
		std::unique_lock lk{ mMtx };
		
		--mWaiting;
		
		if (mWaiting == 0) { // we are the last one waiting
			mWaiting = mThreads;
			mCycle = !mCycle;
			lk.unlock();
			mCondVar.notify_all();
			return;
		}
		
		bool cycle_local = mCycle;
		mCondVar.wait(lk, [&]{ return mCycle == cycle_local; });
		
	}
	
};

} // end namespace ari

#endif