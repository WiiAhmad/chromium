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

void Manager::CheckProfileFlag() {
  const base::CommandLine& command_line = *base::CommandLine::ForCurrentProcess();
  
  if (command_line.HasSwitch(switches::kProfile)) {
    std::string profile_value = command_line.GetSwitchValueASCII(switches::kProfile);
    
    // Show custom text when profile flag is enabled
    LOG(INFO) << "================================================================";
    LOG(INFO) << "    PROFILE FLAG DETECTED: " << profile_value;
    LOG(INFO) << "    Browser session started with custom profile";
    LOG(INFO) << "    Profile data will be stored in: " << profile_value;
    LOG(INFO) << "    Security Mode: Enhanced";
    LOG(INFO) << "================================================================";
  }
}

}  // namespace browser_manager