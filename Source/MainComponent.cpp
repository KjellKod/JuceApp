
#include "MainComponent.h"
#include "PoolJob.h"

MainComponent::MainComponent()
	: menuBar(this),
	  crashButton_("Crash"), 
	  startButton_("Start"),
	  intervalLabel_("intervalLabel_", "Timer interval(ms)"),
	  pool_(3)
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