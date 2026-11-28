// Test-only synthetic response for the XUser token result ABI.
// Never linked into iw5_enhanced, never registered with QueryApiImpl,
// never sent to a game or a network service.
#include "enhanced/gdk_local_queue.h"
#include <cassert>
#include <cstddef>
#include <cstdio>
#include <cstring>
#include <type_traits>

static u64 ticks;
u64 QueueNow() { return ticks; }
void QueueSleep(u32 n) { ticks += n; }
bool QueueSpawn(unsigned long (*)(void *), void *) { return false; }
void Gdk_Zero(void *p, size_t n) { std::memset(p, 0, n); }

namespace test_only {
struct TokenData {
    size_t tokenSize;
    size_t signatureSize;
    const char *token;
    const char *signature;
};
static_assert(sizeof(TokenData) == 32);
static_assert(offsetof(TokenData, tokenSize) == 0);
static_assert(offsetof(TokenData, signatureSize) == 8);
static_assert(offsetof(TokenData, token) == 16);
static_assert(offsetof(TokenData, signature) == 24);
static_assert(std::is_standard_layout<TokenData>::value);
constexpr char token[] = "TEST-ONLY-NOT-A-CREDENTIAL";
constexpr char signature[] = "TEST-ONLY-NOT-A-SIGNATURE";
constexpr size_t resultSize = sizeof(TokenData) + sizeof(token) + sizeof(signature);

enum class Phase { Idle, Pending, Complete, Canceled, Consumed };
struct MockOperation {
    Phase phase = Phase::Idle;
    XTaskQueueHandle queue = nullptr;
    XAsyncBlock *block = nullptr;
    unsigned callbacks = 0;

    static void Complete(void *p, bool canceled) {
        auto &op = *static_cast<MockOperation *>(p);
        op.phase = canceled ? Phase::Canceled : Phase::Complete;
        ++op.callbacks;
        if (op.block->callback) op.block->callback(op.block);
    }

    HRESULT Begin(XAsyncBlock *b) {
        if (!b || !b->queue || phase != Phase::Idle) return GDK_E_INVALIDARG;
        HRESULT hr = LocalQueueDuplicate(nullptr, b->queue, &queue);
        if (hr != GDK_S_OK) return hr;
        block = b;
        phase = Phase::Pending;
        hr = LocalQueueSubmit(nullptr, queue, 1, this, Complete);
        if (hr != GDK_S_OK) {
            phase = Phase::Idle;
            block = nullptr;
            LocalQueueClose(nullptr, queue);
            queue = nullptr;
        }
        return hr;
    }

    HRESULT Status() const {
        switch (phase) {
        case Phase::Pending: return GDK_E_PENDING;
        case Phase::Complete: return GDK_S_OK;
        case Phase::Canceled: return GDK_E_ABORT;
        default: return GDK_E_INVALIDARG;
        }
    }

    HRESULT Size(size_t *out) const {
        if (!out) return GDK_E_POINTER;
        *out = 0;
        HRESULT hr = Status();
        if (hr == GDK_S_OK) *out = resultSize;
        return hr;
    }

    HRESULT Result(size_t capacity, void *buffer, TokenData **out, size_t *used) {
        if (out) *out = nullptr;
        if (used) *used = 0;
        if (!out) return GDK_E_POINTER;
        HRESULT hr = Status();
        if (hr != GDK_S_OK) return hr;
        if (capacity < resultSize) return GDK_E_NOT_SUFFICIENT_BUFFER;
        if (!buffer) return GDK_E_POINTER;
        auto *bytes = static_cast<char *>(buffer);
        TokenData data{sizeof(token), sizeof(signature), bytes + sizeof(TokenData),
                       bytes + sizeof(TokenData) + sizeof(token)};
        std::memcpy(bytes, &data, sizeof(data));
        std::memcpy(bytes + sizeof(data), token, sizeof(token));
        std::memcpy(bytes + sizeof(data) + sizeof(token), signature, sizeof(signature));
        *out = static_cast<TokenData *>(buffer);
        if (used) *used = resultSize;
        phase = Phase::Consumed;
        return GDK_S_OK;
    }

