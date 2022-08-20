#include "kipr/log/log.hpp"

#include <unordered_map>
#include <mutex>
#include <iostream>

using namespace kipr;
using namespace kipr::log;

namespace
{
  std::mutex log_mutex;
}

const Flush kipr::log::flush {};

LogStream::LogStream(LogStream &&rhs)
  : log_(rhs.log_)
  , level_(rhs.level_)
  , flushed_(rhs.flushed_)
  , stream_(std::move(rhs.stream_))
{
  rhs.flushed_ = true;
}

LogStream::LogStream(Log &log, const Level level)
  : log_(log)
  , level_(level)
  , flushed_(false)
  , stream_(new std::ostringstream)
{
}

LogStream::~LogStream()
{
  flush();
}

void LogStream::flush()
{
  if (flushed_) return;
  log_.log(level_, stream_->str());
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
    << static_cast<char>(level)
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
    << static_cast<char>(level)
    << "]: "
    << message
    << std::endl;
}

LogStream Log::log(const Level level)
{
  return LogStream(*this, level);
}