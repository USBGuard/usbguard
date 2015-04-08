#pragma once
#include <build-config.h>
#include "ConfigFile.hpp"
#include <fstream>

namespace usbguard {
  class ConfigFilePrivate
  {
  public:
    ConfigFilePrivate(ConfigFile& p_instance);
    ~ConfigFilePrivate();

    void open(const String& path);
    void write();
    void close();

    void setSettingValue(const String& name, String& value);
    bool hasSettingValue(const String& name) const;
    const String& getSettingValue(const String& name) const;

  protected:
    void parse();
    bool checkNVPair(const String& name, const String& value) const;

  private:
    struct NVPair
    {
      String name;
      String value;
      size_t line_number;
    };

    ConfigFile& _p_instance;
    String _path;
    std::fstream _stream;
    StringVector _lines;
    StringKeyMap<NVPair> _settings;
    bool _dirty;
  };
}
