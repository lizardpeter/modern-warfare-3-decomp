#pragma once

// Platform-neutral identity and entitlement interface. This is new infrastructure,
// not reconstructed Store code. A local profile is not proof of ownership.
namespace iw5::online
{
struct LocalProfile
{
    unsigned char id[16]{};
    char displayName[32]{};

    bool Valid() const noexcept
    {
        bool nonzero = false;
        for (auto b : id)
            nonzero = nonzero || b != 0;
        if (!nonzero || !displayName[0])
            return false;
        for (char c : displayName)
        {
            if (!c)
                return true;
            if (static_cast<unsigned char>(c) < 0x20 || c == 0x7f)
                return false;
        }
        return false;
    }
};

enum class EntitlementDecision { Unavailable, Denied, Verified };
enum class AccessResult { NoProfile, Unavailable, Denied, Verified };

// A trusted application-owned provider must verify the requested product for
// the supplied identity. Neither a profile nor its name is an entitlement.
class EntitlementProvider
{
public:
    virtual ~EntitlementProvider() = default;
    virtual EntitlementDecision Verify(const LocalProfile &profile,
                                       const char *product) noexcept = 0;
};

// Single-owner object. The provider is borrowed and must outlive this object.
// No authorization is cached: changing the identity or revoking a licence
// cannot leave a previously successful decision active.
class Session
{
public:
    explicit Session(EntitlementProvider *provider = nullptr) noexcept
        : provider_(provider) {}

    bool SetProfile(const LocalProfile &profile) noexcept
    {
        ClearProfile();
        if (!profile.Valid())
            return false;
        profile_ = profile;
        active_ = true;
        return true;
    }

    void ClearProfile() noexcept
    {
        active_ = false;
        profile_ = {};
    }

    bool GetProfile(LocalProfile &out) const noexcept
    {
        out = active_ ? profile_ : LocalProfile{};
        return active_;
    }

    AccessResult CheckAccess(const char *product) const noexcept
    {
        if (!active_)
            return AccessResult::NoProfile;
        if (!provider_ || !product || !*product)
            return AccessResult::Unavailable;
        switch (provider_->Verify(profile_, product))
        {
        case EntitlementDecision::Verified: return AccessResult::Verified;
        case EntitlementDecision::Denied: return AccessResult::Denied;
        default: return AccessResult::Unavailable;
        }
    }

private:
    EntitlementProvider *const provider_;
    LocalProfile profile_{};
    bool active_ = false;
};
} // namespace iw5::online
