#pragma once

// Bounded parsing of an unsigned integer immediately following a JSON member name.
// The Store auth serializer emits decimal strings for its unsigned fields.
// The caller locates the member name. This is not a general JSON validator.
namespace iw5::local_json {
using Size = __SIZE_TYPE__;
using UInt = unsigned long long;
inline bool Whitespace(char c) {
    return c == ' ' || c == '\t' || c == '\r' || c == '\n';
}
inline bool Digit(char c) { return c >= '0' && c <= '9'; }
inline bool ReadUnsignedMemberValue(const char *body, Size size, Size pos, UInt *value) {
    if (!body || !value || pos > size) return false;
    while (pos < size && Whitespace(body[pos])) ++pos;
    if (pos == size || body[pos++] != ':') return false;
    while (pos < size && Whitespace(body[pos])) ++pos;
    const bool quoted = pos < size && body[pos] == '"';
    if (quoted) ++pos;
    if (pos == size || !Digit(body[pos])) return false;
    if (body[pos] == '0' && pos + 1 < size && Digit(body[pos + 1])) return false;
    UInt n = 0;
    while (pos < size && Digit(body[pos])) {
        const UInt digit = static_cast<UInt>(body[pos++] - '0');
        if (n > (~UInt(0) - digit) / 10) return false;
        n = n * 10 + digit;
    }
    if (quoted && (pos == size || body[pos++] != '"')) return false;
    while (pos < size && Whitespace(body[pos])) ++pos;
    if (pos == size || (body[pos] != ',' && body[pos] != '}')) return false;
    *value = n;
    return true;
}
} // namespace iw5::local_json
