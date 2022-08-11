#include "kipr/log/log.hpp"

#include <unordered_map>
#include <mutex>
#include <iostream>

using namespace kipr;
using namespace kipr::log;

namespace
{
  const static std::unordered_map<Level, std::string> LEVEL_MAPPINGS {
    { Level::Fatal, "!" },
    { Level::Error, "E" },
    { Level::Warning, "W" },
    { Level::Info, "I" },
    { Level::Debug, "D" },
    { Level::Verbose, "V" }
  };

  std::mutex log_mutex;
}

const Flush kipr::log::flush {};

LogStream::LogStream(Log &log, const Level level)
  : log_(log)
  , level_(level)
  , flushed_(false)
{
}

LogStream::~LogStream()
{
  flush();
}

void LogStream::flush()
{
  if (flushed_) return;
  log_.log(level_, stream_.str());
  flushed_ = true;
}

Log::Log(const std::string &module)
  : module_(module)
{
}

Log::Log(Log &&rhs)
  : module_(std::move(rhs.module_))
{
}

void Log::log(const Level level, const std::string &message, const Location &location)
{
  std::lock_guard<std::mutex> lock(log_mutex);
  std::cout
    << "["
    << module_
    << "] ["
    << LEVEL_MAPPINGS.at(level)
    << "] ["
    << location.file
    << ":"
    << location.line
    << ":"
    << location.column
    << "]: "
    << message
    << std::endl;
}

void Log::log(const Level level, const std::string &message)
{
  std::lock_guard<std::mutex> lock(log_mutex);
  std::cout
    << "["
    << module_
    << "] ["
    << LEVEL_MAPPINGS.at(level)
    << "]: "
    << message
    << std::endl;
}

LogStream Log::log(const Level level)
{
  return LogStream(*this, level);
}