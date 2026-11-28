struct BdSocketAddress
{
	unsigned char address[0x80];
	unsigned short family;
};
static_assert(__builtin_offsetof(BdSocketAddress, family) == 0x80);

// The DemonWare resolver returns the number of addresses written. Zero is its
// normal lookup failure result. The engine's own address parser is separate.
extern "C" unsigned BdGetHostByName(const char *, BdSocketAddress *, unsigned)
{
	return 0;
}

extern "C" int BdSendTo(int socket, const BdSocketAddress *destination, unsigned short,
                         const char *, int)
{
	if (socket == -1)
		return -10;
	return destination->family == 2 || destination->family == 23 ? -5 : -1;
}

extern "C" int BdConnectStream(int, const unsigned *, unsigned short)
{
	return -5;
}
