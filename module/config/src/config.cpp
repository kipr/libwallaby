#include "kipr/config/config.hpp"

#include <sstream>
#include <fstream>
#include <iostream>

#define SEP "/"
#define EQUAL ":"

using namespace kipr;
using namespace kipr::config;

Config::Config()
{
}

Config::Config(const std::map<std::string, std::string> &config)
	: m_config(config)
{
}

Config *Config::load(const std::string &path)
{
	std::ifstream file(path.c_str());
	if(!file.is_open()) return 0;
	std::map<std::string, std::string> ret;
	while(!file.eof()) {
		std::string first;
		std::string second;
		file >> first;
		file.ignore();
		getline(file, second);
		first = first.substr(0, first.find_first_of(EQUAL[0]));
		if(first.empty()) continue;
		ret[first] = second;
	}
	file.close();
	return new Config(ret);
}

bool Config::save(const std::string &path) const
{
	std::ofstream file(path.c_str());
	if(!file.is_open()) return false;
	std::map<std::string, std::string>::const_iterator it = m_config.begin();
	for(; it != m_config.end(); ++it) {
		file << it->first << EQUAL " " << it->second << std::endl;
	}
	file.close();
	return true;
}

void Config::beginGroup(const std::string &group)
{
	m_groups.push_back(safeKey(group));
	m_cachedGroup = "";
}

void Config::endGroup()
{
	if(m_groups.empty()) return;
	m_groups.pop_back();
	m_cachedGroup = "";
}

void Config::clearGroup()
{
	m_groups.clear();
}

void Config::clear()
{
	m_groups.clear();
	m_config.clear();
}

bool Config::containsKey(const std::string &key) const
{
	return m_config.find(group() + safeKey(key)) != m_config.end();
}

bool Config::boolValue(const std::string &key) const
{
	return intValue(key);
}

int Config::intValue(const std::string &key) const
{
	std::stringstream stream(stringValue(key));
	int value = 0;
	stream >> value;
	return value;
}

double Config::doubleValue(const std::string &key) const
{
	std::stringstream stream(stringValue(key));
	double value = 0;
	stream >> value;
	return value;
}

std::string Config::stringValue(const std::string &key) const
{
	std::map<std::string, std::string>::const_iterator cit = m_config.find(group() + safeKey(key));
	// std::cout << "Value for " << key << " = \"" << cit->second << "\"" << std::endl;
	// Invalid key
	if(cit == m_config.end()) return "";
	return cit->second;
}

void Config::setValue(const std::string &key, const bool &value)
{
	setValue(key, (int)(value ? 1 : 0));
}

void Config::setValue(const std::string &key, const int &value)
{
	std::stringstream stream;
	stream << value;
	setValue(key, stream.str());
}

void Config::setValue(const std::string &key, const double &value)
{
	std::stringstream stream;
	stream << value;
	setValue(key, stream.str());
}

void Config::setValue(const std::string &key, const char *value)
{
	setValue(key, std::string(value));
}

void Config::setValue(const std::string &key, const std::string &value)
{
	m_config[group() + safeKey(key)] = value;
}

Config Config::values() const
{
	Config ret;
	const std::string group = Config::group();
	std::map<std::string, std::string>::const_iterator it = m_config.begin();
	for(; it != m_config.end(); ++it) {
		const std::string &key = it->first;
		if(key.substr(0, group.size()).compare(group)) continue;
		ret.setValue(key.substr(group.size()), it->second);
	}
	return ret;
}

void Config::addValues(const Config &config)
{
	const std::string group = Config::group();
	Config values = config.values();
	std::map<std::string, std::string>::const_iterator it = values.m_config.begin();
	for(; it != values.m_config.end(); ++it) setValue(it->first, it->second);
}

std::string Config::safeKey(std::string key) const
{
	std::string::iterator it = key.begin();
	for(; it != key.end(); ++it) {
		if((*it) == EQUAL[0]) *it = '_';
		if((*it) == ' ') *it = '_';
		if((*it) == '\n') *it = '_';
		if((*it) == '\f') *it = '_';
	}
	return key;
}

std::string Config::group() const
{
	if(!m_cachedGroup.empty()) return m_cachedGroup;
	std::vector<std::string>::const_iterator it = m_groups.begin();
	for(; it != m_groups.end(); ++it) m_cachedGroup += (*it) + SEP;
	return m_cachedGroup;
}
