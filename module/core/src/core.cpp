#include "kipr/core/core.h"
#include "kipr/config.h"

const char *kipr_git_branch()
{
  return KIPR_GIT_BRANCH;
}

const char *kipr_git_commit_hash()
{
  return KIPR_GIT_COMMIT_HASH;
}

int kipr_version_major()
{
  return KIPR_VERSION_MAJOR;
}

int kipr_version_minor()
{
  return KIPR_VERSION_MINOR;
}

int kipr_version_patch()
{
  return KIPR_VERSION_PATCH;
}

const char *kipr_version()
{
  return KIPR_VERSION;
}

const char *kipr_build_datetime()
{
  return KIPR_BUILD_DATETIME;
}