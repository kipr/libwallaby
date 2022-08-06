#include "kipr/magneto/magneto.h"
#include "magneto_p.hpp"

using namespace kipr;
using namespace kipr::magneto;

short magneto_x()
{
	return magneto::magneto_x(nullptr);
}

short magneto_y()
{
	return magneto::magneto_y(nullptr);
}

short magneto_z()
{
	return magneto::magneto_z(nullptr);
}

int magneto_calibrate()
{
	return magneto::magneto_calibrate();
}
