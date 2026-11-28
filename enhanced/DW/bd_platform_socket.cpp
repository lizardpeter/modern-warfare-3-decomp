struct BdSocketAddress;

// The DemonWare resolver returns the number of addresses written. Zero is its
// normal lookup failure result. The engine's own address parser is separate.
extern "C" unsigned BdGetHostByName(const char *, BdSocketAddress *, unsigned)
{
	return 0;
}
