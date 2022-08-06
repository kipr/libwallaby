#ifndef SRC_COMPASS_HPP_
#define SRC_COMPASS_HPP_


class Compass
{
public:

	Compass();
	~Compass();

	static void calibrate();

	static void setParams(float meanX, float meanY, float meanZ,
				    float W1, float W2, float div_E1, float div_E2);

	static float getAngle(void);
};



#endif // #define SRC_COMPASS_HPP_
