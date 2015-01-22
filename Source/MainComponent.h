#pragma once
#include "../JuceLibraryCode/JuceHeader.h"

class MainContentComponent   : public Component,
							   public Timer
{

 public:
    MainContentComponent();
    ~MainContentComponent();

    void paint (Graphics&);
    void resized();
	void timerCallback() override;

 private:
	ThreadPool pool_;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
};
