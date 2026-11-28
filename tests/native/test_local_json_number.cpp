#include "enhanced/local_json_number.h"
#include <cassert>
#include <cstring>
#include <initializer_list>
int main() {
    using iw5::local_json::ReadUnsignedMemberValue;
    unsigned long long v = 0;
    const auto parse = [&](const char *s) { return ReadUnsignedMemberValue(s, std::strlen(s), 0, &v); };
    assert(parse(":44,") && v == 44);
    assert(parse("\t\r\n : \t44\r\n}") && v == 44);
    assert(parse(":0}") && v == 0);
    assert(parse(":18446744073709551615}") && v == ~0ull);
    assert(parse(": \"44\",") && v == 44);
    assert(parse(":\"18446744073709551615\"}") && v == ~0ull);
    for (const char *s : {"", "44}", "::44}", ":44", ":44.0}", ":44e0}", ":44xyz}", ":-44}", ":044}",
                          ":18446744073709551616}", ":18446744073709551660}", ":\"44}", ":\"44 \"}",
                          ":\"44\"xyz}", ":\"-44\"}", ":\"044\"}", ":\"18446744073709551616\"}"}) {
        v = 123;
        assert(!parse(s) && v == 123);
    }
    assert(!ReadUnsignedMemberValue(nullptr, 0, 0, &v));
    assert(!ReadUnsignedMemberValue(":44}", 4, 5, &v));
    assert(!ReadUnsignedMemberValue(":44}", 4, 0, nullptr));
    assert(!ReadUnsignedMemberValue(":44}", 3, 0, &v));
}
