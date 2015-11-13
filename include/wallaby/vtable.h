/*
 * vtable.h
 *
 *  Created on: Nov 13, 2015
 *      Author: Joshua Southerland
 */

#ifndef INCLUDE_WALLABY_VTABLE_H_
#define INCLUDE_WALLABY_VTABLE_H_



#ifdef __cplusplus
extern "C" {
#endif

#include "export.h"

#define VF
#define VFL
#define VI
#define VIL
#define VH

#ifdef __GNUC__
#define POSSIBLY_UNUSED __attribute__ ((unused))
#else
#define POSSIBLY_UNUSED
#endif

#ifndef SWIG
#define VTABLE_FUNC_VOID(name, signature, args)                       \
  typedef void (*name##_func) signature;                              \
  EXPORT_SYM extern name##_func g_##name##_func;                      \
  EXPORT_SYM extern const name##_func g_##name##_func_default;        \
  static const char *const name##_signature = "void" #signature;      \
  POSSIBLY_UNUSED static void name signature { (*g_##name##_func) args;  }

#define VTABLE_FUNC(name, returnType, signature, args)                       \
  typedef returnType (*name##_func) signature;                               \
  EXPORT_SYM extern name##_func g_##name##_func;                             \
  EXPORT_SYM extern const name##_func g_##name##_func_default;               \
  static const char *const name##_signature = #returnType #signature;        \
  POSSIBLY_UNUSED static returnType name signature { return (*g_##name##_func) args;  }
#else
#define VTABLE_FUNC_VOID(name, signature, args)                       \
  typedef void (*name##_func) signature;                              \
  static void name signature;

#define VTABLE_FUNC(name, returnType, signature, args)                       \
  typedef returnType (*name##_func) signature;                               \
  static returnType name signature;
#endif

#define VTABLE_SET_DEFAULT(name, impl)                  \
  const name##_func g_##name##_func_default = &impl;    \
  name##_func g_##name##_func = g_##name##_func_default

#ifdef __cplusplus
}
#endif


#endif /* INCLUDE_WALLABY_VTABLE_H_ */
