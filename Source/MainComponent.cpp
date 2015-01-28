
#include "MainComponent.h"
#include "PoolJob.h"

// from Scott Meyers book 
template<typename F, typename... Ts>
std::future<typename std::result_of<F(Ts...)>::type> reallyAsync(F&& f, Ts&&... params)
{
	return std::async(std::launch::async, std::forward<F>(f), std::forward<Ts>(params)...);
}

void fatalFunction()
{
	LOG(DBUG) << "Access violation coming";
	//raise(SIGSEGV);

	int * p = nullptr;
	*p = 10;
}

MainContentComponent::MainContentComponent()
	: pool_(3)
{
    setSize (600, 400);
	startTimer(250);  // was 50  ... now with longer interval the stack dump is written
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

	try
	{
		auto dieInNearFuture = reallyAsync(fatalFunction);
		//pool_.addJob(new PoolJob(count++), true);
	}
	catch (std::exception & e)
	{
		LOG(WARNING) << e.what();
		//dieInNearFuture.wait();
	}
	catch (...)
	{
		LOG(WARNING) << "Unknown exception";
		//dieInNearFuture.wait();
	}
	
}