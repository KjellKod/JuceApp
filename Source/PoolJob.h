#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "g3log/g2logworker.hpp"
#include "g3log/g2log.hpp"
#include "g3log/crashhandler.hpp"

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

		//g2::installSignalHandlerForThread();

		LOG(DBUG) << "Count " << count_ << " handled by thread: " << Thread::getCurrentThreadId();
		
		if (count_ > 20)
		{
			/*LOG(DBUG) << "Exit by SIGABRT";
			raise(SIGABRT);
			LOG(DBUG) << "Exit by SIGFPE";
			raise(SIGFPE);
			LOG(DBUG) << "Exit by SIGSEGV";
			raise(SIGSEGV);
			LOG(DBUG) << "Exit by SIGILL";
			raise(SIGILL);*/
			LOG(DBUG) << "Exit by SIGTERM";
			raise(SIGTERM);
			LOG(DBUG) << "Exit by Access Violation";
			/*int * p = nullptr;
			*p = 10;*/

			LOG(WARNING) << "Expected to have died by now...";

			Thread::sleep(1000);
		}
		return jobHasFinished;
	}

	void removedFromQueue()
	{
		
	}

 private:
	int count_;
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PoolJob)
};