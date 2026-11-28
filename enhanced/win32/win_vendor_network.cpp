extern "C" void BdCompleteWinHttpRequest(void *request, long result);

namespace
{
constexpr long cannotConnect = static_cast<long>(0x80072efdu);
}

extern "C" void BdBeginWinHttpRequest(void *, void *request)
{
	BdCompleteWinHttpRequest(request, cannotConnect);
}

extern "C" long WinHttpInitializeRequest(void *)
{
	return cannotConnect;
}
