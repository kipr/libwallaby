#include "kipr/magneto/magneto.hpp"
#include "magneto_p.hpp"

using namespace kipr;
using namespace kipr::magneto;

signed short Magneto::x()
{
	return magneto_x();
}

signed short Magneto::y()
{
	return magneto_y();
}

signed short Magneto::z()
{
	return magneto_z();
}

short MagnetoX::value() const
{
	return magneto_x();
}

short MagnetoY::value() const
{
	return magneto_y();
}

short MagnetoZ::value() const
{
	return magneto_z();
}
