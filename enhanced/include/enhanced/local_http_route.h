#pragma once

// Private service routing, not a general TLS or certificate exception.
// These exact spellings never leave the process in the enhanced transport.
namespace iw5::local_http
{
template<class Char>
bool Equal(const Char *s, const char *expected)
{
    if (!s) return false;
    while (*expected) {
        if (*s++ != *expected++) return false;
    }
    return *s == 0;
}
template<class Char>
bool IsAuthRoute(const Char *url)
{
    return Equal(url,"https://127.0.0.1:443/auth/") ||
           Equal(url,"https://auth3.prod.demonware.net:443/auth/");
}
}
