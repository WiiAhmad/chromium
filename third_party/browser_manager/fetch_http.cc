#include "third_party/browser_manager/fetch_http.h"

#include "base/json/json_reader.h"
#include "base/logging.h"
#include "base/strings/stringprintf.h"
#include "net/base/url_util.h"
#include "services/network/public/cpp/resource_request.h"
#include "services/network/public/cpp/shared_url_loader_factory.h"
#include "services/network/public/cpp/simple_url_loader.h"
#include "services/network/public/mojom/url_response_head.mojom.h"

namespace browser_manager {

namespace {
// Server endpoint URL where profile data will be sent
const char kProfileEndpoint[] = "http://127.0.0.1/browser/profile";
// Maximum response size (5MB)
const int kMaxResponseSize = 5 * 1024 * 1024;
}  // namespace

// Static singleton instance
FetchHttp* FetchHttp::g_fetch_http_instance = nullptr;

FetchHttp* FetchHttp::GetInstance() {
  if (!g_fetch_http_instance)
    g_fetch_http_instance = new FetchHttp();
  return g_fetch_http_instance;
}

FetchHttp::FetchHttp() {
  // Get the default SharedURLLoaderFactory from the browser process
  url_loader_factory_ = network::SharedURLLoaderFactory::Create(
      content::BrowserContext::GetDefaultStoragePartition(
          content::BrowserContext::GetDefault())
          ->GetURLLoaderFactoryForBrowserProcess());
}

FetchHttp::~FetchHttp() {}

void FetchHttp::FetchProfileData(const std::string& profile_value,
                                FetchCallback callback) {
  // Construct URL with query parameter
  GURL url(kProfileEndpoint);
  url = net::AppendQueryParameter(url, "profile", profile_value);
  
  LOG(INFO) << "Fetching profile data from: " << url.spec();

  // Set up the network request
  auto resource_request = std::make_unique<network::ResourceRequest>();
  resource_request->url = url;
  resource_request->method = "GET";
  
  // Create URL loader with the request
  auto url_loader = network::SimpleURLLoader::Create(
      std::move(resource_request), 
      TRAFFIC_ANNOTATION_FOR_TESTS);
  
  // Set response size limit
  url_loader->SetAllowHttpErrorResults(true);
  url_loader->SetTimeoutDuration(base::TimeDelta::FromSeconds(30));
  url_loader->DownloadToString(
      url_loader_factory_.get(),
      base::BindOnce(&FetchHttp::OnProfileResponseReceived, 
                    base::Unretained(this),
                    std::move(callback)),
      kMaxResponseSize);
}

void FetchHttp::OnProfileResponseReceived(
    FetchCallback callback,
    std::unique_ptr<std::string> response_body) {
  
  if (!response_body) {
    LOG(ERROR) << "Failed to receive response body";
    // Create empty response to pass to callback
    ProfileResponse empty_response;
    std::move(callback).Run(empty_response);
    return;
  }
  
  LOG(INFO) << "Received profile response: " << *response_body;
  
  // Parse the JSON response
  ProfileResponse response = ParseJsonResponse(*response_body);
  
  // Execute the callback with the parsed response
  std::move(callback).Run(response);
}

ProfileResponse FetchHttp::ParseJsonResponse(const std::string& json_data) {
  ProfileResponse response;
  
  // Parse JSON string into dictionary value
  base::JSONReader::ValueWithError result = 
      base::JSONReader::ReadAndReturnValueWithError(
          json_data, base::JSONReader::Options::OPTIONS_ALLOW_TRAILING_COMMAS);
  
  if (!result.value) {
    LOG(ERROR) << "Failed to parse JSON response: " 
               << result.error_message;
    return response;
  }
  
  const base::Value* dict = result.value->GetIfDict();
  if (!dict) {
    LOG(ERROR) << "Response is not a JSON object";
    return response;
  }
  
  // Extract id and timestamp
  const std::string* id = dict->FindString("id");
  if (id) {
    response.id = *id;
    LOG(INFO) << "Profile response id: " << response.id;
  }
  
  const std::string* timestamp = dict->FindString("timestamp");
  if (timestamp) {
    response.timestamp = *timestamp;
    LOG(INFO) << "Profile response timestamp: " << response.timestamp;
  }
  
  return response;
}

}  // namespace browser_manager