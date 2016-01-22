#ifndef INCLUDE_WALLABY_BOTBALL_H_
#define INCLUDE_WALLABY_BOTBALL_H_

#include "export.h"
#include "vtable.h"

#ifdef __cplusplus
extern "C" {
#endif

VF EXPORT_SYM void shut_down_in(double s);

VF EXPORT_SYM void wait_for_light(int light_port_);

VFL

#ifdef __cplusplus
}
#endif

#endif
