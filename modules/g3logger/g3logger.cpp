#pragma once

// Your project must contain an AppConfig.h file with your project-specific settings in it,
// and your header search path must make it accessible to the module's files.
#include "AppConfig.h"
#include "g3logger.h"

#if JUCE_WINDOWS
#include "core/stacktrace_windows.cpp"
#include "core/crashhandler_windows.cpp"
#endif

#if JUCE_MAC
#include "core/crashhandler_unix.cpp"
#endif

#include "core/g2time.cpp"
#include "core/g2logworker.cpp"
#include "core/g2logmessagecapture.cpp"
#include "core/g2logmessage.cpp"
#include "core/g2loglevels.cpp"
#include "core/g2log.cpp"
#include "core/g2filesink.cpp"