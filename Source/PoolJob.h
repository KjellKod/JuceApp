#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "g3log/g2logworker.hpp"
#include "g3log/g2log.hpp"

class PoolJob : public ThreadPoolJob
{

 public:
	PoolJob(int count)
		: ThreadPoolJob("Threadpool Job"),
		  count_(count)
	{
	}

	JobStatus runJob() override
	{
		Thread::sleep(30);

		LOG(DBUG) << "Count " << count_ << " handled from thread: " << Thread::getCurrentThreadId();
		
		if (count_ > 20)
		{
			int * p = nullptr;
			*p = 10;
		}
		return jobNeedsRunningAgain;
	}

	void removedFromQueue()
	{
		
	}

 private:
	int count_;
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PoolJob)
};