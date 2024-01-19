#ifndef _KIPR_COMPASS_COMPASS_HPP_
#define _KIPR_COMPASS_COMPASS_HPP_

namespace kipr
{
  namespace compass
  {
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
  }
}

#endif
