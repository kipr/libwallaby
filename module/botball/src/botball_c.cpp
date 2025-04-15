#include "kipr/botball/botball.h"
#include "kipr/thread/thread.hpp"
#include "kipr/button/button.h"
#include "kipr/analog/analog.h"
#include "kipr/console/console.h"
#include <kipr/console/display.h>
#include "kipr/graphics/graphics.h"
#include "kipr/graphics/graphics_characters.h"
#include <stdio.h>
#include <string.h>
#include <kipr/time/time.h>

#include "kipr/core/cleanup.hpp"

#include <cstdlib>
#include <iostream>

#ifndef WIN32
#include <unistd.h>
#endif

using namespace kipr;

class ShutDownIn : public kipr::thread::Thread
{
public:
	ShutDownIn(double s)
			: m_s(s)
	{
	}

	~ShutDownIn()
	{
	}

	virtual void run()
	{
		const double start = seconds();
		msleep(m_s * 1000.0);
		const double end = seconds();
		std::cout << std::endl << "Shutdown after " << (end - start) << " seconds" << std::endl;
		// Note: Might want to move this to botui in the future.
		// Create::instance()->stop();
		// ToDo: Shutdown all threads
		kipr::core::cleanup(true);
		_exit(0);
	}

private:
	double m_s;
};

void shut_down_in(double s)
{
	static ShutDownIn *s_instance;
	if (s_instance)
	{
		std::cout << "Warning: shut_down_in already called once. Ignoring." << std::endl;
		return;
	}
	s_instance = new ShutDownIn(s);
	s_instance->start();
}