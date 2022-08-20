#ifndef _KIPR_LOG_LOG_HPP_
#define _KIPR_LOG_LOG_HPP_

#include <sstream>
#include <memory>

namespace kipr
{
  namespace log
  {
    struct Flush {};

    extern const Flush flush;

    class Log;

    enum class Level : char
    {
      Fatal = '!',
      Error = 'E',
      Warning = 'W',
      Info = 'I',
      Debug = 'D',
      Verbose = 'V'
    };

    struct Location
    {
      std::string file;
      unsigned line;
      unsigned column;
    };

    class LogStream
    {
    public:
      LogStream(LogStream &&rhs);
      LogStream(Log &log, const Level level);
      ~LogStream();

      inline void operator <<(const Flush &_)
      {
        this->flush();
      }

      template<typename T>
      inline LogStream &operator <<(const T &t)
      {
        (*stream_) << t;
        return *this;
      }

    private:
      void flush();

      Log &log_;
      Level level_;
      bool flushed_;

      std::unique_ptr<std::ostringstream> stream_;
    };

    class Log
    {
    public:
      typedef std::shared_ptr<Log> Ptr;
      typedef std::shared_ptr<const Log> ConstPtr;

      Log(const std::string &module);
      Log(Log &&rhs);


      void log(const Level level, const std::string &message, const Location &location);
      void log(const Level level, const std::string &message);
      LogStream log(const Level level);

      inline void fatal(const std::string &message, const Location &location)
      {
        log(Level::Fatal, message, location);
      }

      inline void fatal(const std::string &message)
      {
        log(Level::Fatal, message);
      }

      inline LogStream fatal()
      {
        return log(Level::Fatal);
      }

      inline void error(const std::string &message, const Location &location)
      {
        log(Level::Error, message, location);
      }

      inline void error(const std::string &message)
      {
        log(Level::Error, message);
      }

      inline LogStream error()
      {
        return log(Level::Error);
      }

      inline void warning(const std::string &message, const Location &location)
      {
        log(Level::Warning, message, location);
      }

      inline void warning(const std::string &message)
      {
        log(Level::Warning, message);
      }

      inline LogStream warning()
      {
        return log(Level::Warning);
      }

      inline void info(const std::string &message, const Location &location)
      {
        log(Level::Info, message, location);
      }

      inline void info(const std::string &message)
      {
        log(Level::Info, message);
      }

      inline LogStream info()
      {
        return log(Level::Info);
      }

      inline void debug(const std::string &message, const Location &location)
      {
        log(Level::Debug, message, location);
      }

      inline void debug(const std::string &message)
      {
        log(Level::Debug, message);
      }

      inline LogStream debug()
      {
        return log(Level::Debug);
      }

      inline void verbose(const std::string &message, const Location &location)
      {
        log(Level::Verbose, message, location);
      }

      inline void verbose(const std::string &message)
      {
        log(Level::Verbose, message);
      }

      inline LogStream verbose()
      {
        return log(Level::Verbose);
      }

    private:
      Log(const Log &) = delete;

      std::string module_;
    };
  }
}

namespace std
{
  template<>
  struct hash<kipr::log::Level>
  {
    size_t operator()(const kipr::log::Level &level) const
    {
      return std::hash<unsigned>()(static_cast<unsigned>(level));
    }
  };
}

#endif