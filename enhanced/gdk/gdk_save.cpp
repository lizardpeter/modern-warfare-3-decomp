// enhanced/gdk/gdk_save.cpp: XGameSave as files in the game folder (docs/online.md).
//
// The original keeps the player's profile ("Gamerprofile_gdk": settings, binds, the online profile) in a GDK save
// container that the Xbox app syncs to the cloud. Here a container is a folder, players\<container>, and a blob is a
// file in it; an update writes each blob to a temporary file first and then replaces the old one, so a crash
// mid-save leaves the previous profile, as the GDK's atomic updates do.
#include "enhanced/gdk.h"

#include <windows.h>

WINBASEAPI HANDLE WINAPI CreateFileA(const char *name, DWORD access, DWORD share, void *security, DWORD disposition,
                                     DWORD flags, HANDLE templ);
WINBASEAPI BOOL WINAPI ReadFile(HANDLE file, void *buffer, DWORD bytes, DWORD *read, void *overlapped);
WINBASEAPI DWORD WINAPI GetFileSize(HANDLE file, DWORD *high);
WINBASEAPI BOOL WINAPI CreateDirectoryA(const char *path, void *security);
WINBASEAPI BOOL WINAPI MoveFileExA(const char *from, const char *to, DWORD flags);
WINBASEAPI BOOL WINAPI DeleteFileA(const char *path);

HRESULT Gdk_QueryInterface(GdkInterface *, const GdkGuid *, void **);
u32 Gdk_AddRef(GdkInterface *);
u32 Gdk_Release(GdkInterface *);
HRESULT Gdk_NotImplemented();

