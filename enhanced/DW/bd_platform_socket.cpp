// enhanced/DW/bd_platform_socket.cpp: DemonWare's platform sockets, official hosts unreachable, the local
// DemonWare reachable (docs/online.md).
#include "enhanced/local_demonware.h"

struct BdSocketAddress
{
	unsigned char address[0x80];
	unsigned short family;
};
static_assert(__builtin_offsetof(BdSocketAddress, family) == 0x80);

extern "C" __declspec(dllimport) int __stdcall connect(unsigned long long, const void *, int);
extern "C" __declspec(dllimport) unsigned short __stdcall htons(unsigned short);
extern "C" __declspec(dllimport) int __stdcall WSAGetLastError();

// 1404148F0: the auth and lobby host names resolve to the local DemonWare on 127.0.0.1, as the original writes an
// address (the IPv4 address only); every other name, the STUN hosts included, finds nothing (0), as an unknown
// host does.
extern "C" unsigned BdGetHostByName(const char *name, BdSocketAddress *out, unsigned max)
{
	if (!max || !out || !LocalDemonWare::IsServiceHost(name) || !LocalDemonWare::Start())
		return 0;
	*reinterpret_cast<unsigned *>(out->address) = LocalDemonWare::kLoopback;
	LocalDemonWare::Trace("resolve %s: 127.0.0.1", name);
	return 1;
}

extern "C" int BdSendTo(int socket, const BdSocketAddress *destination, unsigned short, const char *, int)
{
	if (socket == -1)
		return -10;
	return destination->family == 2 || destination->family == 23 ? -5 : -1;
}

// 140449870: only the lobby address the resolver handed out connects, to the local DemonWare's own port; the
// original's connect and its error mapping otherwise (non-blocking: would-block is -2).
extern "C" int BdConnectStream(int socket, const unsigned *address, unsigned short port)
{
	if (!address || *address != LocalDemonWare::kLoopback || port != LocalDemonWare::kLobbyPort ||
	    !LocalDemonWare::LobbyPort())
		return -5;
	struct
	{
		unsigned short family, port;
		unsigned address;
		unsigned char zero[8];
	} to = {2, htons(LocalDemonWare::LobbyPort()), LocalDemonWare::kLoopback, {}};
	if (connect((unsigned long long)(long long)socket, &to, sizeof(to)) >= 0)
		return 1;
	switch (WSAGetLastError())
	{
	case 0x2714:
		return -8;
	case 0x2726:
		return -9;
	case 0x2733:
		return -2;
	case 0x2738:
		return -6;
	case 0x2742:
	case 0x2743:
	case 0x2745:
	case 0x2746:
	case 0x274C:
	case 0x2751:
		return -5;
	default:
		return -1;
	}
}
