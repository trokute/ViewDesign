#include "ViewDesign/view/figure/image.h"

#include <ViewDesign/platform/curl/http.h>


namespace ViewDesign {


Image::Image(const std::string& url) : Image(Curl::DownloadBytes(url)) {}


} // namespace ViewDesign
