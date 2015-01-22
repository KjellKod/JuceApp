/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"
#include "g3log/g2logworker.hpp"
#include "g3log/g2log.hpp"

//==============================================================================
MainContentComponent::MainContentComponent()
{
    setSize (600, 400);
	startTimer(10);
}

MainContentComponent::~MainContentComponent()
{
}

void MainContentComponent::paint (Graphics& g)
{
    g.fillAll (Colour (0xff001F36));

    g.setFont (Font (16.0f));
    g.setColour (Colours::white);
    g.drawText ("Hello World!", getLocalBounds(), Justification::centred, true);
}

void MainContentComponent::resized()
{
    // This is called when the MainContentComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.
}

void MainContentComponent::timerCallback() 
{
	static int count = 0;

	LOG(DBUG) << count++;

	if (count > 400)
	{
		int * p = nullptr;
		*p = 10;
	}
}