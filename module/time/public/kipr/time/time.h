#ifndef _KIPR_TIME_TIME_H_
#define _KIPR_TIME_TIME_H_

#ifdef __cplusplus
extern "C" {
#endif

void msleep(long msecs);

unsigned long systime();

double seconds();

#ifdef __cplusplus
}
#endif

#endif
