#include "enhanced/local_http_route.h"
#include <cassert>
#include <initializer_list>
int main() {
    using iw5::local_http::IsAuthRoute;
    assert(IsAuthRoute("https://127.0.0.1:443/auth/"));
    assert(IsAuthRoute(L"https://auth3.prod.demonware.net:443/auth/"));
    assert(!IsAuthRoute(static_cast<const wchar_t *>(nullptr)));
    for (const char *url : {"", "https://auth3.prod.demonware.net.evil:443/auth/",
                           "https://127.0.0.1:444/auth/", "https://127.0.0.1:443/auth/?q=1",
                           "https://127.0.0.1:443/other/auth/", "http://127.0.0.1:443/auth/",
                           "https://example.com:443/auth/"}) assert(!IsAuthRoute(url));
}
