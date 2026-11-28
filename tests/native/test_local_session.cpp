#include "enhanced/online/local_session.h"
#include <cassert>
#include <cstring>

using namespace iw5::online;

// This fixture is deliberately compiled into the host test only. The shipping
// runtime does not link a synthetic entitlement provider.
class FixtureProvider final : public EntitlementProvider
{
public:
    EntitlementDecision decision = EntitlementDecision::Unavailable;
    unsigned calls = 0;
    unsigned char observedId = 0;
    EntitlementDecision Verify(const LocalProfile &profile,
                               const char *product) noexcept override
    {
        ++calls;
        observedId = profile.id[0];
        if (std::strcmp(product, "fixture-product") != 0)
            return EntitlementDecision::Denied;
        return decision;
    }
};

static LocalProfile Profile(unsigned char id)
{
    LocalProfile p{};
    p.id[0] = id;
    std::strcpy(p.displayName, "Local test player");
    return p;
}

int main()
{
    const auto a = Profile(1);
    const auto b = Profile(2);
    LocalProfile out{};
    Session unavailable;
    assert(unavailable.CheckAccess("fixture-product") == AccessResult::NoProfile);
    assert(unavailable.SetProfile(a));
    assert(unavailable.GetProfile(out) && out.id[0] == 1);
    assert(unavailable.CheckAccess("fixture-product") == AccessResult::Unavailable);

    FixtureProvider fixture;
    Session session(&fixture);
    assert(session.CheckAccess("fixture-product") == AccessResult::NoProfile);
    assert(fixture.calls == 0);
    assert(session.SetProfile(a));
    assert(session.CheckAccess(nullptr) == AccessResult::Unavailable);
    assert(session.CheckAccess("") == AccessResult::Unavailable);
    assert(fixture.calls == 0);
    assert(session.CheckAccess("fixture-product") == AccessResult::Unavailable);
    fixture.decision = EntitlementDecision::Denied;
    assert(session.CheckAccess("fixture-product") == AccessResult::Denied);
    fixture.decision = EntitlementDecision::Verified;
    assert(session.CheckAccess("fixture-product") == AccessResult::Verified);
    assert(session.CheckAccess("wrong-product") == AccessResult::Denied);
    assert(fixture.calls == 4 && fixture.observedId == 1);

    // Revocation is immediately visible; no cached authorization.
    fixture.decision = EntitlementDecision::Denied;
    assert(session.CheckAccess("fixture-product") == AccessResult::Denied);
    assert(session.SetProfile(b));
    assert(session.CheckAccess("fixture-product") == AccessResult::Denied);
    assert(fixture.observedId == 2);
    fixture.decision = static_cast<EntitlementDecision>(999);
    assert(session.CheckAccess("fixture-product") == AccessResult::Unavailable);

    // Bad profile data cannot preserve an earlier identity.
    assert(!session.SetProfile(Profile(0)));
    assert(!session.GetProfile(out) && out.id[0] == 0);
    assert(session.CheckAccess("fixture-product") == AccessResult::NoProfile);
    auto bad = a;
    std::memset(bad.displayName, 'a', sizeof(bad.displayName));
    assert(!session.SetProfile(bad));
    bad = a;
    bad.displayName[0] = '\n';
    assert(!session.SetProfile(bad));
    assert(session.SetProfile(b));
    session.ClearProfile();
    assert(!session.GetProfile(out) && out.displayName[0] == 0);
    assert(session.CheckAccess("fixture-product") == AccessResult::NoProfile);
}
