
#include "../JuceLibraryCode/JuceHeader.h"
#include "MainComponent.h"



const String SAVED_WORK = "SavedWorkd.txt";

struct LogHolder
{
	LogHolder(const std::string prefix, const std::string folder)
		: log(g2::LogWorker::createWithDefaultLogger(prefix, folder))
	{
		g2::initializeLogging(log.worker.get());
		std::future<std::string> log_file_name = log.sink->call(&g2::FileSink::fileName);
	}

	~LogHolder() {}

	g2::DefaultFileLogger log;

}; // end class LogHolder


class JuceAppApplication  : public JUCEApplication
{
public:
    
    JuceAppApplication() {}

    const String getApplicationName() override       { return ProjectInfo::projectName; }
    const String getApplicationVersion() override    { return ProjectInfo::versionString; }
    bool moreThanOneInstanceAllowed() override       { return true; }

    void initialise (const String& commandLine) override
    {
		LookAndFeel::setDefaultLookAndFeel(&lookAndFeel_);

        mainWindow = new MainWindow (getApplicationName());

		File desktop = File::getSpecialLocation(File::SpecialLocationType::userDesktopDirectory);

		// delete old logs and saved work files
		juce::DirectoryIterator iter(juce::File(desktop), true, "*.log;*.txt");
		while (iter.next())
		{
			juce::File file(iter.getFile());
			String fileName = file.getFileName();
			if ((fileName.startsWith(getApplicationName()) || fileName.startsWith(SAVED_WORK) ) && !file.isDirectory())
				file.deleteFile();
		}

		logger_ = new LogHolder(getApplicationName().toStdString(), desktop.getFullPathName().toStdString());

		g2::setFatalExitHandler(&JuceAppApplication::onFatalError);
#if JUCE_WINDOWS
		g2::setFatalPreLoggingHook([]{__debugbreak(); });
#endif

		LOG(DBUG) << "Hello World!";
    }

	static void onFatalError(g2::FatalMessagePtr fatal_message)
	{
		 AlertWindow::showMessageBoxAsync(AlertWindow::WarningIcon,
										 "Unstable state detected!",
										 "Please save your work and exit this application");
		
		  LOG(WARNING) << "App has crashed";

		  File desktop = File::getSpecialLocation(File::SpecialLocationType::userDesktopDirectory);
		  String path = desktop.getFullPathName() + File::separatorString + SAVED_WORK;
		  File save(path);
		  if (!save.exists())
			  save.create();

		  save.appendText("Saving work");
		  for (int i = 0; i < 10; i++)
			 save.appendText(fatal_message.get()->toString());

		  // now ready to exit, instead of reinventing the wheel we do it the g3log way
		  g2::internal::pushFatalMessageToLogger(fatal_message);
	}

    void shutdown() override
	{
        mainWindow = nullptr; 

		LOG(DBUG) << "Goodbye World!";
		logger_ = nullptr;
    }


    void systemRequestedQuit() override
    {
        // This is called when the app is being asked to quit: you can ignore this
        // request and let the app carry on running, or call quit() to allow the app to close.
        quit();
    }

    void anotherInstanceStarted (const String& commandLine) override
    {
        // When another instance of the app is launched while this one is running,
        // this method is invoked, and the commandLine parameter tells you what
        // the other instance's command-line arguments were.
    }

    
    /*
        This class implements the desktop window that contains an instance of
        our MainComponent class.
    */
    class MainWindow    : public DocumentWindow
    {
    public:
        MainWindow (String name)  : DocumentWindow (name,
                                                    Colours::lightgrey,
                                                    DocumentWindow::allButtons)
        {
            setUsingNativeTitleBar (true);
            setContentOwned (new MainComponent(), true);

            centreWithSize (getWidth(), getHeight());
            setVisible (true);
        }

        void closeButtonPressed() override
        {
            // This is called when the user tries to close this window. Here, we'll just
            // ask the app to quit when this happens, but you can change this to do
            // whatever you need.
            JUCEApplication::getInstance()->systemRequestedQuit();
        }

        /* Note: Be careful if you override any DocumentWindow methods - the base
           class uses a lot of them, so by overriding you might break its functionality.
           It's best to do all your work in your content component instead, but if
           you really have to override any DocumentWindow methods, make sure your
           subclass also calls the superclass's method.
        */

    private:
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainWindow)
    };

private:
	LookAndFeel_V3 lookAndFeel_;
    ScopedPointer<MainWindow> mainWindow;
	ScopedPointer<LogHolder> logger_;
};

// This macro generates the main() routine that launches the app.
START_JUCE_APPLICATION (JuceAppApplication)
