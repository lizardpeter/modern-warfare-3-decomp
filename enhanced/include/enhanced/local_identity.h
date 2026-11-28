#pragma once

// Private IW5 compatibility identity namespace, not an Xbox account or a
// credential. Preserve the existing local DemonWare account value so existing
// local statistics/profile records keep the same key. This single-profile
// runtime intentionally does not derive its identity from the display name.
namespace iw5::local_identity
{
inline constexpr unsigned long long AccountId = 0x0009000000000001ull;
inline constexpr unsigned long long LocalId = 1ull;
}
