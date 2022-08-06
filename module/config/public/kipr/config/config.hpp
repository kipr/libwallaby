#ifndef _KIPR_CONFIG_CONFIG_HPP_
#define _KIPR_CONFIG_CONFIG_HPP_

#include <string>
#include <map>
#include <vector>
#include "kipr/export/export.h"

namespace kipr
{
  namespace config
  {
    class EXPORT_SYM Config
    {
    public:
      Config();
      Config(const std::map<std::string, std::string> &config);

      static Config *load(const std::string &path);
      bool save(const std::string &path) const;

      void beginGroup(const std::string &group);
      void endGroup();
      void clearGroup();

      void clear();

      bool containsKey(const std::string &key) const;

      bool boolValue(const std::string &key) const;
      int intValue(const std::string &key) const;
      double doubleValue(const std::string &key) const;
      std::string stringValue(const std::string &key) const;

      void setValue(const std::string &key, const bool &value);
      void setValue(const std::string &key, const int &value);
      void setValue(const std::string &key, const double &value);
      void setValue(const std::string &key, const char *value);
      void setValue(const std::string &key, const std::string &value);

      Config values() const;
      void addValues(const Config &config);

    private:
      std::string safeKey(std::string key) const;
      std::string group() const;

      std::map<std::string, std::string> m_config;
      std::vector<std::string> m_groups;
      mutable std::string m_cachedGroup;
    };
  }
}

#endif
