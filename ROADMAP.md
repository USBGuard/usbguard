# Roadmap

## 0.6.x

- Resolve GCC 4.8.x compatibility
 - Reimplement json parsing/serialization on top of json-c
- Revision of the logging code
 - Drop spdlog, implement a simple file/console/syslog logger
 - Allow custom logging handlers from third-party code
 - Use logging consistently in the code
- Investigate possibility of supporting comments in the rule file

## 0.7.x

- Per-interface authorization
- Block device signing and verification
- Support for USB authorization on user authentication (probably via
  polkit)

## 0.8.x

- USB device I/O monitoring + rule language extension 

## 0.9.x

## 1.0.0

- First stable release
- No backwards incompatible changes from this point. If some backwards
  incompatible changes will be required, they'll have to go into a 2.0.0
  branch.
