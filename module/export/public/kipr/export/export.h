#ifndef _KIPR_EXPORT_EXPORT_H_
#define _KIPR_EXPORT_EXPORT_H_

#ifdef WIN32
#define EXPORT_SYM __declspec(dllexport)
#else
#define EXPORT_SYM
#endif

#endif
