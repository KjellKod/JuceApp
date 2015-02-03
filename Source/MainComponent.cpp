
#include "MainComponent.h"
#include "PoolJob.h"

void poolFunc(int fatalChoice, CrashState state)
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

MainComponent::MainComponent()
	: menuBar(this),
	  crashButton_("Crash"), 
	  startButton_("Start"),
	  intervalLabel_("intervalLabel_", "Timer interval(ms)"),
	  pool_(SystemStats::getNumCpus())
{
	addAndMakeVisible(&menuBar);

	intervalSlider_.setRange(0.0, 1000.0);
	intervalSlider_.setValue(DEFAULT_INTERVAL);
	intervalLabel_.setColour(Label::ColourIds::textColourId, Colours::white);

	crashButton_.addListener(this);
	startButton_.addListener(this);
	intervalSlider_.addListener(this);

	addAndMakeVisible(&crashButton_);
	addAndMakeVisible(&startButton_);
	addAndMakeVisible(&intervalSlider_);
	addAndMakeVisible(&intervalLabel_);

    setSize (490, 130);
}

MainComponent::~MainComponent()
{
}

void MainComponent::paint (Graphics& g)
{
    g.fillAll (Colour (Colours::darkgrey));
}

void MainComponent::resized()
{
	menuBar.setBounds(0, 0, getWidth(), 20);

	int textH = 24;
	int topMargin = 60;

	intervalLabel_.setBounds(10, topMargin, 100, textH);
	intervalSlider_.setBounds(112, topMargin, 200, textH);
	startButton_.setBounds(335, topMargin, 60, textH);
	crashButton_.setBounds(410, topMargin, 60, textH);
}

StringArray MainComponent::getMenuBarNames()
{
	const char* menuNames[] = { "Crash type",0 };
	 
	return StringArray(menuNames);
}

PopupMenu MainComponent::getMenuForIndex(int index, const String & name)
{
	PopupMenu menu;

	if (name == "Crash type")
	{
		menu.addItem(ABRT, "SIGABRT - Abnormal termination");
		menu.addItem(FPE, "SIGFPE - Floating-point error");
		menu.addItem(SEGV, "SIGSEGV - Illegal storage access");
		menu.addItem(ILL, "SIGILL - Illegal instruction");
		menu.addItem(TERM, "SIGTERM - Termination request");
		menu.addItem(DivisionByZero, "Division by zero");
		menu.addItem(IllegalPrintf, "Illegal printf");
		menu.addItem(OutOfBoundsArrayIndexing, "Out of bounds array indexing");
		menu.addItem(AccessViolation, "Access violation");
		menu.addItem(RaiseSIGABRTAndAccessViolation, "Raise SIGABRT and access violation");
	}
	
	return menu;
}

void MainComponent::menuItemSelected(int menuID, int index)
{
	switch (menuID)
	{
		case ABRT:
		case FPE:
		case SEGV:
		case ILL:
		case TERM:
		case DivisionByZero:
		case IllegalPrintf:
		case OutOfBoundsArrayIndexing:
		case AccessViolation:
		case RaiseSIGABRTAndAccessViolation:
			fatalChoice_ = menuID;
			state_ = State::Uncrashable;
			break;

		default:
			jassertfalse;
	}
}

void MainComponent::timerCallback() 
{
	//pool_.addJob(new PoolJob(fatalChoice_, state_), true);

	pool_.enqueue(poolFunc, fatalChoice_, state_);
}

void MainComponent::buttonClicked(Button* button)
{
	if (&crashButton_ == button)
		state_ = State::Crashable;

	if (&startButton_ == button)
	{
		if (isTimerRunning())
			stopTimer();

		state_ = State::Uncrashable;
		startTimer(timerInterval_);
	}
}

void MainComponent::sliderValueChanged(Slider* slider)
{
	state_ = State::Uncrashable;

	if (&intervalSlider_ == slider) 
	{
		double interval = floor( slider->getValue() );
		slider->setValue(interval);
		timerInterval_ = (int)interval;
	}
}