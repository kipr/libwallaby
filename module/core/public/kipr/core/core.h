#ifndef _KIPR_CORE_CORE_H_
#define _KIPR_CORE_CORE_H_

#ifdef __cplusplus
extern "C" {
#endif

const char *kipr_git_branch();

const char *kipr_git_commit_hash();

int kipr_version_major();

int kipr_version_minor();

int kipr_version_patch();

const char *kipr_version();

const char *kipr_build_datetime();

#ifdef __cplusplus
}
#endif

#endif