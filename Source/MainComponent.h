#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "threadPool/ThreadPool.h"

class MainComponent : public Component,
					  public Timer,
					  public MenuBarModel,
					  public Button::Listener,
					  public Slider::Listener
					  
{
 
 public:
	enum MenuIDs
	{
		ABRT = 1000,
		FPE,
		SEGV,
		ILL,
		TERM,
		DivisionByZero,
		IllegalPrintf,
		OutOfBoundsArrayIndexing,
		AccessViolation,
		RaiseSIGABRTAndAccessViolation
	};

	enum State
	{
		Uncrashable,
		Crashable
	};

	const int DEFAULT_INTERVAL = 50;

 public:
    MainComponent();
    ~MainComponent();

	// Component
    void paint (Graphics&);
    void resized();

	// Timer
	void timerCallback() override;

	// MenuBarModel
	StringArray getMenuBarNames();
	PopupMenu getMenuForIndex(int index, const String & name);
	void menuItemSelected(int menuID, int index);

	// Listener callbacks.
	void buttonClicked(Button* button);
	void sliderValueChanged(Slider* slider);
	
 private:
	MenuBarComponent menuBar;
	TextButton crashButton_;
	TextButton startButton_;
	Slider intervalSlider_;
	Label intervalLabel_;

	//ThreadPool pool_;
	StdThreadPool pool_;
	int fatalChoice_ = 0;
	State state_ = State::Uncrashable;
	int timerInterval_ = DEFAULT_INTERVAL;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
