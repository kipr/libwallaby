#include "kipr/analog/analog.h"
#include "analog_p.hpp"

using namespace kipr;
using namespace kipr::analog;

int analog(int port)
{
	return analog12(port);
}

int analog8(int port)
{
	return ::analog(port) >> 4;
}

int analog10(int port)
{
	return ::analog(port) >> 2;
}

int analog12(int port)
{
	return analog_value(port);
}

int analog_et(int port)
{
	return ::analog(port);
}

void set_analog_pullup(int port, int pullup)
{
	// Not available on the Wallaby
}

int get_analog_pullup(int port)
{
	// Not available on the Wallaby
	return 0;
}
