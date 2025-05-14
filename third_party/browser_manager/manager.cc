#include "third_party/browser_manager/manager.h"

#include "base/command_line.h"
#include "base/logging.h"
#include "components/manager/manager_switches.h"

namespace browser_manager {

namespace {
// Singleton instance
Manager* g_manager_instance = nullptr;
}  // namespace

Manager* Manager::GetInstance() {
  if (!g_manager_instance)
    g_manager_instance = new Manager();
  return g_manager_instance;
}

Manager::Manager() {}

Manager::~Manager() {}

void Manager::LogFlags() {
  const base::CommandLine& command_line = *base::CommandLine::ForCurrentProcess();
  
  // Check for profile flag
  if (command_line.HasSwitch(switches::kProfile)) {
    std::string profile_value = command_line.GetSwitchValueASCII(switches::kProfile);
    LOG(INFO) << "Profile Flag: Found --" << switches::kProfile 
              << " with value: [" << profile_value << "]";
  } else {
    LOG(INFO) << "Profile Flag: Not found";
  }
}

}  // namespace browser_manager