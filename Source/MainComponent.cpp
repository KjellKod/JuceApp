
#include "MainComponent.h"
#include "PoolJob.h"

MainContentComponent::MainContentComponent()
	: pool_(3)
{
    setSize (600, 400);
	startTimer(50);
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
   
}

void MainContentComponent::timerCallback() 
{
	static int count = 0;

	/*if (count > 400)
	{
		int * p = nullptr;
		*p = 10;
	}*/

	pool_.addJob( new PoolJob(count++), true);
}