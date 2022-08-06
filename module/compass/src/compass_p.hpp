#ifndef SRC_COMPASS_P_HPP_
#define SRC_COMPASS_P_HPP_

namespace Private
{

class Compass
{
public:


	~Compass();

	void calibrate();

	void setParams(float meanX, float meanY, float meanZ,
				    float W1, float W2, float div_E1, float div_E2);

	float getAngle(void);

	static Compass * instance();


private:
	Compass();

	float mean_x;
	float mean_y;
	float mean_z;
	float w1;
	float w2;
	float div_e1;
	float div_e2;
};


}// namespace Private

#endif // #define SRC_COMPASS_P_HPP_
