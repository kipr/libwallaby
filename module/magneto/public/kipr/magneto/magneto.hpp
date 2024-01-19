#ifndef _KIPR_MAGNETO_MAGNETO_HPP_
#define _KIPR_MAGNETO_MAGNETO_HPP_

#include "kipr/sensor/sensor.hpp"

namespace kipr
{
	namespace magneto
	{
		class Magneto
		{
		public:
			static short x();
			static short y();
			static short z();
		};

		class MagnetoX : public sensor::Sensor<short>
		{
		public:
			virtual short value() const;
		};

		class MagnetoY : public sensor::Sensor<short>
		{
		public:
			virtual short value() const;
		};

		class MagnetoZ : public sensor::Sensor<short>
		{
		public:
			virtual short value() const;
		};
	}
}

#endif
