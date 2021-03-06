#pragma once

#include "MainComponent.h"


typedef MainComponent::MenuIDs CrashType;
typedef MainComponent::State CrashState;

static void RaiseSIGABRT() 
{
	raise(SIGABRT);
	LOG(WARNING) << "Expected to have died by now...";
}

static void AccessViolation() 
{
	char* ptr = 0;
	LOG(INFO) << "Death by access violation is imminent";
	*ptr = 0;
	LOG(WARNING) << "Expected to have died by now...";
}

class PoolJob : public ThreadPoolJob
{

 public:
	PoolJob(int fatalChoice, CrashState state)
		: ThreadPoolJob("Threadpool Job"),
		  fatalChoice_(fatalChoice),
		  state_(state)
	{	
	}

	JobStatus runJob() override
	{
		Thread::sleep(30);

		if ( state_ == CrashState::Uncrashable)
			return jobNeedsRunningAgain;

		LOG(INFO) << "Handled by thread: " << Thread::getCurrentThreadId();

		switch (fatalChoice_)
		{
			case CrashType::ABRT:
				LOG(DBUG) << "Exit by SIGABRT";
				raise(SIGABRT);
				break;

			case CrashType::FPE:
				LOG(DBUG) << "Exit by SIGFPE";
				raise(SIGFPE);
				break;

			case CrashType::SEGV:
				LOG(DBUG) << "Exit by SIGSEGV";
				raise(SIGSEGV);
				break;

			case CrashType::ILL:
				LOGF(DBUG, "Exit by %s", "SIGILL");
				raise(SIGILL);
				break;

			case CrashType::TERM:
				LOG(DBUG) << "Exit by SIGTERM";
				raise(SIGTERM);
				break;

			case CrashType::DivisionByZero:
			{
				LOG(INFO) << "Division by zero is a big no-no";
				int gShouldBeZero = 0;
				int value = 3;
				auto test = value / gShouldBeZero;
				break;
			}

			case CrashType::IllegalPrintf:
				LOG(DBUG) << "Impending doom due to illeteracy";
				LOGF(DBUG, "2nd attempt at ILLEGAL PRINTF_SYNTAX %d EXAMPLE. %s %s", "hello", 1);
				break;

			case CrashType::OutOfBoundsArrayIndexing:
			{
				LOG(DBUG) << "Exit by out of bounds array index";
				std::vector<int> v;
				v[0] = 5;
				break;
			}
				
			case CrashType::AccessViolation:
			{
				char* ptr = 0;
				LOG(DBUG) << "Death by access violation is imminent";
				*ptr = 0;
				break;
			}
				
			case CrashType::RaiseSIGABRTAndAccessViolation:
			{
				LOG(DBUG) << "Exit by calling RaiseSIGABRT and AccessViolation";
				auto f1 = std::async(std::launch::async, &RaiseSIGABRT);
				auto f2 = std::async(std::launch::async, &AccessViolation);
				f1.wait();
				f2.wait();
				break;
			}
		}

		if (state_ == CrashState::Crashable)
			LOG(WARNING) << "Expected to have died by now...";

		return jobHasFinished;
	}

 private:
	int fatalChoice_;
	CrashState state_;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PoolJob)
};


static void poolFunc(int fatalChoice, CrashState state)
{
	Thread::sleep(30);

	if (state == CrashState::Uncrashable)
		return;

	LOG(INFO) << "Handled by thread: " << Thread::getCurrentThreadId();

	switch (fatalChoice)
	{
		case CrashType::ABRT:
			LOG(DBUG) << "Exit by SIGABRT";
			raise(SIGABRT);
			break;

		case CrashType::FPE:
			LOG(DBUG) << "Exit by SIGFPE";
			raise(SIGFPE);
			break;

		case CrashType::SEGV:
			LOG(DBUG) << "Exit by SIGSEGV";
			raise(SIGSEGV);
			break;

		case CrashType::ILL:
			LOGF(DBUG, "Exit by %s", "SIGILL");
			raise(SIGILL);
			break;

		case CrashType::TERM:
			LOG(DBUG) << "Exit by SIGTERM";
			raise(SIGTERM);
			break;

		case CrashType::DivisionByZero:
		{
			LOG(INFO) << "Division by zero is a big no-no";
			int gShouldBeZero = 0;
			int value = 3;
			auto test = value / gShouldBeZero;
			break;
		}

		case CrashType::IllegalPrintf:
			LOG(DBUG) << "Impending doom due to illeteracy";
			LOGF(DBUG, "2nd attempt at ILLEGAL PRINTF_SYNTAX %d EXAMPLE. %s %s", "hello", 1);
			break;

		case CrashType::OutOfBoundsArrayIndexing:
		{
			LOG(DBUG) << "Exit by out of bounds array index";
			std::vector<int> v;
			v[0] = 5;
			break;
		}

		case CrashType::AccessViolation:
		{
			char* ptr = 0;
			LOG(DBUG) << "Death by access violation is imminent";
			*ptr = 0;
			break;
		}

		case CrashType::RaiseSIGABRTAndAccessViolation:
		{
			LOG(DBUG) << "Exit by calling RaiseSIGABRT and AccessViolation";
			auto f1 = std::async(std::launch::async, &RaiseSIGABRT);
			auto f2 = std::async(std::launch::async, &AccessViolation);
			f1.wait();
			f2.wait();
			break;
		}
	}

	if (state == CrashState::Crashable)
		LOG(WARNING) << "Expected to have died by now...";
}
