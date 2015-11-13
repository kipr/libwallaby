/*
 * warn.hpp
 *
 *  Created on: Nov 13, 2015
 *      Author: Joshua Southerland
 */

#ifndef SRC_WARN_HPP_
#define SRC_WARN_HPP_



#include <cstdio>
#include "wallaby/compat.hpp"

#define MAX_MESSAGE_SIZE 256

#define WARN(x, ...) \
	do { \
		fprintf(stderr, "%s: warning: " x "\n", PRETTYFUNC, ##__VA_ARGS__); \
	} while(0)

#define PWARN(x, ...) \
	do { \
		char buffer[MAX_MESSAGE_SIZE]; \
		sprintf(buffer, "%s: warning: " x "\n", PRETTYFUNC, ##__VA_ARGS__); \
		perror(buffer); \
	} while(0)



#endif /* SRC_WARN_HPP_ */
