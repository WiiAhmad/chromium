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

void Manager::AddFlagToMonitor(const char* flag_name, const char* description) {
  monitored_flags_.push_back({flag_name, description});
}

void Manager::MonitorDefaultFlags() {
  // Add default flags to monitor here
  AddFlagToMonitor(switches::kProfile, "User profile parameter");
  // Add more default flags as needed
}

void Manager::LogCommandLineFlags() {
  const base::CommandLine& command_line = *base::CommandLine::ForCurrentProcess();
  
  for (const auto& flag : monitored_flags_) {
    if (command_line.HasSwitch(flag.flag_name)) {
      std::string value = command_line.GetSwitchValueASCII(flag.flag_name);
      LOG(INFO) << "Flag Check: Found --" << flag.flag_name 
                << " [" << flag.description << "] with value: [" << value << "]";
    } else {
      LOG(INFO) << "Flag Check: Switch --" << flag.flag_name 
                << " [" << flag.description << "] not found.";
    }
  }
}

}  // namespace browser_manager