namespace
{
constexpr HRESULT kBlobNotFound = (HRESULT)0x80830008;  // E_GS_BLOB_NOT_FOUND
constexpr HRESULT kUpdateFailed = (HRESULT)0x80070052;  // HRESULT_FROM_WIN32(ERROR_CANNOT_MAKE): the files couldn't be written
constexpr size_t kMaxName = 128;

const char kProviderIdentity[] = "XGameSaveInitializeProviderAsync";
const char kReadIdentity[] = "XGameSaveReadBlobDataAsync";
const char kSubmitIdentity[] = "XGameSaveSubmitUpdateAsync";

struct Provider
{
	u32 tag;
};
Provider s_provider = {0x56525053};  // the one provider: the local player's saves

struct Container
{
	char path[MAX_PATH];  // ...\players\<container>\ (with the trailing backslash)
};

struct Write
{
	Write *next;
	char name[kMaxName];
	size_t size;
	u8 data[1];
};

struct Update
{
	Container *container;
	Write *writes;
};

// A container or blob name as a file name: letters, digits, '-', '_' and '.' (not leading) kept, the rest '_'.
bool SafeName(const char *name, char *out, size_t capacity)
{
	size_t n = Gdk_StrLen(name);
	if (!n || n >= capacity)
		return false;
	for (size_t i = 0; i < n; ++i)
	{
		char c = name[i];
		bool ok = (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c == '-' || c == '_' ||
		          (c == '.' && i);
		out[i] = ok ? c : '_';
	}
	out[n] = 0;
	return true;
}

bool Join(char *out, size_t capacity, const char *a, const char *b)
{
	size_t na = Gdk_StrLen(a), nb = Gdk_StrLen(b);
	if (na + nb + 1 > capacity)
		return false;
	Gdk_Copy(out, a, na);
	Gdk_Copy(out + na, b, nb + 1);
	return true;
}

// players\ in the game folder, created on first use.
bool PlayersFolder(char *out, size_t capacity)
{
	if (!Join(out, capacity, Gdk_GameFolder(), "players\\"))
		return false;
	CreateDirectoryA(out, nullptr);
	return true;
}

bool ReadWhole(const char *path, u8 **data, size_t *size)
{
	HANDLE f = CreateFileA(path, GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, 0, nullptr);
	if (f == INVALID_HANDLE_VALUE)
		return false;
	DWORD high = 0, low = GetFileSize(f, &high);
	bool ok = !high && low != 0xFFFFFFFF;
	u8 *buffer = ok ? static_cast<u8 *>(Gdk_Alloc(low)) : nullptr;
	DWORD read = 0;
	ok = buffer && (!low || (ReadFile(f, buffer, low, &read, nullptr) && read == low));
	CloseHandle(f);
	if (!ok)
	{
		Gdk_Free(buffer);
		return false;
	}
	*data = buffer;
	*size = low;
	return true;
}

HRESULT InitializeProviderAsync(GdkInterface *, void *, const char *, bool, XAsyncBlock *async)
{
	Provider *p = &s_provider;
	return Gdk_AsyncImmediate(async, kProviderIdentity, GDK_S_OK, &p, sizeof(p));
}

HRESULT InitializeProviderResult(GdkInterface *, XAsyncBlock *async, Provider **provider)
{
	*provider = nullptr;
	return Gdk_AsyncGetResult(async, kProviderIdentity, sizeof(*provider), provider, nullptr);
}

void CloseProvider(GdkInterface *, Provider *) {}

HRESULT CreateContainer(GdkInterface *, Provider *, const char *name, Container **out)
{
	*out = nullptr;
	char safe[kMaxName], folder[MAX_PATH];
	if (!SafeName(name, safe, sizeof(safe)) || !PlayersFolder(folder, sizeof(folder)))
		return GDK_E_INVALIDARG;
	Container *c = static_cast<Container *>(Gdk_Alloc(sizeof(Container)));
	if (!c)
		return GDK_E_OUTOFMEMORY;
	if (!Join(c->path, sizeof(c->path), folder, safe) || !Join(c->path, sizeof(c->path), c->path, "\\"))
	{
		Gdk_Free(c);
		return GDK_E_INVALIDARG;
	}
	*out = c;
	return GDK_S_OK;
}

void CloseContainer(GdkInterface *, Container *c)
{
	Gdk_Free(c);
}

// The read's result: count XGameSaveBlobs, then each blob's name and data, pointers into the caller's buffer.
struct BlobInfo
{
	const char *name;
	u32 size;
};
struct Blob
{
	BlobInfo info;
	u8 *data;
};

struct ReadOp
{
	HRESULT result;
	u32 count;
	size_t bytes;         // the result's size
	char **names;         // count names
	u8 **datas;
	size_t *sizes;
};

void FreeRead(ReadOp *r)
{
	for (u32 i = 0; i < r->count; ++i)
	{
		Gdk_Free(r->names[i]);
		Gdk_Free(r->datas[i]);
	}
	Gdk_Free(r->names);
	Gdk_Free(r->datas);
	Gdk_Free(r->sizes);
	Gdk_Free(r);
}

HRESULT ReadProvider(XAsyncOp op, const XAsyncProviderData *data)
{
	ReadOp *r = static_cast<ReadOp *>(data->context);
	switch (op)
	{
	case XAsyncOp::Begin:
		return Gdk_AsyncSchedule(data->async, 0);
	case XAsyncOp::DoWork:
		Gdk_AsyncComplete(data->async, r->result, r->result >= 0 ? r->bytes : 0);
		return GDK_S_OK;
	case XAsyncOp::GetResult:
	{
		Blob *blobs = static_cast<Blob *>(data->buffer);
		u8 *p = reinterpret_cast<u8 *>(blobs + r->count);
		for (u32 i = 0; i < r->count; ++i)
		{
			size_t n = Gdk_StrLen(r->names[i]) + 1;
			Gdk_Copy(p, r->names[i], n);
			blobs[i].info.name = reinterpret_cast<const char *>(p);
			p += (n + 7) & ~size_t(7);
			Gdk_Copy(p, r->datas[i], r->sizes[i]);
			blobs[i].info.size = (u32)r->sizes[i];
			blobs[i].data = p;
			p += (r->sizes[i] + 7) & ~size_t(7);
		}
		return GDK_S_OK;
	}
	case XAsyncOp::Cleanup:
		FreeRead(r);
		return GDK_S_OK;
	default:
		return GDK_S_OK;
	}
}

HRESULT ReadBlobDataAsync(GdkInterface *, Container *c, const char **names, u32 count, XAsyncBlock *async)
{
	ReadOp *r = static_cast<ReadOp *>(Gdk_Alloc(sizeof(ReadOp)));
	if (!r)
		return GDK_E_OUTOFMEMORY;
	r->names = static_cast<char **>(Gdk_Alloc(sizeof(char *) * (count ? count : 1)));
	r->datas = static_cast<u8 **>(Gdk_Alloc(sizeof(u8 *) * (count ? count : 1)));
	r->sizes = static_cast<size_t *>(Gdk_Alloc(sizeof(size_t) * (count ? count : 1)));
	r->bytes = sizeof(Blob) * count;
	if (!c || !names || !count)
		r->result = GDK_E_INVALIDARG;  // the game always names its blob
	for (u32 i = 0; r->result >= 0 && i < count; ++i)
	{
		char safe[kMaxName], path[MAX_PATH];
		if (!SafeName(names[i], safe, sizeof(safe)) || !Join(path, sizeof(path), c->path, safe))
		{
			r->result = GDK_E_INVALIDARG;
			break;
		}
		size_t n = Gdk_StrLen(names[i]) + 1;
		r->names[i] = static_cast<char *>(Gdk_Alloc(n));
		Gdk_Copy(r->names[i], names[i], n);
		r->count = i + 1;
		if (!ReadWhole(path, &r->datas[i], &r->sizes[i]))
		{
			r->result = kBlobNotFound;
			break;
		}
		r->bytes += ((n + 7) & ~size_t(7)) + ((r->sizes[i] + 7) & ~size_t(7));
	}
	return Gdk_AsyncBegin(async, r, kReadIdentity, nullptr, ReadProvider);
}

HRESULT ReadBlobDataResult(GdkInterface *, XAsyncBlock *async, size_t size, Blob *blobs, u32 *count)
{
	size_t needed = 0;
	HRESULT hr = Gdk_AsyncGetResultSize(async, &needed);
	if (count)
		*count = 0;
	if (hr < 0)
		return hr;
	hr = Gdk_AsyncGetResult(async, kReadIdentity, size, blobs, nullptr);
	if (hr >= 0 && count && needed)  // the blobs come first and the first name right after them
		*count = (u32)((reinterpret_cast<const u8 *>(blobs[0].info.name) - reinterpret_cast<u8 *>(blobs)) / sizeof(Blob));
	return hr;
}

HRESULT CreateUpdate(GdkInterface *, Container *c, const char *, Update **out)
{
	*out = nullptr;
	if (!c)
		return GDK_E_INVALIDARG;
	Update *u = static_cast<Update *>(Gdk_Alloc(sizeof(Update)));
	if (!u)
		return GDK_E_OUTOFMEMORY;
	u->container = c;
	*out = u;
	return GDK_S_OK;
}

void CloseUpdate(GdkInterface *, Update *u)
{
	if (!u)
		return;
	for (Write *w = u->writes; w;)
	{
		Write *next = w->next;
		Gdk_Free(w);
		w = next;
	}
	Gdk_Free(u);
}

HRESULT SubmitBlobWrite(GdkInterface *, Update *u, const char *name, const u8 *data, size_t size)
{
	if (!u || !name || (!data && size))
		return GDK_E_INVALIDARG;
	Write *w = static_cast<Write *>(Gdk_Alloc(sizeof(Write) + size));
	if (!w)
		return GDK_E_OUTOFMEMORY;
	if (!SafeName(name, w->name, sizeof(w->name)))
	{
		Gdk_Free(w);
		return GDK_E_INVALIDARG;
	}
	w->size = size;
	if (size)
		Gdk_Copy(w->data, data, size);
	w->next = u->writes;
	u->writes = w;
	return GDK_S_OK;
}

// Write every blob to <name>.tmp, then move each over the old file.
HRESULT Commit(Update *u)
{
	CreateDirectoryA(u->container->path, nullptr);
	for (Write *w = u->writes; w; w = w->next)
	{
		char path[MAX_PATH], tmp[MAX_PATH];
		if (!Join(path, sizeof(path), u->container->path, w->name) || !Join(tmp, sizeof(tmp), path, ".tmp"))
			return GDK_E_INVALIDARG;
		HANDLE f = CreateFileA(tmp, GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
		if (f == INVALID_HANDLE_VALUE)
			return kUpdateFailed;
		DWORD written = 0;
		bool ok = !w->size || (WriteFile(f, w->data, (DWORD)w->size, &written, nullptr) && written == w->size);
		CloseHandle(f);
		if (!ok)
		{
			DeleteFileA(tmp);
			return kUpdateFailed;
		}
	}
	for (Write *w = u->writes; w; w = w->next)
	{
		char path[MAX_PATH], tmp[MAX_PATH];
		Join(path, sizeof(path), u->container->path, w->name);
		Join(tmp, sizeof(tmp), path, ".tmp");
		constexpr DWORD replace = 1, writeThrough = 8;  // MOVEFILE_REPLACE_EXISTING | MOVEFILE_WRITE_THROUGH
		if (!MoveFileExA(tmp, path, replace | writeThrough))
			return kUpdateFailed;
	}
	return GDK_S_OK;
}

HRESULT SubmitUpdateAsync(GdkInterface *, Update *u, XAsyncBlock *async)
{
	return Gdk_AsyncImmediate(async, kSubmitIdentity, u ? Commit(u) : GDK_E_INVALIDARG, nullptr, 0);
}

HRESULT SubmitUpdateResult(GdkInterface *, XAsyncBlock *async)
{
	return Gdk_AsyncGetResult(async, kSubmitIdentity, 0, nullptr, nullptr);
}
}  // namespace

// Slots from analysis/iw5mp_ship/gdk/thunks.tsv (704c3f58).
static void *const s_gameSaveVtable[] = {
	(void *)Gdk_QueryInterface, (void *)Gdk_AddRef, (void *)Gdk_Release,
	(void *)Gdk_NotImplemented,         // 0x18 XGameSaveInitializeProvider
	(void *)InitializeProviderAsync,    // 0x20
	(void *)InitializeProviderResult,   // 0x28
	(void *)CloseProvider,              // 0x30
	(void *)Gdk_NotImplemented,         // 0x38
	(void *)Gdk_NotImplemented,         // 0x40
	(void *)Gdk_NotImplemented,         // 0x48
	(void *)Gdk_NotImplemented,         // 0x50
	(void *)Gdk_NotImplemented,         // 0x58
	(void *)Gdk_NotImplemented,         // 0x60
	(void *)Gdk_NotImplemented,         // 0x68
	(void *)Gdk_NotImplemented,         // 0x70
	(void *)Gdk_NotImplemented,         // 0x78
	(void *)CreateContainer,            // 0x80
	(void *)CloseContainer,             // 0x88
	(void *)Gdk_NotImplemented,         // 0x90
	(void *)Gdk_NotImplemented,         // 0x98
	(void *)Gdk_NotImplemented,         // 0xA0 XGameSaveReadBlobData
	(void *)ReadBlobDataAsync,          // 0xA8
	(void *)ReadBlobDataResult,         // 0xB0
	(void *)CreateUpdate,               // 0xB8
	(void *)CloseUpdate,                // 0xC0
	(void *)SubmitBlobWrite,            // 0xC8
	(void *)Gdk_NotImplemented,         // 0xD0
	(void *)Gdk_NotImplemented,         // 0xD8 XGameSaveSubmitUpdate
	(void *)SubmitUpdateAsync,          // 0xE0
	(void *)SubmitUpdateResult,         // 0xE8
};
GdkInterface g_gdkGameSave = {s_gameSaveVtable};
