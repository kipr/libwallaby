#include "kipr/magneto/magneto.hpp"
#include "magneto_p.hpp"

using namespace kipr;
using namespace kipr::magneto;

signed short Magneto::x()
{
	return magneto_x(nullptr);
}

signed short Magneto::y()
{
	return magneto_y(nullptr);
}

signed short Magneto::z()
{
	return magneto_z(nullptr);
}

short MagnetoX::value() const
{
	return magneto_x(nullptr);
}

short MagnetoY::value() const
{
	return magneto_y(nullptr);
}

short MagnetoZ::value() const
{
	return magneto_z(nullptr);
}
