#ifndef THIRD_PARTY_BROWSER_MANAGER_MANAGER_H_
#define THIRD_PARTY_BROWSER_MANAGER_MANAGER_H_

#include "base/component_export.h"
#include <string>

namespace browser_manager {

// BrowserManager handles command line flag logging 
class COMPONENT_EXPORT(BROWSER_MANAGER) Manager {
 public:
  // Initializes the manager singleton
  static Manager* GetInstance();
  
  // Logs command line flags, especially profile flag if present
  void LogFlags();

 private:
  Manager();
  ~Manager();
};

}  // namespace browser_manager

#endif  // THIRD_PARTY_BROWSER_MANAGER_MANAGER_H_