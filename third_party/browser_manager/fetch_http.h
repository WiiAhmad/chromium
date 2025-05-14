#ifndef THIRD_PARTY_BROWSER_MANAGER_FETCH_HTTP_H_
#define THIRD_PARTY_BROWSER_MANAGER_FETCH_HTTP_H_

#include "base/callback.h"
#include "base/component_export.h"
#include "services/network/public/cpp/simple_url_loader.h"
#include "url/gurl.h"

#include <memory>
#include <string>

namespace browser_manager {

// Response data structure for profile fetch
struct ProfileResponse {
  std::string id;
  std::string timestamp;
};

// Callback for when HTTP fetch completes
using FetchCallback = base::OnceCallback<void(const ProfileResponse&)>;

// FetchHttp handles HTTP requests for profile data
class COMPONENT_EXPORT(BROWSER_MANAGER) FetchHttp {
 public:
  // Get singleton instance
  static FetchHttp* GetInstance();

  // Sends HTTP request with profile value to specified endpoint
  // and processes the JSON response
  void FetchProfileData(const std::string& profile_value, FetchCallback callback);

 private:
  FetchHttp();
  ~FetchHttp();

  // Handle completion of URL loader
  void OnProfileResponseReceived(
      FetchCallback callback,
      std::unique_ptr<std::string> response_body);

  // Parses JSON response and extracts id and timestamp
  ProfileResponse ParseJsonResponse(const std::string& json_data);

  // Singleton instance
  static FetchHttp* g_fetch_http_instance;

  // Network context for making requests
  scoped_refptr<network::SharedURLLoaderFactory> url_loader_factory_;
};

}  // namespace browser_manager

#endif  // THIRD_PARTY_BROWSER_MANAGER_FETCH_HTTP_H_