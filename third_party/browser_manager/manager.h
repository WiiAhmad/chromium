#ifndef THIRD_PARTY_BROWSER_MANAGER_MANAGER_H_
#define THIRD_PARTY_BROWSER_MANAGER_MANAGER_H_

#include "base/component_export.h"
#include <string>
#include <vector>

namespace browser_manager {

// Structure to define a command line flag to be logged
struct CommandLineFlag {
  const char* flag_name;
  const char* description;
};

// BrowserManager handles common browser operations including flag checking and logging
class COMPONENT_EXPORT(BROWSER_MANAGER) Manager {
 public:
  // Initializes the manager singleton
  static Manager* GetInstance();
  
  // Checks and logs the specified command line flags
  void LogCommandLineFlags();

  // Adds a flag to be monitored
  void AddFlagToMonitor(const char* flag_name, const char* description);

  // Monitors predefined important flags
  void MonitorDefaultFlags();

 private:
  Manager();
  ~Manager();

  // List of flags to monitor and log
  std::vector<CommandLineFlag> monitored_flags_;
};

}  // namespace browser_manager

#endif  // THIRD_PARTY_BROWSER_MANAGER_MANAGER_H_