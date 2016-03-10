/*
 * ir.h
 *
 *  Created on: Nov 13, 2015
 *      Author: Joshua Southerland
 */

#ifndef INCLUDE_WALLABY_IR_H_
#define INCLUDE_WALLABY_IR_H_



#include "export.h"

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * \deprecated non present on the Wallaby
 */
EXPORT_SYM void ir_read();

/*!
 * Gets the sensed y rotation
 * \deprecated non present on the Wallaby
 */
EXPORT_SYM void ir_write();

#ifdef __cplusplus
}
#endif



#endif /* INCLUDE_WALLABY_IR_H_ */
