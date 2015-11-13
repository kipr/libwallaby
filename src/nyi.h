/*
 * nyi.h
 *
 *  Created on: Nov 13, 2015
 *      Author: Joshua Southerland
 */

#ifndef SRC_NYI_H_
#define SRC_NYI_H_

#include "wallaby/compat.hpp"

#define THIS_IS_NYI nyi(PRETTYFUNC);

#ifdef __cplusplus
extern "C" {
#endif

void nyi(const char *name);

#ifdef __cplusplus
}
#endif



#endif /* SRC_NYI_H_ */