    ~MockOperation() {
        assert(phase != Phase::Pending);
        if (queue) LocalQueueClose(nullptr, queue);
    }
};

void Notify(XAsyncBlock *b) { ++*static_cast<unsigned *>(b->context); }

void SuccessfulResult() {
    XTaskQueueHandle queue = nullptr;
    assert(LocalQueueCreate(nullptr, 0, 0, &queue) == GDK_S_OK);
    unsigned callbackCount = 0;
    XAsyncBlock block{};
    block.queue = queue;
    block.context = &callbackCount;
    block.callback = Notify;
    MockOperation op;
    assert(op.Begin(nullptr) == GDK_E_INVALIDARG);
    assert(op.Begin(&block) == GDK_S_OK);
    assert(op.Begin(&block) == GDK_E_INVALIDARG);
    size_t n = 99;
    assert(op.Size(&n) == GDK_E_PENDING && n == 0);
    assert(callbackCount == 0 && op.callbacks == 0);
    assert(LocalQueueDispatch(nullptr, queue, 1, 0));
    assert(callbackCount == 1 && op.callbacks == 1);
    assert(!LocalQueueDispatch(nullptr, queue, 1, 0));
    assert(op.Status() == GDK_S_OK);
    assert(op.Size(nullptr) == GDK_E_POINTER);
    assert(op.Size(&n) == GDK_S_OK && n == resultSize);

    alignas(TokenData) unsigned char buffer[resultSize + 8];
    std::memset(buffer, 0xa5, sizeof(buffer));
    TokenData *out = reinterpret_cast<TokenData *>(buffer);
    size_t used = 99;
    assert(op.Result(n-1, buffer, &out, &used) == GDK_E_NOT_SUFFICIENT_BUFFER);
    assert(!out && !used);
    for (unsigned char b : buffer) assert(b == 0xa5);
    assert(op.Result(n, nullptr, &out, &used) == GDK_E_POINTER);
    assert(op.Result(n, buffer, nullptr, &used) == GDK_E_POINTER);
    assert(op.Result(n, buffer, &out, &used) == GDK_S_OK);
    assert(used == n && out == reinterpret_cast<TokenData *>(buffer));
    TokenData data{};
    std::memcpy(&data, buffer, sizeof(data));
    assert(data.tokenSize == sizeof(token) && data.signatureSize == sizeof(signature));
    assert(data.token == reinterpret_cast<char *>(buffer) + sizeof(data));
    assert(data.signature == data.token + data.tokenSize);
    assert(std::strcmp(data.token, token) == 0);
    assert(std::strcmp(data.signature, signature) == 0);
    for (size_t i = n; i < sizeof(buffer); ++i) assert(buffer[i] == 0xa5);
    assert(op.Result(n, buffer, &out, &used) == GDK_E_INVALIDARG && !out && !used);
    LocalQueueClose(nullptr, queue);
}

void CanceledResult() {
    XTaskQueueHandle queue = nullptr;
    assert(LocalQueueCreate(nullptr, 0, 0, &queue) == GDK_S_OK);
    XAsyncBlock block{};
    block.queue = queue;
    MockOperation op;
    assert(op.Begin(&block) == GDK_S_OK);
    assert(LocalQueueTerminate(nullptr, queue, false, nullptr, nullptr) == GDK_S_OK);
    assert(LocalQueueDispatch(nullptr, queue, 1, 0));
    assert(op.Status() == GDK_E_ABORT && op.callbacks == 1);
    size_t n = 99;
    assert(op.Size(&n) == GDK_E_ABORT && n == 0);
    LocalQueueClose(nullptr, queue);
}
}

int main() {
    test_only::SuccessfulResult();
    test_only::CanceledResult();
    std::puts("PASS: test-only token response ABI, queue completion, bounds, cancellation and one-shot result");
}
