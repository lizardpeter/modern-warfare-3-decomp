typedef unsigned char   undefined;

typedef unsigned char    byte;
typedef unsigned int    dword;
typedef unsigned long long    GUID;
typedef pointer32 ImageBaseOffset32;

typedef long long    longlong;
typedef unsigned char    uchar;
typedef unsigned int    uint;
typedef unsigned long    ulong;
typedef unsigned long long    ulonglong;
typedef unsigned char    undefined1;
typedef unsigned short    undefined2;
typedef unsigned int    undefined4;
typedef unsigned long long    undefined8;
typedef unsigned short    ushort;
typedef short    wchar_t;
typedef unsigned short    word;
typedef undefined1 <NoType>;

typedef unsigned short    wchar16;
typedef ulonglong ulong64;

typedef longlong long64;

typedef undefined4 T_HRESULT;

typedef struct _OVERLAPPED _OVERLAPPED, *P_OVERLAPPED;

typedef ulong ULONG_PTR;

typedef union _union_518 _union_518, *P_union_518;

typedef void *HANDLE;

typedef struct _struct_519 _struct_519, *P_struct_519;

typedef void *PVOID;

typedef ulong DWORD;

struct _struct_519 {
    DWORD Offset;
    DWORD OffsetHigh;
};

union _union_518 {
    struct _struct_519 s;
    PVOID Pointer;
};

struct _OVERLAPPED {
    ULONG_PTR Internal;
    ULONG_PTR InternalHigh;
    union _union_518 u;
    HANDLE hEvent;
};

typedef struct _SECURITY_ATTRIBUTES _SECURITY_ATTRIBUTES, *P_SECURITY_ATTRIBUTES;

typedef void *LPVOID;

typedef int BOOL;

struct _SECURITY_ATTRIBUTES {
    DWORD nLength;
    LPVOID lpSecurityDescriptor;
    BOOL bInheritHandle;
};

typedef struct _SYSTEMTIME _SYSTEMTIME, *P_SYSTEMTIME;

typedef struct _SYSTEMTIME SYSTEMTIME;

typedef ushort WORD;

struct _SYSTEMTIME {
    WORD wYear;
    WORD wMonth;
    WORD wDayOfWeek;
    WORD wDay;
    WORD wHour;
    WORD wMinute;
    WORD wSecond;
    WORD wMilliseconds;
};

typedef struct _TIME_ZONE_INFORMATION _TIME_ZONE_INFORMATION, *P_TIME_ZONE_INFORMATION;

typedef struct _TIME_ZONE_INFORMATION *LPTIME_ZONE_INFORMATION;

typedef long LONG;

typedef wchar_t WCHAR;

struct _TIME_ZONE_INFORMATION {
    LONG Bias;
    WCHAR StandardName[32];
    SYSTEMTIME StandardDate;
    LONG StandardBias;
    WCHAR DaylightName[32];
    SYSTEMTIME DaylightDate;
    LONG DaylightBias;
};

typedef union _LARGE_INTEGER _LARGE_INTEGER, *P_LARGE_INTEGER;

typedef union _LARGE_INTEGER LARGE_INTEGER;

typedef DWORD (*LPPROGRESS_ROUTINE)(LARGE_INTEGER, LARGE_INTEGER, LARGE_INTEGER, LARGE_INTEGER, DWORD, DWORD, HANDLE, HANDLE, LPVOID);

typedef struct _struct_19 _struct_19, *P_struct_19;

typedef struct _struct_20 _struct_20, *P_struct_20;

typedef double LONGLONG;

struct _struct_20 {
    DWORD LowPart;
    LONG HighPart;
};

struct _struct_19 {
    DWORD LowPart;
    LONG HighPart;
};

union _LARGE_INTEGER {
    struct _struct_19 s;
    struct _struct_20 u;
    LONGLONG QuadPart;
};

typedef struct _WIN32_FIND_DATAA _WIN32_FIND_DATAA, *P_WIN32_FIND_DATAA;

typedef struct _FILETIME _FILETIME, *P_FILETIME;

typedef struct _FILETIME FILETIME;

typedef char CHAR;

struct _FILETIME {
    DWORD dwLowDateTime;
    DWORD dwHighDateTime;
};

struct _WIN32_FIND_DATAA {
    DWORD dwFileAttributes;
    FILETIME ftCreationTime;
    FILETIME ftLastAccessTime;
    FILETIME ftLastWriteTime;
    DWORD nFileSizeHigh;
    DWORD nFileSizeLow;
    DWORD dwReserved0;
    DWORD dwReserved1;
    CHAR cFileName[260];
    CHAR cAlternateFileName[14];
};

typedef struct _MEMORYSTATUS _MEMORYSTATUS, *P_MEMORYSTATUS;

typedef ULONG_PTR SIZE_T;

struct _MEMORYSTATUS {
    DWORD dwLength;
    DWORD dwMemoryLoad;
    SIZE_T dwTotalPhys;
    SIZE_T dwAvailPhys;
    SIZE_T dwTotalPageFile;
    SIZE_T dwAvailPageFile;
    SIZE_T dwTotalVirtual;
    SIZE_T dwAvailVirtual;
};

typedef DWORD (*PTHREAD_START_ROUTINE)(LPVOID);

typedef PTHREAD_START_ROUTINE LPTHREAD_START_ROUTINE;

typedef struct _OVERLAPPED *LPOVERLAPPED;

typedef struct _SECURITY_ATTRIBUTES *LPSECURITY_ATTRIBUTES;

typedef struct _WIN32_FIND_DATAA *LPWIN32_FIND_DATAA;

typedef struct _MEMORYSTATUS *LPMEMORYSTATUS;

typedef struct _EXCEPTION_POINTERS _EXCEPTION_POINTERS, *P_EXCEPTION_POINTERS;

typedef LONG (*PTOP_LEVEL_EXCEPTION_FILTER)(struct _EXCEPTION_POINTERS *);

typedef struct _EXCEPTION_RECORD _EXCEPTION_RECORD, *P_EXCEPTION_RECORD;

typedef struct _EXCEPTION_RECORD EXCEPTION_RECORD;

typedef EXCEPTION_RECORD *PEXCEPTION_RECORD;

typedef struct _CONTEXT _CONTEXT, *P_CONTEXT;

typedef struct _CONTEXT CONTEXT;

typedef CONTEXT *PCONTEXT;

typedef struct _FLOATING_SAVE_AREA _FLOATING_SAVE_AREA, *P_FLOATING_SAVE_AREA;

typedef struct _FLOATING_SAVE_AREA FLOATING_SAVE_AREA;

typedef uchar BYTE;

struct _FLOATING_SAVE_AREA {
    DWORD ControlWord;
    DWORD StatusWord;
    DWORD TagWord;
    DWORD ErrorOffset;
    DWORD ErrorSelector;
    DWORD DataOffset;
    DWORD DataSelector;
    BYTE RegisterArea[80];
    DWORD Cr0NpxState;
};

struct _CONTEXT {
    DWORD ContextFlags;
    DWORD Dr0;
    DWORD Dr1;
    DWORD Dr2;
    DWORD Dr3;
    DWORD Dr6;
    DWORD Dr7;
    FLOATING_SAVE_AREA FloatSave;
    DWORD SegGs;
    DWORD SegFs;
    DWORD SegEs;
    DWORD SegDs;
    DWORD Edi;
    DWORD Esi;
    DWORD Ebx;
    DWORD Edx;
    DWORD Ecx;
    DWORD Eax;
    DWORD Ebp;
    DWORD Eip;
    DWORD SegCs;
    DWORD EFlags;
    DWORD Esp;
    DWORD SegSs;
    BYTE ExtendedRegisters[512];
};

struct _EXCEPTION_RECORD {
    DWORD ExceptionCode;
    DWORD ExceptionFlags;
    struct _EXCEPTION_RECORD *ExceptionRecord;
    PVOID ExceptionAddress;
    DWORD NumberParameters;
    ULONG_PTR ExceptionInformation[15];
};

struct _EXCEPTION_POINTERS {
    PEXCEPTION_RECORD ExceptionRecord;
    PCONTEXT ContextRecord;
};

typedef void (*LPOVERLAPPED_COMPLETION_ROUTINE)(DWORD, DWORD, LPOVERLAPPED);

typedef void (*PTIMERAPCROUTINE)(LPVOID, DWORD, DWORD);

typedef struct _SYSTEMTIME *LPSYSTEMTIME;

typedef PTOP_LEVEL_EXCEPTION_FILTER LPTOP_LEVEL_EXCEPTION_FILTER;

typedef struct _MEMORY_BASIC_INFORMATION _MEMORY_BASIC_INFORMATION, *P_MEMORY_BASIC_INFORMATION;

struct _MEMORY_BASIC_INFORMATION {
    PVOID BaseAddress;
    PVOID AllocationBase;
    DWORD AllocationProtect;
    SIZE_T RegionSize;
    DWORD State;
    DWORD Protect;
    DWORD Type;
};

typedef CHAR *LPCSTR;

typedef struct _MEMORY_BASIC_INFORMATION *PMEMORY_BASIC_INFORMATION;

typedef LONG *PLONG;

typedef LARGE_INTEGER *PLARGE_INTEGER;

typedef CHAR *LPSTR;

typedef WCHAR *LPWSTR;

typedef WCHAR *LPCWSTR;

typedef struct IMAGE_DOS_HEADER IMAGE_DOS_HEADER, *PIMAGE_DOS_HEADER;

struct IMAGE_DOS_HEADER {
    char e_magic[2]; /* Magic number */
    word e_cblp; /* Bytes of last page */
    word e_cp; /* Pages in file */
    word e_crlc; /* Relocations */
    word e_cparhdr; /* Size of header in paragraphs */
    word e_minalloc; /* Minimum extra paragraphs needed */
    word e_maxalloc; /* Maximum extra paragraphs needed */
    word e_ss; /* Initial (relative) SS value */
    word e_sp; /* Initial SP value */
    word e_csum; /* Checksum */
    word e_ip; /* Initial IP value */
    word e_cs; /* Initial (relative) CS value */
    word e_lfarlc; /* File address of relocation table */
    word e_ovno; /* Overlay number */
    word e_res[4][4]; /* Reserved words */
    word e_oemid; /* OEM identifier (for e_oeminfo) */
    word e_oeminfo; /* OEM information; e_oemid specific */
    word e_res2[10][10]; /* Reserved words */
    dword e_lfanew; /* File address of new exe header */
    byte e_program[64]; /* Actual DOS program */
};

typedef struct tm tm, *Ptm;

struct tm {
    int tm_sec;
    int tm_min;
    int tm_hour;
    int tm_mday;
    int tm_mon;
    int tm_year;
    int tm_wday;
    int tm_yday;
    int tm_isdst;
};

typedef void (*_PHNDLR)(int);

typedef uint UINT_PTR;

typedef struct DotNetPdbInfo DotNetPdbInfo, *PDotNetPdbInfo;

struct DotNetPdbInfo {
    char signature[4];
    GUID guid;
    dword age;
    char pdbpath[48];
};

typedef struct _exception _exception, *P_exception;

struct _exception {
    int type;
    char *name;
    double arg1;
    double arg2;
    double retval;
};

typedef struct _strflt _strflt, *P_strflt;

struct _strflt {
    int sign;
    int decpt;
    int flag;
    char *mantissa;
};

typedef struct _flt _flt, *P_flt;

struct _flt {
    int flags;
    int nbytes;
    long lval;
    double dval;
};

typedef struct _strflt *STRFLT;

typedef enum enum_3272 {
    INTRNCVT_OK=0,
    INTRNCVT_OVERFLOW=1,
    INTRNCVT_UNDERFLOW=2
} enum_3272;

typedef enum enum_3272 INTRNCVT_STATUS;

typedef struct _flt *FLT;

typedef struct _FILETIME *LPFILETIME;

typedef int (*FARPROC)(void);

typedef HANDLE *LPHANDLE;

typedef DWORD *LPDWORD;

typedef struct HINSTANCE__ HINSTANCE__, *PHINSTANCE__;

struct HINSTANCE__ {
    int unused;
};

typedef HANDLE HGLOBAL;

typedef BOOL *LPBOOL;

typedef void *HGDIOBJ;

typedef struct HINSTANCE__ *HINSTANCE;

typedef void *LPCVOID;

typedef HINSTANCE HMODULE;

typedef HANDLE HLOCAL;

typedef long *LPLONG;

typedef uint UINT;

typedef struct IMAGE_DATA_DIRECTORY IMAGE_DATA_DIRECTORY, *PIMAGE_DATA_DIRECTORY;

struct IMAGE_DATA_DIRECTORY {
    ImageBaseOffset32 VirtualAddress;
    dword Size;
};

typedef struct IMAGE_OPTIONAL_HEADER32 IMAGE_OPTIONAL_HEADER32, *PIMAGE_OPTIONAL_HEADER32;

struct IMAGE_OPTIONAL_HEADER32 {
    word Magic;
    byte MajorLinkerVersion;
    byte MinorLinkerVersion;
    dword SizeOfCode;
    dword SizeOfInitializedData;
    dword SizeOfUninitializedData;
    ImageBaseOffset32 AddressOfEntryPoint;
    ImageBaseOffset32 BaseOfCode;
    ImageBaseOffset32 BaseOfData;
    pointer32 ImageBase;
    dword SectionAlignment;
    dword FileAlignment;
    word MajorOperatingSystemVersion;
    word MinorOperatingSystemVersion;
    word MajorImageVersion;
    word MinorImageVersion;
    word MajorSubsystemVersion;
    word MinorSubsystemVersion;
    dword Win32VersionValue;
    dword SizeOfImage;
    dword SizeOfHeaders;
    dword CheckSum;
    word Subsystem;
    word DllCharacteristics;
    dword SizeOfStackReserve;
    dword SizeOfStackCommit;
    dword SizeOfHeapReserve;
    dword SizeOfHeapCommit;
    dword LoaderFlags;
    dword NumberOfRvaAndSizes;
    struct IMAGE_DATA_DIRECTORY DataDirectory[16];
};

typedef struct IMAGE_SECTION_HEADER IMAGE_SECTION_HEADER, *PIMAGE_SECTION_HEADER;

typedef union Misc Misc, *PMisc;

typedef enum SectionFlags {
    IMAGE_SCN_TYPE_NO_PAD=8,
    IMAGE_SCN_RESERVED_0001=16,
    IMAGE_SCN_CNT_CODE=32,
    IMAGE_SCN_CNT_INITIALIZED_DATA=64,
    IMAGE_SCN_CNT_UNINITIALIZED_DATA=128,
    IMAGE_SCN_LNK_OTHER=256,
    IMAGE_SCN_LNK_INFO=512,
    IMAGE_SCN_RESERVED_0040=1024,
    IMAGE_SCN_LNK_REMOVE=2048,
    IMAGE_SCN_LNK_COMDAT=4096,
    IMAGE_SCN_GPREL=32768,
    IMAGE_SCN_MEM_16BIT=131072,
    IMAGE_SCN_MEM_PURGEABLE=131072,
    IMAGE_SCN_MEM_LOCKED=262144,
    IMAGE_SCN_MEM_PRELOAD=524288,
    IMAGE_SCN_ALIGN_1BYTES=1048576,
    IMAGE_SCN_ALIGN_2BYTES=2097152,
    IMAGE_SCN_ALIGN_4BYTES=3145728,
    IMAGE_SCN_ALIGN_8BYTES=4194304,
    IMAGE_SCN_ALIGN_16BYTES=5242880,
    IMAGE_SCN_ALIGN_32BYTES=6291456,
    IMAGE_SCN_ALIGN_64BYTES=7340032,
    IMAGE_SCN_ALIGN_128BYTES=8388608,
    IMAGE_SCN_ALIGN_256BYTES=9437184,
    IMAGE_SCN_ALIGN_512BYTES=10485760,
    IMAGE_SCN_ALIGN_1024BYTES=11534336,
    IMAGE_SCN_ALIGN_2048BYTES=12582912,
    IMAGE_SCN_ALIGN_4096BYTES=13631488,
    IMAGE_SCN_ALIGN_8192BYTES=14680064,
    IMAGE_SCN_LNK_NRELOC_OVFL=16777216,
    IMAGE_SCN_MEM_DISCARDABLE=33554432,
    IMAGE_SCN_MEM_NOT_CACHED=67108864,
    IMAGE_SCN_MEM_NOT_PAGED=134217728,
    IMAGE_SCN_MEM_SHARED=268435456,
    IMAGE_SCN_MEM_EXECUTE=536870912,
    IMAGE_SCN_MEM_READ=1073741824,
    IMAGE_SCN_MEM_WRITE=2147483648
} SectionFlags;

union Misc {
    dword PhysicalAddress;
    dword VirtualSize;
};

struct IMAGE_SECTION_HEADER {
    char Name[8];
    union Misc Misc;
    ImageBaseOffset32 VirtualAddress;
    dword SizeOfRawData;
    dword PointerToRawData;
    dword PointerToRelocations;
    dword PointerToLinenumbers;
    word NumberOfRelocations;
    word NumberOfLinenumbers;
    enum SectionFlags Characteristics;
};

typedef struct IMAGE_DEBUG_DIRECTORY IMAGE_DEBUG_DIRECTORY, *PIMAGE_DEBUG_DIRECTORY;

struct IMAGE_DEBUG_DIRECTORY {
    dword Characteristics;
    dword TimeDateStamp;
    word MajorVersion;
    word MinorVersion;
    dword Type;
    dword SizeOfData;
    dword AddressOfRawData;
    dword PointerToRawData;
};

typedef struct IMAGE_FILE_HEADER IMAGE_FILE_HEADER, *PIMAGE_FILE_HEADER;

struct IMAGE_FILE_HEADER {
    word Machine; /* 498 */
    word NumberOfSections;
    dword TimeDateStamp;
    dword PointerToSymbolTable;
    dword NumberOfSymbols;
    word SizeOfOptionalHeader;
    word Characteristics;
};

typedef struct IMAGE_NT_HEADERS32 IMAGE_NT_HEADERS32, *PIMAGE_NT_HEADERS32;

struct IMAGE_NT_HEADERS32 {
    char Signature[4];
    struct IMAGE_FILE_HEADER FileHeader;
    struct IMAGE_OPTIONAL_HEADER32 OptionalHeader;
};

typedef struct IMAGE_BASE_RELOCATION IMAGE_BASE_RELOCATION, *PIMAGE_BASE_RELOCATION;

struct IMAGE_BASE_RELOCATION {
    dword VirtualAddress;
    dword SizeOfBlock;
};

typedef struct IMAGE_THUNK_DATA32 IMAGE_THUNK_DATA32, *PIMAGE_THUNK_DATA32;

struct IMAGE_THUNK_DATA32 {
    dword StartAddressOfRawData;
    dword EndAddressOfRawData;
    dword AddressOfIndex;
    dword AddressOfCallBacks;
    dword SizeOfZeroFill;
    dword Characteristics;
};

typedef struct _iobuf _iobuf, *P_iobuf;

struct _iobuf {
    char *_ptr;
    int _cnt;
    char *_base;
    int _flag;
    int _file;
    int _charbuf;
    int _bufsiz;
    char *_tmpfname;
};

typedef struct _iobuf FILE;

typedef char *va_list;

typedef uint uintptr_t;

typedef enum _CertEvents {
    CE_D3DSwap=0,
    CE_D3DResume=1,
    CE_D3DSuspend=2
} _CertEvents;

typedef enum _PROXY_CLASS {
    ProxyFull=0,
    ProxyService=1,
    ProxyTree=2,
    ProxyDirectory=3
} _PROXY_CLASS;

typedef struct GPUFLOW_CONSTANTS GPUFLOW_CONSTANTS, *PGPUFLOW_CONSTANTS;

typedef union GPUFLOW_CONSTANTS_u_0 GPUFLOW_CONSTANTS_u_0, *PGPUFLOW_CONSTANTS_u_0;

typedef struct GPUFLOW_CONSTANTS_u_0_s_1 GPUFLOW_CONSTANTS_u_0_s_1, *PGPUFLOW_CONSTANTS_u_0_s_1;

struct GPUFLOW_CONSTANTS_u_0_s_1 {
    ulong BooleanConstant[8];
    ulong IntegerConstant[32];
};

union GPUFLOW_CONSTANTS_u_0 {
    ulong FlowConstant[40];
    struct GPUFLOW_CONSTANTS_u_0_s_1 _s_1;
};

struct GPUFLOW_CONSTANTS {
    union GPUFLOW_CONSTANTS_u_0 field0_0x0;
};

typedef enum <unnamed-tag_000010D7> {
    SPR_LR=8
} <unnamed-tag_000010D7>;

typedef struct GPU_WINDOWPACKET GPU_WINDOWPACKET, *PGPU_WINDOWPACKET;

struct GPU_WINDOWPACKET {
};

typedef enum _HAL_GPIO_ACTION {
    HalGpioOutputEnable=0,
    HalGpioOutputType=1,
    HalGpioAlternateFunction=2,
    HalGpioOutputData=3,
    HalGpioOutputDataSet=4,
    HalGpioOutputDataClear=5,
    HalGpioInputData=6,
    HalGpioInputInverting=7,
    HalGpioNoiseCancelWidth=8
} _HAL_GPIO_ACTION;

typedef union GPU_CLIPCONTROL GPU_CLIPCONTROL, *PGPU_CLIPCONTROL;

typedef struct GPU_CLIPCONTROL_s_0 GPU_CLIPCONTROL_s_0, *PGPU_CLIPCONTROL_s_0;

struct GPU_CLIPCONTROL_s_0 {
    undefined field0_0x0;
    ulong ClipPlaneMode:2;
    ulong ZNanRetain:1;
    ulong XyNanRetain:1;
    ulong VtxKillOr:1;
    ulong DisableClipErrDetect:1;
    ulong DxClipSpaceDef:1;
    ulong BoundaryEdgeFlagEnable:1;
    ulong ClipPlaneCullOnlyEnable:1;
    ulong ClipDisable:1;
    ulong WNanRetain:1;
    undefined field11_0x4;
    undefined field12_0x5;
    ulong ClipPlaneEnable5:1;
    ulong ClipPlaneEnable4:1;
    ulong ClipPlaneEnable3:1;
    ulong ClipPlaneEnable2:1;
    ulong ClipPlaneEnable1:1;
    ulong ClipPlaneEnable0:1;
};

union GPU_CLIPCONTROL {
    struct GPU_CLIPCONTROL_s_0 _s_0;
    ulong dword;
};

typedef enum _PixSysMonOpcode {
    PIX_SysMon_Nop=0,
    PIX_SysMon_PauseMarker=1,
    PIX_SysMon_ResumeMarker=2
} _PixSysMonOpcode;

typedef union GPU_CONTEXTMISC GPU_CONTEXTMISC, *PGPU_CONTEXTMISC;

typedef struct GPU_CONTEXTMISC_s_0 GPU_CONTEXTMISC_s_0, *PGPU_CONTEXTMISC_s_0;

struct GPU_CONTEXTMISC_s_0 {
    ulong SampleControl:2;
    ulong OutputScreenXY:1;
    ulong InstPredOptimize:1;
    ulong ParamGenPos:8;
    ulong TxCacheSelect:1;
    ulong YieldOptimize:1;
    ulong PerfCounterRef:1;
};

union GPU_CONTEXTMISC {
    struct GPU_CONTEXTMISC_s_0 _s_0;
    ulong dword;
};

typedef struct _D3DSWAP_STATUS _D3DSWAP_STATUS, *P_D3DSWAP_STATUS;

struct _D3DSWAP_STATUS {
    ulong Flags;
    ulong Swap;
    ulong EnqueuedCount;
};

typedef struct _D3DRING_BUFFER_PARAMETERS _D3DRING_BUFFER_PARAMETERS, *P_D3DRING_BUFFER_PARAMETERS;

struct _D3DRING_BUFFER_PARAMETERS {
    ulong Flags;
    ulong PrimarySize;
    void *pPrimary;
    ulong SecondarySize;
    void *pSecondary;
    ulong SegmentCount;
};

typedef enum GPUPERFEVENT_PA_SU {
    GPUPE_PERF_PAPC_PASX_REQ=0,
    GPUPE_PERF_PAPC_PASX_DISABLE_PIPE=1,
    GPUPE_PERF_PAPC_PASX_FIRST_VECTOR=2,
    GPUPE_PERF_PAPC_PASX_SECOND_VECTOR=3,
    GPUPE_PERF_PAPC_PASX_FIRST_DEAD=4,
    GPUPE_PERF_PAPC_PASX_SECOND_DEAD=5,
    GPUPE_PERF_PAPC_PASX_VTX_KILL_DISCARD=6,
    GPUPE_PERF_PAPC_PASX_VTX_NAN_DISCARD=7,
    GPUPE_PERF_PAPC_PA_INPUT_PRIM=8,
    GPUPE_PERF_PAPC_PA_INPUT_NULL_PRIM=9,
    GPUPE_PERF_PAPC_PA_INPUT_EVENT_FLAG=10,
    GPUPE_PERF_PAPC_PA_INPUT_FIRST_PRIM_SLOT=11,
    GPUPE_PERF_PAPC_PA_INPUT_END_OF_PACKET=12,
    GPUPE_PERF_PAPC_CLPR_CULL_PRIM=13,
    GPUPE_PERF_PAPC_CLPR_VVUCP_CULL_PRIM=14,
    GPUPE_PERF_PAPC_CLPR_VV_CULL_PRIM=15,
    GPUPE_PERF_PAPC_CLPR_UCP_CULL_PRIM=16,
    GPUPE_PERF_PAPC_CLPR_VTX_KILL_CULL_PRIM=17,
    GPUPE_PERF_PAPC_CLPR_VTX_NAN_CULL_PRIM=18,
    GPUPE_PERF_PAPC_CLPR_CULL_TO_NULL_PRIM=19,
    GPUPE_PERF_PAPC_CLPR_VVUCP_CLIP_PRIM=20,
    GPUPE_PERF_PAPC_CLPR_VV_CLIP_PRIM=21,
    GPUPE_PERF_PAPC_CLPR_UCP_CLIP_PRIM=22,
    GPUPE_PERF_PAPC_CLPR_POINT_CLIP_CANDIDATE=23,
    GPUPE_PERF_PAPC_CLPR_CLIP_PLANE_CNT_1=24,
    GPUPE_PERF_PAPC_CLPR_CLIP_PLANE_CNT_2=25,
    GPUPE_PERF_PAPC_CLPR_CLIP_PLANE_CNT_3=26,
    GPUPE_PERF_PAPC_CLPR_CLIP_PLANE_CNT_4=27,
    GPUPE_PERF_PAPC_CLPR_CLIP_PLANE_CNT_5_8=28,
    GPUPE_PERF_PAPC_CLPR_CLIP_PLANE_CNT_9_12=29,
    GPUPE_PERF_PAPC_CLPR_CLIP_PLANE_NEAR=30,
    GPUPE_PERF_PAPC_CLPR_CLIP_PLANE_FAR=31,
    GPUPE_PERF_PAPC_CLPR_CLIP_PLANE_LEFT=32,
    GPUPE_PERF_PAPC_CLPR_CLIP_PLANE_RIGHT=33,
    GPUPE_PERF_PAPC_CLPR_CLIP_PLANE_TOP=34,
    GPUPE_PERF_PAPC_CLPR_CLIP_PLANE_BOTTOM=35,
    GPUPE_PERF_PAPC_CLSM_NULL_PRIM=36,
    GPUPE_PERF_PAPC_CLSM_TOTALLY_VISIBLE_PRIM=37,
    GPUPE_PERF_PAPC_CLSM_CLIP_PRIM=38,
    GPUPE_PERF_PAPC_CLSM_CULL_TO_NULL_PRIM=39,
    GPUPE_PERF_PAPC_CLSM_OUT_PRIM_CNT_1=40,
    GPUPE_PERF_PAPC_CLSM_OUT_PRIM_CNT_2=41,
    GPUPE_PERF_PAPC_CLSM_OUT_PRIM_CNT_3=42,
    GPUPE_PERF_PAPC_CLSM_OUT_PRIM_CNT_4=43,
    GPUPE_PERF_PAPC_CLSM_OUT_PRIM_CNT_5_8=44,
    GPUPE_PERF_PAPC_CLSM_OUT_PRIM_CNT_9_13=45,
    GPUPE_PERF_PAPC_CLSM_NON_TRIVIAL_CULL=46,
    GPUPE_PERF_PAPC_SU_INPUT_PRIM=47,
    GPUPE_PERF_PAPC_SU_INPUT_CLIP_PRIM=48,
    GPUPE_PERF_PAPC_SU_INPUT_NULL_PRIM=49,
    GPUPE_PERF_PAPC_SU_ZERO_AREA_CULL_PRIM=50,
    GPUPE_PERF_PAPC_SU_BACK_FACE_CULL_PRIM=51,
    GPUPE_PERF_PAPC_SU_FRONT_FACE_CULL_PRIM=52,
    GPUPE_PERF_PAPC_SU_POLYMODE_FACE_CULL=53,
    GPUPE_PERF_PAPC_SU_POLYMODE_BACK_CULL=54,
    GPUPE_PERF_PAPC_SU_POLYMODE_FRONT_CULL=55,
    GPUPE_PERF_PAPC_SU_POLYMODE_INVALID_FILL=56,
    GPUPE_PERF_PAPC_SU_OUTPUT_PRIM=57,
    GPUPE_PERF_PAPC_SU_OUTPUT_CLIP_PRIM=58,
    GPUPE_PERF_PAPC_SU_OUTPUT_NULL_PRIM=59,
    GPUPE_PERF_PAPC_SU_OUTPUT_EVENT_FLAG=60,
    GPUPE_PERF_PAPC_SU_OUTPUT_FIRST_PRIM_SLOT=61,
    GPUPE_PERF_PAPC_SU_OUTPUT_END_OF_PACKET=62,
    GPUPE_PERF_PAPC_SU_OUTPUT_POLYMODE_FACE=63,
    GPUPE_PERF_PAPC_SU_OUTPUT_POLYMODE_BACK=64,
    GPUPE_PERF_PAPC_SU_OUTPUT_POLYMODE_FRONT=65,
    GPUPE_PERF_PAPC_SU_OUT_CLIP_POLYMODE_FACE=66,
    GPUPE_PERF_PAPC_SU_OUT_CLIP_POLYMODE_BACK=67,
    GPUPE_PERF_PAPC_SU_OUT_CLIP_POLYMODE_FRONT=68,
    GPUPE_PERF_PAPC_PASX_REQ_IDLE=69,
    GPUPE_PERF_PAPC_PASX_REQ_BUSY=70,
    GPUPE_PERF_PAPC_PASX_REQ_STALLED=71,
    GPUPE_PERF_PAPC_PASX_REC_IDLE=72,
    GPUPE_PERF_PAPC_PASX_REC_BUSY=73,
    GPUPE_PERF_PAPC_PASX_REC_STARVED_SX=74,
    GPUPE_PERF_PAPC_PASX_REC_STALLED=75,
    GPUPE_PERF_PAPC_PASX_REC_STALLED_POS_MEM=76,
    GPUPE_PERF_PAPC_PASX_REC_STALLED_CCGSM_IN=77,
    GPUPE_PERF_PAPC_CCGSM_IDLE=78,
    GPUPE_PERF_PAPC_CCGSM_BUSY=79,
    GPUPE_PERF_PAPC_CCGSM_STALLED=80,
    GPUPE_PERF_PAPC_CLPRIM_IDLE=81,
    GPUPE_PERF_PAPC_CLPRIM_BUSY=82,
    GPUPE_PERF_PAPC_CLPRIM_STALLED=83,
    GPUPE_PERF_PAPC_CLPRIM_STARVED_CCGSM=84,
    GPUPE_PERF_PAPC_CLIPSM_IDLE=85,
    GPUPE_PERF_PAPC_CLIPSM_BUSY=86,
    GPUPE_PERF_PAPC_CLIPSM_WAIT_CLIP_VERT_ENGH=87,
    GPUPE_PERF_PAPC_CLIPSM_WAIT_HIGH_PRI_SEQ=88,
    GPUPE_PERF_PAPC_CLIPSM_WAIT_CLIPGA=89,
    GPUPE_PERF_PAPC_CLIPSM_WAIT_AVAIL_VTE_CLIP=90,
    GPUPE_PERF_PAPC_CLIPSM_WAIT_CLIP_OUTSM=91,
    GPUPE_PERF_PAPC_CLIPGA_IDLE=92,
    GPUPE_PERF_PAPC_CLIPGA_BUSY=93,
    GPUPE_PERF_PAPC_CLIPGA_STARVED_VTE_CLIP=94,
    GPUPE_PERF_PAPC_CLIPGA_STALLED=95,
    GPUPE_PERF_PAPC_CLIP_IDLE=96,
    GPUPE_PERF_PAPC_CLIP_BUSY=97,
    GPUPE_PERF_PAPC_SU_IDLE=98,
    GPUPE_PERF_PAPC_SU_BUSY=99,
    GPUPE_PERF_PAPC_SU_STARVED_CLIP=100,
    GPUPE_PERF_PAPC_SU_STALLED_SC=101
} GPUPERFEVENT_PA_SU;

typedef enum GPUPERFEVENT_HZ {
    GPUPE_WRC_1VAL_QUADS=0,
    GPUPE_WRC_2VAL_QUADS=1,
    GPUPE_WRC_3VAL_QUADS=2,
    GPUPE_WRC_4VAL_QUADS=3,
    GPUPE_HZ_WR_BUSY=4,
    GPUPE_HZ_SPARE0=5,
    GPUPE_RDC_TILE_HIT=6,
    GPUPE_RDC_STAGE3_STALL=7,
    GPUPE_HZ_SPARE1=8,
    GPUPE_HZ_SPARE2=9,
    GPUPE_QUADS_KEPT=10,
    GPUPE_QUADS_ZCULL=11,
    GPUPE_QUADS_SCULL=12,
    GPUPE_QUADS_SZCULL=13,
    GPUPE_HZ_RE_BUSY=14,
    GPUPE_BC_SCLK_COUNT=15,
    GPUPE_BC_HZ_VALID0=16,
    GPUPE_BC_HZ_VALID1=17,
    GPUPE_BC_HZ_VALID2=18,
    GPUPE_BC_HZ_VALID3=19,
    GPUPE_SC_SCLK_COUNT=20,
    GPUPE_SC_HZ_COARSE_SND=21,
    GPUPE_HZ_SC_COARSE_HLD=22,
    GPUPE_HZ_SC_HIER_SND=23,
    GPUPE_SC_HZ_HIER_HLD=24,
    GPUPE_HZ_BC_TILE_SND=25,
    GPUPE_BC_HZ_TILE_HLD=26,
    GPUPE_SC_STALL_HZ=27,
    GPUPE_BC_STALL_HZ=28,
    GPUPE_EVENT_STALL=29,
    GPUPE_SC_STARVE_HZ_BUSY=30,
    GPUPE_SC_STARVE_HZ_IDLE=31
} GPUPERFEVENT_HZ;

typedef struct XAUDIOSOURCEFORMAT XAUDIOSOURCEFORMAT, *PXAUDIOSOURCEFORMAT;

typedef union XAUDIOSOURCEFORMAT_u_4 XAUDIOSOURCEFORMAT_u_4, *PXAUDIOSOURCEFORMAT_u_4;

typedef struct XAUDIOSOURCEFORMAT_u_4_s_0 XAUDIOSOURCEFORMAT_u_4_s_0, *PXAUDIOSOURCEFORMAT_u_4_s_0;

typedef struct XAUDIOSOURCEFORMAT_u_4_s_1 XAUDIOSOURCEFORMAT_u_4_s_1, *PXAUDIOSOURCEFORMAT_u_4_s_1;

typedef struct XAUDIOXMAFORMAT XAUDIOXMAFORMAT, *PXAUDIOXMAFORMAT;

struct XAUDIOXMAFORMAT {
    ulong SampleRate;
    uchar ChannelCount;
    uchar DecodeBufferSize;
};

struct XAUDIOSOURCEFORMAT_u_4_s_0 {
    uchar NumStreams;
    struct XAUDIOXMAFORMAT Stream[6];
};

struct XAUDIOSOURCEFORMAT_u_4_s_1 {
    uchar ChannelCount;
    ulong SampleRate;
};

union XAUDIOSOURCEFORMAT_u_4 {
    struct XAUDIOSOURCEFORMAT_u_4_s_0 _s_0;
    struct XAUDIOSOURCEFORMAT_u_4_s_1 _s_1;
};

struct XAUDIOSOURCEFORMAT {
    uchar SampleType;
    union XAUDIOSOURCEFORMAT_u_4 field1_0x4;
};

typedef union D1SCL_COEF_RAM_SELECT D1SCL_COEF_RAM_SELECT, *PD1SCL_COEF_RAM_SELECT;

typedef struct <unnamed-type-bitfields> <unnamed-type-bitfields>, *P<unnamed-type-bitfields>;

typedef union <unnamed-type-bitfields>_u_0 <unnamed-type-bitfields>_u_0, *P<unnamed-type-bitfields>_u_0;

typedef struct <unnamed-type-bitfields>_u_0_s_0 <unnamed-type-bitfields>_u_0_s_0, *P<unnamed-type-bitfields>_u_0_s_0;

struct <unnamed-type-bitfields>_u_0_s_0 {
    undefined field0_0x0;
    undefined field1_0x1;
    uint D1SCL_C_RAM_FILTER_TYPE:2;
};

union <unnamed-type-bitfields>_u_0 {
    struct <unnamed-type-bitfields>_u_0_s_0 _s_0;
    uint D1SCL_C_RAM_PHASE:4;
    uint D1SCL_C_RAM_TAP_PAIR_IDX:3;
};

struct <unnamed-type-bitfields> {
    union <unnamed-type-bitfields>_u_0 field0_0x0;
};

union D1SCL_COEF_RAM_SELECT {
    struct <unnamed-type-bitfields> bitfields;
    struct <unnamed-type-bitfields> bits;
    uint u32All;
    int i32All;
    float f32All;
};

typedef enum <unnamed-tag_00001143> {
    SPR_TSRL=128
} <unnamed-tag_00001143>;

typedef enum _D3DTESSPRIMITIVETYPE {
    D3DTPT_LINELIST=2,
    D3DTPT_LINESTRIP=3,
    D3DTPT_TRIANGLELIST=4,
    D3DTPT_TRIANGLEFAN=5,
    D3DTPT_TRIANGLESTRIP=6,
    D3DTPT_QUADLIST=13,
    D3DTPT_LINEPATCH=16,
    D3DTPT_TRIPATCH=17,
    D3DTPT_QUADPATCH=18,
    D3DTPT_FORCE_DWORD=255
} _D3DTESSPRIMITIVETYPE;

typedef struct _D3DDEVICE_CREATION_PARAMETERS _D3DDEVICE_CREATION_PARAMETERS, *P_D3DDEVICE_CREATION_PARAMETERS;

typedef enum _D3DDEVTYPE {
    D3DDEVTYPE_HAL=1,
    D3DDEVTYPE_COMMAND_BUFFER=2,
    D3DDEVTYPE_FORCE_DWORD=255
} _D3DDEVTYPE;

typedef struct HWND__ HWND__, *PHWND__;

struct _D3DDEVICE_CREATION_PARAMETERS {
    uint AdapterOrdinal;
    enum _D3DDEVTYPE DeviceType;
    struct HWND__ *hFocusWindow;
    ulong BehaviorFlags;
};

struct HWND__ {
    int unused;
};

typedef union DCP_DEBUG DCP_DEBUG, *PDCP_DEBUG;


/* WARNING! conflicting data type names: /iw5mp_xenon.pdb/DCP_DEBUG/<unnamed-type-bitfields> - /iw5mp_xenon.pdb/D1SCL_COEF_RAM_SELECT/<unnamed-type-bitfields> */

union DCP_DEBUG {
    struct <unnamed-type-bitfields> bitfields;
    struct <unnamed-type-bitfields> bits;
    uint u32All;
    int i32All;
    float f32All;
};

typedef enum _RUNTIME_FUNCTION_TYPE {
    FunctionTypeSaveMillicode=0,
    FunctionTypeNoHandler=1,
    FunctionTypeRestoreMillicode=2,
    FunctionTypeHandler=3
} _RUNTIME_FUNCTION_TYPE;

typedef enum VC_PERFCOUNT_SELECT {
    RG_VERTICES=0,
    RG_CLAMPED=1,
    RG_L2_REQUEST=2,
    RG_L1_REQUEST=3,
    RG_MEGAFETCH=4,
    RG_END_OF_GROUP=5,
    RG_CONFLICT=6,
    RG_DWORDS_REQUESTED=7,
    RG_SPARE0=8,
    RG_SPARE1=9,
    CC_STALLS=10,
    CC_HITS=11,
    CC_MISSES=12,
    CC_SECTOR_MISSES=13,
    CC_L2B_STALLS=14,
    CC_MI_STALLS=15,
    CC_MULTICYCLE_STALLS=16,
    CC_EVEN_ALLOC_STALLS=17,
    CC_ODD_ALLOC_STALLS=18,
    CC_EVEN_BUSY_STALLS=19,
    CC_ODD_BUSY_STALLS=20,
    CC_IN_FIFO_EMPTY=21,
    CC_IN_FIFO_FULL=22,
    CC_FREEZE=23,
    CC_SPARE0=24,
    CC_SPARE1=25,
    CC_SPARE2=26,
    CC_SPARE3=27,
    CC_SPARE4=28,
    CC_SPARE5=29,
    MI_REQUESTS_TO_MH=30,
    MI_AGP_REQUESTS=31,
    MI_LATENCY_BITS_4_0=32,
    MI_LATENCY_BITS_9_5=33,
    MI_LATENCY_BITS_14_10=34,
    MI_LATENCY_BITS_17_15=35,
    MI_INPUT_FIFOS_FULL=36,
    MI_INPUT_FIFOS_0_FULL=37,
    MI_INPUT_FIFOS_1_FULL=38,
    MI_SPARE1=39,
    RP_SP_DATA_VALID=40,
    RP_STALLED=41,
    RP_SPARE0=42,
    RP_SPARE1=43,
    RP_SPARE2=44,
    RP_SPARE3=45,
    RP_SPARE4=46,
    RP_SPARE5=47,
    RP_SPARE6=48,
    RP_SPARE7=49,
    DC_NUM_VALIDS=50,
    DC_SPARE0=51,
    DC_SPARE1=52,
    DC_SPARE2=53,
    DC_SPARE3=54,
    DC_SPARE4=55,
    DC_SPARE5=56,
    DC_SPARE6=57,
    DC_SPARE7=58,
    DC_SPARE8=59,
    SQ_VC_SEND=60,
    VC_STARVED_IDLE=61,
    VC_BUSY=62,
    VC_IDLE=63,
    VC_SPARE0=64,
    VC_SPARE1=65,
    VC_SPARE2=66,
    VC_SPARE3=67,
    VC_SPARE4=68,
    VC_SPARE5=69
} VC_PERFCOUNT_SELECT;

typedef struct _D3DVIEWPORT9 _D3DVIEWPORT9, *P_D3DVIEWPORT9;

struct _D3DVIEWPORT9 {
    ulong X;
    ulong Y;
    ulong Width;
    ulong Height;
    float MinZ;
    float MaxZ;
};

typedef enum _D3DVERTEXBLENDFLAGS {
    D3DVBF_0WEIGHTS=0,
    D3DVBF_DISABLE=0,
    D3DVBF_1WEIGHTS=1,
    D3DVBF_2WEIGHTS=2,
    D3DVBF_3WEIGHTS=3,
    D3DVBF_FORCE_DWORD=255,
    D3DVBF_TWEENING=255
} _D3DVERTEXBLENDFLAGS;

typedef struct _CERTIFICATE_REVOCATION_LIST_HEADER _CERTIFICATE_REVOCATION_LIST_HEADER, *P_CERTIFICATE_REVOCATION_LIST_HEADER;

typedef struct _XECRYPT_SIG _XECRYPT_SIG, *P_XECRYPT_SIG;

typedef union _CERTIFICATE_REVOCATION_LIST_HEADER_u_320 _CERTIFICATE_REVOCATION_LIST_HEADER_u_320, *P_CERTIFICATE_REVOCATION_LIST_HEADER_u_320;

typedef struct _CERTIFICATE_REVOCATION_BOX_DATA _CERTIFICATE_REVOCATION_BOX_DATA, *P_CERTIFICATE_REVOCATION_BOX_DATA;

struct _CERTIFICATE_REVOCATION_BOX_DATA {
    ulong64 DateStamp;
    uchar Padding[7];
    uchar UpdateSequence;
};

union _CERTIFICATE_REVOCATION_LIST_HEADER_u_320 {
    struct _CERTIFICATE_REVOCATION_BOX_DATA BoxData;
    uchar BoxDataAsUCHAR[16];
};

struct _XECRYPT_SIG {
    ulong64 aqwPad[28];
    uchar bOne;
    uchar abSalt[10];
    uchar abHash[20];
    uchar bEnd;
};

struct _CERTIFICATE_REVOCATION_LIST_HEADER {
    ulong Signer;
    uchar ConsoleID[5];
    uchar Padding[3];
    uchar Digest[20];
    struct _XECRYPT_SIG Signature;
    uchar Nonce[16];
    uchar Key[16];
    union _CERTIFICATE_REVOCATION_LIST_HEADER_u_320 field7_0x140;
};

typedef enum _D3DHIZFUNC {
    D3DHIZFUNC_DEFAULT=0,
    D3DHIZFUNC_LESS_EQUAL=1,
    D3DHIZFUNC_GREATER_EQUAL=2,
    D3DHIZFUNC_FORCE_DWORD=255
} _D3DHIZFUNC;

typedef struct _D3DCAPS9 _D3DCAPS9, *P_D3DCAPS9;

typedef struct _D3DVSHADERCAPS2_0 _D3DVSHADERCAPS2_0, *P_D3DVSHADERCAPS2_0;

typedef struct _D3DPSHADERCAPS2_0 _D3DPSHADERCAPS2_0, *P_D3DPSHADERCAPS2_0;

struct _D3DVSHADERCAPS2_0 {
    ulong Caps;
    int DynamicFlowControlDepth;
    int NumTemps;
    int StaticFlowControlDepth;
};

struct _D3DPSHADERCAPS2_0 {
    ulong Caps;
    int DynamicFlowControlDepth;
    int NumTemps;
    int StaticFlowControlDepth;
    int NumInstructionSlots;
};

struct _D3DCAPS9 {
    enum _D3DDEVTYPE DeviceType;
    uint AdapterOrdinal;
    ulong Caps;
    ulong Caps2;
    ulong Caps3;
    ulong PresentationIntervals;
    ulong CursorCaps;
    ulong DevCaps;
    ulong PrimitiveMiscCaps;
    ulong RasterCaps;
    ulong ZCmpCaps;
    ulong SrcBlendCaps;
    ulong DestBlendCaps;
    ulong AlphaCmpCaps;
    ulong ShadeCaps;
    ulong TextureCaps;
    ulong TextureFilterCaps;
    ulong CubeTextureFilterCaps;
    ulong VolumeTextureFilterCaps;
    ulong TextureAddressCaps;
    ulong VolumeTextureAddressCaps;
    ulong LineCaps;
    ulong MaxTextureWidth;
    ulong MaxTextureHeight;
    ulong MaxVolumeExtent;
    ulong MaxTextureRepeat;
    ulong MaxTextureAspectRatio;
    ulong MaxAnisotropy;
    float MaxVertexW;
    float GuardBandLeft;
    float GuardBandTop;
    float GuardBandRight;
    float GuardBandBottom;
    float ExtentsAdjust;
    ulong StencilCaps;
    ulong FVFCaps;
    ulong TextureOpCaps;
    ulong MaxTextureBlendStages;
    ulong MaxSimultaneousTextures;
    ulong VertexProcessingCaps;
    ulong MaxActiveLights;
    ulong MaxUserClipPlanes;
    ulong MaxVertexBlendMatrices;
    ulong MaxVertexBlendMatrixIndex;
    float MaxPointSize;
    ulong MaxPrimitiveCount;
    ulong MaxVertexIndex;
    ulong MaxStreams;
    ulong MaxStreamStride;
    ulong VertexShaderVersion;
    ulong MaxVertexShaderConst;
    ulong PixelShaderVersion;
    float PixelShader1xMaxValue;
    ulong DevCaps2;
    float MaxNpatchTessellationLevel;
    ulong Reserved5;
    uint MasterAdapterOrdinal;
    uint AdapterOrdinalInGroup;
    uint NumberOfAdaptersInGroup;
    ulong DeclTypes;
    ulong NumSimultaneousRTs;
    ulong StretchRectFilterCaps;
    struct _D3DVSHADERCAPS2_0 VS20Caps;
    struct _D3DPSHADERCAPS2_0 PS20Caps;
    ulong VertexTextureFilterCaps;
    ulong MaxVShaderInstructionsExecuted;
    ulong MaxPShaderInstructionsExecuted;
    ulong MaxVertexShader30InstructionSlots;
    ulong MaxPixelShader30InstructionSlots;
};

typedef struct _FILE_FS_VOLUME_INFORMATION _FILE_FS_VOLUME_INFORMATION, *P_FILE_FS_VOLUME_INFORMATION;


/* WARNING! conflicting data type names: /iw5mp_xenon.pdb/_LARGE_INTEGER - /winnt.h/_LARGE_INTEGER */

struct _FILE_FS_VOLUME_INFORMATION {
    union _LARGE_INTEGER VolumeCreationTime;
    ulong VolumeSerialNumber;
    ulong VolumeLabelLength;
    uchar SupportsObjects;
    uchar VolumeLabel[1];
};


/* WARNING! conflicting data type names: /iw5mp_xenon.pdb/_CONTEXT - /excpt.h/_CONTEXT */

typedef enum TP_PERFCOUNT_SELECT {
    POINT_QUADS=0,
    BILIN_QUADS=1,
    ANISO_QUADS=2,
    MIP_QUADS=3,
    VOL_QUADS=4,
    MIP_VOL_QUADS=5,
    MIP_ANISO_QUADS=6,
    VOL_ANISO_QUADS=7,
    ANISO_2_1_QUADS=8,
    ANISO_4_1_QUADS=9,
    ANISO_6_1_QUADS=10,
    ANISO_8_1_QUADS=11,
    ANISO_10_1_QUADS=12,
    ANISO_12_1_QUADS=13,
    ANISO_14_1_QUADS=14,
    MIP_VOL_ANISO_QUADS=15,
    ALIGN_2_QUADS=16,
    ALIGN_4_QUADS=17,
    PIX_0_QUAD=18,
    PIX_1_QUAD=19,
    PIX_2_QUAD=20,
    PIX_3_QUAD=21,
    PIX_4_QUAD=22,
    TP_MIPMAP_LOD0=23,
    TP_MIPMAP_LOD1=24,
    TP_MIPMAP_LOD2=25,
    TP_MIPMAP_LOD3=26,
    TP_MIPMAP_LOD4=27,
    TP_MIPMAP_LOD5=28,
    TP_MIPMAP_LOD6=29,
    TP_MIPMAP_LOD7=30,
    TP_MIPMAP_LOD8=31,
    TP_MIPMAP_LOD9=32,
    TP_MIPMAP_LOD10=33,
    TP_MIPMAP_LOD11=34,
    TP_MIPMAP_LOD12=35,
    TP_MIPMAP_LOD13=36,
    TP_MIPMAP_LOD14=37
} TP_PERFCOUNT_SELECT;

typedef enum <unnamed-tag_000010D9> {
    SPR_CTR=9
} <unnamed-tag_000010D9>;

typedef union GPU_GROUPVECTFMTCONTROL GPU_GROUPVECTFMTCONTROL, *PGPU_GROUPVECTFMTCONTROL;

typedef struct GPU_GROUPVECTFMTCONTROL_s_0 GPU_GROUPVECTFMTCONTROL_s_0, *PGPU_GROUPVECTFMTCONTROL_s_0;

struct GPU_GROUPVECTFMTCONTROL_s_0 {
    ulong XOffset:4;
    ulong XConv:4;
    ulong YOffset:4;
    ulong YConv:4;
    ulong ZOffset:4;
    ulong ZConv:4;
    ulong WOffset:4;
    ulong WConv:4;
};

union GPU_GROUPVECTFMTCONTROL {
    struct GPU_GROUPVECTFMTCONTROL_s_0 _s_0;
    ulong dword;
};

typedef enum _MM_KERNEL_STACK_TYPE {
    MmDefaultStackType=0,
    MmTitleStackType=1,
    MmSystemStackType=2
} _MM_KERNEL_STACK_TYPE;

typedef union GPU_DEBUGMISC1 GPU_DEBUGMISC1, *PGPU_DEBUGMISC1;

typedef struct GPU_DEBUGMISC1_s_0 GPU_DEBUGMISC1_s_0, *PGPU_DEBUGMISC1_s_0;

struct GPU_DEBUGMISC1_s_0 {
    ulong DbOnVtx:1;
    ulong DbOnPix:1;
    ulong DbInstCount:8;
    ulong DbBreakAddr:11;
};

union GPU_DEBUGMISC1 {
    struct GPU_DEBUGMISC1_s_0 _s_0;
    ulong dword;
};

typedef union GPU_DEBUGMISC0 GPU_DEBUGMISC0, *PGPU_DEBUGMISC0;

typedef struct GPU_DEBUGMISC0_s_0 GPU_DEBUGMISC0_s_0, *PGPU_DEBUGMISC0_s_0;

struct GPU_DEBUGMISC0_s_0 {
    ulong DbProbBreak:1;
    ulong DbProgOn:1;
    undefined field2_0x1;
    undefined field3_0x2;
    ulong DbProbCount:8;
    ulong DbProbAddr:11;
};

union GPU_DEBUGMISC0 {
    struct GPU_DEBUGMISC0_s_0 _s_0;
    ulong dword;
};

typedef struct _LIST_ENTRY _LIST_ENTRY, *P_LIST_ENTRY;

struct _LIST_ENTRY {
    struct _LIST_ENTRY *Flink;
    struct _LIST_ENTRY *Blink;
};

typedef struct _SINGLE_LIST_ENTRY _SINGLE_LIST_ENTRY, *P_SINGLE_LIST_ENTRY;

struct _SINGLE_LIST_ENTRY {
    struct _SINGLE_LIST_ENTRY *Next;
};

typedef union FSB_TX_TERSTM_AND FSB_TX_TERSTM_AND, *PFSB_TX_TERSTM_AND;


/* WARNING! conflicting data type names: /iw5mp_xenon.pdb/FSB_TX_TERSTM_AND/<unnamed-type-bitfields> - /iw5mp_xenon.pdb/D1SCL_COEF_RAM_SELECT/<unnamed-type-bitfields> */

union FSB_TX_TERSTM_AND {
    struct <unnamed-type-bitfields> bitfields;
    struct <unnamed-type-bitfields> bits;
    uint u32All;
    int i32All;
    float f32All;
};

typedef union GPU_DEPTHCONTROL GPU_DEPTHCONTROL, *PGPU_DEPTHCONTROL;

typedef struct GPU_DEPTHCONTROL_s_0 GPU_DEPTHCONTROL_s_0, *PGPU_DEPTHCONTROL_s_0;

struct GPU_DEPTHCONTROL_s_0 {
    ulong BackFaceEnable:1;
    ulong ZFunc:3;
    ulong ZWriteEnable:1;
    ulong ZEnable:1;
    ulong StencilEnable:1;
    ulong StencilFail:3;
    ulong StencilFuncBF:3;
    ulong StencilZFailBF:3;
    ulong StencilFailBF:3;
    ulong StencilZPassBF:3;
    ulong StencilZPass:3;
    ulong StencilZFail:3;
    ulong StencilFunc:3;
};

union GPU_DEPTHCONTROL {
    struct GPU_DEPTHCONTROL_s_0 _s_0;
    ulong dword;
};

typedef enum IOCPERFEVENT {
    IOCPE_PERF_CYCLES_WAITING_FOR_REGISTER_BUS=0,
    IOCPE_PERF_CYCLES_WAITING_FOR_BIU_SLV_REQ=1,
    IOCPE_PERF_CYCLES_WAITING_FOR_BIU_MST_CPL=2,
    IOCPE_PERF_CYCLES_WAITING_FOR_BSB_MST_REQ=3,
    IOCPE_PERF_CYCLES_WAITING_FOR_MH_SLV_REQ=4,
    IOCPE_PERF_BSB_SLV_REQ_TOTAL_REQUESTS=5,
    IOCPE_PERF_BSB_SLV_REQ_TOTAL_READS=6,
    IOCPE_PERF_BSB_SLV_REQ_TOTAL_WRITES=7,
    IOCPE_PERF_BSB_SLV_REQ_SNOOPED_READS=8,
    IOCPE_PERF_BSB_SLV_REQ_SNOOPED_WRITES=9,
    IOCPE_PERF_BSB_SLV_REQ_NON_SNOOPED_READS=10,
    IOCPE_PERF_BSB_SLV_REQ_NON_SNOOPED_WRITES=11,
    IOCPE_PERF_BSB_SLV_REQ_INTERRUPTS=12,
    IOCPE_PERF_BSB_SLV_REQ_TOTAL_REQUESTS_DWORDS=13,
    IOCPE_PERF_BSB_SLV_REQ_TOTAL_READS_DWORDS=14,
    IOCPE_PERF_BSB_SLV_REQ_TOTAL_WRITES_DWORDS=15,
    IOCPE_PERF_BSB_SLV_REQ_SNOOPED_READS_DWORDS=16,
    IOCPE_PERF_BSB_SLV_REQ_SNOOPED_WRITES_DWORDS=17,
    IOCPE_PERF_BSB_SLV_REQ_NON_SNOOPED_READS_DWORDS=18,
    IOCPE_PERF_BSB_SLV_REQ_NON_SNOOPED_WRITES_DWORDS=19,
    IOCPE_PERF_BSB_SLV_READ_LATENCY_CYCLES=20,
    IOCPE_PERF_BSB_SLV_CPL_TOTAL_READ_COMPLETIONS=21,
    IOCPE_PERF_BSB_SLV_CPL_SNOOPED_READ_COMPLETIONS=22,
    IOCPE_PERF_BSB_SLV_CPL_NON_SNOOPED_READ_COMPLETIONS=23,
    IOCPE_PERF_BSB_SLV_CPL_TOTAL_READ_COMPLETIONS_DWORDS=24,
    IOCPE_PERF_BSB_SLV_CPL_SNOOPED_READ_COMPLETIONS_DWORDS=25,
    IOCPE_PERF_BSB_SLV_CPL_NON_SNOOPED_READ_COMPLETIONS_DWORDS=26,
    IOCPE_PERF_BSB_SLV_CPL_URS=27,
    IOCPE_PERF_BIF_SLV_REQ_TOTAL_REQUESTS=28,
    IOCPE_PERF_BIF_SLV_REQ_TOTAL_CP_WRITEBACKS=29,
    IOCPE_PERF_BIF_SLV_REQ_TOTAL_SNOOPED_WRITES=30,
    IOCPE_PERF_BSB_MST_REQ_TOTAL_REQUESTS=31,
    IOCPE_PERF_BSB_MST_REQ_TOTAL_READS=32,
    IOCPE_PERF_BSB_MST_REQ_TOTAL_WRITES=33,
    IOCPE_PERF_BSB_MST_REQ_TOTAL_REQUESTS_DWORDS=34,
    IOCPE_PERF_BSB_MST_REQ_TOTAL_READS_DWORDS=35,
    IOCPE_PERF_BSB_MST_REQ_TOTAL_WRITES_DWORDS=36,
    IOCPE_PERF_BSB_MST_CPL_TOTAL_COMPLETIONS=37,
    IOCPE_PERF_BSB_MST_CPL_TOTAL_COMPLETIONS_DWORDS=38,
    IOCPE_PERF_REG_BUS_REQ_TOTAL_REQUESTS=39,
    IOCPE_PERF_REG_BUS_REQ_TOTAL_READS=40,
    IOCPE_PERF_REG_BUS_REQ_TOTAL_WRITES=41,
    IOCPE_PERF_REG_BUS_CPL_READ_COMPLETIONS=42,
    IOCPE_PERF_TOTAL_SYNCS_ISSUED=43,
    IOCPE_PERF_BIU_SYNCS_ISSUED=44,
    IOCPE_PERF_MH_SYNCS_ISSUED=45,
    IOCPE_PERF_TOTAL_SYNCS_RECEIVED=46,
    IOCPE_PERF_BIU_SYNCS_RECEIVED=47,
    IOCPE_PERF_MH_SYNCS_RECEIVED=48,
    IOCPE_PERF_FIFO_BSB_MST_REQ_FULL=49,
    IOCPE_PERF_FIFO_BSB_MST_REQ_EMPTY=50,
    IOCPE_PERF_FIFO_SYNC_FULL=51,
    IOCPE_PERF_FIFO_SYNC_EMPTY=52,
    IOCPE_PERF_FIFO_INTERRUPT_FULL=53,
    IOCPE_PERF_FIFO_INTERRUPT_EMPTY=54,
    IOCPE_PERF_FIFO_REG_FULL=55,
    IOCPE_PERF_FIFO_REG_EMPTY=56,
    IOCPE_PERF_FIFO_BIU_SLV_CPL_NEARFULL=57,
    IOCPE_PERF_FIFO_BIU_SLV_CPL_EMPTY=58,
    IOCPE_PERF_FIFO_MH_SLV_CPL_FULL=59,
    IOCPE_PERF_FIFO_MH_SLV_CPL_EMPTY=60,
    IOCPE_PERF_FIFO_BSB_SLV_REQ_NEARFULL=61,
    IOCPE_PERF_FIFO_BSB_SLV_REQ_EMPTY=62,
    IOCPE_PERF_FIFO_BSB_MST_CPL_NEARFULL=63,
    IOCPE_PERF_FIFO_BSB_MST_CPL_EMPTY=64,
    IOCPE_PERF_FIFO_BIU_MST_REQ_NEARFULL=65,
    IOCPE_PERF_FIFO_BIU_MST_REQ_EMPTY=66,
    IOCPE_PERF_FIFO_BIF_SLV_REQ_NEARFULL=67,
    IOCPE_PERF_FIFO_BIF_SLV_REQ_EMPTY=68,
    IOCPE_PERF_FIFO_READ_LATENCY_FULL=69,
    IOCPE_PERF_FIFO_READ_LATENCY_EMPTY=70,
    IOCPE_PERF_NUMBER_OF_SYSTEM_CLOCKS=71,
    IOCPE_PERF_ZERO_COUNT=72,
    IOCPE_PERF_SIXTY_THREE_COUNT=73,
    IOCPE_PERF_BIU_SPARE=74,
    IOCPE_PERF_BSB_SLV_REQ_MESSAGES=75,
    IOCPE_PERF_BSB_MST_REQ_CONFIG_READS=76,
    IOCPE_PERF_BSB_MST_REQ_CONFIG_WRITES=77,
    IOCPE_PERF_BSB_MST_REQ_POSTED_WRITES=78,
    IOCPE_PERF_BSB_MST_REQ_POSTED_WRITES_DWORDS=79,
    IOCPE_PERF_BSB_MST_CPL_NON_POSTED_WRITES=80,
    IOCPE_PERF_CYCLES_WAITING_FOR_INT_SYNC=81,
    IOCPE_PERF_CYCLES_WAITING_FOR_INT_SYNC_SNOOP_PATH=82,
    IOCPE_PERF_CYCLES_WAITING_FOR_INT_SYNC_NON_SNOOP_PATH=83,
    IOCPE_PERF_CYCLES_WAITING_FOR_INT_SYNC_BOTH_PATHS=84,
    IOCPE_PERF_CYCLES_WAITING_FOR_MST_RD_CPL_SYNC=85,
    IOCPE_PERF_CYCLES_WAITING_FOR_MST_RD_CPL_SYNC_SNOOP_PATH=86,
    IOCPE_PERF_CYCLES_WAITING_FOR_MST_RD_CPL_SYNC_NON_SNOOP_PATH=87,
    IOCPE_PERF_CYCLES_WAITING_FOR_MST_RD_CPL_SYNC_BOTH_PATHS=88,
    IOCPE_PERF_CYCLES_WAITING_FOR_NSR_PASS_SW=89,
    IOCPE_PERF_CYCLES_WAITING_FOR_SR_PASS_NSW=90,
    IOCPE_PERF_CYCLES_WAITING_FOR_NSW_PASS_SW=91,
    IOCPE_PERF_CYCLES_WAITING_FOR_SW_PASS_NSW=92,
    IOCPE_PERF_FIFO_MST_CPL_SYNC_TYPE_FIFO_FULL=93,
    IOCPE_PERF_FIFO_MST_CPL_SYNC_TYPE_FIFO_EMPTY=94,
    IOCPE_PERF_FIFO_BSB_SNOOP_SLV_REQ_FULL=95,
    IOCPE_PERF_FIFO_BSB_SNOOP_SLV_REQ_EMPTY=96,
    IOCPE_PERF_FIFO_BSB_SNOOP_SLV_REQ_DATA_FULL=97,
    IOCPE_PERF_FIFO_BSB_SNOOP_SLV_REQ_DATA_EMPTY=98,
    IOCPE_PERF_FIFO_READ_LATENCY_BAD_REQ_FULL=99,
    IOCPE_PERF_FIFO_READ_LATENCY_BAD_REQ_EMPTY=100,
    IOCPE_PERF_FIFO_READ_LATENCY_NON_SNOOP_CAM_ADDRESS_FULL=101,
    IOCPE_PERF_FIFO_READ_LATENCY_NON_SNOOP_CAM_ADDRESS_EMPTY=102
} IOCPERFEVENT;


/* WARNING! conflicting data type names: /iw5mp_xenon.pdb/_EXCEPTION_RECORD - /excpt.h/_EXCEPTION_RECORD */

typedef enum GPUPERFEVENT_RBBM {
    GPUPE_RBBM_COUNT=0,
    GPUPE_RBBM_NRT_BUSY=1,
    GPUPE_RBBM_BC_CNTX0_BUSY=2,
    GPUPE_RBBM_BC_CNTX17_BUSY=3,
    GPUPE_RBBM_SQ_CNTX0_BUSY=4,
    GPUPE_RBBM_SQ_CNTX17_BUSY=5,
    GPUPE_RBBM_VGT_BUSY=6,
    GPUPE_RBBM_VGT_NODMA_BUSY=7,
    GPUPE_RBBM_PA_BUSY=8,
    GPUPE_RBBM_SC_CNTX0_BUSY=9,
    GPUPE_RBBM_SC_CNTX17_BUSY=10,
    GPUPE_RBBM_TPC_BUSY=11,
    GPUPE_RBBM_TC_BUSY=12,
    GPUPE_RBBM_SX_BUSY=13,
    GPUPE_RESERVED1=14,
    GPUPE_RBBM_CP_COHER_BUSY=15,
    GPUPE_RBBM_CP_NRT_BUSY=16,
    GPUPE_RESERVED2=17,
    GPUPE_RBBM_CP_DMA_BUSY=18,
    GPUPE_RESERVED3=19,
    GPUPE_RESERVED4=20,
    GPUPE_RESERVED5=21,
    GPUPE_RBBM_DMA_IDLE_STALL=22,
    GPUPE_RESERVED6=23,
    GPUPE_RESERVED7=24,
    GPUPE_RBBM_GFX_IDLE_STALL=25,
    GPUPE_RBBM_GFX_IDLEC_STALL=26,
    GPUPE_RBBM_INTERRUPT=27
} GPUPERFEVENT_RBBM;

typedef enum GPUSWIZZLE {
    GPUSWIZZLE_X=0,
    GPUSWIZZLE_Y=1,
    GPUSWIZZLE_Z=2,
    GPUSWIZZLE_W=3,
    GPUSWIZZLE_0=4,
    GPUSWIZZLE_1=5,
    GPUSWIZZLE_KEEP=7
} GPUSWIZZLE;

typedef enum GPUGROUPPRIMTYPE {
    GPUGROUPPRIMTYPE_3D_POINT=0,
    GPUGROUPPRIMTYPE_3D_LINE=1,
    GPUGROUPPRIMTYPE_3D_TRI=2,
    GPUGROUPPRIMTYPE_3D_RECT=3,
    GPUGROUPPRIMTYPE_3D_QUAD=4,
    GPUGROUPPRIMTYPE_2D_COPY_RECT_V0=5,
    GPUGROUPPRIMTYPE_2D_COPY_RECT_V1=6,
    GPUGROUPPRIMTYPE_2D_COPY_RECT_V2=7,
    GPUGROUPPRIMTYPE_2D_COPY_RECT_V3=8,
    GPUGROUPPRIMTYPE_2D_FILL_RECT=9,
    GPUGROUPPRIMTYPE_2D_LINE=10,
    GPUGROUPPRIMTYPE_2D_TRI=11,
    GPUGROUPPRIMTYPE_PRIM_INDEX_LINE=12,
    GPUGROUPPRIMTYPE_PRIM_INDEX_TRI=13,
    GPUGROUPPRIMTYPE_PRIM_INDEX_QUAD=14
} GPUGROUPPRIMTYPE;

typedef struct D3DQuery D3DQuery, *PD3DQuery;

struct D3DQuery {
    undefined field0_0x0;
};

typedef enum <unnamed-tag_00001121> {
    SPR_HSPRG1=49
} <unnamed-tag_00001121>;

typedef enum HDCP_STATUS_CODE {
    HDCP_SUCCESS=0,
    HDCP_AUTHENTICATION_FAILED=1,
    HDCP_LINK_INTEGRITY_CHECK_FAILED=2,
    HDCP_REPEATER_AUTHENTICATION_FAILED=3,
    HDCP_NO_RECEIVER_CONNECTED=4,
    HDCP_UNAVAILABLE=5,
    HDCP_KEYS_UNLOCKED=6,
    HDCP_TX_AN_READ_FAILED=7,
    HDCP_TX_AN_WRITE_FAILED=8,
    HDCP_RX_AN_READ_FAILED=9,
    HDCP_RX_AN_WRITE_FAILED=10,
    HDCP_INVALID_AKSV_FORMAT=11,
    HDCP_INVALID_BKSV_FORMAT=12,
    HDCP_AKSV_WRITE_FAILED=13,
    HDCP_BKSV_WRITE_FAILED=14,
    HDCP_KSV_REVOKED=15,
    HDCP_REPEATER_KSV_READ_TIMEOUT=16,
    HDCP_REPEATER_KSV_READ_FAILED=17,
    HDCP_MAX_DEVICES_EXCEEDED=18,
    HDCP_DDC_ERROR=19,
    HDCP_AKSV_READ_FAILED=20,
    HDCP_BKSV_READ_FAILED=21,
    HDCP_HPD_DETECTED=22
} HDCP_STATUS_CODE;

typedef union GPU_VTXCONTROL GPU_VTXCONTROL, *PGPU_VTXCONTROL;

typedef struct GPU_VTXCONTROL_s_0 GPU_VTXCONTROL_s_0, *PGPU_VTXCONTROL_s_0;

struct GPU_VTXCONTROL_s_0 {
    ulong QuantMode:3;
    ulong RoundMode:2;
    ulong PixCenter:1;
};

union GPU_VTXCONTROL {
    struct GPU_VTXCONTROL_s_0 _s_0;
    ulong dword;
};

typedef enum GPUEXECFETCHCACHETYPE {
    GPUEXECFETCHCACHETYPE_TEXTURE=0,
    GPUEXECFETCHCACHETYPE_VERTEX=1
} GPUEXECFETCHCACHETYPE;

typedef enum TCM_PERFCOUNT_SELECT {
    QUAD0_RD_LAT_FIFO_EMPTY=0,
    QUAD0_RD_LAT_FIFO_16TH_FULL=1,
    QUAD0_RD_LAT_FIFO_8TH_FULL=2,
    QUAD0_RD_LAT_FIFO_4TH_FULL=3,
    QUAD0_RD_LAT_FIFO_HALF_FULL=4,
    QUAD0_RD_LAT_FIFO_FULL=5,
    QUAD0_RD_LAT_FIFO_LT_16TH_FULL=6,
    QUAD1_RD_LAT_FIFO_EMPTY=7,
    QUAD1_RD_LAT_FIFO_16TH_FULL=8,
    QUAD1_RD_LAT_FIFO_8TH_FULL=9,
    QUAD1_RD_LAT_FIFO_4TH_FULL=10,
    QUAD1_RD_LAT_FIFO_HALF_FULL=11,
    QUAD1_RD_LAT_FIFO_FULL=12,
    QUAD1_RD_LAT_FIFO_LT_16TH_FULL=13,
    QUAD2_RD_LAT_FIFO_EMPTY=14,
    QUAD2_RD_LAT_FIFO_16TH_FULL=15,
    QUAD2_RD_LAT_FIFO_8TH_FULL=16,
    QUAD2_RD_LAT_FIFO_4TH_FULL=17,
    QUAD2_RD_LAT_FIFO_HALF_FULL=18,
    QUAD2_RD_LAT_FIFO_FULL=19,
    QUAD2_RD_LAT_FIFO_LT_16TH_FULL=20,
    QUAD3_RD_LAT_FIFO_EMPTY=21,
    QUAD3_RD_LAT_FIFO_16TH_FULL=22,
    QUAD3_RD_LAT_FIFO_8TH_FULL=23,
    QUAD3_RD_LAT_FIFO_4TH_FULL=24,
    QUAD3_RD_LAT_FIFO_HALF_FULL=25,
    QUAD3_RD_LAT_FIFO_FULL=26,
    QUAD3_RD_LAT_FIFO_LT_16TH_FULL=27,
    READ_STARVED_QUAD0=28,
    READ_STARVED_QUAD1=29,
    READ_STARVED_QUAD2=30,
    READ_STARVED_QUAD3=31,
    READ_STARVED=32,
    READ_STALLED_QUAD0=33,
    READ_STALLED_QUAD1=34,
    READ_STALLED_QUAD2=35,
    READ_STALLED_QUAD3=36,
    READ_STALLED=37,
    VALID_READ_QUAD0=38,
    VALID_READ_QUAD1=39,
    VALID_READ_QUAD2=40,
    VALID_READ_QUAD3=41,
    TC_TP_STARVED_QUAD0=42,
    TC_TP_STARVED_QUAD1=43,
    TC_TP_STARVED_QUAD2=44,
    TC_TP_STARVED_QUAD3=45,
    TC_TP_STARVED=46
} TCM_PERFCOUNT_SELECT;

typedef struct _D3DSURFACE_PARAMETERS _D3DSURFACE_PARAMETERS, *P_D3DSURFACE_PARAMETERS;

struct _D3DSURFACE_PARAMETERS {
    ulong Base;
    ulong HierarchicalZBase;
    int ColorExpBias;
    enum _D3DHIZFUNC HiZFunc;
};

typedef struct _XMARKETPLACE_ASSET_ENUMERATE_REPLY _XMARKETPLACE_ASSET_ENUMERATE_REPLY, *P_XMARKETPLACE_ASSET_ENUMERATE_REPLY;

typedef struct _XMARKETPLACE_ASSET_PACKAGE _XMARKETPLACE_ASSET_PACKAGE, *P_XMARKETPLACE_ASSET_PACKAGE;


/* WARNING! conflicting data type names: /iw5mp_xenon.pdb/_FILETIME - /WinDef.h/_FILETIME */

typedef struct _XMARKETPLACE_ASSET _XMARKETPLACE_ASSET, *P_XMARKETPLACE_ASSET;

struct _XMARKETPLACE_ASSET {
    ulong dwAssetID;
    ulong dwQuantity;
};

struct _XMARKETPLACE_ASSET_PACKAGE {
    struct _FILETIME ftEnumerate;
    ulong cAssets;
    ulong cTotalAssets;
    struct _XMARKETPLACE_ASSET aAssets[1];
};

struct _XMARKETPLACE_ASSET_ENUMERATE_REPLY {
    uchar signature[256];
    struct _XMARKETPLACE_ASSET_PACKAGE assetPackage;
};

typedef enum _XPROFILE_RACE_CAMERA_LOCATION_OPTIONS {
    XPROFILE_RACE_CAMERA_LOCATION_BEHIND=0,
    XPROFILE_RACE_CAMERA_LOCATION_IN_FRONT=1,
    XPROFILE_RACE_CAMERA_LOCATION_INSIDE=2
} _XPROFILE_RACE_CAMERA_LOCATION_OPTIONS;

typedef struct _DUMP_FRAME_BUFFER_INFO _DUMP_FRAME_BUFFER_INFO, *P_DUMP_FRAME_BUFFER_INFO;

struct _DUMP_FRAME_BUFFER_INFO {
    void *FrameBuffer;
    ulong Pitch;
    ulong Height;
    ulong Width;
    ulong Format;
    ulong Size;
    uchar ReadOnly;
};

typedef enum XGENDIANTYPE {
    XGENDIAN_NONE=0,
    XGENDIAN_8IN128=1,
    XGENDIAN_8IN16=1,
    XGENDIAN_8IN32=1,
    XGENDIAN_8IN64=1,
    XGENDIAN_16IN128=2,
    XGENDIAN_16IN32=2,
    XGENDIAN_16IN64=2,
    XGENDIAN_32IN128=4,
    XGENDIAN_32IN64=4,
    XGENDIAN_64IN128=8
} XGENDIANTYPE;

typedef enum GPULOADTYPE {
    GPULOADTYPE_VERTEX=0,
    GPULOADTYPE_PIXEL=1,
    GPULOADTYPE_SHARED=2
} GPULOADTYPE;

typedef struct D3DAsyncCommandBufferCall D3DAsyncCommandBufferCall, *PD3DAsyncCommandBufferCall;

typedef struct _D3DTAGCOLLECTION _D3DTAGCOLLECTION, *P_D3DTAGCOLLECTION;

struct _D3DTAGCOLLECTION {
    ulong64 m_Mask[5];
};

struct D3DAsyncCommandBufferCall {
    int _padding_;
    int _padding_;
    int _padding_;
    int _padding_;
    int _padding_;
    struct _D3DTAGCOLLECTION m_Inherited;
    struct _D3DTAGCOLLECTION m_Persisted;
};

typedef enum _D3DBASISTYPE {
    D3DBASIS_BEZIER=0,
    D3DBASIS_BSPLINE=1,
    D3DBASIS_CATMULL_ROM=2,
    D3DBASIS_FORCE_DWORD=255
} _D3DBASISTYPE;

typedef enum GPUPERFEVENT_TP {
    GPUPE_POINT_QUADS=0,
    GPUPE_BILIN_QUADS=1,
    GPUPE_ANISO_QUADS=2,
    GPUPE_MIP_QUADS=3,
    GPUPE_VOL_QUADS=4,
    GPUPE_MIP_VOL_QUADS=5,
    GPUPE_MIP_ANISO_QUADS=6,
    GPUPE_VOL_ANISO_QUADS=7,
    GPUPE_ANISO_2_1_QUADS=8,
    GPUPE_ANISO_4_1_QUADS=9,
    GPUPE_ANISO_6_1_QUADS=10,
    GPUPE_ANISO_8_1_QUADS=11,
    GPUPE_ANISO_10_1_QUADS=12,
    GPUPE_ANISO_12_1_QUADS=13,
    GPUPE_ANISO_14_1_QUADS=14,
    GPUPE_ANISO_16_1_QUADS=15,
    GPUPE_MIP_VOL_ANISO_QUADS=16,
    GPUPE_ALIGN_2_QUADS=17,
    GPUPE_ALIGN_4_QUADS=18,
    GPUPE_PIX_0_QUAD=19,
    GPUPE_PIX_1_QUAD=20,
    GPUPE_PIX_2_QUAD=21,
    GPUPE_PIX_3_QUAD=22,
    GPUPE_PIX_4_QUAD=23,
    GPUPE_TP_MIPMAP_LOD0=24,
    GPUPE_TP_MIPMAP_LOD1=25,
    GPUPE_TP_MIPMAP_LOD2=26,
    GPUPE_TP_MIPMAP_LOD3=27,
    GPUPE_TP_MIPMAP_LOD4=28,
    GPUPE_TP_MIPMAP_LOD5=29,
    GPUPE_TP_MIPMAP_LOD6=30,
    GPUPE_TP_MIPMAP_LOD7=31,
    GPUPE_TP_MIPMAP_LOD8=32,
    GPUPE_TP_MIPMAP_LOD9=33,
    GPUPE_TP_MIPMAP_LOD10=34,
    GPUPE_TP_MIPMAP_LOD11=35,
    GPUPE_TP_MIPMAP_LOD12=36,
    GPUPE_TP_MIPMAP_LOD13=37,
    GPUPE_TP_MIPMAP_LOD14=38
} GPUPERFEVENT_TP;

typedef union BIOS_13_SCRATCH BIOS_13_SCRATCH, *PBIOS_13_SCRATCH;


/* WARNING! conflicting data type names: /iw5mp_xenon.pdb/BIOS_13_SCRATCH/<unnamed-type-bitfields> - /iw5mp_xenon.pdb/D1SCL_COEF_RAM_SELECT/<unnamed-type-bitfields> */

union BIOS_13_SCRATCH {
    struct <unnamed-type-bitfields> bitfields;
    struct <unnamed-type-bitfields> bits;
    uint u32All;
    int i32All;
    float f32All;
};

typedef enum DepthFormat {
    DEPTH_24_8=22,
    DEPTH_24_8_FLOAT=23,
    DEPTH_16=24
} DepthFormat;

typedef enum SurfaceFormat {
    FMT_1_REVERSE=0,
    FMT_1=1,
    FMT_8=2,
    FMT_1_5_5_5=3,
    FMT_5_6_5=4,
    FMT_6_5_5=5,
    FMT_8_8_8_8=6,
    FMT_2_10_10_10=7,
    FMT_8_A=8,
    FMT_8_B=9,
    FMT_8_8=10,
    FMT_Cr_Y1_Cb_Y0=11,
    FMT_Y1_Cr_Y0_Cb=12,
    FMT_8_8_8_8_A=14,
    FMT_4_4_4_4=15,
    FMT_10_11_11=16,
    FMT_11_11_10=17,
    FMT_DXT1=18,
    FMT_DXT2_3=19,
    FMT_DXT4_5=20,
    FMT_24_8=22,
    FMT_24_8_FLOAT=23,
    FMT_16=24,
    FMT_16_16=25,
    FMT_16_16_16_16=26,
    FMT_16_EXPAND=27,
    FMT_16_16_EXPAND=28,
    FMT_16_16_16_16_EXPAND=29,
    FMT_16_FLOAT=30,
    FMT_16_16_FLOAT=31,
    FMT_16_16_16_16_FLOAT=32,
    FMT_32=33,
    FMT_32_32=34,
    FMT_32_32_32_32=35,
    FMT_32_FLOAT=36,
    FMT_32_32_FLOAT=37,
    FMT_32_32_32_32_FLOAT=38,
    FMT_32_AS_8=39,
    FMT_32_AS_8_8=40,
    FMT_16_MPEG=41,
    FMT_16_16_MPEG=42,
    FMT_8_INTERLACED=43,
    FMT_32_AS_8_INTERLACED=44,
    FMT_32_AS_8_8_INTERLACED=45,
    FMT_16_INTERLACED=46,
    FMT_16_MPEG_INTERLACED=47,
    FMT_16_16_MPEG_INTERLACED=48,
    FMT_DXN=49,
    FMT_8_8_8_8_AS_16_16_16_16=50,
    FMT_DXT1_AS_16_16_16_16=51,
    FMT_DXT2_3_AS_16_16_16_16=52,
    FMT_DXT4_5_AS_16_16_16_16=53,
    FMT_2_10_10_10_AS_16_16_16_16=54,
    FMT_10_11_11_AS_16_16_16_16=55,
    FMT_11_11_10_AS_16_16_16_16=56,
    FMT_32_32_32_FLOAT=57,
    FMT_DXT3A=58,
    FMT_DXT5A=59,
    FMT_CTX1=60,
    FMT_DXT3A_AS_1_1_1_1=61
} SurfaceFormat;

typedef enum GPUPERFREGISTER {
    GPUPERFREG_BC_PERFCOUNTER0_SELECT=4,
    GPUPERFREG_HZ_PERFCOUNTER0_SELECT=4,
    GPUPERFREG_BC_PERFCOUNTER1_SELECT=5,
    GPUPERFREG_HZ_PERFCOUNTER0_HI=5,
    GPUPERFREG_TCR_PERFCOUNTER0_SELECT=5,
    GPUPERFREG_BC_PERFCOUNTER2_SELECT=6,
    GPUPERFREG_HZ_PERFCOUNTER0_LOW=6,
    GPUPERFREG_TCR_PERFCOUNTER0_HI=6,
    GPUPERFREG_BC_PERFCOUNTER3_SELECT=7,
    GPUPERFREG_HZ_PERFCOUNTER1_SELECT=7,
    GPUPERFREG_TCR_PERFCOUNTER0_LOW=7,
    GPUPERFREG_BC_PERFCOUNTER0_LOW=8,
    GPUPERFREG_HZ_PERFCOUNTER1_HI=8,
    GPUPERFREG_TCR_PERFCOUNTER1_SELECT=8,
    GPUPERFREG_BC_PERFCOUNTER0_HI=9,
    GPUPERFREG_HZ_PERFCOUNTER1_LOW=9,
    GPUPERFREG_TCR_PERFCOUNTER1_HI=9,
    GPUPERFREG_BC_PERFCOUNTER1_LOW=10,
    GPUPERFREG_TCR_PERFCOUNTER1_LOW=10,
    GPUPERFREG_BC_PERFCOUNTER1_HI=11,
    GPUPERFREG_BC_PERFCOUNTER2_LOW=12,
    GPUPERFREG_BC_PERFCOUNTER2_HI=13,
    GPUPERFREG_BC_PERFCOUNTER3_LOW=14,
    GPUPERFREG_BC_PERFCOUNTER3_HI=15,
    GPUPERFREG_MC0_PERFCOUNTER0_SELECT=21,
    GPUPERFREG_MC0_PERFCOUNTER0_HI=22,
    GPUPERFREG_MC0_PERFCOUNTER0_LOW=23,
    GPUPERFREG_MH_PERFCOUNTER0_SELECT=24,
    GPUPERFREG_MH_PERFCOUNTER0_HI=25,
    GPUPERFREG_MH_PERFCOUNTER0_LOW=26,
    GPUPERFREG_MH_PERFCOUNTER1_SELECT=27,
    GPUPERFREG_MH_PERFCOUNTER1_HI=28,
    GPUPERFREG_MH_PERFCOUNTER1_LOW=29,
    GPUPERFREG_MH_PERFCOUNTER2_SELECT=30,
    GPUPERFREG_MH_PERFCOUNTER2_HI=31,
    GPUPERFREG_TP0_PERFCOUNTER0_SELECT=31,
    GPUPERFREG_MH_PERFCOUNTER2_LOW=32,
    GPUPERFREG_TP0_PERFCOUNTER0_HI=32,
    GPUPERFREG_TP0_PERFCOUNTER0_LOW=33,
    GPUPERFREG_TP0_PERFCOUNTER1_SELECT=34,
    GPUPERFREG_TP0_PERFCOUNTER1_HI=35,
    GPUPERFREG_TP0_PERFCOUNTER1_LOW=36,
    GPUPERFREG_TP1_PERFCOUNTER0_SELECT=40,
    GPUPERFREG_TP1_PERFCOUNTER0_HI=41,
    GPUPERFREG_TP1_PERFCOUNTER0_LOW=42,
    GPUPERFREG_TP1_PERFCOUNTER1_SELECT=43,
    GPUPERFREG_TP1_PERFCOUNTER1_HI=44,
    GPUPERFREG_TP1_PERFCOUNTER1_LOW=45,
    GPUPERFREG_TP2_PERFCOUNTER0_SELECT=49,
    GPUPERFREG_TP2_PERFCOUNTER0_HI=50,
    GPUPERFREG_TP2_PERFCOUNTER0_LOW=51,
    GPUPERFREG_TP2_PERFCOUNTER1_SELECT=52,
    GPUPERFREG_TP2_PERFCOUNTER1_HI=53,
    GPUPERFREG_TP2_PERFCOUNTER1_LOW=54,
    GPUPERFREG_TP3_PERFCOUNTER0_SELECT=58,
    GPUPERFREG_TP3_PERFCOUNTER0_HI=59,
    GPUPERFREG_TP3_PERFCOUNTER0_LOW=60,
    GPUPERFREG_TP3_PERFCOUNTER1_SELECT=61,
    GPUPERFREG_TP3_PERFCOUNTER1_HI=62,
    GPUPERFREG_TP3_PERFCOUNTER1_LOW=63,
    GPUPERFREG_BIF_PERFCOUNTER0_SELECT=72,
    GPUPERFREG_VC_PERFCOUNTER0_SELECT=72,
    GPUPERFREG_VGT_PERFCOUNTER0_SELECT=72,
    GPUPERFREG_BIF_PERFCOUNTER0_HI=73,
    GPUPERFREG_VC_PERFCOUNTER0_HI=73,
    GPUPERFREG_VGT_PERFCOUNTER1_SELECT=73,
    GPUPERFREG_BIF_PERFCOUNTER0_LOW=74,
    GPUPERFREG_VC_PERFCOUNTER0_LOW=74,
    GPUPERFREG_VGT_PERFCOUNTER2_SELECT=74,
    GPUPERFREG_VC_PERFCOUNTER1_SELECT=75,
    GPUPERFREG_VGT_PERFCOUNTER3_SELECT=75,
    GPUPERFREG_VC_PERFCOUNTER1_HI=76,
    GPUPERFREG_VGT_PERFCOUNTER0_LOW=76,
    GPUPERFREG_VC_PERFCOUNTER1_LOW=77,
    GPUPERFREG_VGT_PERFCOUNTER0_HI=77,
    GPUPERFREG_VC_PERFCOUNTER2_SELECT=78,
    GPUPERFREG_VGT_PERFCOUNTER1_LOW=78,
    GPUPERFREG_VC_PERFCOUNTER2_HI=79,
    GPUPERFREG_VGT_PERFCOUNTER1_HI=79,
    GPUPERFREG_VC_PERFCOUNTER2_LOW=80,
    GPUPERFREG_VGT_PERFCOUNTER2_LOW=80,
    GPUPERFREG_VC_PERFCOUNTER3_SELECT=81,
    GPUPERFREG_VGT_PERFCOUNTER2_HI=81,
    GPUPERFREG_VC_PERFCOUNTER3_HI=82,
    GPUPERFREG_VGT_PERFCOUNTER3_LOW=82,
    GPUPERFREG_VC_PERFCOUNTER3_LOW=83,
    GPUPERFREG_VGT_PERFCOUNTER3_HI=83,
    GPUPERFREG_TCM_PERFCOUNTER0_SELECT=84,
    GPUPERFREG_MC1_PERFCOUNTER0_SELECT=85,
    GPUPERFREG_TCM_PERFCOUNTER0_HI=85,
    GPUPERFREG_MC1_PERFCOUNTER0_HI=86,
    GPUPERFREG_TCM_PERFCOUNTER0_LOW=86,
    GPUPERFREG_MC1_PERFCOUNTER0_LOW=87,
    GPUPERFREG_TCM_PERFCOUNTER1_SELECT=87,
    GPUPERFREG_TCM_PERFCOUNTER1_HI=88,
    GPUPERFREG_TCM_PERFCOUNTER1_LOW=89,
    GPUPERFREG_TCF_PERFCOUNTER0_SELECT=90,
    GPUPERFREG_TCF_PERFCOUNTER0_HI=91,
    GPUPERFREG_TCF_PERFCOUNTER0_LOW=92,
    GPUPERFREG_TCF_PERFCOUNTER1_SELECT=93,
    GPUPERFREG_TCF_PERFCOUNTER1_HI=94,
    GPUPERFREG_TCF_PERFCOUNTER1_LOW=95,
    GPUPERFREG_TCF_PERFCOUNTER2_SELECT=96,
    GPUPERFREG_TCF_PERFCOUNTER2_HI=97,
    GPUPERFREG_TCF_PERFCOUNTER2_LOW=98,
    GPUPERFREG_TCF_PERFCOUNTER3_SELECT=99,
    GPUPERFREG_TCF_PERFCOUNTER3_HI=100,
    GPUPERFREG_TCF_PERFCOUNTER3_LOW=101,
    GPUPERFREG_TCF_PERFCOUNTER4_SELECT=102,
    GPUPERFREG_TCF_PERFCOUNTER4_HI=103,
    GPUPERFREG_TCF_PERFCOUNTER4_LOW=104,
    GPUPERFREG_TCF_PERFCOUNTER5_SELECT=105,
    GPUPERFREG_TCF_PERFCOUNTER5_HI=106,
    GPUPERFREG_TCF_PERFCOUNTER5_LOW=107,
    GPUPERFREG_TCF_PERFCOUNTER6_SELECT=108,
    GPUPERFREG_TCF_PERFCOUNTER6_HI=109,
    GPUPERFREG_TCF_PERFCOUNTER6_LOW=110,
    GPUPERFREG_TCF_PERFCOUNTER7_SELECT=111,
    GPUPERFREG_TCF_PERFCOUNTER7_HI=112,
    GPUPERFREG_TCF_PERFCOUNTER7_LOW=113,
    GPUPERFREG_TCF_PERFCOUNTER8_SELECT=114,
    GPUPERFREG_TCF_PERFCOUNTER8_HI=115,
    GPUPERFREG_TCF_PERFCOUNTER8_LOW=116,
    GPUPERFREG_TCF_PERFCOUNTER9_SELECT=117,
    GPUPERFREG_TCF_PERFCOUNTER9_HI=118,
    GPUPERFREG_TCF_PERFCOUNTER9_LOW=119,
    GPUPERFREG_TCF_PERFCOUNTER10_SELECT=120,
    GPUPERFREG_TCF_PERFCOUNTER10_HI=121,
    GPUPERFREG_TCF_PERFCOUNTER10_LOW=122,
    GPUPERFREG_TCF_PERFCOUNTER11_SELECT=123,
    GPUPERFREG_TCF_PERFCOUNTER11_HI=124,
    GPUPERFREG_TCF_PERFCOUNTER11_LOW=125,
    GPUPERFREG_PA_SU_PERFCOUNTER0_SELECT=136,
    GPUPERFREG_PA_SU_PERFCOUNTER1_SELECT=137,
    GPUPERFREG_PA_SU_PERFCOUNTER2_SELECT=138,
    GPUPERFREG_PA_SU_PERFCOUNTER3_SELECT=139,
    GPUPERFREG_PA_SU_PERFCOUNTER0_LOW=140,
    GPUPERFREG_PA_SU_PERFCOUNTER0_HI=141,
    GPUPERFREG_PA_SU_PERFCOUNTER1_LOW=142,
    GPUPERFREG_PA_SU_PERFCOUNTER1_HI=143,
    GPUPERFREG_PA_SU_PERFCOUNTER2_LOW=144,
    GPUPERFREG_PA_SU_PERFCOUNTER2_HI=145,
    GPUPERFREG_PA_SU_PERFCOUNTER3_LOW=146,
    GPUPERFREG_PA_SU_PERFCOUNTER3_HI=147,
    GPUPERFREG_RBBM_PERFCOUNTER0_SELECT=149,
    GPUPERFREG_RBBM_PERFCOUNTER1_SELECT=150,
    GPUPERFREG_RBBM_PERFCOUNTER0_LOW=151,
    GPUPERFREG_PA_SC_PERFCOUNTER0_SELECT=152,
    GPUPERFREG_RBBM_PERFCOUNTER0_HI=152,
    GPUPERFREG_PA_SC_PERFCOUNTER1_SELECT=153,
    GPUPERFREG_RBBM_PERFCOUNTER1_LOW=153,
    GPUPERFREG_PA_SC_PERFCOUNTER2_SELECT=154,
    GPUPERFREG_RBBM_PERFCOUNTER1_HI=154,
    GPUPERFREG_PA_SC_PERFCOUNTER3_SELECT=155,
    GPUPERFREG_PA_SC_PERFCOUNTER0_LOW=156,
    GPUPERFREG_PA_SC_PERFCOUNTER0_HI=157,
    GPUPERFREG_PA_SC_PERFCOUNTER1_LOW=158,
    GPUPERFREG_PA_SC_PERFCOUNTER1_HI=159,
    GPUPERFREG_PA_SC_PERFCOUNTER2_LOW=160,
    GPUPERFREG_PA_SC_PERFCOUNTER2_HI=161,
    GPUPERFREG_PA_SC_PERFCOUNTER3_LOW=162,
    GPUPERFREG_PA_SC_PERFCOUNTER3_HI=163,
    GPUPERFREG_SQ_PERFCOUNTER0_SELECT=200,
    GPUPERFREG_SQ_PERFCOUNTER1_SELECT=201,
    GPUPERFREG_SQ_PERFCOUNTER2_SELECT=202,
    GPUPERFREG_SQ_PERFCOUNTER3_SELECT=203,
    GPUPERFREG_SQ_PERFCOUNTER0_LOW=204,
    GPUPERFREG_SQ_PERFCOUNTER0_HI=205,
    GPUPERFREG_SQ_PERFCOUNTER1_LOW=206,
    GPUPERFREG_SQ_PERFCOUNTER1_HI=207,
    GPUPERFREG_SQ_PERFCOUNTER2_LOW=208,
    GPUPERFREG_SQ_PERFCOUNTER2_HI=209,
    GPUPERFREG_SQ_PERFCOUNTER3_LOW=210,
    GPUPERFREG_SQ_PERFCOUNTER3_HI=211,
    GPUPERFREG_SX_PERFCOUNTER0_SELECT=212,
    GPUPERFREG_SX_PERFCOUNTER0_LOW=216,
    GPUPERFREG_SX_PERFCOUNTER0_HI=217,
    GPUPERFREG_CP_PERFCOUNTER0_SELECT=230,
    GPUPERFREG_CP_PERFCOUNTER0_LOW=231,
    GPUPERFREG_CP_PERFCOUNTER0_HI=232,
    GPUPERFREG_CP_PERFMON_CNTL=245,
    GPUPERFREG_VGT_EVENT_INITIATOR=249
} GPUPERFREGISTER;

typedef struct D3DConstantBuffer D3DConstantBuffer, *PD3DConstantBuffer;

struct D3DConstantBuffer {
    int _padding_;
    int _padding_;
    int _padding_;
    int _padding_;
    int _padding_;
    int _padding_;
    ulong Address;
    ulong Size;
};

typedef enum _KINTERRUPT_MODE {
    LevelSensitive=0,
    Latched=1
} _KINTERRUPT_MODE;

typedef enum _D3DXEFFECTDEFAULTTYPE {
    D3DXEDT_STRING=1,
    D3DXEDT_FLOATS=2,
    D3DXEDT_DWORD=3,
    D3DXEDT_FORCEDWORD=255
} _D3DXEFFECTDEFAULTTYPE;

typedef enum __VECTOR_PACK_TYPES {
    VPACK_D3DCOLOR=0,
    VPACK_NORMSHORT2=1,
    VPACK_NORMPACKED32=2,
    VPACK_FLOAT16_2=3,
    VPACK_NORMSHORT4=4,
    VPACK_FLOAT16_4=5,
    VPACK_NORMPACKED64=6
} __VECTOR_PACK_TYPES;

typedef struct D3DSurface D3DSurface, *PD3DSurface;

typedef struct D3DSurface_s_0 D3DSurface_s_0, *PD3DSurface_s_0;

typedef union GPU_SURFACEINFO GPU_SURFACEINFO, *PGPU_SURFACEINFO;

typedef union D3DSurface_u_36 D3DSurface_u_36, *PD3DSurface_u_36;

typedef struct D3DSurface_s_0_s_0 D3DSurface_s_0_s_0, *PD3DSurface_s_0_s_0;

typedef union GPU_DEPTHINFO GPU_DEPTHINFO, *PGPU_DEPTHINFO;

typedef struct GPU_SURFACEINFO_s_0 GPU_SURFACEINFO_s_0, *PGPU_SURFACEINFO_s_0;

typedef struct D3DSurface_u_36_s_0 D3DSurface_u_36_s_0, *PD3DSurface_u_36_s_0;

typedef struct D3DSurface_u_36_s_1 D3DSurface_u_36_s_1, *PD3DSurface_u_36_s_1;

typedef union GPU_COLORINFO GPU_COLORINFO, *PGPU_COLORINFO;

typedef union D3DSurface_s_0_s_0_u_24 D3DSurface_s_0_s_0_u_24, *PD3DSurface_s_0_s_0_u_24;

typedef struct GPU_DEPTHINFO_s_0 GPU_DEPTHINFO_s_0, *PGPU_DEPTHINFO_s_0;

typedef enum _D3DFORMAT {
    D3DFMT_LE_INDEX16=0,
    D3DFMT_INDEX16=1,
    D3DFMT_A8=2,
    D3DFMT_L8=2,
    D3DFMT_LIN_A8=2,
    D3DFMT_LIN_L8=2,
    D3DFMT_LE_INDEX32=4,
    D3DFMT_INDEX32=6,
    D3DFMT_LE_A8R8G8B8=6,
    D3DFMT_LE_X8R8G8B8=6,
    D3DFMT_VERTEXDATA=8,
    D3DFMT_LE_LIN_YUY2=11,
    D3DFMT_LE_YUY2=11,
    D3DFMT_LE_LIN_UYVY=12,
    D3DFMT_LE_UYVY=12,
    D3DFMT_LE_A2R10G10B10=54,
    D3DFMT_LE_X2R10G10B10=54,
    D3DFMT_A1R5G5B5=67,
    D3DFMT_LIN_A1R5G5B5=67,
    D3DFMT_LIN_X1R5G5B5=67,
    D3DFMT_X1R5G5B5=67,
    D3DFMT_LIN_R5G6B5=68,
    D3DFMT_R5G6B5=68,
    D3DFMT_L6V5U5=69,
    D3DFMT_LIN_L6V5U5=69,
    D3DFMT_LIN_R6G5B5=69,
    D3DFMT_R6G5B5=69,
    D3DFMT_A8L8=74,
    D3DFMT_G8R8=74,
    D3DFMT_LIN_A8L8=74,
    D3DFMT_LIN_G8R8=74,
    D3DFMT_LIN_V8U8=74,
    D3DFMT_V8U8=74,
    D3DFMT_LIN_R8G8_B8G8=75,
    D3DFMT_LIN_YUY2=75,
    D3DFMT_R8G8_B8G8=75,
    D3DFMT_YUY2=75,
    D3DFMT_G8R8_G8B8=76,
    D3DFMT_LIN_G8R8_G8B8=76,
    D3DFMT_LIN_UYVY=76,
    D3DFMT_UYVY=76,
    D3DFMT_A4R4G4B4=79,
    D3DFMT_LIN_A4R4G4B4=79,
    D3DFMT_LIN_Q4W4V4U4=79,
    D3DFMT_LIN_X4R4G4B4=79,
    D3DFMT_Q4W4V4U4=79,
    D3DFMT_X4R4G4B4=79,
    D3DFMT_DXT1=82,
    D3DFMT_LIN_DXT1=82,
    D3DFMT_DXT2=83,
    D3DFMT_DXT3=83,
    D3DFMT_LIN_DXT2=83,
    D3DFMT_LIN_DXT3=83,
    D3DFMT_DXT4=84,
    D3DFMT_DXT5=84,
    D3DFMT_LIN_DXT4=84,
    D3DFMT_LIN_DXT5=84,
    D3DFMT_A16B16G16R16_EDRAM=85,
    D3DFMT_D16=88,
    D3DFMT_L16=88,
    D3DFMT_LIN_D16=88,
    D3DFMT_LIN_L16=88,
    D3DFMT_A16B16G16R16=90,
    D3DFMT_LIN_A16B16G16R16=90,
    D3DFMT_LIN_Q16W16V16U16=90,
    D3DFMT_Q16W16V16U16=90,
    D3DFMT_LIN_R16F_EXPAND=91,
    D3DFMT_R16F_EXPAND=91,
    D3DFMT_A16B16G16R16F_EXPAND=93,
    D3DFMT_LIN_A16B16G16R16F_EXPAND=93,
    D3DFMT_LIN_R16F=94,
    D3DFMT_R16F=94,
    D3DFMT_A16B16G16R16F=96,
    D3DFMT_LIN_A16B16G16R16F=96,
    D3DFMT_DXN=113,
    D3DFMT_LIN_DXN=113,
    D3DFMT_DXT3A=122,
    D3DFMT_LIN_DXT3A=122,
    D3DFMT_DXT5A=123,
    D3DFMT_LIN_DXT5A=123,
    D3DFMT_CTX1=124,
    D3DFMT_LIN_CTX1=124,
    D3DFMT_DXT3A_1111=125,
    D3DFMT_LIN_DXT3A_1111=125,
    D3DFMT_A8B8G8R8=134,
    D3DFMT_A8R8G8B8=134,
    D3DFMT_LIN_A8B8G8R8=134,
    D3DFMT_LIN_A8R8G8B8=134,
    D3DFMT_LIN_Q8W8V8U8=134,
    D3DFMT_LIN_X8B8G8R8=134,
    D3DFMT_LIN_X8L8V8U8=134,
    D3DFMT_LIN_X8R8G8B8=134,
    D3DFMT_Q8W8V8U8=134,
    D3DFMT_X8B8G8R8=134,
    D3DFMT_X8L8V8U8=134,
    D3DFMT_X8R8G8B8=134,
    D3DFMT_G16R16_EDRAM=141,
    D3DFMT_D24S8=150,
    D3DFMT_D24X8=150,
    D3DFMT_LIN_D24S8=150,
    D3DFMT_LIN_D24X8=150,
    D3DFMT_D24FS8=151,
    D3DFMT_LIN_D24FS8=151,
    D3DFMT_A16L16=153,
    D3DFMT_G16R16=153,
    D3DFMT_LIN_A16L16=153,
    D3DFMT_LIN_G16R16=153,
    D3DFMT_LIN_V16U16=153,
    D3DFMT_V16U16=153,
    D3DFMT_G16R16F_EXPAND=156,
    D3DFMT_LIN_G16R16F_EXPAND=156,
    D3DFMT_G16R16F=159,
    D3DFMT_LIN_G16R16F=159,
    D3DFMT_D32=161,
    D3DFMT_L32=161,
    D3DFMT_LIN_D32=161,
    D3DFMT_LIN_L32=161,
    D3DFMT_A32L32=162,
    D3DFMT_G32R32=162,
    D3DFMT_LIN_A32L32=162,
    D3DFMT_LIN_G32R32=162,
    D3DFMT_LIN_V32U32=162,
    D3DFMT_V32U32=162,
    D3DFMT_A32B32G32R32=163,
    D3DFMT_LIN_A32B32G32R32=163,
    D3DFMT_LIN_Q32W32V32U32=163,
    D3DFMT_Q32W32V32U32=163,
    D3DFMT_LIN_R32F=164,
    D3DFMT_R32F=164,
    D3DFMT_G32R32F=165,
    D3DFMT_LIN_G32R32F=165,
    D3DFMT_A32B32G32R32F=166,
    D3DFMT_LIN_A32B32G32R32F=166,
    D3DFMT_A2B10G10R10=182,
    D3DFMT_A2R10G10B10=182,
    D3DFMT_A2W10V10U10=182,
    D3DFMT_LIN_A2B10G10R10=182,
    D3DFMT_LIN_A2R10G10B10=182,
    D3DFMT_LIN_A2W10V10U10=182,
    D3DFMT_LIN_X2R10G10B10=182,
    D3DFMT_X2R10G10B10=182,
    D3DFMT_LIN_R10G11B11=183,
    D3DFMT_LIN_W10V11U11=183,
    D3DFMT_R10G11B11=183,
    D3DFMT_W10V11U11=183,
    D3DFMT_LIN_R11G11B10=184,
    D3DFMT_LIN_W11V11U10=184,
    D3DFMT_R11G11B10=184,
    D3DFMT_W11V11U10=184,
    D3DFMT_A2B10G10R10F_EDRAM=191,
    D3DFMT_FORCE_DWORD=255,
    D3DFMT_UNKNOWN=255
} _D3DFORMAT;

typedef struct GPU_COLORINFO_s_0 GPU_COLORINFO_s_0, *PGPU_COLORINFO_s_0;

typedef struct D3DSurface_s_0_s_0_u_24_s_0 D3DSurface_s_0_s_0_u_24_s_0, *PD3DSurface_s_0_s_0_u_24_s_0;

typedef struct D3DSurface_s_0_s_0_u_24_s_1 D3DSurface_s_0_s_0_u_24_s_1, *PD3DSurface_s_0_s_0_u_24_s_1;

typedef union GPU_HICONTROL GPU_HICONTROL, *PGPU_HICONTROL;

typedef struct D3DBaseTexture D3DBaseTexture, *PD3DBaseTexture;

typedef union D3DSurface_s_0_s_0_u_24_s_1_u_4 D3DSurface_s_0_s_0_u_24_s_1_u_4, *PD3DSurface_s_0_s_0_u_24_s_1_u_4;

typedef struct GPU_HICONTROL_s_0 GPU_HICONTROL_s_0, *PGPU_HICONTROL_s_0;

typedef union GPUTEXTURE_FETCH_CONSTANT GPUTEXTURE_FETCH_CONSTANT, *PGPUTEXTURE_FETCH_CONSTANT;

typedef struct D3DSurface_s_0_s_0_u_24_s_1_u_4_s_0 D3DSurface_s_0_s_0_u_24_s_1_u_4_s_0, *PD3DSurface_s_0_s_0_u_24_s_1_u_4_s_0;

typedef struct GPUTEXTURE_FETCH_CONSTANT_s_0 GPUTEXTURE_FETCH_CONSTANT_s_0, *PGPUTEXTURE_FETCH_CONSTANT_s_0;

typedef struct GPUTEXTURE_FETCH_CONSTANT_s_0_s_0 GPUTEXTURE_FETCH_CONSTANT_s_0_s_0, *PGPUTEXTURE_FETCH_CONSTANT_s_0_s_0;

typedef union <unnamed-type-Size> <unnamed-type-Size>, *P<unnamed-type-Size>;

typedef struct GPUTEXTURE_FETCH_CONSTANT_s_0_s_0_s_0 GPUTEXTURE_FETCH_CONSTANT_s_0_s_0_s_0, *PGPUTEXTURE_FETCH_CONSTANT_s_0_s_0_s_0;

typedef struct GPUTEXTURESIZE_1D GPUTEXTURESIZE_1D, *PGPUTEXTURESIZE_1D;

typedef struct GPUTEXTURESIZE_2D GPUTEXTURESIZE_2D, *PGPUTEXTURESIZE_2D;

typedef struct GPUTEXTURESIZE_3D GPUTEXTURESIZE_3D, *PGPUTEXTURESIZE_3D;

typedef struct GPUTEXTURESIZE_STACK GPUTEXTURESIZE_STACK, *PGPUTEXTURESIZE_STACK;

typedef union GPUTEXTURE_FETCH_CONSTANT_s_0_s_0_s_0_u_4 GPUTEXTURE_FETCH_CONSTANT_s_0_s_0_s_0_u_4, *PGPUTEXTURE_FETCH_CONSTANT_s_0_s_0_s_0_u_4;

typedef struct GPUTEXTURE_FETCH_CONSTANT_s_0_s_0_s_0_u_4_s_0 GPUTEXTURE_FETCH_CONSTANT_s_0_s_0_s_0_u_4_s_0, *PGPUTEXTURE_FETCH_CONSTANT_s_0_s_0_s_0_u_4_s_0;

typedef struct GPUTEXTURE_FETCH_CONSTANT_s_0_s_0_s_0_u_4_s_1 GPUTEXTURE_FETCH_CONSTANT_s_0_s_0_s_0_u_4_s_1, *PGPUTEXTURE_FETCH_CONSTANT_s_0_s_0_s_0_u_4_s_1;

struct GPUTEXTURESIZE_2D {
    undefined field0_0x0;
    ulong Height:13;
    ulong Width:13;
};

struct GPUTEXTURESIZE_1D {
    ulong Width:24;
};

struct GPUTEXTURESIZE_3D {
    undefined field0_0x0;
    undefined field1_0x1;
    ulong Depth:10;
    ulong Height:11;
    ulong Width:11;
};

struct GPUTEXTURESIZE_STACK {
    undefined field0_0x0;
    undefined field1_0x1;
    undefined field2_0x2;
    ulong Depth:6;
    ulong Height:13;
    ulong Width:13;
};

union <unnamed-type-Size> {
    struct GPUTEXTURESIZE_1D OneD;
    struct GPUTEXTURESIZE_2D TwoD;
    struct GPUTEXTURESIZE_3D ThreeD;
    struct GPUTEXTURESIZE_STACK Stack;
};

struct GPUTEXTURE_FETCH_CONSTANT_s_0_s_0_s_0_u_4_s_0 {
    undefined field0_0x0;
    undefined field1_0x1;
    undefined field2_0x2;
    undefined field3_0x3;
    union <unnamed-type-Size> Size;
};

struct GPUTEXTURE_FETCH_CONSTANT_s_0_s_0_s_0_u_4_s_1 {
    ulong SwizzleX:3;
    undefined field1_0x1;
    undefined field2_0x2;
    undefined field3_0x3;
    undefined field4_0x4;
    undefined field5_0x5;
    undefined field6_0x6;
    undefined field7_0x7;
    ulong SwizzleY:3;
    ulong SwizzleZ:3;
    ulong MinFilter:2;
    ulong BorderSize:1;
    ulong MinMipLevel:4;
    ulong MaxMipLevel:4;
    int LODBias:10;
    int GradExpAdjustV:5;
    int GradExpAdjustH:5;
    ulong MinAnisoWalk:1;
    ulong MagAnisoWalk:1;
    ulong VolMinFilter:1;
    ulong VolMagFilter:1;
    ulong MipFilter:2;
    ulong AnisoFilter:3;
    int ExpAdjust:6;
    ulong MagFilter:2;
    ulong SwizzleW:3;
};

union GPUTEXTURE_FETCH_CONSTANT_s_0_s_0_s_0_u_4 {
    struct GPUTEXTURE_FETCH_CONSTANT_s_0_s_0_s_0_u_4_s_0 _s_0;
    struct GPUTEXTURE_FETCH_CONSTANT_s_0_s_0_s_0_u_4_s_1 _s_1;
};

struct GPUTEXTURE_FETCH_CONSTANT_s_0_s_0_s_0 {
    int _padding_;
    union GPUTEXTURE_FETCH_CONSTANT_s_0_s_0_s_0_u_4 field1_0x4;
};

struct GPUTEXTURE_FETCH_CONSTANT_s_0_s_0 {
    struct GPUTEXTURE_FETCH_CONSTANT_s_0_s_0_s_0 field0_0x0;
    ulong NumFormat:1;
    undefined field2_0x2;
    undefined field3_0x3;
    undefined field4_0x4;
    undefined field5_0x5;
    union <unnamed-type-Size> Size;
};

struct GPUTEXTURE_FETCH_CONSTANT_s_0 {
    struct GPUTEXTURE_FETCH_CONSTANT_s_0_s_0 field0_0x0;
    ulong ClampY:3;
    ulong ClampX:3;
    ulong SignW:2;
    undefined field4_0x3;
    ulong Tiled:1;
    ulong Endian:2;
    ulong DataFormat:6;
    ulong BaseAddress:20;
    union <unnamed-type-Size> Size;
    ulong ClampPolicy:1;
    ulong Stacked:1;
    ulong RequestSize:2;
    ulong Pitch:9;
    ulong TriClamp:2;
    ulong ForceBCWToMax:1;
    ulong MipAddress:20;
    int AnisoBias:4;
    ulong PackedMips:1;
    ulong Dimension:2;
    ulong BorderColor:2;
    ulong ClampZ:3;
};

struct GPU_COLORINFO_s_0 {
    undefined field0_0x0;
    undefined field1_0x1;
    ulong ColorExpBias:6;
    ulong ColorFormat:4;
    ulong ColorBase:12;
};

union GPU_COLORINFO {
    struct GPU_COLORINFO_s_0 _s_0;
    ulong dword;
};

struct GPU_HICONTROL_s_0 {
    ulong HiStencilFunc:1;
    ulong HiZFunc:1;
    ulong HiStencilEnable:1;
    ulong HiStencilWriteEnable:1;
    ulong HiZEnable:1;
    ulong HiZWriteEnable:1;
    ulong HiStencilRef:8;
    ulong HiBaseAddr:15;
    undefined field8_0x4;
};

struct GPU_SURFACEINFO_s_0 {
    undefined field0_0x0;
    undefined field1_0x1;
    ulong HiZPitch:14;
    ulong MsaaSamples:2;
    ulong SurfacePitch:14;
};

union GPU_SURFACEINFO {
    struct GPU_SURFACEINFO_s_0 _s_0;
    ulong dword;
};

struct GPU_DEPTHINFO_s_0 {
    undefined field0_0x0;
    undefined field1_0x1;
    ulong DisableHZClamp:1;
    ulong DepthFormat:1;
    ulong DepthBase:12;
};

union GPU_DEPTHINFO {
    struct GPU_DEPTHINFO_s_0 _s_0;
    ulong dword;
};

struct D3DSurface_s_0_s_0_u_24_s_1_u_4_s_0 {
    undefined field0_0x0;
    undefined field1_0x1;
    undefined field2_0x2;
    ulong MipLevel:4;
};

union D3DSurface_s_0_s_0_u_24_s_1_u_4 {
    struct D3DSurface_s_0_s_0_u_24_s_1_u_4_s_0 _s_0;
    ulong ArrayIndex:6;
};

struct D3DSurface_s_0_s_0_u_24_s_1 {
    struct D3DBaseTexture *Parent;
    union D3DSurface_s_0_s_0_u_24_s_1_u_4 field1_0x4;
};

union GPU_HICONTROL {
    struct GPU_HICONTROL_s_0 _s_0;
    ulong dword;
};

struct D3DSurface_s_0_s_0_u_24_s_0 {
    union GPU_SURFACEINFO SurfaceInfo;
    char _padding_;
    union GPU_HICONTROL HiControl;
};

union D3DSurface_s_0_s_0_u_24 {
    struct D3DSurface_s_0_s_0_u_24_s_0 _s_0;
    struct D3DSurface_s_0_s_0_u_24_s_1 _s_1;
};

struct D3DSurface_s_0_s_0 {
    undefined field0_0x0;
    undefined field1_0x1;
    undefined field2_0x2;
    undefined field3_0x3;
    union GPU_COLORINFO ColorInfo;
    undefined field5_0xb;
    undefined field6_0xc;
    undefined field7_0xd;
    undefined field8_0xe;
    undefined field9_0xf;
    undefined field10_0x10;
    undefined field11_0x11;
    undefined field12_0x12;
    undefined field13_0x13;
    undefined field14_0x14;
    undefined field15_0x15;
    undefined field16_0x16;
    undefined field17_0x17;
    union D3DSurface_s_0_s_0_u_24 field18_0x18;
};

struct D3DSurface_s_0 {
    struct D3DSurface_s_0_s_0 field0_0x0;
    union GPU_DEPTHINFO DepthInfo;
    undefined field2_0x6;
    undefined field3_0x7;
    undefined field4_0x8;
    undefined field5_0x9;
    undefined field6_0xa;
    undefined field7_0xb;
    undefined field8_0xc;
    undefined field9_0xd;
    undefined field10_0xe;
    undefined field11_0xf;
    undefined field12_0x10;
    undefined field13_0x11;
    undefined field14_0x12;
    undefined field15_0x13;
    undefined field16_0x14;
    undefined field17_0x15;
    undefined field18_0x16;
    undefined field19_0x17;
    undefined field20_0x18;
    undefined field21_0x19;
    union GPU_SURFACEINFO SurfaceInfo;
};

struct D3DSurface_u_36_s_0 {
    undefined field0_0x0;
    undefined field1_0x1;
    ulong Width:14;
    undefined field3_0x4;
    undefined field4_0x5;
    undefined field5_0x6;
};

struct D3DSurface_u_36_s_1 {
    ulong Height:15;
    undefined field1_0x3;
    enum _D3DFORMAT Format;
};

union D3DSurface_u_36 {
    struct D3DSurface_u_36_s_0 _s_0;
    struct D3DSurface_u_36_s_1 _s_1;
};

struct D3DSurface {
    struct D3DSurface_s_0 field0_0x0;
    undefined field1_0x1;
    undefined field2_0x2;
    undefined field3_0x3;
    undefined field4_0x4;
    undefined field5_0x5;
    undefined field6_0x6;
    undefined field7_0x7;
    undefined field8_0x8;
    undefined field9_0x9;
    undefined field10_0xa;
    undefined field11_0xb;
    undefined field12_0xc;
    undefined field13_0xd;
    undefined field14_0xe;
    undefined field15_0xf;
    undefined field16_0x10;
    undefined field17_0x11;
    undefined field18_0x12;
    undefined field19_0x13;
    undefined field20_0x14;
    union GPU_SURFACEINFO SurfaceInfo;
    undefined field22_0x1c;
    undefined field23_0x1d;
    undefined field24_0x1e;
    undefined field25_0x1f;
    undefined field26_0x20;
    undefined field27_0x21;
    undefined field28_0x22;
    undefined field29_0x23;
    union D3DSurface_u_36 field30_0x24;
    undefined field31_0x2b;
    ulong Size;
};

union GPUTEXTURE_FETCH_CONSTANT {
    struct GPUTEXTURE_FETCH_CONSTANT_s_0 _s_0;
    ulong dword[6];
};

struct D3DBaseTexture {
    undefined field0_0x0;
    undefined field1_0x1;
    undefined field2_0x2;
    undefined field3_0x3;
    undefined field4_0x4;
    undefined field5_0x5;
    undefined field6_0x6;
    undefined field7_0x7;
    undefined field8_0x8;
    undefined field9_0x9;
    undefined field10_0xa;
    undefined field11_0xb;
    undefined field12_0xc;
    undefined field13_0xd;
    undefined field14_0xe;
    undefined field15_0xf;
    undefined field16_0x10;
    undefined field17_0x11;
    undefined field18_0x12;
    undefined field19_0x13;
    undefined field20_0x14;
    undefined field21_0x15;
    undefined field22_0x16;
    undefined field23_0x17;
    ulong MipFlush;
    union GPUTEXTURE_FETCH_CONSTANT Format;
};

typedef union CG_INT_MASK_REG CG_INT_MASK_REG, *PCG_INT_MASK_REG;


/* WARNING! conflicting data type names: /iw5mp_xenon.pdb/CG_INT_MASK_REG/<unnamed-type-bitfields> - /iw5mp_xenon.pdb/D1SCL_COEF_RAM_SELECT/<unnamed-type-bitfields> */

union CG_INT_MASK_REG {
    struct <unnamed-type-bitfields> bitfields;
    struct <unnamed-type-bitfields> bits;
    uint u32All;
    int i32All;
    float f32All;
};

typedef union GPUVERTEX_FETCH_CONSTANT GPUVERTEX_FETCH_CONSTANT, *PGPUVERTEX_FETCH_CONSTANT;

typedef struct GPUVERTEX_FETCH_CONSTANT_s_0 GPUVERTEX_FETCH_CONSTANT_s_0, *PGPUVERTEX_FETCH_CONSTANT_s_0;

struct GPUVERTEX_FETCH_CONSTANT_s_0 {
    ulong BaseAddress:30;
    undefined field1_0x4;
    undefined field2_0x5;
    undefined field3_0x6;
    ulong ClampDisable:2;
    ulong RequestSize:2;
    ulong AddressClamp:1;
    ulong Size:24;
    ulong Endian:2;
    ulong Type:2;
};

union GPUVERTEX_FETCH_CONSTANT {
    struct GPUVERTEX_FETCH_CONSTANT_s_0 _s_0;
    ulong dword[2];
};

typedef struct _D3DXSHADER_WRITEINFO _D3DXSHADER_WRITEINFO, *P_D3DXSHADER_WRITEINFO;

struct _D3DXSHADER_WRITEINFO {
    ulong Instruction;
    ushort Component[4];
};

typedef enum _D3DRESOURCETYPE {
    D3DRTYPE_NONE=0,
    D3DRTYPE_VERTEXBUFFER=1,
    D3DRTYPE_INDEXBUFFER=2,
    D3DRTYPE_TEXTURE=3,
    D3DRTYPE_SURFACE=4,
    D3DRTYPE_VERTEXDECLARATION=5,
    D3DRTYPE_VERTEXSHADER=6,
    D3DRTYPE_PIXELSHADER=7,
    D3DRTYPE_CONSTANTBUFFER=8,
    D3DRTYPE_COMMANDBUFFER=9,
    D3DRTYPE_ASYNCCOMMANDBUFFERCALL=10,
    D3DRTYPE_PERFCOUNTERBATCH=11,
    D3DRTYPE_OCCLUSIONQUERYBATCH=12,
    D3DRTYPE_VOLUME=16,
    D3DRTYPE_VOLUMETEXTURE=17,
    D3DRTYPE_CUBETEXTURE=18,
    D3DRTYPE_ARRAYTEXTURE=19,
    D3DRTYPE_LINETEXTURE=20,
    D3DRTYPE_FORCE_DWORD=255
} _D3DRESOURCETYPE;

typedef enum _D3DSHADER_MISCTYPE_OFFSETS {
    D3DSMO_POSITION=0,
    D3DSMO_FACE=1
} _D3DSHADER_MISCTYPE_OFFSETS;

typedef struct _D3DBOX _D3DBOX, *P_D3DBOX;

struct _D3DBOX {
    uint Left;
    uint Top;
    uint Right;
    uint Bottom;
    uint Front;
    uint Back;
};

typedef enum _ARGON_EEPROM_FIELD {
    ArgonEepromErrorBitfield=0,
    ArgonEepromV12P0=1,
    ArgonEepromAll=2
} _ARGON_EEPROM_FIELD;

typedef struct XAUDIOFXINIT XAUDIOFXINIT, *PXAUDIOFXINIT;

struct XAUDIOFXINIT {
    uchar EffectId;
    void *pContext;
};

typedef enum <unnamed-tag_00001163> {
    SPR_DCIDR0=186
} <unnamed-tag_00001163>;

typedef enum <unnamed-tag_00001187> {
    SPR_HID0=240
} <unnamed-tag_00001187>;

typedef enum _XPROFILE_VOICE_THRU_SPEAKERS_OPTIONS {
    XPROFILE_VOICE_THRU_SPEAKERS_OFF=0,
    XPROFILE_VOICE_THRU_SPEAKERS_ON=1,
    XPROFILE_VOICE_THRU_SPEAKERS_BOTH=2
} _XPROFILE_VOICE_THRU_SPEAKERS_OPTIONS;

typedef enum _SUITE_TYPE {
    SmallBusiness=0,
    Enterprise=1,
    BackOffice=2,
    CommunicationServer=3,
    TerminalServer=4,
    SmallBusinessRestricted=5,
    EmbeddedNT=6,
    DataCenter=7,
    SingleUserTS=8,
    MaxSuiteType=9
} _SUITE_TYPE;

typedef struct _KTRAP_FRAME _KTRAP_FRAME, *P_KTRAP_FRAME;

typedef struct _STACK_FRAME_HEADER _STACK_FRAME_HEADER, *P_STACK_FRAME_HEADER;

typedef union _KTRAP_FRAME_u_336 _KTRAP_FRAME_u_336, *P_KTRAP_FRAME_u_336;

typedef struct _KTRAP_FRAME_u_336_s_1 _KTRAP_FRAME_u_336_s_1, *P_KTRAP_FRAME_u_336_s_1;

typedef struct _KINTERRUPT _KINTERRUPT, *P_KINTERRUPT;

struct _KINTERRUPT {
    void (*ServiceRoutine)(struct _KINTERRUPT *, void *);
    void *ServiceContext;
    ulong SpinLock;
    ulong ServiceCount;
    uchar BusIrql;
    uchar Irql;
    uchar Connected:1;
    uchar Mode:7;
    uchar TargetNumber;
};

struct _KTRAP_FRAME_u_336_s_1 {
    uchar OldIrql;
    struct _KINTERRUPT *InterruptObject;
    uchar InterruptIrql;
};

struct _STACK_FRAME_HEADER {
    ulong BackChain;
    ulong CompilerReserved;
    ulong CalleeOwned1;
    ulong CalleeOwned2;
    ulong64 Parameter0;
    ulong64 Parameter1;
    ulong64 Parameter2;
    ulong64 Parameter3;
    ulong64 Parameter4;
    ulong64 Parameter5;
    ulong64 Parameter6;
    ulong64 Parameter7;
};

union _KTRAP_FRAME_u_336 {
    uchar ExceptionRecord[80];
    struct _KTRAP_FRAME_u_336_s_1 _s_1;
};

struct _KTRAP_FRAME {
    struct _STACK_FRAME_HEADER Header;
    ulong64 Gpr0;
    ulong64 Gpr1;
    ulong64 Gpr2;
    ulong64 Gpr3;
    ulong64 Gpr4;
    ulong64 Gpr5;
    ulong64 Gpr6;
    ulong64 Gpr7;
    ulong64 Gpr8;
    ulong64 Gpr9;
    ulong64 Gpr10;
    ulong64 Gpr11;
    ulong64 Gpr12;
    ulong64 Gpr13;
    ulong64 Gpr14;
    ulong64 Gpr15;
    ulong64 Gpr16;
    ulong64 Gpr17;
    ulong64 Gpr18;
    ulong64 Gpr19;
    ulong64 Gpr20;
    ulong64 Gpr21;
    ulong64 Gpr22;
    ulong64 Gpr23;
    ulong64 Gpr24;
    ulong64 Gpr25;
    ulong64 Gpr26;
    ulong64 Gpr27;
    ulong64 Gpr28;
    ulong64 Gpr29;
    ulong64 Gpr30;
    ulong64 Gpr31;
    union _KTRAP_FRAME_u_336 field33_0x150;
    ulong64 Ctr;
    ulong Lr;
    ulong Iar;
    ulong Cr;
    ulong Msr;
    ulong Xer;
    ulong UserModeControl;
};

typedef struct _SID_AND_ATTRIBUTES _SID_AND_ATTRIBUTES, *P_SID_AND_ATTRIBUTES;

struct _SID_AND_ATTRIBUTES {
    void *Sid;
    ulong Attributes;
};

typedef struct D3DCommandBuffer D3DCommandBuffer, *PD3DCommandBuffer;

struct D3DCommandBuffer {
    int _padding_;
    int _padding_;
    int _padding_;
    int _padding_;
    int _padding_;
    struct _D3DTAGCOLLECTION m_Inherited;
    struct _D3DTAGCOLLECTION m_Persisted;
};

typedef enum _D3DXMESHOPT {
    D3DXMESHOPTINT_ALREADYATTRSORTED=0,
    D3DXMESHOPTINT_INVERTFACEREMAP=0,
    D3DXMESHOPTINT_PMESHSAVE=0,
    D3DXMESHOPTINT_VALIDBITS=0,
    D3DXMESHOPT_ATTRSORT=0,
    D3DXMESHOPT_COMPACT=0,
    D3DXMESHOPT_DEVICEINDEPENDENT=0,
    D3DXMESHOPT_DONOTSPLIT=0,
    D3DXMESHOPT_IGNOREVERTS=0,
    D3DXMESHOPT_STRIPREORDER=0,
    D3DXMESHOPT_VALIDBITS=0,
    D3DXMESHOPT_VERTEXCACHE=0
} _D3DXMESHOPT;

typedef union PCIE_VC0_RESOURCE_STATUS PCIE_VC0_RESOURCE_STATUS, *PPCIE_VC0_RESOURCE_STATUS;


/* WARNING! conflicting data type names: /iw5mp_xenon.pdb/PCIE_VC0_RESOURCE_STATUS/<unnamed-type-bitfields> - /iw5mp_xenon.pdb/D1SCL_COEF_RAM_SELECT/<unnamed-type-bitfields> */

union PCIE_VC0_RESOURCE_STATUS {
    struct <unnamed-type-bitfields> bitfields;
    struct <unnamed-type-bitfields> bits;
    uint u32All;
    int i32All;
    float f32All;
};

typedef enum <unnamed-tag_000010F5> {
    SPR_CTRL_W=152
} <unnamed-tag_000010F5>;

typedef union GPUFETCH_CONSTANT GPUFETCH_CONSTANT, *PGPUFETCH_CONSTANT;

union GPUFETCH_CONSTANT {
    union GPUTEXTURE_FETCH_CONSTANT Texture;
    undefined1 Vertex[24];
};

typedef struct _D3DPWLGAMMA _D3DPWLGAMMA, *P_D3DPWLGAMMA;

typedef struct _D3DPWLGAMMAENTRY _D3DPWLGAMMAENTRY, *P_D3DPWLGAMMAENTRY;

struct _D3DPWLGAMMAENTRY {
    ushort Base;
    ushort Delta;
};

struct _D3DPWLGAMMA {
    struct _D3DPWLGAMMAENTRY red[128];
    struct _D3DPWLGAMMAENTRY green[128];
    struct _D3DPWLGAMMAENTRY blue[128];
};

typedef enum PIX_INDEX_TYPE {
    IndexType_FrameStart=0,
    IndexType_FrameEnd=1,
    IndexType_Straggler=2,
    IndexType_GpuTiming=3,
    IndexType_Video=4,
    IndexType_Audio=5
} PIX_INDEX_TYPE;

typedef enum _D3DSHADER_COMPARISON {
    D3DSPC_RESERVED0=0,
    D3DSPC_GT=1,
    D3DSPC_EQ=2,
    D3DSPC_GE=3,
    D3DSPC_LT=4,
    D3DSPC_NE=5,
    D3DSPC_LE=6,
    D3DSPC_RESERVED1=7
} _D3DSHADER_COMPARISON;

typedef enum GPUPERFEVENT_MC {
    GPUPE_RANK_BANK0_ACCESSES=0,
    GPUPE_RANK_BANK1_ACCESSES=1,
    GPUPE_RANK_BANK2_ACCESSES=2,
    GPUPE_RANK_BANK3_ACCESSES=3,
    GPUPE_RANK_BANK4_ACCESSES=4,
    GPUPE_RANK_BANK5_ACCESSES=5,
    GPUPE_RANK_BANK6_ACCESSES=6,
    GPUPE_RANK_BANK7_ACCESSES=7,
    GPUPE_RANK_BANK8_ACCESSES=8,
    GPUPE_RANK_BANK9_ACCESSES=9,
    GPUPE_RANK_BANK10_ACCESSES=10,
    GPUPE_RANK_BANK11_ACCESSES=11,
    GPUPE_RANK_BANK12_ACCESSES=12,
    GPUPE_RANK_BANK13_ACCESSES=13,
    GPUPE_RANK_BANK14_ACCESSES=14,
    GPUPE_RANK_BANK15_ACCESSES=15,
    GPUPE_READ_2_WRITE=16,
    GPUPE_WRITE_2_READ=17,
    GPUPE_NEW_PAGE_ACCESSES=18,
    GPUPE_TOTAL_ACCESSES=19,
    GPUPE_READ_ACCESSES=20,
    GPUPE_ACCESS_PRESENT_NO_ISSUE_CLKS=21,
    GPUPE_CMD_PRESENT_NO_XFER_CLKS=22,
    GPUPE_URGENT_DC_ACCESSES=23,
    GPUPE_URGENT_SB_ACCESSES=24,
    GPUPE_URGENT_BIU_ACCESSES=25,
    GPUPE_NEW_BIUF_ACCESSES=26,
    GPUPE_NEW_CP_ACCESSES=27,
    GPUPE_NEW_TC_ACCESSES=28,
    GPUPE_NEW_VC_ACCESSES=29,
    GPUPE_NEW_BC_CP_ACCESSES=30,
    GPUPE_NEW_BC_EX_ACCESSES=31,
    GPUPE_NEW_VGT_ACCESSES=32,
    GPUPE_NEW_DC_ACCESSES=33,
    GPUPE_NEW_SB_ACCESSES=34,
    GPUPE_NEW_BIUS_ACCESSES=35,
    GPUPE_BIUS_READ_ACCESSES=36,
    GPUPE_SB_READ_ACCESSES=37,
    GPUPE_CP_READ_ACCESSES=38,
    GPUPE_NTH_SMPG_ACCESS_IS_BIUF=39,
    GPUPE_NTH_SMPG_ACCESS_IS_CP=40,
    GPUPE_NTH_SMPG_ACCESS_IS_TC=41,
    GPUPE_NTH_SMPG_ACCESS_IS_VC=42,
    GPUPE_NTH_SMPG_ACCESS_IS_BC_CP=43,
    GPUPE_NTH_SMPG_ACCESS_IS_BC_EX=44,
    GPUPE_NTH_SMPG_ACCESS_IS_VGT=45,
    GPUPE_NTH_SMPG_ACCESS_IS_DC=46,
    GPUPE_NTH_SMPG_ACCESS_IS_SB=47,
    GPUPE_NTH_SMPG_ACCESS_IS_BIUS=48,
    GPUPE_N_VALID_ENTRY_IN_TEXTURE_CAM_CLKS=49,
    GPUPE_N_VALID_ENTRY_IN_VC_CAM_CLKS=50,
    GPUPE_N_VALID_ENTRY_IN_BIUS_CAM_CLKS=51,
    GPUPE_N_SB_BUF_USED_CLKS=52,
    GPUPE_N_TC_BUF_USED_CLKS=53,
    GPUPE_N_VC_BUF_USED_CLKS=54,
    GPUPE_N_DC_BUF_USED_CLKS=55,
    GPUPE_N_VGT_BUF_USED_CLK=56,
    GPUPE_N_CP_BUF_USED_CLKS=57
} GPUPERFEVENT_MC;

typedef enum GPUPERFEVENT_MH {
    GPUPE_CP_READ_MEMORY=0,
    GPUPE_CP_READ_PGLB=1,
    GPUPE_CP_WRITE_MEMORY=2,
    GPUPE_CP_WRITE_SNOOPED=3,
    GPUPE_CP_WRITE_WRITEBACK=4,
    GPUPE_MH_CP_RTR=5,
    GPUPE_VGT_READ_MEMORY=6,
    GPUPE_VGT_READ_PGLB=7,
    GPUPE_MH_VGT_RTR=8,
    GPUPE_IOC_READ=9,
    GPUPE_IOC_WRITE=10,
    GPUPE_IOC_READ_BYTE_COUNT=11,
    GPUPE_IOC_WRITE_BYTE_COUNT=12,
    GPUPE_IOC_URGENT=13,
    GPUPE_MH_IOC_RTR=14,
    GPUPE_TC0_READ=15,
    GPUPE_MH_TC0_RTR=16,
    GPUPE_TC1_READ=17,
    GPUPE_MH_TC1_RTR=18,
    GPUPE_VC0_READ_MEMORY=19,
    GPUPE_VC0_READ_PGLB=20,
    GPUPE_MH_VC0_RTR=21,
    GPUPE_VC1_READ_MEMORY=22,
    GPUPE_VC1_READ_PGLB=23,
    GPUPE_MH_VC1_RTR=24,
    GPUPE_BC0_CP_WRITE=25,
    GPUPE_BC0_EX_WRITE=26,
    GPUPE_MH_BC0_RTR=27,
    GPUPE_BC1_CP_WRITE=28,
    GPUPE_BC1_EX_WRITE=29,
    GPUPE_MH_BC1_RTR=30,
    GPUPE_DC_GRAPHICS_REQ=31,
    GPUPE_DC_OVERLAY_REQ=32,
    GPUPE_DC_URGENT=33,
    GPUPE_PGLB_BIU_REQ=34,
    GPUPE_BIU_PGL_READ_DATA=35,
    GPUPE_PGL_MHS_READ_DATA=36,
    GPUPE_MH_MC0_READ_REQS=37,
    GPUPE_MH_MC0_WRITE_REQS=38,
    GPUPE_MH_MC1_READ_REQS=39,
    GPUPE_MH_MC1_WRITE_REQS=40,
    GPUPE_MC0_MH_READ_DATA=41,
    GPUPE_MC1_MH_READ_DATA=42,
    GPUPE_MH_CP_SEND=43,
    GPUPE_MH_VGT_SEND=44,
    GPUPE_MH_IOC_SEND=45,
    GPUPE_MH_TC0_SEND=46,
    GPUPE_MH_TC1_SEND=47,
    GPUPE_MH_VC0_SEND=48,
    GPUPE_MH_VC1_SEND=49,
    GPUPE_MH_DC_SEND=50,
    GPUPE_DC0_INFLIGHT_FULL=51,
    GPUPE_DC1_INFLIGHT_FULL=52,
    GPUPE_VC0_INFLIGHT_FULL=53,
    GPUPE_VC1_INFLIGHT_FULL=54,
    GPUPE_TC0_INFLIGHT_FULL=55,
    GPUPE_TC1_INFLIGHT_FULL=56,
    GPUPE_CP0_INFLIGHT_FULL=57,
    GPUPE_CP1_INFLIGHT_FULL=58,
    GPUPE_VGT0_INFLIGHT_FULL=59,
    GPUPE_VGT1_INFLIGHT_FULL=60,
    GPUPE_SB0_INFLIGHT_FULL=61,
    GPUPE_SB1_INFLIGHT_FULL=62,
    GPUPE_VCPGL_INFLIGHT_FULL=63,
    GPUPE_CPPGL_INFLIGHT_FULL=64,
    GPUPE_VGTPGL_INFLIGHT_FULL=65,
    GPUPE_MC0_DC_Q_FULL=66,
    GPUPE_MC0_VC_Q_FULL=67,
    GPUPE_MC0_TC_Q_FULL=68,
    GPUPE_MC0_CP_Q_FULL=69,
    GPUPE_MC0_SB_Q_FULL=70,
    GPUPE_MC0_VGT_Q_FULL=71,
    GPUPE_MC0_BCCP_Q_FULL=72,
    GPUPE_MC0_BCEX_Q_FULL=73,
    GPUPE_MC1_DC_Q_FULL=74,
    GPUPE_MC1_VC_Q_FULL=75,
    GPUPE_MC1_TC_Q_FULL=76,
    GPUPE_MC1_CP_Q_FULL=77,
    GPUPE_MC1_SB_Q_FULL=78,
    GPUPE_MC1_VGT_Q_FULL=79,
    GPUPE_MC1_BCCP_Q_FULL=80,
    GPUPE_MC1_BCEX_Q_FULL=81,
    GPUPE_CP_PGL_CACHE_HIT=82,
    GPUPE_VGT_PGL_CACHE_HIT=83,
    GPUPE_VC_PGL_CACHE_HIT=84,
    GPUPE_CP_PGL_FULL=85,
    GPUPE_VGT_PGL_FULL=86,
    GPUPE_VC0_PGL_FULL=87,
    GPUPE_VC1_PGL_FULL=88,
    GPUPE_MC0_WDB_FULL=89,
    GPUPE_MC0_TAGBUF_FULL=90,
    GPUPE_MC1_WDB_FULL=91,
    GPUPE_MC1_TAGBUF_FULL=92,
    GPUPE_PGL_TAGBUF_FULL=93,
    GPUPE_CP_WRITENOTIFY=94,
    GPUPE_BC_WRITENOTIFY=95,
    GPUPE_IOC_SYNC=96,
    GPUPE_PGL_BIU_LATENCY=97,
    GPUPE_MH_MC0_LATENCY=98,
    GPUPE_MH_MC0_DC_LATENCY=99,
    GPUPE_MH_MC0_VC_LATENCY=100,
    GPUPE_MH_MC0_TC_LATENCY=101,
    GPUPE_MH_MC0_CP_LATENCY=102,
    GPUPE_MH_MC0_SB_LATENCY=103,
    GPUPE_MH_MC0_VGT_LATENCY=104,
    GPUPE_MH_MC1_LATENCY=105,
    GPUPE_MH_MC1_DC_LATENCY=106,
    GPUPE_MH_MC1_VC_LATENCY=107,
    GPUPE_MH_MC1_TC_LATENCY=108,
    GPUPE_MH_MC1_CP_LATENCY=109,
    GPUPE_MH_MC1_SB_LATENCY=110,
    GPUPE_MH_MC1_VGT_LATENCY=111,
    GPUPE_MH_PGL_LATENCY=112,
    GPUPE_MH_PGL_CP_LATENCY=113,
    GPUPE_MH_PGL_VC_LATENCY=114,
    GPUPE_MH_PGL_VGT_LATENCY=115,
    GPUPE_TC0_RDY_AND_NOROOM=116,
    GPUPE_TC1_RDY_AND_NOROOM=117,
    GPUPE_IOC_RDY_AND_NOROOM=118
} GPUPERFEVENT_MH;

typedef union DC_SURFACE_2_INFO DC_SURFACE_2_INFO, *PDC_SURFACE_2_INFO;


/* WARNING! conflicting data type names: /iw5mp_xenon.pdb/DC_SURFACE_2_INFO/<unnamed-type-bitfields> - /iw5mp_xenon.pdb/D1SCL_COEF_RAM_SELECT/<unnamed-type-bitfields> */

union DC_SURFACE_2_INFO {
    struct <unnamed-type-bitfields> bitfields;
    struct <unnamed-type-bitfields> bits;
    uint u32All;
    int i32All;
    float f32All;
};

typedef enum GPUPATHSELECT {
    GPUPATHSELECT_VERTEX_REUSE=0,
    GPUPATHSELECT_TESS_ENABLE=1,
    GPUPATHSELECT_PASSTHRU=2
} GPUPATHSELECT;

typedef enum _D3DTEXTUREADDRESS {
    D3DTADDRESS_WRAP=0,
    D3DTADDRESS_MIRROR=1,
    D3DTADDRESS_CLAMP=2,
    D3DTADDRESS_MIRRORONCE=3,
    D3DTADDRESS_BORDER_HALF=4,
    D3DTADDRESS_MIRRORONCE_BORDER_HALF=5,
    D3DTADDRESS_BORDER=6,
    D3DTADDRESS_MIRRORONCE_BORDER=7,
    D3DTADDRESS_FORCE_DWORD=255
} _D3DTEXTUREADDRESS;

typedef struct D3DPixelShader D3DPixelShader, *PD3DPixelShader;

struct D3DPixelShader {
    undefined field0_0x0;
    undefined field1_0x1;
    undefined field2_0x2;
    undefined field3_0x3;
    undefined field4_0x4;
    undefined field5_0x5;
    undefined field6_0x6;
    undefined field7_0x7;
    undefined field8_0x8;
    undefined field9_0x9;
    undefined field10_0xa;
    undefined field11_0xb;
    undefined field12_0xc;
    undefined field13_0xd;
    undefined field14_0xe;
    undefined field15_0xf;
    undefined field16_0x10;
    undefined field17_0x11;
    undefined field18_0x12;
    undefined field19_0x13;
    undefined field20_0x14;
    undefined field21_0x15;
    undefined field22_0x16;
    undefined field23_0x17;
};

typedef enum GPUPERFEVENT_TCR {
    GPUPE_DGMMPD_IPMUX0_STALL=0,
    GPUPE_DGMMPD_IPMUX1_STALL=1,
    GPUPE_DGMMPD_IPMUX2_STALL=2,
    GPUPE_DGMMPD_IPMUX3_STALL=3,
    GPUPE_DGMMPD_IPMUX_ALL_STALL=4,
    GPUPE_OPMUX0_L2_WRITES=5,
    GPUPE_OPMUX1_L2_WRITES=6,
    GPUPE_OPMUX2_L2_WRITES=7,
    GPUPE_OPMUX3_L2_WRITES=8
} GPUPERFEVENT_TCR;

typedef union MH_PERFMON_PERFCOUNTER0_SELECT MH_PERFMON_PERFCOUNTER0_SELECT, *PMH_PERFMON_PERFCOUNTER0_SELECT;


/* WARNING! conflicting data type names: /iw5mp_xenon.pdb/MH_PERFMON_PERFCOUNTER0_SELECT/<unnamed-type-bitfields> - /iw5mp_xenon.pdb/D1SCL_COEF_RAM_SELECT/<unnamed-type-bitfields> */

union MH_PERFMON_PERFCOUNTER0_SELECT {
    struct <unnamed-type-bitfields> bitfields;
    struct <unnamed-type-bitfields> bits;
    uint u32All;
    int i32All;
    float f32All;
};

typedef enum _MAX_FVF_DECL_SIZE {
    MAX_FVF_DECL_SIZE=65
} _MAX_FVF_DECL_SIZE;

typedef enum _D3DPATCHEDGESTYLE {
    D3DPATCHEDGE_DISCRETE=0,
    D3DPATCHEDGE_CONTINUOUS=1,
    D3DPATCHEDGE_FORCE_DWORD=255
} _D3DPATCHEDGESTYLE;

typedef enum _D3DDECLTYPE {
    D3DDECLTYPE_SHORT2=89,
    D3DDECLTYPE_SHORT2N=89,
    D3DDECLTYPE_USHORT2=89,
    D3DDECLTYPE_USHORT2N=89,
    D3DDECLTYPE_SHORT4=90,
    D3DDECLTYPE_SHORT4N=90,
    D3DDECLTYPE_USHORT4=90,
    D3DDECLTYPE_USHORT4N=90,
    D3DDECLTYPE_FLOAT16_2=95,
    D3DDECLTYPE_FLOAT16_4=96,
    D3DDECLTYPE_BYTE4=134,
    D3DDECLTYPE_BYTE4N=134,
    D3DDECLTYPE_D3DCOLOR=134,
    D3DDECLTYPE_UBYTE4=134,
    D3DDECLTYPE_UBYTE4N=134,
    D3DDECLTYPE_DEC3=135,
    D3DDECLTYPE_DEC3N=135,
    D3DDECLTYPE_DEC4=135,
    D3DDECLTYPE_DEC4N=135,
    D3DDECLTYPE_UDEC3=135,
    D3DDECLTYPE_UDEC3N=135,
    D3DDECLTYPE_UDEC4=135,
    D3DDECLTYPE_UDEC4N=135,
    D3DDECLTYPE_HEND3=144,
    D3DDECLTYPE_HEND3N=144,
    D3DDECLTYPE_UHEND3=144,
    D3DDECLTYPE_UHEND3N=144,
    D3DDECLTYPE_DHEN3=145,
    D3DDECLTYPE_DHEN3N=145,
    D3DDECLTYPE_UDHEN3=145,
    D3DDECLTYPE_UDHEN3N=145,
    D3DDECLTYPE_INT1=161,
    D3DDECLTYPE_INT1N=161,
    D3DDECLTYPE_UINT1=161,
    D3DDECLTYPE_UINT1N=161,
    D3DDECLTYPE_INT2=162,
    D3DDECLTYPE_INT2N=162,
    D3DDECLTYPE_UINT2=162,
    D3DDECLTYPE_UINT2N=162,
    D3DDECLTYPE_INT4=163,
    D3DDECLTYPE_INT4N=163,
    D3DDECLTYPE_UINT4=163,
    D3DDECLTYPE_UINT4N=163,
    D3DDECLTYPE_FLOAT1=164,
    D3DDECLTYPE_FLOAT2=165,
    D3DDECLTYPE_FLOAT4=166,
    D3DDECLTYPE_FLOAT3=185,
    D3DDECLTYPE_UNUSED=255
} _D3DDECLTYPE;

typedef enum IOCPERFREGISTER {
    IOCPERFREG_IOC_PERFMON_CNTL=128,
    IOCPERFREG_IOC_PERFCOUNTER0_SELECT=129,
    IOCPERFREG_IOC_PERFCOUNTER0_HI=130,
    IOCPERFREG_IOC_PERFCOUNTER0_LOW=131,
    IOCPERFREG_IOC_PERFCOUNTER1_SELECT=132,
    IOCPERFREG_IOC_PERFCOUNTER1_HI=133,
    IOCPERFREG_IOC_PERFCOUNTER1_LOW=134,
    IOCPERFREG_IOC_PERFCOUNTER2_SELECT=135,
    IOCPERFREG_IOC_PERFCOUNTER2_HI=136,
    IOCPERFREG_IOC_PERFCOUNTER2_LOW=137,
    IOCPERFREG_IOC_PERFCOUNTER3_SELECT=138,
    IOCPERFREG_IOC_PERFCOUNTER3_HI=139,
    IOCPERFREG_IOC_PERFCOUNTER3_LOW=140
} IOCPERFREGISTER;

typedef union MC0_STATUS_1 MC0_STATUS_1, *PMC0_STATUS_1;


/* WARNING! conflicting data type names: /iw5mp_xenon.pdb/MC0_STATUS_1/<unnamed-type-bitfields> - /iw5mp_xenon.pdb/D1SCL_COEF_RAM_SELECT/<unnamed-type-bitfields> */

union MC0_STATUS_1 {
    struct <unnamed-type-bitfields> bitfields;
    struct <unnamed-type-bitfields> bits;
    uint u32All;
    int i32All;
    float f32All;
};

typedef enum _FSCACHE_PROCESS_TYPE {
    FsCacheProcessTypeThread=0,
    FsCacheProcessTypeTitle=1,
    FsCacheProcessTypeSystem=2
} _FSCACHE_PROCESS_TYPE;

typedef enum <unnamed-tag_00001250> {
    ODD_AUTHTYPE_LEGACY=0,
    ODD_AUTHTYPE_EXTENDED=1,
    ODD_AUTHTYPE_ADVANCED=2
} <unnamed-tag_00001250>;

typedef enum <unnamed-tag_00001141> {
    SPR_LPIDR=63
} <unnamed-tag_00001141>;

typedef enum _D3DCOLORSPACE {
    D3DCOLORSPACE_RGB=0,
    D3DCOLORSPACE_YCbCr601=1,
    D3DCOLORSPACE_YCbCr709=2,
    D3DCOLORSPACE_FORCE_DWORD=255
} _D3DCOLORSPACE;

typedef enum VGT_OUTPATH_SELECT {
    VGT_OUTPATH_VTX_REUSE=0,
    VGT_OUTPATH_TESS_EN=1,
    VGT_OUTPATH_PASSTHRU=2
} VGT_OUTPATH_SELECT;

typedef struct _D3DVIEWPORTF9 _D3DVIEWPORTF9, *P_D3DVIEWPORTF9;

struct _D3DVIEWPORTF9 {
    float X;
    float Y;
    float Width;
    float Height;
    float MinZ;
    float MaxZ;
    ulong Flags;
};

typedef struct _D3DPRESENT_PARAMETERS_ _D3DPRESENT_PARAMETERS_, *P_D3DPRESENT_PARAMETERS_;

typedef enum _D3DMULTISAMPLE_TYPE {
    D3DMULTISAMPLE_NONE=0,
    D3DMULTISAMPLE_2_SAMPLES=1,
    D3DMULTISAMPLE_4_SAMPLES=2,
    D3DMULTISAMPLE_FORCE_DWORD=255
} _D3DMULTISAMPLE_TYPE;

typedef enum _D3DSWAPEFFECT {
    D3DSWAPEFFECT_DISCARD=1,
    D3DSWAPEFFECT_FLIP=2,
    D3DSWAPEFFECT_COPY=3,
    D3DSWAPEFFECT_FORCE_DWORD=255
} _D3DSWAPEFFECT;

typedef struct _D3DVIDEO_SCALER_PARAMETERS _D3DVIDEO_SCALER_PARAMETERS, *P_D3DVIDEO_SCALER_PARAMETERS;

typedef struct _D3DRECT _D3DRECT, *P_D3DRECT;

struct _D3DRECT {
    long x1;
    long y1;
    long x2;
    long y2;
};

struct _D3DVIDEO_SCALER_PARAMETERS {
    struct _D3DRECT ScalerSourceRect;
    ulong ScaledOutputWidth;
    ulong ScaledOutputHeight;
    ulong FilterProfile;
};

struct _D3DPRESENT_PARAMETERS_ {
    uint BackBufferWidth;
    uint BackBufferHeight;
    enum _D3DFORMAT BackBufferFormat;
    uint BackBufferCount;
    enum _D3DMULTISAMPLE_TYPE MultiSampleType;
    ulong MultiSampleQuality;
    enum _D3DSWAPEFFECT SwapEffect;
    struct HWND__ *hDeviceWindow;
    int Windowed;
    int EnableAutoDepthStencil;
    enum _D3DFORMAT AutoDepthStencilFormat;
    ulong Flags;
    uint FullScreen_RefreshRateInHz;
    uint PresentationInterval;
    int DisableAutoBackBuffer;
    int DisableAutoFrontBuffer;
    enum _D3DFORMAT FrontBufferFormat;
    short _padding_;
    enum _D3DCOLORSPACE FrontBufferColorSpace;
    struct _D3DRING_BUFFER_PARAMETERS RingBufferParameters;
    struct _D3DVIDEO_SCALER_PARAMETERS VideoScalerParameters;
};

typedef struct GPU_TESSELLATORPACKET GPU_TESSELLATORPACKET, *PGPU_TESSELLATORPACKET;

typedef struct GPU_TESSELLATORPACKET_s_0 GPU_TESSELLATORPACKET_s_0, *PGPU_TESSELLATORPACKET_s_0;

typedef union GPU_POINTMINMAX GPU_POINTMINMAX, *PGPU_POINTMINMAX;

typedef union GPU_LINECONTROL GPU_LINECONTROL, *PGPU_LINECONTROL;

typedef union GPU_OUTPUTPATHCONTROL GPU_OUTPUTPATHCONTROL, *PGPU_OUTPUTPATHCONTROL;

typedef union GPU_HOSCONTROL GPU_HOSCONTROL, *PGPU_HOSCONTROL;

typedef union GPU_GROUPPRIMTYPE GPU_GROUPPRIMTYPE, *PGPU_GROUPPRIMTYPE;

typedef struct GPU_TESSELLATORPACKET_s_0_s_0 GPU_TESSELLATORPACKET_s_0_s_0, *PGPU_TESSELLATORPACKET_s_0_s_0;

typedef union GPU_GROUPVECTCONTROL GPU_GROUPVECTCONTROL, *PGPU_GROUPVECTCONTROL;

typedef union GPU_MPASSPSCONTROL GPU_MPASSPSCONTROL, *PGPU_MPASSPSCONTROL;

typedef struct GPU_POINTMINMAX_s_0 GPU_POINTMINMAX_s_0, *PGPU_POINTMINMAX_s_0;

typedef struct GPU_GROUPPRIMTYPE_s_0 GPU_GROUPPRIMTYPE_s_0, *PGPU_GROUPPRIMTYPE_s_0;

typedef struct GPU_TESSELLATORPACKET_s_0_s_0_s_0 GPU_TESSELLATORPACKET_s_0_s_0_s_0, *PGPU_TESSELLATORPACKET_s_0_s_0_s_0;

typedef union GPU_VIZQUERY GPU_VIZQUERY, *PGPU_VIZQUERY;

typedef struct GPU_GROUPVECTCONTROL_s_0 GPU_GROUPVECTCONTROL_s_0, *PGPU_GROUPVECTCONTROL_s_0;

typedef struct GPU_MPASSPSCONTROL_s_0 GPU_MPASSPSCONTROL_s_0, *PGPU_MPASSPSCONTROL_s_0;

typedef union GPU_ENHANCE GPU_ENHANCE, *PGPU_ENHANCE;

typedef struct GPU_VIZQUERY_s_0 GPU_VIZQUERY_s_0, *PGPU_VIZQUERY_s_0;

struct GPU_VIZQUERY_s_0 {
    ulong KillPixPostHiZ:1;
    ulong VizQueryId:6;
    ulong VizQueryEnable:1;
    ulong KillPixPostDetailMask:1;
};

struct GPU_GROUPVECTCONTROL_s_0 {
    ulong _padding_:4;
    ulong CompWEnable:1;
    ulong CompZEnable:1;
    ulong CompYEnable:1;
    ulong CompXEnable:1;
    ulong Stride:8;
    ulong Shift:8;
};

union GPU_GROUPVECTCONTROL {
    struct GPU_GROUPVECTCONTROL_s_0 _s_0;
    ulong dword;
};

struct GPU_MPASSPSCONTROL_s_0 {
    undefined field0_0x0;
    undefined field1_0x1;
    undefined field2_0x2;
    ulong MPassPsEnable:1;
    undefined field4_0x4;
    undefined field5_0x5;
    ulong MPassPixVecPerPass:20;
};

union GPU_MPASSPSCONTROL {
    struct GPU_MPASSPSCONTROL_s_0 _s_0;
    ulong dword;
};

struct GPU_GROUPPRIMTYPE_s_0 {
    undefined field0_0x0;
    ulong GroupRetainQuads:1;
    ulong GroupRetainOrder:1;
    ulong GroupPrimOrder:3;
    undefined field4_0x3;
    undefined field5_0x4;
    ulong GroupPrimType:4;
};

union GPU_GROUPPRIMTYPE {
    struct GPU_GROUPPRIMTYPE_s_0 _s_0;
    ulong dword;
};

union GPU_VIZQUERY {
    struct GPU_VIZQUERY_s_0 _s_0;
    ulong dword;
};

union GPU_ENHANCE {
    ulong Misc:16;
    ulong dword;
};

struct GPU_TESSELLATORPACKET_s_0_s_0_s_0 {
    int _padding_;
    int _padding_;
    union GPU_ENHANCE Enhance;
    int _padding_;
    int _padding_;
    int _padding_;
    int _padding_;
    int _padding_;
    int _padding_;
    int _padding_;
    int _padding_;
    int _padding_;
    int _padding_;
    int _padding_;
    int _padding_;
    int _padding_;
    int _padding_;
    int _padding_;
    int _padding_;
    union GPU_MPASSPSCONTROL MPassPsControl;
};

struct GPU_TESSELLATORPACKET_s_0_s_0 {
    struct GPU_TESSELLATORPACKET_s_0_s_0_s_0 field0_0x0;
    union GPU_VIZQUERY VizQuery;
    undefined field2_0x5;
    undefined field3_0x6;
    undefined field4_0x7;
    undefined field5_0x8;
    undefined field6_0x9;
    undefined field7_0xa;
    undefined field8_0xb;
    undefined field9_0xc;
    undefined field10_0xd;
    undefined field11_0xe;
    undefined field12_0xf;
    undefined field13_0x10;
    undefined field14_0x11;
    undefined field15_0x12;
    undefined field16_0x13;
    undefined field17_0x14;
    undefined field18_0x15;
    undefined field19_0x16;
    undefined field20_0x17;
    undefined field21_0x18;
    undefined field22_0x19;
    undefined field23_0x1a;
    undefined field24_0x1b;
    undefined field25_0x1c;
    undefined field26_0x1d;
    undefined field27_0x1e;
    undefined field28_0x1f;
    undefined field29_0x20;
    undefined field30_0x21;
    undefined field31_0x22;
    undefined field32_0x23;
    undefined field33_0x24;
    undefined field34_0x25;
    undefined field35_0x26;
    undefined field36_0x27;
    undefined field37_0x28;
    undefined field38_0x29;
    undefined field39_0x2a;
    undefined field40_0x2b;
    undefined field41_0x2c;
    undefined field42_0x2d;
    undefined field43_0x2e;
    undefined field44_0x2f;
    undefined field45_0x30;
    undefined field46_0x31;
    undefined field47_0x32;
    undefined field48_0x33;
    undefined field49_0x34;
    undefined field50_0x35;
    undefined field51_0x36;
    undefined field52_0x37;
    undefined field53_0x38;
    undefined field54_0x39;
    undefined field55_0x3a;
    undefined field56_0x3b;
    undefined field57_0x3c;
    undefined field58_0x3d;
    undefined field59_0x3e;
    undefined field60_0x3f;
    undefined field61_0x40;
    undefined field62_0x41;
    undefined field63_0x42;
    undefined field64_0x43;
    undefined field65_0x44;
    undefined field66_0x45;
    undefined field67_0x46;
    undefined field68_0x47;
    undefined field69_0x48;
    union GPU_MPASSPSCONTROL MPassPsControl;
};

struct GPU_TESSELLATORPACKET_s_0 {
    struct GPU_TESSELLATORPACKET_s_0_s_0 field0_0x0;
    ulong GroupFirstDecr;
    undefined field2_0x5;
    undefined field3_0x6;
    undefined field4_0x7;
    undefined field5_0x8;
    undefined field6_0x9;
    undefined field7_0xa;
    undefined field8_0xb;
    undefined field9_0xc;
    undefined field10_0xd;
    undefined field11_0xe;
    undefined field12_0xf;
    undefined field13_0x10;
    undefined field14_0x11;
    undefined field15_0x12;
    undefined field16_0x13;
    undefined field17_0x14;
    undefined field18_0x15;
    undefined field19_0x16;
    undefined field20_0x17;
    undefined field21_0x18;
    undefined field22_0x19;
    undefined field23_0x1a;
    undefined field24_0x1b;
    undefined field25_0x1c;
    undefined field26_0x1d;
    undefined field27_0x1e;
    undefined field28_0x1f;
    undefined field29_0x20;
    undefined field30_0x21;
    undefined field31_0x22;
    undefined field32_0x23;
    undefined field33_0x24;
    undefined field34_0x25;
    undefined field35_0x26;
    undefined field36_0x27;
    undefined field37_0x28;
    ulong GroupDecr;
    union GPU_GROUPVECTCONTROL GroupVect0Control;
    union GPU_GROUPVECTCONTROL GroupVect1Control;
    union GPU_GROUPVECTFMTCONTROL GroupVect0FmtControl;
    union GPU_GROUPVECTFMTCONTROL GroupVect1FmtControl;
    ulong Unused2[2];
    union GPU_MPASSPSCONTROL MPassPsControl;
    union GPU_GROUPPRIMTYPE GroupPrimType;
};

struct GPU_POINTMINMAX_s_0 {
    ulong MinSize:16;
    ulong MaxSize:16;
};

union GPU_LINECONTROL {
    ulong Width:16;
    ulong dword;
};

union GPU_POINTMINMAX {
    struct GPU_POINTMINMAX_s_0 _s_0;
    ulong dword;
};

union GPU_OUTPUTPATHCONTROL {
    ulong PathSelect:2;
    ulong dword;
};

union GPU_HOSCONTROL {
    ulong TessMode:2;
    ulong dword;
};

struct GPU_TESSELLATORPACKET {
    struct GPU_TESSELLATORPACKET_s_0 field0_0x0;
    union GPU_POINTMINMAX PointMinMax;
    union GPU_LINECONTROL LineControl;
    ulong Unused1;
    union GPU_OUTPUTPATHCONTROL OutputPathControl;
    union GPU_HOSCONTROL HosControl;
    float HosMaxTessLevel;
    float HosMinTessLevel;
    ulong HosReuseDepth;
    union GPU_GROUPPRIMTYPE GroupPrimType;
    undefined field10_0x27;
    undefined field11_0x28;
    undefined field12_0x29;
    undefined field13_0x2a;
    undefined field14_0x2b;
    undefined field15_0x2c;
    undefined field16_0x2d;
    undefined field17_0x2e;
    undefined field18_0x2f;
    undefined field19_0x30;
    undefined field20_0x31;
    undefined field21_0x32;
    undefined field22_0x33;
    undefined field23_0x34;
    undefined field24_0x35;
    undefined field25_0x36;
    undefined field26_0x37;
    undefined field27_0x38;
    undefined field28_0x39;
    undefined field29_0x3a;
    undefined field30_0x3b;
    undefined field31_0x3c;
    undefined field32_0x3d;
    undefined field33_0x3e;
    undefined field34_0x3f;
    undefined field35_0x40;
    undefined field36_0x41;
    undefined field37_0x42;
    undefined field38_0x43;
    undefined field39_0x44;
    undefined field40_0x45;
    undefined field41_0x46;
    undefined field42_0x47;
    undefined field43_0x48;
    undefined field44_0x49;
    undefined field45_0x4a;
    undefined field46_0x4b;
    undefined field47_0x4c;
    undefined field48_0x4d;
    undefined field49_0x4e;
    undefined field50_0x4f;
    undefined field51_0x50;
    undefined field52_0x51;
    undefined field53_0x52;
    undefined field54_0x53;
};

typedef enum <unnamed-tag_000010D3> {
    SPR_XER=1
} <unnamed-tag_000010D3>;

typedef enum <unnamed-tag_000010F7> {
    SPR_VRSAVE=0
} <unnamed-tag_000010F7>;

typedef enum <unnamed-tag_00001189> {
    SPR_HID1=241
} <unnamed-tag_00001189>;

typedef enum _D3DZBUFFERTYPE {
    D3DZB_FALSE=0,
    D3DZB_TRUE=1,
    D3DZB_FORCE_DWORD=255
} _D3DZBUFFERTYPE;

typedef enum <unnamed-tag_00001051> {
    FLAG_BIT_SV_PROTECTED_NO_EJECT_REBOOT=0,
    FLAG_SV_PROTECTED_NONE=0,
    FLAG_BIT_SV_PROTECTED_DISC_AUTHENTICATED=1,
    FLAG_SV_PROTECTED_NO_EJECT_REBOOT=1,
    FLAG_BIT_SV_PROTECTED_AUTH_EX_CAPABLE=2,
    FLAG_SV_PROTECTED_DISC_AUTHENTICATED=2,
    FLAG_BIT_SV_PROTECTED_MAXIMUM=3,
    FLAG_SV_PROTECTED_AUTH_EX_CAPABLE=4
} <unnamed-tag_00001051>;

typedef enum _XPROFILE_CONTROL_SENSITIVITY_OPTIONS {
    XPROFILE_CONTROL_SENSITIVITY_MEDIUM=0,
    XPROFILE_CONTROL_SENSITIVITY_LOW=1,
    XPROFILE_CONTROL_SENSITIVITY_HIGH=2
} _XPROFILE_CONTROL_SENSITIVITY_OPTIONS;

typedef enum _XPROFILE_ACTION_MOVEMENT_CONTROL_OPTIONS {
    XPROFILE_ACTION_MOVEMENT_CONTROL_L_THUMBSTICK=0,
    XPROFILE_ACTION_MOVEMENT_CONTROL_R_THUMBSTICK=1
} _XPROFILE_ACTION_MOVEMENT_CONTROL_OPTIONS;

typedef union GPU_VTECONTROL GPU_VTECONTROL, *PGPU_VTECONTROL;

typedef struct GPU_VTECONTROL_s_0 GPU_VTECONTROL_s_0, *PGPU_VTECONTROL_s_0;

struct GPU_VTECONTROL_s_0 {
    ulong VportZOffsetEnable:1;
    ulong VportZScaleEnable:1;
    ulong VportYOffsetEnable:1;
    ulong VportYScaleEnable:1;
    ulong VportXOffsetEnable:1;
    ulong VportXScaleEnable:1;
    ulong PerfCounterRef:1;
    ulong VtxW0Fmt:1;
    ulong VtxZFmt:1;
    ulong VtxXyFmt:1;
};

union GPU_VTECONTROL {
    struct GPU_VTECONTROL_s_0 _s_0;
    ulong dword;
};

typedef union TP1_PERFCOUNTER1_LOW TP1_PERFCOUNTER1_LOW, *PTP1_PERFCOUNTER1_LOW;


/* WARNING! conflicting data type names: /iw5mp_xenon.pdb/TP1_PERFCOUNTER1_LOW/<unnamed-type-bitfields> - /iw5mp_xenon.pdb/D1SCL_COEF_RAM_SELECT/<unnamed-type-bitfields> */

union TP1_PERFCOUNTER1_LOW {
    struct <unnamed-type-bitfields> bitfields;
    struct <unnamed-type-bitfields> bits;
    uint u32All;
    int i32All;
    float f32All;
};

typedef enum UCODESHADERTYPE {
    UCODESHADERTYPE_PIXEL=0,
    UCODESHADERTYPE_VERTEX=1
} UCODESHADERTYPE;

typedef struct _D3DPERFCOUNTER_VALUES _D3DPERFCOUNTER_VALUES, *P_D3DPERFCOUNTER_VALUES;

typedef union _ULARGE_INTEGER _ULARGE_INTEGER, *P_ULARGE_INTEGER;

typedef struct _ULARGE_INTEGER_s_0 _ULARGE_INTEGER_s_0, *P_ULARGE_INTEGER_s_0;

typedef struct <unnamed-type-u> <unnamed-type-u>, *P<unnamed-type-u>;

struct <unnamed-type-u> {
    ulong HighPart;
    ulong LowPart;
};

struct _ULARGE_INTEGER_s_0 {
    ulong HighPart;
    ulong LowPart;
};

union _ULARGE_INTEGER {
    struct _ULARGE_INTEGER_s_0 _s_0;
    struct <unnamed-type-u> u;
    ulong64 QuadPart;
};

struct _D3DPERFCOUNTER_VALUES {
    union _ULARGE_INTEGER CP[1];
    union _ULARGE_INTEGER RBBM[2];
    union _ULARGE_INTEGER SQ[4];
    union _ULARGE_INTEGER VGT[4];
    union _ULARGE_INTEGER VC[4];
    union _ULARGE_INTEGER PA_SU[4];
    union _ULARGE_INTEGER PA_SC[4];
    union _ULARGE_INTEGER HZ[2];
    union _ULARGE_INTEGER TCR[2];
    union _ULARGE_INTEGER TCM[2];
    union _ULARGE_INTEGER TCF[12];
    union _ULARGE_INTEGER TP0[2];
    union _ULARGE_INTEGER TP1[2];
    union _ULARGE_INTEGER TP2[2];
    union _ULARGE_INTEGER TP3[2];
    union _ULARGE_INTEGER SX[1];
    union _ULARGE_INTEGER BC[4];
    union _ULARGE_INTEGER MC0[1];
    union _ULARGE_INTEGER MC1[1];
    union _ULARGE_INTEGER MH[3];
    union _ULARGE_INTEGER BIF[1];
};

typedef enum _XE_RSA_KEY_TYPE {
    XE_PIRS_RSA_KEY=0,
    XE_LIVEDRM_RSA_KEY=1,
    XE_DEVICE_RSA_KEY=2,
    XE_XSIGNER2_RSA_KEY=3,
    XE_MAX_RSA_KEY=4
} _XE_RSA_KEY_TYPE;

typedef union D1CRTC_OVERSCAN_LEFT D1CRTC_OVERSCAN_LEFT, *PD1CRTC_OVERSCAN_LEFT;


/* WARNING! conflicting data type names: /iw5mp_xenon.pdb/D1CRTC_OVERSCAN_LEFT/<unnamed-type-bitfields> - /iw5mp_xenon.pdb/D1SCL_COEF_RAM_SELECT/<unnamed-type-bitfields> */

union D1CRTC_OVERSCAN_LEFT {
    struct <unnamed-type-bitfields> bitfields;
    struct <unnamed-type-bitfields> bits;
    uint u32All;
    int i32All;
    float f32All;
};

typedef enum _D3DLIGHTTYPE {
    D3DLIGHT_POINT=1,
    D3DLIGHT_SPOT=2,
    D3DLIGHT_DIRECTIONAL=3,
    D3DLIGHT_FORCE_DWORD=255
} _D3DLIGHTTYPE;

typedef enum GPUTESSMODE {
    GPUTESSMODE_DISCRETE=0,
    GPUTESSMODE_CONTINUOUS=1,
    GPUTESSMODE_ADAPTIVE=2
} GPUTESSMODE;

typedef enum CP_OP {
    CP_OP_NOP=16,
    CP_OP_IB_PREFETCH_START=22,
    CP_OP_IB_PREFETCH_END=23,
    CP_OP_SUBLK_PREFETCH=31,
    CP_OP_INSTR_PREFETCH=32,
    CP_OP_REG_RMW=33,
    CP_OP_DRAW_INDX=34,
    CP_OP_VIZ_QUERY=35,
    CP_OP_SET_STATE=37,
    CP_OP_WAIT_FOR_IDLE=38,
    CP_OP_IM_LOAD=39,
    CP_OP_IM_LOAD_IMMEDIATE=43,
    CP_OP_SET_CONSTANT=45,
    CP_OP_LOAD_ALU_CONSTANT=47,
    CP_OP_3D_DRAW_INDX_2=54,
    CP_OP_INDIRECT_BUFFER_PFD=55,
    CP_OP_MPEG_INDEX=58,
    CP_OP_INVALIDATE_STATE=59,
    CP_OP_WAIT_REG_MEM=60,
    CP_OP_MEM_WRITE=61,
    CP_OP_REG_TO_MEM=62,
    CP_OP_INDIRECT_BUFFER=63,
    CP_OP_CP_INTERRUPT=64,
    CP_OP_COND_WRITE=69,
    CP_OP_EVENT_WRITE=70,
    CP_OP_INSTR_MATCH=71,
    CP_OP_ME_INIT=72,
    CP_OP_CONST_PREFETCH=73,
    CP_OP_MEM_WRITE_CNTR=79,
    CP_OP_OLD_SET_BIN_MASK=80,
    CP_OP_OLD_SET_BIN_SELECT=81,
    CP_OP_WAIT_REG_EQ=82,
    CP_OP_WAIT_REG_GTE=83,
    CP_OP_CPU_INTERRUPT=84,
    CP_OP_INCR_UPDATE_STATE=85,
    CP_OP_INCR_UPDATE_CONST=86,
    CP_OP_INCR_UPDATE_INSTR=87,
    CP_OP_EVENT_WRITE_SHD=88,
    CP_OP_EVENT_WRITE_CFL=89,
    CP_OP_EVENT_WRITE_SER=90,
    CP_OP_EVENT_WRITE_ZPD=91,
    CP_OP_WAIT_IB_PFD_COMPLETE=93,
    CP_OP_CONTEXT_UPDATE=94,
    CP_OP_SET_BIN_MASK_LO=96,
    CP_OP_SET_BIN_MASK_HI=97,
    CP_OP_SET_BIN_SELECT_LO=98,
    CP_OP_SET_BIN_SELECT_HI=99,
    CP_OP_SET_BRACKET_OFFSET=100,
    CP_OP_EVENT_WRITE_ZPD_EX=101,
    CP_OP_REG_TO_MEM_EX=102,
    CP_OP_EVENT_WRITE_SHD_PS_EX=103,
    CP_OP_QUEUE_JUMP=104,
    CP_OP_COND_EXEC=105,
    CP_OP_TYPE0=125,
    CP_OP_TYPE2=126
} CP_OP;

typedef struct _D3DCOLORVALUE _D3DCOLORVALUE, *P_D3DCOLORVALUE;

struct _D3DCOLORVALUE {
    float r;
    float g;
    float b;
    float a;
};

typedef enum <unnamed-tag_0000111F> {
    SPR_HSPRG0=48
} <unnamed-tag_0000111F>;

typedef enum <unnamed-tag_000010F1> {
    SPR_ACCR=29
} <unnamed-tag_000010F1>;

typedef enum GPUEXPORTREGISTER_PS {
    GPUEXPORTREGISTER_PS_COLOR_0=0,
    GPUEXPORTREGISTER_PS_COLOR_1=1,
    GPUEXPORTREGISTER_PS_COLOR_2=2,
    GPUEXPORTREGISTER_PS_COLOR_3=3,
    GPUEXPORTREGISTER_PS_EXPORT_ADDRESS=32,
    GPUEXPORTREGISTER_PS_EXPORT_DATA_0=33,
    GPUEXPORTREGISTER_PS_EXPORT_DATA_1=34,
    GPUEXPORTREGISTER_PS_EXPORT_DATA_2=35,
    GPUEXPORTREGISTER_PS_EXPORT_DATA_3=36,
    GPUEXPORTREGISTER_PS_EXPORT_DATA_4=37,
    GPUEXPORTREGISTER_PS_DEPTH=61
} GPUEXPORTREGISTER_PS;

typedef struct __vector4 __vector4, *P__vector4;

typedef union __vector4_u_0 __vector4_u_0, *P__vector4_u_0;

typedef struct __vector4_u_0_s_2 __vector4_u_0_s_2, *P__vector4_u_0_s_2;

struct __vector4_u_0_s_2 {
    float x;
    float y;
    float z;
    float w;
};

union __vector4_u_0 {
    float vector4_f32[4];
    uint vector4_u32[4];
    struct __vector4_u_0_s_2 _s_2;
    float v[4];
    uint u[4];
};

struct __vector4 {
    union __vector4_u_0 field0_0x0;
};

typedef enum GPUALUSRCSELECT {
    GPUALUSRCSELECT_C=0,
    GPUALUSRCSELECT_R=1
} GPUALUSRCSELECT;

typedef enum _D3DCMPFUNC {
    D3DCMP_NEVER=0,
    D3DCMP_LESS=1,
    D3DCMP_EQUAL=2,
    D3DCMP_LESSEQUAL=3,
    D3DCMP_GREATER=4,
    D3DCMP_NOTEQUAL=5,
    D3DCMP_GREATEREQUAL=6,
    D3DCMP_ALWAYS=7,
    D3DCMP_FORCE_DWORD=255
} _D3DCMPFUNC;

typedef enum GPUALUVECTOROP {
    GPUALUVECTOROP_ADD=0,
    GPUALUVECTOROP_MUL=1,
    GPUALUVECTOROP_MAX=2,
    GPUALUVECTOROP_MIN=3,
    GPUALUVECTOROP_SEQ=4,
    GPUALUVECTOROP_SGT=5,
    GPUALUVECTOROP_SGE=6,
    GPUALUVECTOROP_SNE=7,
    GPUALUVECTOROP_FRC=8,
    GPUALUVECTOROP_TRUNC=9,
    GPUALUVECTOROP_FLOOR=10,
    GPUALUVECTOROP_MAD=11,
    GPUALUVECTOROP_CNDEQ=12,
    GPUALUVECTOROP_CNDGE=13,
    GPUALUVECTOROP_CNDGT=14,
    GPUALUVECTOROP_DP4=15,
    GPUALUVECTOROP_DP3=16,
    GPUALUVECTOROP_DP2ADD=17,
    GPUALUVECTOROP_CUBE=18,
    GPUALUVECTOROP_MAX4=19,
    GPUALUVECTOROP_SETPEQP=20,
    GPUALUVECTOROP_SETPNEP=21,
    GPUALUVECTOROP_SETPGTP=22,
    GPUALUVECTOROP_SETPGEP=23,
    GPUALUVECTOROP_KILLEQ=24,
    GPUALUVECTOROP_KILLGT=25,
    GPUALUVECTOROP_KILLGE=26,
    GPUALUVECTOROP_KILLNE=27,
    GPUALUVECTOROP_DST=28,
    GPUALUVECTOROP_MAXA=29
} GPUALUVECTOROP;

typedef enum GPUPERFEVENT_VC {
    GPUPE_RG_VERTICES=0,
    GPUPE_RG_CLAMPED=1,
    GPUPE_RG_L2_REQUEST=2,
    GPUPE_RG_L1_REQUEST=3,
    GPUPE_RG_MEGAFETCH=4,
    GPUPE_RG_END_OF_GROUP=5,
    GPUPE_RG_CONFLICT=6,
    GPUPE_RG_DWORDS_REQUESTED=7,
    GPUPE_RG_SPARE0=8,
    GPUPE_RG_SPARE1=9,
    GPUPE_CC_STALLS=10,
    GPUPE_CC_HITS=11,
    GPUPE_CC_MISSES=12,
    GPUPE_CC_SECTOR_MISSES=13,
    GPUPE_CC_L2B_STALLS=14,
    GPUPE_CC_MI_STALLS=15,
    GPUPE_CC_MULTICYCLE_STALLS=16,
    GPUPE_CC_EVEN_ALLOC_STALLS=17,
    GPUPE_CC_ODD_ALLOC_STALLS=18,
    GPUPE_CC_EVEN_BUSY_STALLS=19,
    GPUPE_CC_ODD_BUSY_STALLS=20,
    GPUPE_CC_IN_FIFO_EMPTY=21,
    GPUPE_CC_IN_FIFO_FULL=22,
    GPUPE_CC_FREEZE=23,
    GPUPE_CC_SPARE0=24,
    GPUPE_CC_SPARE1=25,
    GPUPE_CC_SPARE2=26,
    GPUPE_CC_SPARE3=27,
    GPUPE_CC_SPARE4=28,
    GPUPE_CC_SPARE5=29,
    GPUPE_MI_REQUESTS_TO_MH=30,
    GPUPE_MI_AGP_REQUESTS=31,
    GPUPE_MI_LATENCY_BITS_4_0=32,
    GPUPE_MI_LATENCY_BITS_9_5=33,
    GPUPE_MI_LATENCY_BITS_14_10=34,
    GPUPE_MI_LATENCY_BITS_17_15=35,
    GPUPE_MI_INPUT_FIFOS_FULL=36,
    GPUPE_MI_INPUT_FIFOS_0_FULL=37,
    GPUPE_MI_INPUT_FIFOS_1_FULL=38,
    GPUPE_MI_SPARE1=39,
    GPUPE_RP_SP_DATA_VALID=40,
    GPUPE_RP_STALLED=41,
    GPUPE_RP_SPARE0=42,
    GPUPE_RP_SPARE1=43,
    GPUPE_RP_SPARE2=44,
    GPUPE_RP_SPARE3=45,
    GPUPE_RP_SPARE4=46,
    GPUPE_RP_SPARE5=47,
    GPUPE_RP_SPARE6=48,
    GPUPE_RP_SPARE7=49,
    GPUPE_DC_NUM_VALIDS=50,
    GPUPE_DC_SPARE0=51,
    GPUPE_DC_SPARE1=52,
    GPUPE_DC_SPARE2=53,
    GPUPE_DC_SPARE3=54,
    GPUPE_DC_SPARE4=55,
    GPUPE_DC_SPARE5=56,
    GPUPE_DC_SPARE6=57,
    GPUPE_DC_SPARE7=58,
    GPUPE_DC_SPARE8=59,
    GPUPE_SQ_VC_SEND=60,
    GPUPE_VC_STARVED_IDLE=61,
    GPUPE_VC_BUSY=62,
    GPUPE_VC_IDLE=63,
    GPUPE_VC_SPARE0=64,
    GPUPE_VC_SPARE1=65,
    GPUPE_VC_SPARE2=66,
    GPUPE_VC_SPARE3=67,
    GPUPE_VC_SPARE4=68,
    GPUPE_VC_SPARE5=69
} GPUPERFEVENT_VC;

typedef struct D3DPerfCounters D3DPerfCounters, *PD3DPerfCounters;

struct D3DPerfCounters {
    undefined field0_0x0;
};

typedef enum RB_PERFCNT_SELECT {
    BC_CNTX0_BUSY=0,
    BC_CNTX17_BUSY=1,
    BC_RQ_STALLED=2,
    BC_AZ_STALLED=3,
    BC_MH_CPY_STALLED=4,
    BC_MH_EXP_STALLED=5,
    BC_SC_STARVED=6,
    BC_SX_STARVED=7,
    BC_ACC_COUNT=8,
    BC_DRAW_COUNT=9,
    BC_ACC2_COUNT=10,
    BC_DRAW2_COUNT=11,
    BC_SETZ_COUNT=12,
    BC_READ_COUNT=13,
    BC_READ_ACC_COUNT=14,
    BC_STATE_COUNT=15,
    BC_STATE2_COUNT=16,
    BC_COPY_WRITE_COUNT=17,
    BC_EXPORT_COUNT=18
} RB_PERFCNT_SELECT;

typedef enum GPUPRIMTYPE {
    GPUPRIMTYPE_NONE=0,
    GPUPRIMTYPE_POINTLIST=1,
    GPUPRIMTYPE_LINELIST=2,
    GPUPRIMTYPE_LINESTRIP=3,
    GPUPRIMTYPE_TRILIST=4,
    GPUPRIMTYPE_TRIFAN=5,
    GPUPRIMTYPE_TRISTRIP=6,
    GPUPRIMTYPE_TRI_WITH_WFLAGS=7,
    GPUPRIMTYPE_RECTLIST=8,
    GPUPRIMTYPE_LINELOOP=12,
    GPUPRIMTYPE_QUADLIST=13,
    GPUPRIMTYPE_QUADSTRIP=14,
    GPUPRIMTYPE_POLYGON=15,
    GPUPRIMTYPE_2D_COPY_RECT_LIST_V0=16,
    GPUPRIMTYPE_2D_COPY_RECT_LIST_V1=17,
    GPUPRIMTYPE_2D_COPY_RECT_LIST_V2=18,
    GPUPRIMTYPE_2D_COPY_RECT_LIST_V3=19,
    GPUPRIMTYPE_2D_FILL_RECT_LIST=20,
    GPUPRIMTYPE_2D_LINE_STRIP=21,
    GPUPRIMTYPE_2D_TRI_STRIP=22
} GPUPRIMTYPE;

typedef enum GPUPERFEVENT_TCF {
    GPUPE_VALID_CYCLES=0,
    GPUPE_SINGLE_PHASES=1,
    GPUPE_ANISO_PHASES=2,
    GPUPE_MIP_PHASES=3,
    GPUPE_VOL_PHASES=4,
    GPUPE_MIP_VOL_PHASES=5,
    GPUPE_MIP_ANISO_PHASES=6,
    GPUPE_VOL_ANISO_PHASES=7,
    GPUPE_ANISO_2_1_PHASES=8,
    GPUPE_ANISO_4_1_PHASES=9,
    GPUPE_ANISO_6_1_PHASES=10,
    GPUPE_ANISO_8_1_PHASES=11,
    GPUPE_ANISO_10_1_PHASES=12,
    GPUPE_ANISO_12_1_PHASES=13,
    GPUPE_ANISO_14_1_PHASES=14,
    GPUPE_ANISO_16_1_PHASES=15,
    GPUPE_MIP_VOL_ANISO_PHASES=16,
    GPUPE_ALIGN_2_PHASES=17,
    GPUPE_ALIGN_4_PHASES=18,
    GPUPE_TPC_BUSY=19,
    GPUPE_TPC_STALLED=20,
    GPUPE_TPC_STARVED=21,
    GPUPE_TPC_WORKING=22,
    GPUPE_TPC_WALKER_BUSY=23,
    GPUPE_TPC_WALKER_STALLED=24,
    GPUPE_TPC_WALKER_WORKING=25,
    GPUPE_TPC_ALIGNER_BUSY=26,
    GPUPE_TPC_ALIGNER_STALLED=27,
    GPUPE_TPC_ALIGNER_STALLED_BY_BLEND=28,
    GPUPE_TPC_ALIGNER_STALLED_BY_CACHE=29,
    GPUPE_TPC_ALIGNER_WORKING=30,
    GPUPE_TPC_BLEND_BUSY=31,
    GPUPE_TPC_BLEND_SYNC=32,
    GPUPE_TPC_BLEND_STARVED=33,
    GPUPE_TPC_BLEND_WORKING=34,
    GPUPE_OPCODE_0X00=35,
    GPUPE_OPCODE_0X01=36,
    GPUPE_OPCODE_0X04=37,
    GPUPE_OPCODE_0X10=38,
    GPUPE_OPCODE_0X11=39,
    GPUPE_OPCODE_0X12=40,
    GPUPE_OPCODE_0X13=41,
    GPUPE_OPCODE_0X18=42,
    GPUPE_OPCODE_0X19=43,
    GPUPE_OPCODE_0X1A=44,
    GPUPE_OPCODE_OTHER=45,
    GPUPE_RESERVED_46=46,
    GPUPE_RESERVED_47=47,
    GPUPE_RESERVED_48=48,
    GPUPE_RESERVED_49=49,
    GPUPE_RESERVED_50=50,
    GPUPE_RESERVED_51=51,
    GPUPE_RESERVED_52=52,
    GPUPE_RESERVED_53=53,
    GPUPE_RESERVED_54=54,
    GPUPE_RESERVED_55=55,
    GPUPE_IN_FIFO_0_EMPTY=56,
    GPUPE_IN_FIFO_0_LT_HALF_FULL=57,
    GPUPE_IN_FIFO_0_HALF_FULL=58,
    GPUPE_IN_FIFO_0_FULL=59,
    GPUPE_IN_FIFO_1_EMPTY=60,
    GPUPE_IN_FIFO_1_LT_HALF_FULL=61,
    GPUPE_IN_FIFO_1_HALF_FULL=62,
    GPUPE_IN_FIFO_1_FULL=63,
    GPUPE_IN_FIFO_2_EMPTY=64,
    GPUPE_IN_FIFO_2_LT_HALF_FULL=65,
    GPUPE_IN_FIFO_2_HALF_FULL=66,
    GPUPE_IN_FIFO_2_FULL=67,
    GPUPE_IN_FIFO_3_EMPTY=68,
    GPUPE_IN_FIFO_3_LT_HALF_FULL=69,
    GPUPE_IN_FIFO_3_HALF_FULL=70,
    GPUPE_IN_FIFO_3_FULL=71,
    GPUPE_IN_FIFO_TPC_EMPTY=72,
    GPUPE_IN_FIFO_TPC_LT_HALF_FULL=73,
    GPUPE_IN_FIFO_TPC_HALF_FULL=74,
    GPUPE_IN_FIFO_TPC_FULL=75,
    GPUPE_TPC_TC_XFC=76,
    GPUPE_TPC_TC_STATE=77,
    GPUPE_TC_STALL=78,
    GPUPE_QUAD0_TAPS=79,
    GPUPE_QUAD1_TAPS=80,
    GPUPE_QUAD2_TAPS=81,
    GPUPE_QUAD3_TAPS=82,
    GPUPE_QUADS=83,
    GPUPE_TCA_SYNC_STALL=84,
    GPUPE_TAG_STALL=85,
    GPUPE_SLICE_STALL=86,
    GPUPE_SKEW_STALL=87,
    GPUPE_TCB_SYNC_STALL=88,
    GPUPE_TCA_VALID=89,
    GPUPE_PROBES_VALID=90,
    GPUPE_MISS_STALL=91,
    GPUPE_FETCH_FIFO_STALL=92,
    GPUPE_TCO_STALL=93,
    GPUPE_ANY_STALL=94,
    GPUPE_TAG_MISSES=95,
    GPUPE_TAG_HITS=96,
    GPUPE_SUB_TAG_MISSES=97,
    GPUPE_SET0_INVALIDATES=98,
    GPUPE_SET1_INVALIDATES=99,
    GPUPE_SET2_INVALIDATES=100,
    GPUPE_SET3_INVALIDATES=101,
    GPUPE_SET0_TAG_MISSES=102,
    GPUPE_SET1_TAG_MISSES=103,
    GPUPE_SET2_TAG_MISSES=104,
    GPUPE_SET3_TAG_MISSES=105,
    GPUPE_SET0_TAG_HITS=106,
    GPUPE_SET1_TAG_HITS=107,
    GPUPE_SET2_TAG_HITS=108,
    GPUPE_SET3_TAG_HITS=109,
    GPUPE_SET0_SUB_TAG_MISSES=110,
    GPUPE_SET1_SUB_TAG_MISSES=111,
    GPUPE_SET2_SUB_TAG_MISSES=112,
    GPUPE_SET3_SUB_TAG_MISSES=113,
    GPUPE_SET0_EVICT1=114,
    GPUPE_SET0_EVICT2=115,
    GPUPE_SET0_EVICT3=116,
    GPUPE_SET0_EVICT4=117,
    GPUPE_SET0_EVICT5=118,
    GPUPE_SET0_EVICT6=119,
    GPUPE_SET0_EVICT7=120,
    GPUPE_SET0_EVICT8=121,
    GPUPE_SET0_EVICT9=122,
    GPUPE_SET0_EVICT10=123,
    GPUPE_SET0_EVICT11=124,
    GPUPE_SET0_EVICT12=125,
    GPUPE_SET0_EVICT13=126,
    GPUPE_SET0_EVICT14=127,
    GPUPE_SET0_EVICT15=128,
    GPUPE_SET0_EVICT16=129,
    GPUPE_SET1_EVICT1=130,
    GPUPE_SET1_EVICT2=131,
    GPUPE_SET1_EVICT3=132,
    GPUPE_SET1_EVICT4=133,
    GPUPE_SET1_EVICT5=134,
    GPUPE_SET1_EVICT6=135,
    GPUPE_SET1_EVICT7=136,
    GPUPE_SET1_EVICT8=137,
    GPUPE_SET1_EVICT9=138,
    GPUPE_SET1_EVICT10=139,
    GPUPE_SET1_EVICT11=140,
    GPUPE_SET1_EVICT12=141,
    GPUPE_SET1_EVICT13=142,
    GPUPE_SET1_EVICT14=143,
    GPUPE_SET1_EVICT15=144,
    GPUPE_SET1_EVICT16=145,
    GPUPE_SET2_EVICT1=146,
    GPUPE_SET2_EVICT2=147,
    GPUPE_SET2_EVICT3=148,
    GPUPE_SET2_EVICT4=149,
    GPUPE_SET2_EVICT5=150,
    GPUPE_SET2_EVICT6=151,
    GPUPE_SET2_EVICT7=152,
    GPUPE_SET2_EVICT8=153,
    GPUPE_SET2_EVICT9=154,
    GPUPE_SET2_EVICT10=155,
    GPUPE_SET2_EVICT11=156,
    GPUPE_SET2_EVICT12=157,
    GPUPE_SET2_EVICT13=158,
    GPUPE_SET2_EVICT14=159,
    GPUPE_SET2_EVICT15=160,
    GPUPE_SET2_EVICT16=161,
    GPUPE_SET3_EVICT1=162,
    GPUPE_SET3_EVICT2=163,
    GPUPE_SET3_EVICT3=164,
    GPUPE_SET3_EVICT4=165,
    GPUPE_SET3_EVICT5=166,
    GPUPE_SET3_EVICT6=167,
    GPUPE_SET3_EVICT7=168,
    GPUPE_SET3_EVICT8=169,
    GPUPE_SET3_EVICT9=170,
    GPUPE_SET3_EVICT10=171,
    GPUPE_SET3_EVICT11=172,
    GPUPE_SET3_EVICT12=173,
    GPUPE_SET3_EVICT13=174,
    GPUPE_SET3_EVICT14=175,
    GPUPE_SET3_EVICT15=176,
    GPUPE_SET3_EVICT16=177,
    GPUPE_FF_EMPTY=178,
    GPUPE_FF_LT_HALF_FULL=179,
    GPUPE_FF_HALF_FULL=180,
    GPUPE_FF_FULL=181,
    GPUPE_FF_XFC=182,
    GPUPE_FF_STALLED=183,
    GPUPE_FG_MASKS=184,
    GPUPE_FG_LEFT_MASKS=185,
    GPUPE_FG_LEFT_MASK_STALLED=186,
    GPUPE_FG_LEFT_NOT_DONE_STALL=187,
    GPUPE_FG_LEFT_FG_STALL=188,
    GPUPE_FG_LEFT_SECTORS=189,
    GPUPE_FG_RIGHT_MASKS=190,
    GPUPE_FG_RIGHT_MASK_STALLED=191,
    GPUPE_FG_RIGHT_NOT_DONE_STALL=192,
    GPUPE_FG_RIGHT_FG_STALL=193,
    GPUPE_FG_RIGHT_SECTORS=194,
    GPUPE_FG0_REQUESTS=195,
    GPUPE_FG0_STALLED=196,
    GPUPE_FG1_REQUESTS=197,
    GPUPE_FG1_STALLED=198,
    GPUPE_MEM_REQ512=199,
    GPUPE_MEM_REQ_SENT=200,
    GPUPE_MEM_AGP_READ_REQ=201,
    GPUPE_MEM_LOCAL_READ_REQ=202,
    GPUPE_TC0_MH_STALLED=203,
    GPUPE_TC1_MH_STALLED=204
} GPUPERFEVENT_TCF;

typedef struct D3DDevice D3DDevice, *PD3DDevice;

typedef struct _D3DConstants _D3DConstants, *P_D3DConstants;

typedef struct GPU_DESTINATIONPACKET GPU_DESTINATIONPACKET, *PGPU_DESTINATIONPACKET;

typedef struct GPU_VALUESPACKET GPU_VALUESPACKET, *PGPU_VALUESPACKET;

typedef struct GPU_PROGRAMPACKET GPU_PROGRAMPACKET, *PGPU_PROGRAMPACKET;

typedef struct GPU_CONTROLPACKET GPU_CONTROLPACKET, *PGPU_CONTROLPACKET;

typedef struct GPU_MISCPACKET GPU_MISCPACKET, *PGPU_MISCPACKET;

typedef struct GPU_POINTPACKET GPU_POINTPACKET, *PGPU_POINTPACKET;

typedef union _D3DConstants_u_0 _D3DConstants_u_0, *P_D3DConstants_u_0;

typedef union _D3DConstants_u_768 _D3DConstants_u_768, *P_D3DConstants_u_768;

typedef union _D3DConstants_u_8960 _D3DConstants_u_8960, *P_D3DConstants_u_8960;

typedef union GPU_COLORMASK GPU_COLORMASK, *PGPU_COLORMASK;

typedef union GPU_STENCILREFMASK GPU_STENCILREFMASK, *PGPU_STENCILREFMASK;

typedef union GPU_PROGRAMCONTROL GPU_PROGRAMCONTROL, *PGPU_PROGRAMCONTROL;

typedef union GPU_INTERPOLATORCONTROL GPU_INTERPOLATORCONTROL, *PGPU_INTERPOLATORCONTROL;

typedef union GPU_WRAPPING0 GPU_WRAPPING0, *PGPU_WRAPPING0;

typedef union GPU_WRAPPING1 GPU_WRAPPING1, *PGPU_WRAPPING1;

typedef struct GPU_MISCPACKET_s_0 GPU_MISCPACKET_s_0, *PGPU_MISCPACKET_s_0;

typedef union GPU_AACONFIG GPU_AACONFIG, *PGPU_AACONFIG;

typedef union GPU_CONST GPU_CONST, *PGPU_CONST;

typedef union GPU_VERTEXREUSEBLOCKCONTROL GPU_VERTEXREUSEBLOCKCONTROL, *PGPU_VERTEXREUSEBLOCKCONTROL;

typedef union GPU_OUTDEALLOCCONTROL GPU_OUTDEALLOCCONTROL, *PGPU_OUTDEALLOCCONTROL;

typedef union GPU_COPYCONTROL GPU_COPYCONTROL, *PGPU_COPYCONTROL;

typedef union GPU_COPYFUNC GPU_COPYFUNC, *PGPU_COPYFUNC;

typedef union GPU_COPYREF GPU_COPYREF, *PGPU_COPYREF;

typedef union GPU_COPYMASK GPU_COPYMASK, *PGPU_COPYMASK;

typedef union GPU_SAMPLECOUNTCONTROL GPU_SAMPLECOUNTCONTROL, *PGPU_SAMPLECOUNTCONTROL;

typedef struct _D3DConstants_u_0_s_1 _D3DConstants_u_0_s_1, *P_D3DConstants_u_0_s_1;

typedef struct _D3DConstants_u_768_s_1 _D3DConstants_u_768_s_1, *P_D3DConstants_u_768_s_1;

typedef struct _D3DConstants_u_8960_s_1 _D3DConstants_u_8960_s_1, *P_D3DConstants_u_8960_s_1;

typedef struct GPU_COLORMASK_s_0 GPU_COLORMASK_s_0, *PGPU_COLORMASK_s_0;

typedef struct GPU_STENCILREFMASK_s_0 GPU_STENCILREFMASK_s_0, *PGPU_STENCILREFMASK_s_0;

typedef struct GPU_PROGRAMCONTROL_s_0 GPU_PROGRAMCONTROL_s_0, *PGPU_PROGRAMCONTROL_s_0;

typedef struct GPU_INTERPOLATORCONTROL_s_0 GPU_INTERPOLATORCONTROL_s_0, *PGPU_INTERPOLATORCONTROL_s_0;

typedef struct GPU_WRAPPING0_s_0 GPU_WRAPPING0_s_0, *PGPU_WRAPPING0_s_0;

typedef struct GPU_WRAPPING1_s_0 GPU_WRAPPING1_s_0, *PGPU_WRAPPING1_s_0;

typedef struct GPU_MISCPACKET_s_0_s_0 GPU_MISCPACKET_s_0_s_0, *PGPU_MISCPACKET_s_0_s_0;

typedef union GPU_COPYDESTPITCH GPU_COPYDESTPITCH, *PGPU_COPYDESTPITCH;

typedef struct GPU_AACONFIG_s_0 GPU_AACONFIG_s_0, *PGPU_AACONFIG_s_0;

typedef struct GPU_CONST_s_0 GPU_CONST_s_0, *PGPU_CONST_s_0;

typedef struct GPU_COPYCONTROL_s_0 GPU_COPYCONTROL_s_0, *PGPU_COPYCONTROL_s_0;

typedef struct GPU_COPYFUNC_s_0 GPU_COPYFUNC_s_0, *PGPU_COPYFUNC_s_0;

typedef struct GPU_COPYREF_s_0 GPU_COPYREF_s_0, *PGPU_COPYREF_s_0;

typedef struct GPU_COPYMASK_s_0 GPU_COPYMASK_s_0, *PGPU_COPYMASK_s_0;

typedef struct GPU_SAMPLECOUNTCONTROL_s_0 GPU_SAMPLECOUNTCONTROL_s_0, *PGPU_SAMPLECOUNTCONTROL_s_0;

typedef union GPU_COPYDESTINFO GPU_COPYDESTINFO, *PGPU_COPYDESTINFO;

typedef struct GPU_COPYDESTPITCH_s_0 GPU_COPYDESTPITCH_s_0, *PGPU_COPYDESTPITCH_s_0;

typedef struct GPU_COPYDESTINFO_s_0 GPU_COPYDESTINFO_s_0, *PGPU_COPYDESTINFO_s_0;

struct GPU_COPYCONTROL_s_0 {
    ulong CopySampleSelect:3;
    ulong CopySrcSelect:3;
    undefined field2_0x1;
    ulong CopyCommand:2;
    undefined field4_0x3;
    undefined field5_0x4;
    ulong DepthClearEnable:1;
    ulong ColorClearEnable:1;
};

union GPU_COPYCONTROL {
    struct GPU_COPYCONTROL_s_0 _s_0;
    ulong dword;
};

struct GPU_COPYDESTINFO_s_0 {
    undefined field0_0x0;
    ulong CopyDestNumber:3;
    ulong CopyDestExpBias:6;
    ulong CopyDestSwap:1;
    ulong CopyDestFormat:6;
    ulong CopyDestSlice:3;
    ulong CopyDestArray:1;
    ulong CopyDestEndian:3;
};

union GPU_COPYDESTINFO {
    struct GPU_COPYDESTINFO_s_0 _s_0;
    ulong dword;
};

struct GPU_COPYDESTPITCH_s_0 {
    undefined field0_0x0;
    undefined field1_0x1;
    ulong CopyDestHeight:14;
    ulong CopyDestPitch:14;
};

union GPU_COPYDESTPITCH {
    struct GPU_COPYDESTPITCH_s_0 _s_0;
    ulong dword;
};

struct GPU_MISCPACKET_s_0_s_0 {
    undefined field0_0x0;
    undefined field1_0x1;
    undefined field2_0x2;
    undefined field3_0x3;
    union GPU_COPYDESTINFO CopyDestInfo;
    undefined field5_0xb;
    undefined field6_0xc;
    undefined field7_0xd;
    undefined field8_0xe;
    undefined field9_0xf;
    undefined field10_0x10;
    undefined field11_0x11;
    undefined field12_0x12;
    undefined field13_0x13;
    undefined field14_0x14;
    undefined field15_0x15;
    undefined field16_0x16;
    undefined field17_0x17;
    undefined field18_0x18;
    undefined field19_0x19;
    undefined field20_0x1a;
    undefined field21_0x1b;
    undefined field22_0x1c;
    undefined field23_0x1d;
    undefined field24_0x1e;
    undefined field25_0x1f;
    undefined field26_0x20;
    undefined field27_0x21;
    undefined field28_0x22;
    undefined field29_0x23;
    undefined field30_0x24;
    undefined field31_0x25;
    undefined field32_0x26;
    undefined field33_0x27;
    undefined field34_0x28;
    undefined field35_0x29;
    undefined field36_0x2a;
    undefined field37_0x2b;
    undefined field38_0x2c;
    undefined field39_0x2d;
    undefined field40_0x2e;
    undefined field41_0x2f;
    undefined field42_0x30;
    undefined field43_0x31;
    undefined field44_0x32;
    undefined field45_0x33;
    undefined field46_0x34;
    undefined field47_0x35;
    undefined field48_0x36;
    undefined field49_0x37;
    undefined field50_0x38;
    undefined field51_0x39;
    undefined field52_0x3a;
    undefined field53_0x3b;
    undefined field54_0x3c;
    undefined field55_0x3d;
    undefined field56_0x3e;
    undefined field57_0x3f;
    undefined field58_0x40;
    undefined field59_0x41;
    undefined field60_0x42;
    undefined field61_0x43;
    undefined field62_0x44;
    undefined field63_0x45;
    undefined field64_0x46;
    undefined field65_0x47;
    undefined field66_0x48;
    undefined field67_0x49;
    undefined field68_0x4a;
    undefined field69_0x4b;
    undefined field70_0x4c;
    undefined field71_0x4d;
    undefined field72_0x4e;
    undefined field73_0x4f;
    undefined field74_0x50;
    undefined field75_0x51;
    undefined field76_0x52;
    undefined field77_0x53;
    undefined field78_0x54;
    undefined field79_0x55;
    undefined field80_0x56;
    undefined field81_0x57;
    undefined field82_0x58;
    undefined field83_0x59;
    undefined field84_0x5a;
    undefined field85_0x5b;
    undefined field86_0x5c;
    undefined field87_0x5d;
    undefined field88_0x5e;
    undefined field89_0x5f;
    undefined field90_0x60;
    undefined field91_0x61;
    undefined field92_0x62;
    undefined field93_0x63;
    undefined field94_0x64;
    undefined field95_0x65;
    undefined field96_0x66;
    undefined field97_0x67;
    undefined field98_0x68;
    undefined field99_0x69;
    undefined field100_0x6a;
    undefined field101_0x6b;
    undefined field102_0x6c;
    undefined field103_0x6d;
    undefined field104_0x6e;
    undefined field105_0x6f;
    ulong HiClear;
    union GPU_COPYDESTPITCH CopyDestPitch;
};

struct GPU_MISCPACKET_s_0 {
    struct GPU_MISCPACKET_s_0_s_0 field0_0x0;
    ulong CopyDestBase;
    undefined field2_0x5;
    undefined field3_0x6;
    undefined field4_0x7;
    undefined field5_0x8;
    undefined field6_0x9;
    undefined field7_0xa;
    undefined field8_0xb;
    undefined field9_0xc;
    undefined field10_0xd;
    undefined field11_0xe;
    undefined field12_0xf;
    undefined field13_0x10;
    undefined field14_0x11;
    undefined field15_0x12;
    undefined field16_0x13;
    undefined field17_0x14;
    undefined field18_0x15;
    undefined field19_0x16;
    undefined field20_0x17;
    undefined field21_0x18;
    undefined field22_0x19;
    undefined field23_0x1a;
    undefined field24_0x1b;
    undefined field25_0x1c;
    undefined field26_0x1d;
    undefined field27_0x1e;
    undefined field28_0x1f;
    undefined field29_0x20;
    undefined field30_0x21;
    undefined field31_0x22;
    undefined field32_0x23;
    undefined field33_0x24;
    undefined field34_0x25;
    undefined field35_0x26;
    undefined field36_0x27;
    undefined field37_0x28;
    undefined field38_0x29;
    undefined field39_0x2a;
    undefined field40_0x2b;
    undefined field41_0x2c;
    undefined field42_0x2d;
    undefined field43_0x2e;
    undefined field44_0x2f;
    undefined field45_0x30;
    undefined field46_0x31;
    undefined field47_0x32;
    undefined field48_0x33;
    undefined field49_0x34;
    undefined field50_0x35;
    undefined field51_0x36;
    undefined field52_0x37;
    undefined field53_0x38;
    undefined field54_0x39;
    undefined field55_0x3a;
    undefined field56_0x3b;
    undefined field57_0x3c;
    undefined field58_0x3d;
    undefined field59_0x3e;
    undefined field60_0x3f;
    undefined field61_0x40;
    undefined field62_0x41;
    undefined field63_0x42;
    undefined field64_0x43;
    undefined field65_0x44;
    undefined field66_0x45;
    undefined field67_0x46;
    undefined field68_0x47;
    undefined field69_0x48;
    undefined field70_0x49;
    undefined field71_0x4a;
    undefined field72_0x4b;
    undefined field73_0x4c;
    undefined field74_0x4d;
    undefined field75_0x4e;
    undefined field76_0x4f;
    undefined field77_0x50;
    undefined field78_0x51;
    undefined field79_0x52;
    undefined field80_0x53;
    undefined field81_0x54;
    undefined field82_0x55;
    undefined field83_0x56;
    undefined field84_0x57;
    undefined field85_0x58;
    undefined field86_0x59;
    undefined field87_0x5a;
    undefined field88_0x5b;
    undefined field89_0x5c;
    undefined field90_0x5d;
    undefined field91_0x5e;
    undefined field92_0x5f;
    undefined field93_0x60;
    undefined field94_0x61;
    undefined field95_0x62;
    undefined field96_0x63;
    undefined field97_0x64;
    union GPU_COPYDESTPITCH CopyDestPitch;
    union GPU_COPYCONTROL CopyControl;
};

struct GPU_COPYFUNC_s_0 {
    ulong CopyFuncGreen:3;
    ulong CopyFuncRed:3;
    ulong CopyFuncAlpha:3;
    ulong CopyFuncBlue:3;
};

union GPU_COPYFUNC {
    struct GPU_COPYFUNC_s_0 _s_0;
    ulong dword;
};

struct _D3DConstants_u_8960_s_1 {
    ulong VertexShaderB[4];
    ulong PixelShaderB[4];
    ulong VertexShaderI[16];
    ulong PixelShaderI[16];
};

union _D3DConstants_u_8960 {
    ulong Flow[40];
    struct _D3DConstants_u_8960_s_1 _s_1;
};

struct GPU_WRAPPING0_s_0 {
    ulong Wrap9:4;
    ulong Wrap8:4;
    ulong Wrap11:4;
    ulong Wrap10:4;
    ulong Wrap13:4;
    ulong Wrap12:4;
    ulong Wrap15:4;
    ulong Wrap14:4;
};

union GPU_WRAPPING0 {
    struct GPU_WRAPPING0_s_0 _s_0;
    ulong dword;
};

struct GPU_INTERPOLATORCONTROL_s_0 {
    ulong ParamShade:16;
    ulong SamplingPattern:16;
};

union GPU_INTERPOLATORCONTROL {
    struct GPU_INTERPOLATORCONTROL_s_0 _s_0;
    ulong dword;
};

struct GPU_WRAPPING1_s_0 {
    ulong Wrap1:4;
    ulong Wrap0:4;
    ulong Wrap3:4;
    ulong Wrap2:4;
    ulong Wrap5:4;
    ulong Wrap4:4;
    ulong Wrap7:4;
    ulong Wrap6:4;
};

union GPU_WRAPPING1 {
    struct GPU_WRAPPING1_s_0 _s_0;
    ulong dword;
};

struct GPU_PROGRAMCONTROL_s_0 {
    ulong _padding_:2;
    ulong VsMaxReg:6;
    ulong _padding_:2;
    ulong PsMaxReg:6;
    ulong VsExportCount:4;
    ulong GenIndexPix:1;
    ulong ParamGen:1;
    ulong PsResource:1;
    ulong VsResource:1;
    ulong GenIndexVtx:1;
    ulong PsExportColorCount:3;
    ulong PsExportZ:1;
    ulong VsExportMode:3;
};

union GPU_PROGRAMCONTROL {
    struct GPU_PROGRAMCONTROL_s_0 _s_0;
    ulong dword;
};

struct GPU_PROGRAMPACKET {
    union GPU_PROGRAMCONTROL ProgramControl;
    union GPU_CONTEXTMISC ContextMisc;
    union GPU_INTERPOLATORCONTROL InterpolatorControl;
    union GPU_WRAPPING0 Wrapping0;
    union GPU_WRAPPING1 Wrapping1;
};

struct GPU_AACONFIG_s_0 {
    undefined field0_0x0;
    ulong MaxSampleDist:4;
    ulong MsaaNumSamples:3;
};

union GPU_AACONFIG {
    struct GPU_AACONFIG_s_0 _s_0;
    ulong dword;
};

struct GPU_DESTINATIONPACKET {
};

struct _D3DConstants_u_0_s_1 {
    undefined1 TextureFetch[624];
    undefined1 VertexFetch[144];
};

union _D3DConstants_u_0 {
    undefined1 Fetch[768];
    struct _D3DConstants_u_0_s_1 _s_1;
};

struct GPU_COPYREF_s_0 {
    ulong CopyRefRed:8;
    ulong CopyRefGreen:8;
    ulong CopyRefBlue:8;
    ulong CopyRefAlpha:8;
};

union GPU_VERTEXREUSEBLOCKCONTROL {
    ulong VtxReuseDepth:8;
    ulong dword;
};

struct _D3DConstants_u_768_s_1 {
    struct __vector4 VertexShaderF[256];
    struct __vector4 PixelShaderF[256];
};

union _D3DConstants_u_768 {
    struct __vector4 Alu[512];
    struct _D3DConstants_u_768_s_1 _s_1;
};

struct _D3DConstants {
    union _D3DConstants_u_0 field0_0x0;
    union _D3DConstants_u_768 field1_0x300;
    union _D3DConstants_u_8960 field2_0x2300;
};

struct GPU_POINTPACKET {
    float PolyOffsetFrontScale;
    float PolyOffsetFrontOffset;
    float PolyOffsetBackScale;
    float PolyOffsetBackOffset;
    float PointXRad;
    float PointYRad;
    float PointConstantSize;
    float PointCullRad;
};

struct GPU_COLORMASK_s_0 {
    ulong Write1:4;
    ulong Write0:4;
    ulong Write3:4;
    ulong Write2:4;
};

union GPU_COLORMASK {
    struct GPU_COLORMASK_s_0 _s_0;
    ulong dword;
};

struct GPU_STENCILREFMASK_s_0 {
    ulong Ref:8;
    ulong Mask:8;
    ulong WriteMask:8;
};

union GPU_STENCILREFMASK {
    struct GPU_STENCILREFMASK_s_0 _s_0;
    ulong dword;
};

struct GPU_VALUESPACKET {
    ulong MaxVtxIndx;
    ulong MinVtxIndx;
    ulong IndxOffset;
    ulong MultiPrimIbResetIndx;
    union GPU_COLORMASK ColorMask;
    float BlendRed;
    float BlendGreen;
    float BlendBlue;
    float BlendAlpha;
    ulong Unused[3];
    union GPU_STENCILREFMASK StencilRefMaskBF;
    union GPU_STENCILREFMASK StencilRefMask;
    float AlphaRef;
    float VportXScale;
    float VportXOffset;
    float VportYScale;
    float VportYOffset;
    float VportZScale;
    float VportZOffset;
};

union GPU_OUTDEALLOCCONTROL {
    ulong DeallocDist:7;
    ulong dword;
};

union GPU_COPYREF {
    struct GPU_COPYREF_s_0 _s_0;
    ulong dword;
};

struct GPU_COPYMASK_s_0 {
    ulong CopyMaskRed:8;
    ulong CopyMaskGreen:8;
    ulong CopyMaskBlue:8;
    ulong CopyMaskAlpha:8;
};

struct GPU_SAMPLECOUNTCONTROL_s_0 {
    ulong CopySampleCount:1;
    ulong ResetSampleCount:1;
};

union GPU_SAMPLECOUNTCONTROL {
    struct GPU_SAMPLECOUNTCONTROL_s_0 _s_0;
    ulong dword;
};

union GPU_COPYMASK {
    struct GPU_COPYMASK_s_0 _s_0;
    ulong dword;
};

struct GPU_CONST_s_0 {
    undefined field0_0x0;
    ulong Size:9;
    undefined field2_0x3;
    ulong Base:9;
};

union GPU_CONST {
    struct GPU_CONST_s_0 _s_0;
    ulong dword;
};

struct GPU_MISCPACKET {
    struct GPU_MISCPACKET_s_0 field0_0x0;
    union GPU_AACONFIG AaConfig;
    union GPU_VTXCONTROL VtxControl;
    float GbVertClipAdj;
    float GbVertDiscAdj;
    float GbHorzClipAdj;
    float GbHorzDiscAdj;
    union GPU_CONST VsConst;
    undefined field8_0x1f;
    undefined field9_0x20;
    undefined field10_0x21;
    undefined field11_0x22;
    undefined field12_0x23;
    undefined field13_0x24;
    undefined field14_0x25;
    undefined field15_0x26;
    undefined field16_0x27;
    undefined field17_0x28;
    undefined field18_0x29;
    undefined field19_0x2a;
    undefined field20_0x2b;
    undefined field21_0x2c;
    undefined field22_0x2d;
    undefined field23_0x2e;
    undefined field24_0x2f;
    undefined field25_0x30;
    undefined field26_0x31;
    undefined field27_0x32;
    undefined field28_0x33;
    undefined field29_0x34;
    undefined field30_0x35;
    undefined field31_0x36;
    undefined field32_0x37;
    undefined field33_0x38;
    undefined field34_0x39;
    undefined field35_0x3a;
    undefined field36_0x3b;
    undefined field37_0x3c;
    undefined field38_0x3d;
    undefined field39_0x3e;
    undefined field40_0x3f;
    ulong Unused2[2];
    ulong AaMask;
    ulong Unused3[3];
    union GPU_VERTEXREUSEBLOCKCONTROL VertexReuseBlockControl;
    union GPU_OUTDEALLOCCONTROL OutDeallocControl;
    union GPU_COPYCONTROL CopyControl;
    undefined field47_0x66;
    undefined field48_0x67;
    undefined field49_0x68;
    undefined field50_0x69;
    undefined field51_0x6a;
    undefined field52_0x6b;
    undefined field53_0x6c;
    undefined field54_0x6d;
    undefined field55_0x6e;
    undefined field56_0x6f;
    undefined field57_0x70;
    undefined field58_0x71;
    undefined field59_0x72;
    undefined field60_0x73;
    ulong DepthClear;
    ulong ColorClear;
    ulong ColorClearLo;
    union GPU_COPYFUNC CopyFunc;
    union GPU_COPYREF CopyRef;
    union GPU_COPYMASK CopyMask;
    ulong CopySurfaceSlice;
    union GPU_SAMPLECOUNTCONTROL SampleCountControl;
    ulong SampleCountAddress;
};

struct GPU_CONTROLPACKET {
};

struct D3DDevice {
    struct _D3DTAGCOLLECTION m_Pending;
    ulong64 m_Predicated_PendingMask2;
    ulong *m_pRing;
    ulong *m_pRingLimit;
    ulong *m_pRingGuarantee;
    ulong m_ReferenceCount;
    void (*m_SetRenderStateCall[101])(struct D3DDevice *, ulong);
    void (*m_SetSamplerStateCall[20])(struct D3DDevice *, ulong, ulong);
    ulong (*m_GetRenderStateCall[101])(struct D3DDevice *);
    ulong (*m_GetSamplerStateCall[20])(struct D3DDevice *, ulong);
    undefined field10_0x408;
    undefined field11_0x409;
    undefined field12_0x40a;
    undefined field13_0x40b;
    undefined field14_0x40c;
    undefined field15_0x40d;
    undefined field16_0x40e;
    undefined field17_0x40f;
    undefined field18_0x410;
    undefined field19_0x411;
    undefined field20_0x412;
    undefined field21_0x413;
    undefined field22_0x414;
    undefined field23_0x415;
    undefined field24_0x416;
    undefined field25_0x417;
    undefined field26_0x418;
    undefined field27_0x419;
    undefined field28_0x41a;
    undefined field29_0x41b;
    undefined field30_0x41c;
    undefined field31_0x41d;
    undefined field32_0x41e;
    undefined field33_0x41f;
    undefined field34_0x420;
    undefined field35_0x421;
    undefined field36_0x422;
    undefined field37_0x423;
    undefined field38_0x424;
    undefined field39_0x425;
    undefined field40_0x426;
    undefined field41_0x427;
    undefined field42_0x428;
    undefined field43_0x429;
    undefined field44_0x42a;
    undefined field45_0x42b;
    undefined field46_0x42c;
    undefined field47_0x42d;
    undefined field48_0x42e;
    undefined field49_0x42f;
    undefined field50_0x430;
    undefined field51_0x431;
    undefined field52_0x432;
    undefined field53_0x433;
    undefined field54_0x434;
    undefined field55_0x435;
    undefined field56_0x436;
    undefined field57_0x437;
    undefined field58_0x438;
    undefined field59_0x439;
    undefined field60_0x43a;
    undefined field61_0x43b;
    undefined field62_0x43c;
    undefined field63_0x43d;
    undefined field64_0x43e;
    undefined field65_0x43f;
    undefined field66_0x440;
    undefined field67_0x441;
    undefined field68_0x442;
    undefined field69_0x443;
    undefined field70_0x444;
    undefined field71_0x445;
    undefined field72_0x446;
    undefined field73_0x447;
    undefined field74_0x448;
    undefined field75_0x449;
    undefined field76_0x44a;
    undefined field77_0x44b;
    undefined field78_0x44c;
    undefined field79_0x44d;
    undefined field80_0x44e;
    undefined field81_0x44f;
    undefined field82_0x450;
    undefined field83_0x451;
    undefined field84_0x452;
    undefined field85_0x453;
    undefined field86_0x454;
    undefined field87_0x455;
    undefined field88_0x456;
    undefined field89_0x457;
    undefined field90_0x458;
    undefined field91_0x459;
    undefined field92_0x45a;
    undefined field93_0x45b;
    undefined field94_0x45c;
    undefined field95_0x45d;
    undefined field96_0x45e;
    undefined field97_0x45f;
    undefined field98_0x460;
    undefined field99_0x461;
    undefined field100_0x462;
    undefined field101_0x463;
    undefined field102_0x464;
    undefined field103_0x465;
    undefined field104_0x466;
    undefined field105_0x467;
    undefined field106_0x468;
    undefined field107_0x469;
    undefined field108_0x46a;
    undefined field109_0x46b;
    undefined field110_0x46c;
    undefined field111_0x46d;
    undefined field112_0x46e;
    undefined field113_0x46f;
    undefined field114_0x470;
    undefined field115_0x471;
    undefined field116_0x472;
    undefined field117_0x473;
    undefined field118_0x474;
    undefined field119_0x475;
    undefined field120_0x476;
    undefined field121_0x477;
    undefined field122_0x478;
    undefined field123_0x479;
    undefined field124_0x47a;
    undefined field125_0x47b;
    undefined field126_0x47c;
    undefined field127_0x47d;
    undefined field128_0x47e;
    undefined field129_0x47f;
    struct _D3DConstants m_Constants;
    float m_ClipPlanes[6][4];
    struct GPU_DESTINATIONPACKET m_DestinationPacket;
    undefined field133_0x2881;
    undefined field134_0x2882;
    undefined field135_0x2883;
    undefined field136_0x2884;
    undefined field137_0x2885;
    undefined field138_0x2886;
    undefined field139_0x2887;
    undefined field140_0x2888;
    undefined field141_0x2889;
    undefined field142_0x288a;
    undefined field143_0x288b;
    undefined field144_0x288c;
    undefined field145_0x288d;
    undefined field146_0x288e;
    undefined field147_0x288f;
    undefined field148_0x2890;
    undefined field149_0x2891;
    undefined field150_0x2892;
    undefined field151_0x2893;
    undefined field152_0x2894;
    undefined field153_0x2895;
    undefined field154_0x2896;
    undefined field155_0x2897;
    undefined field156_0x2898;
    undefined field157_0x2899;
    undefined field158_0x289a;
    undefined field159_0x289b;
    undefined field160_0x289c;
    undefined field161_0x289d;
    undefined field162_0x289e;
    undefined field163_0x289f;
    undefined field164_0x28a0;
    undefined field165_0x28a1;
    undefined field166_0x28a2;
    undefined field167_0x28a3;
    undefined field168_0x28a4;
    undefined field169_0x28a5;
    undefined field170_0x28a6;
    undefined field171_0x28a7;
    undefined field172_0x28a8;
    undefined field173_0x28a9;
    undefined field174_0x28aa;
    undefined field175_0x28ab;
    undefined field176_0x28ac;
    undefined field177_0x28ad;
    undefined field178_0x28ae;
    undefined field179_0x28af;
    undefined field180_0x28b0;
    undefined field181_0x28b1;
    undefined field182_0x28b2;
    undefined field183_0x28b3;
    undefined field184_0x28b4;
    undefined field185_0x28b5;
    undefined field186_0x28b6;
    undefined field187_0x28b7;
    undefined field188_0x28b8;
    undefined field189_0x28b9;
    undefined field190_0x28ba;
    undefined field191_0x28bb;
    undefined field192_0x28bc;
    undefined field193_0x28bd;
    undefined field194_0x28be;
    undefined field195_0x28bf;
    struct GPU_WINDOWPACKET m_WindowPacket;
    undefined field197_0x28c1;
    undefined field198_0x28c2;
    undefined field199_0x28c3;
    undefined field200_0x28c4;
    undefined field201_0x28c5;
    undefined field202_0x28c6;
    undefined field203_0x28c7;
    undefined field204_0x28c8;
    undefined field205_0x28c9;
    undefined field206_0x28ca;
    undefined field207_0x28cb;
    struct GPU_VALUESPACKET m_ValuesPacket;
    struct GPU_PROGRAMPACKET m_ProgramPacket;
    struct GPU_CONTROLPACKET m_ControlPacket;
    undefined field211_0x2935;
    undefined field212_0x2936;
    undefined field213_0x2937;
    undefined field214_0x2938;
    undefined field215_0x2939;
    undefined field216_0x293a;
    undefined field217_0x293b;
    undefined field218_0x293c;
    undefined field219_0x293d;
    undefined field220_0x293e;
    undefined field221_0x293f;
    undefined field222_0x2940;
    undefined field223_0x2941;
    undefined field224_0x2942;
    undefined field225_0x2943;
    undefined field226_0x2944;
    undefined field227_0x2945;
    undefined field228_0x2946;
    undefined field229_0x2947;
    undefined field230_0x2948;
    undefined field231_0x2949;
    undefined field232_0x294a;
    undefined field233_0x294b;
    undefined field234_0x294c;
    undefined field235_0x294d;
    undefined field236_0x294e;
    undefined field237_0x294f;
    undefined field238_0x2950;
    undefined field239_0x2951;
    undefined field240_0x2952;
    undefined field241_0x2953;
    undefined field242_0x2954;
    undefined field243_0x2955;
    undefined field244_0x2956;
    undefined field245_0x2957;
    undefined field246_0x2958;
    undefined field247_0x2959;
    undefined field248_0x295a;
    undefined field249_0x295b;
    undefined field250_0x295c;
    undefined field251_0x295d;
    undefined field252_0x295e;
    undefined field253_0x295f;
    undefined field254_0x2960;
    undefined field255_0x2961;
    undefined field256_0x2962;
    undefined field257_0x2963;
    struct GPU_TESSELLATORPACKET m_TessellatorPacket;
    struct GPU_MISCPACKET m_MiscPacket;
    struct GPU_POINTPACKET m_PointPacket;
    uchar m_MaxAnisotropy[26];
    uchar m_ZFilter[26];
    undefined field263_0x2aa4;
    undefined field264_0x2aa5;
    undefined field265_0x2aa6;
    undefined field266_0x2aa7;
    undefined field267_0x2aa8;
    undefined field268_0x2aa9;
    undefined field269_0x2aaa;
    undefined field270_0x2aab;
    undefined field271_0x2aac;
    undefined field272_0x2aad;
    undefined field273_0x2aae;
    undefined field274_0x2aaf;
    undefined field275_0x2ab0;
    undefined field276_0x2ab1;
    undefined field277_0x2ab2;
    undefined field278_0x2ab3;
    undefined field279_0x2ab4;
    undefined field280_0x2ab5;
    undefined field281_0x2ab6;
    undefined field282_0x2ab7;
    undefined field283_0x2ab8;
    undefined field284_0x2ab9;
    undefined field285_0x2aba;
    undefined field286_0x2abb;
    undefined field287_0x2abc;
    undefined field288_0x2abd;
    undefined field289_0x2abe;
    undefined field290_0x2abf;
    undefined field291_0x2ac0;
    undefined field292_0x2ac1;
    undefined field293_0x2ac2;
    undefined field294_0x2ac3;
    undefined field295_0x2ac4;
    undefined field296_0x2ac5;
    undefined field297_0x2ac6;
    undefined field298_0x2ac7;
    undefined field299_0x2ac8;
    undefined field300_0x2ac9;
    undefined field301_0x2aca;
    undefined field302_0x2acb;
    undefined field303_0x2acc;
    undefined field304_0x2acd;
    undefined field305_0x2ace;
    undefined field306_0x2acf;
    undefined field307_0x2ad0;
    undefined field308_0x2ad1;
    undefined field309_0x2ad2;
    undefined field310_0x2ad3;
    undefined field311_0x2ad4;
    undefined field312_0x2ad5;
    undefined field313_0x2ad6;
    undefined field314_0x2ad7;
    undefined field315_0x2ad8;
    undefined field316_0x2ad9;
    undefined field317_0x2ada;
    undefined field318_0x2adb;
    undefined field319_0x2adc;
    undefined field320_0x2add;
    undefined field321_0x2ade;
    undefined field322_0x2adf;
    undefined field323_0x2ae0;
    undefined field324_0x2ae1;
    undefined field325_0x2ae2;
    undefined field326_0x2ae3;
    undefined field327_0x2ae4;
    undefined field328_0x2ae5;
    undefined field329_0x2ae6;
    undefined field330_0x2ae7;
    undefined field331_0x2ae8;
    undefined field332_0x2ae9;
    undefined field333_0x2aea;
    undefined field334_0x2aeb;
    undefined field335_0x2aec;
    undefined field336_0x2aed;
    undefined field337_0x2aee;
    undefined field338_0x2aef;
    undefined field339_0x2af0;
    undefined field340_0x2af1;
    undefined field341_0x2af2;
    undefined field342_0x2af3;
    undefined field343_0x2af4;
    undefined field344_0x2af5;
    undefined field345_0x2af6;
    undefined field346_0x2af7;
    undefined field347_0x2af8;
    undefined field348_0x2af9;
    undefined field349_0x2afa;
    undefined field350_0x2afb;
    undefined field351_0x2afc;
    undefined field352_0x2afd;
    undefined field353_0x2afe;
    undefined field354_0x2aff;
};

typedef struct _IMAGE_IA64_RUNTIME_FUNCTION_ENTRY _IMAGE_IA64_RUNTIME_FUNCTION_ENTRY, *P_IMAGE_IA64_RUNTIME_FUNCTION_ENTRY;

struct _IMAGE_IA64_RUNTIME_FUNCTION_ENTRY {
    ulong BeginAddress;
    ulong EndAddress;
    ulong UnwindInfoAddress;
};

typedef enum BIUPERFREGISTER {
    BIUPERFREG_BIU_PERFMON_CNTL=64,
    BIUPERFREG_BIU_PERFCOUNTER0_SELECT=65,
    BIUPERFREG_BIU_PERFCOUNTER0_HI=66,
    BIUPERFREG_BIU_PERFCOUNTER0_LOW=67,
    BIUPERFREG_BIU_PERFCOUNTER1_SELECT=68,
    BIUPERFREG_BIU_PERFCOUNTER1_HI=69,
    BIUPERFREG_BIU_PERFCOUNTER1_LOW=70,
    BIUPERFREG_BIU_PERFCOUNTER2_SELECT=71,
    BIUPERFREG_BIU_PERFCOUNTER2_HI=72,
    BIUPERFREG_BIU_PERFCOUNTER2_LOW=73,
    BIUPERFREG_BIU_PERFCOUNTER3_SELECT=74,
    BIUPERFREG_BIU_PERFCOUNTER3_HI=75,
    BIUPERFREG_BIU_PERFCOUNTER3_LOW=76
} BIUPERFREGISTER;

typedef enum _ACL_INFORMATION_CLASS {
    AclRevisionInformation=1,
    AclSizeInformation=2
} _ACL_INFORMATION_CLASS;

typedef enum TCF_PERFCOUNT_SELECT {
    VALID_CYCLES=0,
    SINGLE_PHASES=1,
    ANISO_PHASES=2,
    MIP_PHASES=3,
    VOL_PHASES=4,
    MIP_VOL_PHASES=5,
    MIP_ANISO_PHASES=6,
    VOL_ANISO_PHASES=7,
    ANISO_2_1_PHASES=8,
    ANISO_4_1_PHASES=9,
    ANISO_6_1_PHASES=10,
    ANISO_8_1_PHASES=11,
    ANISO_10_1_PHASES=12,
    ANISO_12_1_PHASES=13,
    ANISO_14_1_PHASES=14,
    ANISO_16_1_PHASES=15,
    MIP_VOL_ANISO_PHASES=16,
    ALIGN_2_PHASES=17,
    ALIGN_4_PHASES=18,
    TPC_BUSY=19,
    TPC_STALLED=20,
    TPC_STARVED=21,
    TPC_WORKING=22,
    TPC_WALKER_BUSY=23,
    TPC_WALKER_STALLED=24,
    TPC_WALKER_WORKING=25,
    TPC_ALIGNER_BUSY=26,
    TPC_ALIGNER_STALLED=27,
    TPC_ALIGNER_STALLED_BY_BLEND=28,
    TPC_ALIGNER_STALLED_BY_CACHE=29,
    TPC_ALIGNER_WORKING=30,
    TPC_BLEND_BUSY=31,
    TPC_BLEND_SYNC=32,
    TPC_BLEND_STARVED=33,
    TPC_BLEND_WORKING=34,
    OPCODE_0x00=35,
    OPCODE_0x01=36,
    OPCODE_0x04=37,
    OPCODE_0x10=38,
    OPCODE_0x11=39,
    OPCODE_0x12=40,
    OPCODE_0x13=41,
    OPCODE_0x18=42,
    OPCODE_0x19=43,
    OPCODE_0x1A=44,
    OPCODE_OTHER=45,
    IN_FIFO_0_EMPTY=56,
    IN_FIFO_0_LT_HALF_FULL=57,
    IN_FIFO_0_HALF_FULL=58,
    IN_FIFO_0_FULL=59,
    IN_FIFO_1_EMPTY=60,
    IN_FIFO_1_LT_HALF_FULL=61,
    IN_FIFO_1_HALF_FULL=62,
    IN_FIFO_1_FULL=63,
    IN_FIFO_2_EMPTY=64,
    IN_FIFO_2_LT_HALF_FULL=65,
    IN_FIFO_2_HALF_FULL=66,
    IN_FIFO_2_FULL=67,
    IN_FIFO_3_EMPTY=68,
    IN_FIFO_3_LT_HALF_FULL=69,
    IN_FIFO_3_HALF_FULL=70,
    IN_FIFO_3_FULL=71,
    IN_FIFO_TPC_EMPTY=72,
    IN_FIFO_TPC_LT_HALF_FULL=73,
    IN_FIFO_TPC_HALF_FULL=74,
    IN_FIFO_TPC_FULL=75,
    TPC_TC_XFC=76,
    TPC_TC_STATE=77,
    TC_STALL=78,
    QUAD0_TAPS=79,
    QUAD1_TAPS=80,
    QUAD2_TAPS=81,
    QUAD3_TAPS=82,
    QUADS=83,
    TCA_SYNC_STALL=84,
    TAG_STALL=85,
    SLICE_STALL=86,
    SKEW_STALL=87,
    TCB_SYNC_STALL=88,
    TCA_VALID=89,
    PROBES_VALID=90,
    MISS_STALL=91,
    FETCH_FIFO_STALL=92,
    TCO_STALL=93,
    ANY_STALL=94,
    TAG_MISSES=95,
    TAG_HITS=96,
    SUB_TAG_MISSES=97,
    SET0_INVALIDATES=98,
    SET1_INVALIDATES=99,
    SET2_INVALIDATES=100,
    SET3_INVALIDATES=101,
    SET0_TAG_MISSES=102,
    SET1_TAG_MISSES=103,
    SET2_TAG_MISSES=104,
    SET3_TAG_MISSES=105,
    SET0_TAG_HITS=106,
    SET1_TAG_HITS=107,
    SET2_TAG_HITS=108,
    SET3_TAG_HITS=109,
    SET0_SUB_TAG_MISSES=110,
    SET1_SUB_TAG_MISSES=111,
    SET2_SUB_TAG_MISSES=112,
    SET3_SUB_TAG_MISSES=113,
    SET0_EVICT1=114,
    SET0_EVICT2=115,
    SET0_EVICT3=116,
    SET0_EVICT4=117,
    SET0_EVICT5=118,
    SET0_EVICT6=119,
    SET0_EVICT7=120,
    SET0_EVICT8=121,
    SET0_EVICT9=122,
    SET0_EVICT10=123,
    SET0_EVICT11=124,
    SET0_EVICT12=125,
    SET0_EVICT13=126,
    SET0_EVICT14=127,
    SET0_EVICT15=128,
    SET0_EVICT16=129,
    SET1_EVICT1=130,
    SET1_EVICT2=131,
    SET1_EVICT3=132,
    SET1_EVICT4=133,
    SET1_EVICT5=134,
    SET1_EVICT6=135,
    SET1_EVICT7=136,
    SET1_EVICT8=137,
    SET1_EVICT9=138,
    SET1_EVICT10=139,
    SET1_EVICT11=140,
    SET1_EVICT12=141,
    SET1_EVICT13=142,
    SET1_EVICT14=143,
    SET1_EVICT15=144,
    SET1_EVICT16=145,
    SET2_EVICT1=146,
    SET2_EVICT2=147,
    SET2_EVICT3=148,
    SET2_EVICT4=149,
    SET2_EVICT5=150,
    SET2_EVICT6=151,
    SET2_EVICT7=152,
    SET2_EVICT8=153,
    SET2_EVICT9=154,
    SET2_EVICT10=155,
    SET2_EVICT11=156,
    SET2_EVICT12=157,
    SET2_EVICT13=158,
    SET2_EVICT14=159,
    SET2_EVICT15=160,
    SET2_EVICT16=161,
    SET3_EVICT1=162,
    SET3_EVICT2=163,
    SET3_EVICT3=164,
    SET3_EVICT4=165,
    SET3_EVICT5=166,
    SET3_EVICT6=167,
    SET3_EVICT7=168,
    SET3_EVICT8=169,
    SET3_EVICT9=170,
    SET3_EVICT10=171,
    SET3_EVICT11=172,
    SET3_EVICT12=173,
    SET3_EVICT13=174,
    SET3_EVICT14=175,
    SET3_EVICT15=176,
    SET3_EVICT16=177,
    FF_EMPTY=178,
    FF_LT_HALF_FULL=179,
    FF_HALF_FULL=180,
    FF_FULL=181,
    FF_XFC=182,
    FF_STALLED=183,
    FG_MASKS=184,
    FG_LEFT_MASKS=185,
    FG_LEFT_MASK_STALLED=186,
    FG_LEFT_NOT_DONE_STALL=187,
    FG_LEFT_FG_STALL=188,
    FG_LEFT_SECTORS=189,
    FG_RIGHT_MASKS=190,
    FG_RIGHT_MASK_STALLED=191,
    FG_RIGHT_NOT_DONE_STALL=192,
    FG_RIGHT_FG_STALL=193,
    FG_RIGHT_SECTORS=194,
    FG0_REQUESTS=195,
    FG0_STALLED=196,
    FG1_REQUESTS=197,
    FG1_STALLED=198,
    MEM_REQ512=199,
    MEM_REQ_SENT=200,
    MEM_AGP_READ_REQ=201,
    MEM_LOCAL_READ_REQ=202,
    TC0_MH_STALLED=203,
    TC1_MH_STALLED=204
} TCF_PERFCOUNT_SELECT;

typedef union DCCG_CLK_CNTL DCCG_CLK_CNTL, *PDCCG_CLK_CNTL;


/* WARNING! conflicting data type names: /iw5mp_xenon.pdb/DCCG_CLK_CNTL/<unnamed-type-bitfields> - /iw5mp_xenon.pdb/D1SCL_COEF_RAM_SELECT/<unnamed-type-bitfields> */

union DCCG_CLK_CNTL {
    struct <unnamed-type-bitfields> bitfields;
    struct <unnamed-type-bitfields> bits;
    uint u32All;
    int i32All;
    float f32All;
};

typedef enum _D3DHISTENCILCMPFUNC {
    D3DHSCMP_EQUAL=0,
    D3DHSCMP_NOTEQUAL=1,
    D3DHSCMP_FORCE_DWORD=255
} _D3DHISTENCILCMPFUNC;

typedef struct D3DVolume D3DVolume, *PD3DVolume;

typedef union D3DVolume_u_28 D3DVolume_u_28, *PD3DVolume_u_28;

typedef struct D3DVolume_u_28_s_0 D3DVolume_u_28_s_0, *PD3DVolume_u_28_s_0;

struct D3DVolume_u_28_s_0 {
    undefined field0_0x0;
    undefined field1_0x1;
    undefined field2_0x2;
    ulong MipLevel:4;
    undefined field4_0x4;
    undefined field5_0x5;
    undefined field6_0x6;
    undefined field7_0x7;
};

union D3DVolume_u_28 {
    struct D3DVolume_u_28_s_0 _s_0;
    ulong ArrayIndex:6;
};

struct D3DVolume {
    undefined field0_0x0;
    undefined field1_0x1;
    undefined field2_0x2;
    undefined field3_0x3;
    undefined field4_0x4;
    undefined field5_0x5;
    undefined field6_0x6;
    undefined field7_0x7;
    undefined field8_0x8;
    undefined field9_0x9;
    undefined field10_0xa;
    undefined field11_0xb;
    undefined field12_0xc;
    undefined field13_0xd;
    undefined field14_0xe;
    undefined field15_0xf;
    undefined field16_0x10;
    undefined field17_0x11;
    undefined field18_0x12;
    undefined field19_0x13;
    undefined field20_0x14;
    undefined field21_0x15;
    undefined field22_0x16;
    undefined field23_0x17;
    struct D3DBaseTexture *Parent;
    union D3DVolume_u_28 field25_0x1c;
};

typedef enum ColorArrayX {
    ARRAYX_2D_COLOR=0,
    ARRAYX_3D_SLICE_COLOR=1
} ColorArrayX;

typedef enum _D3DDECLUSAGE {
    D3DDECLUSAGE_POSITION=0,
    D3DDECLUSAGE_BLENDWEIGHT=1,
    D3DDECLUSAGE_BLENDINDICES=2,
    D3DDECLUSAGE_NORMAL=3,
    D3DDECLUSAGE_PSIZE=4,
    D3DDECLUSAGE_TEXCOORD=5,
    D3DDECLUSAGE_TANGENT=6,
    D3DDECLUSAGE_BINORMAL=7,
    D3DDECLUSAGE_TESSFACTOR=8,
    D3DDECLUSAGE_COLOR=10,
    D3DDECLUSAGE_FOG=11,
    D3DDECLUSAGE_DEPTH=12,
    D3DDECLUSAGE_SAMPLE=13
} _D3DDECLUSAGE;

typedef enum _XPROFILE_RACE_ACCELERATOR_CONTROL_OPTIONS {
    XPROFILE_RACE_ACCELERATOR_CONTROL_TRIGGER=0,
    XPROFILE_RACE_ACCELERATOR_CONTROL_BUTTON=1
} _XPROFILE_RACE_ACCELERATOR_CONTROL_OPTIONS;

typedef enum <unnamed-tag_00001185> {
    SPR_ICIDR1=213
} <unnamed-tag_00001185>;

typedef struct XpsGlobal XpsGlobal, *PXpsGlobal;

typedef struct D3DXpsThread D3DXpsThread, *PD3DXpsThread;

typedef struct _KDPC _KDPC, *P_KDPC;

struct _KDPC {
    short Type;
    uchar InsertedNumber;
    uchar TargetNumber;
    struct _LIST_ENTRY DpcListEntry;
    void (*DeferredRoutine)(struct _KDPC *, void *, void *, void *);
    void *DeferredContext;
    void *SystemArgument1;
    void *SystemArgument2;
};

struct D3DXpsThread {
    struct XpsGlobal *pGlobal;
    ulong Thread;
    ulong EndPosition;
    ulong Wrap;
    ulong *pRingStart;
    ulong *pRingLimit;
    ulong WriteBackAddress;
    ulong *pTail;
    struct _KDPC WorkerKDpc;
};

struct XpsGlobal {
    long Cookie;
    ulong *pPrimaryRingBuffer;
    ulong PrimaryRingBufferIndex;
    ulong PrimaryRingBufferIndexMask;
    void (*pCallback)(struct D3DXpsThread *, void *, void *, ulong);
    void *pCallbackContext;
    ulong NextInstanceIndex;
    ulong SubmitInstanceCount;
    void *pSubmitData;
    ulong *pCommand;
    long ThreadCount;
    long IdleThreadCount;
};

typedef enum _RESTRICTED_PRIVILEGE_BIT {
    RESTRICTED_PRIVILEGE_BIT_REVERT=0,
    RESTRICTED_PRIVILEGE_BIT_INTEGRATION=1,
    RESTRICTED_PRIVILEGE_BIT_CHINA_TRIAL=2,
    RESTRICTED_PRIVILEGE_BIT_XGD2_EXEMPT=3,
    RESTRICTED_PRIVILEGE_BIT_PROFILE=4,
    RESTRICTED_PRIVILEGE_BIT_PREVIEW=5,
    RESTRICTED_PRIVILEGE_BIT_AUTOMATION=6,
    RESTRICTED_PRIVILEGE_BIT_DIRECT_USB_STORAGE=7,
    RESTRICTED_PRIVILEGE_BIT_INTERNAL=8,
    RESTRICTED_PRIVILEGE_BIT_BETA=9,
    RESTRICTED_PRIVILEGE_BIT_PUBLIC=10,
    RESTRICTED_PRIVILEGE_BIT_HARDWARE=11,
    RESTRICTED_PRIVILEGE_BIT_0C=12,
    RESTRICTED_PRIVILEGE_BIT_0D=13,
    RESTRICTED_PRIVILEGE_BIT_0E=14,
    RESTRICTED_PRIVILEGE_BIT_0F=15,
    RESTRICTED_PRIVILEGE_BIT_10=16,
    RESTRICTED_PRIVILEGE_BIT_11=17,
    RESTRICTED_PRIVILEGE_BIT_12=18,
    RESTRICTED_PRIVILEGE_BIT_13=19,
    RESTRICTED_PRIVILEGE_BIT_14=20,
    RESTRICTED_PRIVILEGE_BIT_15=21,
    RESTRICTED_PRIVILEGE_BIT_16=22,
    RESTRICTED_PRIVILEGE_BIT_17=23,
    RESTRICTED_PRIVILEGE_BIT_18=24,
    RESTRICTED_PRIVILEGE_BIT_19=25,
    RESTRICTED_PRIVILEGE_BIT_1A=26,
    RESTRICTED_PRIVILEGE_BIT_1B=27,
    RESTRICTED_PRIVILEGE_BIT_1C=28,
    RESTRICTED_PRIVILEGE_BIT_1D=29,
    RESTRICTED_PRIVILEGE_BIT_1E=30,
    RESTRICTED_PRIVILEGE_BIT_1F=31,
    RESTRICTED_PRIVILEGE_BIT_20=32,
    RESTRICTED_PRIVILEGE_BIT_21=33,
    RESTRICTED_PRIVILEGE_BIT_22=34,
    RESTRICTED_PRIVILEGE_BIT_23=35,
    RESTRICTED_PRIVILEGE_BIT_24=36,
    RESTRICTED_PRIVILEGE_BIT_25=37,
    RESTRICTED_PRIVILEGE_BIT_26=38,
    RESTRICTED_PRIVILEGE_BIT_27=39,
    RESTRICTED_PRIVILEGE_BIT_28=40,
    RESTRICTED_PRIVILEGE_BIT_29=41,
    RESTRICTED_PRIVILEGE_BIT_2A=42,
    RESTRICTED_PRIVILEGE_BIT_2B=43,
    RESTRICTED_PRIVILEGE_BIT_2C=44,
    RESTRICTED_PRIVILEGE_BIT_2D=45,
    RESTRICTED_PRIVILEGE_BIT_2E=46,
    RESTRICTED_PRIVILEGE_BIT_2F=47,
    RESTRICTED_PRIVILEGE_BIT_30=48,
    RESTRICTED_PRIVILEGE_BIT_31=49,
    RESTRICTED_PRIVILEGE_BIT_32=50,
    RESTRICTED_PRIVILEGE_BIT_33=51,
    RESTRICTED_PRIVILEGE_BIT_34=52,
    RESTRICTED_PRIVILEGE_BIT_35=53,
    RESTRICTED_PRIVILEGE_BIT_36=54,
    RESTRICTED_PRIVILEGE_BIT_37=55,
    RESTRICTED_PRIVILEGE_BIT_38=56,
    RESTRICTED_PRIVILEGE_BIT_39=57,
    RESTRICTED_PRIVILEGE_BIT_3A=58,
    RESTRICTED_PRIVILEGE_BIT_3B=59,
    RESTRICTED_PRIVILEGE_BIT_3C=60,
    RESTRICTED_PRIVILEGE_BIT_3D=61,
    RESTRICTED_PRIVILEGE_BIT_3E=62,
    RESTRICTED_PRIVILEGE_BIT_3F=63,
    RESTRICTED_PRIVILEGE_BIT_MAXIMUM=64
} _RESTRICTED_PRIVILEGE_BIT;

typedef enum GPUCMPFUNC {
    GPUCMP_NEVER=0,
    GPUCMP_LESS=1,
    GPUCMP_EQUAL=2,
    GPUCMP_LESS_EQUAL=3,
    GPUCMP_GREATER=4,
    GPUCMP_NOT_EQUAL=5,
    GPUCMP_GREATER_EQUAL=6,
    GPUCMP_ALWAYS=7
} GPUCMPFUNC;

typedef enum VGT_GROUP_CONV_SEL {
    VGT_GRP_INDEX_16=0,
    VGT_GRP_INDEX_32=1,
    VGT_GRP_UINT_16=2,
    VGT_GRP_UINT_32=3,
    VGT_GRP_SINT_16=4,
    VGT_GRP_SINT_32=5,
    VGT_GRP_FLOAT_32=6,
    VGT_GRP_AUTO_PRIM=7,
    VGT_GRP_FIX_1_23_TO_FLOAT=8
} VGT_GROUP_CONV_SEL;

typedef enum <unnamed-tag_000010F3> {
    SPR_CTRL_R=136
} <unnamed-tag_000010F3>;

typedef enum _D3DXPLAYBACK_TYPE {
    D3DXPLAY_LOOP=0,
    D3DXPLAY_ONCE=1,
    D3DXPLAY_PINGPONG=2,
    D3DXPLAY_FORCE_DWORD=255
} _D3DXPLAYBACK_TYPE;

typedef enum GPUPERFEVENT_VGT {
    GPUPE_VGT_SQ_EVENT_WINDOW_ACTIVE=0,
    GPUPE_VGT_SQ_SEND=1,
    GPUPE_VGT_SQ_STALLED=2,
    GPUPE_VGT_SQ_STARVED_BUSY=3,
    GPUPE_VGT_SQ_STARVED_IDLE=4,
    GPUPE_VGT_SQ_STATIC=5,
    GPUPE_VGT_PA_EVENT_WINDOW_ACTIVE=6,
    GPUPE_VGT_PA_CLIP_V_SEND=7,
    GPUPE_VGT_PA_CLIP_V_STALLED=8,
    GPUPE_VGT_PA_CLIP_V_STARVED_BUSY=9,
    GPUPE_VGT_PA_CLIP_V_STARVED_IDLE=10,
    GPUPE_VGT_PA_CLIP_V_STATIC=11,
    GPUPE_VGT_PA_CLIP_P_SEND=12,
    GPUPE_VGT_PA_CLIP_P_STALLED=13,
    GPUPE_VGT_PA_CLIP_P_STARVED_BUSY=14,
    GPUPE_VGT_PA_CLIP_P_STARVED_IDLE=15,
    GPUPE_VGT_PA_CLIP_P_STATIC=16,
    GPUPE_VGT_PA_CLIP_S_SEND=17,
    GPUPE_VGT_PA_CLIP_S_STALLED=18,
    GPUPE_VGT_PA_CLIP_S_STARVED_BUSY=19,
    GPUPE_VGT_PA_CLIP_S_STARVED_IDLE=20,
    GPUPE_VGT_PA_CLIP_S_STATIC=21,
    GPUPE_RBIU_FIFOS_EVENT_WINDOW_ACTIVE=22,
    GPUPE_RBIU_IMMED_DATA_FIFO_STARVED=23,
    GPUPE_RBIU_IMMED_DATA_FIFO_STALLED=24,
    GPUPE_RBIU_DMA_REQUEST_FIFO_STARVED=25,
    GPUPE_RBIU_DMA_REQUEST_FIFO_STALLED=26,
    GPUPE_RBIU_DRAW_INITIATOR_FIFO_STARVED=27,
    GPUPE_RBIU_DRAW_INITIATOR_FIFO_STALLED=28,
    GPUPE_SPARE29=29,
    GPUPE_SPARE30=30,
    GPUPE_SPARE31=31,
    GPUPE_SPARE32=32,
    GPUPE_SPARE33=33,
    GPUPE_SPARE34=34,
    GPUPE_SPARE35=35,
    GPUPE_SPARE36=36,
    GPUPE_SPARE37=37,
    GPUPE_SPARE38=38,
    GPUPE_SPARE39=39,
    GPUPE_TE_SU_IN_VALID=40,
    GPUPE_TE_SU_IN_READ=41,
    GPUPE_TE_SU_IN_PRIM=42,
    GPUPE_TE_SU_IN_EOP=43,
    GPUPE_TE_SU_IN_NULL_PRIM=44,
    GPUPE_TE_WK_IN_VALID=45,
    GPUPE_TE_WK_IN_READ=46,
    GPUPE_TE_OUT_PRIM_VALID=47,
    GPUPE_TE_OUT_PRIM_READ=48
} GPUPERFEVENT_VGT;

typedef enum GPUPERFEVENT_TCM {
    GPUPE_QUAD0_RD_LAT_FIFO_EMPTY=0,
    GPUPE_QUAD0_RD_LAT_FIFO_16TH_FULL=1,
    GPUPE_QUAD0_RD_LAT_FIFO_8TH_FULL=2,
    GPUPE_QUAD0_RD_LAT_FIFO_4TH_FULL=3,
    GPUPE_QUAD0_RD_LAT_FIFO_HALF_FULL=4,
    GPUPE_QUAD0_RD_LAT_FIFO_FULL=5,
    GPUPE_QUAD0_RD_LAT_FIFO_LT_16TH_FULL=6,
    GPUPE_QUAD1_RD_LAT_FIFO_EMPTY=7,
    GPUPE_QUAD1_RD_LAT_FIFO_16TH_FULL=8,
    GPUPE_QUAD1_RD_LAT_FIFO_8TH_FULL=9,
    GPUPE_QUAD1_RD_LAT_FIFO_4TH_FULL=10,
    GPUPE_QUAD1_RD_LAT_FIFO_HALF_FULL=11,
    GPUPE_QUAD1_RD_LAT_FIFO_FULL=12,
    GPUPE_QUAD1_RD_LAT_FIFO_LT_16TH_FULL=13,
    GPUPE_QUAD2_RD_LAT_FIFO_EMPTY=14,
    GPUPE_QUAD2_RD_LAT_FIFO_16TH_FULL=15,
    GPUPE_QUAD2_RD_LAT_FIFO_8TH_FULL=16,
    GPUPE_QUAD2_RD_LAT_FIFO_4TH_FULL=17,
    GPUPE_QUAD2_RD_LAT_FIFO_HALF_FULL=18,
    GPUPE_QUAD2_RD_LAT_FIFO_FULL=19,
    GPUPE_QUAD2_RD_LAT_FIFO_LT_16TH_FULL=20,
    GPUPE_QUAD3_RD_LAT_FIFO_EMPTY=21,
    GPUPE_QUAD3_RD_LAT_FIFO_16TH_FULL=22,
    GPUPE_QUAD3_RD_LAT_FIFO_8TH_FULL=23,
    GPUPE_QUAD3_RD_LAT_FIFO_4TH_FULL=24,
    GPUPE_QUAD3_RD_LAT_FIFO_HALF_FULL=25,
    GPUPE_QUAD3_RD_LAT_FIFO_FULL=26,
    GPUPE_QUAD3_RD_LAT_FIFO_LT_16TH_FULL=27,
    GPUPE_READ_STARVED_QUAD0=28,
    GPUPE_READ_STARVED_QUAD1=29,
    GPUPE_READ_STARVED_QUAD2=30,
    GPUPE_READ_STARVED_QUAD3=31,
    GPUPE_READ_STARVED=32,
    GPUPE_READ_STALLED_QUAD0=33,
    GPUPE_READ_STALLED_QUAD1=34,
    GPUPE_READ_STALLED_QUAD2=35,
    GPUPE_READ_STALLED_QUAD3=36,
    GPUPE_READ_STALLED=37,
    GPUPE_VALID_READ_QUAD0=38,
    GPUPE_VALID_READ_QUAD1=39,
    GPUPE_VALID_READ_QUAD2=40,
    GPUPE_VALID_READ_QUAD3=41,
    GPUPE_TC_TP_STARVED_QUAD0=42,
    GPUPE_TC_TP_STARVED_QUAD1=43,
    GPUPE_TC_TP_STARVED_QUAD2=44,
    GPUPE_TC_TP_STARVED_QUAD3=45,
    GPUPE_TC_TP_STARVED=46
} GPUPERFEVENT_TCM;

typedef enum Abs_modifier {
    NO_ABS_MOD=0,
    ABS_MOD=1
} Abs_modifier;

typedef enum _FSINFOCLASS {
    FileFsVolumeInformation=1,
    FileFsLabelInformation=2,
    FileFsSizeInformation=3,
    FileFsDeviceInformation=4,
    FileFsAttributeInformation=5,
    FileFsControlInformation=6,
    FileFsFullSizeInformation=7,
    FileFsObjectIdInformation=8,
    FileFsMaximumInformation=9
} _FSINFOCLASS;

typedef struct _D3DGAMMARAMP _D3DGAMMARAMP, *P_D3DGAMMARAMP;

struct _D3DGAMMARAMP {
    ushort red[256];
    ushort green[256];
    ushort blue[256];
};

typedef struct _D3DLOCKED_RECT _D3DLOCKED_RECT, *P_D3DLOCKED_RECT;

struct _D3DLOCKED_RECT {
    int Pitch;
    void *pBits;
};

typedef struct D3DStateBlock D3DStateBlock, *PD3DStateBlock;

struct D3DStateBlock {
    undefined field0_0x0;
};

typedef enum _SECURITY_IMPERSONATION_LEVEL {
    SecurityAnonymous=0,
    SecurityIdentification=1,
    SecurityImpersonation=2,
    SecurityDelegation=3
} _SECURITY_IMPERSONATION_LEVEL;

typedef union RB_BC_AZ1_CRC RB_BC_AZ1_CRC, *PRB_BC_AZ1_CRC;


/* WARNING! conflicting data type names: /iw5mp_xenon.pdb/RB_BC_AZ1_CRC/<unnamed-type-bitfields> - /iw5mp_xenon.pdb/D1SCL_COEF_RAM_SELECT/<unnamed-type-bitfields> */

union RB_BC_AZ1_CRC {
    struct <unnamed-type-bitfields> bitfields;
    struct <unnamed-type-bitfields> bits;
    uint u32All;
    int i32All;
    float f32All;
};

typedef enum _FIRMWARE_REENTRY {
    HalHaltRoutine=0,
    HalRebootRoutine=1,
    HalKdRebootRoutine=2,
    HalFatalErrorRebootRoutine=3,
    HalResetSMCRoutine=4,
    HalPowerDownRoutine=5,
    HalRebootQuiesceRoutine=6,
    HalForceShutdownRoutine=7,
    HalMaximumRoutine=8
} _FIRMWARE_REENTRY;

typedef enum GPUPERFEVENT_CP {
    GPUPE_CP_COUNT=0,
    GPUPE_CP_RBIU_STALL=1,
    GPUPE_CP_RBIU_TAF=2,
    GPUPE_CP_PFP_STALL=3,
    GPUPE_CP_RESERVED0=4,
    GPUPE_CP_RESERVED1=5,
    GPUPE_CP_NRT_RCIU_STALL=6,
    GPUPE_CP_Reserved2=7,
    GPUPE_CP_NRT_MIU_STALL=8,
    GPUPE_CP_CSF_PFP_I1_FULL=9,
    GPUPE_CP_CSF_PFP_I2_FULL=10,
    GPUPE_CP_CSF_PFP_REQ_FULL=11,
    GPUPE_CP_RESERVED3=12,
    GPUPE_CP_RING_ROQ_FULL=13,
    GPUPE_CP_I1_ROQ_FULL=14,
    GPUPE_CP_I2_ROQ_FULL=15,
    GPUPE_CP_ST_ROQ_FULL=16,
    GPUPE_CP_RTST_ROQ_FULL=17,
    GPUPE_CP_MIU_TAG_MEM_FULL=18,
    GPUPE_CP_MIU_WRITECLEAN=19,
    GPUPE_CP_RESERVED4=20,
    GPUPE_CP_RESERVED5=21,
    GPUPE_CP_NRT_WRITE_STALL=22,
    GPUPE_CP_NRT_READ_STALL=23,
    GPUPE_CP_WC_FIFO_FULL=24,
    GPUPE_CP_VTX_DEALLOC_FIFO_FULL=25,
    GPUPE_CP_PIX_DEALLOC_FIFO_FULL=26,
    GPUPE_CP_VTX_EVENT_FIFO_FULL=27,
    GPUPE_CP_PIX_EVENT_FIFO_FULL=28,
    GPUPE_CP_CF_EVENT_FIFO_FULL=29,
    GPUPE_CP_ME_RB_STARVED=30,
    GPUPE_CP_ME_I1_STARVED=31,
    GPUPE_CP_ME_I2_STARVED=32,
    GPUPE_CP_ME_ST_STARVED=33,
    GPUPE_CP_RESERVED6=34,
    GPUPE_CP_RESERVED7=35,
    GPUPE_CP_RESERVED8=36,
    GPUPE_CP_RESERVED9=37,
    GPUPE_CP_RESERVED10=38,
    GPUPE_CP_RESERVED11=39,
    GPUPE_RCIU_RBBM_DWORD_SENT=40,
    GPUPE_ME_PARSER_BUSY_CLOCKS=41,
    GPUPE_ME_WAIT_CONTEXT_AVAIL=42,
    GPUPE_PFP_TYPE0_PACKET=43,
    GPUPE_PFP_TYPE3_PACKET=44,
    GPUPE_CSF_RB_WPTR_NEQ_RPTR=45,
    GPUPE_CSF_I1_SIZE_NEQ_ZERO=46,
    GPUPE_CSF_I2_SIZE_NEQ_ZERO=47,
    GPUPE_CSF_RB_I1_I2_FETCHING=48,
    GPUPE_CP_RESERVED12=49,
    GPUPE_CP_RESERVED13=50,
    GPUPE_CP_RESERVED14=51,
    GPUPE_CP_RESERVED15=52,
    GPUPE_CP_RESERVED16=53,
    GPUPE_CP_RESERVED17=54,
    GPUPE_CP_RESERVED18=55,
    GPUPE_CP_RESERVED19=56,
    GPUPE_CP_RESERVED20=57,
    GPUPE_CP_RESERVED21=58,
    GPUPE_CP_RESERVED22=59,
    GPUPE_CP_RESERVED23=60,
    GPUPE_CP_RESERVED24=61,
    GPUPE_CP_RESERVED25=62,
    GPUPE_CP_RESERVED26=63
} GPUPERFEVENT_CP;

typedef enum PerfEncode {
    cp_read_memory=0,
    cp_read_pglb=1,
    cp_write_memory=2,
    cp_write_snooped=3,
    cp_write_writeback=4,
    mh_cp_rtr=5,
    vgt_read_memory=6,
    vgt_read_pglb=7,
    mh_vgt_rtr=8,
    ioc_read=9,
    ioc_write=10,
    ioc_read_byte_count=11,
    ioc_write_byte_count=12,
    ioc_urgent=13,
    mh_ioc_rtr=14,
    tc0_read=15,
    mh_tc0_rtr=16,
    tc1_read=17,
    mh_tc1_rtr=18,
    vc0_read_memory=19,
    vc0_read_pglb=20,
    mh_vc0_rtr=21,
    vc1_read_memory=22,
    vc1_read_pglb=23,
    mh_vc1_rtr=24,
    bc0_cp_write=25,
    bc0_ex_write=26,
    mh_bc0_rtr=27,
    bc1_cp_write=28,
    bc1_ex_write=29,
    mh_bc1_rtr=30,
    dc_graphics_req=31,
    dc_overlay_req=32,
    dc_urgent=33,
    pglb_biu_req=34,
    biu_pgl_read_data=35,
    pgl_mhs_read_data=36,
    mh_mc0_read_reqs=37,
    mh_mc0_write_reqs=38,
    mh_mc1_read_reqs=39,
    mh_mc1_write_reqs=40,
    mc0_mh_read_data=41,
    mc1_mh_read_data=42,
    mh_cp_send=43,
    mh_vgt_send=44,
    mh_ioc_send=45,
    mh_tc0_send=46,
    mh_tc1_send=47,
    mh_vc0_send=48,
    mh_vc1_send=49,
    mh_dc_send=50,
    dc0_inflight_full=51,
    dc1_inflight_full=52,
    vc0_inflight_full=53,
    vc1_inflight_full=54,
    tc0_inflight_full=55,
    tc1_inflight_full=56,
    cp0_inflight_full=57,
    cp1_inflight_full=58,
    vgt0_inflight_full=59,
    vgt1_inflight_full=60,
    sb0_inflight_full=61,
    sb1_inflight_full=62,
    vcpgl_inflight_full=63,
    cppgl_inflight_full=64,
    vgtpgl_inflight_full=65,
    mc0_dc_q_full=66,
    mc0_vc_q_full=67,
    mc0_tc_q_full=68,
    mc0_cp_q_full=69,
    mc0_sb_q_full=70,
    mc0_vgt_q_full=71,
    mc0_bccp_q_full=72,
    mc0_bcex_q_full=73,
    mc1_dc_q_full=74,
    mc1_vc_q_full=75,
    mc1_tc_q_full=76,
    mc1_cp_q_full=77,
    mc1_sb_q_full=78,
    mc1_vgt_q_full=79,
    mc1_bccp_q_full=80,
    mc1_bcex_q_full=81,
    cp_pgl_cache_hit=82,
    vgt_pgl_cache_hit=83,
    vc_pgl_cache_hit=84,
    cp_pgl_full=85,
    vgt_pgl_full=86,
    vc0_pgl_full=87,
    vc1_pgl_full=88,
    mc0_wdb_full=89,
    mc0_tagbuf_full=90,
    mc1_wdb_full=91,
    mc1_tagbuf_full=92,
    pgl_tagbuf_full=93,
    cp_writenotify=94,
    bc_writenotify=95,
    ioc_sync=96,
    pgl_biu_latency=97,
    mh_mc0_latency=98,
    mh_mc0_dc_latency=99,
    mh_mc0_vc_latency=100,
    mh_mc0_tc_latency=101,
    mh_mc0_cp_latency=102,
    mh_mc0_sb_latency=103,
    mh_mc0_vgt_latency=104,
    mh_mc1_latency=105,
    mh_mc1_dc_latency=106,
    mh_mc1_vc_latency=107,
    mh_mc1_tc_latency=108,
    mh_mc1_cp_latency=109,
    mh_mc1_sb_latency=110,
    mh_mc1_vgt_latency=111,
    mh_pgl_latency=112,
    mh_pgl_cp_latency=113,
    mh_pgl_vc_latency=114,
    mh_pgl_vgt_latency=115,
    tc0_rdy_and_noroom=116,
    tc1_rdy_and_noroom=117,
    ioc_rdy_and_noroom=118
} PerfEncode;

typedef enum _PixGroupsType {
    PIXGROUP_GraphicsTiming=1,
    PIXGROUP_GraphicsCModel=2,
    PIXGROUP_UserTiming=4,
    PIXGROUP_UserOther=8,
    PIXGROUP_XAudioTiming=16,
    PIXGROUP_Resource=32
} _PixGroupsType;

typedef enum _D3DTEXTUREFILTERTYPE {
    D3DTEXF_POINT=0,
    D3DTEXF_LINEAR=1,
    D3DTEXF_NONE=2,
    D3DTEXF_ANISOTROPIC=4,
    D3DTEXF_FORCE_DWORD=255
} _D3DTEXTUREFILTERTYPE;

typedef enum DCPERFEVENT {
    GPUPE_DMIF_PER_DCREQ_EVENT=0,
    GPUPE_LOGIC1_EVENT=0,
    GPUPE_DMIF_PER_DCGRPH_REQ_EVENT=1,
    GPUPE_DMIF_PER_DCOVL_REQ_EVENT=2,
    GPUPE_DMIF_PER_DCREQ_SIZE_EVENT=3,
    GPUPE_DMIF_PER_DCGRPH_REQ_SIZE_EVENT=4,
    GPUPE_DMIF_PER_DCOVL_REQ_SIZE_EVENT=5,
    GPUPE_DMIF_PER_DCSURFACE_UPDATE_EVENT=6,
    GPUPE_DMIF_PER_DC_MH_REQ_EVENT=7,
    GPUPE_DMIF_PER_DC_MH_D1GRPH_REQ_EVENT=8,
    GPUPE_DMIF_PER_DC_MH_D1OVL_REQ_EVENT=9,
    GPUPE_DMIF_PER_DC_MH_REQ_SIZE_EVENT=10,
    GPUPE_DMIF_PER_DC_MH_D1GRPH_REQ_SIZE_EVENT=11,
    GPUPE_DMIF_PER_DC_MH_D1OVL_REQ_SIZE_EVENT=12,
    GPUPE_DMIF_PER_DC_MH_SURFACE_UPDATE_EVENT=13,
    GPUPE_DMIF_PER_MH_DC_RTR_EVENT=14,
    GPUPE_DMIF_PER_CMD_PROC_WAIT_RTR_STATE_EVENT=15,
    GPUPE_DMIF_PER_CMD_PROC_IDLE_STATE_EVENT=16,
    GPUPE_DMIF_PER_DC_MH_URGENT_EVENT=17,
    GPUPE_DMIF_PER_MH_DC_SEND_EVENT=18,
    GPUPE_DMIF_PER_MH_DC_SEND_D1GRPH_EVENT=19,
    GPUPE_DMIF_PER_MH_DC_SEND_D1OVL_EVENT=20,
    GPUPE_DMIF_PER_DC_MH_RTR_EVENT=21,
    GPUPE_DMIF_PER_DMIF_BUSY_EVENT=22,
    GPUPE_DMIF_PER_DMIF_BUSY_MH_DC_SEND_EVENT=23,
    GPUPE_DMIF_PER_DMIF_BUSY_DC_MH_RTR_EVENT=24,
    GPUPE_DMIF_PER_DMIF_DCSEND_EVENT=25,
    GPUPE_DMIF_PER_DMIF_DCNOT_RTS_EVENT=26,
    GPUPE_DMIF_PER_DCDMIF_NOT_RTR_EVENT=27,
    GPUPE_DCP_PER_LUT_HOST_RW_EVENT=28,
    GPUPE_DCP_PER_LUT_RW_BY_HOST_EVENT=29,
    GPUPE_DCP_PER_RTR_LOW_BY_LUT_HOST_RW_EVENT=30,
    GPUPE_DCCG_PER_SCLK_R_RBBMIF_CLOCK_ON_EVENT=31,
    GPUPE_DCCG_PER_SCLK_R_DISCLOCK_ON_EVENT=32,
    GPUPE_DCCG_PER_SCLK_G_SCL_CLOCK_ON_EVENT=33,
    GPUPE_DCCG_PER_SCLK_G_DCCLOCK_ON_EVENT=34,
    GPUPE_DCCG_PER_PCLK_CRTC_CLOCK_ON_EVENT=35,
    GPUPE_DCCG_PER_DVOACLK_C_CLOCK_ON_EVENT=36,
    GPUPE_DCCG_PER_DVOACLK_D_CLOCK_ON_EVENT=37,
    GPUPE_CRTC1_PER_START_LINE_EVENT=38,
    GPUPE_CRTC1_PER_HSYNC_A_EVENT=39,
    GPUPE_CRTC1_PER_VSYNC_A_EVENT=40,
    GPUPE_CRTC1_PER_H_DATA_ACTIVE_EVENT=41,
    GPUPE_CRTC1_PER_V_DATA_ACTIVE_EVENT=42,
    GPUPE_CRTC1_PER_DATA_ACTIVE_EVENT=43,
    GPUPE_CRTC1_PER_H_BLANK_EVENT=44,
    GPUPE_CRTC1_PER_V_BLANK_EVENT=45,
    GPUPE_CRTC1_PER_BLANK_EVENT=46,
    GPUPE_CRTC1_PER_INTERLACE_SELECT_EVENT=47,
    GPUPE_CRTC1_PER_STEREO_SELECT_EVENT=48,
    GPUPE_SCL1_PER_HOST_CONFLICT_EVENT=49,
    GPUPE_SCL1_PER_ADVANCE_FILTER_POS_EVENT=50,
    GPUPE_SCL1_PER_TAINC_EVENT=51,
    GPUPE_SCL1_PER_REQUEST_EOL_EVENT=52,
    GPUPE_SCL1_PER_V_COEF_PRELOAD_EVENT=53,
    GPUPE_SCL1_PER_EOL_EVENT=54,
    GPUPE_SCL1_PER_SOF_EVENT=55,
    GPUPE_LB_PER_DISP1_RESET_REQ_EVENT=56,
    GPUPE_LB_PER_DISP1_REQ_SEND_EVENT=57,
    GPUPE_LB_PER_DISP1_REQ_UNDERFLOW_EVENT=58,
    GPUPE_LB_PER_DISP1_DATA_UNDERFLOW_EVENT=59,
    GPUPE_LB_PER_DISP1_URGENT_EVENT=60,
    GPUPE_LB_PER_DISP1_VBLANK_STAT_EVENT=61,
    GPUPE_LB_PER_DISP1_VLINE_STAT_EVENT=62,
    GPUPE_DOUT_PER_SCL_DISP1_MODE_CHANGE_INTERRUPT_EVENT=63,
    GPUPE_DOUT_PER_LB_D1_VLINE_INTERRUPT_EVENT=64,
    GPUPE_DOUT_PER_LB_D1_VBLANK_INTERRUPT_EVENT=65,
    GPUPE_DOUT_PER_DISTIMER_INTERRUPT_EVENT=66
} DCPERFEVENT;

typedef enum PIX_REQUEST_MODE {
    RequestMode_None=0,
    RequestMode_TimingCapture=1,
    RequestMode_ProfilerCapture=2,
    RequestMode_CpuTraceCapture_FullFrame=3,
    RequestMode_CpuTraceCapture_MarkerBased=4,
    RequestMode_GpuCapture=5,
    RequestMode_SetOptions=6,
    RequestMode_GetCapture=7,
    RequestMode_EnumData=8,
    RequestMode_GetIndexerState=9,
    RequestMode_Pause=10,
    RequestMode_Unpause=11,
    RequestMode_MoviePlayback=12,
    RequestMode_Test=13,
    RequestMode_GetMovieData=14,
    RequestMode_ResourceCapture=15
} PIX_REQUEST_MODE;

typedef enum PIXMeta {
    PIXMeta_SetResolveDestTexture=0,
    PIXMeta_ClearResolveDestTexture=1,
    PIXMeta_Clear=2,
    PIXMeta_DrawPrimitiveUP=3,
    PIXMeta_DrawIndexedPrimitiveUP=4,
    PIXMeta_BeginVertices=5,
    PIXMeta_BeginIndexedVertices=6,
    PIXMeta_GameCertEdramClear=7,
    PIXMeta_Unused4=8,
    PIXMeta_OobComment=9,
    PIXMeta_OobMetaData=10,
    PIXMeta_SecondaryRingBuffer=11,
    PIXMeta_BeginIndirectBuffer=12,
    PIXMeta_EndIndirectBuffer=13,
    PIXMeta_PatchVertexShaderToMatchPixelShader=14,
    PIXMeta_PatchVertexShaderToMatchVertexDeclarationFast=15,
    PIXMeta_PatchVertexShaderToMatchVertexDeclarationSlow=16,
    PIXMeta_VertexShaderDebuggerHint=17,
    PIXMeta_PixelShaderDebuggerHint=18,
    PIXMeta_VertexShaderDebuggerHintShort=19,
    PIXMeta_PixelShaderDebuggerHintShort=20,
    PIXMeta_UserBeginEvent=21,
    PIXMeta_UserEndEvent=22,
    PIXMeta_SetRenderTarget=23,
    PIXMeta_PatchVertexShaderToMatchPixelShader_Undo=24,
    PIXMeta_SetShaderGPRAllocationBegin=25,
    PIXMeta_SetShaderGPRAllocationEnd=26,
    PIXMeta_BeginTiling=27,
    PIXMeta_EndTiling=28,
    PIXMeta_UserMarkerEvent=29,
    PIXMeta_BeginZPass=30,
    PIXMeta_EndZPass=31,
    PIXMeta_XpsBegin=32,
    PIXMeta_XpsEnd=33,
    PIXMeta_BeginRunCommandBuffer=34,
    PIXMeta_EndRunCommandBuffer=35,
    PIXMeta_FXLiteDependency=36,
    PIXMeta_IndirectLoadShader=37,
    PIXMeta_IndirectLoadAluConstant=38,
    PIXMeta_IndirectReserved=39,
    PIXMeta_BeginClear=40,
    PIXMeta_EndClear=41,
    PIXMeta_TileInfo=42,
    PIXMeta_DisableTiling=43,
    PIXMeta_ReenableTiling=44,
    PIXMeta_BeginExport_Begin=45,
    PIXMeta_BeginExport_End=46,
    PIXMeta_EndExport_Begin=47,
    PIXMeta_EndExport_End=48,
    PIXMeta_InsertBlockOnAsyncResourcesBegin=49,
    PIXMeta_InsertBlockOnAsyncResourcesEnd=50,
    PIXMeta_IgnoreTexture=51,
    PIXMeta_StopIgnoringTexture=52,
    PIXMeta_IgnoreMemoryRange=53,
    PIXMeta_StopIgnoringMemoryRange=54,
    PIXMeta_SetTextureName=55,
    PIXMeta_ReportNewTexture=56,
    PIXMeta_ReportDeletedTexture=57,
    PIXMeta_ReportMovedMemoryRange=58,
    PIXMeta_ReportFreedMemoryRange=59,
    PIXMeta_EndResourceCapture=60,
    PIXMeta_SynchronizeResourceCaptureThreads=61,
    PIXMeta_SwapTexture=62,
    PIXMeta_BeginCodecVideoEncode=63,
    PIXMeta_EndCodecVideoEncode=64,
    PIXMeta_BeginVdSwap=65,
    PIXMeta_EndVdSwap=66,
    PIXMeta_BeginPixDecodeVideoFrame=67,
    PIXMeta_EndPixDecodeVideoFrame=68,
    PIXMeta_BeginNuiProcess=69,
    PIXMeta_EndNuiProcess=70,
    PIXMeta_InternalResolve=71,
    PIXMeta_InsertAsyncCommandBufferCall=72,
    PIXMeta_EndInsertAsyncCommandBufferCall=73,
    PIXMeta_BeginIndirectBufferPFD=74,
    PIXMeta_EndIndirectBufferPFD=75,
    PIXMeta_NuiEvent=76
} PIXMeta;

typedef struct _D3DBLENDSTATE _D3DBLENDSTATE, *P_D3DBLENDSTATE;

struct _D3DBLENDSTATE {
    ulong BlendOp:3;
    ulong SrcBlend:5;
    ulong DestBlend:8;
    ulong BlendOpAlpha:3;
    ulong SrcBlendAlpha:5;
    ulong DestBlendAlpha:8;
};

typedef enum _KTHREAD_STATE {
    Initialized=0,
    Ready=1,
    Running=2,
    Standby=3,
    Terminated=4,
    Waiting=5,
    DeferredReady=6
} _KTHREAD_STATE;

typedef enum _D3DXWELDEPSILONSFLAGS {
    D3DXWELDEPSILONS_WELDALL=1,
    D3DXWELDEPSILONS_WELDPARTIALMATCHES=2,
    D3DXWELDEPSILONS_DONOTREMOVEVERTICES=4,
    D3DXWELDEPSILONS_DONOTSPLIT=8,
    D3DXWELDEPSILONS_VALIDBITS=15
} _D3DXWELDEPSILONSFLAGS;

typedef enum _D3DSTENCILOP {
    D3DSTENCILOP_KEEP=0,
    D3DSTENCILOP_ZERO=1,
    D3DSTENCILOP_REPLACE=2,
    D3DSTENCILOP_INCRSAT=3,
    D3DSTENCILOP_DECRSAT=4,
    D3DSTENCILOP_INVERT=5,
    D3DSTENCILOP_INCR=6,
    D3DSTENCILOP_DECR=7,
    D3DSTENCILOP_FORCE_DWORD=255
} _D3DSTENCILOP;

typedef enum <unnamed-tag_0000113F> {
    SPR_LPCR=62
} <unnamed-tag_0000113F>;

typedef struct D3DXCOLOR D3DXCOLOR, *PD3DXCOLOR;

struct D3DXCOLOR {
    float r;
    float g;
    float b;
    float a;
};

typedef union FSB_TX_PMC2_OR FSB_TX_PMC2_OR, *PFSB_TX_PMC2_OR;


/* WARNING! conflicting data type names: /iw5mp_xenon.pdb/FSB_TX_PMC2_OR/<unnamed-type-bitfields> - /iw5mp_xenon.pdb/D1SCL_COEF_RAM_SELECT/<unnamed-type-bitfields> */

union FSB_TX_PMC2_OR {
    struct <unnamed-type-bitfields> bitfields;
    struct <unnamed-type-bitfields> bits;
    uint u32All;
    int i32All;
    float f32All;
};

typedef enum GPUINDEXSELECT {
    GPUINDEXSELECT_DMA=0,
    GPUINDEXSELECT_IMMEDIATE=1,
    GPUINDEXSELECT_AUTO=2
} GPUINDEXSELECT;

typedef enum _D3DSHADER_PARAM_REGISTER_TYPE {
    D3DSPR_TEMP=0,
    D3DSPR_INPUT=1,
    D3DSPR_CONST=2,
    D3DSPR_ADDR=3,
    D3DSPR_TEXTURE=3,
    D3DSPR_RASTOUT=4,
    D3DSPR_ATTROUT=5,
    D3DSPR_OUTPUT=6,
    D3DSPR_TEXCRDOUT=6,
    D3DSPR_CONSTINT=7,
    D3DSPR_COLOROUT=8,
    D3DSPR_DEPTHOUT=9,
    D3DSPR_SAMPLER=10,
    D3DSPR_CONST2=11,
    D3DSPR_CONST3=12,
    D3DSPR_CONST4=13,
    D3DSPR_CONSTBOOL=14,
    D3DSPR_LOOP=15,
    D3DSPR_TEMPFLOAT16=16,
    D3DSPR_MISCTYPE=17,
    D3DSPR_LABEL=18,
    D3DSPR_PREDICATE=19,
    D3DSPR_FORCE_DWORD=255
} _D3DSHADER_PARAM_REGISTER_TYPE;

typedef struct _EX_THREAD_REGISTRATION _EX_THREAD_REGISTRATION, *P_EX_THREAD_REGISTRATION;

typedef struct _KTHREAD _KTHREAD, *P_KTHREAD;

typedef struct _DISPATCHER_HEADER _DISPATCHER_HEADER, *P_DISPATCHER_HEADER;

typedef struct _KTIMER _KTIMER, *P_KTIMER;

typedef struct _KWAIT_BLOCK _KWAIT_BLOCK, *P_KWAIT_BLOCK;

typedef struct _KPROCESS _KPROCESS, *P_KPROCESS;

typedef union _KTHREAD_u_156 _KTHREAD_u_156, *P_KTHREAD_u_156;

typedef struct _KPRCB _KPRCB, *P_KPRCB;

typedef struct _KAPC _KAPC, *P_KAPC;

typedef struct _KSEMAPHORE _KSEMAPHORE, *P_KSEMAPHORE;

typedef struct _KQUEUE _KQUEUE, *P_KQUEUE;

typedef struct _KTHREAD_u_156_s_0 _KTHREAD_u_156_s_0, *P_KTHREAD_u_156_s_0;

struct _EX_THREAD_REGISTRATION {
    void (*NotificationRoutine)(struct _EX_THREAD_REGISTRATION *, struct _KTHREAD *, ulong);
    long Priority;
    struct _LIST_ENTRY ListEntry;
};

struct _KWAIT_BLOCK {
    struct _LIST_ENTRY WaitListEntry;
    struct _KTHREAD *Thread;
    void *Object;
    struct _KWAIT_BLOCK *NextWaitBlock;
    ushort WaitKey;
    ushort WaitType;
};

struct _KTHREAD_u_156_s_0 {
    ushort MsrEnableMaskHigh;
    ushort MsrEnableMaskLow;
};

union _KTHREAD_u_156 {
    struct _KTHREAD_u_156_s_0 _s_0;
    ulong MsrEnableMask;
};

struct _DISPATCHER_HEADER {
    uchar Type;
    uchar Absolute;
    uchar ProcessType;
    uchar Inserted;
    long SignalState;
    struct _LIST_ENTRY WaitListHead;
};

struct _KPRCB {
    struct _KTHREAD *CurrentThread;
    struct _KTHREAD *NextThread;
    struct _KTHREAD *IdleThread;
    uchar Number;
    ulong SetMember;
    ulong DpcTime;
    ulong InterruptTime;
    ulong InterruptCount;
    ulong IpiFrozen;
    void *CurrentPacket[3];
    ulong TargetSet;
    void (*WorkerRoutine)(void *, void *, void *, void *);
    struct _KPRCB *SignalDone;
    ulong RequestSummary;
    ulong DpcInterruptRequested;
    ulong DpcLock;
    struct _LIST_ENTRY DpcListHead;
    ulong DpcRoutineActive;
    ulong ReadyListLock;
    ulong IdleThreadActive;
    struct _SINGLE_LIST_ENTRY DeferredReadyListHead;
    ulong ReadySummary;
    ulong ReadySummaryMask;
    struct _LIST_ENTRY DispatcherReadyListHead[32];
    struct _KDPC ThreadReaperDpc;
    struct _LIST_ENTRY ThreadReaperListHead;
    struct _KDPC SwitchProcessorThreadDpc;
};

struct _KAPC {
    short Type;
    uchar ApcMode;
    uchar Inserted;
    struct _KTHREAD *Thread;
    struct _LIST_ENTRY ApcListEntry;
    void (*KernelRoutine)(struct _KAPC *, void (**)(void *, void *, void *), void **, void **, void **);
    void (*RundownRoutine)(struct _KAPC *);
    void (*NormalRoutine)(void *, void *, void *);
    void *NormalContext;
    void *SystemArgument1;
    void *SystemArgument2;
};

struct _KTIMER {
    struct _DISPATCHER_HEADER Header;
    union _ULARGE_INTEGER DueTime;
    struct _LIST_ENTRY TimerListEntry;
    struct _KDPC *Dpc;
    long Period;
};

struct _KSEMAPHORE {
    struct _DISPATCHER_HEADER Header;
    long Limit;
};

struct _KTHREAD {
    struct _DISPATCHER_HEADER Header;
    struct _LIST_ENTRY MutantListHead;
    struct _KTIMER Timer;
    struct _KWAIT_BLOCK TimerWaitBlock;
    ulong KernelTime;
    void *StackBase;
    void *StackLimit;
    void *KernelStack;
    void *TlsData;
    uchar State;
    uchar Alerted[2];
    uchar Alertable;
    uchar Priority;
    uchar FpuExceptionEnable;
    uchar CreateProcessType;
    uchar CurrentProcessType;
    struct _LIST_ENTRY ApcListHead[2];
    struct _KPROCESS *Process;
    uchar KernelApcInProgress;
    uchar KernelApcPending;
    uchar UserApcPending;
    uchar ApcQueueable;
    ulong ApcQueueLock;
    ulong ContextSwitches;
    struct _LIST_ENTRY ReadyListEntry;
    union _KTHREAD_u_156 field25_0x9c;
    long WaitStatus;
    uchar WaitIrql;
    uchar WaitMode;
    uchar WaitNext;
    uchar WaitReason;
    struct _KWAIT_BLOCK *WaitBlockList;
    ulong Padding1;
    ulong KernelApcDisable;
    long Quantum;
    char Saturation;
    uchar BasePriority;
    uchar PriorityDecrement;
    uchar DisableBoost;
    char SuspendCount;
    uchar Preempted;
    uchar HasTerminated;
    uchar CurrentProcessor;
    struct _KPRCB *CurrentPrcb;
    struct _KPRCB *AffinityPrcb;
    uchar IdlePriorityClass;
    uchar NormalPriorityClass;
    uchar TimeCriticalPriorityClass;
    uchar HasAsyncTerminated;
    ulong ActiveTimerListLock;
    void *StackAllocatedBase;
    struct _KAPC SuspendApc;
    struct _KSEMAPHORE SuspendSemaphore;
    struct _LIST_ENTRY ThreadListEntry;
    struct _KQUEUE *Queue;
    struct _LIST_ENTRY QueueListEntry;
    void (*UserModeDispatcher)(struct _EXCEPTION_RECORD *, struct _KTRAP_FRAME *);
    struct _KTRAP_FRAME *UserModeTrapFrame;
    union _LARGE_INTEGER CreateTime;
    union _LARGE_INTEGER ExitTime;
    long ExitStatus;
    struct _LIST_ENTRY ActiveTimerListHead;
    void *ThreadId;
    void (*StartAddress)(void *);
    struct _LIST_ENTRY IrpList;
    void *DebugMonitorData;
    ulong LastWin32ErrorCode;
    void *Win32CurrentFiber;
    ulong Padding2;
    ulong CreateOptions;
    float Vscr[4];
    float Vr[128][4];
    double Fpscr;
    double Fpr[32];
    struct _KAPC TerminateApc;
};

struct _KQUEUE {
    struct _DISPATCHER_HEADER Header;
    struct _LIST_ENTRY EntryListHead;
    ulong CurrentCount;
    ulong MaximumCount;
    struct _LIST_ENTRY ThreadListHead;
};

struct _KPROCESS {
    ulong ThreadListLock;
    struct _LIST_ENTRY ThreadListHead;
    long ThreadQuantum;
    ulong DirectoryTableBase;
    ulong ThreadCount;
    uchar IdlePriorityClass;
    uchar NormalPriorityClass;
    uchar TimeCriticalPriorityClass;
    uchar DisableQuantum;
    ulong DefaultKernelStackSize;
    void *TlsStaticDataImage;
    ulong SizeOfTlsStaticData;
    ulong SizeOfTlsStaticDataImage;
    ushort SizeOfTlsSlots;
    uchar Terminating;
    uchar ProcessType;
    ulong TlsSlotBitmap[8];
    ulong FileObjectListLock;
    struct _LIST_ENTRY FileObjectListHead;
    void *Win32DefaultHeapHandle;
};

typedef enum _EXCEPTION_DISPOSITION {
    ExceptionContinueExecution=0,
    ExceptionContinueSearch=1,
    ExceptionNestedException=2,
    ExceptionCollidedUnwind=3
} _EXCEPTION_DISPOSITION;

typedef enum GPUSAMPLECONTROL {
    GPUSAMPLECONTROL_CENTROIDS_ONLY=0,
    GPUSAMPLECONTROL_CENTERS_ONLY=1,
    GPUSAMPLECONTROL_CENTROIDS_AND_CENTERS=2
} GPUSAMPLECONTROL;

typedef enum _D3DTEXTURETRANSFORMFLAGS {
    D3DTTFF_DISABLE=0,
    D3DTTFF_PROJECTED=0,
    D3DTTFF_COUNT1=1,
    D3DTTFF_COUNT2=2,
    D3DTTFF_COUNT3=3,
    D3DTTFF_COUNT4=4,
    D3DTTFF_FORCE_DWORD=255
} _D3DTEXTURETRANSFORMFLAGS;

typedef enum _XPLAYERLIST_BUTTON_TYPE {
    XPLAYERLIST_BUTTON_TYPE_TITLECUSTOM=0,
    XPLAYERLIST_BUTTON_TYPE_PLAYERREVIEW=1,
    XPLAYERLIST_BUTTON_TYPE_GAMEINVITE=2,
    XPLAYERLIST_BUTTON_TYPE_MESSAGE=3,
    XPLAYERLIST_BUTTON_TYPE_FRIENDREQUEST=4
} _XPLAYERLIST_BUTTON_TYPE;

typedef struct _D3DSWAPDATA _D3DSWAPDATA, *P_D3DSWAPDATA;

struct _D3DSWAPDATA {
    ulong Flags;
    ulong Swap;
    ulong LastVBlank;
    ulong LastSwapVBlank;
    ulong PercentScanned;
    ulong SwapVBlank;
};

typedef enum GPUALUSCALAROP {
    GPUALUSCALAROP_ADD=0,
    GPUALUSCALAROP_ADDPREV=1,
    GPUALUSCALAROP_MUL=2,
    GPUALUSCALAROP_MULPREV=3,
    GPUALUSCALAROP_MULPREV2=4,
    GPUALUSCALAROP_MAX=5,
    GPUALUSCALAROP_MIN=6,
    GPUALUSCALAROP_SEQ=7,
    GPUALUSCALAROP_SGT=8,
    GPUALUSCALAROP_SGE=9,
    GPUALUSCALAROP_SNE=10,
    GPUALUSCALAROP_FRC=11,
    GPUALUSCALAROP_TRUNC=12,
    GPUALUSCALAROP_FLOOR=13,
    GPUALUSCALAROP_EXP=14,
    GPUALUSCALAROP_LOGC=15,
    GPUALUSCALAROP_LOG=16,
    GPUALUSCALAROP_RCPC=17,
    GPUALUSCALAROP_RCPF=18,
    GPUALUSCALAROP_RCP=19,
    GPUALUSCALAROP_RSQC=20,
    GPUALUSCALAROP_RSQF=21,
    GPUALUSCALAROP_RSQ=22,
    GPUALUSCALAROP_MAXA=23,
    GPUALUSCALAROP_MAXAF=24,
    GPUALUSCALAROP_SUB=25,
    GPUALUSCALAROP_SUBPREV=26,
    GPUALUSCALAROP_SETPEQ=27,
    GPUALUSCALAROP_SETPNE=28,
    GPUALUSCALAROP_SETPGT=29,
    GPUALUSCALAROP_SETPGE=30,
    GPUALUSCALAROP_SETPINV=31,
    GPUALUSCALAROP_SETPPOP=32,
    GPUALUSCALAROP_SETPCLR=33,
    GPUALUSCALAROP_SETPRSTR=34,
    GPUALUSCALAROP_KILLEQ=35,
    GPUALUSCALAROP_KILLGT=36,
    GPUALUSCALAROP_KILLGE=37,
    GPUALUSCALAROP_KILLNE=38,
    GPUALUSCALAROP_KILLONE=39,
    GPUALUSCALAROP_SQRT=40,
    GPUALUSCALAROP_MULC0=42,
    GPUALUSCALAROP_MULC1=43,
    GPUALUSCALAROP_ADDC0=44,
    GPUALUSCALAROP_ADDC1=45,
    GPUALUSCALAROP_SUBC0=46,
    GPUALUSCALAROP_SUBC1=47,
    GPUALUSCALAROP_SIN=48,
    GPUALUSCALAROP_COS=49,
    GPUALUSCALAROP_RETAINPREV=50
} GPUALUSCALAROP;

typedef struct _IO_STATUS_BLOCK _IO_STATUS_BLOCK, *P_IO_STATUS_BLOCK;

typedef union _IO_STATUS_BLOCK_u_0 _IO_STATUS_BLOCK_u_0, *P_IO_STATUS_BLOCK_u_0;

union _IO_STATUS_BLOCK_u_0 {
    long Status;
    void *Pointer;
};

struct _IO_STATUS_BLOCK {
    union _IO_STATUS_BLOCK_u_0 field0_0x0;
    ulong Information;
};

typedef union RTS_J_W_REF RTS_J_W_REF, *PRTS_J_W_REF;


/* WARNING! conflicting data type names: /iw5mp_xenon.pdb/RTS_J_W_REF/<unnamed-type-bitfields> - /iw5mp_xenon.pdb/D1SCL_COEF_RAM_SELECT/<unnamed-type-bitfields> */

union RTS_J_W_REF {
    struct <unnamed-type-bitfields> bitfields;
    struct <unnamed-type-bitfields> bits;
    uint u32All;
    int i32All;
    float f32All;
};

typedef enum GPUPERFEVENT_BC {
    GPUPE_BC_CNTX0_BUSY=0,
    GPUPE_BC_CNTX17_BUSY=1,
    GPUPE_BC_RQ_STALLED=2,
    GPUPE_BC_AZ_STALLED=3,
    GPUPE_BC_MH_CPY_STALLED=4,
    GPUPE_BC_MH_EXP_STALLED=5,
    GPUPE_BC_SC_STARVED=6,
    GPUPE_BC_SX_STARVED=7,
    GPUPE_BC_ACC_COUNT=8,
    GPUPE_BC_DRAW_COUNT=9,
    GPUPE_BC_ACC2_COUNT=10,
    GPUPE_BC_DRAW2_COUNT=11,
    GPUPE_BC_SETZ_COUNT=12,
    GPUPE_BC_READ_COUNT=13,
    GPUPE_BC_READ_ACC_COUNT=14,
    GPUPE_BC_STATE_COUNT=15,
    GPUPE_BC_STATE2_COUNT=16,
    GPUPE_BC_COPY_WRITE_COUNT=17,
    GPUPE_BC_EXPORT_COUNT=18
} GPUPERFEVENT_BC;

typedef enum RecordType {
    Record_End=0,
    Record_Version=1,
    Record_RestoreRanges=2,
    Record_RangeError=3,
    Record_FrontBuffer=4,
    Record_InitialMemory=5,
    Record_CpuModifiedMemory=6,
    Record_CommandBuffer=7,
    Record_XpsMemory=8,
    Record_FenceAddress=9,
    Record_MicrocodeVersion=10,
    Record_MAX=11
} RecordType;

typedef struct D3DCubeTexture D3DCubeTexture, *PD3DCubeTexture;

struct D3DCubeTexture {
    undefined field0_0x0;
    undefined field1_0x1;
    undefined field2_0x2;
    undefined field3_0x3;
    undefined field4_0x4;
    undefined field5_0x5;
    undefined field6_0x6;
    undefined field7_0x7;
    undefined field8_0x8;
    undefined field9_0x9;
    undefined field10_0xa;
    undefined field11_0xb;
    undefined field12_0xc;
    undefined field13_0xd;
    undefined field14_0xe;
    undefined field15_0xf;
    undefined field16_0x10;
    undefined field17_0x11;
    undefined field18_0x12;
    undefined field19_0x13;
    undefined field20_0x14;
    undefined field21_0x15;
    undefined field22_0x16;
    undefined field23_0x17;
    undefined field24_0x18;
    undefined field25_0x19;
    undefined field26_0x1a;
    undefined field27_0x1b;
    undefined field28_0x1c;
    undefined field29_0x1d;
    undefined field30_0x1e;
    undefined field31_0x1f;
    undefined field32_0x20;
    undefined field33_0x21;
    undefined field34_0x22;
    undefined field35_0x23;
    undefined field36_0x24;
    undefined field37_0x25;
    undefined field38_0x26;
    undefined field39_0x27;
    undefined field40_0x28;
    undefined field41_0x29;
    undefined field42_0x2a;
    undefined field43_0x2b;
    undefined field44_0x2c;
    undefined field45_0x2d;
    undefined field46_0x2e;
    undefined field47_0x2f;
    undefined field48_0x30;
    undefined field49_0x31;
    undefined field50_0x32;
    undefined field51_0x33;
};

typedef struct D3DIndexBuffer D3DIndexBuffer, *PD3DIndexBuffer;

struct D3DIndexBuffer {
    int _padding_;
    int _padding_;
    int _padding_;
    int _padding_;
    int _padding_;
    int _padding_;
    ulong Address;
    ulong Size;
};

typedef struct _D3DSURFACE_DESC _D3DSURFACE_DESC, *P_D3DSURFACE_DESC;

struct _D3DSURFACE_DESC {
    enum _D3DFORMAT Format;
    short _padding_;
    enum _D3DRESOURCETYPE Type;
    ulong Usage;
    ulong Pool;
    enum _D3DMULTISAMPLE_TYPE MultiSampleType;
    ulong MultiSampleQuality;
    uint Width;
    uint Height;
};

typedef struct Direct3D Direct3D, *PDirect3D;

struct Direct3D {
    undefined field0_0x0;
};

typedef enum VGT_DI_SOURCE_SELECT {
    DI_SRC_SEL_DMA=0,
    DI_SRC_SEL_IMMEDIATE=1,
    DI_SRC_SEL_AUTO_INDEX=2,
    DI_SRC_SEL_RESERVED=3
} VGT_DI_SOURCE_SELECT;

typedef enum <unnamed-tag_00001181> {
    SPR_IRMR1=212
} <unnamed-tag_00001181>;

typedef enum _KUSER_MODE_TYPE {
    UserModeTitle=0,
    UserModeSystem=1
} _KUSER_MODE_TYPE;

typedef struct _D3DVERTEXBUFFER_DESC _D3DVERTEXBUFFER_DESC, *P_D3DVERTEXBUFFER_DESC;

struct _D3DVERTEXBUFFER_DESC {
    enum _D3DFORMAT Format;
    short _padding_;
    enum _D3DRESOURCETYPE Type;
    ulong Usage;
    ulong Pool;
    uint Size;
    ulong FVF;
};

typedef enum GPUMINMAGFILTER {
    GPUMINMAGFILTER_POINT=0,
    GPUMINMAGFILTER_LINEAR=1,
    GPUMINMAGFILTER_KEEP=3
} GPUMINMAGFILTER;

typedef enum Sample_Cntl {
    CENTROIDS_ONLY=0,
    CENTERS_ONLY=1,
    CENTROIDS_AND_CENTERS=2,
    UNDEF=3
} Sample_Cntl;

typedef enum _XALLOC_ALLOCATOR_IDS {
    eXALLOCAllocatorId_GameMin=0,
    eXALLOCAllocatorId_GameMax=127,
    eXALLOCAllocatorId_D3D=128,
    eXALLOCAllocatorId_MsReservedMin=128,
    eXALLOCAllocatorId_D3DX=129,
    eXALLOCAllocatorId_XAUDIO=130,
    eXALLOCAllocatorId_XAPI=131,
    eXALLOCAllocatorId_XACT=132,
    eXALLOCAllocatorId_XBOXKERNEL=133,
    eXALLOCAllocatorId_XBDM=134,
    eXALLOCAllocatorId_XGRAPHICS=135,
    eXALLOCAllocatorId_XONLINE=136,
    eXALLOCAllocatorId_XVOICE=137,
    eXALLOCAllocatorId_XHV=138,
    eXALLOCAllocatorId_USB=139,
    eXALLOCAllocatorId_XMV=140,
    eXALLOCAllocatorId_SHADERCOMPILER=141,
    eXALLOCAllocatorId_XUI=142,
    eXALLOCAllocatorId_XASYNC=143,
    eXALLOCAllocatorId_XCAM=144,
    eXALLOCAllocatorId_XVIS=145,
    eXALLOCAllocatorId_XIME=146,
    eXALLOCAllocatorId_XFILECACHE=147,
    eXALLOCAllocatorId_XRN=148,
    eXALLOCAllocatorID_XMCORE=149,
    eXALLOCAllocatorID_XMASSIVE=150,
    eXALLOCAllocatorId_XAUDIO2=151,
    eXALLOCAllocatorId_XAVATAR=152,
    eXALLOCAllocatorId_XLSP=153,
    eXALLOCAllocatorId_D3DAlloc=154,
    eXALLOCAllocatorID_NUISPEECH=155,
    eXALLOCAllocatorId_NuiApi=156,
    eXALLOCAllocatorId_NuiIdentity=157,
    eXALLOCAllocatorId_XTweak=158,
    eXALLOCAllocatorId_XAMCOMMON=159,
    eXALLOCAllocatorId_NUIUI=160,
    eXALLOCAllocatorId_LUA=161,
    eXALLOCAllocatorId_MsReservedMax=191,
    eXALLOCAllocatorId_MiddlewareReservedMin=192,
    eXALLOCAllocatorId_MiddlewareReservedMax=255
} _XALLOC_ALLOCATOR_IDS;

typedef struct D3DResource D3DResource, *PD3DResource;

struct D3DResource {
    ulong Common;
    ulong ReferenceCount;
    ulong Fence;
    ulong ReadFence;
    ulong Identifier;
    ulong BaseFlush;
};

typedef enum _D3DBLOCKTYPE {
    D3DBLOCKTYPE_NONE=0,
    D3DBLOCKTYPE_PRIMARY_OVERRUN=1,
    D3DBLOCKTYPE_SECONDARY_OVERRUN=2,
    D3DBLOCKTYPE_SWAP_THROTTLE=3,
    D3DBLOCKTYPE_BLOCK_UNTIL_IDLE=4,
    D3DBLOCKTYPE_BLOCK_UNTIL_NOT_BUSY=5,
    D3DBLOCKTYPE_BLOCK_ON_FENCE=6,
    D3DBLOCKTYPE_VERTEX_SHADER_RELEASE=7,
    D3DBLOCKTYPE_PIXEL_SHADER_RELEASE=8,
    D3DBLOCKTYPE_VERTEX_BUFFER_RELEASE=9,
    D3DBLOCKTYPE_VERTEX_BUFFER_LOCK=10,
    D3DBLOCKTYPE_INDEX_BUFFER_RELEASE=11,
    D3DBLOCKTYPE_INDEX_BUFFER_LOCK=12,
    D3DBLOCKTYPE_TEXTURE_RELEASE=13,
    D3DBLOCKTYPE_TEXTURE_LOCK=14,
    D3DBLOCKTYPE_COMMAND_BUFFER_RELEASE=15,
    D3DBLOCKTYPE_COMMAND_BUFFER_LOCK=16,
    D3DBLOCKTYPE_CONSTANT_BUFFER_RELEASE=17,
    D3DBLOCKTYPE_CONSTANT_BUFFER_LOCK=18,
    D3DBLOCKTYPE_ASYNC_CB_CALL_BLOCK_ALLOC=19,
    D3DBLOCKTYPE_ASYNC_CB_CALL_RESET=20,
    D3DBLOCKTYPE_ASYNC_CB_CALL_RELEASE=21,
    D3DBLOCKTYPE_PERFCOUNTER_BATCH_LOCK=22,
    D3DBLOCKTYPE_PERFCOUNTER_BATCH_RELEASE=23,
    D3DBLOCKTYPE_OCCLUSIONQUERY_BATCH_LOCK=24,
    D3DBLOCKTYPE_OCCLUSIONQUERY_BATCH_RELEASE=25,
    D3DBLOCKTYPE_MAX=26
} _D3DBLOCKTYPE;

typedef struct D3DVertexDeclaration D3DVertexDeclaration, *PD3DVertexDeclaration;

struct D3DVertexDeclaration {
    undefined field0_0x0;
    undefined field1_0x1;
    undefined field2_0x2;
    undefined field3_0x3;
    undefined field4_0x4;
    undefined field5_0x5;
    undefined field6_0x6;
    undefined field7_0x7;
    undefined field8_0x8;
    undefined field9_0x9;
    undefined field10_0xa;
    undefined field11_0xb;
    undefined field12_0xc;
    undefined field13_0xd;
    undefined field14_0xe;
    undefined field15_0xf;
    undefined field16_0x10;
    undefined field17_0x11;
    undefined field18_0x12;
    undefined field19_0x13;
    undefined field20_0x14;
    undefined field21_0x15;
    undefined field22_0x16;
    undefined field23_0x17;
};

typedef enum PIX_LOG_TYPE {
    LogType_HardDisk=0,
    LogType_PerfPipe=1,
    LogType_None=2,
    PIX_LOGS=2
} PIX_LOG_TYPE;

typedef struct _XOVERLAPPED _XOVERLAPPED, *P_XOVERLAPPED;

struct _XOVERLAPPED {
    ulong InternalLow;
    ulong InternalHigh;
    ulong InternalContext;
    void *hEvent;
    void (*pCompletionRoutine)(ulong, ulong, struct _XOVERLAPPED *);
    ulong dwCompletionContext;
    ulong dwExtendedError;
};

typedef struct D3DTexture D3DTexture, *PD3DTexture;

struct D3DTexture {
    undefined field0_0x0;
    undefined field1_0x1;
    undefined field2_0x2;
    undefined field3_0x3;
    undefined field4_0x4;
    undefined field5_0x5;
    undefined field6_0x6;
    undefined field7_0x7;
    undefined field8_0x8;
    undefined field9_0x9;
    undefined field10_0xa;
    undefined field11_0xb;
    undefined field12_0xc;
    undefined field13_0xd;
    undefined field14_0xe;
    undefined field15_0xf;
    undefined field16_0x10;
    undefined field17_0x11;
    undefined field18_0x12;
    undefined field19_0x13;
    undefined field20_0x14;
    undefined field21_0x15;
    undefined field22_0x16;
    undefined field23_0x17;
    undefined field24_0x18;
    undefined field25_0x19;
    undefined field26_0x1a;
    undefined field27_0x1b;
    undefined field28_0x1c;
    undefined field29_0x1d;
    undefined field30_0x1e;
    undefined field31_0x1f;
    undefined field32_0x20;
    undefined field33_0x21;
    undefined field34_0x22;
    undefined field35_0x23;
    undefined field36_0x24;
    undefined field37_0x25;
    undefined field38_0x26;
    undefined field39_0x27;
    undefined field40_0x28;
    undefined field41_0x29;
    undefined field42_0x2a;
    undefined field43_0x2b;
    undefined field44_0x2c;
    undefined field45_0x2d;
    undefined field46_0x2e;
    undefined field47_0x2f;
    undefined field48_0x30;
    undefined field49_0x31;
    undefined field50_0x32;
    undefined field51_0x33;
};

typedef union TP0_PERFCOUNTER0_LOW TP0_PERFCOUNTER0_LOW, *PTP0_PERFCOUNTER0_LOW;


/* WARNING! conflicting data type names: /iw5mp_xenon.pdb/TP0_PERFCOUNTER0_LOW/<unnamed-type-bitfields> - /iw5mp_xenon.pdb/D1SCL_COEF_RAM_SELECT/<unnamed-type-bitfields> */

union TP0_PERFCOUNTER0_LOW {
    struct <unnamed-type-bitfields> bitfields;
    struct <unnamed-type-bitfields> bits;
    uint u32All;
    int i32All;
    float f32All;
};

typedef union VGT_GROUP_VECT_1_FMT_CNTL VGT_GROUP_VECT_1_FMT_CNTL, *PVGT_GROUP_VECT_1_FMT_CNTL;


/* WARNING! conflicting data type names: /iw5mp_xenon.pdb/VGT_GROUP_VECT_1_FMT_CNTL/<unnamed-type-bitfields> - /iw5mp_xenon.pdb/D1SCL_COEF_RAM_SELECT/<unnamed-type-bitfields> */

union VGT_GROUP_VECT_1_FMT_CNTL {
    struct <unnamed-type-bitfields> bitfields;
    struct <unnamed-type-bitfields> bits;
    uint u32All;
    int i32All;
    float f32All;
};

typedef union CP_2D_PALETTE_BASE CP_2D_PALETTE_BASE, *PCP_2D_PALETTE_BASE;


/* WARNING! conflicting data type names: /iw5mp_xenon.pdb/CP_2D_PALETTE_BASE/<unnamed-type-bitfields> - /iw5mp_xenon.pdb/D1SCL_COEF_RAM_SELECT/<unnamed-type-bitfields> */

union CP_2D_PALETTE_BASE {
    struct <unnamed-type-bitfields> bitfields;
    struct <unnamed-type-bitfields> bits;
    uint u32All;
    int i32All;
    float f32All;
};

typedef enum <unnamed-tag_0000113B> {
    SPR_HSRR1=59
} <unnamed-tag_0000113B>;

typedef enum _D3DBLEND {
    D3DBLEND_ZERO=0,
    D3DBLEND_ONE=1,
    D3DBLEND_SRCCOLOR=4,
    D3DBLEND_INVSRCCOLOR=5,
    D3DBLEND_SRCALPHA=6,
    D3DBLEND_INVSRCALPHA=7,
    D3DBLEND_DESTCOLOR=8,
    D3DBLEND_INVDESTCOLOR=9,
    D3DBLEND_DESTALPHA=10,
    D3DBLEND_INVDESTALPHA=11,
    D3DBLEND_BLENDFACTOR=12,
    D3DBLEND_INVBLENDFACTOR=13,
    D3DBLEND_CONSTANTALPHA=14,
    D3DBLEND_INVCONSTANTALPHA=15,
    D3DBLEND_SRCALPHASAT=16,
    D3DBLEND_FORCE_DWORD=255
} _D3DBLEND;

typedef enum Ressource_type {
    ALU=0,
    TEXTURE=1
} Ressource_type;

typedef enum TileDepthFunc {
    TILE_LTE=0,
    TILE_GTE=1
} TileDepthFunc;

typedef enum _XAMUSER_SIGNIN_STATE {
    eXamUserSigninState_NotSignedIn=0,
    eXamUserSigninState_SignedInLocally=1,
    eXamUserSigninState_SignedInToLive=2
} _XAMUSER_SIGNIN_STATE;

typedef struct D3DXFLOAT16 D3DXFLOAT16, *PD3DXFLOAT16;

struct D3DXFLOAT16 {
    ushort value;
};

typedef enum <unnamed-tag_0000115F> {
    SPR_DRMR0=185
} <unnamed-tag_0000115F>;

typedef enum _D3DFOGMODE {
    D3DFOG_NONE=0,
    D3DFOG_EXP=1,
    D3DFOG_EXP2=2,
    D3DFOG_LINEAR=3,
    D3DFOG_FORCE_DWORD=255
} _D3DFOGMODE;

typedef struct _GUID _GUID, *P_GUID;

struct _GUID {
    ulong Data1;
    ushort Data2;
    ushort Data3;
    uchar Data4[8];
};

typedef enum _XPLAYERLIST_FLAGS {
    XPLAYERLIST_FLAG_CUSTOMTEXT=1
} _XPLAYERLIST_FLAGS;

typedef enum GPUPERFEVENT_PA_SC {
    GPUPE_SC_SR_WINDOW_VALID=0,
    GPUPE_SC_CW_WINDOW_VALID=1,
    GPUPE_SC_QM_WINDOW_VALID=2,
    GPUPE_SC_QPP_WINDOW_VALID=3,
    GPUPE_SC_ITER_WINDOW_VALID=4,
    GPUPE_SC_STARVED_BY_PA=5,
    GPUPE_SC_STARVED_BY_RCC=6,
    GPUPE_SC_STALLED_BY_PRIM_FF=7,
    GPUPE_SC_STALLED_BY_RCC=8,
    GPUPE_SC_STALLED_BY_BC=9,
    GPUPE_SC_STALLED_BY_SX=10,
    GPUPE_SC_STALLED_BY_SX0=11,
    GPUPE_SC_STALLED_BY_SX1=12,
    GPUPE_SC_STALLED_BY_SQ=13,
    GPUPE_SC_STALLED_BY_SP=14,
    GPUPE_SC_WAIT_FOR_R1=15,
    GPUPE_SC_SCISSOR_DISCARD=16,
    GPUPE_SC_BB_DISCARD=17,
    GPUPE_SC_SUPERT_PRIM_DISCARD=18,
    GPUPE_SC_RT_PRIM=19,
    GPUPE_SC_TILE_VALID=20,
    GPUPE_SC_TILE_PER_PRIM_H0=21,
    GPUPE_SC_TILE_PER_PRIM_H1=22,
    GPUPE_SC_TILE_PER_PRIM_H2=23,
    GPUPE_SC_TILE_PER_PRIM_H3=24,
    GPUPE_SC_TILE_PER_PRIM_H4=25,
    GPUPE_SC_TILE_PER_PRIM_H5=26,
    GPUPE_SC_TILE_PER_PRIM_H6=27,
    GPUPE_SC_TILE_PER_PRIM_H7=28,
    GPUPE_SC_TILE_PER_PRIM_H8=29,
    GPUPE_SC_TILE_PER_PRIM_H9=30,
    GPUPE_SC_TILE_PER_PRIM_H10=31,
    GPUPE_SC_TILE_PER_PRIM_H11=32,
    GPUPE_SC_TILE_PER_PRIM_H12=33,
    GPUPE_SC_TILE_PER_PRIM_H13=34,
    GPUPE_SC_TILE_PER_PRIM_H14=35,
    GPUPE_SC_TILE_PER_PRIM_H15=36,
    GPUPE_SC_TILE_PER_PRIM_H16=37,
    GPUPE_SC_SUPERT_TILE_DISCARD=38,
    GPUPE_SC_QM_NUM_QUADS=39,
    GPUPE_SC_QM_MASK_H0=40,
    GPUPE_SC_QM_MASK_H1=41,
    GPUPE_SC_QM_MASK_H2=42,
    GPUPE_SC_QM_MASK_H3=43,
    GPUPE_SC_QM_MASK_H4=44,
    GPUPE_SC_QM_MASK_H5=45,
    GPUPE_SC_QM_MASK_H6=46,
    GPUPE_SC_QM_MASK_H7=47,
    GPUPE_SC_QM_MASK_H8=48,
    GPUPE_SC_QM_MASK_H9=49,
    GPUPE_SC_QM_MASK_H10=50,
    GPUPE_SC_QM_MASK_H11=51,
    GPUPE_SC_QM_MASK_H12=52,
    GPUPE_SC_QM_MASK_H13=53,
    GPUPE_SC_QM_MASK_H14=54,
    GPUPE_SC_QM_MASK_H15=55,
    GPUPE_SC_QM_MASK_H16=56,
    GPUPE_SC_QM_COVERED_H0=57,
    GPUPE_SC_QM_COVERED_H1=58,
    GPUPE_SC_QM_COVERED_H2=59,
    GPUPE_SC_QM_COVERED_H3=60,
    GPUPE_SC_QM_COVERED_H4=61,
    GPUPE_SC_QM_COVERED_H5=62,
    GPUPE_SC_QM_COVERED_H6=63,
    GPUPE_SC_QM_COVERED_H7=64,
    GPUPE_SC_QM_COVERED_H8=65,
    GPUPE_SC_QM_COVERED_H9=66,
    GPUPE_SC_QM_COVERED_H10=67,
    GPUPE_SC_QM_COVERED_H11=68,
    GPUPE_SC_QM_COVERED_H12=69,
    GPUPE_SC_QM_COVERED_H13=70,
    GPUPE_SC_QM_COVERED_H14=71,
    GPUPE_SC_QM_COVERED_H15=72,
    GPUPE_SC_QM_COVERED_H16=73,
    GPUPE_SC_HIER_NUM_QUADS=74,
    GPUPE_SC_HIER_MASK_H0=75,
    GPUPE_SC_HIER_MASK_H1=76,
    GPUPE_SC_HIER_MASK_H2=77,
    GPUPE_SC_HIER_MASK_H3=78,
    GPUPE_SC_HIER_MASK_H4=79,
    GPUPE_SC_HIER_MASK_H5=80,
    GPUPE_SC_HIER_MASK_H6=81,
    GPUPE_SC_HIER_MASK_H7=82,
    GPUPE_SC_HIER_MASK_H8=83,
    GPUPE_SC_HIER_MASK_H9=84,
    GPUPE_SC_HIER_MASK_H10=85,
    GPUPE_SC_HIER_MASK_H11=86,
    GPUPE_SC_HIER_MASK_H12=87,
    GPUPE_SC_HIER_MASK_H13=88,
    GPUPE_SC_HIER_MASK_H14=89,
    GPUPE_SC_HIER_MASK_H15=90,
    GPUPE_SC_HIER_MASK_H16=91,
    GPUPE_SC_DET_NUM_QUADS=92,
    GPUPE_SC_PKR_QD_PER_ROW_H1=93,
    GPUPE_SC_PKR_QD_PER_ROW_H2=94,
    GPUPE_SC_PKR_QD_PER_ROW_H3=95,
    GPUPE_SC_PKR_QD_PER_ROW_H4=96,
    GPUPE_SC_PKR_END_OF_VECTOR=97,
    GPUPE_SC_PKR_ONE_CLK=98,
    GPUPE_SC_QD_WITH_1_PIX=99,
    GPUPE_SC_QD_WITH_2_PIX=100,
    GPUPE_SC_QD_WITH_3_PIX=101,
    GPUPE_SC_QD_WITH_4_PIX=102,
    GPUPE_SC_SR_WINDOW_VALID_BUSY=103,
    GPUPE_SC_CW_WINDOW_VALID_BUSY=104,
    GPUPE_SC_QM_WINDOW_VALID_BUSY=105,
    GPUPE_SC_QPP_WINDOW_VALID_BUSY=106,
    GPUPE_SC_ITER_WINDOW_VALID_BUSY=107
} GPUPERFEVENT_PA_SC;

typedef union DCP_CRC_P0_LAST DCP_CRC_P0_LAST, *PDCP_CRC_P0_LAST;


/* WARNING! conflicting data type names: /iw5mp_xenon.pdb/DCP_CRC_P0_LAST/<unnamed-type-bitfields> - /iw5mp_xenon.pdb/D1SCL_COEF_RAM_SELECT/<unnamed-type-bitfields> */

union DCP_CRC_P0_LAST {
    struct <unnamed-type-bitfields> bitfields;
    struct <unnamed-type-bitfields> bits;
    uint u32All;
    int i32All;
    float f32All;
};

typedef enum GPUNUMFORMAT {
    GPUNUMFORMAT_FRACTION=0,
    GPUNUMFORMAT_INTEGER=1
} GPUNUMFORMAT;

typedef enum CP_FUNCTION {
    CP_FUNCTION_NEVER=0,
    CP_FUNCTION_LESS=1,
    CP_FUNCTION_LEQUAL=2,
    CP_FUNCTION_EQUAL=3,
    CP_FUNCTION_NOTEQUAL=4,
    CP_FUNCTION_GEQUAL=5,
    CP_FUNCTION_GREATER=6,
    CP_FUNCTION_ALWAYS=7
} CP_FUNCTION;

typedef struct _D3DADAPTER_IDENTIFIER9 _D3DADAPTER_IDENTIFIER9, *P_D3DADAPTER_IDENTIFIER9;

struct _D3DADAPTER_IDENTIFIER9 {
    char Driver[512];
    char Description[512];
    char DeviceName[32];
    union _LARGE_INTEGER DriverVersion;
    ulong VendorId;
    ulong DeviceId;
    ulong SubSysId;
    ulong Revision;
    struct _GUID DeviceIdentifier;
    ulong WHQLLevel;
};

typedef struct _D3DVBLANKDATA _D3DVBLANKDATA, *P_D3DVBLANKDATA;

struct _D3DVBLANKDATA {
    ulong VBlank;
    ulong Swap;
    ulong Flags;
};

typedef struct sockaddr sockaddr, *Psockaddr;

struct sockaddr {
    ushort sa_family;
    char sa_data[14];
};

typedef union IRQ_BRIDGE_CNTL IRQ_BRIDGE_CNTL, *PIRQ_BRIDGE_CNTL;


/* WARNING! conflicting data type names: /iw5mp_xenon.pdb/IRQ_BRIDGE_CNTL/<unnamed-type-bitfields> - /iw5mp_xenon.pdb/D1SCL_COEF_RAM_SELECT/<unnamed-type-bitfields> */

union IRQ_BRIDGE_CNTL {
    struct <unnamed-type-bitfields> bitfields;
    struct <unnamed-type-bitfields> bits;
    uint u32All;
    int i32All;
    float f32All;
};

typedef struct _TOKEN_PRIMARY_GROUP _TOKEN_PRIMARY_GROUP, *P_TOKEN_PRIMARY_GROUP;

struct _TOKEN_PRIMARY_GROUP {
    void *PrimaryGroup;
};

typedef enum ColorFormat {
    COLOR_8=2,
    COLOR_1_5_5_5=3,
    COLOR_5_6_5=4,
    COLOR_6_5_5=5,
    COLOR_8_8_8_8=6,
    COLOR_2_10_10_10=7,
    COLOR_8_A=8,
    COLOR_8_B=9,
    COLOR_8_8=10,
    COLOR_8_8_8_8_A=14,
    COLOR_4_4_4_4=15,
    COLOR_10_11_11=16,
    COLOR_11_11_10=17,
    COLOR_16=24,
    COLOR_16_16=25,
    COLOR_16_16_16_16=26,
    COLOR_16_FLOAT=30,
    COLOR_16_16_FLOAT=31,
    COLOR_16_16_16_16_FLOAT=32,
    COLOR_32_FLOAT=36,
    COLOR_32_32_FLOAT=37,
    COLOR_32_32_32_32_FLOAT=38
} ColorFormat;

typedef enum _D3DSHADER_INSTRUCTION_OPCODE_TYPE {
    D3DSIO_NOP=0,
    D3DSIO_MOV=1,
    D3DSIO_ADD=2,
    D3DSIO_SUB=3,
    D3DSIO_MAD=4,
    D3DSIO_MUL=5,
    D3DSIO_RCP=6,
    D3DSIO_RSQ=7,
    D3DSIO_DP3=8,
    D3DSIO_DP4=9,
    D3DSIO_MIN=10,
    D3DSIO_MAX=11,
    D3DSIO_SLT=12,
    D3DSIO_SGE=13,
    D3DSIO_EXP=14,
    D3DSIO_LOG=15,
    D3DSIO_LIT=16,
    D3DSIO_DST=17,
    D3DSIO_LRP=18,
    D3DSIO_FRC=19,
    D3DSIO_M4x4=20,
    D3DSIO_M4x3=21,
    D3DSIO_M3x4=22,
    D3DSIO_M3x3=23,
    D3DSIO_M3x2=24,
    D3DSIO_CALL=25,
    D3DSIO_CALLNZ=26,
    D3DSIO_LOOP=27,
    D3DSIO_RET=28,
    D3DSIO_ENDLOOP=29,
    D3DSIO_LABEL=30,
    D3DSIO_DCL=31,
    D3DSIO_POW=32,
    D3DSIO_CRS=33,
    D3DSIO_SGN=34,
    D3DSIO_ABS=35,
    D3DSIO_NRM=36,
    D3DSIO_SINCOS=37,
    D3DSIO_REP=38,
    D3DSIO_ENDREP=39,
    D3DSIO_IF=40,
    D3DSIO_IFC=41,
    D3DSIO_ELSE=42,
    D3DSIO_ENDIF=43,
    D3DSIO_BREAK=44,
    D3DSIO_BREAKC=45,
    D3DSIO_MOVA=46,
    D3DSIO_DEFB=47,
    D3DSIO_DEFI=48,
    D3DSIO_TEXCOORD=64,
    D3DSIO_TEXKILL=65,
    D3DSIO_TEX=66,
    D3DSIO_TEXBEM=67,
    D3DSIO_TEXBEML=68,
    D3DSIO_TEXREG2AR=69,
    D3DSIO_TEXREG2GB=70,
    D3DSIO_TEXM3x2PAD=71,
    D3DSIO_TEXM3x2TEX=72,
    D3DSIO_TEXM3x3PAD=73,
    D3DSIO_TEXM3x3TEX=74,
    D3DSIO_RESERVED0=75,
    D3DSIO_TEXM3x3SPEC=76,
    D3DSIO_TEXM3x3VSPEC=77,
    D3DSIO_EXPP=78,
    D3DSIO_LOGP=79,
    D3DSIO_CND=80,
    D3DSIO_DEF=81,
    D3DSIO_TEXREG2RGB=82,
    D3DSIO_TEXDP3TEX=83,
    D3DSIO_TEXM3x2DEPTH=84,
    D3DSIO_TEXDP3=85,
    D3DSIO_TEXM3x3=86,
    D3DSIO_TEXDEPTH=87,
    D3DSIO_CMP=88,
    D3DSIO_BEM=89,
    D3DSIO_DP2ADD=90,
    D3DSIO_DSX=91,
    D3DSIO_DSY=92,
    D3DSIO_TEXLDD=93,
    D3DSIO_SETP=94,
    D3DSIO_TEXLDL=95,
    D3DSIO_BREAKP=96,
    D3DSIO_PHASE=253,
    D3DSIO_COMMENT=254,
    D3DSIO_END=255,
    D3DSIO_FORCE_DWORD=255
} _D3DSHADER_INSTRUCTION_OPCODE_TYPE;

typedef enum Instruction_serial {
    NOT_SERIAL=0,
    SERIAL=1
} Instruction_serial;

typedef union D1MODE_PRIORITY_A_CNT D1MODE_PRIORITY_A_CNT, *PD1MODE_PRIORITY_A_CNT;


/* WARNING! conflicting data type names: /iw5mp_xenon.pdb/D1MODE_PRIORITY_A_CNT/<unnamed-type-bitfields> - /iw5mp_xenon.pdb/D1SCL_COEF_RAM_SELECT/<unnamed-type-bitfields> */

union D1MODE_PRIORITY_A_CNT {
    struct <unnamed-type-bitfields> bitfields;
    struct <unnamed-type-bitfields> bits;
    uint u32All;
    int i32All;
    float f32All;
};

typedef enum _XHASHTYPE {
    eXHashSHA1Deprecated=0,
    eXHashSHA256=1,
    eXHashSHA384=2,
    eXHashSHA512=3
} _XHASHTYPE;

typedef enum tagSTGTY {
    STGTY_STORAGE=1,
    STGTY_STREAM=2,
    STGTY_LOCKBYTES=3,
    STGTY_PROPERTY=4
} tagSTGTY;

typedef enum GPUEXPORTREGISTER_VS {
    GPUEXPORTREGISTER_VS_INTERPOLATOR_0=0,
    GPUEXPORTREGISTER_VS_INTERPOLATOR_1=1,
    GPUEXPORTREGISTER_VS_INTERPOLATOR_2=2,
    GPUEXPORTREGISTER_VS_INTERPOLATOR_3=3,
    GPUEXPORTREGISTER_VS_INTERPOLATOR_4=4,
    GPUEXPORTREGISTER_VS_INTERPOLATOR_5=5,
    GPUEXPORTREGISTER_VS_INTERPOLATOR_6=6,
    GPUEXPORTREGISTER_VS_INTERPOLATOR_7=7,
    GPUEXPORTREGISTER_VS_INTERPOLATOR_8=8,
    GPUEXPORTREGISTER_VS_INTERPOLATOR_9=9,
    GPUEXPORTREGISTER_VS_INTERPOLATOR_10=10,
    GPUEXPORTREGISTER_VS_INTERPOLATOR_11=11,
    GPUEXPORTREGISTER_VS_INTERPOLATOR_12=12,
    GPUEXPORTREGISTER_VS_INTERPOLATOR_13=13,
    GPUEXPORTREGISTER_VS_INTERPOLATOR_14=14,
    GPUEXPORTREGISTER_VS_INTERPOLATOR_15=15,
    GPUEXPORTREGISTER_VS_EXPORT_ADDRESS=32,
    GPUEXPORTREGISTER_VS_EXPORT_DATA_0=33,
    GPUEXPORTREGISTER_VS_EXPORT_DATA_1=34,
    GPUEXPORTREGISTER_VS_EXPORT_DATA_2=35,
    GPUEXPORTREGISTER_VS_EXPORT_DATA_3=36,
    GPUEXPORTREGISTER_VS_EXPORT_DATA_4=37,
    GPUEXPORTREGISTER_VS_POSITION=62,
    GPUEXPORTREGISTER_VS_SPRITE_EDGE_KILL=63
} GPUEXPORTREGISTER_VS;

typedef enum _XPROFILE_GAMER_DIFFICULTY_OPTIONS {
    XPROFILE_GAMER_DIFFICULTY_NORMAL=0,
    XPROFILE_GAMER_DIFFICULTY_EASY=1,
    XPROFILE_GAMER_DIFFICULTY_HARD=2
} _XPROFILE_GAMER_DIFFICULTY_OPTIONS;

typedef union FSB_TR_CTRL_OR FSB_TR_CTRL_OR, *PFSB_TR_CTRL_OR;


/* WARNING! conflicting data type names: /iw5mp_xenon.pdb/FSB_TR_CTRL_OR/<unnamed-type-bitfields> - /iw5mp_xenon.pdb/D1SCL_COEF_RAM_SELECT/<unnamed-type-bitfields> */

union FSB_TR_CTRL_OR {
    struct <unnamed-type-bitfields> bitfields;
    struct <unnamed-type-bitfields> bits;
    uint u32All;
    int i32All;
    float f32All;
};

typedef union GPU_COLORCONTROL GPU_COLORCONTROL, *PGPU_COLORCONTROL;

typedef struct GPU_COLORCONTROL_s_0 GPU_COLORCONTROL_s_0, *PGPU_COLORCONTROL_s_0;

struct GPU_COLORCONTROL_s_0 {
    undefined field0_0x0;
    undefined field1_0x1;
    undefined field2_0x2;
    ulong AlphaToMaskOffset3:2;
    ulong AlphaToMaskOffset2:2;
    ulong AlphaToMaskEnable:1;
    ulong AlphaToMaskOffset1:2;
    ulong AlphaTestEnable:1;
    ulong AlphaToMaskOffset0:2;
    ulong AlphaFunc:3;
};

union GPU_COLORCONTROL {
    struct GPU_COLORCONTROL_s_0 _s_0;
    ulong dword;
};

typedef struct XAUDIOVOICEPACKETCALLBACK XAUDIOVOICEPACKETCALLBACK, *PXAUDIOVOICEPACKETCALLBACK;

struct XAUDIOVOICEPACKETCALLBACK {
    void *pVoiceContext;
    void *pPacketContext;
    T_HRESULT Status;
};

typedef struct _D3DSURFACES _D3DSURFACES, *P_D3DSURFACES;

struct _D3DSURFACES {
    struct D3DSurface *pDepthStencilSurface;
    struct D3DSurface *pRenderTarget[4];
};

typedef enum GPUFLOWOP {
    GPUFLOWOP_NOP=0,
    GPUFLOWOP_EXEC=1,
    GPUFLOWOP_EXEC_END=2,
    GPUFLOWOP_COND_EXEC=3,
    GPUFLOWOP_COND_EXEC_END=4,
    GPUFLOWOP_COND_EXEC_PRED=5,
    GPUFLOWOP_COND_EXEC_PRED_END=6,
    GPUFLOWOP_LOOP_START=7,
    GPUFLOWOP_LOOP_END=8,
    GPUFLOWOP_COND_CALL=9,
    GPUFLOWOP_RETURN=10,
    GPUFLOWOP_COND_JUMP=11,
    GPUFLOWOP_ALLOC=12,
    GPUFLOWOP_COND_EXEC_PRED_CLEAN=13,
    GPUFLOWOP_COND_EXEC_PRED_CLEAN_END=14,
    GPUFLOWOP_VFETCH_END=15
} GPUFLOWOP;

typedef union AZ1_SECONDARY_STATE_DWORD1 AZ1_SECONDARY_STATE_DWORD1, *PAZ1_SECONDARY_STATE_DWORD1;


/* WARNING! conflicting data type names: /iw5mp_xenon.pdb/AZ1_SECONDARY_STATE_DWORD1/<unnamed-type-bitfields> - /iw5mp_xenon.pdb/D1SCL_COEF_RAM_SELECT/<unnamed-type-bitfields> */

union AZ1_SECONDARY_STATE_DWORD1 {
    struct <unnamed-type-bitfields> bitfields;
    struct <unnamed-type-bitfields> bits;
    uint u32All;
    int i32All;
    float f32All;
};

typedef enum <unnamed-tag_0000115B> {
    SPR_TLB_RMT=183
} <unnamed-tag_0000115B>;

typedef enum <unnamed-tag_0000117F> {
    SPR_IRSR1=211
} <unnamed-tag_0000117F>;

typedef struct _PAM_SET_NOTIFICATON_HANDLER_EX _PAM_SET_NOTIFICATON_HANDLER_EX, *P_PAM_SET_NOTIFICATON_HANDLER_EX;

typedef enum _PIX_NOTIFICATION {
    PIX_NOTIFY_COUNTER_OVERFLOW=0,
    PIX_NOTIFY_STREAM_ENGINE_OUT_OF_MEMORY=1,
    PIX_NOTIFY_TIMING_CAPTURE=2,
    PIX_NOTIFY_NO=3,
    PIX_NOTIFY_BAD_D3D_VERSION=4,
    PIX_NOTIFY_GPU_CAPTURE=5,
    PIX_NOTIFY_NO_FRAME=6,
    PIX_NOTIFY_CPU_TRACE=7,
    PIX_NOTIFY_PAUSE=8,
    PIX_NOTIFY_RAW=9,
    PIX_NOTIFY_MISSING_HARDDRIVE=10,
    PIX_NOTIFY_VIDEO_OP_SUCCEEDED=11,
    PIX_NOTIFY_VIDEO_OP_ERROR=12,
    PIX_NOTIFY_MOVIE_DATA=13,
    PIX_NOTIFY_RESOURCE_CAPTURE=14,
    PIX_NOTIFY_REBOOT=15,
    PIX_NOTIFY_COUNT=16
} _PIX_NOTIFICATION;

struct _PAM_SET_NOTIFICATON_HANDLER_EX {
    ulong Size;
    int (*NewHandler)(enum _PIX_NOTIFICATION, void *);
    int (*OldHandler)(enum _PIX_NOTIFICATION, void *);
};

typedef enum GPUDESTBASE7 {
    GPUDESTBASE7_CLIPPLANE_DISABLED=0,
    GPUDESTBASE7_CLIPPLANE_ENABLED=0
} GPUDESTBASE7;

typedef enum <unnamed-tag_000010ED> {
    SPR_SRR0=26
} <unnamed-tag_000010ED>;

typedef enum _FP__IEEE_ROUNDING_MODE {
    FpRoundNearest=0,
    FpRoundMinusInfinity=1,
    FpRoundPlusInfinity=2,
    FpRoundChopped=3
} _FP__IEEE_ROUNDING_MODE;

typedef enum _D3DTAG {
    D3DTAG_PIXELSHADERCONSTANTS=0,
    D3DTAG_VERTEXSHADERCONSTANTS=0,
    D3DTAG_WAITUNTILIDLE=0,
    D3DTAG_WAITUNTILIDLEFLAGS=0,
    D3DTAG_FLUSHCACHES=1,
    D3DTAG_DESTINATIONPACKET=6,
    D3DTAG_SURFACEINFO=6,
    D3DTAG_COLOR0INFO=7,
    D3DTAG_FLOWCONSTANTS=7,
    D3DTAG_MISCCONSTANTS=7,
    D3DTAG_DEPTHINFO=8,
    D3DTAG_RESERVED0=8,
    D3DTAG_CLIPPLANE0=9,
    D3DTAG_CLIPPLANES=9,
    D3DTAG_COLOR1INFO=9,
    D3DTAG_POINTSIZE=9,
    D3DTAG_TESSELLATORPACKET=9,
    D3DTAG_CLIPPLANE1=10,
    D3DTAG_COLOR2INFO=10,
    D3DTAG_POINTMINMAX=10,
    D3DTAG_CLIPPLANE2=11,
    D3DTAG_COLOR3INFO=11,
    D3DTAG_LINECONTROL=11,
    D3DTAG_CLIPPLANE3=12,
    D3DTAG_COHERDESTBASE0=12,
    D3DTAG_LINESTIPPLE=12,
    D3DTAG_CLIPPLANE4=13,
    D3DTAG_COHERDESTBASE1=13,
    D3DTAG_OUTPUTPATHCONTROL=13,
    D3DTAG_CLIPPLANE5=14,
    D3DTAG_COHERDESTBASE2=14,
    D3DTAG_HOSCONTROL=14,
    D3DTAG_COHERDESTBASE3=15,
    D3DTAG_HOSMAXTESSLEVEL=15,
    D3DTAG_TERMINATOR=15,
    D3DTAG_WINDOWOFFSET=15,
    D3DTAG_WINDOWPACKET=15,
    D3DTAG_COHERDESTBASE4=16,
    D3DTAG_HOSMINTESSLEVEL=16,
    D3DTAG_WINDOWSCISSORTL=16,
    D3DTAG_COHERDESTBASE5=17,
    D3DTAG_HOSREUSEDEPTH=17,
    D3DTAG_WINDOWSCISSORBR=17,
    D3DTAG_COHERDESTBASE6=18,
    D3DTAG_GROUPPRIMTYPE=18,
    D3DTAG_POINTPACKET=18,
    D3DTAG_POLYOFFSETFRONTSCALE=18,
    D3DTAG_COHERDESTBASE7=19,
    D3DTAG_GROUPFIRSTDECR=19,
    D3DTAG_POLYOFFSETFRONTOFFSET=19,
    D3DTAG_GROUPDECR=20,
    D3DTAG_POLYOFFSETBACKSCALE=20,
    D3DTAG_SCREENSCISSORTL=20,
    D3DTAG_GROUPVECT0CONTROL=21,
    D3DTAG_POLYOFFSETBACKOFFSET=21,
    D3DTAG_SCREENSCISSORBR=21,
    D3DTAG_GROUPVECT1CONTROL=22,
    D3DTAG_MAXVTXINDX=22,
    D3DTAG_POINTXRAD=22,
    D3DTAG_VALUESPACKET=22,
    D3DTAG_GROUPVECT0FMTCONTROL=23,
    D3DTAG_MINVTXINDX=23,
    D3DTAG_POINTYRAD=23,
    D3DTAG_GROUPVECT1FMTCONTROL=24,
    D3DTAG_INDXOFFSET=24,
    D3DTAG_POINTCONSTANTSIZE=24,
    D3DTAG_MULTIPRIMIBRESETINDX=25,
    D3DTAG_POINTCULLRAD=25,
    D3DTAG_UNUSED_TESSELLATOR0=25,
    D3DTAG_COLORMASK=26,
    D3DTAG_MISCPACKET=26,
    D3DTAG_SCLINECONTROL=26,
    D3DTAG_UNUSED_TESSELLATOR1=26,
    D3DTAG_AACONFIG=27,
    D3DTAG_BLENDFACTOR=27,
    D3DTAG_BLENDRED=27,
    D3DTAG_MPASSPSCONTROL=27,
    D3DTAG_BLENDGREEN=28,
    D3DTAG_VIZQUERY=28,
    D3DTAG_VTXCONTROL=28,
    D3DTAG_BLENDBLUE=29,
    D3DTAG_ENHANCE=29,
    D3DTAG_GBVERTCLIPADJ=29,
    D3DTAG_BLENDALPHA=30,
    D3DTAG_GBVERTDISCADJ=30,
    D3DTAG_GBHORZCLIPADJ=31,
    D3DTAG_UNUSED_VALUES0=31,
    D3DTAG_FETCHCONSTANTS=32,
    D3DTAG_GBHORZDISCADJ=32,
    D3DTAG_TEXTUREFETCHCONSTANTS=32,
    D3DTAG_UNUSED_VALUES1=32,
    D3DTAG_UNUSED_VALUES2=33,
    D3DTAG_VSCONST=33,
    D3DTAG_PSCONST=34,
    D3DTAG_STENCILREFMASKBF=34,
    D3DTAG_STENCILREFMASKS=34,
    D3DTAG_DEBUGMISC0=35,
    D3DTAG_STENCILREFMASK=35,
    D3DTAG_ALPHAREF=36,
    D3DTAG_DEBUGMISC1=36,
    D3DTAG_UNUSED_MISC0=37,
    D3DTAG_VPORTXSCALE=37,
    D3DTAG_UNUSED_MISC1=38,
    D3DTAG_VPORTXOFFSET=38,
    D3DTAG_UNUSED_MISC2=39,
    D3DTAG_VPORTYSCALE=39,
    D3DTAG_UNUSED_MISC3=40,
    D3DTAG_VPORTYOFFSET=40,
    D3DTAG_UNUSED_MISC4=41,
    D3DTAG_VPORTZSCALE=41,
    D3DTAG_UNUSED_MISC5=42,
    D3DTAG_VPORTZOFFSET=42,
    D3DTAG_PIXELSHADER=43,
    D3DTAG_SHADERFLAGS=43,
    D3DTAG_UNUSED_MISC6=43,
    D3DTAG_AAMASK=44,
    D3DTAG_VERTEXSHADER=44,
    D3DTAG_UNUSED_MISC7=45,
    D3DTAG_ZPASSEDRAMMODE=45,
    D3DTAG_HIZENABLE=46,
    D3DTAG_UNUSED_MISC8=46,
    D3DTAG_PROGRAMCONTROL=47,
    D3DTAG_PROGRAMPACKET=47,
    D3DTAG_UNUSED_MISC9=47,
    D3DTAG_CONTEXTMISC=48,
    D3DTAG_VERTEXREUSEBLOCKCONTROL=48,
    D3DTAG_INTERPOLATORCONTROL=49,
    D3DTAG_OUTDEALLOCCONTROL=49,
    D3DTAG_COPYCONTROL=50,
    D3DTAG_WRAPPING0=50,
    D3DTAG_COPYDESTBASE=51,
    D3DTAG_WRAPPING1=51,
    D3DTAG_CONTROLPACKET=52,
    D3DTAG_COPYDESTPITCH=52,
    D3DTAG_DEPTHCONTROL=52,
    D3DTAG_BLENDCONTROL0=53,
    D3DTAG_COPYDESTINFO=53,
    D3DTAG_COLORCONTROL=54,
    D3DTAG_HICLEAR=54,
    D3DTAG_DEPTHCLEAR=55,
    D3DTAG_HICONTROL=55,
    D3DTAG_CLIPCONTROL=56,
    D3DTAG_COLORCLEAR=56,
    D3DTAG_COLORCLEARLO=57,
    D3DTAG_MODECONTROL=57,
    D3DTAG_COPYFUNC=58,
    D3DTAG_VERTEXFETCHCONSTANTS=58,
    D3DTAG_VTECONTROL=58,
    D3DTAG_COPYREF=59,
    D3DTAG_UNUSED_CONTROL0=59,
    D3DTAG_COPYMASK=60,
    D3DTAG_EDRAMMODECONTROL=60,
    D3DTAG_BLENDCONTROL1=61,
    D3DTAG_COPYSURFACESLICE=61,
    D3DTAG_BLENDCONTROL2=62,
    D3DTAG_SAMPLECOUNTCONTROL=62,
    D3DTAG_BLENDCONTROL3=63,
    D3DTAG_SAMPLECOUNTADDRESS=63
} _D3DTAG;

typedef union PA_SU_POLY_OFFSET_FRONT_OFFSET PA_SU_POLY_OFFSET_FRONT_OFFSET, *PPA_SU_POLY_OFFSET_FRONT_OFFSET;


/* WARNING! conflicting data type names: /iw5mp_xenon.pdb/PA_SU_POLY_OFFSET_FRONT_OFFSET/<unnamed-type-bitfields> - /iw5mp_xenon.pdb/D1SCL_COEF_RAM_SELECT/<unnamed-type-bitfields> */

union PA_SU_POLY_OFFSET_FRONT_OFFSET {
    struct <unnamed-type-bitfields> bitfields;
    struct <unnamed-type-bitfields> bits;
    uint u32All;
    int i32All;
    float f32All;
};

typedef enum _FP__IEEE_FORMAT {
    FpFormatFp32=0,
    FpFormatFp64=1,
    FpFormatFp80=2,
    FpFormatFp128=3,
    FpFormatI16=4,
    FpFormatI32=5,
    FpFormatI64=6,
    FpFormatU16=7,
    FpFormatU32=8,
    FpFormatU64=9,
    FpFormatCompare=10,
    FpFormatString=11
} _FP__IEEE_FORMAT;

typedef enum GPUSURFACENUMBER {
    GPUSURFACENUMBER_UREPEAT=0,
    GPUSURFACENUMBER_SREPEAT=1,
    GPUSURFACENUMBER_UINTEGER=2,
    GPUSURFACENUMBER_SINTEGER=3,
    GPUSURFACENUMBER_FLOAT=7
} GPUSURFACENUMBER;

typedef enum GPUREQUESTSIZE {
    GPUREQUESTSIZE_256BIT=0,
    GPUREQUESTSIZE_512BIT=1
} GPUREQUESTSIZE;

typedef enum <unnamed-tag_00001119> {
    CERTIFICATE_LOG_COUNT=62
} <unnamed-tag_00001119>;

typedef enum CP_CODE_ID {
    CP_CODE_ID_VERTEX=0,
    CP_CODE_ID_PIXEL=1,
    CP_CODE_ID_SHARED=2
} CP_CODE_ID;

typedef struct _LDRX_SOURCE _LDRX_SOURCE, *P_LDRX_SOURCE;

typedef union _LDRX_SOURCE_u_12 _LDRX_SOURCE_u_12, *P_LDRX_SOURCE_u_12;

union _LDRX_SOURCE_u_12 {
    void *ImageFileHandle;
    uchar *Data;
};

struct _LDRX_SOURCE {
    void *XexHeaders;
    ulong ImageSize;
    long (*ImageFileRead)(void *, ulong, void *, ulong, struct _IO_STATUS_BLOCK *, void *);
    union _LDRX_SOURCE_u_12 field3_0xc;
    ulong SizeOfData;
    ulong ImageMediaTypes;
};

typedef struct tagRECT tagRECT, *PtagRECT;

struct tagRECT {
    long left;
    long top;
    long right;
    long bottom;
};

typedef enum _XCONFIG_USER_ENTRIES {
    XCONFIG_USER_DATA=0,
    XCONFIG_USER_TIME_ZONE_BIAS=1,
    XCONFIG_USER_TIME_ZONE_STD_NAME=2,
    XCONFIG_USER_TIME_ZONE_DLT_NAME=3,
    XCONFIG_USER_TIME_ZONE_STD_DATE=4,
    XCONFIG_USER_TIME_ZONE_DLT_DATE=5,
    XCONFIG_USER_TIME_ZONE_STD_BIAS=6,
    XCONFIG_USER_TIME_ZONE_DLT_BIAS=7,
    XCONFIG_USER_DEFAULT_PROFILE=8,
    XCONFIG_USER_LANGUAGE=9,
    XCONFIG_USER_VIDEO_FLAGS=10,
    XCONFIG_USER_AUDIO_FLAGS=11,
    XCONFIG_USER_RETAIL_FLAGS=12,
    XCONFIG_USER_DEVKIT_FLAGS=13,
    XCONFIG_USER_COUNTRY=14,
    XCONFIG_USER_PC_FLAGS=15,
    XCONFIG_USER_SMB_CONFIG=16,
    XCONFIG_USER_LIVE_PUID=17,
    XCONFIG_USER_LIVE_CREDENTIALS=18,
    XCONFIG_USER_AV_COMPOSITE_SCREENSZ=19,
    XCONFIG_USER_AV_COMPONENT_SCREENSZ=20,
    XCONFIG_USER_AV_VGA_SCREENSZ=21,
    XCONFIG_USER_PC_GAME=22,
    XCONFIG_USER_PC_PASSWORD=23,
    XCONFIG_USER_PC_MOVIE=24,
    XCONFIG_USER_PC_GAME_RATING=25,
    XCONFIG_USER_PC_MOVIE_RATING=26,
    XCONFIG_USER_PC_HINT=27,
    XCONFIG_USER_PC_HINT_ANSWER=28,
    XCONFIG_USER_PC_OVERRIDE=29,
    XCONFIG_USER_MUSIC_PLAYBACK_MODE=30,
    XCONFIG_USER_MUSIC_VOLUME=31,
    XCONFIG_USER_MUSIC_FLAGS=32,
    XCONFIG_USER_ARCADE_FLAGS=33,
    XCONFIG_USER_PC_VERSION=34,
    XCONFIG_USER_PC_TV=35,
    XCONFIG_USER_PC_TV_RATING=36,
    XCONFIG_USER_PC_EXPLICIT_VIDEO=37,
    XCONFIG_USER_PC_EXPLICIT_VIDEO_RATING=38,
    XCONFIG_USER_PC_UNRATED_VIDEO=39,
    XCONFIG_USER_PC_UNRATED_VIDEO_RATING=40,
    XCONFIG_USER_VIDEO_OUTPUT_BLACK_LEVELS=41,
    XCONFIG_USER_VIDEO_PLAYER_DISPLAY_MODE=42,
    XCONFIG_USER_ALTERNATE_VIDEO_TIMING_ID=43,
    XCONFIG_USER_VIDEO_DRIVER_OPTIONS=44,
    XCONFIG_USER_MUSIC_UI_FLAGS=45,
    XCONFIG_USER_VIDEO_MEDIA_SOURCE_TYPE=46,
    XCONFIG_USER_MUSIC_MEDIA_SOURCE_TYPE=47,
    XCONFIG_USER_PHOTO_MEDIA_SOURCE_TYPE=48
} _XCONFIG_USER_ENTRIES;

typedef enum _D3DBLENDOP {
    D3DBLENDOP_ADD=0,
    D3DBLENDOP_SUBTRACT=1,
    D3DBLENDOP_MIN=2,
    D3DBLENDOP_MAX=3,
    D3DBLENDOP_REVSUBTRACT=4,
    D3DBLENDOP_FORCE_DWORD=255
} _D3DBLENDOP;

typedef enum _TOKEN_INFORMATION_CLASS {
    TokenUser=1,
    TokenGroups=2,
    TokenPrivileges=3,
    TokenOwner=4,
    TokenPrimaryGroup=5,
    TokenDefaultDacl=6,
    TokenSource=7,
    TokenType=8,
    TokenImpersonationLevel=9,
    TokenStatistics=10,
    TokenRestrictedSids=11,
    TokenSessionId=12
} _TOKEN_INFORMATION_CLASS;

typedef enum <unnamed-tag_0000137B> {
    DVD_AUTH_EX_ENFORCED_VERSION_GLOBAL=0,
    DVD_AUTH_EX_ENFORCED_VERSION_LOCAL=0,
    DVD_AUTH_EX_FLAG_BIT_DATA_UNIQUE=0,
    DVD_AUTH_EX_FLAG_BIT_ENTRY_PROFILEID=0,
    DVD_AUTH_EX_FLAG_BIT_HEADER_LOCAL=0,
    DVD_AUTH_EX_FLAG_BIT_TABLE_MAXIMUM=0,
    DVD_AUTH_EX_FLAG_DATA_NONE=0,
    DVD_AUTH_EX_FLAG_ENTRY_NONE=0,
    DVD_AUTH_EX_FLAG_HEADER_ALTERNATE=0,
    DVD_AUTH_EX_FLAG_HEADER_NONE=0,
    DVD_AUTH_EX_FLAG_NONE=0,
    DVD_AUTH_EX_FLAG_TABLE_NONE=0,
    DVD_AUTH_EX_CHALLENGE_DEFAULT_FAILURE=1,
    DVD_AUTH_EX_FLAG_BIT_DATA_MAXIMUM=1,
    DVD_AUTH_EX_FLAG_BIT_ENTRY_TITLEID=1,
    DVD_AUTH_EX_FLAG_BIT_HEADER_KEYED=1,
    DVD_AUTH_EX_FLAG_DATA_UNIQUE=1,
    DVD_AUTH_EX_FLAG_DATA_VALID=1,
    DVD_AUTH_EX_FLAG_ENTRY_PROFILEID=1,
    DVD_AUTH_EX_FLAG_HEADER_LOCAL=1,
    DVD_AUTH_EX_HEADER_VERSION=1,
    DVD_AUTH_EX_TOLERANCE_MINIMUM=1,
    DVD_AUTH_EX_UNLOCK_PERIOD_MINIMUM=1,
    DVD_AUTH_EX_FLAG_BIT_ENTRY_MAXIMUM=2,
    DVD_AUTH_EX_FLAG_BIT_HEADER_MAXIMUM=2,
    DVD_AUTH_EX_FLAG_ENTRY_TITLEID=2,
    DVD_AUTH_EX_FLAG_HEADER_KEYED=2,
    DVD_AUTH_EX_CHALLENGE_DEFAULT=3,
    DVD_AUTH_EX_CHALLENGE_DEFAULT_SUCCESS=3,
    DVD_AUTH_EX_CHALLENGE_MINIMUM=3,
    DVD_AUTH_EX_TABLE_VERSION_LOCAL=3,
    DVD_AUTH_EX_UNLOCK_PERIOD_DEFAULT=3,
    DVD_AUTH_EX_TOLERANCE_DEFAULT=5,
    DVD_AUTH_EX_TABLE_VERSION_GLOBAL=6,
    DVD_AUTH_EX_CHALLENGE_MAXIMUM=12,
    DVD_AUTH_EX_FLAG_BIT_HEADER_ALTERNATE=15,
    DVD_AUTH_EX_UNLOCK_PERIOD_MAXIMUM=24,
    DVD_AUTH_EX_SIGNER_LIVE=76,
    DVD_AUTH_EX_SIGNER_MNFG=77,
    DVD_AUTH_EX_SIGNER_PIRS=80,
    DVD_AUTH_EX_TOLERANCE_MAXIMUM=180
} <unnamed-tag_0000137B>;

typedef enum _D3DSTATEBLOCKTYPE {
    D3DSBT_ALL=1,
    D3DSBT_PIXELSTATE=2,
    D3DSBT_VERTEXSTATE=3,
    D3DSBT_FORCE_DWORD=255
} _D3DSTATEBLOCKTYPE;

typedef enum GPUCULL {
    GPUCULL_NONE_FRONTFACE_CCW=0,
    GPUCULL_FRONT_FRONTFACE_CCW=1,
    GPUCULL_BACK_FRONTFACE_CCW=2,
    GPUCULL_NONE_FRONTFACE_CW=4,
    GPUCULL_FRONT_FRONTFACE_CW=5,
    GPUCULL_BACK_FRONTFACE_CW=6
} GPUCULL;

typedef enum <unnamed-tag_0000126C> {
    XEIKA_DATA_ODD_VERSION=1,
    XEIKA_DATA_VERSION=1,
    XEIKA_PADDING_SIZE=70,
    XEIKA_DATA_SIGNATURE=71,
    XEIKA_CERTIFICATE_SIZE=136
} <unnamed-tag_0000126C>;

typedef enum GPUSYNCSPACE {
    GPUSYNCSPACE_REGISTER=0,
    GPUSYNCSPACE_MEMORY=1
} GPUSYNCSPACE;

typedef enum <unnamed-tag_0000115D> {
    SPR_DRSR0=184
} <unnamed-tag_0000115D>;

typedef enum _XUSER_SIGNIN_STATE {
    eXUserSigninState_NotSignedIn=0,
    eXUserSigninState_SignedInLocally=1,
    eXUserSigninState_SignedInToLive=2
} _XUSER_SIGNIN_STATE;

typedef enum _XSESSION_STATE {
    XSESSION_STATE_LOBBY=0,
    XSESSION_STATE_REGISTRATION=1,
    XSESSION_STATE_INGAME=2,
    XSESSION_STATE_REPORTING=3,
    XSESSION_STATE_DELETED=4
} _XSESSION_STATE;

typedef enum GPUMIPFILTER {
    GPUMIPFILTER_POINT=0,
    GPUMIPFILTER_LINEAR=1,
    GPUMIPFILTER_BASEMAP=2,
    GPUMIPFILTER_KEEP=3
} GPUMIPFILTER;

typedef struct XAUDIOSOURCEFXPACKETCALLBACK XAUDIOSOURCEFXPACKETCALLBACK, *PXAUDIOSOURCEFXPACKETCALLBACK;

struct XAUDIOSOURCEFXPACKETCALLBACK {
    void *pEffectContext;
    void *pPacketContext;
    T_HRESULT Status;
};

typedef union BC_DUMMY_CRAYRB_ENUMS BC_DUMMY_CRAYRB_ENUMS, *PBC_DUMMY_CRAYRB_ENUMS;


/* WARNING! conflicting data type names: /iw5mp_xenon.pdb/BC_DUMMY_CRAYRB_ENUMS/<unnamed-type-bitfields> - /iw5mp_xenon.pdb/D1SCL_COEF_RAM_SELECT/<unnamed-type-bitfields> */

union BC_DUMMY_CRAYRB_ENUMS {
    struct <unnamed-type-bitfields> bitfields;
    struct <unnamed-type-bitfields> bits;
    uint u32All;
    int i32All;
    float f32All;
};

typedef enum <unnamed-tag_000010EF> {
    SPR_SRR1=27
} <unnamed-tag_000010EF>;

typedef enum _D3DXMESHSIMP {
    D3DXMESHSIMP_VERTEX=1,
    D3DXMESHSIMP_FACE=2,
    D3DXMESHSIMP_VALIDBITS=3
} _D3DXMESHSIMP;

typedef enum __VECTOR_PACK_MASK {
    VPACK_32=1,
    VPACK_64LO=2,
    VPACK_64HI=3
} __VECTOR_PACK_MASK;

typedef enum _XSHOWMARKETPLACEDOWNLOADITEMSUI_ENTRYPOINTS {
    XSHOWMARKETPLACEDOWNLOADITEMS_ENTRYPOINT_FREEITEMS=232,
    XSHOWMARKETPLACEDOWNLOADITEMS_ENTRYPOINT_PAIDITEMS=233,
    XSHOWMARKETPLACEDOWNLOADITEMS_ENTRYPOINT_MAX=234
} _XSHOWMARKETPLACEDOWNLOADITEMSUI_ENTRYPOINTS;

typedef enum PIX_CHUNK_TYPE {
    ChunkType_CpuTiming0=0,
    ChunkType_CpuTiming1=1,
    ChunkType_CpuTiming2=2,
    ChunkType_CpuTiming3=3,
    ChunkType_CpuTiming4=4,
    ChunkType_CpuTiming5=5,
    ChunkType_Profiler0=6,
    PIX_OVERFLOWABLE_CHUNK_TYPES=6,
    ChunkType_Profiler1=7,
    ChunkType_Profiler2=8,
    ChunkType_Profiler3=9,
    ChunkType_Profiler4=10,
    ChunkType_Profiler5=11,
    ChunkType_Audio=12,
    ChunkType_GpuVideo=13,
    PIX_CPU_NON_INDEX_CHUNK_TYPES=13,
    ChunkType_GpuTiming=14,
    ChunkType_IndexHardDisk=15,
    PIX_NON_INDEX_CHUNK_TYPES=15,
    ChunkType_IndexPerfPipe=16,
    ChunkType_Padding=17,
    PIX_CHUNK_TYPES=18
} PIX_CHUNK_TYPE;

typedef enum _XMSG_FLAGS {
    XMSG_FLAG_DISABLE_EDIT_RECIPIENTS=1
} _XMSG_FLAGS;

typedef enum _XPROFILE_RACE_BRAKE_CONTROL_OPTIONS {
    XPROFILE_RACE_BRAKE_CONTROL_TRIGGER=0,
    XPROFILE_RACE_BRAKE_CONTROL_BUTTON=1
} _XPROFILE_RACE_BRAKE_CONTROL_OPTIONS;

typedef union MC1_BSB_SNOOPED_TIMER_CNTL MC1_BSB_SNOOPED_TIMER_CNTL, *PMC1_BSB_SNOOPED_TIMER_CNTL;


/* WARNING! conflicting data type names: /iw5mp_xenon.pdb/MC1_BSB_SNOOPED_TIMER_CNTL/<unnamed-type-bitfields> - /iw5mp_xenon.pdb/D1SCL_COEF_RAM_SELECT/<unnamed-type-bitfields> */

union MC1_BSB_SNOOPED_TIMER_CNTL {
    struct <unnamed-type-bitfields> bitfields;
    struct <unnamed-type-bitfields> bits;
    uint u32All;
    int i32All;
    float f32All;
};

typedef union _DYNAMIC_REVOCATION_LIST _DYNAMIC_REVOCATION_LIST, *P_DYNAMIC_REVOCATION_LIST;

union _DYNAMIC_REVOCATION_LIST {
    struct _CERTIFICATE_REVOCATION_LIST_HEADER RevocationList;
    uchar Reserved[32768];
};

typedef enum _PIX_AUTOMATION_MESSAGE_ID {
    PAM_SEND_PIX_COMMAND=0,
    PAM_SET_NOTIFICATON_HANDLER=1,
    PAM_COUNT=2
} _PIX_AUTOMATION_MESSAGE_ID;

typedef enum _D3DXREGISTER_SET {
    D3DXRS_BOOL=0,
    D3DXRS_INT4=1,
    D3DXRS_FLOAT4=2,
    D3DXRS_SAMPLER=3,
    D3DXRS_FORCE_DWORD=255
} _D3DXREGISTER_SET;

typedef union _KUSER_MODE_PTE _KUSER_MODE_PTE, *P_KUSER_MODE_PTE;

typedef struct _KUSER_MODE_PTE_s_0 _KUSER_MODE_PTE_s_0, *P_KUSER_MODE_PTE_s_0;

struct _KUSER_MODE_PTE_s_0 {
    ulong Dirty:1;
    ulong WriteThrough:1;
    ulong InhibitCache:1;
    ulong MemoryCoherency:1;
    ulong Guarded:1;
    ulong NoExecute:1;
    ulong PageProtection:2;
    ulong PageFrameNumber:20;
    ulong Reserved:3;
    ulong Accessed:1;
};

union _KUSER_MODE_PTE {
    struct _KUSER_MODE_PTE_s_0 _s_0;
    ulong AsULONG;
};

typedef union MC0_TIMING_CNTL_1 MC0_TIMING_CNTL_1, *PMC0_TIMING_CNTL_1;


/* WARNING! conflicting data type names: /iw5mp_xenon.pdb/MC0_TIMING_CNTL_1/<unnamed-type-bitfields> - /iw5mp_xenon.pdb/D1SCL_COEF_RAM_SELECT/<unnamed-type-bitfields> */

union MC0_TIMING_CNTL_1 {
    struct <unnamed-type-bitfields> bitfields;
    struct <unnamed-type-bitfields> bits;
    uint u32All;
    int i32All;
    float f32All;
};

typedef enum _D3DPRIMITIVETYPE {
    D3DPT_POINTLIST=1,
    D3DPT_LINELIST=2,
    D3DPT_LINESTRIP=3,
    D3DPT_TRIANGLELIST=4,
    D3DPT_TRIANGLEFAN=5,
    D3DPT_TRIANGLESTRIP=6,
    D3DPT_RECTLIST=8,
    D3DPT_QUADLIST=13,
    D3DPT_FORCE_DWORD=255
} _D3DPRIMITIVETYPE;

typedef struct D3DVolumeTexture D3DVolumeTexture, *PD3DVolumeTexture;

struct D3DVolumeTexture {
    undefined field0_0x0;
    undefined field1_0x1;
    undefined field2_0x2;
    undefined field3_0x3;
    undefined field4_0x4;
    undefined field5_0x5;
    undefined field6_0x6;
    undefined field7_0x7;
    undefined field8_0x8;
    undefined field9_0x9;
    undefined field10_0xa;
    undefined field11_0xb;
    undefined field12_0xc;
    undefined field13_0xd;
    undefined field14_0xe;
    undefined field15_0xf;
    undefined field16_0x10;
    undefined field17_0x11;
    undefined field18_0x12;
    undefined field19_0x13;
    undefined field20_0x14;
    undefined field21_0x15;
    undefined field22_0x16;
    undefined field23_0x17;
    undefined field24_0x18;
    undefined field25_0x19;
    undefined field26_0x1a;
    undefined field27_0x1b;
    undefined field28_0x1c;
    undefined field29_0x1d;
    undefined field30_0x1e;
    undefined field31_0x1f;
    undefined field32_0x20;
    undefined field33_0x21;
    undefined field34_0x22;
    undefined field35_0x23;
    undefined field36_0x24;
    undefined field37_0x25;
    undefined field38_0x26;
    undefined field39_0x27;
    undefined field40_0x28;
    undefined field41_0x29;
    undefined field42_0x2a;
    undefined field43_0x2b;
    undefined field44_0x2c;
    undefined field45_0x2d;
    undefined field46_0x2e;
    undefined field47_0x2f;
    undefined field48_0x30;
    undefined field49_0x31;
    undefined field50_0x32;
    undefined field51_0x33;
};

typedef struct _D3DVERTEXELEMENT9 _D3DVERTEXELEMENT9, *P_D3DVERTEXELEMENT9;

struct _D3DVERTEXELEMENT9 {
    ushort Stream;
    ushort Offset;
    ulong Type;
    uchar Method;
    uchar Usage;
    uchar UsageIndex;
};

typedef enum _D3DFILLMODE {
    D3DFILL_SOLID=0,
    D3DFILL_POINT=1,
    D3DFILL_WIREFRAME=37,
    D3DFILL_FORCE_DWORD=255
} _D3DFILLMODE;

typedef enum VGT_DI_PRIM_TYPE {
    DI_PT_NONE=0,
    DI_PT_POINTLIST=1,
    DI_PT_LINELIST=2,
    DI_PT_LINESTRIP=3,
    DI_PT_TRILIST=4,
    DI_PT_TRIFAN=5,
    DI_PT_TRISTRIP=6,
    DI_PT_TRI_WITH_WFLAGS=7,
    DI_PT_RECTLIST=8,
    DI_PT_UNUSED_1=9,
    DI_PT_UNUSED_2=10,
    DI_PT_UNUSED_3=11,
    DI_PT_LINELOOP=12,
    DI_PT_QUADLIST=13,
    DI_PT_QUADSTRIP=14,
    DI_PT_POLYGON=15,
    DI_PT_2D_COPY_RECT_LIST_V0=16,
    DI_PT_2D_COPY_RECT_LIST_V1=17,
    DI_PT_2D_COPY_RECT_LIST_V2=18,
    DI_PT_2D_COPY_RECT_LIST_V3=19,
    DI_PT_2D_FILL_RECT_LIST=20,
    DI_PT_2D_LINE_STRIP=21,
    DI_PT_2D_TRI_STRIP=22
} VGT_DI_PRIM_TYPE;

typedef union BIOS_9_SCRATCH BIOS_9_SCRATCH, *PBIOS_9_SCRATCH;


/* WARNING! conflicting data type names: /iw5mp_xenon.pdb/BIOS_9_SCRATCH/<unnamed-type-bitfields> - /iw5mp_xenon.pdb/D1SCL_COEF_RAM_SELECT/<unnamed-type-bitfields> */

union BIOS_9_SCRATCH {
    struct <unnamed-type-bitfields> bitfields;
    struct <unnamed-type-bitfields> bits;
    uint u32All;
    int i32All;
    float f32All;
};

typedef enum _FILE_INFO_BY_HANDLE_CLASS {
    FileBasicInfo=0,
    FileStandardInfo=1,
    FileNameInfo=2,
    FileRenameInfo=3,
    FileDispositionInfo=4,
    FileAllocationInfo=5,
    FileEndOfFileInfo=6,
    FileStreamInfo=7,
    FileCompressionInfo=8,
    FileAttributeTagInfo=9,
    FileIdBothDirectoryInfo=10,
    FileIdBothDirectoryRestartInfo=11,
    FileIoPriorityHintInfo=12,
    MaximumFileInfoByHandleClass=13
} _FILE_INFO_BY_HANDLE_CLASS;

typedef enum _PMCCounterSource {
    PMC_PB0T0=0,
    PMC_PB0T1=1,
    PMC_PB1T0=2,
    PMC_PB1T1=3,
    PMC_PB2T0=4,
    PMC_PB2T1=5,
    PMC_PMW=6,
    PMC_unknown=7,
    PMC_count=8
} _PMCCounterSource;

typedef enum <unnamed-tag_00001115> {
    SPR_TBU_W=29
} <unnamed-tag_00001115>;

typedef enum GPUDEPTHARRAY {
    GPUDEPTHARRAY_2D_ALT_DEPTH=0,
    GPUDEPTHARRAY_2D_DEPTH=1
} GPUDEPTHARRAY;

typedef enum VGT_GRP_PRIM_ORDER {
    VGT_GRP_LIST=0,
    VGT_GRP_STRIP=1,
    VGT_GRP_FAN=2,
    VGT_GRP_LOOP=3,
    VGT_GRP_POLYGON=4
} VGT_GRP_PRIM_ORDER;

typedef enum _D3DFILTER_TYPE {
    D3DFILTER_DEFAULT=0,
    D3DFILTER_CATMULL=1,
    D3DFILTER_KAISER=2,
    D3DFILTER_GAUSSIAN=3,
    D3DFILTER_MITCHELL=4,
    D3DFILTER_LANCZOS=5,
    D3DFILTER_BILINEAR=6,
    D3DFILTER_POINT=7,
    D3DFILTER_FORCE_DWORD=255
} _D3DFILTER_TYPE;

typedef struct XMALOOPREGION XMALOOPREGION, *PXMALOOPREGION;

struct XMALOOPREGION {
    ulong LoopStart;
    ulong LoopEnd;
    uchar LoopSubframeEnd;
    uchar LoopSubframeSkip;
};

typedef struct _DMN_PGO_MODLOAD _DMN_PGO_MODLOAD, *P_DMN_PGO_MODLOAD;

typedef struct HPGOMODULE__ HPGOMODULE__, *PHPGOMODULE__;

struct _DMN_PGO_MODLOAD {
    char *pszModuleName;
    struct HPGOMODULE__ *hPgoModule;
    ulong ThreadId;
};

struct HPGOMODULE__ {
    int unused;
};

typedef enum _D3DXERR {
    D3DXERR_CANNOTMODIFYINDEXBUFFER=84,
    D3DXERR_INVALIDMESH=85,
    D3DXERR_CANNOTATTRSORT=86,
    D3DXERR_SKINNINGNOTSUPPORTED=87,
    D3DXERR_TOOMANYINFLUENCES=88,
    D3DXERR_INVALIDDATA=89,
    D3DXERR_LOADEDMESHASNODATA=90,
    D3DXERR_DUPLICATENAMEDFRAGMENT=91,
    D3DXERR_MICROCODEEMPTY=104,
    D3DXERR_MICROCODETOOBIG=105,
    D3DXERR_MICROCODEUNSUPPORTEDTARGET=106,
    D3DXERR_MICROCODEOPTIMIZERFAILED=107,
    D3DXERR_MICROCODETOOMANYLEVELS=108,
    D3DXERR_MICROCODETOOMANYREGS=109,
    D3DXERR_MICROCODETOOMANYCONSTS=110,
    D3DXERR_MICROCODETOOMANYRSINSTS=111,
    D3DXERR_MICROCODETOOMANYTEXINSTS=112,
    D3DXERR_MICROCODETOOMANYALUINSTS=113,
    D3DXERR_MICROCODETOOMANYCFINSTS=114,
    D3DXERR_MICROCODETOOMANYTOTALINSTS=115,
    D3DXERR_MICROCODETOOMANYCOMPILERLITERALS=116,
    D3DXERR_MICROCODE_DEFB_CONSTANT_OUT_OF_RANGE=117,
    D3DXERR_MICROCODE_DEFI_CONSTANT_OUT_OF_RANGE=118,
    D3DXERR_MICROCODE_DEF_CONSTANT_OUT_OF_RANGE=119,
    D3DXERR_MICROCODE_DEFB_CONSTANT_OUT_OF_ALPHA_PIXEL_SHADER_RANGE=120,
    D3DXERR_MICROCODE_DEFI_CONSTANT_OUT_OF_ALPHA_PIXEL_SHADER_RANGE=121,
    D3DXERR_MICROCODE_DEF_CONSTANT_OUT_OF_ALPHA_PIXEL_SHADER_RANGE=122,
    D3DXERR_MICROCODE_DEFB_CONSTANT_OUT_OF_ALPHA_VERTEX_SHADER_RANGE=123,
    D3DXERR_MICROCODE_DEFI_CONSTANT_OUT_OF_ALPHA_VERTEX_SHADER_RANGE=124,
    D3DXERR_MICROCODE_DEF_CONSTANT_OUT_OF_ALPHA_VERTEX_SHADER_RANGE=125,
    D3DXERR_MICROCODE_DUPLICATE_DEFB_CONSTANT=126,
    D3DXERR_MICROCODE_DUPLICATE_DEFI_CONSTANT=127,
    D3DXERR_MICROCODE_DUPLICATE_DEF_CONSTANT=128
} _D3DXERR;

typedef enum GPUBLENDOP {
    GPUBLENDOP_ADD=0,
    GPUBLENDOP_SUBTRACT=1,
    GPUBLENDOP_MIN=2,
    GPUBLENDOP_MAX=3,
    GPUBLENDOP_REVSUBTRACT=4
} GPUBLENDOP;

typedef enum IMPORT_OBJECT_TYPE {
    IMPORT_OBJECT_CODE=0,
    IMPORT_OBJECT_DATA=1,
    IMPORT_OBJECT_CONST=2
} IMPORT_OBJECT_TYPE;

typedef union RBBM_INT_ACK RBBM_INT_ACK, *PRBBM_INT_ACK;


/* WARNING! conflicting data type names: /iw5mp_xenon.pdb/RBBM_INT_ACK/<unnamed-type-bitfields> - /iw5mp_xenon.pdb/D1SCL_COEF_RAM_SELECT/<unnamed-type-bitfields> */

union RBBM_INT_ACK {
    struct <unnamed-type-bitfields> bitfields;
    struct <unnamed-type-bitfields> bits;
    uint u32All;
    int i32All;
    float f32All;
};

typedef enum <unnamed-tag_0000117D> {
    SPR_ICIDR0=210
} <unnamed-tag_0000117D>;

typedef union EP_UNUSED_BAR_4 EP_UNUSED_BAR_4, *PEP_UNUSED_BAR_4;


/* WARNING! conflicting data type names: /iw5mp_xenon.pdb/EP_UNUSED_BAR_4/<unnamed-type-bitfields> - /iw5mp_xenon.pdb/D1SCL_COEF_RAM_SELECT/<unnamed-type-bitfields> */

union EP_UNUSED_BAR_4 {
    struct <unnamed-type-bitfields> bitfields;
    struct <unnamed-type-bitfields> bits;
    uint u32All;
    int i32All;
    float f32All;
};

typedef struct _OSVERSIONINFOEXW _OSVERSIONINFOEXW, *P_OSVERSIONINFOEXW;

struct _OSVERSIONINFOEXW {
    ulong dwOSVersionInfoSize;
    ulong dwMajorVersion;
    ulong dwMinorVersion;
    ulong dwBuildNumber;
    ulong dwPlatformId;
    wchar_t szCSDVersion[128];
    ushort wServicePackMajor;
    ushort wServicePackMinor;
    ushort wSuiteMask;
    uchar wProductType;
    uchar wReserved;
};

typedef enum _D3DDEGREETYPE {
    D3DDEGREE_LINEAR=1,
    D3DDEGREE_QUADRATIC=2,
    D3DDEGREE_CUBIC=3,
    D3DDEGREE_QUINTIC=5,
    D3DDEGREE_FORCE_DWORD=255
} _D3DDEGREETYPE;

typedef enum _XCONFIG_XNET_ENTRIES {
    XCONFIG_XNET_ALL=0,
    XCONFIG_XNET_DATA=1
} _XCONFIG_XNET_ENTRIES;

typedef union GPUALU_INSTRUCTION GPUALU_INSTRUCTION, *PGPUALU_INSTRUCTION;

typedef struct GPUALU_INSTRUCTION_s_0 GPUALU_INSTRUCTION_s_0, *PGPUALU_INSTRUCTION_s_0;

struct GPUALU_INSTRUCTION_s_0 {
    ulong AbsConstants:1;
    ulong VectorDestRelative:1;
    ulong VectorDest:6;
    ulong ResultExport:1;
    ulong ScalarDestRelative:1;
    ulong ScalarDest:6;
    ulong ScalarMask:4;
    ulong VectorMask:4;
    ulong ScalarOp:6;
    ulong ScalarSaturate:1;
    ulong VectorSaturate:1;
    ulong SrcCSwizzle:8;
    ulong SrcBSwizzle:8;
    ulong SrcASwizzle:8;
    ulong Constant0Relative:1;
    ulong Constant1Relative:1;
    ulong RelativeAddress:1;
    ulong Predicated:1;
    ulong PredicationCondition:1;
    ulong SrcANegate:1;
    ulong SrcBNegate:1;
    ulong SrcCNegate:1;
    ulong SrcCRegConst:8;
    ulong SrcBRegConst:8;
    ulong SrcARegConst:8;
    ulong SrcASelect:1;
    ulong SrcBSelect:1;
    ulong SrcCSelect:1;
    ulong VectorOp:5;
};

union GPUALU_INSTRUCTION {
    struct GPUALU_INSTRUCTION_s_0 _s_0;
    ulong dword[3];
};

typedef enum GPUADDRESSCLAMP {
    GPUADDRESSCLAMP_CLAMP_TO_LAST=0,
    GPUADDRESSCLAMP_CLAMP_TO_CONSTANT=1
} GPUADDRESSCLAMP;

typedef struct _DM_ODD_SPINDLE_SPEED_CHANGE_DATA _DM_ODD_SPINDLE_SPEED_CHANGE_DATA, *P_DM_ODD_SPINDLE_SPEED_CHANGE_DATA;

struct _DM_ODD_SPINDLE_SPEED_CHANGE_DATA {
    ulong CurrentSpeed;
    ulong TargetSpeed;
    long Status;
};

typedef union VC_CNTL VC_CNTL, *PVC_CNTL;


/* WARNING! conflicting data type names: /iw5mp_xenon.pdb/VC_CNTL/<unnamed-type-bitfields> - /iw5mp_xenon.pdb/D1SCL_COEF_RAM_SELECT/<unnamed-type-bitfields> */

union VC_CNTL {
    struct <unnamed-type-bitfields> bitfields;
    struct <unnamed-type-bitfields> bits;
    uint u32All;
    int i32All;
    float f32All;
};

typedef struct D3DLineTexture D3DLineTexture, *PD3DLineTexture;

struct D3DLineTexture {
    undefined field0_0x0;
    undefined field1_0x1;
    undefined field2_0x2;
    undefined field3_0x3;
    undefined field4_0x4;
    undefined field5_0x5;
    undefined field6_0x6;
    undefined field7_0x7;
    undefined field8_0x8;
    undefined field9_0x9;
    undefined field10_0xa;
    undefined field11_0xb;
    undefined field12_0xc;
    undefined field13_0xd;
    undefined field14_0xe;
    undefined field15_0xf;
    undefined field16_0x10;
    undefined field17_0x11;
    undefined field18_0x12;
    undefined field19_0x13;
    undefined field20_0x14;
    undefined field21_0x15;
    undefined field22_0x16;
    undefined field23_0x17;
    undefined field24_0x18;
    undefined field25_0x19;
    undefined field26_0x1a;
    undefined field27_0x1b;
    undefined field28_0x1c;
    undefined field29_0x1d;
    undefined field30_0x1e;
    undefined field31_0x1f;
    undefined field32_0x20;
    undefined field33_0x21;
    undefined field34_0x22;
    undefined field35_0x23;
    undefined field36_0x24;
    undefined field37_0x25;
    undefined field38_0x26;
    undefined field39_0x27;
    undefined field40_0x28;
    undefined field41_0x29;
    undefined field42_0x2a;
    undefined field43_0x2b;
    undefined field44_0x2c;
    undefined field45_0x2d;
    undefined field46_0x2e;
    undefined field47_0x2f;
    undefined field48_0x30;
    undefined field49_0x31;
    undefined field50_0x32;
    undefined field51_0x33;
};

typedef struct _DM_INPUT_BIND_AUTOMATION _DM_INPUT_BIND_AUTOMATION, *P_DM_INPUT_BIND_AUTOMATION;

struct _DM_INPUT_BIND_AUTOMATION {
    ulong dwUserIndex;
    int fBind;
};

typedef enum <unnamed-tag_00001117> {
    SPR_PVR=31
} <unnamed-tag_00001117>;

typedef enum KCONTINUE_STATUS {
    ContinueError=0,
    ContinueSuccess=1,
    ContinueProcessorReselected=2,
    ContinueNextProcessor=3
} KCONTINUE_STATUS;

typedef enum _XPROFILE_PREFERRED_COLOR_OPTIONS {
    XPROFILE_PREFERRED_COLOR_NONE=0,
    XPROFILE_PREFERRED_COLOR_BLACK=1,
    XPROFILE_PREFERRED_COLOR_WHITE=2,
    XPROFILE_PREFERRED_COLOR_YELLOW=3,
    XPROFILE_PREFERRED_COLOR_ORANGE=4,
    XPROFILE_PREFERRED_COLOR_PINK=5,
    XPROFILE_PREFERRED_COLOR_RED=6,
    XPROFILE_PREFERRED_COLOR_PURPLE=7,
    XPROFILE_PREFERRED_COLOR_BLUE=8,
    XPROFILE_PREFERRED_COLOR_GREEN=9,
    XPROFILE_PREFERRED_COLOR_BROWN=10,
    XPROFILE_PREFERRED_COLOR_SILVER=11
} _XPROFILE_PREFERRED_COLOR_OPTIONS;

typedef enum GPUVSEXPORTMODE {
    GPUVSEXPORTMODE_POSITION_ONLY=0,
    GPUVSEXPORTMODE_SPRITE=2,
    GPUVSEXPORTMODE_EDGE=3,
    GPUVSEXPORTMODE_KILL=4,
    GPUVSEXPORTMODE_SPRITE_KILL=5,
    GPUVSEXPORTMODE_EDGE_KILL=6,
    GPUVSEXPORTMODE_MULTIPASS=7
} GPUVSEXPORTMODE;

typedef enum GPUPERFEVENT_SQ {
    GPUPE_SQ_PIXEL_VECTORS_SUB=0,
    GPUPE_SQ_VERTEX_VECTORS_SUB=1,
    GPUPE_SQ_ALU0_ACTIVE_VTX_SIMD0=2,
    GPUPE_SQ_ALU1_ACTIVE_VTX_SIMD0=3,
    GPUPE_SQ_ALU0_ACTIVE_PIX_SIMD0=4,
    GPUPE_SQ_ALU1_ACTIVE_PIX_SIMD0=5,
    GPUPE_SQ_ALU0_ACTIVE_VTX_SIMD1=6,
    GPUPE_SQ_ALU1_ACTIVE_VTX_SIMD1=7,
    GPUPE_SQ_ALU0_ACTIVE_PIX_SIMD1=8,
    GPUPE_SQ_ALU1_ACTIVE_PIX_SIMD1=9,
    GPUPE_SQ_EXPORT_CYCLES=10,
    GPUPE_SQ_ALU_CST_WRITTEN=11,
    GPUPE_SQ_TEX_CST_WRITTEN=12,
    GPUPE_SQ_ALU_CST_STALL=13,
    GPUPE_SQ_ALU_TEX_STALL=14,
    GPUPE_SQ_INST_WRITTEN=15,
    GPUPE_SQ_BOOLEAN_WRITTEN=16,
    GPUPE_SQ_LOOPS_WRITTEN=17,
    GPUPE_SQ_PIXEL_SWAP_IN=18,
    GPUPE_SQ_PIXEL_SWAP_OUT=19,
    GPUPE_SQ_VERTEX_SWAP_IN=20,
    GPUPE_SQ_VERTEX_SWAP_OUT=21,
    GPUPE_SQ_ALU_VTX_INST_ISSUED=22,
    GPUPE_SQ_TEX_VTX_INST_ISSUED=23,
    GPUPE_SQ_VC_VTX_INST_ISSUED=24,
    GPUPE_SQ_CF_VTX_INST_ISSUED=25,
    GPUPE_SQ_ALU_PIX_INST_ISSUED=26,
    GPUPE_SQ_TEX_PIX_INST_ISSUED=27,
    GPUPE_SQ_VC_PIX_INST_ISSUED=28,
    GPUPE_SQ_CF_PIX_INST_ISSUED=29,
    GPUPE_SQ_ALU0_FIFO_EMPTY_SIMD0=30,
    GPUPE_SQ_ALU1_FIFO_EMPTY_SIMD0=31,
    GPUPE_SQ_ALU0_FIFO_EMPTY_SIMD1=32,
    GPUPE_SQ_ALU1_FIFO_EMPTY_SIMD1=33,
    GPUPE_SQ_ALU_NOPS=34,
    GPUPE_SQ_PRED_SKIP=35,
    GPUPE_SQ_SYNC_ALU_STALL_SIMD0_VTX=36,
    GPUPE_SQ_SYNC_ALU_STALL_SIMD1_VTX=37,
    GPUPE_SQ_SYNC_TEX_STALL_VTX=38,
    GPUPE_SQ_SYNC_VC_STALL_VTX=39,
    GPUPE_SQ_CONSTANTS_USED_SIMD0=40,
    GPUPE_SQ_CONSTANTS_SENT_SP_SIMD0=41,
    GPUPE_SQ_GPR_STALL_VTX=42,
    GPUPE_SQ_GPR_STALL_PIX=43,
    GPUPE_SQ_VTX_RS_STALL=44,
    GPUPE_SQ_PIX_RS_STALL=45,
    GPUPE_SQ_SX_PC_FULL=46,
    GPUPE_SQ_SX_EXP_BUFF_FULL=47,
    GPUPE_SQ_SX_POS_BUFF_FULL=48,
    GPUPE_SQ_INTERP_QUADS=49,
    GPUPE_SQ_INTERP_ACTIVE=50,
    GPUPE_SQ_IN_PIXEL_STALL=51,
    GPUPE_SQ_IN_VTX_STALL=52,
    GPUPE_SQ_VTX_CNT=53,
    GPUPE_SQ_VTX_16_VECTOR=54,
    GPUPE_SQ_VTX_32_VECTOR=55,
    GPUPE_SQ_VTX_48_VECTOR=56,
    GPUPE_SQ_PIXEL_16_VECTOR=57,
    GPUPE_SQ_PIXEL_32_VECTOR=58,
    GPUPE_SQ_PIXEL_48_VECTOR=59,
    GPUPE_SQ_CONSTANTS_USED_SIMD1=60,
    GPUPE_SQ_CONSTANTS_SENT_SP_SIMD1=61,
    GPUPE_SQ_SX_MEM_EXP_FULL=62,
    GPUPE_SQ_ALU0_ACTIVE_VTX_SIMD2=63,
    GPUPE_SQ_ALU1_ACTIVE_VTX_SIMD2=64,
    GPUPE_SQ_ALU0_ACTIVE_PIX_SIMD2=65,
    GPUPE_SQ_ALU1_ACTIVE_PIX_SIMD2=66,
    GPUPE_SQ_ALU0_ACTIVE_VTX_SIMD3=67,
    GPUPE_SQ_ALU1_ACTIVE_VTX_SIMD3=68,
    GPUPE_SQ_ALU0_ACTIVE_PIX_SIMD3=69,
    GPUPE_SQ_ALU1_ACTIVE_PIX_SIMD3=70,
    GPUPE_SQ_ALU0_FIFO_EMPTY_SIMD2=71,
    GPUPE_SQ_ALU1_FIFO_EMPTY_SIMD2=72,
    GPUPE_SQ_ALU0_FIFO_EMPTY_SIMD3=73,
    GPUPE_SQ_ALU1_FIFO_EMPTY_SIMD3=74,
    GPUPE_SQ_SYNC_ALU_STALL_SIMD2_VTX=75,
    GPUPE_SQ_SYNC_ALU_STALL_SIMD3_VTX=76,
    GPUPE_SQ_SYNC_ALU_STALL_SIMD0_PIX=77,
    GPUPE_SQ_SYNC_ALU_STALL_SIMD1_PIX=78,
    GPUPE_SQ_SYNC_ALU_STALL_SIMD2_PIX=79,
    GPUPE_SQ_SYNC_ALU_STALL_SIMD3_PIX=80,
    GPUPE_SQ_SYNC_TEX_STALL_PIX=81,
    GPUPE_SQ_SYNC_VC_STALL_PIX=82,
    GPUPE_SQ_CONSTANTS_USED_SIMD2=83,
    GPUPE_SQ_CONSTANTS_SENT_SP_SIMD2=84,
    GPUPE_SQ_CONSTANTS_USED_SIMD3=85,
    GPUPE_SQ_CONSTANTS_SENT_SP_SIMD3=86,
    GPUPE_SQ_ALU0_FIFO_FULL_SIMD0=87,
    GPUPE_SQ_ALU1_FIFO_FULL_SIMD0=88,
    GPUPE_SQ_ALU0_FIFO_FULL_SIMD1=89,
    GPUPE_SQ_ALU1_FIFO_FULL_SIMD1=90,
    GPUPE_SQ_ALU0_FIFO_FULL_SIMD2=91,
    GPUPE_SQ_ALU1_FIFO_FULL_SIMD2=92,
    GPUPE_SQ_ALU0_FIFO_FULL_SIMD3=93,
    GPUPE_SQ_ALU1_FIFO_FULL_SIMD3=94,
    GPUPE_VC_PERF_STATIC=95,
    GPUPE_VC_PERF_STALLED=96,
    GPUPE_VC_PERF_STARVED=97,
    GPUPE_VC_PERF_SEND=98,
    GPUPE_VC_PERF_ACTUAL_STARVED=99,
    GPUPE_PIXEL_THREAD_0_ACTIVE=100,
    GPUPE_VERTEX_THREAD_0_ACTIVE=101,
    GPUPE_PIXEL_THREAD_0_NUMBER=102,
    GPUPE_VERTEX_THREAD_0_NUMBER=103,
    GPUPE_VERTEX_EVENT_NUMBER=104,
    GPUPE_PIXEL_EVENT_NUMBER=105
} GPUPERFEVENT_SQ;

typedef enum _KOBJECTS {
    EventNotificationObject=0,
    EventSynchronizationObject=1,
    MutantObject=2,
    ProcessObject=3,
    QueueObject=4,
    SemaphoreObject=5,
    ThreadObject=6,
    Spare1Object=7,
    TimerNotificationObject=8,
    TimerSynchronizationObject=9,
    Spare2Object=10,
    Spare3Object=11,
    Spare4Object=12,
    Spare5Object=13,
    Spare6Object=14,
    Spare7Object=15,
    Spare8Object=16,
    Spare9Object=17,
    ApcObject=18,
    DpcObject=19,
    DeviceQueueObject=20,
    EventPairObject=21,
    InterruptObject=22,
    ProfileObject=23
} _KOBJECTS;

typedef struct XAUDIOSOURCEFXINIT XAUDIOSOURCEFXINIT, *PXAUDIOSOURCEFXINIT;

struct XAUDIOSOURCEFXINIT {
    struct XAUDIOFXINIT EffectHeader;
    struct XAUDIOSOURCEFORMAT Format;
    float MaxPitchShift;
    uchar MaxPacketCount;
    uchar Flags;
    void (*pfnPacketCompletionCallback)(struct XAUDIOSOURCEFXPACKETCALLBACK *);
    void (*pfnPacketLoopCallback)(struct XAUDIOSOURCEFXPACKETCALLBACK *);
};

typedef struct _XHASH_STATE_SHA1 _XHASH_STATE_SHA1, *P_XHASH_STATE_SHA1;

struct _XHASH_STATE_SHA1 {
    uchar Internal[88];
};

typedef enum GPUVERTEXFORMAT {
    GPUVERTEXFORMAT_8_8_8_8=6,
    GPUVERTEXFORMAT_2_10_10_10=7,
    GPUVERTEXFORMAT_10_11_11=16,
    GPUVERTEXFORMAT_11_11_10=17,
    GPUVERTEXFORMAT_16_16=25,
    GPUVERTEXFORMAT_16_16_16_16=26,
    GPUVERTEXFORMAT_16_16_FLOAT=31,
    GPUVERTEXFORMAT_16_16_16_16_FLOAT=32,
    GPUVERTEXFORMAT_32=33,
    GPUVERTEXFORMAT_32_32=34,
    GPUVERTEXFORMAT_32_32_32_32=35,
    GPUVERTEXFORMAT_32_FLOAT=36,
    GPUVERTEXFORMAT_32_32_FLOAT=37,
    GPUVERTEXFORMAT_32_32_32_32_FLOAT=38,
    GPUVERTEXFORMAT_32_32_32_FLOAT=57
} GPUVERTEXFORMAT;

typedef enum SC_PERFCNT_SELECT {
    SC_SR_WINDOW_VALID=0,
    SC_CW_WINDOW_VALID=1,
    SC_QM_WINDOW_VALID=2,
    SC_QPP_WINDOW_VALID=3,
    SC_ITER_WINDOW_VALID=4,
    SC_STARVED_BY_PA=5,
    SC_STARVED_BY_RCC=6,
    SC_STALLED_BY_PRIM_FF=7,
    SC_STALLED_BY_RCC=8,
    SC_STALLED_BY_BC=9,
    SC_STALLED_BY_SX=10,
    SC_STALLED_BY_SX0=11,
    SC_STALLED_BY_SX1=12,
    SC_STALLED_BY_SQ=13,
    SC_STALLED_BY_SP=14,
    SC_WAIT_FOR_R1=15,
    SC_SCISSOR_DISCARD=16,
    SC_BB_DISCARD=17,
    SC_SUPERT_PRIM_DISCARD=18,
    SC_RT_PRIM=19,
    SC_TILE_VALID=20,
    SC_TILE_PER_PRIM_H0=21,
    SC_TILE_PER_PRIM_H1=22,
    SC_TILE_PER_PRIM_H2=23,
    SC_TILE_PER_PRIM_H3=24,
    SC_TILE_PER_PRIM_H4=25,
    SC_TILE_PER_PRIM_H5=26,
    SC_TILE_PER_PRIM_H6=27,
    SC_TILE_PER_PRIM_H7=28,
    SC_TILE_PER_PRIM_H8=29,
    SC_TILE_PER_PRIM_H9=30,
    SC_TILE_PER_PRIM_H10=31,
    SC_TILE_PER_PRIM_H11=32,
    SC_TILE_PER_PRIM_H12=33,
    SC_TILE_PER_PRIM_H13=34,
    SC_TILE_PER_PRIM_H14=35,
    SC_TILE_PER_PRIM_H15=36,
    SC_TILE_PER_PRIM_H16=37,
    SC_SUPERT_TILE_DISCARD=38,
    SC_QM_NUM_QUADS=39,
    SC_QM_MASK_H0=40,
    SC_QM_MASK_H1=41,
    SC_QM_MASK_H2=42,
    SC_QM_MASK_H3=43,
    SC_QM_MASK_H4=44,
    SC_QM_MASK_H5=45,
    SC_QM_MASK_H6=46,
    SC_QM_MASK_H7=47,
    SC_QM_MASK_H8=48,
    SC_QM_MASK_H9=49,
    SC_QM_MASK_H10=50,
    SC_QM_MASK_H11=51,
    SC_QM_MASK_H12=52,
    SC_QM_MASK_H13=53,
    SC_QM_MASK_H14=54,
    SC_QM_MASK_H15=55,
    SC_QM_MASK_H16=56,
    SC_QM_COVERED_H0=57,
    SC_QM_COVERED_H1=58,
    SC_QM_COVERED_H2=59,
    SC_QM_COVERED_H3=60,
    SC_QM_COVERED_H4=61,
    SC_QM_COVERED_H5=62,
    SC_QM_COVERED_H6=63,
    SC_QM_COVERED_H7=64,
    SC_QM_COVERED_H8=65,
    SC_QM_COVERED_H9=66,
    SC_QM_COVERED_H10=67,
    SC_QM_COVERED_H11=68,
    SC_QM_COVERED_H12=69,
    SC_QM_COVERED_H13=70,
    SC_QM_COVERED_H14=71,
    SC_QM_COVERED_H15=72,
    SC_QM_COVERED_H16=73,
    SC_HIER_NUM_QUADS=74,
    SC_HIER_MASK_H0=75,
    SC_HIER_MASK_H1=76,
    SC_HIER_MASK_H2=77,
    SC_HIER_MASK_H3=78,
    SC_HIER_MASK_H4=79,
    SC_HIER_MASK_H5=80,
    SC_HIER_MASK_H6=81,
    SC_HIER_MASK_H7=82,
    SC_HIER_MASK_H8=83,
    SC_HIER_MASK_H9=84,
    SC_HIER_MASK_H10=85,
    SC_HIER_MASK_H11=86,
    SC_HIER_MASK_H12=87,
    SC_HIER_MASK_H13=88,
    SC_HIER_MASK_H14=89,
    SC_HIER_MASK_H15=90,
    SC_HIER_MASK_H16=91,
    SC_DET_NUM_QUADS=92,
    SC_PKR_QD_PER_ROW_H1=93,
    SC_PKR_QD_PER_ROW_H2=94,
    SC_PKR_QD_PER_ROW_H3=95,
    SC_PKR_QD_PER_ROW_H4=96,
    SC_PKR_END_OF_VECTOR=97,
    SC_PKR_ONE_CLK=98,
    SC_QD_WITH_1_PIX=99,
    SC_QD_WITH_2_PIX=100,
    SC_QD_WITH_3_PIX=101,
    SC_QD_WITH_4_PIX=102,
    SC_SR_WINDOW_VALID_BUSY=103,
    SC_CW_WINDOW_VALID_BUSY=104,
    SC_QM_WINDOW_VALID_BUSY=105,
    SC_QPP_WINDOW_VALID_BUSY=106,
    SC_ITER_WINDOW_VALID_BUSY=107
} SC_PERFCNT_SELECT;

typedef enum MC_PERFCNT_SELECT {
    RANK_BANK0_ACCESSES=0,
    RANK_BANK1_ACCESSES=1,
    RANK_BANK2_ACCESSES=2,
    RANK_BANK3_ACCESSES=3,
    RANK_BANK4_ACCESSES=4,
    RANK_BANK5_ACCESSES=5,
    RANK_BANK6_ACCESSES=6,
    RANK_BANK7_ACCESSES=7,
    RANK_BANK8_ACCESSES=8,
    RANK_BANK9_ACCESSES=9,
    RANK_BANK10_ACCESSES=10,
    RANK_BANK11_ACCESSES=11,
    RANK_BANK12_ACCESSES=12,
    RANK_BANK13_ACCESSES=13,
    RANK_BANK14_ACCESSES=14,
    RANK_BANK15_ACCESSES=15,
    READ_2_WRITE=16,
    WRITE_2_READ=17,
    NEW_PAGE_ACCESSES=18,
    TOTAL_ACCESSES=19,
    READ_ACCESSES=20,
    ACCESS_PRESENT_NO_ISSUE_CLKS=21,
    CMD_PRESENT_NO_XFER_CLKS=22,
    URGENT_DC_ACCESSES=23,
    URGENT_SB_ACCESSES=24,
    URGENT_BIU_ACCESSES=25,
    NEW_BIUF_ACCESSES=26,
    NEW_CP_ACCESSES=27,
    NEW_TC_ACCESSES=28,
    NEW_VC_ACCESSES=29,
    NEW_BC_CP_ACCESSES=30,
    NEW_BC_EX_ACCESSES=31,
    NEW_VGT_ACCESSES=32,
    NEW_DC_ACCESSES=33,
    NEW_SB_ACCESSES=34,
    NEW_BIUS_ACCESSES=35,
    BIUS_READ_ACCESSES=36,
    SB_READ_ACCESSES=37,
    CP_READ_ACCESSES=38,
    NTH_SMPG_ACCESS_IS_BIUF=39,
    NTH_SMPG_ACCESS_IS_CP=40,
    NTH_SMPG_ACCESS_IS_TC=41,
    NTH_SMPG_ACCESS_IS_VC=42,
    NTH_SMPG_ACCESS_IS_BC_CP=43,
    NTH_SMPG_ACCESS_IS_BC_EX=44,
    NTH_SMPG_ACCESS_IS_VGT=45,
    NTH_SMPG_ACCESS_IS_DC=46,
    NTH_SMPG_ACCESS_IS_SB=47,
    NTH_SMPG_ACCESS_IS_BIUS=48,
    N_VALID_ENTRY_IN_TEXTURE_CAM_CLKS=49,
    N_VALID_ENTRY_IN_VC_CAM_CLKS=50,
    N_VALID_ENTRY_IN_BIUS_CAM_CLKS=51,
    N_SB_BUF_USED_CLKS=52,
    N_TC_BUF_USED_CLKS=53,
    N_VC_BUF_USED_CLKS=54,
    N_DC_BUF_USED_CLKS=55,
    N_VGT_BUF_USED_CLKS=56,
    N_CP_BUF_USED_CLKS=57
} MC_PERFCNT_SELECT;

typedef enum BIUPERFEVENT {
    BIUPE_RECV_REQUESTS=0,
    BIUPE_RECV_RESPONSES=1,
    BIUPE_IOC_READS=2,
    BIUPE_IOC_WRITES=3,
    BIUPE_MEM_READS=4,
    BIUPE_MEM_WRITES=5,
    BIUPE_FSB_SYNCS=6,
    BIUPE_EIEIOS=7,
    BIUPE_EOIS=8,
    BIUPE_FLUSH_ACKS=9,
    BIUPE_REQUEST_READ_DATA_BYTES_8_BYTE_AND_LESS_TRANSFERS_DEPENDENT_ON_SECONDARY_CONTROL_REGISTER=10,
    BIUPE_REQUEST_READ_DATA_BYTES_16_BYTE_AND_MORE_TRANSFERS_DEPENDENT_ON_SECONDARY_CONTROL_REGISTER=11,
    BIUPE_REQUEST_WRITE_DATA_BYTES_8_BYTE_AND_LESS_TRANSFERS_DEPENDENT_ON_SECONDARY_CONTROL_REGISTER=12,
    BIUPE_REQUEST_WRITE_DATA_BYTES_16_BYTE_AND_MORE_TRANSFER_DEPENDENT_ON_SECONDARY_CONTROL_REGISTER=13,
    BIUPE_RESERVED0=14,
    BIUPE_XMIT_REQUESTS=15,
    BIUPE_XMIT_RESPONSES=16,
    BIUPE_READ128S=17,
    BIUPE_FLUSH_REQ_READS=18,
    BIUPE_FLUSH_REQ_WRITES=19,
    BIUPE_CPUWB=20,
    BIUPE_INTERRUPTS=21,
    BIUPE_RESPONSES_MC0=22,
    BIUPE_RESPONSES_MC1=23,
    BIUPE_RESPONSES_IOC_MST=24,
    BIUPE_MC0_RESPONSE_READ_DATA_BYTES_8_BYTE_AND_LESS_TRANSFERS_DEPENDENT_ON_SECONDARY_CONTROL_REGISTER_=25,
    BIUPE_MC0_RESPONSE_READ_DATA_BYTES_16_BYTE_AND_MORE_TRANSFERS_DEPENDENT_ON_SECONDARY_CONTROL_REGISTER_=26,
    BIUPE_MC1_RESPONSE_READ_DATA_BYTES_8_BYTE_AND_LESS_TRANSFERS_DEPENDENT_ON_SECONDARY_CONTROL_REGISTER_=27,
    BIUPE_MC1_RESPONSE_READ_DATA_BYTES_16_BYTE_AND_MORE_TRANSFERS_DEPENDENT_ON_SECONDARY_CONTROL_REGISTER_=28,
    BIUPE_IOC_MST_RESPONSE_READ_DATA_BYTES_8_BYTE_AND_LESS_TRANSFERS_DEPENDENT_ON_SECONDARY_CONTROL_REGISTER_=29,
    BIUPE_IOC_MST_RESPONSE_READ_DATA_BYTES_16_BYTE_AND_MORE_TRANSFERS_DEPENDENT_ON_SECONDARY_CONTROL_REGISTER_=30,
    BIUPE_RESERVED1A=31,
    BIUPE_RESERVED2A=32,
    BIUPE_RESERVED3A=33,
    BIUPE_RESERVED4A=34,
    BIUPE_RESERVED5A=35,
    BIUPE_RESERVED6A=36,
    BIUPE_RESERVED7A=37,
    BIUPE_RESERVED8A=38,
    BIUPE_RESERVED9A=39,
    BIUPE_REQUESTS=40,
    BIUPE_IOC_MAST_READS=41,
    BIUPE_IOC_MAST_WRITES=42,
    BIUPE_IOC_MAST_EOIS=43,
    BIUPE_IOC_MAST_REQUEST_READ_DATA_BYTES_8_BYTE_AND_LESS_TRANSFERS_DEPENDENT_ON_SECONDARY_CONTROL_REGISTER=44,
    BIUPE_IOC_MAST_REQUEST_READ_DATA_BYTES_16_BYTE_AND_MORE_TRANSFERS_DEPENDENT_ON_SECONDARY_CONTROL_REGISTER=45,
    BIUPE_IOC_MAST_REQUEST_WRITE_DATA_BYTES_8_BYTE_AND_LESS_TRANSFERS_DEPENDENT_ON_SECONDARY_CONTROL_REGISTER=46,
    BIUPE_IOC_MAST_REQUEST_WRITE_DATA_BYTES_16_BYTE_AND_MORE_TRANSFER_DEPENDENT_ON_SECONDARY_CONTROL_REGISTER=47,
    BIUPE_RESERVED10=48,
    BIUPE_RESERVED11=49,
    BIUPE_IOC_SLV_REQUESTS=50,
    BIUPE_IOC_SLV_SNOOP_READS=51,
    BIUPE_IOC_SLV_SNOOP_WRITES=52,
    BIUPE_IOC_SLV_INTERRUPTS=53,
    BIUPE_IOC_SLV_SYNCS=54,
    BIUPE_IOC_SLV_SNOOP_READS_NS=55,
    BIUPE_IOC_SLV_SNOOP_WRITES_NS=56,
    BIUPE_IOC_SLV_CPUWBS=57,
    BIUPE_IOC_SLV_REQUEST_READ_DATA_BYTES_DIVIDED_BY_4_DEPENDENT_ON_SECONDARY_CONTROL_REGISTER_=58,
    BIUPE_IOC_SLV_REQUEST_WRITE_DATA_BYTES_DIVIDED_BY_4_DEPENDENT_ON_SECONDARY_CONTROL_REGISTER_=59,
    BIUPE_IOC_SLV_RESPONSES_BEATS_=60,
    BIUPE_IOC_SLV_RESP_READ_DATA_BYTES_DIVIDED_BY_4_=61,
    BIUPE_IOC_SLV_SYNCCLEANS=62,
    BIUPE_RESERVED12=63,
    BIUPE_RESERVED13=64,
    BIUPE_RESERVED14=65,
    BIUPE_RESERVED15=66,
    BIUPE_RESERVED16=67,
    BIUPE_RESERVED17=68,
    BIUPE_RESERVED18=69,
    BIUPE_MC_0_FAST_PATH_CPU_READS=70,
    BIUPE_MC_0_ORDERED_CPU_PATH_READS=71,
    BIUPE_MC_0_ORDERED_CPU_PATH_WRITES=72,
    BIUPE_MC_0_ORDERED_IOC_PATH_READS=73,
    BIUPE_MC_0_ORDERED_IOC_PATH_WRITES=74,
    BIUPE_MC_0_SYNC_A=75,
    BIUPE_MC_0_SYNC_B=76,
    BIUPE_RESERVED19=77,
    BIUPE_RESERVED20=78,
    BIUPE_RESERVED21=79,
    BIUPE_MC_1_FAST_PATH_CPU_READS=80,
    BIUPE_MC_1_ORDERED_CPU_PATH_READS=81,
    BIUPE_MC_1_ORDERED_CPU_PATH_WRITES=82,
    BIUPE_MC_1_ORDERED_IOC_PATH_READS=83,
    BIUPE_MC_1_ORDERED_IOC_PATH_WRITES=84,
    BIUPE_MC_1_SYNC_A=85,
    BIUPE_MC_1_SYNC_B=86,
    BIUPE_MCQF_NEARFULL=100,
    BIUPE_MCQDF_NEARFULL=101,
    BIUPE_IMQF_NEARFULL=102,
    BIUPE_PSF_NEARFULL=103,
    BIUPE_M0SF_NEARFULL=104,
    BIUPE_M1SF_NEARFULL=105,
    BIUPE_IMSF_NEARFULL=106,
    BIUPE_IMSDF_NEARFULL=107,
    BIUPE_ISQF_NEARFULL=108,
    BIUPE_ISSF_NEARFULL=109,
    BIUPE_ISYF_NEARFULL=110,
    BIUPE_CPYF_NEARFULL=111,
    BIUPE_PQF_NEARFULL=112,
    BIUPE_TOTAL_MEMORY_LATENCY_FAST_READS_MUST_BE_DIVIDED_BY_MEM_READ_REQUESTS_FOR_AVERAGE_LATENCY=140,
    BIUPE_TOTAL_MEMORY_LATENCY_ORDERED_CPU_READS_MUST_BE_DIVIDED_BY_MEM_READ_REQUESTS_FOR_AVERAGE_LATENCY=141,
    BIUPE_TOTAL_MEMORY_LATENCY_ORDERED_IOC_READS_MUST_BE_DIVIDED_BY_MEM_READ_REQUESTS_FOR_AVERAGE_LATENCY=142,
    BIUPE_TOTAL_FLUSH_LATENCY_FOR_SNOOP_READS_MUST_BE_DIVIED_BY_FLUSH_REQUESTS_FOR_AVERAGE_LATENCY=143,
    BIUPE_TOTAL_FLUSH_LATENCY_FOR_SNOOP_WRITES_MUST_BE_DIVIED_BY_FLUSH_REQUESTS_FOR_AVERAGE_LATENCY=144,
    BIUPE_SNOOP_COMPLETION_BUFFER_FULL=145,
    BIUPE_RESERVED62=146,
    BIUPE_RESERVED63=147,
    BIUPE_RESERVED64=148,
    BIUPE_RESERVED65=149,
    BIUPE_MC0_SLOW_CREDIT_COUNT_IS_ZERO=150,
    BIUPE_MC1_SLOW_CREDIT_COUNT_IS_ZERO=151,
    BIUPE_MC0_FAST_CREDIT_COUNT_IS_ZERO=152,
    BIUPE_MC1_FAST_CREDIT_COUNT_IS_ZERO=153,
    BIUPE_RESERVED66=154,
    BIUPE_RESERVED67=155,
    BIUPE_FSB_SLV_REQ_STALL_FOR_COMMAND_CREDITS=156,
    BIUPE_FSB_SLV_REQ_STALL_FOR_DATA_CREDITS=157,
    BIUPE_FSB_PG_REQ_STALL_FOR_CMD_CREDITS=158,
    BIUPE_SCLK_COUNTS_SCLKS_IN_THE_MCLK_DOMAIN_=200,
    BIUPE_IOC_MASTER_REQ_STALL_BIU_HAS_DATA_BUT_IOC_NOT_READY_SCLK_DOMAIN_=201,
    BIUPE_IOC_SLAVE_RSP_STALL_SCLK_DOMAIN_=202,
    BIUPE_INCREMENT_BY_0_DISABLE_COUNT=252,
    BIUPE_INCREMENT_BY_1_USED_TO_COUNT_CLOCKS=253,
    BIUPE_INCREMENT_BY_63_USED_FOR_SIMULATION_TO_RAPIDLY_INCREMENT_COUNTER=254,
    BIUPE_OTHERS=255
} BIUPERFEVENT;

typedef struct _D3DRASTER_STATUS _D3DRASTER_STATUS, *P_D3DRASTER_STATUS;

struct _D3DRASTER_STATUS {
    int InVBlank;
    uint ScanLine;
};

typedef enum _PixDataOpcodesType {
    PIXOP_Clear=0,
    PIXOP_DrawVert=1,
    PIXOP_DrawVertUP=2,
    PIXOP_DrawIVert=3,
    PIXOP_DrawIVertUP=4,
    PIXOP_Swap=5,
    PIXOP_VBlank=6,
    PIXOP_VBlankMissed=7,
    PIXOP_VBlankSwap=8,
    PIXOP_ActiveResourceLock=9,
    PIXOP_ResourceLock=10,
    PIXOP_PrimaryOverrun=11,
    PIXOP_SecondaryOverrun=12,
    PIXOP_BlockOnTime=13,
    PIXOP_BlockUntilIdle=14,
    PIXOP_SwapThrottle=15,
    PIXOP_LockVertex=16,
    PIXOP_LockIndex=17,
    PIXOP_LockTexture=18,
    PIXOP_LockSurface=19,
    PIXOP_UserEvent=20,
    PIXOP_UserMarker=21,
    PIXOP_UserString=22,
    PIXOP_Resolve=23,
    PIXOP_Last=24,
    PIXOP_QPCFreq=25,
    PIXOP_SingleR300=26,
    PIXOP_CModel=27,
    PIXOP_CModelFrame=28,
    PIXOP_CModelStatsColumn=29,
    PIXOP_CModelImage=30,
    PIXOP_CModelCompressedImage=31,
    PIXOP_CModelData=32,
    PIXOP_ClearF=33,
    PIXOP_CModelRenderData=34,
    PIXOP_EmulatorImageNative=35,
    PIXOP_EmulatorCompressedImageNative=36,
    PIXOP_XPS=37,
    PIXOP_BlockOnResourceRelease=38,
    PIXOP_BlockOnVertexBufferRelease=39,
    PIXOP_BlockOnIndexBufferRelease=40,
    PIXOP_BlockOnTextureRelease=41,
    PIXOP_BlockOnSurfaceRelease=42,
    PIXOP_BlockOnVertexShaderRelease=43,
    PIXOP_BlockOnPixelShaderRelease=44,
    PIXOP_BlockOnResourceLock=45,
    PIXOP_BlockOnVertexBufferLock=46,
    PIXOP_BlockOnIndexBufferLock=47,
    PIXOP_BlockOnTextureLock=48,
    PIXOP_BlockOnResourceBusy=49,
    PIXOP_SysMon=50,
    PIXOP_LockVolume=51,
    PIXOP_GPU=52,
    PIXOP_DrawTessVertices=53,
    PIXOP_DrawITessVertices=54,
    PIXOP_GPUSync=55,
    PIXOP_Kickoff=56,
    PIXOP_Synchronize=57,
    PIXOP_BeginVertices=58,
    PIXOP_SyncToVBlank=59,
    PIXOP_GPUBegin=60,
    PIXOP_PIXOverhead=61,
    PIXOP_PIXThreadPage0=62,
    PIXOP_PIXThreadPage1=63,
    PIXOP_PIXThreadPage2=64,
    PIXOP_PIXThreadPage3=65,
    PIXOP_PIXThreadPage4=66,
    PIXOP_PIXThreadPage5=67,
    PIXOP_PIXThreadedTiming=68,
    PIXOP_ContextSwap=69,
    PIXOP_InitialThreads=70,
    PIXOP_XAMCommandBuffer=71,
    PIXOP_Truncated=72,
    PIXOP_BeginIVertices=73,
    PIXOP_ThreadName=74,
    PIXOP_BatchCommands=75,
    PIXOP_TimeStamp=76,
    PIXOP_ImageBufferHeader=77,
    PIXOP_ImageHeaderName=78,
    PIXOP_EDRAMClear=79,
    PIXOP_InsertCallback=80,
    PIXOP_SetRenderTarget=81,
    PIXOP_BeginTiling=82,
    PIXOP_EndTiling=83,
    PIXOP_BeginZPass=84,
    PIXOP_EndZPass=85,
    PIXOP_TileCount=86,
    PIXOP_BlockOnCommandBufferRelease=87,
    PIXOP_BlockOnCommandBufferLock=88,
    PIXOP_BlockOnConstantBufferRelease=89,
    PIXOP_BlockOnConstantBufferLock=90,
    PIXOP_LockConstantBuffer=91,
    PIXOP_OnePassTileCount=92,
    PIXOP_UpdbData=93,
    PIXOP_DrPixCounterData=94,
    PIXOP_Chunk=95,
    PIXOP_Timebase=96,
    PIXOP_SysMonState=97,
    PIXOP_DrPixRawCounterData=98,
    PIXOP_FrameTimestamp=99,
    PIXOP_MissingCounterData=100,
    PIXOP_UserCounter=101,
    PIXOP_ModuleInfo=102,
    PIXOP_ModulePdbInfo=103,
    PIXOP_InsertBlockOnAsyncResources=104,
    PIXOP_RawDataFile=105,
    PIXOP_MovieGammaRamp=106,
    PIXOP_MovieVideoData=107,
    PIXOP_MovieAudioData=108,
    PIXOP_MovieRestart=109,
    PIXOP_Nop=110,
    PIXOP_ResourceCapture=111,
    PIXOP_ResourceThumbnailDescription=112,
    PIXOP_ResourceThumbnailData=113,
    PIXOP_ResourceEndAutoSave=114,
    PIXOP_ResourceTextureNameGroup=115,
    PIXOP_ResourceTextureDimensionsGroup=116,
    PIXOP_ResourceNewTextureUsageInfo=117,
    PIXOP_ResourceUpdateTextureUsageInfo=118,
    PIXOP_ResourceNewUnusedTextureInfo=119,
    PIXOP_ResourceUpdateUnusedTextureInfo=120,
    PIXOP_ResourceError=121,
    PIXOP_ResourceTextureNameAddr=122,
    PIXOP_ResourceNewResolveTargetInfo=123,
    PIXOP_ResourceUpdateResolveTargetInfo=124,
    PIXOP_ResourceScreenshotDescription=125,
    PIXOP_ResourceScreenshotData=126,
    PIXOP_ResourceTimeStampStart=127,
    PIXOP_BlockOnAsyncCommandBuffer=128,
    PIXOP_InsertAsyncCommandBufferCall=129,
    PIXOP_FixupAndSignalAsyncCommandBufferCall=130,
    PIXOP_RunCommandBuffer=131,
    PIXOP_BlockOnAsyncCBAlloc=132,
    PIXOP_BlockOnAsyncCBCallRelease=133,
    PIXOP_BlockOnAsyncCBCallReset=134,
    PIXOP_BlockOnAsyncCBCallBlockAlloc=135,
    PIXOP_BlockOnPerfCounterBatchLock=136,
    PIXOP_BlockOnPerfCounterBatchRelease=137,
    PIXOP_BlockOnOcclusionQueryBatchLock=138,
    PIXOP_BlockOnOcclusionQueryBatchRelease=139
} _PixDataOpcodesType;

typedef enum <unnamed-tag_00001111> {
    SPR_TBL_W=28
} <unnamed-tag_00001111>;

typedef enum _KDRIVER_NOTIFICATION_TYPE {
    DriverQuiesceRundown=0,
    DriverQuiesceStartup=1,
    DriverShutdown=2
} _KDRIVER_NOTIFICATION_TYPE;

typedef enum <unnamed-tag_000011B3> {
    SPR_PIR=255
} <unnamed-tag_000011B3>;

typedef enum GPUPIXCENTER {
    GPUPIXCENTER_ZERO=0,
    GPUPIXCENTER_HALF=1
} GPUPIXCENTER;

typedef enum _XCONFIG_STATISTIC_ENTRIES {
    XCONFIG_STATISTICS_DATA=0,
    XCONFIG_STATISTICS_XUID_MAC_ADDRESS=1,
    XCONFIG_STATISTICS_XUID_COUNT=2,
    XCONFIG_STATISTICS_ODD_FAILURES=3,
    XCONFIG_STATISTICS_HDD_SMART_DATA=4,
    XCONFIG_STATISTICS_UEM_ERRORS=5,
    XCONFIG_STATISTICS_FPM_ERRORS=6,
    XCONFIG_STATISTICS_LAST_REPORT_TIME=7,
    XCONFIG_STATISTICS_BUG_CHECK_DATA=8,
    XCONFIG_STATISTICS_TEMPERATURE=9
} _XCONFIG_STATISTIC_ENTRIES;

typedef enum ColorformatX {
    COLORX_8_8_8_8=0,
    COLORX_8_8_8_8_GAMMA=1,
    COLORX_2_10_10_10=2,
    COLORX_2_10_10_10_FLOAT=3,
    COLORX_16_16=4,
    COLORX_16_16_16_16=5,
    COLORX_16_16_FLOAT=6,
    COLORX_16_16_16_16_FLOAT=7,
    COLORX_2_10_10_10_AS_10_10_10_10=10,
    COLORX_2_10_10_10_FLOAT_AS_16_16_16_16=12,
    COLORX_32_FLOAT=14,
    COLORX_32_32_FLOAT=15
} ColorformatX;

typedef enum _D3DTEXTUREOP {
    D3DTOP_DISABLE=1,
    D3DTOP_SELECTARG1=2,
    D3DTOP_SELECTARG2=3,
    D3DTOP_MODULATE=4,
    D3DTOP_MODULATE2X=5,
    D3DTOP_MODULATE4X=6,
    D3DTOP_ADD=7,
    D3DTOP_ADDSIGNED=8,
    D3DTOP_ADDSIGNED2X=9,
    D3DTOP_SUBTRACT=10,
    D3DTOP_ADDSMOOTH=11,
    D3DTOP_BLENDDIFFUSEALPHA=12,
    D3DTOP_BLENDTEXTUREALPHA=13,
    D3DTOP_BLENDFACTORALPHA=14,
    D3DTOP_BLENDTEXTUREALPHAPM=15,
    D3DTOP_BLENDCURRENTALPHA=16,
    D3DTOP_PREMODULATE=17,
    D3DTOP_MODULATEALPHA_ADDCOLOR=18,
    D3DTOP_MODULATECOLOR_ADDALPHA=19,
    D3DTOP_MODULATEINVALPHA_ADDCOLOR=20,
    D3DTOP_MODULATEINVCOLOR_ADDALPHA=21,
    D3DTOP_BUMPENVMAP=22,
    D3DTOP_BUMPENVMAPLUMINANCE=23,
    D3DTOP_DOTPRODUCT3=24,
    D3DTOP_MULTIPLYADD=25,
    D3DTOP_LERP=26,
    D3DTOP_MAX=27,
    D3DTOP_FORCE_DWORD=255
} _D3DTEXTUREOP;

typedef enum <unnamed-tag_00001135> {
    SPR_HRMOR=57
} <unnamed-tag_00001135>;

typedef enum CopyFmtCommand {
    RAW=0,
    CONV_FORMAT=1,
    U1U1U1U1=2,
    NULL_COPY=3
} CopyFmtCommand;

typedef enum _KWAIT_REASON {
    Executive=0,
    DelayExecution=1,
    Suspended=2,
    UserRequest=3,
    WrQueue=4,
    WrFsCacheIn=5,
    WrFsCacheOut=6,
    WrReadWriteLock=7,
    WrCriticalSection=8,
    MaximumWaitReason=9
} _KWAIT_REASON;

typedef struct D3DVertexShader D3DVertexShader, *PD3DVertexShader;

struct D3DVertexShader {
    undefined field0_0x0;
    undefined field1_0x1;
    undefined field2_0x2;
    undefined field3_0x3;
    undefined field4_0x4;
    undefined field5_0x5;
    undefined field6_0x6;
    undefined field7_0x7;
    undefined field8_0x8;
    undefined field9_0x9;
    undefined field10_0xa;
    undefined field11_0xb;
    undefined field12_0xc;
    undefined field13_0xd;
    undefined field14_0xe;
    undefined field15_0xf;
    undefined field16_0x10;
    undefined field17_0x11;
    undefined field18_0x12;
    undefined field19_0x13;
    undefined field20_0x14;
    undefined field21_0x15;
    undefined field22_0x16;
    undefined field23_0x17;
};

typedef enum _STF_CONTROL_CODE {
    StfLockVolume=0,
    StfUnlockVolume=1,
    StfFlushDirtyBuffers=2,
    StfBuildVolumeDescriptor=3,
    StfResetWriteState=4,
    StfReadPersistentStatus=5
} _STF_CONTROL_CODE;

typedef enum _IO_PRIORITY {
    IoPriorityLow=0,
    IoPriorityBelowNormal=1,
    IoPriorityNormal=2,
    IoPriorityAboveNormal=3,
    IoPriorityHigh=4,
    IoPriorityCritical=5,
    IoPriorityMaximum=6
} _IO_PRIORITY;

typedef struct D3DVertexBuffer D3DVertexBuffer, *PD3DVertexBuffer;

struct D3DVertexBuffer {
    undefined field0_0x0;
    undefined field1_0x1;
    undefined field2_0x2;
    undefined field3_0x3;
    undefined field4_0x4;
    undefined field5_0x5;
    undefined field6_0x6;
    undefined field7_0x7;
    undefined field8_0x8;
    undefined field9_0x9;
    undefined field10_0xa;
    undefined field11_0xb;
    undefined field12_0xc;
    undefined field13_0xd;
    undefined field14_0xe;
    undefined field15_0xf;
    undefined field16_0x10;
    undefined field17_0x11;
    undefined field18_0x12;
    undefined field19_0x13;
    undefined field20_0x14;
    undefined field21_0x15;
    undefined field22_0x16;
    undefined field23_0x17;
    union GPUVERTEX_FETCH_CONSTANT Format;
};

typedef enum SurfaceNumberX {
    NUMBERX_UREPEAT=0,
    NUMBERX_SREPEAT=1,
    NUMBERX_UINTEGER=2,
    NUMBERX_SINTEGER=3,
    NUMBERX_FLOAT=7
} SurfaceNumberX;

typedef enum GPUINDEXTYPE {
    GPUINDEXTYPE_16BIT=0,
    GPUINDEXTYPE_32BIT=1
} GPUINDEXTYPE;

typedef enum _XEKEY_INDEX {
    XEKEY_MANUFACTURING_MODE=0,
    XEKEY_SECURED_DATA_BASE=0,
    XEKEY_SECURED_DATA_LIMIT=0,
    XEKEY_ALTERNATE_KEY_VAULT=1,
    XEKEY_RESTRICTED_PRIVILEGES_FLAGS=2,
    XEKEY_RESERVED_BYTE3=3,
    XEKEY_ODD_FEATURES=4,
    XEKEY_ODD_AUTHTYPE=5,
    XEKEY_RESTRICTED_HVEXT_LOADER=6,
    XEKEY_POLICY_FLASH_SIZE=7,
    XEKEY_POLICY_BUILTIN_USBMU_SIZE=8,
    XEKEY_RESERVED_DWORD4=9,
    XEKEY_RESTRICTED_PRIVILEGES=10,
    XEKEY_RESERVED_QWORD2=11,
    XEKEY_RESERVED_QWORD3=12,
    XEKEY_RESERVED_QWORD4=13,
    XEKEY_RESERVED_KEY1=14,
    XEKEY_RESERVED_KEY2=15,
    XEKEY_RESERVED_KEY3=16,
    XEKEY_RESERVED_KEY4=17,
    XEKEY_RESERVED_RANDOM_KEY1=18,
    XEKEY_RESERVED_RANDOM_KEY2=19,
    XEKEY_CONSOLE_SERIAL_NUMBER=20,
    XEKEY_MOBO_SERIAL_NUMBER=21,
    XEKEY_GAME_REGION=22,
    XEKEY_CONSOLE_OBFUSCATION_KEY=23,
    XEKEY_KEY_OBFUSCATION_KEY=24,
    XEKEY_ROAMABLE_OBFUSCATION_KEY=25,
    XEKEY_DVD_KEY=26,
    XEKEY_PRIMARY_ACTIVATION_KEY=27,
    XEKEY_SECONDARY_ACTIVATION_KEY=28,
    XEKEY_GLOBAL_DEVICE_2DES_KEY1=29,
    XEKEY_GLOBAL_DEVICE_2DES_KEY2=30,
    XEKEY_WIRELESS_CONTROLLER_MS_2DES_KEY1=31,
    XEKEY_WIRELESS_CONTROLLER_MS_2DES_KEY2=32,
    XEKEY_WIRED_WEBCAM_MS_2DES_KEY1=33,
    XEKEY_WIRED_WEBCAM_MS_2DES_KEY2=34,
    XEKEY_WIRED_CONTROLLER_MS_2DES_KEY1=35,
    XEKEY_WIRED_CONTROLLER_MS_2DES_KEY2=36,
    XEKEY_MEMORY_UNIT_MS_2DES_KEY1=37,
    XEKEY_MEMORY_UNIT_MS_2DES_KEY2=38,
    XEKEY_OTHER_XSM3_DEVICE_MS_2DES_KEY1=39,
    XEKEY_OTHER_XSM3_DEVICE_MS_2DES_KEY2=40,
    XEKEY_WIRELESS_CONTROLLER_3P_2DES_KEY1=41,
    XEKEY_WIRELESS_CONTROLLER_3P_2DES_KEY2=42,
    XEKEY_WIRED_WEBCAM_3P_2DES_KEY1=43,
    XEKEY_WIRED_WEBCAM_3P_2DES_KEY2=44,
    XEKEY_WIRED_CONTROLLER_3P_2DES_KEY1=45,
    XEKEY_WIRED_CONTROLLER_3P_2DES_KEY2=46,
    XEKEY_MEMORY_UNIT_3P_2DES_KEY1=47,
    XEKEY_MEMORY_UNIT_3P_2DES_KEY2=48,
    XEKEY_OTHER_XSM3_DEVICE_3P_2DES_KEY1=49,
    XEKEY_OTHER_XSM3_DEVICE_3P_2DES_KEY2=50,
    XEKEY_CONSOLE_PRIVATE_KEY=51,
    XEKEY_XEIKA_PRIVATE_KEY=52,
    XEKEY_CARDEA_PRIVATE_KEY=53,
    XEKEY_CONSOLE_CERTIFICATE=54,
    XEKEY_XEIKA_CERTIFICATE=55,
    XEKEY_CARDEA_CERTIFICATE=56,
    XEKEY_CONSTANT_PIRS_KEY=57,
    XEKEY_MAX_KEY_INDEX=57,
    XEKEY_CONSTANT_ALT_MASTER_KEY=58,
    XEKEY_CONSTANT_ALT_LIVE_KEY=59,
    XEKEY_CONSTANT_MASTER_KEY=60,
    XEKEY_CONSTANT_LIVE_KEY=61,
    XEKEY_CONSTANT_XB1_GREEN_KEY=62,
    XEKEY_CONSTANT_SATA_DISK_SECURITY_KEY=63,
    XEKEY_CONSTANT_DEVICE_REVOCATION_KEY=64,
    XEKEY_CONSTANT_XMACS_KEY=65,
    XEKEY_CONSTANT_REVOCATION_LIST_NONCE=66,
    XEKEY_CONSTANT_CROSS_PLATFORM_SYSLINK_KEY=67,
    XEKEY_SPECIAL_KEY_VAULT_SIGNATURE=68,
    XEKEY_SPECIAL_SECROM_DIGEST=69,
    XEKEY_SPECIAL_SECDATA=70,
    XEKEY_SPECIAL_DVD_FIRMWARE_KEY=71,
    XEKEY_MAX_CONSTANT_INDEX=72,
    XEKEY_TITLE_KEYS_BASE=224,
    XEKEY_TITLE_KEYS_LIMIT=232,
    XEKEY_TITLE_KEYS_RESET=240
} _XEKEY_INDEX;

typedef enum <unnamed-tag_0000119D> {
    SPR_TDABRX=248
} <unnamed-tag_0000119D>;

typedef enum _D3DVS_ADDRESSMODE_TYPE {
    D3DVS_ADDRMODE_ABSOLUTE=0,
    D3DVS_ADDRMODE_RELATIVE=0,
    D3DVS_ADDRMODE_FORCE_DWORD=255
} _D3DVS_ADDRESSMODE_TYPE;

typedef union PA_CL_UCP_2_W PA_CL_UCP_2_W, *PPA_CL_UCP_2_W;


/* WARNING! conflicting data type names: /iw5mp_xenon.pdb/PA_CL_UCP_2_W/<unnamed-type-bitfields> - /iw5mp_xenon.pdb/D1SCL_COEF_RAM_SELECT/<unnamed-type-bitfields> */

union PA_CL_UCP_2_W {
    struct <unnamed-type-bitfields> bitfields;
    struct <unnamed-type-bitfields> bits;
    uint u32All;
    int i32All;
    float f32All;
};

typedef struct D3DOcclusionQueryBatch D3DOcclusionQueryBatch, *PD3DOcclusionQueryBatch;

struct D3DOcclusionQueryBatch {
    int _padding_;
    int _padding_;
    int _padding_;
    int _padding_;
    int _padding_;
    int _padding_;
    ulong m_NumSlots;
    ulong m_NumTiles;
    ulong m_SlotSize;
    uint m_SlotsUsed;
    uchar *m_pBase;
    uchar *m_pCachedReadOnlyBase;
};

typedef struct D3DPerfCounterBatch D3DPerfCounterBatch, *PD3DPerfCounterBatch;

struct D3DPerfCounterBatch {
    int _padding_;
    int _padding_;
    int _padding_;
    int _padding_;
    int _padding_;
    int _padding_;
    ulong m_Flags;
    ulong m_NumSlots;
    ulong m_NumPasses;
    ulong m_SlotSize;
    uint m_SlotsUsed;
    uchar *m_pBase;
    uchar *m_pCachedReadOnlyBase;
};

typedef union GPU_EDRAMMODECONTROL GPU_EDRAMMODECONTROL, *PGPU_EDRAMMODECONTROL;

typedef struct GPU_EDRAMMODECONTROL_s_0 GPU_EDRAMMODECONTROL_s_0, *PGPU_EDRAMMODECONTROL_s_0;

struct GPU_EDRAMMODECONTROL_s_0 {
    ulong ColorDepthMacro:1;
    ulong EdramMode:3;
};

union GPU_EDRAMMODECONTROL {
    struct GPU_EDRAMMODECONTROL_s_0 _s_0;
    ulong dword;
};

typedef enum SQ_PERFCNT_SELECT {
    SQ_PIXEL_VECTORS_SUB=0,
    SQ_VERTEX_VECTORS_SUB=1,
    SQ_ALU0_ACTIVE_VTX_SIMD0=2,
    SQ_ALU1_ACTIVE_VTX_SIMD0=3,
    SQ_ALU0_ACTIVE_PIX_SIMD0=4,
    SQ_ALU1_ACTIVE_PIX_SIMD0=5,
    SQ_ALU0_ACTIVE_VTX_SIMD1=6,
    SQ_ALU1_ACTIVE_VTX_SIMD1=7,
    SQ_ALU0_ACTIVE_PIX_SIMD1=8,
    SQ_ALU1_ACTIVE_PIX_SIMD1=9,
    SQ_EXPORT_CYCLES=10,
    SQ_ALU_CST_WRITTEN=11,
    SQ_TEX_CST_WRITTEN=12,
    SQ_ALU_CST_STALL=13,
    SQ_ALU_TEX_STALL=14,
    SQ_INST_WRITTEN=15,
    SQ_BOOLEAN_WRITTEN=16,
    SQ_LOOPS_WRITTEN=17,
    SQ_PIXEL_SWAP_IN=18,
    SQ_PIXEL_SWAP_OUT=19,
    SQ_VERTEX_SWAP_IN=20,
    SQ_VERTEX_SWAP_OUT=21,
    SQ_ALU_VTX_INST_ISSUED=22,
    SQ_TEX_VTX_INST_ISSUED=23,
    SQ_VC_VTX_INST_ISSUED=24,
    SQ_CF_VTX_INST_ISSUED=25,
    SQ_ALU_PIX_INST_ISSUED=26,
    SQ_TEX_PIX_INST_ISSUED=27,
    SQ_VC_PIX_INST_ISSUED=28,
    SQ_CF_PIX_INST_ISSUED=29,
    SQ_ALU0_FIFO_EMPTY_SIMD0=30,
    SQ_ALU1_FIFO_EMPTY_SIMD0=31,
    SQ_ALU0_FIFO_EMPTY_SIMD1=32,
    SQ_ALU1_FIFO_EMPTY_SIMD1=33,
    SQ_ALU_NOPS=34,
    SQ_PRED_SKIP=35,
    SQ_SYNC_ALU_STALL_SIMD0_VTX=36,
    SQ_SYNC_ALU_STALL_SIMD1_VTX=37,
    SQ_SYNC_TEX_STALL_VTX=38,
    SQ_SYNC_VC_STALL_VTX=39,
    SQ_CONSTANTS_USED_SIMD0=40,
    SQ_CONSTANTS_SENT_SP_SIMD0=41,
    SQ_GPR_STALL_VTX=42,
    SQ_GPR_STALL_PIX=43,
    SQ_VTX_RS_STALL=44,
    SQ_PIX_RS_STALL=45,
    SQ_SX_PC_FULL=46,
    SQ_SX_EXP_BUFF_FULL=47,
    SQ_SX_POS_BUFF_FULL=48,
    SQ_INTERP_QUADS=49,
    SQ_INTERP_ACTIVE=50,
    SQ_IN_PIXEL_STALL=51,
    SQ_IN_VTX_STALL=52,
    SQ_VTX_CNT=53,
    SQ_VTX_16_VECTOR=54,
    SQ_VTX_32_VECTOR=55,
    SQ_VTX_48_VECTOR=56,
    SQ_PIXEL_16_VECTOR=57,
    SQ_PIXEL_32_VECTOR=58,
    SQ_PIXEL_48_VECTOR=59,
    SQ_CONSTANTS_USED_SIMD1=60,
    SQ_CONSTANTS_SENT_SP_SIMD1=61,
    SQ_SX_MEM_EXP_FULL=62,
    SQ_ALU0_ACTIVE_VTX_SIMD2=63,
    SQ_ALU1_ACTIVE_VTX_SIMD2=64,
    SQ_ALU0_ACTIVE_PIX_SIMD2=65,
    SQ_ALU1_ACTIVE_PIX_SIMD2=66,
    SQ_ALU0_ACTIVE_VTX_SIMD3=67,
    SQ_ALU1_ACTIVE_VTX_SIMD3=68,
    SQ_ALU0_ACTIVE_PIX_SIMD3=69,
    SQ_ALU1_ACTIVE_PIX_SIMD3=70,
    SQ_ALU0_FIFO_EMPTY_SIMD2=71,
    SQ_ALU1_FIFO_EMPTY_SIMD2=72,
    SQ_ALU0_FIFO_EMPTY_SIMD3=73,
    SQ_ALU1_FIFO_EMPTY_SIMD3=74,
    SQ_SYNC_ALU_STALL_SIMD2_VTX=75,
    SQ_SYNC_ALU_STALL_SIMD3_VTX=76,
    SQ_SYNC_ALU_STALL_SIMD0_PIX=77,
    SQ_SYNC_ALU_STALL_SIMD1_PIX=78,
    SQ_SYNC_ALU_STALL_SIMD2_PIX=79,
    SQ_SYNC_ALU_STALL_SIMD3_PIX=80,
    SQ_SYNC_TEX_STALL_PIX=81,
    SQ_SYNC_VC_STALL_PIX=82,
    SQ_CONSTANTS_USED_SIMD2=83,
    SQ_CONSTANTS_SENT_SP_SIMD2=84,
    SQ_CONSTANTS_USED_SIMD3=85,
    SQ_CONSTANTS_SENT_SP_SIMD3=86,
    SQ_ALU0_FIFO_FULL_SIMD0=87,
    SQ_ALU1_FIFO_FULL_SIMD0=88,
    SQ_ALU0_FIFO_FULL_SIMD1=89,
    SQ_ALU1_FIFO_FULL_SIMD1=90,
    SQ_ALU0_FIFO_FULL_SIMD2=91,
    SQ_ALU1_FIFO_FULL_SIMD2=92,
    SQ_ALU0_FIFO_FULL_SIMD3=93,
    SQ_ALU1_FIFO_FULL_SIMD3=94,
    VC_PERF_STATIC=95,
    VC_PERF_STALLED=96,
    VC_PERF_STARVED=97,
    VC_PERF_SEND=98,
    VC_PERF_ACTUAL_STARVED=99,
    PIXEL_THREAD_0_ACTIVE=100,
    VERTEX_THREAD_0_ACTIVE=101,
    PIXEL_THREAD_0_NUMBER=102,
    VERTEX_THREAD_0_NUMBER=103,
    VERTEX_EVENT_NUMBER=104,
    PIXEL_EVENT_NUMBER=105
} SQ_PERFCNT_SELECT;

typedef enum _D3DSAMPLER_TEXTURE_TYPE {
    D3DSTT_2D=0,
    D3DSTT_CUBE=0,
    D3DSTT_UNKNOWN=0,
    D3DSTT_VOLUME=0,
    D3DSTT_FORCE_DWORD=255
} _D3DSAMPLER_TEXTURE_TYPE;

typedef enum _D3DCULL {
    D3DCULL_NONE=0,
    D3DCULL_CW=2,
    D3DCULL_CCW=6,
    D3DCULL_FORCE_DWORD=255
} _D3DCULL;

typedef enum GPUPERFEVENT_SX {
    GPUPE_SX_SC_QUADS=0,
    GPUPE_SX_SC_QUAD_FIFO_FULL=1,
    GPUPE_SX_EXPORT_VECTORS=2,
    GPUPE_SX_DUMMY_QUADS=3,
    GPUPE_SX_ALPHA_FAIL=4,
    GPUPE_SX_RB_QUAD_BUSY=5,
    GPUPE_SX_RB_COLOR_BUSY=6,
    GPUPE_SX_RB_QUAD_STALL=7,
    GPUPE_SX_RB_COLOR_STALL=8
} GPUPERFEVENT_SX;

typedef union CP_RT11_SIZE CP_RT11_SIZE, *PCP_RT11_SIZE;


/* WARNING! conflicting data type names: /iw5mp_xenon.pdb/CP_RT11_SIZE/<unnamed-type-bitfields> - /iw5mp_xenon.pdb/D1SCL_COEF_RAM_SELECT/<unnamed-type-bitfields> */

union CP_RT11_SIZE {
    struct <unnamed-type-bitfields> bitfields;
    struct <unnamed-type-bitfields> bits;
    uint u32All;
    int i32All;
    float f32All;
};

typedef enum <unnamed-tag_00001137> {
    SPR_HSRR0=58
} <unnamed-tag_00001137>;

typedef enum tagLOCKTYPE {
    LOCK_WRITE=1,
    LOCK_EXCLUSIVE=2,
    LOCK_ONLYONCE=4
} tagLOCKTYPE;

typedef enum GPUTEXTUREFETCHOP {
    GPUTEXTUREFETCHOP_FETCH_TEXTURE_MAP=1,
    GPUTEXTUREFETCHOP_GET_BORDER_COLOR_FRACTION=16,
    GPUTEXTUREFETCHOP_GET_COMPUTED_TEX_LOD=17,
    GPUTEXTUREFETCHOP_GET_GRADIENTS=18,
    GPUTEXTUREFETCHOP_GET_WEIGHTS=19,
    GPUTEXTUREFETCHOP_SET_TEX_LOD=24,
    GPUTEXTUREFETCHOP_SET_GRADIENTS_H=25,
    GPUTEXTUREFETCHOP_SET_GRADIENTS_V=26
} GPUTEXTUREFETCHOP;

typedef enum SurfaceSwap {
    SWAP_LOWRED=0,
    SWAP_LOWBLUE=1
} SurfaceSwap;

typedef enum GPUCLIPPLANEMODE {
    GPUCLIPPLANEMODE_CULL_CENTER_NO_BIAS=0,
    GPUCLIPPLANEMODE_CULL_CENTER_RADIUS_BIAS=1,
    GPUCLIPPLANEMODE_CULL_CENTER_RADIUS_BIAS_EXPAND=2,
    GPUCLIPPLANEMODE_ALWAYS_EXPAND=3
} GPUCLIPPLANEMODE;

typedef enum _PixCCVideoOp {
    PIX_CC_Video_Stop=0,
    PIX_CC_Video_Play=1
} _PixCCVideoOp;

typedef union CONFIG_CNTL CONFIG_CNTL, *PCONFIG_CNTL;


/* WARNING! conflicting data type names: /iw5mp_xenon.pdb/CONFIG_CNTL/<unnamed-type-bitfields> - /iw5mp_xenon.pdb/D1SCL_COEF_RAM_SELECT/<unnamed-type-bitfields> */

union CONFIG_CNTL {
    struct <unnamed-type-bitfields> bitfields;
    struct <unnamed-type-bitfields> bits;
    uint u32All;
    int i32All;
    float f32All;
};

typedef enum GPUPERFEVENT_BIF {
    GPUPE_0=0,
    GPUPE_1=1,
    GPUPE_2=2,
    GPUPE_3=3,
    GPUPE_4=4,
    GPUPE_5=5
} GPUPERFEVENT_BIF;

typedef enum _XCONFIG_MEDIA_CENTER_ENTRIES {
    XCONFIG_MEDIA_CENTER_DATA=0,
    XCONFIG_MEDIA_CENTER_MEDIA_PLAYER=1,
    XCONFIG_MEDIA_CENTER_XESLED_VERSION=2,
    XCONFIG_MEDIA_CENTER_XESLED_TRUST_SECRET=3,
    XCONFIG_MEDIA_CENTER_XESLED_TRUST_CODE=4,
    XCONFIG_MEDIA_CENTER_XESLED_HOST_ID=5,
    XCONFIG_MEDIA_CENTER_XESLED_KEY=6,
    XCONFIG_MEDIA_CENTER_XESLED_HOST_MAC_ADDRESS=7,
    XCONFIG_MEDIA_CENTER_SERVER_UUID=8,
    XCONFIG_MEDIA_CENTER_SERVER_NAME=9,
    XCONFIG_MEDIA_CENTER_SERVER_FLAG=10
} _XCONFIG_MEDIA_CENTER_ENTRIES;

typedef enum <unnamed-tag_00001131> {
    SPR_RMOR=56
} <unnamed-tag_00001131>;

typedef enum <unnamed-tag_00001155> {
    SPR_TLB_RPN=181
} <unnamed-tag_00001155>;

typedef struct _D3DLOCKED_TAIL _D3DLOCKED_TAIL, *P_D3DLOCKED_TAIL;

struct _D3DLOCKED_TAIL {
    int RowPitch;
    int SlicePitch;
    void *pBits;
};

typedef union PA_SU_PERFCOUNTER2_HI PA_SU_PERFCOUNTER2_HI, *PPA_SU_PERFCOUNTER2_HI;


/* WARNING! conflicting data type names: /iw5mp_xenon.pdb/PA_SU_PERFCOUNTER2_HI/<unnamed-type-bitfields> - /iw5mp_xenon.pdb/D1SCL_COEF_RAM_SELECT/<unnamed-type-bitfields> */

union PA_SU_PERFCOUNTER2_HI {
    struct <unnamed-type-bitfields> bitfields;
    struct <unnamed-type-bitfields> bits;
    uint u32All;
    int i32All;
    float f32All;
};

typedef enum GPUHISTENCILFUNC {
    GPUHISTENCILFUNC_EQUAL=0,
    GPUHISTENCILFUNC_NOT_EQUAL=1
} GPUHISTENCILFUNC;

typedef enum Allocation_type {
    SQ_NO_ALLOC=0,
    SQ_POSITION=1,
    SQ_PARAMETER_PIXEL=2,
    SQ_MEMORY=3
} Allocation_type;

typedef enum eDM_PROFILE_PMC_SETUP {
    DM_PROFILE_PMC_L2=0,
    DM_PROFILE_PMC_DL1=1,
    DM_PROFILE_PMC_IL1=2,
    DM_PROFILE_PMC_LHS=3,
    DM_PROFILE_PMC_DERAT=4,
    DM_PROFILE_PMC_IERAT=5,
    DM_PROFILE_PMC_BRANCH=6,
    DM_PROFILE_PMC_UCODE=7,
    DM_PROFILE_PMC_INSTR=8,
    DM_PROFILE_PMC_MISALIGNED=9,
    DM_PROFILE_PMC_STQ_FULL=10,
    DM_PROFILE_PMC_DIVSQRT=11,
    DM_PROFILE_PMC_DENORM=12,
    DM_PROFILE_PMC_FPU_NAN=13,
    DM_PROFILE_PMC_VMX_ABNORMAL=14,
    DM_PROFILE_PMC_LAST=15
} eDM_PROFILE_PMC_SETUP;

typedef struct _D3DMIPTAIL_DESC _D3DMIPTAIL_DESC, *P_D3DMIPTAIL_DESC;

struct _D3DMIPTAIL_DESC {
    enum _D3DFORMAT Format;
    short _padding_;
    enum _D3DRESOURCETYPE Type;
    ulong Usage;
    ulong Pool;
    uint Width;
    uint Height;
    uint Depth;
    uint BaseLevel;
    uint Size;
};

typedef enum _D3DCUBEMAP_FACES {
    D3DCUBEMAP_FACE_POSITIVE_X=0,
    D3DCUBEMAP_FACE_NEGATIVE_X=1,
    D3DCUBEMAP_FACE_POSITIVE_Y=2,
    D3DCUBEMAP_FACE_NEGATIVE_Y=3,
    D3DCUBEMAP_FACE_POSITIVE_Z=4,
    D3DCUBEMAP_FACE_NEGATIVE_Z=5,
    D3DCUBEMAP_FACE_FORCE_DWORD=255
} _D3DCUBEMAP_FACES;

typedef enum _D3DXPATCHMESH {
    D3DXPATCHMESH_DEFAULT=0
} _D3DXPATCHMESH;

typedef enum <unnamed-tag_00001179> {
    SPR_IRMR0=209
} <unnamed-tag_00001179>;

typedef enum GPUCOPYSAMPLESELECT {
    GPUCOPYSAMPLESELECT_SAMPLE_0=0,
    GPUCOPYSAMPLESELECT_SAMPLE_1=1,
    GPUCOPYSAMPLESELECT_SAMPLE_2=2,
    GPUCOPYSAMPLESELECT_SAMPLE_3=3,
    GPUCOPYSAMPLESELECT_SAMPLES_0_1=4,
    GPUCOPYSAMPLESELECT_SAMPLES_2_3=5,
    GPUCOPYSAMPLESELECT_SAMPLES_0_1_2_3=6
} GPUCOPYSAMPLESELECT;

typedef enum RBBM_PERFCOUNT1_SEL {
    RBBM1_COUNT=0,
    RBBM1_NRT_BUSY=1,
    RBBM1_BC_CNTX0_BUSY=2,
    RBBM1_BC_CNTX17_BUSY=3,
    RBBM1_SQ_CNTX0_BUSY=4,
    RBBM1_SQ_CNTX17_BUSY=5,
    RBBM1_VGT_BUSY=6,
    RBBM1_VGT_NODMA_BUSY=7,
    RBBM1_PA_BUSY=8,
    RBBM1_SC_CNTX0_BUSY=9,
    RBBM1_SC_CNTX17_BUSY=10,
    RBBM1_TPC_BUSY=11,
    RBBM1_TC_BUSY=12,
    RBBM1_SX_BUSY=13,
    Reserved1=14,
    RBBM1_CP_COHER_BUSY=15,
    RBBM1_CP_NRT_BUSY=16,
    Reserved2=17,
    RBBM1_CP_DMA_BUSY=18,
    Reserved3=19,
    Reserved4=20,
    Reserved5=21,
    RBBM1_DMA_IDLE_STALL=22,
    Reserved6=23,
    Reserved7=24,
    RBBM1_GFX_IDLE_STALL=25,
    RBBM1_GFX_IDLEC_STALL=26,
    RBBM1_INTERRUPT=27
} RBBM_PERFCOUNT1_SEL;

typedef enum tagSTREAM_SEEK {
    STREAM_SEEK_SET=0,
    STREAM_SEEK_CUR=1,
    STREAM_SEEK_END=2
} tagSTREAM_SEEK;

typedef enum <unnamed-tag_000010A3> {
    CERTIFICATE_REVOCATION_LIST_DYNAMIC_MAXIMUM=0,
    CERTIFICATE_REVOCATION_LIST_INVALID=0,
    CERTIFICATE_REVOCATION_LIST_MAXIMUM=0,
    CERTIFICATE_REVOCATION_LIST_LIVE=76,
    CERTIFICATE_REVOCATION_LIST_PIRS=80
} <unnamed-tag_000010A3>;

typedef enum _TWO_WAY_LOCK_ACTIONS {
    XLF_LOCK_SHARED=0,
    XLF_LOCK_EXCLUSIVE=1,
    XLF_LOCK_DOWNGRADE=2,
    XLF_LOCK_UPGRADE_SAFELY=3,
    XLF_LOCK_UPGRADE_IGNORE=4
} _TWO_WAY_LOCK_ACTIONS;

typedef union GPU_BLENDCONTROL GPU_BLENDCONTROL, *PGPU_BLENDCONTROL;

typedef struct GPU_BLENDCONTROL_s_0 GPU_BLENDCONTROL_s_0, *PGPU_BLENDCONTROL_s_0;

struct GPU_BLENDCONTROL_s_0 {
    ulong ColorBlendOp:3;
    ulong ColorSrcBlend:5;
    ulong ColorDestBlend:5;
    ulong AlphaBlendOp:3;
    ulong AlphaSrcBlend:5;
    ulong AlphaDestBlend:5;
};

union GPU_BLENDCONTROL {
    struct GPU_BLENDCONTROL_s_0 _s_0;
    ulong dword;
};

typedef union PA_SC_DEBUG_CW_1 PA_SC_DEBUG_CW_1, *PPA_SC_DEBUG_CW_1;


/* WARNING! conflicting data type names: /iw5mp_xenon.pdb/PA_SC_DEBUG_CW_1/<unnamed-type-bitfields> - /iw5mp_xenon.pdb/D1SCL_COEF_RAM_SELECT/<unnamed-type-bitfields> */

union PA_SC_DEBUG_CW_1 {
    struct <unnamed-type-bitfields> bitfields;
    struct <unnamed-type-bitfields> bits;
    uint u32All;
    int i32All;
    float f32All;
};

typedef enum _XUSER_PROFILE_SOURCE {
    XSOURCE_NO_VALUE=0,
    XSOURCE_DEFAULT=1,
    XSOURCE_TITLE=2,
    XSOURCE_PERMISSION_DENIED=3
} _XUSER_PROFILE_SOURCE;

typedef enum <unnamed-tag_000011B1> {
    SPR_TDABR=253
} <unnamed-tag_000011B1>;

typedef enum GPUSTENCILOP {
    GPUSTENCILOP_KEEP=0,
    GPUSTENCILOP_ZERO=1,
    GPUSTENCILOP_REPLACE=2,
    GPUSTENCILOP_INCRSAT=3,
    GPUSTENCILOP_DECRSAT=4,
    GPUSTENCILOP_INVERT=5,
    GPUSTENCILOP_INCR=6,
    GPUSTENCILOP_DECR=7
} GPUSTENCILOP;

typedef enum _SOFTWARE_SECURITY_STAT_INDEX {
    SOFTWARE_SECURITY_STAT_DVD_BOOT_FAILURES=0,
    SOFTWARE_SECURITY_STAT_FUSE_BLOW_FAILURES=1,
    SOFTWARE_SECURITY_STAT_DVD_AUTH_EX_FAILURES=2,
    SOFTWARE_SECURITY_STAT_MAXIMUM=3
} _SOFTWARE_SECURITY_STAT_INDEX;

typedef enum _D3DFHZS_FLUSHTYPE {
    D3DFHZS_ASYNCHRONOUS=0,
    D3DFHZS_SYNCHRONOUS=1
} _D3DFHZS_FLUSHTYPE;

typedef enum PIX_API_TYPE {
    ApiType_BeginNamedEvent=0,
    ApiType_EndNamedEvent=1,
    ApiType_SetMarker=2,
    ApiType_Print=3,
    ApiType_Other=4,
    ApiType_Other_Overflow=5
} PIX_API_TYPE;

typedef enum _D3DSCREENEXTENTQUERYMODE {
    D3DSEQM_CULLED=0,
    D3DSEQM_NONCULLED=1,
    D3DSEQM_PRECLIP=2
} _D3DSCREENEXTENTQUERYMODE;

typedef enum _XCONFIG_CATEGORY_TYPES {
    XCONFIG_STATIC_CATEGORY=0,
    XCONFIG_STATISTIC_CATEGORY=1,
    XCONFIG_SECURED_CATEGORY=2,
    XCONFIG_USER_CATEGORY=3,
    XCONFIG_XNET_MACHINE_ACCOUNT_CATEGORY=4,
    XCONFIG_XNET_PARAMETERS_CATEGORY=5,
    XCONFIG_MEDIA_CENTER_CATEGORY=6,
    XCONFIG_CONSOLE_CATEGORY=7,
    XCONFIG_DVD_CATEGORY=8,
    XCONFIG_IPTV_CATEGORY=9,
    XCONFIG_SYSTEM_CATEGORY=10,
    XCONFIG_DEVKIT_CATEGORY=11,
    XCONFIG_CATEGORY_MAX=12
} _XCONFIG_CATEGORY_TYPES;

typedef enum _D3DXPARAMETER_TYPE {
    D3DXPT_VOID=0,
    D3DXPT_BOOL=1,
    D3DXPT_INT=2,
    D3DXPT_FLOAT=3,
    D3DXPT_STRING=4,
    D3DXPT_TEXTURE=5,
    D3DXPT_TEXTURE1D=6,
    D3DXPT_TEXTURE2D=7,
    D3DXPT_TEXTURE3D=8,
    D3DXPT_TEXTURECUBE=9,
    D3DXPT_SAMPLER=10,
    D3DXPT_SAMPLER1D=11,
    D3DXPT_SAMPLER2D=12,
    D3DXPT_SAMPLER3D=13,
    D3DXPT_SAMPLERCUBE=14,
    D3DXPT_PIXELSHADER=15,
    D3DXPT_VERTEXSHADER=16,
    D3DXPT_PIXELFRAGMENT=17,
    D3DXPT_VERTEXFRAGMENT=18,
    D3DXPT_FORCE_DWORD=255
} _D3DXPARAMETER_TYPE;

typedef enum VGT_EVENT_TYPE {
    VS_DEALLOC=0,
    PS_DEALLOC=1,
    VS_DONE_TS=2,
    PS_DONE_TS=3,
    CACHE_FLUSH_TS=4,
    CONTEXT_DONE=5,
    CACHE_FLUSH=6,
    VIZQUERY_START=7,
    VIZQUERY_END=8,
    SC_WAIT_WC=9,
    MPASS_PS_CP_REFETCH=10,
    MPASS_PS_RST_START=11,
    MPASS_PS_INCR_START=12,
    RST_PIX_CNT=13,
    RST_VTX_CNT=14,
    TILE_FLUSH=15,
    CACHE_FLUSH_AND_INV_TS_EVENT=20,
    ZPASS_DONE=21,
    CACHE_FLUSH_AND_INV_EVENT=22,
    PERFCOUNTER_START=23,
    PERFCOUNTER_STOP=24,
    SCREEN_EXT_INIT=25,
    SCREEN_EXT_RPT=26,
    VS_FETCH_DONE_TS=27
} VGT_EVENT_TYPE;

typedef struct _D3DRESOLVE_PARAMETERS _D3DRESOLVE_PARAMETERS, *P_D3DRESOLVE_PARAMETERS;

struct _D3DRESOLVE_PARAMETERS {
    enum _D3DFORMAT ColorFormat;
    int ColorExpBias;
    enum _D3DFORMAT DepthFormat;
};

typedef struct _XINPUT_ACCELERATION _XINPUT_ACCELERATION, *P_XINPUT_ACCELERATION;

struct _XINPUT_ACCELERATION {
    float XAxis;
    float YAxis;
    float ZAxis;
};

typedef enum VC_type {
    ALU_TP_REQUEST=0,
    VC_REQUEST=1
} VC_type;

typedef enum _KPROCESS_TYPE {
    ProcessTypeIdle=0,
    ProcessTypeNone=0,
    ProcessTypeTitle=1,
    ProcessTypeSystem=2
} _KPROCESS_TYPE;

typedef enum _D3DMATERIALCOLORSOURCE {
    D3DMCS_MATERIAL=0,
    D3DMCS_COLOR1=1,
    D3DMCS_COLOR2=2,
    D3DMCS_FORCE_DWORD=255
} _D3DMATERIALCOLORSOURCE;

typedef enum _XSTORAGE_FACILITY {
    XSTORAGE_FACILITY_GAME_CLIP=1,
    XSTORAGE_FACILITY_PER_TITLE=2,
    XSTORAGE_FACILITY_PER_USER_TITLE=3
} _XSTORAGE_FACILITY;

typedef enum DepthformatX {
    DEPTHX_24_8=0,
    DEPTHX_24_8_FLOAT=1
} DepthformatX;

typedef struct _CEA861_AUDIO_INFOFRAME _CEA861_AUDIO_INFOFRAME, *P_CEA861_AUDIO_INFOFRAME;

struct _CEA861_AUDIO_INFOFRAME {
    uchar PacketType;
    uchar Version;
    uchar Length:5;
    uchar Checksum;
    uchar CodingType:4;
    uchar ChannelCount:3;
    uchar SamplingFrequency:3;
    uchar SampleSize:2;
    uchar MaximumBitRate;
    uchar SpeakerAllocation;
    uchar DownmixInhibit:1;
    uchar LevelShiftValue:4;
    uchar Reserved[5];
};

typedef enum GPUEDRAMDEPTHFORMAT {
    GPUEDRAMDEPTHFORMAT_24_8=0,
    GPUEDRAMDEPTHFORMAT_24_8_FLOAT=1
} GPUEDRAMDEPTHFORMAT;

typedef enum GPUGROUPPRIMORDER {
    GPUGROUPPRIMORDER_LIST=0,
    GPUGROUPPRIMORDER_STRIP=1,
    GPUGROUPPRIMORDER_FAN=2,
    GPUGROUPPRIMORDER_LOOP=3,
    GPUGROUPPRIMORDER_POLYGON=4
} GPUGROUPPRIMORDER;

typedef enum <unnamed-tag_00001151> {
    SPR_TLB_VPN=180
} <unnamed-tag_00001151>;

typedef struct _D3DINDEXBUFFER_DESC _D3DINDEXBUFFER_DESC, *P_D3DINDEXBUFFER_DESC;

struct _D3DINDEXBUFFER_DESC {
    enum _D3DFORMAT Format;
    short _padding_;
    enum _D3DRESOURCETYPE Type;
    ulong Usage;
    ulong Pool;
    uint Size;
};

typedef enum <unnamed-tag_00001175> {
    SPR_IRSR0=208
} <unnamed-tag_00001175>;

typedef struct _D3DGPUVERSION _D3DGPUVERSION, *P_D3DGPUVERSION;

typedef enum _D3DGPUMAJORVERSION {
    D3DGPUMAJORVERSION_BETAPREVIEW=2,
    D3DGPUMAJORVERSION_FINAL=3
} _D3DGPUMAJORVERSION;

struct _D3DGPUVERSION {
    enum _D3DGPUMAJORVERSION MajorVersion;
    short _padding_;
    ushort MinorVersion;
};

typedef enum GPUSAMPLES {
    GPUSAMPLES_1X=0,
    GPUSAMPLES_2X=1,
    GPUSAMPLES_4X=2
} GPUSAMPLES;

typedef union GPU_SCLINECONTROL GPU_SCLINECONTROL, *PGPU_SCLINECONTROL;

typedef struct GPU_SCLINECONTROL_s_0 GPU_SCLINECONTROL_s_0, *PGPU_SCLINECONTROL_s_0;

struct GPU_SCLINECONTROL_s_0 {
    ulong BresControl:8;
    ulong LastPixel:1;
    ulong ExpandLineWidth:1;
    ulong UseBresControl:1;
};

union GPU_SCLINECONTROL {
    struct GPU_SCLINECONTROL_s_0 _s_0;
    ulong dword;
};

typedef struct _XECRYPT_RSAPUB_1024 _XECRYPT_RSAPUB_1024, *P_XECRYPT_RSAPUB_1024;

typedef struct _XECRYPT_RSA _XECRYPT_RSA, *P_XECRYPT_RSA;

struct _XECRYPT_RSA {
    ulong cqw;
    ulong dwPubExp;
    ulong64 qwReserved;
};

struct _XECRYPT_RSAPUB_1024 {
    struct _XECRYPT_RSA Rsa;
    ulong64 aqwM[16];
};

typedef enum <unnamed-tag_000010E3> {
    SPR_DEC=22
} <unnamed-tag_000010E3>;

typedef union PCIE_LC_LINK_WIDTH_CNTL PCIE_LC_LINK_WIDTH_CNTL, *PPCIE_LC_LINK_WIDTH_CNTL;


/* WARNING! conflicting data type names: /iw5mp_xenon.pdb/PCIE_LC_LINK_WIDTH_CNTL/<unnamed-type-bitfields> - /iw5mp_xenon.pdb/D1SCL_COEF_RAM_SELECT/<unnamed-type-bitfields> */

union PCIE_LC_LINK_WIDTH_CNTL {
    struct <unnamed-type-bitfields> bitfields;
    struct <unnamed-type-bitfields> bits;
    uint u32All;
    int i32All;
    float f32All;
};

typedef enum _D3DHIZENABLEMODE {
    D3DHIZ_DISABLE=0,
    D3DHIZ_ENABLE=1,
    D3DHIZ_AUTOMATIC=2,
    D3DHIZ_FORCE_DWORD=255
} _D3DHIZENABLEMODE;

typedef enum _XCONFIG_CONSOLE_ENTRIES {
    XCONFIG_CONSOLE_DATA=0,
    XCONFIG_CONSOLE_SCREEN_SAVER=1,
    XCONFIG_CONSOLE_AUTO_SHUT_OFF=2,
    XCONFIG_CONSOLE_WIRELESS_SETTINGS=3,
    XCONFIG_CONSOLE_CAMERA_SETTINGS=4,
    XCONFIG_CONSOLE_PLAYTIMERDATA=5,
    XCONFIG_CONSOLE_MEDIA_DISABLEAUTOLAUNCH=6,
    XCONFIG_CONSOLE_KEYBOARD_LAYOUT=7,
    XCONFIG_CONSOLE_PC_TITLE_EXEMPTIONS=8,
    XCONFIG_CONSOLE_NUI=9,
    XCONFIG_CONSOLE_VOICE=10,
    XCONFIG_CONSOLE_RETAIL_EX_FLAGS=11,
    XCONFIG_CONSOLE_DASH_FIRST_USE_TUTORIAL_FLAGS=12
} _XCONFIG_CONSOLE_ENTRIES;

typedef enum VGT_DMA_SWAP_MODE {
    VGT_DMA_SWAP_NONE=0,
    VGT_DMA_SWAP_16_BIT=1,
    VGT_DMA_SWAP_32_BIT=2,
    VGT_DMA_SWAP_WORD=3
} VGT_DMA_SWAP_MODE;

typedef enum GPUDIMENSION {
    GPUDIMENSION_1D=0,
    GPUDIMENSION_2D=1,
    GPUDIMENSION_3D=2,
    GPUDIMENSION_CUBEMAP=3
} GPUDIMENSION;

typedef enum _XSHOWMARKETPLACEUI_PRIVATE_ENTRYPOINTS {
    XSHOWMARKETPLACEUI_PRIVATE_ENTRYPOINT_ADDPOINTS=6,
    XSHOWMARKETPLACEUI_PRIVATE_ENTRYPOINT_PREPAIDCODE=7,
    XSHOWMARKETPLACEUI_PRIVATE_ENTRYPOINT_FNC=8,
    XSHOWMARKETPLACEUI_PRIVATE_ENTRYPOINT_GAMERTAGPURCHASE=9,
    XSHOWMARKETPLACEUI_PRIVATE_ENTRYPOINT_SIGNUP_CREDITCARD=10,
    XSHOWMARKETPLACEUI_PRIVATE_ENTRYPOINT_SIGNUP_PARENTCREDITCARD=11,
    XSHOWMARKETPLACEUI_PRIVATE_ENTRYPOINT_VIEWQUEUE=12,
    XSHOWMARKETPLACEUI_PRIVATE_ENTRYPOINT_VIEWDOWNLOADDETAILS=13,
    XSHOWMARKETPLACEUI_PRIVATE_ENTRYPOINT_MEDIAITEM=14,
    XSHOWMARKETPLACEUI_PRIVATE_ENTRYPOINT_GOLD_UPSELL=15,
    XSHOWMARKETPLACEUI_PRIVATE_ENTRYPOINT_TERMSOFUSE=16,
    XSHOWMARKETPLACEUI_PRIVATE_ENTRYPOINT_DIRECT_ACQUIRE=17,
    XSHOWMARKETPLACEUI_PRIVATE_ENTRYPOINT_PURCHASEPOINTS=18,
    XSHOWMARKETPLACEUI_PRIVATE_ENTRYPOINT_PAYMENT_OPTIONS=19,
    XSHOWMARKETPLACEUI_PRIVATE_ENTRYPOINT_BUNDLE_ITEM_DOWNLOAD=20,
    XSHOWMARKETPLACEUI_PRIVATE_ENTRYPOINT_MAX=21
} _XSHOWMARKETPLACEUI_PRIVATE_ENTRYPOINTS;

typedef enum _D3DSVERROR_TYPE {
    D3DSVE_GENERIC_ERROR=0,
    D3DSVE_INSTRUCTION_WARNING=1,
    D3DSVE_INSTRUCTION_ERROR=2,
    D3DSVE_ELEMENT_WARNING=3,
    D3DSVE_ELEMENT_ERROR=4,
    D3DSVE_GLOBAL_WARNING=5,
    D3DSVE_GLOBAL_ERROR=6,
    D3DSVE_GENERIC_WARNING=7
} _D3DSVERROR_TYPE;

typedef enum RBBM_PERFCOUNT2_SEL {
    RBBM2_COUNT=0,
    RBBM2_NRT_BUSY=1,
    RBBM2_RC_CNTX0_BUSY=2,
    RBBM2_RC_CNTX17_BUSY=3,
    RBBM2_SQ_CNTX0_BUSY=4,
    RBBM2_SQ_CNTX17_BUSY=5,
    RBBM2_VGT_BUSY=6,
    RBBM2_VGT_NODMA_BUSY=7,
    RBBM2_PA_BUSY=8,
    RBBM2_SC_CNTX0_BUSY=9,
    RBBM2_SC_CNTX17_BUSY=10,
    RBBM2_TPC_BUSY=11,
    RBBM2_TC_BUSY=12,
    RBBM2_SX_BUSY=13,
    Reserved8=14,
    RBBM2_CP_COHER_BUSY=15,
    RBBM2_CP_NRT_BUSY=16,
    Reserved9=17,
    RBBM2_CP_DMA_BUSY=18,
    Reserved10=19,
    Reserved11=20,
    Reserved12=21,
    RBBM2_DMA_IDLE_STALL=22,
    Reserved13=23,
    Reserved14=24,
    RBBM2_GFX_IDLE_STALL=25,
    RBBM2_GFX_IDLEC_STALL=26,
    RBBM2_INTERRUPT=27
} RBBM_PERFCOUNT2_SEL;

typedef struct _D3DVOLUME_DESC _D3DVOLUME_DESC, *P_D3DVOLUME_DESC;

struct _D3DVOLUME_DESC {
    enum _D3DFORMAT Format;
    short _padding_;
    enum _D3DRESOURCETYPE Type;
    ulong Usage;
    ulong Pool;
    uint Width;
    uint Height;
    uint Depth;
};

typedef enum GPUENDIAN {
    GPUENDIAN_NONE=0,
    GPUENDIAN_8IN16=1,
    GPUENDIAN_8IN32=2,
    GPUENDIAN_16IN32=3
} GPUENDIAN;

typedef struct _D3DOCCLUSIONSTATS _D3DOCCLUSIONSTATS, *P_D3DOCCLUSIONSTATS;

struct _D3DOCCLUSIONSTATS {
    ulong ZPass;
    ulong ZFail;
    ulong StencilFail;
    ulong Total;
};

typedef enum VertexMode {
    POSITION_1_VECTOR=0,
    POSITION_2_VECTORS_UNUSED=1,
    POSITION_2_VECTORS_SPRITE=2,
    POSITION_2_VECTORS_EDGE=3,
    POSITION_2_VECTORS_KILL=4,
    POSITION_2_VECTORS_SPRITE_KILL=5,
    POSITION_2_VECTORS_EDGE_KILL=6,
    MULTIPASS=7
} VertexMode;

typedef enum _D3DPERFCOUNTERINDEX {
    D3DPERFCOUNTERINDEX_TIMESTAMP=0,
    D3DPERFCOUNTERINDEX_CP0=1,
    D3DPERFCOUNTERINDEX_RBBM0=2,
    D3DPERFCOUNTERINDEX_RBBM1=3,
    D3DPERFCOUNTERINDEX_SQ0=4,
    D3DPERFCOUNTERINDEX_SQ1=5,
    D3DPERFCOUNTERINDEX_SQ2=6,
    D3DPERFCOUNTERINDEX_SQ3=7,
    D3DPERFCOUNTERINDEX_VGT0=8,
    D3DPERFCOUNTERINDEX_VGT1=9,
    D3DPERFCOUNTERINDEX_VGT2=10,
    D3DPERFCOUNTERINDEX_VGT3=11,
    D3DPERFCOUNTERINDEX_VC0=12,
    D3DPERFCOUNTERINDEX_VC1=13,
    D3DPERFCOUNTERINDEX_VC2=14,
    D3DPERFCOUNTERINDEX_VC3=15,
    D3DPERFCOUNTERINDEX_PA_SU0=16,
    D3DPERFCOUNTERINDEX_PA_SU1=17,
    D3DPERFCOUNTERINDEX_PA_SU2=18,
    D3DPERFCOUNTERINDEX_PA_SU3=19,
    D3DPERFCOUNTERINDEX_PA_SC0=20,
    D3DPERFCOUNTERINDEX_PA_SC1=21,
    D3DPERFCOUNTERINDEX_PA_SC2=22,
    D3DPERFCOUNTERINDEX_PA_SC3=23,
    D3DPERFCOUNTERINDEX_HZ0=24,
    D3DPERFCOUNTERINDEX_HZ1=25,
    D3DPERFCOUNTERINDEX_TCR0=26,
    D3DPERFCOUNTERINDEX_TCR1=27,
    D3DPERFCOUNTERINDEX_TCM0=28,
    D3DPERFCOUNTERINDEX_TCM1=29,
    D3DPERFCOUNTERINDEX_TCF0=30,
    D3DPERFCOUNTERINDEX_TCF1=31,
    D3DPERFCOUNTERINDEX_TCF2=32,
    D3DPERFCOUNTERINDEX_TCF3=33,
    D3DPERFCOUNTERINDEX_TCF4=34,
    D3DPERFCOUNTERINDEX_TCF5=35,
    D3DPERFCOUNTERINDEX_TCF6=36,
    D3DPERFCOUNTERINDEX_TCF7=37,
    D3DPERFCOUNTERINDEX_TCF8=38,
    D3DPERFCOUNTERINDEX_TCF9=39,
    D3DPERFCOUNTERINDEX_TCF10=40,
    D3DPERFCOUNTERINDEX_TCF11=41,
    D3DPERFCOUNTERINDEX_TP00=42,
    D3DPERFCOUNTERINDEX_TP01=43,
    D3DPERFCOUNTERINDEX_TP10=44,
    D3DPERFCOUNTERINDEX_TP11=45,
    D3DPERFCOUNTERINDEX_TP20=46,
    D3DPERFCOUNTERINDEX_TP21=47,
    D3DPERFCOUNTERINDEX_TP30=48,
    D3DPERFCOUNTERINDEX_TP31=49,
    D3DPERFCOUNTERINDEX_SX0=50,
    D3DPERFCOUNTERINDEX_BC0=51,
    D3DPERFCOUNTERINDEX_BC1=52,
    D3DPERFCOUNTERINDEX_BC2=53,
    D3DPERFCOUNTERINDEX_BC3=54,
    D3DPERFCOUNTERINDEX_MC00=55,
    D3DPERFCOUNTERINDEX_MC10=56,
    D3DPERFCOUNTERINDEX_MH0=57,
    D3DPERFCOUNTERINDEX_MH1=58,
    D3DPERFCOUNTERINDEX_MH2=59,
    D3DPERFCOUNTERINDEX_BIF0=60,
    D3DPERFCOUNTERINDEX_MAX=61
} _D3DPERFCOUNTERINDEX;

typedef enum _D3DXTRANSITION_TYPE {
    D3DXTRANSITION_LINEAR=0,
    D3DXTRANSITION_EASEINEASEOUT=1,
    D3DXTRANSITION_FORCE_DWORD=255
} _D3DXTRANSITION_TYPE;

typedef enum GPUGROUPCONV {
    GPUGROUPCONV_INDEX_16=0,
    GPUGROUPCONV_INDEX_32=1,
    GPUGROUPCONV_UINT_16=2,
    GPUGROUPCONV_UINT_32=3,
    GPUGROUPCONV_SINT_16=4,
    GPUGROUPCONV_SINT_32=5,
    GPUGROUPCONV_FLOAT_32=6,
    GPUGROUPCONV_AUTO_PRIM=7,
    GPUGROUPCONV_FIX_1_23_TO_FLOAT=8
} GPUGROUPCONV;

typedef enum VGT_PERFCOUNT_SELECT {
    vgt_sq_event_window_active=0,
    vgt_sq_send=1,
    vgt_sq_stalled=2,
    vgt_sq_starved_busy=3,
    vgt_sq_starved_idle=4,
    vgt_sq_static=5,
    vgt_pa_event_window_active=6,
    vgt_pa_clip_v_send=7,
    vgt_pa_clip_v_stalled=8,
    vgt_pa_clip_v_starved_busy=9,
    vgt_pa_clip_v_starved_idle=10,
    vgt_pa_clip_v_static=11,
    vgt_pa_clip_p_send=12,
    vgt_pa_clip_p_stalled=13,
    vgt_pa_clip_p_starved_busy=14,
    vgt_pa_clip_p_starved_idle=15,
    vgt_pa_clip_p_static=16,
    vgt_pa_clip_s_send=17,
    vgt_pa_clip_s_stalled=18,
    vgt_pa_clip_s_starved_busy=19,
    vgt_pa_clip_s_starved_idle=20,
    vgt_pa_clip_s_static=21,
    rbiu_fifos_event_window_active=22,
    rbiu_immed_data_fifo_starved=23,
    rbiu_immed_data_fifo_stalled=24,
    rbiu_dma_request_fifo_starved=25,
    rbiu_dma_request_fifo_stalled=26,
    rbiu_draw_initiator_fifo_starved=27,
    rbiu_draw_initiator_fifo_stalled=28,
    spare29=29,
    spare30=30,
    spare31=31,
    spare32=32,
    spare33=33,
    spare34=34,
    spare35=35,
    spare36=36,
    spare37=37,
    spare38=38,
    spare39=39,
    te_su_in_valid=40,
    te_su_in_read=41,
    te_su_in_prim=42,
    te_su_in_eop=43,
    te_su_in_null_prim=44,
    te_wk_in_valid=45,
    te_wk_in_read=46,
    te_out_prim_valid=47,
    te_out_prim_read=48
} VGT_PERFCOUNT_SELECT;

typedef struct _LDRX_PLUGIN _LDRX_PLUGIN, *P_LDRX_PLUGIN;

struct _LDRX_PLUGIN {
    ulong Format;
    long (*LoadFunction)(uchar *, struct _LDRX_SOURCE *, struct _LDRX_SOURCE *);
};

typedef enum GPUCONSTANTTYPE {
    GPUCONSTANTTYPE_INVALID_TEXTURE=0,
    GPUCONSTANTTYPE_INVALID_VERTEX=1,
    GPUCONSTANTTYPE_TEXTURE=2,
    GPUCONSTANTTYPE_VERTEX=3
} GPUCONSTANTTYPE;

typedef enum _XC_PASSKEY {
    XC_PASSKEY_NONE=0,
    XC_PASSKEY_CODE_X=1,
    XC_PASSKEY_CODE_Y=2,
    XC_PASSKEY_CODE_DPAD_LEFT=3,
    XC_PASSKEY_CODE_DPAD_RIGHT=4,
    XC_PASSKEY_CODE_DPAD_UP=5,
    XC_PASSKEY_CODE_DPAD_DOWN=6,
    XC_PASSKEY_CODE_LTRIGGER=9,
    XC_PASSKEY_CODE_RTRIGGER=10,
    XC_PASSKEY_CODE_LSHOULDER=11,
    XC_PASSKEY_CODE_RSHOULDER=12,
    XC_PASSKEY_CODE_UNKNOWN=255
} _XC_PASSKEY;

typedef enum <unnamed-tag_000010E5> {
    SPR_SDR1=25
} <unnamed-tag_000010E5>;

typedef union GC_DEVICE_ID GC_DEVICE_ID, *PGC_DEVICE_ID;


/* WARNING! conflicting data type names: /iw5mp_xenon.pdb/GC_DEVICE_ID/<unnamed-type-bitfields> - /iw5mp_xenon.pdb/D1SCL_COEF_RAM_SELECT/<unnamed-type-bitfields> */

union GC_DEVICE_ID {
    struct <unnamed-type-bitfields> bitfields;
    struct <unnamed-type-bitfields> bits;
    uint u32All;
    int i32All;
    float f32All;
};

typedef enum StencilOp {
    STENCIL_KEEP=0,
    STENCIL_ZERO=1,
    STENCIL_REPLACE=2,
    STENCIL_INCR_CLAMP=3,
    STENCIL_DECR_CLAMP=4,
    STENCIL_INVERT=5,
    STENCIL_INCR_WRAP=6,
    STENCIL_DECR_WRAP=7
} StencilOp;

typedef struct D3DArrayTexture D3DArrayTexture, *PD3DArrayTexture;

struct D3DArrayTexture {
    undefined field0_0x0;
    undefined field1_0x1;
    undefined field2_0x2;
    undefined field3_0x3;
    undefined field4_0x4;
    undefined field5_0x5;
    undefined field6_0x6;
    undefined field7_0x7;
    undefined field8_0x8;
    undefined field9_0x9;
    undefined field10_0xa;
    undefined field11_0xb;
    undefined field12_0xc;
    undefined field13_0xd;
    undefined field14_0xe;
    undefined field15_0xf;
    undefined field16_0x10;
    undefined field17_0x11;
    undefined field18_0x12;
    undefined field19_0x13;
    undefined field20_0x14;
    undefined field21_0x15;
    undefined field22_0x16;
    undefined field23_0x17;
    undefined field24_0x18;
    undefined field25_0x19;
    undefined field26_0x1a;
    undefined field27_0x1b;
    undefined field28_0x1c;
    undefined field29_0x1d;
    undefined field30_0x1e;
    undefined field31_0x1f;
    undefined field32_0x20;
    undefined field33_0x21;
    undefined field34_0x22;
    undefined field35_0x23;
    undefined field36_0x24;
    undefined field37_0x25;
    undefined field38_0x26;
    undefined field39_0x27;
    undefined field40_0x28;
    undefined field41_0x29;
    undefined field42_0x2a;
    undefined field43_0x2b;
    undefined field44_0x2c;
    undefined field45_0x2d;
    undefined field46_0x2e;
    undefined field47_0x2f;
    undefined field48_0x30;
    undefined field49_0x31;
    undefined field50_0x32;
    undefined field51_0x33;
};

typedef struct _D3DPERFCOUNTER_EVENTS _D3DPERFCOUNTER_EVENTS, *P_D3DPERFCOUNTER_EVENTS;

struct _D3DPERFCOUNTER_EVENTS {
    enum GPUPERFEVENT_CP CP[4];
    enum GPUPERFEVENT_RBBM RBBM[8];
    enum GPUPERFEVENT_SQ SQ[16];
    enum GPUPERFEVENT_VGT VGT[16];
    enum GPUPERFEVENT_VC VC[16];
    enum GPUPERFEVENT_PA_SU PA_SU[16];
    enum GPUPERFEVENT_PA_SC PA_SC[16];
    enum GPUPERFEVENT_HZ HZ[8];
    enum GPUPERFEVENT_TCR TCR[8];
    enum GPUPERFEVENT_TCM TCM[8];
    enum GPUPERFEVENT_TCF TCF[48];
    enum GPUPERFEVENT_TP TP0[8];
    enum GPUPERFEVENT_TP TP1[8];
    enum GPUPERFEVENT_TP TP2[8];
    enum GPUPERFEVENT_TP TP3[8];
    enum GPUPERFEVENT_SX SX[4];
    enum GPUPERFEVENT_BC BC[16];
    enum GPUPERFEVENT_MC MC0[4];
    enum GPUPERFEVENT_MC MC1[4];
    enum GPUPERFEVENT_MH MH[12];
    enum GPUPERFEVENT_BIF BIF[4];
};

typedef enum IMPORT_OBJECT_NAME_TYPE {
    IMPORT_OBJECT_ORDINAL=0,
    IMPORT_OBJECT_NAME=1,
    IMPORT_OBJECT_NAME_NO_PREFIX=2,
    IMPORT_OBJECT_NAME_UNDECORATE=3
} IMPORT_OBJECT_NAME_TYPE;

typedef enum _D3DXPATCHMESHTYPE {
    D3DXPATCHMESH_RECT=1,
    D3DXPATCHMESH_TRI=2,
    D3DXPATCHMESH_NPATCH=3,
    D3DXPATCHMESH_FORCE_DWORD=255
} _D3DXPATCHMESHTYPE;

typedef enum VGT_GRP_PRIM_TYPE {
    VGT_GRP_3D_POINT=0,
    VGT_GRP_3D_LINE=1,
    VGT_GRP_3D_TRI=2,
    VGT_GRP_3D_RECT=3,
    VGT_GRP_3D_QUAD=4,
    VGT_GRP_2D_COPY_RECT_V0=5,
    VGT_GRP_2D_COPY_RECT_V1=6,
    VGT_GRP_2D_COPY_RECT_V2=7,
    VGT_GRP_2D_COPY_RECT_V3=8,
    VGT_GRP_2D_FILL_RECT=9,
    VGT_GRP_2D_LINE=10,
    VGT_GRP_2D_TRI=11,
    VGT_GRP_PRIM_INDEX_LINE=12,
    VGT_GRP_PRIM_INDEX_TRI=13,
    VGT_GRP_PRIM_INDEX_QUAD=14
} VGT_GRP_PRIM_TYPE;

typedef union GPU_POINTSIZE GPU_POINTSIZE, *PGPU_POINTSIZE;

typedef struct GPU_POINTSIZE_s_0 GPU_POINTSIZE_s_0, *PGPU_POINTSIZE_s_0;

struct GPU_POINTSIZE_s_0 {
    ulong Height:16;
    ulong Width:16;
};

union GPU_POINTSIZE {
    struct GPU_POINTSIZE_s_0 _s_0;
    ulong dword;
};

typedef struct _UNICODE_STRING _UNICODE_STRING, *P_UNICODE_STRING;

struct _UNICODE_STRING {
    ushort Length;
    ushort MaximumLength;
    wchar_t *Buffer;
};

typedef union FSB_TX_PMC4 FSB_TX_PMC4, *PFSB_TX_PMC4;


/* WARNING! conflicting data type names: /iw5mp_xenon.pdb/FSB_TX_PMC4/<unnamed-type-bitfields> - /iw5mp_xenon.pdb/D1SCL_COEF_RAM_SELECT/<unnamed-type-bitfields> */

union FSB_TX_PMC4 {
    struct <unnamed-type-bitfields> bitfields;
    struct <unnamed-type-bitfields> bits;
    uint u32All;
    int i32All;
    float f32All;
};

typedef enum <unnamed-tag_00001171> {
    SPR_DRMR1=188
} <unnamed-tag_00001171>;

typedef enum <unnamed-tag_00001195> {
    SPR_HID5=246
} <unnamed-tag_00001195>;

typedef enum <unnamed-tag_0000110D> {
    SPR_SPRG2=18
} <unnamed-tag_0000110D>;

typedef enum GPUENDIAN128 {
    GPUENDIAN128_NONE=0,
    GPUENDIAN128_8IN16=1,
    GPUENDIAN128_8IN32=2,
    GPUENDIAN128_16IN32=3,
    GPUENDIAN128_8IN64=4,
    GPUENDIAN128_8IN128=5
} GPUENDIAN128;

typedef enum _XPROFILE_ACTION_AUTO_AIM_OPTIONS {
    XPROFILE_ACTION_AUTO_AIM_OFF=0,
    XPROFILE_ACTION_AUTO_AIM_ON=1
} _XPROFILE_ACTION_AUTO_AIM_OPTIONS;

typedef enum TileStencilFunc {
    TILE_EQ=0,
    TILE_NEQ=1
} TileStencilFunc;

typedef enum CP_PERFCOUNT_SEL {
    CP_COUNT=0,
    CP_RBIU_STALL=1,
    CP_RBIU_TAF=2,
    CP_PFP_STALL=3,
    CP_Reserved0=4,
    CP_Reserved1=5,
    CP_NRT_RCIU_STALL=6,
    CP_Reserved2=7,
    CP_NRT_MIU_STALL=8,
    CP_CSF_PFP_I1_FULL=9,
    CP_CSF_PFP_I2_FULL=10,
    CP_CSF_PFP_REQ_FULL=11,
    CP_Reserved3=12,
    CP_RING_ROQ_FULL=13,
    CP_I1_ROQ_FULL=14,
    CP_I2_ROQ_FULL=15,
    CP_ST_ROQ_FULL=16,
    CP_RTST_ROQ_FULL=17,
    CP_MIU_TAG_MEM_FULL=18,
    CP_MIU_WRITECLEAN=19,
    CP_Reserved4=20,
    CP_Reserved5=21,
    CP_NRT_WRITE_STALL=22,
    CP_NRT_READ_STALL=23,
    CP_WC_FIFO_FULL=24,
    CP_VTX_DEALLOC_FIFO_FULL=25,
    CP_PIX_DEALLOC_FIFO_FULL=26,
    CP_VTX_EVENT_FIFO_FULL=27,
    CP_PIX_EVENT_FIFO_FULL=28,
    CP_CF_EVENT_FIFO_FULL=29,
    CP_ME_RB_STARVED=30,
    CP_ME_I1_STARVED=31,
    CP_ME_I2_STARVED=32,
    CP_ME_ST_STARVED=33,
    CP_Reserved6=34,
    CP_Reserved7=35,
    CP_Reserved8=36,
    CP_Reserved9=37,
    CP_Reserved10=38,
    CP_Reserved11=39,
    RCIU_RBBM_DWORD_SENT=40,
    ME_PARSER_BUSY_CLOCKS=41,
    ME_WAIT_CONTEXT_AVAIL=42,
    PFP_TYPE0_PACKET=43,
    PFP_TYPE3_PACKET=44,
    CSF_RB_WPTR_NEQ_RPTR=45,
    CSF_I1_SIZE_NEQ_ZERO=46,
    CSF_I2_SIZE_NEQ_ZERO=47,
    CSF_RB_I1_I2_FETCHING=48,
    CP_Reserved12=49,
    CP_Reserved13=50,
    CP_Reserved14=51,
    CP_Reserved15=52,
    CP_Reserved16=53,
    CP_Reserved17=54,
    CP_Reserved18=55,
    CP_Reserved19=56,
    CP_Reserved20=57,
    CP_Reserved21=58,
    CP_Reserved22=59,
    CP_Reserved23=60,
    CP_Reserved24=61,
    CP_Reserved25=62,
    CP_Reserved26=63
} CP_PERFCOUNT_SEL;

typedef enum _EVENT_TYPE {
    NotificationEvent=0,
    SynchronizationEvent=1
} _EVENT_TYPE;

typedef enum Addressing {
    RELATIVE_ADDR=0,
    ABSOLUTE_ADDR=1
} Addressing;

typedef enum _RESTRICTED_PRIVILEGE {
    RESTRICTED_PRIVILEGE_ANY=0,
    RESTRICTED_PRIVILEGE_BETA=0,
    RESTRICTED_PRIVILEGE_HARDWARE=0,
    RESTRICTED_PRIVILEGE_INTERNAL=0,
    RESTRICTED_PRIVILEGE_NONE=0,
    RESTRICTED_PRIVILEGE_PUBLIC=0,
    RESTRICTED_PRIVILEGE_REVERT=1,
    RESTRICTED_PRIVILEGE_INTEGRATION=2,
    RESTRICTED_PRIVILEGE_CHINA_TRIAL=4,
    RESTRICTED_PRIVILEGE_XGD2_EXEMPT=8,
    RESTRICTED_PRIVILEGE_PROFILE=16,
    RESTRICTED_PRIVILEGE_PREVIEW=32,
    RESTRICTED_PRIVILEGE_AUTOMATION=64,
    RESTRICTED_PRIVILEGE_DIRECT_USB_STORAGE=128
} _RESTRICTED_PRIVILEGE;

typedef enum _D3DDECLMETHOD {
    D3DDECLMETHOD_DEFAULT=0,
    D3DDECLMETHOD_PARTIALU=1,
    D3DDECLMETHOD_PARTIALV=2,
    D3DDECLMETHOD_CROSSUV=3,
    D3DDECLMETHOD_UV=4,
    D3DDECLMETHOD_LOOKUP=5,
    D3DDECLMETHOD_LOOKUPPRESAMPLED=6
} _D3DDECLMETHOD;

typedef enum SurfaceArray {
    ARRAY_1D=0,
    ARRAY_2D=1,
    ARRAY_3D=2,
    ARRAY_3D_SLICE=3
} SurfaceArray;

typedef struct _D3DLOCKED_BOX _D3DLOCKED_BOX, *P_D3DLOCKED_BOX;

struct _D3DLOCKED_BOX {
    int RowPitch;
    int SlicePitch;
    void *pBits;
};

typedef enum <unnamed-tag_00001173> {
    SPR_DCIDR1=189
} <unnamed-tag_00001173>;

typedef enum <unnamed-tag_00001282> {
    XEX_SHIM_IDENTIFIER_VERSION_4_DIGIT=0,
    XEX_SHIM_IDENTIFIER_MAXIMUM=1
} <unnamed-tag_00001282>;

typedef enum <unnamed-tag_00001197> {
    SPR_DABRX=247
} <unnamed-tag_00001197>;

typedef enum RecordResourceType {
    Resource_VertexBuffer=0,
    Resource_IndexBuffer=1,
    Resource_Texture=2,
    Resource_Context=3,
    Resource_Shader=4,
    Resource_Export=5,
    Resource_AluConstants=6,
    Resource_MAX=7
} RecordResourceType;

typedef union GPU_MODECONTROL GPU_MODECONTROL, *PGPU_MODECONTROL;

typedef struct GPU_MODECONTROL_s_0 GPU_MODECONTROL_s_0, *PGPU_MODECONTROL_s_0;

struct GPU_MODECONTROL_s_0 {
    ulong PolyModeFrontPType:3;
    ulong PolyMode:2;
    ulong CullMode:3;
    ulong MsaaEnable:1;
    ulong PolyOffsetParaEnable:1;
    ulong PolyOffsetBackEnable:1;
    ulong PolyOffsetFrontEnable:1;
    ulong PolyModeBackPType:3;
    ulong QuadOrderEnable:1;
    ulong MultiPrimIbEnable:1;
    ulong PerspCorrDisable:1;
    ulong ProvokingVtxLast:1;
    ulong VtxWindowOffsetEnable:1;
    ulong ScOneQuadPerClock:1;
};

union GPU_MODECONTROL {
    struct GPU_MODECONTROL_s_0 _s_0;
    ulong dword;
};

typedef enum <unnamed-tag_0000110F> {
    SPR_SPRG3=19
} <unnamed-tag_0000110F>;

typedef enum _D3DXPRIORITY_TYPE {
    D3DXPRIORITY_LOW=0,
    D3DXPRIORITY_HIGH=1,
    D3DXPRIORITY_FORCE_DWORD=255
} _D3DXPRIORITY_TYPE;

typedef enum <unnamed-tag_000010E1> {
    SPR_DAR=19
} <unnamed-tag_000010E1>;

typedef enum BIU_PERFCOUNT_SELECT {
    recv_requests=0,
    recv_responses=1,
    ioc_reads=2,
    ioc_writes=3,
    mem_reads=4,
    mem_writes=5,
    fsb_syncs=6,
    EIEIOs=7,
    EOIs=8,
    Flush_Acks=9,
    request_read_data_bytes_8_byte_and_less_transfers____dependent_on_secondary_control_register=10,
    request_read_data_bytes_16_byte_and_more_transfers___dependent_on_secondary_control_register=11,
    request_write_data_bytes_8_byte_and_less_transfers___dependent_on_secondary_control_register=12,
    request_write_data_bytes_16_byte_and_more_transfer___dependent_on_secondary_control_register=13,
    reserved0=14,
    xmit_requests=15,
    xmit_responses=16,
    read128s=17,
    flush_req_reads=18,
    flush_req_writes=19,
    cpuwb=20,
    interrupts=21,
    responses_mc0=22,
    responses_mc1=23,
    responses_ioc_mst=24,
    mc0_response_read_data_bytes_8_byte_and_less_transfers____dependent_on_secondary_control_register_=25,
    mc0_response_read_data_bytes_16_byte_and_more_transfers___dependent_on__secondary_control_register_=26,
    mc1_response_read_data_bytes_8_byte_and_less_transfers____dependent_on_secondary_control_register_=27,
    mc1_response_read_data_bytes_16_byte_and_more_transfers___dependent_on__secondary_control_register_=28,
    ioc_mst_response_read_data_bytes_8_byte_and_less_transfers____dependent_on_secondary_control_register_=29,
    ioc_mst_response_read_data_bytes_16_byte_and_more_transfers___dependent_on__secondary_control_register_=30,
    reserved1=31,
    reserved2=32,
    reserved3=33,
    reserved4=34,
    reserved5=35,
    reserved6=36,
    reserved7=37,
    reserved8=38,
    reserved9=39,
    requests=40,
    ioc_mast_reads=41,
    ioc_mast_writes=42,
    ioc_mast_eois=43,
    ioc_mast_request_read_data_bytes__8_byte_and_less_transfers____dependent_on_secondary_control_register=44,
    ioc_mast_request_read_data_bytes__16_byte_and_more_transfers___dependent_on_secondary_control_register=45,
    ioc_mast_request_write_data_bytes__8_byte_and_less_transfers___dependent_on_secondary_control_register=46,
    ioc_mast_request_write_data_bytes__16_byte_and_more_transfer___dependent_on_secondary_control_register=47,
    reserved10=48,
    reserved11=49,
    ioc_slv_requests=50,
    ioc_slv_snoop_reads=51,
    ioc_slv_snoop_writes=52,
    ioc_slv_interrupts=53,
    ioc_slv_syncs=54,
    ioc_slv_snoop_reads_ns=55,
    ioc_slv_snoop_writes_ns=56,
    ioc_slv_cpuwbs=57,
    ioc_slv_request_read_data_bytes__divided_by_4____dependent_on_secondary_control_register_=58,
    ioc_slv_request_write_data_bytes__divided_by_4____dependent_on_secondary_control_register_=59,
    ioc_slv_responses__beats_=60,
    ioc_slv_resp_read_data_bytes__divided_by_4_=61,
    ioc_slv_synccleans=62,
    reserved12=63,
    reserved13=64,
    reserved14=65,
    reserved15=66,
    reserved16=67,
    reserved17=68,
    reserved18=69,
    mc_0_fast_path_CPU_reads=70,
    mc_0_ordered_CPU_path_reads=71,
    mc_0_ordered_CPU_path_writes=72,
    mc_0_ordered_IOC_path_reads=73,
    mc_0_ordered_IOC_path_writes=74,
    mc_0_sync_a=75,
    mc_0_sync_b=76,
    reserved19=77,
    reserved20=78,
    reserved21=79,
    mc_1_fast_path_CPU_reads=80,
    mc_1_ordered_CPU_path_reads=81,
    mc_1_ordered_CPU_path_writes=82,
    mc_1_ordered_IOC_path_reads=83,
    mc_1_ordered_IOC_path_writes=84,
    mc_1_sync_a=85,
    mc_1_sync_b=86,
    reserved22=87,
    reserved23=88,
    reserved24=89,
    reserved25=90,
    reserved26=91,
    reserved27=92,
    reserved28=93,
    reserved29=94,
    reserved30=95,
    reserved31=96,
    reserved32=97,
    reserved33=98,
    reserved34=99,
    MCQF_nearfull=100,
    MCQDF_nearfull=101,
    IMQF_nearfull=102,
    PSF_nearfull=103,
    M0SF_nearfull=104,
    M1SF_nearfull=105,
    IMSF_nearfull=106,
    IMSDF_nearfull=107,
    ISQF__nearfull=108,
    ISSF_nearfull=109,
    ISYF_nearfull=110,
    CPYF_nearfull=111,
    PQF_nearfull=112,
    reserved35=113,
    reserved36=114,
    reserved37=115,
    reserved38=116,
    reserved39=117,
    reserved40=118,
    reserved41=119,
    reserved42=120,
    reserved43=121,
    reserved44=122,
    reserved45=123,
    reserved46=124,
    reserved47=125,
    reserved48=126,
    reserved49=127,
    reserved50=128,
    reserved51=129,
    reserved52=130,
    reserved53=131,
    reserved54=132,
    reserved55=133,
    reserved56=134,
    reserved57=135,
    reserved58=136,
    reserved59=137,
    reserved60=138,
    reserved61=139,
    total_memory_latency_fast_reads__must_be_divided_by_mem_read_requests_for_average_latency_=140,
    total_memory_latency_ordered_CPU_reads__must_be_divided_by=141,
    total_memory_latency_ordered_IOC_reads__must_be_divided_by=142,
    total_flush_latency_for_snoop_reads___must_be_divied=143,
    total_flush_latency_for_snoop_writes___must_be_divied=144,
    snoop_completion_buffer_full=145,
    reserved62=146,
    reserved63=147,
    reserved64=148,
    reserved65=149,
    MC0_slow_credit_count_is_zero=150,
    MC1_slow_credit_count_is_zero=151,
    MC0_fast_credit_count_is_zero=152,
    MC1_fast_credit_count_is_zero=153,
    reserved66=154,
    reserved67=155,
    FSB_slv_req_stall_for_command_credits=156,
    FSB_slv_req_stall_for_data_credits=157,
    FSB_PG_req_stall_for_cmd_credits=158,
    reserved68=159,
    reserved69=160,
    reserved70=161,
    reserved71=162,
    reserved72=163,
    reserved73=164,
    reserved74=165,
    reserved75=166,
    reserved76=167,
    reserved77=168,
    reserved78=169,
    reserved79=170,
    reserved80=171,
    reserved81=172,
    reserved82=173,
    reserved83=174,
    reserved84=175,
    reserved85=176,
    reserved86=177,
    reserved87=178,
    reserved88=179,
    reserved89=180,
    reserved90=181,
    reserved91=182,
    reserved92=183,
    reserved93=184,
    reserved94=185,
    reserved95=186,
    reserved96=187,
    reserved97=188,
    reserved98=189,
    reserved99=190,
    reserved100=191,
    reserved101=192,
    reserved102=193,
    reserved103=194,
    reserved104=195,
    reserved105=196,
    reserved106=197,
    reserved107=198,
    reserved108=199,
    SCLK__counts_sclks_in_the_mclk_domain_=200,
    IOC_MASTER_REQ_STALL__BIU_has_data_but_IOC_not_ready____sclk_domain_=201,
    IOC_SLAVE_RSP__STALL___sclk_domain_=202,
    reserved109=203,
    reserved110=204,
    reserved111=205,
    reserved112=206,
    reserved113=207,
    reserved114=208,
    reserved115=209,
    reserved116=210,
    reserved117=211,
    reserved118=212,
    reserved119=213,
    reserved120=214,
    reserved121=215,
    reserved122=216,
    reserved123=217,
    reserved124=218,
    reserved125=219,
    reserved126=220,
    reserved127=221,
    reserved128=222,
    reserved129=223,
    reserved130=224,
    reserved131=225,
    reserved132=226,
    reserved133=227,
    reserved134=228,
    reserved135=229,
    reserved136=230,
    reserved137=231,
    reserved138=232,
    reserved139=233,
    reserved140=234,
    reserved141=235,
    reserved142=236,
    reserved143=237,
    reserved144=238,
    reserved145=239,
    reserved146=240,
    reserved147=241,
    reserved148=242,
    reserved149=243,
    reserved150=244,
    reserved151=245,
    reserved152=246,
    reserved153=247,
    reserved154=248,
    reserved155=249,
    reserved156=250,
    reserved157=251,
    Increment_by__0__disable_count=252,
    Increment_by__1__used_to_count_clocks=253,
    Increment_by__63_used_for_simulation_to_rapidly_increment_counter=254,
    OTHERS=255
} BIU_PERFCOUNT_SELECT;

typedef enum _XCONFIG_DEVKIT_ENTRIES {
    XCONFIG_DEVKIT_ALL=0,
    XCONFIG_DEVKIT_USBD_ROOT_HUB_PORT_DISABLE_MASK=1,
    XCONFIG_DEVKIT_XAM_FEATURE_ENABLE_DISABLE_MASK=2
} _XCONFIG_DEVKIT_ENTRIES;

typedef enum GPUSIGN {
    GPUSIGN_UNSIGNED=0,
    GPUSIGN_SIGNED=1,
    GPUSIGN_BIAS=2,
    GPUSIGN_GAMMA=3
} GPUSIGN;

typedef enum _XCONFIG_DVD_ENTRIES {
    XCONFIG_DVD_ALL=0,
    XCONFIG_DVD_VOLUME_ID=1,
    XCONFIG_DVD_BOOKMARK=2
} _XCONFIG_DVD_ENTRIES;

typedef enum _XCONFIG_SECURED_ENTRIES {
    XCONFIG_SECURED_DATA=0,
    XCONFIG_SECURED_MAC_ADDRESS=1,
    XCONFIG_SECURED_AV_REGION=2,
    XCONFIG_SECURED_GAME_REGION=3,
    XCONFIG_SECURED_DVD_REGION=4,
    XCONFIG_SECURED_RESET_KEY=5,
    XCONFIG_SECURED_SYSTEM_FLAGS=6,
    XCONFIG_SECURED_POWER_MODE=7,
    XCONFIG_SECURED_ONLINE_NETWORK_ID=8,
    XCONFIG_SECURED_POWER_VCS_CONTROL=9
} _XCONFIG_SECURED_ENTRIES;

typedef enum TCR_PERFCOUNT_SELECT {
    DGMMPD_IPMUX0_STALL=0,
    DGMMPD_IPMUX1_STALL=1,
    DGMMPD_IPMUX2_STALL=2,
    DGMMPD_IPMUX3_STALL=3,
    DGMMPD_IPMUX_ALL_STALL=4,
    OPMUX0_L2_WRITES=5,
    OPMUX1_L2_WRITES=6,
    OPMUX2_L2_WRITES=7,
    OPMUX3_L2_WRITES=8
} TCR_PERFCOUNT_SELECT;

typedef struct _D3DPOINT _D3DPOINT, *P_D3DPOINT;

struct _D3DPOINT {
    long x;
    long y;
};

typedef enum _D3DCOUNTER {
    D3DCOUNTER_FRAMETIME=0,
    D3DCOUNTER_FRAMESPERSECOND=1,
    D3DCOUNTER_FRAMECOUNT=2,
    D3DCOUNTER_FRAME_BLOCKED_PERCENT=3,
    D3DCOUNTER_FRAME_THROTTLED_PERCENT=4,
    D3DCOUNTER_FRAME_ACTIVE_PERCENT=5,
    D3DCOUNTER_XAM_GPU_PERCENT=6,
    D3DCOUNTER_FRAME_GPU_IDLE_PERCENT=7,
    D3DCOUNTER_PRIMITIVES_CULLED_PERCENT=8,
    D3DCOUNTER_PRIMITIVES_CLIPPED_PERCENT=9,
    D3DCOUNTER_PRIMITIVES_VISIBLE_PERCENT=10,
    D3DCOUNTER_BANDWIDTH_SYSTEM=11,
    D3DCOUNTER_BANDWIDTH_INDICES=12,
    D3DCOUNTER_BANDWIDTH_TEXTURE=13,
    D3DCOUNTER_BANDWIDTH_VERTEX=14,
    D3DCOUNTER_BANDWIDTH_RESOLVE=15,
    D3DCOUNTER_BANDWIDTH_MEMEXPORT=16,
    D3DCOUNTER_XAM_COMMAND_BUFFER_SIZE=17,
    D3DCOUNTER_FRAME_GPU_WASTED_PERCENT=18,
    D3DCOUNTER_MAX=19
} _D3DCOUNTER;

typedef enum <unnamed-tag_000011AB> {
    SPR_BKMK=252
} <unnamed-tag_000011AB>;

typedef enum CFOpcode {
    NOP=0,
    EXECUTE=1,
    EXECUTE_END=2,
    COND_EXECUTE=3,
    COND_EXECUTE_END=4,
    COND_PRED_EXECUTE=5,
    COND_PRED_EXECUTE_END=6,
    LOOP_START=7,
    LOOP_END=8,
    COND_CALL=9,
    RETURN=10,
    COND_JMP=11,
    ALLOCATE=12,
    COND_EXECUTE_PRED_CLEAN=13,
    COND_EXECUTE_PRED_CLEAN_END=14,
    MARK_VS_FETCH_DONE=15
} CFOpcode;

typedef enum SwizzleType {
    NO_SWIZZLE=0,
    SHIFT_RIGHT_1=1,
    SHIFT_RIGHT_2=2,
    SHIFT_RIGHT_3=3
} SwizzleType;

typedef enum GPUCOMMANDOP {
    GPUCOMMANDOP_NOP=16,
    GPUCOMMANDOP_REG_RMW=33,
    GPUCOMMANDOP_DRAW=34,
    GPUCOMMANDOP_VIZ_QUERY=35,
    GPUCOMMANDOP_SET_STATE=37,
    GPUCOMMANDOP_WAIT_FOR_IDLE=38,
    GPUCOMMANDOP_LOAD_SHADER=39,
    GPUCOMMANDOP_LOAD_SHADER_IMMEDIATE=43,
    GPUCOMMANDOP_SET_CONSTANT=45,
    GPUCOMMANDOP_LOAD_ALU_CONSTANT=47,
    GPUCOMMANDOP_DRAW_IMMEDIATE=54,
    GPUCOMMANDOP_MPEG_INDEX=58,
    GPUCOMMANDOP_INVALIDATE_STATE=59,
    GPUCOMMANDOP_WAIT_REG_MEM=60,
    GPUCOMMANDOP_MEM_WRITE=61,
    GPUCOMMANDOP_REG_TO_MEM=62,
    GPUCOMMANDOP_INDIRECT_BUFFER=63,
    GPUCOMMANDOP_COND_WRITE=69,
    GPUCOMMANDOP_EVENT_WRITE=70,
    GPUCOMMANDOP_ME_INIT=72,
    GPUCOMMANDOP_FIX_2_FLT_REG=77,
    GPUCOMMANDOP_MEM_WRITE_COUNTER=79,
    GPUCOMMANDOP_WAIT_REG_EQ=82,
    GPUCOMMANDOP_WAIT_REG_GTE=83,
    GPUCOMMANDOP_CPU_INTERRUPT=84,
    GPUCOMMANDOP_EVENT_WRITE_SHADER=88,
    GPUCOMMANDOP_EVENT_WRITE_CACHE_FLUSH=89,
    GPUCOMMANDOP_EVENT_WRITE_SCREEN_EXTENT=90,
    GPUCOMMANDOP_EVENT_WRITE_ZPASS_DONE=91,
    GPUCOMMANDOP_CONTEXT_UPDATE=94,
    GPUCOMMANDOP_SET_BIN_MASK_LO=96,
    GPUCOMMANDOP_SET_BIN_MASK_HI=97,
    GPUCOMMANDOP_SET_BIN_SELECT_LO=98,
    GPUCOMMANDOP_SET_BIN_SELECT_HI=99
} GPUCOMMANDOP;

typedef enum <unnamed-tag_0000112D> {
    SPR_HIOR=55
} <unnamed-tag_0000112D>;

typedef enum GPUALLOCBUFFERSELECT {
    GPUALLOCBUFFERSELECT_POSITION=1,
    GPUALLOCBUFFERSELECT_COLORS=2,
    GPUALLOCBUFFERSELECT_INTERPOLATORS=2,
    GPUALLOCBUFFERSELECT_EXPORT=3
} GPUALLOCBUFFERSELECT;

typedef enum <unnamed-tag_00001191> {
    SPR_DABR=245
} <unnamed-tag_00001191>;

typedef enum GPUREGISTER {
    GPUREG_ALUCONSTANTS=0,
    GPUREG_BOOLEANCONSTANTS=0,
    GPUREG_CONTROLPACKET=0,
    GPUREG_DEPTHCONTROL=0,
    GPUREG_DESTINATIONPACKET=0,
    GPUREG_FETCHCONSTANTS=0,
    GPUREG_FLOWCONSTANTS=0,
    GPUREG_FLUSHFETCHCONSTANTS=0,
    GPUREG_GPRMANAGEMENT=0,
    GPUREG_MAXVTXINDX=0,
    GPUREG_MISCPACKET=0,
    GPUREG_PACKET0=0,
    GPUREG_PACKET2=0,
    GPUREG_PACKET4=0,
    GPUREG_PACKET6=0,
    GPUREG_SCLINECONTROL=0,
    GPUREG_SURFACEINFO=0,
    GPUREG_VALUESPACKET=0,
    GPUREG_AACONFIG=1,
    GPUREG_BCCONTROL=1,
    GPUREG_BLENDCONTROL0=1,
    GPUREG_COLOR0INFO=1,
    GPUREG_MINVTXINDX=1,
    GPUREG_COLORCONTROL=2,
    GPUREG_DEPTHINFO=2,
    GPUREG_INDXOFFSET=2,
    GPUREG_INSTSTOREMANAGEMENT=2,
    GPUREG_VTXCONTROL=2,
    GPUREG_COLOR1INFO=3,
    GPUREG_GBVERTCLIPADJ=3,
    GPUREG_HICONTROL=3,
    GPUREG_MULTIPRIMIBRESETINDX=3,
    GPUREG_CLIPCONTROL=4,
    GPUREG_COLOR2INFO=4,
    GPUREG_COLORMASK=4,
    GPUREG_GBVERTDISCADJ=4,
    GPUREG_INVALIDATECONSTANTS=4,
    GPUREG_BLENDRED=5,
    GPUREG_COLOR3INFO=5,
    GPUREG_GBHORZCLIPADJ=5,
    GPUREG_MODECONTROL=5,
    GPUREG_BLENDGREEN=6,
    GPUREG_COHERDESTBASE0=6,
    GPUREG_GBHORZDISCADJ=6,
    GPUREG_VTECONTROL=6,
    GPUREG_BLENDBLUE=7,
    GPUREG_COHERDESTBASE1=7,
    GPUREG_UNUSED3=7,
    GPUREG_VSCONST=7,
    GPUREG_BLENDALPHA=8,
    GPUREG_COHERDESTBASE2=8,
    GPUREG_EDRAMMODECONTROL=8,
    GPUREG_INTEGERCONSTANTS=8,
    GPUREG_PSCONST=8,
    GPUREG_BLENDCONTROL1=9,
    GPUREG_COHERDESTBASE3=9,
    GPUREG_DEBUGMISC0=9,
    GPUREG_UNUSED0=9,
    GPUREG_BLENDCONTROL2=10,
    GPUREG_COHERDESTBASE4=10,
    GPUREG_DEBUGMISC1=10,
    GPUREG_UNUSED1=10,
    GPUREG_BLENDCONTROL3=11,
    GPUREG_COHERDESTBASE5=11,
    GPUREG_UNUSED2=11,
    GPUREG_UNUSED7=11,
    GPUREG_COHERDESTBASE6=12,
    GPUREG_STENCILREFMASKBF=12,
    GPUREG_UNUSED8=12,
    GPUREG_COHERDESTBASE7=13,
    GPUREG_STENCILREFMASK=13,
    GPUREG_UNUSED9=13,
    GPUREG_ALPHAREF=14,
    GPUREG_SCREENSCISSORTL=14,
    GPUREG_UNUSED10=14,
    GPUREG_SCREENSCISSORBR=15,
    GPUREG_UNUSED11=15,
    GPUREG_VPORTXSCALE=15,
    GPUREG_UNUSED12=16,
    GPUREG_VPORTXOFFSET=16,
    GPUREG_UNUSED13=17,
    GPUREG_VPORTYSCALE=17,
    GPUREG_AAMASK=18,
    GPUREG_VPORTYOFFSET=18,
    GPUREG_UNUSED14=19,
    GPUREG_VPORTZSCALE=19,
    GPUREG_UNUSED15=20,
    GPUREG_VPORTZOFFSET=20,
    GPUREG_UNUSED16=21,
    GPUREG_VERTEXREUSEBLOCKCONTROL=22,
    GPUREG_OUTDEALLOCCONTROL=23,
    GPUREG_COPYCONTROL=24,
    GPUREG_COPYDESTBASE=25,
    GPUREG_COPYDESTPITCH=26,
    GPUREG_COPYDESTINFO=27,
    GPUREG_HICLEAR=28,
    GPUREG_DEPTHCLEAR=29,
    GPUREG_COLORCLEAR=30,
    GPUREG_COLORCLEARLO=31,
    GPUREG_COPYFUNC=32,
    GPUREG_COPYREF=33,
    GPUREG_COPYMASK=34,
    GPUREG_COPYSURFACESLICE=35,
    GPUREG_SAMPLECOUNTCONTROL=36,
    GPUREG_SAMPLECOUNTADDRESS=37,
    GPUREG_COHERSIZEHOST=47,
    GPUREG_COHERBASEHOST=48,
    GPUREG_COHERSTATUSHOST=49,
    GPUREG_CPUINTERRUPTACK=94,
    GPUREG_CALLBACKADDRESS=124,
    GPUREG_PACKET1=128,
    GPUREG_PACKET3=128,
    GPUREG_PACKET5=128,
    GPUREG_PACKET7=128,
    GPUREG_POINTPACKET=128,
    GPUREG_POINTSIZE=128,
    GPUREG_POLYOFFSETFRONTSCALE=128,
    GPUREG_PROGRAMCONTROL=128,
    GPUREG_PROGRAMPACKET=128,
    GPUREG_TESSELLATORPACKET=128,
    GPUREG_WINDOWOFFSET=128,
    GPUREG_WINDOWPACKET=128,
    GPUREG_CONTEXTMISC=129,
    GPUREG_POINTMINMAX=129,
    GPUREG_POLYOFFSETFRONTOFFSET=129,
    GPUREG_WINDOWSCISSORTL=129,
    GPUREG_INTERPOLATORCONTROL=130,
    GPUREG_LINECONTROL=130,
    GPUREG_POLYOFFSETBACKSCALE=130,
    GPUREG_WINDOWSCISSORBR=130,
    GPUREG_POLYOFFSETBACKOFFSET=131,
    GPUREG_UNUSED4=131,
    GPUREG_WRAPPING0=131,
    GPUREG_OUTPUTPATHCONTROL=132,
    GPUREG_POINTXRAD=132,
    GPUREG_WRAPPING1=132,
    GPUREG_HOSCONTROL=133,
    GPUREG_POINTYRAD=133,
    GPUREG_HOSMAXTESSLEVEL=134,
    GPUREG_POINTCONSTANTSIZE=134,
    GPUREG_HOSMINTESSLEVEL=135,
    GPUREG_POINTCULLRAD=135,
    GPUREG_CLIPPLANE0=136,
    GPUREG_HOSREUSEDEPTH=136,
    GPUREG_GROUPPRIMTYPE=137,
    GPUREG_GROUPFIRSTDECR=138,
    GPUREG_GROUPDECR=139,
    GPUREG_CLIPPLANE1=140,
    GPUREG_GROUPVECT0CONTROL=140,
    GPUREG_GROUPVECT1CONTROL=141,
    GPUREG_GROUPVECT0FMTCONTROL=142,
    GPUREG_GROUPVECT1FMTCONTROL=143,
    GPUREG_CLIPPLANE2=144,
    GPUREG_UNUSED5=144,
    GPUREG_UNUSED6=145,
    GPUREG_MPASSPSCONTROL=146,
    GPUREG_VIZQUERY=147,
    GPUREG_CLIPPLANE3=148,
    GPUREG_ENHANCE=148,
    GPUREG_CLIPPLANE4=152,
    GPUREG_CLIPPLANE5=156,
    GPUREG_WAITUNTIL=200
} GPUREGISTER;

typedef enum _XMEMCODEC_TYPE {
    XMEMCODEC_DEFAULT=0,
    XMEMCODEC_LZX=1
} _XMEMCODEC_TYPE;

typedef enum VGT_DI_INDEX_SIZE {
    DI_INDEX_SIZE_16_BIT=0,
    DI_INDEX_SIZE_32_BIT=1
} VGT_DI_INDEX_SIZE;

typedef enum GPUTEXTUREFORMAT {
    GPUTEXTUREFORMAT_1_REVERSE=0,
    GPUTEXTUREFORMAT_1=1,
    GPUTEXTUREFORMAT_8=2,
    GPUTEXTUREFORMAT_1_5_5_5=3,
    GPUTEXTUREFORMAT_5_6_5=4,
    GPUTEXTUREFORMAT_6_5_5=5,
    GPUTEXTUREFORMAT_8_8_8_8=6,
    GPUTEXTUREFORMAT_2_10_10_10=7,
    GPUTEXTUREFORMAT_8_A=8,
    GPUTEXTUREFORMAT_8_B=9,
    GPUTEXTUREFORMAT_8_8=10,
    GPUTEXTUREFORMAT_Cr_Y1_Cb_Y0_REP=11,
    GPUTEXTUREFORMAT_Y1_Cr_Y0_Cb_REP=12,
    GPUTEXTUREFORMAT_16_16_EDRAM=13,
    GPUTEXTUREFORMAT_8_8_8_8_A=14,
    GPUTEXTUREFORMAT_4_4_4_4=15,
    GPUTEXTUREFORMAT_10_11_11=16,
    GPUTEXTUREFORMAT_11_11_10=17,
    GPUTEXTUREFORMAT_DXT1=18,
    GPUTEXTUREFORMAT_DXT2_3=19,
    GPUTEXTUREFORMAT_DXT4_5=20,
    GPUTEXTUREFORMAT_16_16_16_16_EDRAM=21,
    GPUTEXTUREFORMAT_24_8=22,
    GPUTEXTUREFORMAT_24_8_FLOAT=23,
    GPUTEXTUREFORMAT_16=24,
    GPUTEXTUREFORMAT_16_16=25,
    GPUTEXTUREFORMAT_16_16_16_16=26,
    GPUTEXTUREFORMAT_16_EXPAND=27,
    GPUTEXTUREFORMAT_16_16_EXPAND=28,
    GPUTEXTUREFORMAT_16_16_16_16_EXPAND=29,
    GPUTEXTUREFORMAT_16_FLOAT=30,
    GPUTEXTUREFORMAT_16_16_FLOAT=31,
    GPUTEXTUREFORMAT_16_16_16_16_FLOAT=32,
    GPUTEXTUREFORMAT_32=33,
    GPUTEXTUREFORMAT_32_32=34,
    GPUTEXTUREFORMAT_32_32_32_32=35,
    GPUTEXTUREFORMAT_32_FLOAT=36,
    GPUTEXTUREFORMAT_32_32_FLOAT=37,
    GPUTEXTUREFORMAT_32_32_32_32_FLOAT=38,
    GPUTEXTUREFORMAT_32_AS_8=39,
    GPUTEXTUREFORMAT_32_AS_8_8=40,
    GPUTEXTUREFORMAT_16_MPEG=41,
    GPUTEXTUREFORMAT_16_16_MPEG=42,
    GPUTEXTUREFORMAT_8_INTERLACED=43,
    GPUTEXTUREFORMAT_32_AS_8_INTERLACED=44,
    GPUTEXTUREFORMAT_32_AS_8_8_INTERLACED=45,
    GPUTEXTUREFORMAT_16_INTERLACED=46,
    GPUTEXTUREFORMAT_16_MPEG_INTERLACED=47,
    GPUTEXTUREFORMAT_16_16_MPEG_INTERLACED=48,
    GPUTEXTUREFORMAT_DXN=49,
    GPUTEXTUREFORMAT_8_8_8_8_AS_16_16_16_16=50,
    GPUTEXTUREFORMAT_DXT1_AS_16_16_16_16=51,
    GPUTEXTUREFORMAT_DXT2_3_AS_16_16_16_16=52,
    GPUTEXTUREFORMAT_DXT4_5_AS_16_16_16_16=53,
    GPUTEXTUREFORMAT_2_10_10_10_AS_16_16_16_16=54,
    GPUTEXTUREFORMAT_10_11_11_AS_16_16_16_16=55,
    GPUTEXTUREFORMAT_11_11_10_AS_16_16_16_16=56,
    GPUTEXTUREFORMAT_32_32_32_FLOAT=57,
    GPUTEXTUREFORMAT_DXT3A=58,
    GPUTEXTUREFORMAT_DXT5A=59,
    GPUTEXTUREFORMAT_CTX1=60,
    GPUTEXTUREFORMAT_DXT3A_AS_1_1_1_1=61,
    GPUTEXTUREFORMAT_8_8_8_8_GAMMA_EDRAM=62,
    GPUTEXTUREFORMAT_2_10_10_10_FLOAT_EDRAM=63
} GPUTEXTUREFORMAT;

typedef enum _MEMORY_REGION_TYPE {
    MemoryRegionThread=0,
    MemoryRegionTitle=1,
    MemoryRegionSystem=2,
    MemoryRegionTitleDebug=3,
    MemoryRegionSystemDebug=4
} _MEMORY_REGION_TYPE;

typedef enum ScalarOpcode {
    ADDs=0,
    ADD_PREVs=1,
    MULs=2,
    MUL_PREVs=3,
    MUL_PREV2s=4,
    MAXs=5,
    MINs=6,
    SETEs=7,
    SETGTs=8,
    SETGTEs=9,
    SETNEs=10,
    FRACs=11,
    TRUNCs=12,
    FLOORs=13,
    EXP_IEEE=14,
    LOG_CLAMP=15,
    LOG_IEEE=16,
    RECIP_CLAMP=17,
    RECIP_FF=18,
    RECIP_IEEE=19,
    RECIPSQ_CLAMP=20,
    RECIPSQ_FF=21,
    RECIPSQ_IEEE=22,
    MOVAs=23,
    MOVA_FLOORs=24,
    SUBs=25,
    SUB_PREVs=26,
    PRED_SETEs=27,
    PRED_SETNEs=28,
    PRED_SETGTs=29,
    PRED_SETGTEs=30,
    PRED_SET_INVs=31,
    PRED_SET_POPs=32,
    PRED_SET_CLRs=33,
    PRED_SET_RESTOREs=34,
    KILLEs=35,
    KILLGTs=36,
    KILLGTEs=37,
    KILLNEs=38,
    KILLONEs=39,
    SQRT_IEEE=40,
    MUL_CONST_0=42,
    MUL_CONST_1=43,
    ADD_CONST_0=44,
    ADD_CONST_1=45,
    SUB_CONST_0=46,
    SUB_CONST_1=47,
    SIN=48,
    COS=49,
    RETAIN_PREV=50
} ScalarOpcode;

typedef enum GPUEXECSERIALIZEMODE {
    GPUEXECSERIALIZEMODE_UNSERIALIZED=0,
    GPUEXECSERIALIZEMODE_SERIALIZED=1
} GPUEXECSERIALIZEMODE;

typedef enum GPUCOLORARRAY {
    GPUCOLORARRAY_2D_COLOR=0,
    GPUCOLORARRAY_3D_SLICE_COLOR=1
} GPUCOLORARRAY;

typedef enum _PixFacility {
    PIXFAC_Default=0,
    PIXFAC_Audio=1
} _PixFacility;

typedef enum EdramMode {
    EDRAM_NOP=0,
    COLOR_DEPTH=4,
    DOUBLE_DEPTH=5,
    EDRAM_COPY=6
} EdramMode;

typedef enum _D3DXSHCOMPRESSQUALITYTYPE {
    D3DXSHCQUAL_FASTLOWQUALITY=1,
    D3DXSHCQUAL_SLOWHIGHQUALITY=2,
    D3DXSHCQUAL_FORCE_DWORD=255
} _D3DXSHCOMPRESSQUALITYTYPE;

typedef enum InputModifier {
    NIL=0,
    NEGATE=1
} InputModifier;

typedef enum _D3DTEXTURESTAGESTATETYPE {
    D3DTSS_COLOROP=1,
    D3DTSS_COLORARG1=2,
    D3DTSS_COLORARG2=3,
    D3DTSS_ALPHAOP=4,
    D3DTSS_ALPHAARG1=5,
    D3DTSS_ALPHAARG2=6,
    D3DTSS_BUMPENVMAT00=7,
    D3DTSS_BUMPENVMAT01=8,
    D3DTSS_BUMPENVMAT10=9,
    D3DTSS_BUMPENVMAT11=10,
    D3DTSS_TEXCOORDINDEX=11,
    D3DTSS_BUMPENVLSCALE=22,
    D3DTSS_BUMPENVLOFFSET=23,
    D3DTSS_TEXTURETRANSFORMFLAGS=24,
    D3DTSS_COLORARG0=26,
    D3DTSS_ALPHAARG0=27,
    D3DTSS_RESULTARG=28,
    D3DTSS_CONSTANT=32,
    D3DTSS_MAX=33,
    D3DTSS_FORCE_DWORD=255
} _D3DTEXTURESTAGESTATETYPE;

typedef union BIOS_5_SCRATCH BIOS_5_SCRATCH, *PBIOS_5_SCRATCH;


/* WARNING! conflicting data type names: /iw5mp_xenon.pdb/BIOS_5_SCRATCH/<unnamed-type-bitfields> - /iw5mp_xenon.pdb/D1SCL_COEF_RAM_SELECT/<unnamed-type-bitfields> */

union BIOS_5_SCRATCH {
    struct <unnamed-type-bitfields> bitfields;
    struct <unnamed-type-bitfields> bits;
    uint u32All;
    int i32All;
    float f32All;
};

typedef enum VGT_DI_MAJOR_MODE_SELECT {
    DI_MAJOR_MODE_0=0,
    DI_MAJOR_MODE_1=1
} VGT_DI_MAJOR_MODE_SELECT;

typedef struct _XCONTENT_DATA _XCONTENT_DATA, *P_XCONTENT_DATA;

struct _XCONTENT_DATA {
    ulong DeviceID;
    ulong dwContentType;
    wchar_t szDisplayName[128];
    char szFileName[42];
};

typedef enum CompareRef {
    REF_NEVER=0,
    REF_LESS=1,
    REF_EQUAL=2,
    REF_LEQUAL=3,
    REF_GREATER=4,
    REF_NOTEQUAL=5,
    REF_GEQUAL=6,
    REF_ALWAYS=7
} CompareRef;

typedef enum _XSECURITY_STATUS_CHECKS {
    XSECURITY_STATUS_ANY=0,
    XSECURITY_STATUS_VERSION_CHECK=1,
    XSECURITY_STATUS_DISC_CHECK=2,
    XSECURITY_STATUS_MAXIMUM=3
} _XSECURITY_STATUS_CHECKS;

typedef enum _NT_PRODUCT_TYPE {
    NtProductWinNt=1,
    NtProductLanManNt=2,
    NtProductServer=3
} _NT_PRODUCT_TYPE;

typedef enum _D3DVS_RASTOUT_OFFSETS {
    D3DSRO_POSITION=0,
    D3DSRO_FOG=1,
    D3DSRO_POINT_SIZE=2,
    D3DSRO_FORCE_DWORD=255
} _D3DVS_RASTOUT_OFFSETS;

typedef enum GPUEXECINSTRUCTIONTYPE {
    GPUEXECINSTRUCTIONTYPE_ALU=0,
    GPUEXECINSTRUCTIONTYPE_FETCH=1
} GPUEXECINSTRUCTIONTYPE;

typedef enum SurfaceEndian {
    ENDIAN_NONE=0,
    ENDIAN_8IN16=1,
    ENDIAN_8IN32=2,
    ENDIAN_16IN32=3,
    ENDIAN_8IN64=4,
    ENDIAN_8IN128=5
} SurfaceEndian;

typedef enum CopySrcSelect {
    SRC_CRT0=0,
    SRC_CRT1=1,
    SRC_CRT2=2,
    SRC_CRT3=3,
    SRC_Z=4
} CopySrcSelect;

typedef enum _D3DSHADER_ADDRESSMODE_TYPE {
    D3DSHADER_ADDRMODE_ABSOLUTE=0,
    D3DSHADER_ADDRMODE_RELATIVE=0,
    D3DSHADER_ADDRMODE_FORCE_DWORD=255
} _D3DSHADER_ADDRESSMODE_TYPE;

typedef enum <unnamed-tag_0000114D> {
    SPR_TLB_HINT=178
} <unnamed-tag_0000114D>;

typedef enum _XPROFILE_RACE_TRANSMISSION_OPTIONS {
    XPROFILE_RACE_TRANSMISSION_AUTO=0,
    XPROFILE_RACE_TRANSMISSION_MANUAL=1
} _XPROFILE_RACE_TRANSMISSION_OPTIONS;

typedef enum GPUBORDERCOLOR {
    GPUBORDERCOLOR_ABGR_BLACK=0,
    GPUBORDERCOLOR_ABGR_WHITE=1,
    GPUBORDERCOLOR_ACBYCR_BLACK=2,
    GPUBORDERCOLOR_ACBCRY_BLACK=3
} GPUBORDERCOLOR;

typedef enum GPUCOPYCOMMAND {
    GPUCOPYCOMMAND_RAW=0,
    GPUCOPYCOMMAND_CONVERT=1,
    GPUCOPYCOMMAND_1_1_1_1=2,
    GPUCOPYCOMMAND_NULL=3
} GPUCOPYCOMMAND;

typedef enum PixPMCPBCycleCost {
    CycleCost_1=0,
    CycleCost_2=1,
    CycleCost_5=2,
    CycleCost_11=3,
    CycleCost_40=4,
    CycleCost_685=5,
    CycleCost_Count=6
} PixPMCPBCycleCost;

typedef enum _MEMORY_UNIT_TYPE {
    MemoryUnitBuiltInFlash=0,
    MemoryUnitBuiltInMmc=0,
    MemoryUnitBuiltInUsbMass=0,
    MemoryUnitSlotA=0,
    MemoryUnitSlotB=0,
    MemoryUnitUsbMass=0
} _MEMORY_UNIT_TYPE;

typedef enum _D3DRENDERSTATETYPE {
    D3DRS_WRAP4=0,
    D3DRS_WRAP5=4,
    D3DRS_WRAP6=8,
    D3DRS_WRAP7=12,
    D3DRS_WRAP8=16,
    D3DRS_WRAP9=20,
    D3DRS_WRAP10=24,
    D3DRS_WRAP11=28,
    D3DRS_WRAP12=32,
    D3DRS_WRAP13=36,
    D3DRS_WRAP14=40,
    D3DRS_ZENABLE=40,
    D3DRS_WRAP15=44,
    D3DRS_ZFUNC=44,
    D3DRS_VIEWPORTENABLE=48,
    D3DRS_ZWRITEENABLE=48,
    D3DRS_FILLMODE=52,
    D3DRS_HIGHPRECISIONBLENDENABLE=52,
    D3DRS_CULLMODE=56,
    D3DRS_HIGHPRECISIONBLENDENABLE1=56,
    D3DRS_ALPHABLENDENABLE=60,
    D3DRS_HIGHPRECISIONBLENDENABLE2=60,
    D3DRS_HIGHPRECISIONBLENDENABLE3=64,
    D3DRS_SEPARATEALPHABLENDENABLE=64,
    D3DRS_BLENDFACTOR=68,
    D3DRS_HALFPIXELOFFSET=68,
    D3DRS_PRIMITIVERESETENABLE=72,
    D3DRS_SRCBLEND=72,
    D3DRS_DESTBLEND=76,
    D3DRS_PRIMITIVERESETINDEX=76,
    D3DRS_ALPHATOMASKENABLE=80,
    D3DRS_BLENDOP=80,
    D3DRS_ALPHATOMASKOFFSETS=84,
    D3DRS_SRCBLENDALPHA=84,
    D3DRS_DESTBLENDALPHA=88,
    D3DRS_GUARDBAND_X=88,
    D3DRS_BLENDOPALPHA=92,
    D3DRS_GUARDBAND_Y=92,
    D3DRS_ALPHATESTENABLE=96,
    D3DRS_DISCARDBAND_X=96,
    D3DRS_ALPHAREF=100,
    D3DRS_DISCARDBAND_Y=100,
    D3DRS_ALPHAFUNC=104,
    D3DRS_HISTENCILENABLE=104,
    D3DRS_HISTENCILWRITEENABLE=108,
    D3DRS_STENCILENABLE=108,
    D3DRS_HISTENCILFUNC=112,
    D3DRS_TWOSIDEDSTENCILMODE=112,
    D3DRS_HISTENCILREF=116,
    D3DRS_STENCILFAIL=116,
    D3DRS_PRESENTINTERVAL=120,
    D3DRS_STENCILZFAIL=120,
    D3DRS_PRESENTIMMEDIATETHRESHOLD=124,
    D3DRS_STENCILPASS=124,
    D3DRS_HIZENABLE=128,
    D3DRS_STENCILFUNC=128,
    D3DRS_HIZWRITEENABLE=132,
    D3DRS_STENCILREF=132,
    D3DRS_LASTPIXEL=136,
    D3DRS_STENCILMASK=136,
    D3DRS_LINEWIDTH=140,
    D3DRS_STENCILWRITEMASK=140,
    D3DRS_BUFFER2FRAMES=144,
    D3DRS_CCW_STENCILFAIL=144,
    D3DRS_CCW_STENCILZFAIL=148,
    D3DRS_MAX=148,
    D3DRS_CCW_STENCILPASS=152,
    D3DRS_CCW_STENCILFUNC=156,
    D3DRS_CCW_STENCILREF=160,
    D3DRS_CCW_STENCILMASK=164,
    D3DRS_CCW_STENCILWRITEMASK=168,
    D3DRS_CLIPPLANEENABLE=172,
    D3DRS_POINTSIZE=176,
    D3DRS_POINTSIZE_MIN=180,
    D3DRS_POINTSPRITEENABLE=184,
    D3DRS_POINTSIZE_MAX=188,
    D3DRS_MULTISAMPLEANTIALIAS=192,
    D3DRS_MULTISAMPLEMASK=196,
    D3DRS_SCISSORTESTENABLE=200,
    D3DRS_SLOPESCALEDEPTHBIAS=204,
    D3DRS_DEPTHBIAS=208,
    D3DRS_COLORWRITEENABLE=212,
    D3DRS_COLORWRITEENABLE1=216,
    D3DRS_COLORWRITEENABLE2=220,
    D3DRS_COLORWRITEENABLE3=224,
    D3DRS_TESSELLATIONMODE=228,
    D3DRS_MINTESSELLATIONLEVEL=232,
    D3DRS_MAXTESSELLATIONLEVEL=236,
    D3DRS_WRAP0=240,
    D3DRS_WRAP1=244,
    D3DRS_WRAP2=248,
    D3DRS_WRAP3=252,
    D3DRS_FORCE_DWORD=255
} _D3DRENDERSTATETYPE;

typedef enum CP_CONST_ID {
    CP_CONST_ID_ALU=0,
    CP_CONST_ID_FETCH=1,
    CP_CONST_ID_BOOLEAN=2,
    CP_CONST_ID_LOOP=3,
    CP_CONST_ID_REGISTER=4
} CP_CONST_ID;

typedef enum SU_PERFCNT_SELECT {
    PERF_PAPC_PASX_REQ=0,
    PERF_PAPC_PASX_DISABLE_PIPE=1,
    PERF_PAPC_PASX_FIRST_VECTOR=2,
    PERF_PAPC_PASX_SECOND_VECTOR=3,
    PERF_PAPC_PASX_FIRST_DEAD=4,
    PERF_PAPC_PASX_SECOND_DEAD=5,
    PERF_PAPC_PASX_VTX_KILL_DISCARD=6,
    PERF_PAPC_PASX_VTX_NAN_DISCARD=7,
    PERF_PAPC_PA_INPUT_PRIM=8,
    PERF_PAPC_PA_INPUT_NULL_PRIM=9,
    PERF_PAPC_PA_INPUT_EVENT_FLAG=10,
    PERF_PAPC_PA_INPUT_FIRST_PRIM_SLOT=11,
    PERF_PAPC_PA_INPUT_END_OF_PACKET=12,
    PERF_PAPC_CLPR_CULL_PRIM=13,
    PERF_PAPC_CLPR_VVUCP_CULL_PRIM=14,
    PERF_PAPC_CLPR_VV_CULL_PRIM=15,
    PERF_PAPC_CLPR_UCP_CULL_PRIM=16,
    PERF_PAPC_CLPR_VTX_KILL_CULL_PRIM=17,
    PERF_PAPC_CLPR_VTX_NAN_CULL_PRIM=18,
    PERF_PAPC_CLPR_CULL_TO_NULL_PRIM=19,
    PERF_PAPC_CLPR_VVUCP_CLIP_PRIM=20,
    PERF_PAPC_CLPR_VV_CLIP_PRIM=21,
    PERF_PAPC_CLPR_UCP_CLIP_PRIM=22,
    PERF_PAPC_CLPR_POINT_CLIP_CANDIDATE=23,
    PERF_PAPC_CLPR_CLIP_PLANE_CNT_1=24,
    PERF_PAPC_CLPR_CLIP_PLANE_CNT_2=25,
    PERF_PAPC_CLPR_CLIP_PLANE_CNT_3=26,
    PERF_PAPC_CLPR_CLIP_PLANE_CNT_4=27,
    PERF_PAPC_CLPR_CLIP_PLANE_CNT_5_8=28,
    PERF_PAPC_CLPR_CLIP_PLANE_CNT_9_12=29,
    PERF_PAPC_CLPR_CLIP_PLANE_NEAR=30,
    PERF_PAPC_CLPR_CLIP_PLANE_FAR=31,
    PERF_PAPC_CLPR_CLIP_PLANE_LEFT=32,
    PERF_PAPC_CLPR_CLIP_PLANE_RIGHT=33,
    PERF_PAPC_CLPR_CLIP_PLANE_TOP=34,
    PERF_PAPC_CLPR_CLIP_PLANE_BOTTOM=35,
    PERF_PAPC_CLSM_NULL_PRIM=36,
    PERF_PAPC_CLSM_TOTALLY_VISIBLE_PRIM=37,
    PERF_PAPC_CLSM_CLIP_PRIM=38,
    PERF_PAPC_CLSM_CULL_TO_NULL_PRIM=39,
    PERF_PAPC_CLSM_OUT_PRIM_CNT_1=40,
    PERF_PAPC_CLSM_OUT_PRIM_CNT_2=41,
    PERF_PAPC_CLSM_OUT_PRIM_CNT_3=42,
    PERF_PAPC_CLSM_OUT_PRIM_CNT_4=43,
    PERF_PAPC_CLSM_OUT_PRIM_CNT_5_8=44,
    PERF_PAPC_CLSM_OUT_PRIM_CNT_9_13=45,
    PERF_PAPC_CLSM_NON_TRIVIAL_CULL=46,
    PERF_PAPC_SU_INPUT_PRIM=47,
    PERF_PAPC_SU_INPUT_CLIP_PRIM=48,
    PERF_PAPC_SU_INPUT_NULL_PRIM=49,
    PERF_PAPC_SU_ZERO_AREA_CULL_PRIM=50,
    PERF_PAPC_SU_BACK_FACE_CULL_PRIM=51,
    PERF_PAPC_SU_FRONT_FACE_CULL_PRIM=52,
    PERF_PAPC_SU_POLYMODE_FACE_CULL=53,
    PERF_PAPC_SU_POLYMODE_BACK_CULL=54,
    PERF_PAPC_SU_POLYMODE_FRONT_CULL=55,
    PERF_PAPC_SU_POLYMODE_INVALID_FILL=56,
    PERF_PAPC_SU_OUTPUT_PRIM=57,
    PERF_PAPC_SU_OUTPUT_CLIP_PRIM=58,
    PERF_PAPC_SU_OUTPUT_NULL_PRIM=59,
    PERF_PAPC_SU_OUTPUT_EVENT_FLAG=60,
    PERF_PAPC_SU_OUTPUT_FIRST_PRIM_SLOT=61,
    PERF_PAPC_SU_OUTPUT_END_OF_PACKET=62,
    PERF_PAPC_SU_OUTPUT_POLYMODE_FACE=63,
    PERF_PAPC_SU_OUTPUT_POLYMODE_BACK=64,
    PERF_PAPC_SU_OUTPUT_POLYMODE_FRONT=65,
    PERF_PAPC_SU_OUT_CLIP_POLYMODE_FACE=66,
    PERF_PAPC_SU_OUT_CLIP_POLYMODE_BACK=67,
    PERF_PAPC_SU_OUT_CLIP_POLYMODE_FRONT=68,
    PERF_PAPC_PASX_REQ_IDLE=69,
    PERF_PAPC_PASX_REQ_BUSY=70,
    PERF_PAPC_PASX_REQ_STALLED=71,
    PERF_PAPC_PASX_REC_IDLE=72,
    PERF_PAPC_PASX_REC_BUSY=73,
    PERF_PAPC_PASX_REC_STARVED_SX=74,
    PERF_PAPC_PASX_REC_STALLED=75,
    PERF_PAPC_PASX_REC_STALLED_POS_MEM=76,
    PERF_PAPC_PASX_REC_STALLED_CCGSM_IN=77,
    PERF_PAPC_CCGSM_IDLE=78,
    PERF_PAPC_CCGSM_BUSY=79,
    PERF_PAPC_CCGSM_STALLED=80,
    PERF_PAPC_CLPRIM_IDLE=81,
    PERF_PAPC_CLPRIM_BUSY=82,
    PERF_PAPC_CLPRIM_STALLED=83,
    PERF_PAPC_CLPRIM_STARVED_CCGSM=84,
    PERF_PAPC_CLIPSM_IDLE=85,
    PERF_PAPC_CLIPSM_BUSY=86,
    PERF_PAPC_CLIPSM_WAIT_CLIP_VERT_ENGH=87,
    PERF_PAPC_CLIPSM_WAIT_HIGH_PRI_SEQ=88,
    PERF_PAPC_CLIPSM_WAIT_CLIPGA=89,
    PERF_PAPC_CLIPSM_WAIT_AVAIL_VTE_CLIP=90,
    PERF_PAPC_CLIPSM_WAIT_CLIP_OUTSM=91,
    PERF_PAPC_CLIPGA_IDLE=92,
    PERF_PAPC_CLIPGA_BUSY=93,
    PERF_PAPC_CLIPGA_STARVED_VTE_CLIP=94,
    PERF_PAPC_CLIPGA_STALLED=95,
    PERF_PAPC_CLIP_IDLE=96,
    PERF_PAPC_CLIP_BUSY=97,
    PERF_PAPC_SU_IDLE=98,
    PERF_PAPC_SU_BUSY=99,
    PERF_PAPC_SU_STARVED_CLIP=100,
    PERF_PAPC_SU_STALLED_SC=101
} SU_PERFCNT_SELECT;

typedef enum _D3DTRILINEARTHRESHOLD {
    D3DTRILINEAR_IMMEDIATE=0,
    D3DTRILINEAR_ONESIXTH=1,
    D3DTRILINEAR_ONEFOURTH=2,
    D3DTRILINEAR_THREEEIGHTHS=3,
    D3DTRILINEAR_FORCE_DWORD=255
} _D3DTRILINEARTHRESHOLD;

typedef enum _XPROFILE_GAMERCARD_ZONE_OPTIONS {
    XPROFILE_GAMERCARD_ZONE_XBOX_1=0,
    XPROFILE_GAMERCARD_ZONE_RR=1,
    XPROFILE_GAMERCARD_ZONE_PRO=2,
    XPROFILE_GAMERCARD_ZONE_FAMILY=3,
    XPROFILE_GAMERCARD_ZONE_UNDERGROUND=4
} _XPROFILE_GAMERCARD_ZONE_OPTIONS;

typedef enum HZ_PERFCOUNT_SELECT {
    WRC_1VAL_QUADS=0,
    WRC_2VAL_QUADS=1,
    WRC_3VAL_QUADS=2,
    WRC_4VAL_QUADS=3,
    HZ_WR_BUSY=4,
    HZ_SPARE0=5,
    RDC_TILE_HIT=6,
    RDC_STAGE3_STALL=7,
    HZ_SPARE1=8,
    HZ_SPARE2=9,
    QUADS_KEPT=10,
    QUADS_ZCULL=11,
    QUADS_SCULL=12,
    QUADS_SZCULL=13,
    HZ_RE_BUSY=14,
    BC_SCLK_COUNT=15,
    BC_HZ_VALID0=16,
    BC_HZ_VALID1=17,
    BC_HZ_VALID2=18,
    BC_HZ_VALID3=19,
    SC_SCLK_COUNT=20,
    SC_HZ_COARSE_SND=21,
    HZ_SC_COARSE_HLD=22,
    HZ_SC_HIER_SND=23,
    SC_HZ_HIER_HLD=24,
    HZ_BC_TILE_SND=25,
    BC_HZ_TILE_HLD=26,
    SC_STALL_HZ=27,
    BC_STALL_HZ=28,
    EVENT_STALL=29,
    SC_STARVE_HZ_BUSY=30,
    SC_STARVE_HZ_IDLE=31
} HZ_PERFCOUNT_SELECT;

typedef enum DumpDedicatedBlockId {
    DumpDedicatedBlockEtxObjectId=0,
    DumpDedicatedBlockEtxObjectPointerId=1,
    DumpDedicatedBlockEtxBufferId=2,
    DumpDedicatedBlockLiveHiveTableId=3,
    DumpDedicatedBlockLiveHiveBufferId=4,
    DumpDedicatedBlockXamUIThreadId=5,
    DumpDedicatedBlockXamTitleLoaderPointerId=6,
    DumpDedicatedBlockXamTitleLoaderObjectId=7,
    DumpDedicatedBlockMaxDedicatedBlockId=8
} DumpDedicatedBlockId;

typedef enum _XPROFILE_GAMER_YAXIS_INVERSION_OPTIONS {
    XPROFILE_YAXIS_INVERSION_OFF=0,
    XPROFILE_YAXIS_INVERSION_ON=1
} _XPROFILE_GAMER_YAXIS_INVERSION_OPTIONS;

typedef union DMIF_DEBUG_ID DMIF_DEBUG_ID, *PDMIF_DEBUG_ID;


/* WARNING! conflicting data type names: /iw5mp_xenon.pdb/DMIF_DEBUG_ID/<unnamed-type-bitfields> - /iw5mp_xenon.pdb/D1SCL_COEF_RAM_SELECT/<unnamed-type-bitfields> */

union DMIF_DEBUG_ID {
    struct <unnamed-type-bitfields> bitfields;
    struct <unnamed-type-bitfields> bits;
    uint u32All;
    int i32All;
    float f32All;
};

typedef enum GPUHIZFUNC {
    GPUHIZFUNC_LESS_EQUAL=0,
    GPUHIZFUNC_GREATER_EQUAL=1
} GPUHIZFUNC;

typedef enum GPUINITIATOR {
    GPUINITIATOR_VS_DEALLOC=0,
    GPUINITIATOR_PS_DEALLOC=1,
    GPUINITIATOR_VS_DONE_WRITE_BACK=2,
    GPUINITIATOR_PS_DONE_WRITE_BACK=3,
    GPUINITIATOR_CACHE_FLUSH_WRITE_BACK=4,
    GPUINITIATOR_CONTEXT_DONE=5,
    GPUINITIATOR_CACHE_FLUSH=6,
    GPUINITIATOR_VIZQUERY_START=7,
    GPUINITIATOR_VIZQUERY_END=8,
    GPUINITIATOR_SC_WAIT_WC=9,
    GPUINITIATOR_MPASS_PS_CP_REFETCH=10,
    GPUINITIATOR_MPASS_PS_RST_START=11,
    GPUINITIATOR_MPASS_PS_INCR_START=12,
    GPUINITIATOR_RST_PIX_CNT=13,
    GPUINITIATOR_RST_VTX_CNT=14,
    GPUINITIATOR_TILE_FLUSH=15,
    GPUINITIATOR_CACHE_FLUSH_AND_INV_WRITE_BACK_EVENT=20,
    GPUINITIATOR_ZPASS_DONE=21,
    GPUINITIATOR_CACHE_FLUSH_AND_INV_EVENT=22,
    GPUINITIATOR_PERFCOUNTER_START=23,
    GPUINITIATOR_PERFCOUNTER_STOP=24,
    GPUINITIATOR_SCREEN_EXT_INIT=25,
    GPUINITIATOR_SCREEN_EXT_RPT=26,
    GPUINITIATOR_VS_FETCH_DONE_WRITE_BACK=27
} GPUINITIATOR;

typedef union PCIE_PERF_COUNT0_SLV_S_C_CLK PCIE_PERF_COUNT0_SLV_S_C_CLK, *PPCIE_PERF_COUNT0_SLV_S_C_CLK;


/* WARNING! conflicting data type names: /iw5mp_xenon.pdb/PCIE_PERF_COUNT0_SLV_S_C_CLK/<unnamed-type-bitfields> - /iw5mp_xenon.pdb/D1SCL_COEF_RAM_SELECT/<unnamed-type-bitfields> */

union PCIE_PERF_COUNT0_SLV_S_C_CLK {
    struct <unnamed-type-bitfields> bitfields;
    struct <unnamed-type-bitfields> bits;
    uint u32All;
    int i32All;
    float f32All;
};

typedef enum GPUFILLMODE {
    GPUFILL_POINT=0,
    GPUFILL_WIREFRAME=1,
    GPUFILL_SOLID=2
} GPUFILLMODE;

typedef struct _PROCESS_HEAP_ENTRY _PROCESS_HEAP_ENTRY, *P_PROCESS_HEAP_ENTRY;

typedef union _PROCESS_HEAP_ENTRY_u_12 _PROCESS_HEAP_ENTRY_u_12, *P_PROCESS_HEAP_ENTRY_u_12;

typedef struct <unnamed-type-Block> <unnamed-type-Block>, *P<unnamed-type-Block>;

typedef struct <unnamed-type-Region> <unnamed-type-Region>, *P<unnamed-type-Region>;

struct <unnamed-type-Region> {
    ulong dwCommittedSize;
    ulong dwUnCommittedSize;
    void *lpFirstBlock;
    void *lpLastBlock;
};

struct <unnamed-type-Block> {
    void *hMem;
    ulong dwReserved[3];
};

union _PROCESS_HEAP_ENTRY_u_12 {
    struct <unnamed-type-Block> Block;
    struct <unnamed-type-Region> Region;
};

struct _PROCESS_HEAP_ENTRY {
    void *lpData;
    ulong cbData;
    uchar cbOverhead;
    uchar iRegionIndex;
    ushort wFlags;
    union _PROCESS_HEAP_ENTRY_u_12 field5_0xc;
};

typedef enum <unnamed-tag_0000114F> {
    SPR_TLB_INDEX=179
} <unnamed-tag_0000114F>;

typedef enum _FP_IEEE_OPERATION_CODE {
    FpCodeUnspecified=0,
    FpCodeAdd=1,
    FpCodeSubtract=2,
    FpCodeMultiply=3,
    FpCodeDivide=4,
    FpCodeSquareRoot=5,
    FpCodeRemainder=6,
    FpCodeCompare=7,
    FpCodeConvert=8,
    FpCodeRound=9,
    FpCodeTruncate=10,
    FpCodeFloor=11,
    FpCodeCeil=12,
    FpCodeAcos=13,
    FpCodeAsin=14,
    FpCodeAtan=15,
    FpCodeAtan2=16,
    FpCodeCabs=17,
    FpCodeCos=18,
    FpCodeCosh=19,
    FpCodeExp=20,
    FpCodeFabs=21,
    FpCodeFmod=22,
    FpCodeFrexp=23,
    FpCodeHypot=24,
    FpCodeLdexp=25,
    FpCodeLog=26,
    FpCodeLog10=27,
    FpCodeModf=28,
    FpCodePow=29,
    FpCodeSin=30,
    FpCodeSinh=31,
    FpCodeTan=32,
    FpCodeTanh=33,
    FpCodeY0=34,
    FpCodeY1=35,
    FpCodeYn=36
} _FP_IEEE_OPERATION_CODE;

typedef enum SurfaceNumber {
    NUMBER_UREPEAT=0,
    NUMBER_SREPEAT=1,
    NUMBER_UINTEGER=2,
    NUMBER_SINTEGER=3,
    NUMBER_GAMMA=4,
    NUMBER_FIXED=5,
    NUMBER_FLOAT=7
} SurfaceNumber;

typedef union CP_RT10_COMMAND CP_RT10_COMMAND, *PCP_RT10_COMMAND;


/* WARNING! conflicting data type names: /iw5mp_xenon.pdb/CP_RT10_COMMAND/<unnamed-type-bitfields> - /iw5mp_xenon.pdb/D1SCL_COEF_RAM_SELECT/<unnamed-type-bitfields> */

union CP_RT10_COMMAND {
    struct <unnamed-type-bitfields> bitfields;
    struct <unnamed-type-bitfields> bits;
    uint u32All;
    int i32All;
    float f32All;
};

typedef enum _XPRIVILEGE_TYPE {
    XPRIVILEGE_SHARE_CONTENT_OUTSIDE_LIVE=211,
    XPRIVILEGE_UNSAFE_PROGRAMMING=212,
    XPRIVILEGE_CONTENT_AUTHOR=222,
    XPRIVILEGE_VIDEO_COMMUNICATIONS_FRIENDS_ONLY=234,
    XPRIVILEGE_VIDEO_COMMUNICATIONS=235,
    XPRIVILEGE_TRADE_CONTENT=238,
    XPRIVILEGE_PRESENCE_FRIENDS_ONLY=243,
    XPRIVILEGE_PRESENCE=244,
    XPRIVILEGE_PURCHASE_CONTENT=245,
    XPRIVILEGE_USER_CREATED_CONTENT_FRIENDS_ONLY=246,
    XPRIVILEGE_USER_CREATED_CONTENT=247,
    XPRIVILEGE_PROFILE_VIEWING_FRIENDS_ONLY=248,
    XPRIVILEGE_PROFILE_VIEWING=249,
    XPRIVILEGE_COMMUNICATIONS_FRIENDS_ONLY=251,
    XPRIVILEGE_COMMUNICATIONS=252,
    XPRIVILEGE_MULTIPLAYER_SESSIONS=254
} _XPRIVILEGE_TYPE;

typedef union RBBM_PERFCOUNTER2_HI RBBM_PERFCOUNTER2_HI, *PRBBM_PERFCOUNTER2_HI;


/* WARNING! conflicting data type names: /iw5mp_xenon.pdb/RBBM_PERFCOUNTER2_HI/<unnamed-type-bitfields> - /iw5mp_xenon.pdb/D1SCL_COEF_RAM_SELECT/<unnamed-type-bitfields> */

union RBBM_PERFCOUNTER2_HI {
    struct <unnamed-type-bitfields> bitfields;
    struct <unnamed-type-bitfields> bits;
    uint u32All;
    int i32All;
    float f32All;
};

typedef enum GPUANISOFILTER {
    GPUANISOFILTER_DISABLED=0,
    GPUANISOFILTER_MAX1TO1=1,
    GPUANISOFILTER_MAX2TO1=2,
    GPUANISOFILTER_MAX4TO1=3,
    GPUANISOFILTER_MAX8TO1=4,
    GPUANISOFILTER_MAX16TO1=5,
    GPUANISOFILTER_KEEP=7
} GPUANISOFILTER;

typedef enum DCPERFREGISTER {
    DCPERFREG_DC_PERFCOUNTER0_SELECT=200,
    DCPERFREG_DC_PERFCOUNTER0_HI=201,
    DCPERFREG_DC_PERFCOUNTER0_LOW=202,
    DCPERFREG_DC_PERFCOUNTER1_SELECT=203,
    DCPERFREG_DC_PERFCOUNTER1_HI=204,
    DCPERFREG_DC_PERFCOUNTER1_LOW=205
} DCPERFREGISTER;

typedef enum UCODESHADERMAGIC {
    UCODESHADERMAGIC_CURRENT=17
} UCODESHADERMAGIC;

typedef enum CompareFrag {
    FRAG_NEVER=0,
    FRAG_LESS=1,
    FRAG_EQUAL=2,
    FRAG_LEQUAL=3,
    FRAG_GREATER=4,
    FRAG_NOTEQUAL=5,
    FRAG_GEQUAL=6,
    FRAG_ALWAYS=7
} CompareFrag;

typedef enum GPUCOLORFORMAT {
    GPUCOLORFORMAT_8=2,
    GPUCOLORFORMAT_1_5_5_5=3,
    GPUCOLORFORMAT_5_6_5=4,
    GPUCOLORFORMAT_6_5_5=5,
    GPUCOLORFORMAT_8_8_8_8=6,
    GPUCOLORFORMAT_2_10_10_10=7,
    GPUCOLORFORMAT_8_A=8,
    GPUCOLORFORMAT_8_B=9,
    GPUCOLORFORMAT_8_8=10,
    GPUCOLORFORMAT_8_8_8_8_A=14,
    GPUCOLORFORMAT_4_4_4_4=15,
    GPUCOLORFORMAT_10_11_11=16,
    GPUCOLORFORMAT_11_11_10=17,
    GPUCOLORFORMAT_16=24,
    GPUCOLORFORMAT_16_16=25,
    GPUCOLORFORMAT_16_16_16_16=26,
    GPUCOLORFORMAT_16_FLOAT=30,
    GPUCOLORFORMAT_16_16_FLOAT=31,
    GPUCOLORFORMAT_16_16_16_16_FLOAT=32,
    GPUCOLORFORMAT_32_FLOAT=36,
    GPUCOLORFORMAT_32_32_FLOAT=37,
    GPUCOLORFORMAT_32_32_32_32_FLOAT=38,
    GPUCOLORFORMAT_2_10_10_10_FLOAT=62
} GPUCOLORFORMAT;

typedef struct tagPALETTEENTRY tagPALETTEENTRY, *PtagPALETTEENTRY;

struct tagPALETTEENTRY {
    uchar peRed;
    uchar peGreen;
    uchar peBlue;
    uchar peFlags;
};

typedef enum <unnamed-tag_000010DB> {
    SPR_DSISR=18
} <unnamed-tag_000010DB>;

typedef enum <unnamed-tag_000010FF> {
    SPR_TBU_R=13
} <unnamed-tag_000010FF>;

typedef enum _PixCCVideoEncodingType {
    PIX_CC_VIDEO_QUALITY_LOW=0,
    PIX_CC_VIDEO_QUALITY_HIGH=1
} _PixCCVideoEncodingType;

typedef enum _XCONFIG_SYSTEM_ENTRIES {
    XCONFIG_SYSTEM_ALL=0,
    XCONFIG_SYSTEM_ALARM_TIME=1,
    XCONFIG_SYSTEM_PREVIOUS_FLASH_VERSION=2
} _XCONFIG_SYSTEM_ENTRIES;

typedef enum CombFuncX {
    COMB_DST_PLUS_SRC=0,
    COMB_SRC_MINUS_DST=1,
    COMB_MIN_DST_SRC=2,
    COMB_MAX_DST_SRC=3,
    COMB_DST_MINUS_SRC=4,
    COMB_DST_PLUS_SRC_BIAS=5
} CombFuncX;

typedef enum <unnamed-tag_000011A9> {
    SPR_TIABR=251
} <unnamed-tag_000011A9>;

typedef enum _XEKEYEX_INDEX {
    XEKEYEX_VIDEO_DEVICE_PUBLIC_KEY=0,
    XEKEYEX_VIDEO_DEVICE_PRIVATE_KEY=1,
    XEKEYEX_VIDEO_DEVICE_CERTIFICATE=2,
    XEKEYEX_IPTV_AV_PRIVATE_KEY=3,
    XEKEYEX_IPTV_KEY_FIRST=3,
    XEKEYEX_IPTV_AUTH_PRIVATE_KEY=4,
    XEKEYEX_IPTV_AV_CERTIFICATE=5,
    XEKEYEX_IPTV_AUTH_CERTIFICATE=6,
    XEKEYEX_IPTV_AV_SERIALIZED_CERTIFICATE=7,
    XEKEYEX_IPTV_AUTH_SERIALIZED_CERTIFICATE=8,
    XEKEYEX_IPTV_KEY_LAST=8,
    XEKEYEX_VIDEO_DEVICE_PR_PUBLIC_KEY_SIGN=9,
    XEKEYEX_VIDEO_DEVICE_PR_PRIVATE_KEY_SIGN=10,
    XEKEYEX_VIDEO_DEVICE_PR_PUBLIC_KEY_ENCRYPT=11,
    XEKEYEX_VIDEO_DEVICE_PR_PRIVATE_KEY_ENCRYPT=12,
    XEKEYEX_VIDEO_DEVICE_PR_CERTIFICATE=13,
    XEKEYEX_COUNT=14,
    XEKEYEX_MAX_KEY_INDEX=14,
    XEKEYEX_START=255
} _XEKEYEX_INDEX;

typedef enum HDMI_AUDIO_FORMAT {
    HDMI_AUDIO_FORMAT_PCM=0,
    HDMI_AUDIO_FORMAT_AC3=1,
    HDMI_AUDIO_FORMAT_DTS=2,
    HDMI_AUDIO_FORMAT_WMA_PRO=3
} HDMI_AUDIO_FORMAT;

typedef enum <unnamed-tag_00001107> {
    SPR_SPRG1=17
} <unnamed-tag_00001107>;

typedef enum _POOL_TYPE {
    PoolTypeThread=0,
    PoolTypeTitle=1,
    PoolTypeSystem=2,
    PoolTypeTitleDebug=3,
    PoolTypeSystemDebug=4
} _POOL_TYPE;

typedef enum GPUSCREENEXTENTS {
    GPUSCREENEXTENTS_ALLTILES=0,
    GPUSCREENEXTENTS_NONCULLED=1,
    GPUSCREENEXTENTS_PRIMEXTENTS=2
} GPUSCREENEXTENTS;

typedef enum _FP_IEEE_COMPARE_RESULT {
    FpCompareEqual=0,
    FpCompareGreater=1,
    FpCompareLess=2,
    FpCompareUnordered=3
} _FP_IEEE_COMPARE_RESULT;

typedef enum <unnamed-tag_0000114B> {
    SPR_TTR=154
} <unnamed-tag_0000114B>;

typedef enum _D3DSAMPLERSTATETYPE {
    D3DSAMP_ADDRESSU=0,
    D3DSAMP_ADDRESSV=4,
    D3DSAMP_ADDRESSW=8,
    D3DSAMP_BORDERCOLOR=12,
    D3DSAMP_MAGFILTER=16,
    D3DSAMP_MINFILTER=20,
    D3DSAMP_MIPFILTER=24,
    D3DSAMP_MIPMAPLODBIAS=28,
    D3DSAMP_MAXMIPLEVEL=32,
    D3DSAMP_MAXANISOTROPY=36,
    D3DSAMP_MAGFILTERZ=40,
    D3DSAMP_MINFILTERZ=44,
    D3DSAMP_SEPARATEZFILTERENABLE=48,
    D3DSAMP_MINMIPLEVEL=52,
    D3DSAMP_TRILINEARTHRESHOLD=56,
    D3DSAMP_ANISOTROPYBIAS=60,
    D3DSAMP_HGRADIENTEXPBIAS=64,
    D3DSAMP_VGRADIENTEXPBIAS=68,
    D3DSAMP_WHITEBORDERCOLORW=72,
    D3DSAMP_POINTBORDERENABLE=76,
    D3DSAMP_MAX=80,
    D3DSAMP_FORCE_DWORD=255
} _D3DSAMPLERSTATETYPE;

typedef enum _XBACKGROUND_DOWNLOAD_MODE {
    XBACKGROUND_DOWNLOAD_MODE_ALWAYS_ALLOW=1,
    XBACKGROUND_DOWNLOAD_MODE_AUTO=2
} _XBACKGROUND_DOWNLOAD_MODE;

typedef enum ColorArray {
    ARRAY_2D_ALT_COLOR=0,
    ARRAY_2D_COLOR=1,
    ARRAY_3D_SLICE_COLOR=3
} ColorArray;

typedef union FSB_TLA_COUNT5_INIT FSB_TLA_COUNT5_INIT, *PFSB_TLA_COUNT5_INIT;


/* WARNING! conflicting data type names: /iw5mp_xenon.pdb/FSB_TLA_COUNT5_INIT/<unnamed-type-bitfields> - /iw5mp_xenon.pdb/D1SCL_COEF_RAM_SELECT/<unnamed-type-bitfields> */

union FSB_TLA_COUNT5_INIT {
    struct <unnamed-type-bitfields> bitfields;
    struct <unnamed-type-bitfields> bits;
    uint u32All;
    int i32All;
    float f32All;
};

typedef enum _XSECURITY_LOGGING_LEVELS {
    XSECURITY_LOG_LEVEL0=0,
    XSECURITY_LOG_LEVEL1=1,
    XSECURITY_LOG_LEVEL2=2,
    XSECURITY_LOG_LEVEL3=3,
    XSECURITY_LOG_LEVEL4=4,
    XSECURITY_LOG_LEVEL5=5,
    XSECURITY_LOG_LEVEL6=6,
    XSECURITY_LOG_LEVEL7=7,
    XSECURITY_LOG_LEVEL8=8,
    XSECURITY_LOG_LEVEL9=9,
    XSECURITY_LOG_LEVEL10=10,
    XSECURITY_LOG_LEVEL11=11,
    XSECURITY_LOG_LEVEL12=12,
    XSECURITY_LOG_LEVEL13=13,
    XSECURITY_LOG_LEVEL14=14,
    XSECURITY_LOG_LEVEL15=15
} _XSECURITY_LOGGING_LEVELS;

typedef enum _TOKEN_TYPE {
    TokenPrimary=1,
    TokenImpersonation=2
} _TOKEN_TYPE;

typedef enum _D3DSHADER_PARAM_SRCMOD_TYPE {
    D3DSPSM_ABS=0,
    D3DSPSM_ABSNEG=0,
    D3DSPSM_BIAS=0,
    D3DSPSM_BIASNEG=0,
    D3DSPSM_COMP=0,
    D3DSPSM_DW=0,
    D3DSPSM_DZ=0,
    D3DSPSM_NEG=0,
    D3DSPSM_NONE=0,
    D3DSPSM_NOT=0,
    D3DSPSM_SIGN=0,
    D3DSPSM_SIGNNEG=0,
    D3DSPSM_X2=0,
    D3DSPSM_X2NEG=0,
    D3DSPSM_FORCE_DWORD=255
} _D3DSHADER_PARAM_SRCMOD_TYPE;

typedef enum _ePMCSetup {
    PMC_SETUP_FIRST=0,
    PMC_SETUP_OVERVIEW_PB0T0=0,
    PMC_SETUP_OVERVIEW_PB0T1=1,
    PMC_SETUP_OVERVIEW_PB1T0=2,
    PMC_SETUP_OVERVIEW_PB1T1=3,
    PMC_SETUP_OVERVIEW_PB2T0=4,
    PMC_SETUP_OVERVIEW_PB2T1=5,
    PMC_SETUP_OVERVIEW_PB0=6,
    PMC_SETUP_OVERVIEW_PB1=7,
    PMC_SETUP_OVERVIEW_PB2=8,
    PMC_SETUP_OVERVIEW_6THREADS=9,
    PMC_SETUP_INSTRUCTIONMIX_PB0T0=10,
    PMC_SETUP_INSTRUCTIONMIX_PB0T1=11,
    PMC_SETUP_INSTRUCTIONMIX_PB1T0=12,
    PMC_SETUP_INSTRUCTIONMIX_PB1T1=13,
    PMC_SETUP_INSTRUCTIONMIX_PB2T0=14,
    PMC_SETUP_INSTRUCTIONMIX_PB2T1=15,
    PMC_SETUP_FLUSHREASONS_PB0T0=16,
    PMC_SETUP_FLUSHREASONS_PB0T1=17,
    PMC_SETUP_FLUSHREASONS_PB1T0=18,
    PMC_SETUP_FLUSHREASONS_PB1T1=19,
    PMC_SETUP_FLUSHREASONS_PB2T0=20,
    PMC_SETUP_FLUSHREASONS_PB2T1=21,
    PMC_SETUP_VMXMIX_PB0T0=22,
    PMC_SETUP_VMXMIX_PB0T1=23,
    PMC_SETUP_VMXMIX_PB1T0=24,
    PMC_SETUP_VMXMIX_PB1T1=25,
    PMC_SETUP_VMXMIX_PB2T0=26,
    PMC_SETUP_VMXMIX_PB2T1=27,
    PMC_SETUP_VMXFPU_PB0T0=28,
    PMC_SETUP_VMXFPU_PB0T1=29,
    PMC_SETUP_VMXFPU_PB1T0=30,
    PMC_SETUP_VMXFPU_PB1T1=31,
    PMC_SETUP_VMXFPU_PB2T0=32,
    PMC_SETUP_VMXFPU_PB2T1=33,
    PMC_SETUP_FPU_PB0T0=34,
    PMC_SETUP_FPU_PB0T1=35,
    PMC_SETUP_FPU_PB1T0=36,
    PMC_SETUP_FPU_PB1T1=37,
    PMC_SETUP_FPU_PB2T0=38,
    PMC_SETUP_FPU_PB2T1=39,
    PMC_SETUP_PREFETCH=40,
    PMC_SETUP_MEMORY_LATENCY_PB0=41,
    PMC_SETUP_MEMORY_LATENCY_PB1=42,
    PMC_SETUP_MEMORY_LATENCY_PB2=43,
    PMC_SETUP_TLBS=44,
    PMC_SETUP_LAST=45
} _ePMCSetup;

typedef enum _D3DXINCLUDE_TYPE {
    D3DXINC_LOCAL=0,
    D3DXINC_SYSTEM=1,
    D3DXINC_FORCE_DWORD=255
} _D3DXINCLUDE_TYPE;

typedef enum GPUROUNDMODE {
    GPUROUNDMODE_TRUNCATE=0,
    GPUROUNDMODE_ROUND=1,
    GPUROUNDMODE_ROUND_TO_EVEN=2,
    GPUROUNDMODE_ROUND_TO_ODD=3
} GPUROUNDMODE;

typedef enum _D3DXCOMPRESSION_FLAGS {
    D3DXCOMPRESS_DEFAULT=0,
    D3DXCOMPRESS_FORCE_DWORD=255
} _D3DXCOMPRESSION_FLAGS;

typedef enum _AUDIT_EVENT_TYPE {
    AuditEventObjectAccess=0,
    AuditEventDirectoryServiceAccess=1
} _AUDIT_EVENT_TYPE;

typedef enum _XPROFILE_ACTION_AUTO_CENTER_OPTIONS {
    XPROFILE_ACTION_AUTO_CENTER_OFF=0,
    XPROFILE_ACTION_AUTO_CENTER_ON=1
} _XPROFILE_ACTION_AUTO_CENTER_OPTIONS;

typedef enum _D3DQUERYTYPE {
    D3DQUERYTYPE_VCACHE=4,
    D3DQUERYTYPE_RESOURCEMANAGER=5,
    D3DQUERYTYPE_VERTEXSTATS=6,
    D3DQUERYTYPE_EVENT=8,
    D3DQUERYTYPE_OCCLUSION=9,
    D3DQUERYTYPE_SCREENEXTENT=10
} _D3DQUERYTYPE;

typedef enum GPUVERTEXFETCHOP {
    GPUVERTEXFETCHOP_FETCH_VERTEX=0
} GPUVERTEXFETCHOP;

typedef enum _XCUSTOMACTION_FLAGS {
    XCUSTOMACTION_FLAG_CLOSES_GUIDE=1,
    XCUSTOMACTION_FLAG_DELETES_MESSAGE=2
} _XCUSTOMACTION_FLAGS;

typedef enum Exporting {
    NOT_EXPORTING=0,
    EXPORTING=1
} Exporting;

typedef enum <unnamed-tag_0000118D> {
    SPR_IABR=242
} <unnamed-tag_0000118D>;

typedef enum GPUBLEND {
    GPUBLEND_ZERO=0,
    GPUBLEND_ONE=1,
    GPUBLEND_SRCCOLOR=4,
    GPUBLEND_INVSRCCOLOR=5,
    GPUBLEND_SRCALPHA=6,
    GPUBLEND_INVSRCALPHA=7,
    GPUBLEND_DESTCOLOR=8,
    GPUBLEND_INVDESTCOLOR=9,
    GPUBLEND_DESTALPHA=10,
    GPUBLEND_INVDESTALPHA=11,
    GPUBLEND_BLENDFACTOR=12,
    GPUBLEND_INVBLENDFACTOR=13,
    GPUBLEND_CONSTANTALPHA=14,
    GPUBLEND_INVCONSTANTALPHA=15,
    GPUBLEND_SRCALPHASAT=16
} GPUBLEND;

typedef enum _D3DXEVENT_TYPE {
    D3DXEVENT_TRACKSPEED=0,
    D3DXEVENT_TRACKWEIGHT=1,
    D3DXEVENT_TRACKPOSITION=2,
    D3DXEVENT_TRACKENABLE=3,
    D3DXEVENT_PRIORITYBLEND=4,
    D3DXEVENT_FORCE_DWORD=255
} _D3DXEVENT_TYPE;

typedef enum _D3DXMESH {
    D3DXMESH_IB_DYNAMIC=0,
    D3DXMESH_IB_MANAGED=0,
    D3DXMESH_IB_SOFTWAREPROCESSING=0,
    D3DXMESH_IB_SYSTEMMEM=0,
    D3DXMESH_IB_WRITEONLY=0,
    D3DXMESH_NPATCHES=0,
    D3DXMESH_SOFTWAREPROCESSING=0,
    D3DXMESH_USEHWONLY=0,
    D3DXMESH_VB_SHARE=0,
    D3DXMESH_VB_SOFTWAREPROCESSING=0,
    D3DXMESH_32BIT=1,
    D3DXMESH_DONOTCLIP=2,
    D3DXMESH_POINTS=4,
    D3DXMESH_RTPATCHES=8,
    D3DXMESH_SYSTEMMEM=16,
    D3DXMESH_VB_SYSTEMMEM=16,
    D3DXMESH_MANAGED=32,
    D3DXMESH_VB_MANAGED=32,
    D3DXMESH_VB_WRITEONLY=64,
    D3DXMESH_WRITEONLY=64,
    D3DXMESH_DYNAMIC=128,
    D3DXMESH_VB_DYNAMIC=128,
    D3DXMESH_VALIDBITS=255,
    D3DXMESH_VALIDBLENDEDBITS=255,
    D3DXMESH_VALIDCLONEBITS=255
} _D3DXMESH;

typedef enum _D3DXMESHDATATYPE {
    D3DXMESHTYPE_MESH=1,
    D3DXMESHTYPE_PMESH=2,
    D3DXMESHTYPE_PATCHMESH=3,
    D3DXMESHTYPE_FORCE_DWORD=255
} _D3DXMESHDATATYPE;

typedef enum _D3DBACKBUFFER_TYPE {
    D3DBACKBUFFER_TYPE_MONO=0,
    D3DBACKBUFFER_TYPE_LEFT=1,
    D3DBACKBUFFER_TYPE_RIGHT=2,
    D3DBACKBUFFER_TYPE_FORCE_DWORD=255
} _D3DBACKBUFFER_TYPE;

typedef enum XMARKETPLACE_OFFERING_TYPE {
    XMARKETPLACE_OFFERING_TYPE_ARCADE=0,
    XMARKETPLACE_OFFERING_TYPE_AVATARITEM=0,
    XMARKETPLACE_OFFERING_TYPE_CONSUMABLE=0,
    XMARKETPLACE_OFFERING_TYPE_TILE=0,
    XMARKETPLACE_OFFERING_TYPE_VIDEO=0,
    XMARKETPLACE_OFFERING_TYPE_CONTENT=2,
    XMARKETPLACE_OFFERING_TYPE_GAME_DEMO=32,
    XMARKETPLACE_OFFERING_TYPE_GAME_TRAILER=64,
    XMARKETPLACE_OFFERING_TYPE_THEME=128
} XMARKETPLACE_OFFERING_TYPE;

typedef enum _TIMER_TYPE {
    NotificationTimer=0,
    SynchronizationTimer=1
} _TIMER_TYPE;

typedef enum <unnamed-tag_000010FB> {
    SPR_SPRG3_R=3
} <unnamed-tag_000010FB>;

typedef enum _XALLOC_PRIVATE_ALLOCATOR_IDS {
    eXALLOCAllocatorId_MsPrivateMin=176,
    eXALLOCAllocatorId_XHTTP=176,
    eXALLOCAllocatorId_MsPrivateMax=191
} _XALLOC_PRIVATE_ALLOCATOR_IDS;

typedef enum _FILE_INFORMATION_CLASS {
    FileDirectoryInformation=1,
    FileFullDirectoryInformation=2,
    FileBothDirectoryInformation=3,
    FileBasicInformation=4,
    FileStandardInformation=5,
    FileInternalInformation=6,
    FileEaInformation=7,
    FileAccessInformation=8,
    FileNameInformation=9,
    FileRenameInformation=10,
    FileLinkInformation=11,
    FileNamesInformation=12,
    FileDispositionInformation=13,
    FilePositionInformation=14,
    FileFullEaInformation=15,
    FileModeInformation=16,
    FileAlignmentInformation=17,
    FileAllInformation=18,
    FileAllocationInformation=19,
    FileEndOfFileInformation=20,
    FileAlternateNameInformation=21,
    FileStreamInformation=22,
    FileMountPartitionInformation=23,
    FileMountPartitionsInformation=24,
    FilePipeRemoteInformation=25,
    FileSectorInformation=26,
    FileXctdCompressionInformation=27,
    FileCompressionInformation=28,
    FileObjectIdInformation=29,
    FileCompletionInformation=30,
    FileMoveClusterInformation=31,
    FileIoPriorityInformation=32,
    FileReparsePointInformation=33,
    FileNetworkOpenInformation=34,
    FileAttributeTagInformation=35,
    FileTrackingInformation=36,
    FileMaximumInformation=37
} _FILE_INFORMATION_CLASS;

typedef enum <unnamed-tag_000011A5> {
    SPR_HID7=250
} <unnamed-tag_000011A5>;

typedef enum VectorOpcode {
    ADDv=0,
    MULv=1,
    MAXv=2,
    MINv=3,
    SETEv=4,
    SETGTv=5,
    SETGTEv=6,
    SETNEv=7,
    FRACv=8,
    TRUNCv=9,
    FLOORv=10,
    MULADDv=11,
    CNDEv=12,
    CNDGTEv=13,
    CNDGTv=14,
    DOT4v=15,
    DOT3v=16,
    DOT2ADDv=17,
    CUBEv=18,
    MAX4v=19,
    PRED_SETE_PUSHv=20,
    PRED_SETNE_PUSHv=21,
    PRED_SETGT_PUSHv=22,
    PRED_SETGTE_PUSHv=23,
    KILLEv=24,
    KILLGTv=25,
    KILLGTEv=26,
    KILLNEv=27,
    DSTv=28,
    MOVAv=29
} VectorOpcode;

typedef enum GPUCOPYSRCSELECT {
    GPUCOPYSRCSELECT_RENDER_TARGET_0=0,
    GPUCOPYSRCSELECT_RENDER_TARGET_1=1,
    GPUCOPYSRCSELECT_RENDER_TARGET_2=2,
    GPUCOPYSRCSELECT_RENDER_TARGET_3=3,
    GPUCOPYSRCSELECT_DEPTH_STENCIL=4
} GPUCOPYSRCSELECT;

typedef enum SurfaceTiling {
    ARRAY_LINEAR=0,
    ARRAY_TILED=1
} SurfaceTiling;

typedef enum GPUCONSTANTID {
    GPUCONSTANTID_ALU=0,
    GPUCONSTANTID_FETCH=1,
    GPUCONSTANTID_BOOLEAN=2,
    GPUCONSTANTID_INTEGER=3,
    GPUCONSTANTID_REGISTER=4
} GPUCONSTANTID;

typedef enum <unnamed-tag_00001103> {
    SPR_SPRG0=16
} <unnamed-tag_00001103>;

typedef struct _DM_MEMORY_REGION _DM_MEMORY_REGION, *P_DM_MEMORY_REGION;

struct _DM_MEMORY_REGION {
    void *BaseAddress;
    ulong Size;
    ulong Protect;
};

typedef enum _WAIT_TYPE {
    WaitAll=0,
    WaitAny=1
} _WAIT_TYPE;

typedef enum <unnamed-tag_0000116B> {
    SPR_DRSR1=187
} <unnamed-tag_0000116B>;

typedef enum <unnamed-tag_0000118F> {
    SPR_HID4=244
} <unnamed-tag_0000118F>;

typedef enum _KPOWER_MODE {
    PowerModeQuiet=0,
    PowerModeFull=1
} _KPOWER_MODE;

typedef enum _PixSmallOpcodesType {
    PIXSMALL_EndNamedEvent=0,
    PIXSMALL_Escape=1,
    PIXSMALL_BeginDrawVertices=2,
    PIXSMALL_EndDrawVertices=3,
    PIXSMALL_BeginDrawIndexedVertices=4,
    PIXSMALL_EndDrawIndexedVertices=5,
    PIXSMALL_BeginVertices=6,
    PIXSMALL_EndVertices=7,
    PIXSMALL_BeginKickOff=8,
    PIXSMALL_EndKickOff=9,
    PIXSMALL_BeginNamedEvent_Tokenize_VarArgs=16,
    PIXSMALL_BeginNamedEvent_Tokenize_NOVarArgs=17,
    PIXSMALL_BeginNamedEvent_Tokenize_Float=18,
    PIXSMALL_BeginNamedEvent_Tokenize_Int=19,
    PIXSMALL_BeginNamedEvent_Tokenize_String=20,
    PIXSMALL_BeginNamedEvent_Copy_VarArgs0=25,
    PIXSMALL_BeginNamedEvent_Copy_VarArgs1=26,
    PIXSMALL_BeginNamedEvent_Copy_VarArgs2=27,
    PIXSMALL_BeginNamedEvent_Copy_VarArgs3=28,
    PIXSMALL_BeginNamedEvent_Copy_NOVarArgs=29,
    PIXSMALL_BeginNamedEvent_Copy_Float=30,
    PIXSMALL_BeginNamedEvent_Copy_Int=31,
    PIXSMALL_BeginNamedEvent_Copy_String=32,
    PIXSMALL_BeginNamedEvent_Copy_Double=33,
    PIXSMALL_BeginNamedEvent_Copy_Int64=34,
    PIXSMALL_SetMarker_Tokenize_VarArgs=37,
    PIXSMALL_SetMarker_Tokenize_NOVarArgs=38,
    PIXSMALL_SetMarker_Tokenize_Float=39,
    PIXSMALL_SetMarker_Tokenize_Int=40,
    PIXSMALL_SetMarker_Tokenize_String=41,
    PIXSMALL_SetMarker_Copy_VarArgs0=46,
    PIXSMALL_SetMarker_Copy_VarArgs1=47,
    PIXSMALL_SetMarker_Copy_VarArgs2=48,
    PIXSMALL_SetMarker_Copy_VarArgs3=49,
    PIXSMALL_SetMarker_Copy_NOVarArgs=50,
    PIXSMALL_SetMarker_Copy_Float=51,
    PIXSMALL_SetMarker_Copy_Int=52,
    PIXSMALL_SetMarker_Copy_String=53,
    PIXSMALL_PrintA_Tokenize_VarArgs=58,
    PIXSMALL_PrintA_Tokenize_NOVarArgs=59,
    PIXSMALL_PrintA_Tokenize_Float=60,
    PIXSMALL_PrintA_Tokenize_Int=61,
    PIXSMALL_PrintA_Tokenize_String=62,
    PIXSMALL_PrintA_Copy_VarArgs0=67,
    PIXSMALL_PrintA_Copy_VarArgs1=68,
    PIXSMALL_PrintA_Copy_VarArgs2=69,
    PIXSMALL_PrintA_Copy_VarArgs3=70,
    PIXSMALL_PrintA_Copy_NOVarArgs=71,
    PIXSMALL_PrintA_Copy_Float=72,
    PIXSMALL_PrintA_Copy_Int=73,
    PIXSMALL_PrintA_Copy_String=74,
    PIXSMALL_PrintW_Tokenize_VarArgs=79,
    PIXSMALL_PrintW_Tokenize_NOVarArgs=80,
    PIXSMALL_PrintW_Tokenize_Float=81,
    PIXSMALL_PrintW_Tokenize_Int=82,
    PIXSMALL_PrintW_Tokenize_String=83,
    PIXSMALL_PrintW_Copy_VarArgs0=88,
    PIXSMALL_PrintW_Copy_VarArgs1=89,
    PIXSMALL_PrintW_Copy_VarArgs2=90,
    PIXSMALL_PrintW_Copy_VarArgs3=91,
    PIXSMALL_PrintW_Copy_NOVarArgs=92,
    PIXSMALL_PrintW_Copy_Float=93,
    PIXSMALL_PrintW_Copy_Int=94,
    PIXSMALL_PrintW_Copy_String=95,
    PIXSMALL_Escape_Gpu=96,
    PIXSMALL_Escape_Cpu_Without_Timestamp=97
} _PixSmallOpcodesType;

typedef union PA_SU_PERFCOUNTER0_LOW PA_SU_PERFCOUNTER0_LOW, *PPA_SU_PERFCOUNTER0_LOW;


/* WARNING! conflicting data type names: /iw5mp_xenon.pdb/PA_SU_PERFCOUNTER0_LOW/<unnamed-type-bitfields> - /iw5mp_xenon.pdb/D1SCL_COEF_RAM_SELECT/<unnamed-type-bitfields> */

union PA_SU_PERFCOUNTER0_LOW {
    struct <unnamed-type-bitfields> bitfields;
    struct <unnamed-type-bitfields> bits;
    uint u32All;
    int i32All;
    float f32All;
};

typedef enum GPUTRICLAMP {
    GPUTRICLAMP_NORMAL=0,
    GPUTRICLAMP_ONE_SIXTH=1,
    GPUTRICLAMP_ONE_FOURTH=2,
    GPUTRICLAMP_THREE_EIGHTHS=3
} GPUTRICLAMP;

typedef enum _D3DXIMAGE_FILEFORMAT {
    D3DXIFF_BMP=0,
    D3DXIFF_JPG=1,
    D3DXIFF_TGA=2,
    D3DXIFF_PNG=3,
    D3DXIFF_DDS=4,
    D3DXIFF_PPM=5,
    D3DXIFF_DIB=6,
    D3DXIFF_HDR=7,
    D3DXIFF_PFM=8,
    D3DXIFF_FORCE_DWORD=255
} _D3DXIMAGE_FILEFORMAT;

typedef enum BlendOpX {
    BLENDX_ZERO=0,
    BLENDX_ONE=1,
    BLENDX_SRC_COLOR=4,
    BLENDX_ONE_MINUS_SRC_COLOR=5,
    BLENDX_SRC_ALPHA=6,
    BLENDX_ONE_MINUS_SRC_ALPHA=7,
    BLENDX_DST_COLOR=8,
    BLENDX_ONE_MINUS_DST_COLOR=9,
    BLENDX_DST_ALPHA=10,
    BLENDX_ONE_MINUS_DST_ALPHA=11,
    BLENDX_CONSTANT_COLOR=12,
    BLENDX_ONE_MINUS_CONSTANT_COLOR=13,
    BLENDX_CONSTANT_ALPHA=14,
    BLENDX_ONE_MINUS_CONSTANT_ALPHA=15,
    BLENDX_SRC_ALPHA_SATURATE=16
} BlendOpX;

typedef enum <unnamed-tag_000010FD> {
    SPR_TB_R=12
} <unnamed-tag_000010FD>;

typedef enum XONLINE_NAT_TYPE {
    XONLINE_NAT_OPEN=1,
    XONLINE_NAT_MODERATE=2,
    XONLINE_NAT_STRICT=3
} XONLINE_NAT_TYPE;

typedef enum IOC_PERFCOUNT_SELECT {
    IOC_PERF_SEL_00=0,
    IOC_PERF_SEL_01=1,
    IOC_PERF_SEL_02=2,
    IOC_PERF_SEL_03=3,
    IOC_PERF_SEL_04=4,
    IOC_PERF_SEL_05=5,
    IOC_PERF_SEL_06=6,
    IOC_PERF_SEL_07=7,
    IOC_PERF_SEL_08=8,
    IOC_PERF_SEL_09=9,
    IOC_SPARES=10
} IOC_PERFCOUNT_SELECT;

typedef enum _D3DTRANSFORMSTATETYPE {
    D3DTS_WORLD=0,
    D3DTS_VIEW=1,
    D3DTS_PROJECTION=2,
    D3DTS_TEXTURE0=3,
    D3DTS_TEXTURE1=4,
    D3DTS_TEXTURE2=5,
    D3DTS_TEXTURE3=6,
    D3DTS_TEXTURE4=7,
    D3DTS_TEXTURE5=8,
    D3DTS_TEXTURE6=9,
    D3DTS_TEXTURE7=10,
    D3DTS_MAX=11,
    D3DTS_FORCE_DWORD=255
} _D3DTRANSFORMSTATETYPE;

typedef enum _XCONFIG_IPTV_ENTRIES {
    XCONFIG_IPTV_DATA=0,
    XCONFIG_IPTV_SERVICE_PROVIDER_NAME=1,
    XCONFIG_IPTV_PROVISIONING_SERVER_URL=2,
    XCONFIG_IPTV_SUPPORT_INFO=3,
    XCONFIG_IPTV_BOOTSTRAP_SERVER_URL=4
} _XCONFIG_IPTV_ENTRIES;

typedef union TP2_PERFCOUNTER1_HI TP2_PERFCOUNTER1_HI, *PTP2_PERFCOUNTER1_HI;


/* WARNING! conflicting data type names: /iw5mp_xenon.pdb/TP2_PERFCOUNTER1_HI/<unnamed-type-bitfields> - /iw5mp_xenon.pdb/D1SCL_COEF_RAM_SELECT/<unnamed-type-bitfields> */

union TP2_PERFCOUNTER1_HI {
    struct <unnamed-type-bitfields> bitfields;
    struct <unnamed-type-bitfields> bits;
    uint u32All;
    int i32All;
    float f32All;
};

typedef struct _XENUMERATEPARAM _XENUMERATEPARAM, *P_XENUMERATEPARAM;

struct _XENUMERATEPARAM {
    ulong dwUserIndex;
    ulong dwFlags;
    void *pvPrivateEnumStructure;
    void *pvBuffer;
    ulong cbBuffer;
    ulong cItemsRequested;
    ulong *pcItemsReturned;
    struct _XOVERLAPPED *pxoverlapped;
};

typedef enum GPUEDRAMMODE {
    GPUEDRAMMODE_NOP=0,
    GPUEDRAMMODE_COLOR_DEPTH=4,
    GPUEDRAMMODE_DOUBLE_DEPTH=5,
    GPUEDRAMMODE_COPY=6
} GPUEDRAMMODE;

typedef enum <unnamed-tag_00001347> {
    TITLE_HASH_FLAG_NONE=0,
    TITLE_HASH_FLAG_VALID=0,
    TITLE_HASH_SIZE_ALIGNMENT=0,
    TITLE_HASH_SIZE_DEFAULT=0
} <unnamed-tag_00001347>;

typedef enum <unnamed-tag_00001238> {
    ODD_POLICY_FLAG_ALTERNATE_DA=0,
    ODD_POLICY_FLAG_BIT_DISCARDED_DYNAMIC=0,
    ODD_POLICY_FLAG_CHECK_DSP=0,
    ODD_POLICY_FLAG_NONE=0,
    ODD_POLICY_FLAG_BIT_DISCARDED_STATIC=1,
    ODD_POLICY_FLAG_BIT_DISCARDED_RANGE=2,
    ODD_POLICY_FLAG_BIT_DISCARDED_MEMORY=3,
    ODD_POLICY_FLAG_BIT_WRITE_LOCK=4,
    ODD_POLICY_FLAG_BIT_CHECK_DYNAMIC=5,
    ODD_POLICY_FLAG_BIT_CHECK_STATIC=6,
    ODD_POLICY_FLAG_BIT_CHECK_MEMORY=7,
    ODD_POLICY_FLAG_BIT_CHECK_DSP=8,
    ODD_POLICY_FLAG_BIT_ALTERNATE_DA=9,
    ODD_POLICY_FLAG_BIT_MAXIMUM=10,
    ODD_POLICY_FLAG_WRITE_LOCK=16,
    ODD_POLICY_FLAG_CHECK_DYNAMIC=32,
    ODD_POLICY_FLAG_CHECK_FIRMWARE=32,
    ODD_POLICY_FLAG_CHECK_STATIC=64,
    ODD_POLICY_FLAG_CHECK_MEMORY=128
} <unnamed-tag_00001238>;

typedef struct _D3DDISPLAYMODE _D3DDISPLAYMODE, *P_D3DDISPLAYMODE;

struct _D3DDISPLAYMODE {
    uint Width;
    uint Height;
    uint RefreshRate;
    enum _D3DFORMAT Format;
};

typedef enum _D3DTESSELLATIONMODE {
    D3DTM_DISCRETE=0,
    D3DTM_CONTINUOUS=1,
    D3DTM_PEREDGE=2,
    D3DTM_FORCE_DWORD=255
} _D3DTESSELLATIONMODE;

typedef struct _CERTIFICATE_REVOCATION_DATA _CERTIFICATE_REVOCATION_DATA, *P_CERTIFICATE_REVOCATION_DATA;

struct _CERTIFICATE_REVOCATION_DATA {
    ulong Size;
    ulong Version;
    ulong Count;
};

typedef enum _D3DXCALLBACK_SEARCH_FLAGS {
    D3DXCALLBACK_SEARCH_EXCLUDING_INITIAL_POSITION=1,
    D3DXCALLBACK_SEARCH_BEHIND_INITIAL_POSITION=2,
    D3DXCALLBACK_SEARCH_FORCE_DWORD=255
} _D3DXCALLBACK_SEARCH_FLAGS;

typedef union GPU_POINT GPU_POINT, *PGPU_POINT;

typedef struct GPU_POINT_s_0 GPU_POINT_s_0, *PGPU_POINT_s_0;

struct GPU_POINT_s_0 {
    undefined field0_0x0;
    undefined field1_0x1;
    int Y:15;
    int X:15;
};

union GPU_POINT {
    struct GPU_POINT_s_0 _s_0;
    ulong dword;
};

typedef enum <unnamed-tag_000011A1> {
    SPR_HID6=249
} <unnamed-tag_000011A1>;

typedef union PA_CL_UCP_1_X PA_CL_UCP_1_X, *PPA_CL_UCP_1_X;


/* WARNING! conflicting data type names: /iw5mp_xenon.pdb/PA_CL_UCP_1_X/<unnamed-type-bitfields> - /iw5mp_xenon.pdb/D1SCL_COEF_RAM_SELECT/<unnamed-type-bitfields> */

union PA_CL_UCP_1_X {
    struct <unnamed-type-bitfields> bitfields;
    struct <unnamed-type-bitfields> bits;
    uint u32All;
    int i32All;
    float f32All;
};

typedef enum GPUSURFACESWAP {
    SURFACESWAP_LOW_RED=0,
    SURFACESWAP_LOW_BLUE=1
} GPUSURFACESWAP;

typedef enum GPUCLAMPPOLICY {
    GPUCLAMPPOLICY_D3D=0,
    GPUCLAMPPOLICY_OGL=1
} GPUCLAMPPOLICY;

typedef enum <unnamed-tag_00001232> {
    RESTRICTED_PRIVILEGES_FLAG_BIT_ENABLE_NONE=0,
    RESTRICTED_PRIVILEGES_FLAG_BIT_ENABLE_SELECTIVE=1,
    RESTRICTED_PRIVILEGES_FLAG_ENABLE_NONE=1,
    RESTRICTED_PRIVILEGES_FLAG_BIT_SIGNED_KEYVAULT=2,
    RESTRICTED_PRIVILEGES_FLAG_ENABLE_SELECTIVE=2,
    RESTRICTED_PRIVILEGES_FLAG_SIGNED_KEYVAULT=4
} <unnamed-tag_00001232>;

typedef enum _GET_FILEEX_INFO_LEVELS {
    GetFileExInfoStandard=0,
    GetFileExMaxInfoLevel=1
} _GET_FILEEX_INFO_LEVELS;

typedef enum _XGI_STATS_ENUMERATOR_TYPE {
    XGI_STATS_ENUM_BY_XUID=0,
    XGI_STATS_ENUM_BY_RANK=1,
    XGI_STATS_ENUM_BY_RANK_PER_SPEC=2,
    XGI_STATS_ENUM_BY_RATING=3
} _XGI_STATS_ENUMERATOR_TYPE;

typedef enum <unnamed-tag_00001147> {
    SPR_TSRR=129
} <unnamed-tag_00001147>;

typedef enum GPUVERTEXSIGN {
    GPUVERTEXSIGN_UNSIGNED=0,
    GPUVERTEXSIGN_SIGNED=1
} GPUVERTEXSIGN;

typedef enum GPUEDRAMCOLORFORMAT {
    GPUEDRAMCOLORFORMAT_8_8_8_8=0,
    GPUEDRAMCOLORFORMAT_8_8_8_8_GAMMA=1,
    GPUEDRAMCOLORFORMAT_2_10_10_10=2,
    GPUEDRAMCOLORFORMAT_2_10_10_10_FLOAT=3,
    GPUEDRAMCOLORFORMAT_16_16=4,
    GPUEDRAMCOLORFORMAT_16_16_16_16=5,
    GPUEDRAMCOLORFORMAT_16_16_FLOAT=6,
    GPUEDRAMCOLORFORMAT_16_16_16_16_FLOAT=7,
    GPUEDRAMCOLORFORMAT_2_10_10_10_AS_10_10_10_10=10,
    GPUEDRAMCOLORFORMAT_2_10_10_10_FLOAT_AS_16_16_16_16=12,
    GPUEDRAMCOLORFORMAT_32_FLOAT=14,
    GPUEDRAMCOLORFORMAT_32_32_FLOAT=15
} GPUEDRAMCOLORFORMAT;

typedef enum DM_FILEEVENT_API {
    Dm_FileEvent_NtCreateFile=0,
    Dm_FileEvent_NtOpenFile=1,
    Dm_FileEvent_NtDeleteFile=2,
    Dm_FileEvent_NtReadFile=3,
    Dm_FileEvent_NtWriteFile=4,
    Dm_FileEvent_NtReadFileScatter=5,
    Dm_FileEvent_NtWriteFileGather=6,
    Dm_FileEvent_NtFlushBuffersFile=7,
    Dm_FileEvent_NtClose=8,
    Dm_FileEvent_FscSetCacheElementCount=9
} DM_FILEEVENT_API;

typedef enum _D3DXPARAMETER_CLASS {
    D3DXPC_SCALAR=0,
    D3DXPC_VECTOR=1,
    D3DXPC_MATRIX_ROWS=2,
    D3DXPC_MATRIX_COLUMNS=3,
    D3DXPC_OBJECT=4,
    D3DXPC_STRUCT=5,
    D3DXPC_FORCE_DWORD=255
} _D3DXPARAMETER_CLASS;

typedef enum _XPROFILE_CONTROLLER_VIBRATION_OPTIONS {
    XPROFILE_CONTROLLER_VIBRATION_OFF=0,
    XPROFILE_CONTROLLER_VIBRATION_ON=3
} _XPROFILE_CONTROLLER_VIBRATION_OPTIONS;

typedef enum _SID_NAME_USE {
    SidTypeUser=1,
    SidTypeGroup=2,
    SidTypeDomain=3,
    SidTypeAlias=4,
    SidTypeWellKnownGroup=5,
    SidTypeDeletedAccount=6,
    SidTypeInvalid=7,
    SidTypeUnknown=8,
    SidTypeComputer=9
} _SID_NAME_USE;

typedef enum OperandSelect0 {
    CONSTANT=0,
    NON_CONSTANT=1
} OperandSelect0;

typedef enum GPUCLAMP {
    GPUCLAMP_WRAP=0,
    GPUCLAMP_MIRROR=1,
    GPUCLAMP_CLAMP_TO_LAST=2,
    GPUCLAMP_MIRROR_ONCE_TO_LAST=3,
    GPUCLAMP_CLAMP_HALFWAY=4,
    GPUCLAMP_MIRROR_ONCE_HALFWAY=5,
    GPUCLAMP_CLAMP_TO_BORDER=6,
    GPUCLAMP_MIRROR_TO_BORDER=7
} GPUCLAMP;

typedef struct _DUMP_BASIC_SERVICES _DUMP_BASIC_SERVICES, *P_DUMP_BASIC_SERVICES;

struct _DUMP_BASIC_SERVICES {
    ulong (*GetHardwareThreadContext)(ulong, struct _CONTEXT *);
    void (*ResetGetHardwareThreadContext)(void);
    ulong (*GetImagePageTableEntry)(ulong);
    void (*CollectBucketData)(uchar *, struct _EXCEPTION_RECORD *, struct _CONTEXT *, ulong);
    void (*InitProgressBar)(ulong);
    void (*IncrementProgressBar)(void);
    void (*DbgPrintf)(char *);
};

typedef enum OperandSelect1 {
    ABSOLUTE_REG=0,
    RELATIVE_REG=1
} OperandSelect1;

typedef enum XGUCODESHADERVERSION {
    XGUCODESHADERVERSION_PIXEL=0,
    XGUCODESHADERVERSION_VERTEX=1
} XGUCODESHADERVERSION;

typedef union GPUCOMMAND_CONTEXT_UPDATE GPUCOMMAND_CONTEXT_UPDATE, *PGPUCOMMAND_CONTEXT_UPDATE;

union GPUCOMMAND_CONTEXT_UPDATE {
    ulong Unused:32;
    ulong dword[1];
};

typedef enum _MODE {
    KernelMode=0,
    UserMode=1,
    MaximumMode=2
} _MODE;

typedef enum _DASH_BACKSTACK_ENTRYPOINT_TYPE {
    DASH_ENTRYPOINT_INVALID=0,
    EPIX_ENTRYPOINT_BDE=1,
    EPIX_ENTRYPOINT_MANIFEST=2
} _DASH_BACKSTACK_ENTRYPOINT_TYPE;

typedef enum _XCONFIG_STATIC_ENTRIES {
    XCONFIG_STATIC_DATA=0,
    XCONFIG_STATIC_FIRST_POWER_ON_DATE=1,
    XCONFIG_STATIC_SMC_CONFIG=2
} _XCONFIG_STATIC_ENTRIES;

typedef enum SX_PERFCNT_SELECT {
    SX_SC_QUADS=0,
    SX_SC_QUAD_FIFO_FULL=1,
    SX_EXPORT_VECTORS=2,
    SX_DUMMY_QUADS=3,
    SX_ALPHA_FAIL=4,
    SX_RB_QUAD_BUSY=5,
    SX_RB_COLOR_BUSY=6,
    SX_RB_QUAD_STALL=7,
    SX_RB_COLOR_STALL=8
} SX_PERFCNT_SELECT;

typedef enum GPUQUANTMODE {
    GPUQUANTMODE_16TH=0,
    GPUQUANTMODE_8TH=1,
    GPUQUANTMODE_4TH=2,
    GPUQUANTMODE_HALF=3,
    GPUQUANTMODE_ONE=4
} GPUQUANTMODE;

typedef enum DepthArray {
    ARRAY_2D_ALT_DEPTH=0,
    ARRAY_2D_DEPTH=1
} DepthArray;

typedef enum _XSHOWMARKETPLACEUI_ENTRYPOINTS {
    XSHOWMARKETPLACEUI_ENTRYPOINT_CONTENTLIST=0,
    XSHOWMARKETPLACEUI_ENTRYPOINT_CONTENTITEM=1,
    XSHOWMARKETPLACEUI_ENTRYPOINT_MEMBERSHIPLIST=2,
    XSHOWMARKETPLACEUI_ENTRYPOINT_MEMBERSHIPITEM=3,
    XSHOWMARKETPLACEUI_ENTRYPOINT_CONTENTLIST_BACKGROUND=4,
    XSHOWMARKETPLACEUI_ENTRYPOINT_CONTENTITEM_BACKGROUND=5,
    XSHOWMARKETPLACEUI_ENTRYPOINT_MAX=6
} _XSHOWMARKETPLACEUI_ENTRYPOINTS;

typedef enum <unnamed-tag_00001343> {
    FLAG_BIT_SSB_AUTH_EX_FAILURE=0,
    FLAG_SSB_CIV_HASH_FAILURE=0,
    FLAG_SSB_CONSOLE_BANNED=0,
    FLAG_SSB_KEYVAULT_POLICY_VIOLATION=0,
    FLAG_SSB_NONE=0,
    FLAG_SSB_ODD_VIOLATION=0,
    FLAG_SSB_UNAUTHORIZED_INSTALL=0,
    FLAG_BIT_SSB_AUTH_EX_NO_TABLE=1,
    FLAG_SSB_AUTH_EX_FAILURE=1,
    FLAG_BIT_SSB_AUTH_EX_RESERVED=2,
    FLAG_SSB_AUTH_EX_NO_TABLE=2,
    FLAG_BIT_SSB_INVALID_DVD_GEOMETRY=3,
    FLAG_BIT_SSB_INVALID_DVD_DMI=4,
    FLAG_SSB_AUTH_EX_RESERVED=4,
    FLAG_BIT_SSB_DVD_KEYVAULT_PAIR_MISMATCH=5,
    FLAG_BIT_SSB_CRL_DATA_INVALID=6,
    FLAG_BIT_SSB_CRL_CERTIFICATE_REVOKED=7,
    FLAG_BIT_SSB_UNAUTHORIZED_INSTALL=8,
    FLAG_SSB_INVALID_DVD_GEOMETRY=8,
    FLAG_BIT_SSB_KEYVAULT_POLICY_VIOLATION=9,
    FLAG_BIT_SSB_CONSOLE_BANNED=10,
    FLAG_BIT_SSB_ODD_VIOLATION=11,
    FLAG_BIT_SSB_CIV_HASH_FAILURE=12,
    FLAG_BIT_SSB_LAST=13,
    FLAG_SSB_INVALID_DVD_DMI=16,
    FLAG_SSB_DVD_KEYVAULT_PAIR_MISMATCH=32,
    FLAG_BIT_SSB_MAXIMUM=64,
    FLAG_SSB_CRL_DATA_INVALID=64,
    FLAG_SSB_CRL_CERTIFICATE_REVOKED=128
} <unnamed-tag_00001343>;

typedef enum <unnamed-tag_00001125> {
    SPR_HDEC=54
} <unnamed-tag_00001125>;

typedef enum GPUSYNCFUNCTION {
    GPUSYNCFUNCTION_NEVER=0,
    GPUSYNCFUNCTION_LESS=1,
    GPUSYNCFUNCTION_LEQUAL=2,
    GPUSYNCFUNCTION_EQUAL=3,
    GPUSYNCFUNCTION_NOTEQUAL=4,
    GPUSYNCFUNCTION_GEQUAL=5,
    GPUSYNCFUNCTION_GREATER=6,
    GPUSYNCFUNCTION_ALWAYS=7
} GPUSYNCFUNCTION;

typedef enum <unnamed-tag_00001149> {
    SPR_TSCR=153
} <unnamed-tag_00001149>;

typedef union RTS_INV_W_DX RTS_INV_W_DX, *PRTS_INV_W_DX;


/* WARNING! conflicting data type names: /iw5mp_xenon.pdb/RTS_INV_W_DX/<unnamed-type-bitfields> - /iw5mp_xenon.pdb/D1SCL_COEF_RAM_SELECT/<unnamed-type-bitfields> */

union RTS_INV_W_DX {
    struct <unnamed-type-bitfields> bitfields;
    struct <unnamed-type-bitfields> bits;
    uint u32All;
    int i32All;
    float f32All;
};


/* WARNING! conflicting data type names: /iw5mp_xenon.pdb/MC1_BSB_SNOOPED_TIMER_CNTL/<unnamed-type-bitfields>/<unnamed-type-bitfields>_u_0 - /iw5mp_xenon.pdb/D1SCL_COEF_RAM_SELECT/<unnamed-type-bitfields>/<unnamed-type-bitfields>_u_0 */


/* WARNING! conflicting data type names: /iw5mp_xenon.pdb/MC1_BSB_SNOOPED_TIMER_CNTL/<unnamed-type-bitfields>/<unnamed-type-bitfields>_u_0/<unnamed-type-bitfields>_u_0_s_0 - /iw5mp_xenon.pdb/D1SCL_COEF_RAM_SELECT/<unnamed-type-bitfields>/<unnamed-type-bitfields>_u_0/<unnamed-type-bitfields>_u_0_s_0 */

typedef enum YesNoMaybe {
    Yes=0,
    No=1,
    Maybe=16
} YesNoMaybe;

typedef enum AccessType {
    NoAccess=0,
    Read=1,
    Write=2,
    ReadWrite=3
} AccessType;


/* WARNING! conflicting data type names: /iw5mp_xenon.pdb/VGT_GROUP_VECT_1_FMT_CNTL/<unnamed-type-bitfields>/<unnamed-type-bitfields>_u_0 - /iw5mp_xenon.pdb/D1SCL_COEF_RAM_SELECT/<unnamed-type-bitfields>/<unnamed-type-bitfields>_u_0 */

typedef struct <unnamed-type-bitfields>_u_0_s_2 <unnamed-type-bitfields>_u_0_s_2, *P<unnamed-type-bitfields>_u_0_s_2;

typedef union <unnamed-type-bitfields>_u_0_s_2_u_0 <unnamed-type-bitfields>_u_0_s_2_u_0, *P<unnamed-type-bitfields>_u_0_s_2_u_0;

typedef struct <unnamed-type-bitfields>_u_0_s_2_u_0_s_0 <unnamed-type-bitfields>_u_0_s_2_u_0_s_0, *P<unnamed-type-bitfields>_u_0_s_2_u_0_s_0;

struct <unnamed-type-bitfields>_u_0_s_2_u_0_s_0 {
    undefined field0_0x0;
    undefined field1_0x1;
    uint Z_CONV:4;
};

union <unnamed-type-bitfields>_u_0_s_2_u_0 {
    struct <unnamed-type-bitfields>_u_0_s_2_u_0_s_0 _s_0;
    uint Y_OFFSET:4;
};

struct <unnamed-type-bitfields>_u_0_s_2 {
    union <unnamed-type-bitfields>_u_0_s_2_u_0 field0_0x0;
};

typedef struct <unnamed-type-bitfields>_u_0_s_3 <unnamed-type-bitfields>_u_0_s_3, *P<unnamed-type-bitfields>_u_0_s_3;

typedef union <unnamed-type-bitfields>_u_0_s_3_u_0 <unnamed-type-bitfields>_u_0_s_3_u_0, *P<unnamed-type-bitfields>_u_0_s_3_u_0;

typedef struct <unnamed-type-bitfields>_u_0_s_3_u_0_s_0 <unnamed-type-bitfields>_u_0_s_3_u_0_s_0, *P<unnamed-type-bitfields>_u_0_s_3_u_0_s_0;

struct <unnamed-type-bitfields>_u_0_s_3_u_0_s_0 {
    undefined field0_0x0;
    uint Y_CONV:4;
};

union <unnamed-type-bitfields>_u_0_s_3_u_0 {
    struct <unnamed-type-bitfields>_u_0_s_3_u_0_s_0 _s_0;
    uint X_OFFSET:4;
};

struct <unnamed-type-bitfields>_u_0_s_3 {
    union <unnamed-type-bitfields>_u_0_s_3_u_0 field0_0x0;
};


/* WARNING! conflicting data type names: /iw5mp_xenon.pdb/VGT_GROUP_VECT_1_FMT_CNTL/<unnamed-type-bitfields>/<unnamed-type-bitfields>_u_0/<unnamed-type-bitfields>_u_0_s_0 - /iw5mp_xenon.pdb/D1SCL_COEF_RAM_SELECT/<unnamed-type-bitfields>/<unnamed-type-bitfields>_u_0/<unnamed-type-bitfields>_u_0_s_0 */

typedef struct <unnamed-type-bitfields>_u_0_s_1 <unnamed-type-bitfields>_u_0_s_1, *P<unnamed-type-bitfields>_u_0_s_1;

typedef union <unnamed-type-bitfields>_u_0_s_1_u_0 <unnamed-type-bitfields>_u_0_s_1_u_0, *P<unnamed-type-bitfields>_u_0_s_1_u_0;

typedef struct <unnamed-type-bitfields>_u_0_s_1_u_0_s_0 <unnamed-type-bitfields>_u_0_s_1_u_0_s_0, *P<unnamed-type-bitfields>_u_0_s_1_u_0_s_0;

struct <unnamed-type-bitfields>_u_0_s_1_u_0_s_0 {
    undefined field0_0x0;
    undefined field1_0x1;
    undefined field2_0x2;
    uint W_CONV:4;
};

union <unnamed-type-bitfields>_u_0_s_1_u_0 {
    struct <unnamed-type-bitfields>_u_0_s_1_u_0_s_0 _s_0;
    uint Z_OFFSET:4;
};

struct <unnamed-type-bitfields>_u_0_s_1 {
    union <unnamed-type-bitfields>_u_0_s_1_u_0 field0_0x0;
};


/* WARNING! conflicting data type names: /iw5mp_xenon.pdb/MC0_TIMING_CNTL_1/<unnamed-type-bitfields>/<unnamed-type-bitfields>_u_0 - /iw5mp_xenon.pdb/D1SCL_COEF_RAM_SELECT/<unnamed-type-bitfields>/<unnamed-type-bitfields>_u_0 */


/* WARNING! conflicting data type names: /iw5mp_xenon.pdb/MC0_TIMING_CNTL_1/<unnamed-type-bitfields>/<unnamed-type-bitfields>_u_0/<unnamed-type-bitfields>_u_0_s_0 - /iw5mp_xenon.pdb/D1SCL_COEF_RAM_SELECT/<unnamed-type-bitfields>/<unnamed-type-bitfields>_u_0/<unnamed-type-bitfields>_u_0_s_0 */


/* WARNING! conflicting data type names: /iw5mp_xenon.pdb/MC0_TIMING_CNTL_1/<unnamed-type-bitfields>/<unnamed-type-bitfields>_u_0/<unnamed-type-bitfields>_u_0_s_1 - /iw5mp_xenon.pdb/VGT_GROUP_VECT_1_FMT_CNTL/<unnamed-type-bitfields>/<unnamed-type-bitfields>_u_0/<unnamed-type-bitfields>_u_0_s_1 */


/* WARNING! conflicting data type names: /iw5mp_xenon.pdb/MC0_TIMING_CNTL_1/<unnamed-type-bitfields>/<unnamed-type-bitfields>_u_0/<unnamed-type-bitfields>_u_0_s_1/<unnamed-type-bitfields>_u_0_s_1_u_0 - /iw5mp_xenon.pdb/VGT_GROUP_VECT_1_FMT_CNTL/<unnamed-type-bitfields>/<unnamed-type-bitfields>_u_0/<unnamed-type-bitfields>_u_0_s_1/<unnamed-type-bitfields>_u_0_s_1_u_0 */


/* WARNING! conflicting data type names: /iw5mp_xenon.pdb/MC0_TIMING_CNTL_1/<unnamed-type-bitfields>/<unnamed-type-bitfields>_u_0/<unnamed-type-bitfields>_u_0_s_1/<unnamed-type-bitfields>_u_0_s_1_u_0/<unnamed-type-bitfields>_u_0_s_1_u_0_s_0 - /iw5mp_xenon.pdb/VGT_GROUP_VECT_1_FMT_CNTL/<unnamed-type-bitfields>/<unnamed-type-bitfields>_u_0/<unnamed-type-bitfields>_u_0_s_1/<unnamed-type-bitfields>_u_0_s_1_u_0/<unnamed-type-bitfields>_u_0_s_1_u_0_s_0 */


/* WARNING! conflicting data type names: /iw5mp_xenon.pdb/CONFIG_CNTL/<unnamed-type-bitfields>/<unnamed-type-bitfields>_u_0 - /iw5mp_xenon.pdb/D1SCL_COEF_RAM_SELECT/<unnamed-type-bitfields>/<unnamed-type-bitfields>_u_0 */


/* WARNING! conflicting data type names: /iw5mp_xenon.pdb/CONFIG_CNTL/<unnamed-type-bitfields>/<unnamed-type-bitfields>_u_0/<unnamed-type-bitfields>_u_0_s_0 - /iw5mp_xenon.pdb/D1SCL_COEF_RAM_SELECT/<unnamed-type-bitfields>/<unnamed-type-bitfields>_u_0/<unnamed-type-bitfields>_u_0_s_0 */


/* WARNING! conflicting data type names: /iw5mp_xenon.pdb/RBBM_INT_ACK/<unnamed-type-bitfields>/<unnamed-type-bitfields>_u_0 - /iw5mp_xenon.pdb/D1SCL_COEF_RAM_SELECT/<unnamed-type-bitfields>/<unnamed-type-bitfields>_u_0 */


/* WARNING! conflicting data type names: /iw5mp_xenon.pdb/RBBM_INT_ACK/<unnamed-type-bitfields>/<unnamed-type-bitfields>_u_0/<unnamed-type-bitfields>_u_0_s_0 - /iw5mp_xenon.pdb/D1SCL_COEF_RAM_SELECT/<unnamed-type-bitfields>/<unnamed-type-bitfields>_u_0/<unnamed-type-bitfields>_u_0_s_0 */


/* WARNING! conflicting data type names: /iw5mp_xenon.pdb/DCCG_CLK_CNTL/<unnamed-type-bitfields>/<unnamed-type-bitfields>_u_0 - /iw5mp_xenon.pdb/D1SCL_COEF_RAM_SELECT/<unnamed-type-bitfields>/<unnamed-type-bitfields>_u_0 */


/* WARNING! conflicting data type names: /iw5mp_xenon.pdb/DCCG_CLK_CNTL/<unnamed-type-bitfields>/<unnamed-type-bitfields>_u_0/<unnamed-type-bitfields>_u_0_s_0 - /iw5mp_xenon.pdb/D1SCL_COEF_RAM_SELECT/<unnamed-type-bitfields>/<unnamed-type-bitfields>_u_0/<unnamed-type-bitfields>_u_0_s_0 */


/* WARNING! conflicting data type names: /iw5mp_xenon.pdb/PA_SC_DEBUG_CW_1/<unnamed-type-bitfields>/<unnamed-type-bitfields>_u_0 - /iw5mp_xenon.pdb/D1SCL_COEF_RAM_SELECT/<unnamed-type-bitfields>/<unnamed-type-bitfields>_u_0 */


/* WARNING! conflicting data type names: /iw5mp_xenon.pdb/PA_SC_DEBUG_CW_1/<unnamed-type-bitfields>/<unnamed-type-bitfields>_u_0/<unnamed-type-bitfields>_u_0_s_0 - /iw5mp_xenon.pdb/D1SCL_COEF_RAM_SELECT/<unnamed-type-bitfields>/<unnamed-type-bitfields>_u_0/<unnamed-type-bitfields>_u_0_s_0 */


/* WARNING! conflicting data type names: /iw5mp_xenon.pdb/EP_UNUSED_BAR_4/<unnamed-type-bitfields>/<unnamed-type-bitfields>_u_0 - /iw5mp_xenon.pdb/D1SCL_COEF_RAM_SELECT/<unnamed-type-bitfields>/<unnamed-type-bitfields>_u_0 */


/* WARNING! conflicting data type names: /iw5mp_xenon.pdb/EP_UNUSED_BAR_4/<unnamed-type-bitfields>/<unnamed-type-bitfields>_u_0/<unnamed-type-bitfields>_u_0_s_0 - /iw5mp_xenon.pdb/D1SCL_COEF_RAM_SELECT/<unnamed-type-bitfields>/<unnamed-type-bitfields>_u_0/<unnamed-type-bitfields>_u_0_s_0 */


/* WARNING! conflicting data type names: /iw5mp_xenon.pdb/IRQ_BRIDGE_CNTL/<unnamed-type-bitfields>/<unnamed-type-bitfields>_u_0 - /iw5mp_xenon.pdb/D1SCL_COEF_RAM_SELECT/<unnamed-type-bitfields>/<unnamed-type-bitfields>_u_0 */


/* WARNING! conflicting data type names: /iw5mp_xenon.pdb/IRQ_BRIDGE_CNTL/<unnamed-type-bitfields>/<unnamed-type-bitfields>_u_0/<unnamed-type-bitfields>_u_0_s_0 - /iw5mp_xenon.pdb/D1SCL_COEF_RAM_SELECT/<unnamed-type-bitfields>/<unnamed-type-bitfields>_u_0/<unnamed-type-bitfields>_u_0_s_0 */


/* WARNING! conflicting data type names: /iw5mp_xenon.pdb/CP_RT10_COMMAND/<unnamed-type-bitfields>/<unnamed-type-bitfields>_u_0 - /iw5mp_xenon.pdb/D1SCL_COEF_RAM_SELECT/<unnamed-type-bitfields>/<unnamed-type-bitfields>_u_0 */

typedef struct <unnamed-type-bitfields>_u_0_s_4 <unnamed-type-bitfields>_u_0_s_4, *P<unnamed-type-bitfields>_u_0_s_4;

typedef union <unnamed-type-bitfields>_u_0_s_4_u_0 <unnamed-type-bitfields>_u_0_s_4_u_0, *P<unnamed-type-bitfields>_u_0_s_4_u_0;

typedef struct <unnamed-type-bitfields>_u_0_s_4_u_0_s_0 <unnamed-type-bitfields>_u_0_s_4_u_0_s_0, *P<unnamed-type-bitfields>_u_0_s_4_u_0_s_0;

struct <unnamed-type-bitfields>_u_0_s_4_u_0_s_0 {
    undefined field0_0x0;
    uint SIGNAL_CONDITIONING:2;
    undefined field2_0x2;
    undefined field3_0x3;
};

union <unnamed-type-bitfields>_u_0_s_4_u_0 {
    struct <unnamed-type-bitfields>_u_0_s_4_u_0_s_0 _s_0;
    uint SIGNAL_SOURCE:5;
};

struct <unnamed-type-bitfields>_u_0_s_4 {
    union <unnamed-type-bitfields>_u_0_s_4_u_0 field0_0x0;
};


/* WARNING! conflicting data type names: /iw5mp_xenon.pdb/CP_RT10_COMMAND/<unnamed-type-bitfields>/<unnamed-type-bitfields>_u_0/<unnamed-type-bitfields>_u_0_s_2 - /iw5mp_xenon.pdb/VGT_GROUP_VECT_1_FMT_CNTL/<unnamed-type-bitfields>/<unnamed-type-bitfields>_u_0/<unnamed-type-bitfields>_u_0_s_2 */


/* WARNING! conflicting data type names: /iw5mp_xenon.pdb/CP_RT10_COMMAND/<unnamed-type-bitfields>/<unnamed-type-bitfields>_u_0/<unnamed-type-bitfields>_u_0_s_3 - /iw5mp_xenon.pdb/VGT_GROUP_VECT_1_FMT_CNTL/<unnamed-type-bitfields>/<unnamed-type-bitfields>_u_0/<unnamed-type-bitfields>_u_0_s_3 */


/* WARNING! conflicting data type names: /iw5mp_xenon.pdb/CP_RT10_COMMAND/<unnamed-type-bitfields>/<unnamed-type-bitfields>_u_0/<unnamed-type-bitfields>_u_0_s_0 - /iw5mp_xenon.pdb/D1SCL_COEF_RAM_SELECT/<unnamed-type-bitfields>/<unnamed-type-bitfields>_u_0/<unnamed-type-bitfields>_u_0_s_0 */


/* WARNING! conflicting data type names: /iw5mp_xenon.pdb/CP_RT10_COMMAND/<unnamed-type-bitfields>/<unnamed-type-bitfields>_u_0/<unnamed-type-bitfields>_u_0_s_1 - /iw5mp_xenon.pdb/VGT_GROUP_VECT_1_FMT_CNTL/<unnamed-type-bitfields>/<unnamed-type-bitfields>_u_0/<unnamed-type-bitfields>_u_0_s_1 */


/* WARNING! conflicting data type names: /iw5mp_xenon.pdb/CP_RT10_COMMAND/<unnamed-type-bitfields>/<unnamed-type-bitfields>_u_0/<unnamed-type-bitfields>_u_0_s_2/<unnamed-type-bitfields>_u_0_s_2_u_0 - /iw5mp_xenon.pdb/VGT_GROUP_VECT_1_FMT_CNTL/<unnamed-type-bitfields>/<unnamed-type-bitfields>_u_0/<unnamed-type-bitfields>_u_0_s_2/<unnamed-type-bitfields>_u_0_s_2_u_0 */


/* WARNING! conflicting data type names: /iw5mp_xenon.pdb/CP_RT10_COMMAND/<unnamed-type-bitfields>/<unnamed-type-bitfields>_u_0/<unnamed-type-bitfields>_u_0_s_2/<unnamed-type-bitfields>_u_0_s_2_u_0/<unnamed-type-bitfields>_u_0_s_2_u_0_s_0 - /iw5mp_xenon.pdb/VGT_GROUP_VECT_1_FMT_CNTL/<unnamed-type-bitfields>/<unnamed-type-bitfields>_u_0/<unnamed-type-bitfields>_u_0_s_2/<unnamed-type-bitfields>_u_0_s_2_u_0/<unnamed-type-bitfields>_u_0_s_2_u_0_s_0 */


/* WARNING! conflicting data type names: /iw5mp_xenon.pdb/CP_RT10_COMMAND/<unnamed-type-bitfields>/<unnamed-type-bitfields>_u_0/<unnamed-type-bitfields>_u_0_s_3/<unnamed-type-bitfields>_u_0_s_3_u_0 - /iw5mp_xenon.pdb/VGT_GROUP_VECT_1_FMT_CNTL/<unnamed-type-bitfields>/<unnamed-type-bitfields>_u_0/<unnamed-type-bitfields>_u_0_s_3/<unnamed-type-bitfields>_u_0_s_3_u_0 */


/* WARNING! conflicting data type names: /iw5mp_xenon.pdb/CP_RT10_COMMAND/<unnamed-type-bitfields>/<unnamed-type-bitfields>_u_0/<unnamed-type-bitfields>_u_0_s_3/<unnamed-type-bitfields>_u_0_s_3_u_0/<unnamed-type-bitfields>_u_0_s_3_u_0_s_0 - /iw5mp_xenon.pdb/VGT_GROUP_VECT_1_FMT_CNTL/<unnamed-type-bitfields>/<unnamed-type-bitfields>_u_0/<unnamed-type-bitfields>_u_0_s_3/<unnamed-type-bitfields>_u_0_s_3_u_0/<unnamed-type-bitfields>_u_0_s_3_u_0_s_0 */


/* WARNING! conflicting data type names: /iw5mp_xenon.pdb/CP_RT10_COMMAND/<unnamed-type-bitfields>/<unnamed-type-bitfields>_u_0/<unnamed-type-bitfields>_u_0_s_1/<unnamed-type-bitfields>_u_0_s_1_u_0 - /iw5mp_xenon.pdb/VGT_GROUP_VECT_1_FMT_CNTL/<unnamed-type-bitfields>/<unnamed-type-bitfields>_u_0/<unnamed-type-bitfields>_u_0_s_1/<unnamed-type-bitfields>_u_0_s_1_u_0 */


/* WARNING! conflicting data type names: /iw5mp_xenon.pdb/CP_RT10_COMMAND/<unnamed-type-bitfields>/<unnamed-type-bitfields>_u_0/<unnamed-type-bitfields>_u_0_s_1/<unnamed-type-bitfields>_u_0_s_1_u_0/<unnamed-type-bitfields>_u_0_s_1_u_0_s_0 - /iw5mp_xenon.pdb/VGT_GROUP_VECT_1_FMT_CNTL/<unnamed-type-bitfields>/<unnamed-type-bitfields>_u_0/<unnamed-type-bitfields>_u_0_s_1/<unnamed-type-bitfields>_u_0_s_1_u_0/<unnamed-type-bitfields>_u_0_s_1_u_0_s_0 */


/* WARNING! conflicting data type names: /iw5mp_xenon.pdb/D1MODE_PRIORITY_A_CNT/<unnamed-type-bitfields>/<unnamed-type-bitfields>_u_0 - /iw5mp_xenon.pdb/D1SCL_COEF_RAM_SELECT/<unnamed-type-bitfields>/<unnamed-type-bitfields>_u_0 */


/* WARNING! conflicting data type names: /iw5mp_xenon.pdb/D1MODE_PRIORITY_A_CNT/<unnamed-type-bitfields>/<unnamed-type-bitfields>_u_0/<unnamed-type-bitfields>_u_0_s_0 - /iw5mp_xenon.pdb/D1SCL_COEF_RAM_SELECT/<unnamed-type-bitfields>/<unnamed-type-bitfields>_u_0/<unnamed-type-bitfields>_u_0_s_0 */


/* WARNING! conflicting data type names: /iw5mp_xenon.pdb/D1MODE_PRIORITY_A_CNT/<unnamed-type-bitfields>/<unnamed-type-bitfields>_u_0/<unnamed-type-bitfields>_u_0_s_1 - /iw5mp_xenon.pdb/VGT_GROUP_VECT_1_FMT_CNTL/<unnamed-type-bitfields>/<unnamed-type-bitfields>_u_0/<unnamed-type-bitfields>_u_0_s_1 */


/* WARNING! conflicting data type names: /iw5mp_xenon.pdb/D1MODE_PRIORITY_A_CNT/<unnamed-type-bitfields>/<unnamed-type-bitfields>_u_0/<unnamed-type-bitfields>_u_0_s_1/<unnamed-type-bitfields>_u_0_s_1_u_0 - /iw5mp_xenon.pdb/VGT_GROUP_VECT_1_FMT_CNTL/<unnamed-type-bitfields>/<unnamed-type-bitfields>_u_0/<unnamed-type-bitfields>_u_0_s_1/<unnamed-type-bitfields>_u_0_s_1_u_0 */


/* WARNING! conflicting data type names: /iw5mp_xenon.pdb/D1MODE_PRIORITY_A_CNT/<unnamed-type-bitfields>/<unnamed-type-bitfields>_u_0/<unnamed-type-bitfields>_u_0_s_1/<unnamed-type-bitfields>_u_0_s_1_u_0/<unnamed-type-bitfields>_u_0_s_1_u_0_s_0 - /iw5mp_xenon.pdb/VGT_GROUP_VECT_1_FMT_CNTL/<unnamed-type-bitfields>/<unnamed-type-bitfields>_u_0/<unnamed-type-bitfields>_u_0_s_1/<unnamed-type-bitfields>_u_0_s_1_u_0/<unnamed-type-bitfields>_u_0_s_1_u_0_s_0 */


/* WARNING! conflicting data type names: /iw5mp_xenon.pdb/VC_CNTL/<unnamed-type-bitfields>/<unnamed-type-bitfields>_u_0 - /iw5mp_xenon.pdb/D1SCL_COEF_RAM_SELECT/<unnamed-type-bitfields>/<unnamed-type-bitfields>_u_0 */


/* WARNING! conflicting data type names: /iw5mp_xenon.pdb/VC_CNTL/<unnamed-type-bitfields>/<unnamed-type-bitfields>_u_0/<unnamed-type-bitfields>_u_0_s_0 - /iw5mp_xenon.pdb/D1SCL_COEF_RAM_SELECT/<unnamed-type-bitfields>/<unnamed-type-bitfields>_u_0/<unnamed-type-bitfields>_u_0_s_0 */


/* WARNING! conflicting data type names: /iw5mp_xenon.pdb/FSB_TR_CTRL_OR/<unnamed-type-bitfields>/<unnamed-type-bitfields>_u_0 - /iw5mp_xenon.pdb/D1SCL_COEF_RAM_SELECT/<unnamed-type-bitfields>/<unnamed-type-bitfields>_u_0 */


/* WARNING! conflicting data type names: /iw5mp_xenon.pdb/FSB_TR_CTRL_OR/<unnamed-type-bitfields>/<unnamed-type-bitfields>_u_0/<unnamed-type-bitfields>_u_0_s_0 - /iw5mp_xenon.pdb/D1SCL_COEF_RAM_SELECT/<unnamed-type-bitfields>/<unnamed-type-bitfields>_u_0/<unnamed-type-bitfields>_u_0_s_0 */

typedef struct _LARGE_INTEGER_s_0 _LARGE_INTEGER_s_0, *P_LARGE_INTEGER_s_0;

struct _LARGE_INTEGER_s_0 {
    long HighPart;
    ulong LowPart;
};


/* WARNING! conflicting data type names: /iw5mp_xenon.pdb/_LARGE_INTEGER/<unnamed-type-u> - /iw5mp_xenon.pdb/_ULARGE_INTEGER/<unnamed-type-u> */


/* WARNING! conflicting data type names: /iw5mp_xenon.pdb/CG_INT_MASK_REG/<unnamed-type-bitfields>/<unnamed-type-bitfields>_u_0 - /iw5mp_xenon.pdb/D1SCL_COEF_RAM_SELECT/<unnamed-type-bitfields>/<unnamed-type-bitfields>_u_0 */


/* WARNING! conflicting data type names: /iw5mp_xenon.pdb/CG_INT_MASK_REG/<unnamed-type-bitfields>/<unnamed-type-bitfields>_u_0/<unnamed-type-bitfields>_u_0_s_0 - /iw5mp_xenon.pdb/D1SCL_COEF_RAM_SELECT/<unnamed-type-bitfields>/<unnamed-type-bitfields>_u_0/<unnamed-type-bitfields>_u_0_s_0 */


/* WARNING! conflicting data type names: /iw5mp_xenon.pdb/PCIE_VC0_RESOURCE_STATUS/<unnamed-type-bitfields>/<unnamed-type-bitfields>_u_0 - /iw5mp_xenon.pdb/D1SCL_COEF_RAM_SELECT/<unnamed-type-bitfields>/<unnamed-type-bitfields>_u_0 */


/* WARNING! conflicting data type names: /iw5mp_xenon.pdb/PCIE_VC0_RESOURCE_STATUS/<unnamed-type-bitfields>/<unnamed-type-bitfields>_u_0/<unnamed-type-bitfields>_u_0_s_0 - /iw5mp_xenon.pdb/D1SCL_COEF_RAM_SELECT/<unnamed-type-bitfields>/<unnamed-type-bitfields>_u_0/<unnamed-type-bitfields>_u_0_s_0 */

typedef enum LazyState {
    Lazy_PixelShaderConstants=0,
    Lazy_VertexShaderConstants=0,
    Lazy_WaitUntilIdle=0,
    Lazy_WaitUntilIdleFlags=0,
    Lazy_FlushCaches=1,
    Lazy_DestinationPacket=6,
    Lazy_SurfaceInfo=6,
    Lazy_Color0Info=7,
    Lazy_FlowConstants=7,
    Lazy_MiscConstants=7,
    Lazy_DepthInfo=8,
    Lazy_Reserved0=8,
    Lazy_ClipPlane0=9,
    Lazy_ClipPlanes=9,
    Lazy_Color1Info=9,
    Lazy_PointSize=9,
    Lazy_TessellatorPacket=9,
    Lazy_ClipPlane1=10,
    Lazy_Color2Info=10,
    Lazy_PointMinMax=10,
    Lazy_ClipPlane2=11,
    Lazy_Color3Info=11,
    Lazy_LineControl=11,
    Lazy_ClipPlane3=12,
    Lazy_CoherDestBase0=12,
    Lazy_LineStipple=12,
    Lazy_ClipPlane4=13,
    Lazy_CoherDestBase1=13,
    Lazy_OutputPathControl=13,
    Lazy_ClipPlane5=14,
    Lazy_CoherDestBase2=14,
    Lazy_HosControl=14,
    Lazy_CoherDestBase3=15,
    Lazy_HosMaxTessLevel=15,
    Lazy_Terminator=15,
    Lazy_CoherDestBase4=16,
    Lazy_HosMinTessLevel=16,
    Lazy_CoherDestBase5=17,
    Lazy_HosReuseDepth=17,
    Lazy_CoherDestBase6=18,
    Lazy_GroupPrimType=18,
    Lazy_PointPacket=18,
    Lazy_PolyOffsetFrontScale=18,
    Lazy_CoherDestBase7=19,
    Lazy_GroupFirstDecr=19,
    Lazy_PolyOffsetFrontOffset=19,
    Lazy_GroupDecr=20,
    Lazy_PolyOffsetBackScale=20,
    Lazy_ScreenScissorTL=20,
    Lazy_GroupVect0Control=21,
    Lazy_PolyOffsetBackOffset=21,
    Lazy_ScreenScissorBR=21,
    Lazy_GroupVect1Control=22,
    Lazy_MaxVtxIndx=22,
    Lazy_PointXRad=22,
    Lazy_ValuesPacket=22,
    Lazy_GroupVect0FmtControl=23,
    Lazy_MinVtxIndx=23,
    Lazy_PointYRad=23,
    Lazy_GroupVect1FmtControl=24,
    Lazy_IndxOffset=24,
    Lazy_PointConstantSize=24,
    Lazy_MultiPrimIbResetIndx=25,
    Lazy_PointCullRad=25,
    Lazy_Unused_Tessellator0=25,
    Lazy_ColorMask=26,
    Lazy_MiscPacket=26,
    Lazy_ScLineControl=26,
    Lazy_Unused_Tessellator1=26,
    Lazy_AaConfig=27,
    Lazy_BlendRed=27,
    Lazy_MPassPsControl=27,
    Lazy_BlendGreen=28,
    Lazy_VizQuery=28,
    Lazy_VtxControl=28,
    Lazy_BlendBlue=29,
    Lazy_Enhance=29,
    Lazy_GbVertClipAdj=29,
    Lazy_BlendAlpha=30,
    Lazy_GbVertDiscAdj=30,
    Lazy_GbHorzClipAdj=31,
    Lazy_Unused_Values0=31,
    Lazy_FetchConstants=32,
    Lazy_GbHorzDiscAdj=32,
    Lazy_TextureFetchConstants=32,
    Lazy_Unused_Values1=32,
    Lazy_Unused_Values2=33,
    Lazy_VsConst=33,
    Lazy_PsConst=34,
    Lazy_StencilRefMaskBF=34,
    Lazy_DebugMisc0=35,
    Lazy_StencilRefMask=35,
    Lazy_AlphaRef=36,
    Lazy_DebugMisc1=36,
    Lazy_Unused_Misc0=37,
    Lazy_VportXScale=37,
    Lazy_Unused_Misc1=38,
    Lazy_VportXOffset=38,
    Lazy_Unused_Misc2=39,
    Lazy_VportYScale=39,
    Lazy_Unused_Misc3=40,
    Lazy_VportYOffset=40,
    Lazy_Unused_Misc4=41,
    Lazy_VportZScale=41,
    Lazy_Unused_Misc5=42,
    Lazy_VportZOffset=42,
    Lazy_LoadPixelShader=43,
    Lazy_LoadShaders=43,
    Lazy_Unused_Misc6=43,
    Lazy_AaMask=44,
    Lazy_LoadVertexShader=44,
    Lazy_Unused_Misc7=45,
    Lazy_ZPassEdramMode=45,
    Lazy_HiZEnable=46,
    Lazy_Unused_Misc8=46,
    Lazy_ProgramControl=47,
    Lazy_ProgramPacket=47,
    Lazy_Unused_Misc9=47,
    Lazy_ContextMisc=48,
    Lazy_VertexReuseBlockControl=48,
    Lazy_InterpolatorControl=49,
    Lazy_OutDeallocControl=49,
    Lazy_CopyControl=50,
    Lazy_Wrapping0=50,
    Lazy_CopyDestBase=51,
    Lazy_Wrapping1=51,
    Lazy_ControlPacket=52,
    Lazy_CopyDestPitch=52,
    Lazy_DepthControl=52,
    Lazy_BlendControl0=53,
    Lazy_CopyDestInfo=53,
    Lazy_ColorControl=54,
    Lazy_HiClear=54,
    Lazy_DepthClear=55,
    Lazy_HiControl=55,
    Lazy_ClipControl=56,
    Lazy_ColorClear=56,
    Lazy_ColorClearLo=57,
    Lazy_ModeControl=57,
    Lazy_CopyFunc=58,
    Lazy_VertexFetchConstants=58,
    Lazy_VteControl=58,
    Lazy_CopyRef=59,
    Lazy_Unused_Control0=59,
    Lazy_CopyMask=60,
    Lazy_EdramModeControl=60,
    Lazy_BlendControl1=61,
    Lazy_CopySurfaceSlice=61,
    Lazy_BlendControl2=62,
    Lazy_SampleCountControl=62,
    Lazy_BlendControl3=63,
    Lazy_SampleCountAddress=63
} LazyState;

typedef enum BeginType {
    BeginType_None=0,
    BeginType_BeginRing=1,
    BeginType_BeginRingBig=2,
    BeginType_BeginRingManual=3,
    BeginType_BeginBuffer=4
} BeginType;

typedef enum ERTWorkerCollectorCommands {
    eFence=0,
    eThumbnailForHash=1,
    eThumbnailForHash2=2,
    eThumbnailForPix=3,
    eThumbnailForChannelCheck=4,
    eMipData=5,
    eResolveData=6,
    eUpdateIssue=7,
    eEndCapture=8,
    eSynchronizeResourceTrackerThreads=9,
    eScreenshot=10,
    eDestroyData=11
} ERTWorkerCollectorCommands;

typedef enum BracketType {
    BracketType_None=0,
    BracketType_BeginVertices=1,
    BracketType_BeginIndexedVertices=2,
    BracketType_BeginShaderConstantF1=3,
    BracketType_BeginShaderConstantF4=4,
    BracketType_GpuBeginShaderConstantF4=5
} BracketType;

typedef enum MIPPACKINGTYPE {
    MIPPACKING_DISABLED=0,
    MIPPACKING_ENABLED=1,
    MIPPACKING_DEFAULT=2
} MIPPACKINGTYPE;

typedef enum LazyPrim {
    LazyPrim_Regular=0,
    LazyPrim_RunCommandBuffer=1,
    LazyPrim_ResolveOrClear=2,
    LazyPrim_EndCommandBuffer=3
} LazyPrim;

typedef enum Queue_Commands {
    Queue_BeginTiling=0,
    Queue_BeginZPass=0,
    Queue_CommandFlag=0,
    Queue_CommandMask=0,
    Queue_EndTiling=0,
    Queue_EndZPass=0,
    Queue_IndirectBuffer1=0,
    Queue_IndirectBuffer1PFD=0,
    Queue_Initialize=0,
    Queue_Invalidate=0,
    Queue_InvokeRenderPass=0,
    Queue_NextChunkFlags=0,
    Queue_TilingPatch=0,
    Queue_WorkerInterrupt=0,
    Queue_XpsAutoPredication=0,
    Queue_XpsBegin=0,
    Queue_XpsEnd=0,
    Queue_XpsSetCallback=0,
    Queue_XpsSetPredication=0,
    Queue_LastCommandMarker=1
} Queue_Commands;

typedef enum TEXTURE_ISSUE {
    ISSUE_HASH_CONFLICT=0,
    ISSUE_NONE=0,
    ISSUE_REDUNDANT_ALL=0,
    ISSUE_REDUNDANT_NO_NEAR=0,
    ISSUE_REDUNDANT_W_CONST=0,
    ISSUE_REDUNDANT_W_NEAR_CONST=0,
    ISSUE_REDUNDANT_X_CONST=0,
    ISSUE_REDUNDANT_X_EQ_W=0,
    ISSUE_REDUNDANT_X_EQ_Y=0,
    ISSUE_REDUNDANT_X_EQ_Z=0,
    ISSUE_REDUNDANT_X_NEAR_CONST=0,
    ISSUE_REDUNDANT_X_NEAR_W=0,
    ISSUE_REDUNDANT_X_NEAR_Y=0,
    ISSUE_REDUNDANT_X_NEAR_Z=0,
    ISSUE_REDUNDANT_Y_CONST=0,
    ISSUE_REDUNDANT_Y_EQ_W=0,
    ISSUE_REDUNDANT_Y_EQ_Z=0,
    ISSUE_REDUNDANT_Y_NEAR_CONST=0,
    ISSUE_REDUNDANT_Y_NEAR_W=0,
    ISSUE_REDUNDANT_Y_NEAR_Z=0,
    ISSUE_REDUNDANT_Z_CONST=0,
    ISSUE_REDUNDANT_Z_EQ_W=0,
    ISSUE_REDUNDANT_Z_NEAR_CONST=0,
    ISSUE_REDUNDANT_Z_NEAR_W=0,
    ISSUE_USED_ALL=0,
    ISSUE_USED_AS_RESOLVE_TARGET=0,
    ISSUE_USED_IN_UNHACKED_SHADER=0,
    ISSUE_USED_IN_VERTEX_SHADER=0,
    ISSUE_UNUSED_X=1,
    ISSUE_UNUSED_Y=2,
    ISSUE_UNUSED_Z=4,
    ISSUE_REDUNDANT_SHIFT=8,
    ISSUE_UNUSED_W=8,
    ISSUE_UNUSED_MIP__MAXMIP_0=16,
    ISSUE_UNUSED_MIP__MIPFILTER_BASE=32,
    ISSUE_UNUSED_SHADER_RELATED_ALL=47,
    ISSUE_UNUSED_BASE=64,
    ISSUE_UNUSED_CONST=128,
    ISSUE_UNUSED_ALL=255
} TEXTURE_ISSUE;

typedef enum _D3DPATCH {
    D3DPATCH_SCREENSPACEEXTENTS=0,
    D3DPATCH_R300_VIEWPORT=1,
    D3DPATCH_R300_SCISSOR=2,
    D3DPATCH_R300_DRAWVERTICESUP=3,
    D3DPATCH_R300_DRAWVERTICES=4,
    D3DPATCH_R300_DRAWINDEXEDVERTICES=5,
    D3DPATCH_R300_INSERTCALLBACK=6
} _D3DPATCH;

typedef enum D3DQUERYSTATE {
    QUERY_READYFORBEGIN=1,
    QUERY_READYFOREND=2
} D3DQUERYSTATE;

typedef enum XBMCAPTURETYPE {
    XBMCAPTURE_FULL=0,
    XBMCAPTURE_THUMBNAIL=1,
    XBMCAPTURE_GAMEREPLAY_1280_720_CLIPPED=2
} XBMCAPTURETYPE;

typedef enum ERareInstruction {
    eMysteryOps_2_3_27=0,
    eMysteryOp_4=1,
    eVfetchTcache=2,
    eSetGradientsH=3,
    eSetGradientsV=4,
    eGetBCF=5,
    eGetWeights=6,
    ePixelVectors=7,
    eRareInstructionCount=7
} ERareInstruction;

typedef enum BufferState {
    BufferState_Uninitialized=0,
    BufferState_Runnable=1,
    BufferState_CommandBufferBracket=2,
    BufferState_FixupCreationBracket=3,
    BufferState_ReconstructionBracket=4,
    BufferState_DynamicFixupBracket=5,
    BufferState_Deconstructed=6,
    BufferState_OutOfMemory=7
} BufferState;

typedef enum <unnamed-tag_00001059> {
    RU_BUF_LEN=0
} <unnamed-tag_00001059>;

typedef enum MEMORYREGION {
    MEMREGION_SYSTEM=0,
    MEMREGION_TITLE=1,
    MEMREGION_XAM_FRONT_BUFFER=2
} MEMORYREGION;

typedef enum HackFailureReason {
    eSuccess=0,
    eNotNecessary=1,
    eTooManyGPRs=2,
    eTooManyCConstants=3,
    eIndexedCConstants=4,
    eTooManyInterpolants=5,
    eTooManyFetchConstants=6,
    eTooManyInstructions=7,
    eFetchesTooComplex=8,
    eOutOfMemory=9,
    eUnknownFailure=10,
    eNYI=11
} HackFailureReason;

typedef enum FixupType {
    Fixup_SetSurfaces=1,
    Fixup_SetClipRect=2,
    Fixup_SetViewport=3,
    Fixup_SetShaderConstant=4,
    Fixup_SetVertexShader=5,
    Fixup_SetPixelShader=6,
    Fixup_SetTexture=7,
    Fixup_SetIndexBuffer=8,
    Fixup_SetVertexBuffer=9,
    Fixup_SetConstantBuffer=10,
    Fixup_SetCommandBuffer=11
} FixupType;


/* WARNING! conflicting data type names: /iw5mp_xenon.pdb/PCIE_LC_LINK_WIDTH_CNTL/<unnamed-type-bitfields>/<unnamed-type-bitfields>_u_0 - /iw5mp_xenon.pdb/D1SCL_COEF_RAM_SELECT/<unnamed-type-bitfields>/<unnamed-type-bitfields>_u_0 */


/* WARNING! conflicting data type names: /iw5mp_xenon.pdb/PCIE_LC_LINK_WIDTH_CNTL/<unnamed-type-bitfields>/<unnamed-type-bitfields>_u_0/<unnamed-type-bitfields>_u_0_s_0 - /iw5mp_xenon.pdb/D1SCL_COEF_RAM_SELECT/<unnamed-type-bitfields>/<unnamed-type-bitfields>_u_0/<unnamed-type-bitfields>_u_0_s_0 */


/* WARNING! conflicting data type names: /iw5mp_xenon.pdb/PCIE_LC_LINK_WIDTH_CNTL/<unnamed-type-bitfields>/<unnamed-type-bitfields>_u_0/<unnamed-type-bitfields>_u_0_s_1 - /iw5mp_xenon.pdb/VGT_GROUP_VECT_1_FMT_CNTL/<unnamed-type-bitfields>/<unnamed-type-bitfields>_u_0/<unnamed-type-bitfields>_u_0_s_1 */


/* WARNING! conflicting data type names: /iw5mp_xenon.pdb/PCIE_LC_LINK_WIDTH_CNTL/<unnamed-type-bitfields>/<unnamed-type-bitfields>_u_0/<unnamed-type-bitfields>_u_0_s_1/<unnamed-type-bitfields>_u_0_s_1_u_0 - /iw5mp_xenon.pdb/VGT_GROUP_VECT_1_FMT_CNTL/<unnamed-type-bitfields>/<unnamed-type-bitfields>_u_0/<unnamed-type-bitfields>_u_0_s_1/<unnamed-type-bitfields>_u_0_s_1_u_0 */


/* WARNING! conflicting data type names: /iw5mp_xenon.pdb/PCIE_LC_LINK_WIDTH_CNTL/<unnamed-type-bitfields>/<unnamed-type-bitfields>_u_0/<unnamed-type-bitfields>_u_0_s_1/<unnamed-type-bitfields>_u_0_s_1_u_0/<unnamed-type-bitfields>_u_0_s_1_u_0_s_0 - /iw5mp_xenon.pdb/VGT_GROUP_VECT_1_FMT_CNTL/<unnamed-type-bitfields>/<unnamed-type-bitfields>_u_0/<unnamed-type-bitfields>_u_0_s_1/<unnamed-type-bitfields>_u_0_s_1_u_0/<unnamed-type-bitfields>_u_0_s_1_u_0_s_0 */


/* WARNING! conflicting data type names: /iw5mp_xenon.pdb/MC0_STATUS_1/<unnamed-type-bitfields>/<unnamed-type-bitfields>_u_0 - /iw5mp_xenon.pdb/D1SCL_COEF_RAM_SELECT/<unnamed-type-bitfields>/<unnamed-type-bitfields>_u_0 */


/* WARNING! conflicting data type names: /iw5mp_xenon.pdb/MC0_STATUS_1/<unnamed-type-bitfields>/<unnamed-type-bitfields>_u_0/<unnamed-type-bitfields>_u_0_s_0 - /iw5mp_xenon.pdb/D1SCL_COEF_RAM_SELECT/<unnamed-type-bitfields>/<unnamed-type-bitfields>_u_0/<unnamed-type-bitfields>_u_0_s_0 */


/* WARNING! conflicting data type names: /iw5mp_xenon.pdb/MC0_STATUS_1/<unnamed-type-bitfields>/<unnamed-type-bitfields>_u_0/<unnamed-type-bitfields>_u_0_s_1 - /iw5mp_xenon.pdb/VGT_GROUP_VECT_1_FMT_CNTL/<unnamed-type-bitfields>/<unnamed-type-bitfields>_u_0/<unnamed-type-bitfields>_u_0_s_1 */


/* WARNING! conflicting data type names: /iw5mp_xenon.pdb/MC0_STATUS_1/<unnamed-type-bitfields>/<unnamed-type-bitfields>_u_0/<unnamed-type-bitfields>_u_0_s_4 - /iw5mp_xenon.pdb/CP_RT10_COMMAND/<unnamed-type-bitfields>/<unnamed-type-bitfields>_u_0/<unnamed-type-bitfields>_u_0_s_4 */

typedef struct <unnamed-type-bitfields>_u_0_s_5 <unnamed-type-bitfields>_u_0_s_5, *P<unnamed-type-bitfields>_u_0_s_5;

typedef union <unnamed-type-bitfields>_u_0_s_5_u_0 <unnamed-type-bitfields>_u_0_s_5_u_0, *P<unnamed-type-bitfields>_u_0_s_5_u_0;

typedef struct <unnamed-type-bitfields>_u_0_s_5_u_0_s_0 <unnamed-type-bitfields>_u_0_s_5_u_0_s_0, *P<unnamed-type-bitfields>_u_0_s_5_u_0_s_0;

struct <unnamed-type-bitfields>_u_0_s_5_u_0_s_0 {
    undefined field0_0x0;
    uint BM_STATE6:2;
    undefined field2_0x2;
    undefined field3_0x3;
};

union <unnamed-type-bitfields>_u_0_s_5_u_0 {
    struct <unnamed-type-bitfields>_u_0_s_5_u_0_s_0 _s_0;
    uint BM_STATE5:2;
};

struct <unnamed-type-bitfields>_u_0_s_5 {
    union <unnamed-type-bitfields>_u_0_s_5_u_0 field0_0x0;
};


/* WARNING! conflicting data type names: /iw5mp_xenon.pdb/MC0_STATUS_1/<unnamed-type-bitfields>/<unnamed-type-bitfields>_u_0/<unnamed-type-bitfields>_u_0_s_2 - /iw5mp_xenon.pdb/VGT_GROUP_VECT_1_FMT_CNTL/<unnamed-type-bitfields>/<unnamed-type-bitfields>_u_0/<unnamed-type-bitfields>_u_0_s_2 */


/* WARNING! conflicting data type names: /iw5mp_xenon.pdb/MC0_STATUS_1/<unnamed-type-bitfields>/<unnamed-type-bitfields>_u_0/<unnamed-type-bitfields>_u_0_s_3 - /iw5mp_xenon.pdb/VGT_GROUP_VECT_1_FMT_CNTL/<unnamed-type-bitfields>/<unnamed-type-bitfields>_u_0/<unnamed-type-bitfields>_u_0_s_3 */

typedef struct <unnamed-type-bitfields>_u_0_s_6 <unnamed-type-bitfields>_u_0_s_6, *P<unnamed-type-bitfields>_u_0_s_6;

typedef union <unnamed-type-bitfields>_u_0_s_6_u_0 <unnamed-type-bitfields>_u_0_s_6_u_0, *P<unnamed-type-bitfields>_u_0_s_6_u_0;

typedef struct <unnamed-type-bitfields>_u_0_s_6_u_0_s_0 <unnamed-type-bitfields>_u_0_s_6_u_0_s_0, *P<unnamed-type-bitfields>_u_0_s_6_u_0_s_0;

struct <unnamed-type-bitfields>_u_0_s_6_u_0_s_0 {
    undefined field0_0x0;
    uint BM_STATE4:2;
};

union <unnamed-type-bitfields>_u_0_s_6_u_0 {
    struct <unnamed-type-bitfields>_u_0_s_6_u_0_s_0 _s_0;
    uint BM_STATE3:2;
};

struct <unnamed-type-bitfields>_u_0_s_6 {
    union <unnamed-type-bitfields>_u_0_s_6_u_0 field0_0x0;
};


/* WARNING! conflicting data type names: /iw5mp_xenon.pdb/MC0_STATUS_1/<unnamed-type-bitfields>/<unnamed-type-bitfields>_u_0/<unnamed-type-bitfields>_u_0_s_1/<unnamed-type-bitfields>_u_0_s_1_u_0 - /iw5mp_xenon.pdb/VGT_GROUP_VECT_1_FMT_CNTL/<unnamed-type-bitfields>/<unnamed-type-bitfields>_u_0/<unnamed-type-bitfields>_u_0_s_1/<unnamed-type-bitfields>_u_0_s_1_u_0 */


/* WARNING! conflicting data type names: /iw5mp_xenon.pdb/MC0_STATUS_1/<unnamed-type-bitfields>/<unnamed-type-bitfields>_u_0/<unnamed-type-bitfields>_u_0_s_1/<unnamed-type-bitfields>_u_0_s_1_u_0/<unnamed-type-bitfields>_u_0_s_1_u_0_s_0 - /iw5mp_xenon.pdb/VGT_GROUP_VECT_1_FMT_CNTL/<unnamed-type-bitfields>/<unnamed-type-bitfields>_u_0/<unnamed-type-bitfields>_u_0_s_1/<unnamed-type-bitfields>_u_0_s_1_u_0/<unnamed-type-bitfields>_u_0_s_1_u_0_s_0 */


/* WARNING! conflicting data type names: /iw5mp_xenon.pdb/MC0_STATUS_1/<unnamed-type-bitfields>/<unnamed-type-bitfields>_u_0/<unnamed-type-bitfields>_u_0_s_4/<unnamed-type-bitfields>_u_0_s_4_u_0 - /iw5mp_xenon.pdb/CP_RT10_COMMAND/<unnamed-type-bitfields>/<unnamed-type-bitfields>_u_0/<unnamed-type-bitfields>_u_0_s_4/<unnamed-type-bitfields>_u_0_s_4_u_0 */


/* WARNING! conflicting data type names: /iw5mp_xenon.pdb/MC0_STATUS_1/<unnamed-type-bitfields>/<unnamed-type-bitfields>_u_0/<unnamed-type-bitfields>_u_0_s_4/<unnamed-type-bitfields>_u_0_s_4_u_0/<unnamed-type-bitfields>_u_0_s_4_u_0_s_0 - /iw5mp_xenon.pdb/CP_RT10_COMMAND/<unnamed-type-bitfields>/<unnamed-type-bitfields>_u_0/<unnamed-type-bitfields>_u_0_s_4/<unnamed-type-bitfields>_u_0_s_4_u_0/<unnamed-type-bitfields>_u_0_s_4_u_0_s_0 */


/* WARNING! conflicting data type names: /iw5mp_xenon.pdb/MC0_STATUS_1/<unnamed-type-bitfields>/<unnamed-type-bitfields>_u_0/<unnamed-type-bitfields>_u_0_s_2/<unnamed-type-bitfields>_u_0_s_2_u_0 - /iw5mp_xenon.pdb/VGT_GROUP_VECT_1_FMT_CNTL/<unnamed-type-bitfields>/<unnamed-type-bitfields>_u_0/<unnamed-type-bitfields>_u_0_s_2/<unnamed-type-bitfields>_u_0_s_2_u_0 */


/* WARNING! conflicting data type names: /iw5mp_xenon.pdb/MC0_STATUS_1/<unnamed-type-bitfields>/<unnamed-type-bitfields>_u_0/<unnamed-type-bitfields>_u_0_s_2/<unnamed-type-bitfields>_u_0_s_2_u_0/<unnamed-type-bitfields>_u_0_s_2_u_0_s_0 - /iw5mp_xenon.pdb/VGT_GROUP_VECT_1_FMT_CNTL/<unnamed-type-bitfields>/<unnamed-type-bitfields>_u_0/<unnamed-type-bitfields>_u_0_s_2/<unnamed-type-bitfields>_u_0_s_2_u_0/<unnamed-type-bitfields>_u_0_s_2_u_0_s_0 */


/* WARNING! conflicting data type names: /iw5mp_xenon.pdb/MC0_STATUS_1/<unnamed-type-bitfields>/<unnamed-type-bitfields>_u_0/<unnamed-type-bitfields>_u_0_s_3/<unnamed-type-bitfields>_u_0_s_3_u_0 - /iw5mp_xenon.pdb/VGT_GROUP_VECT_1_FMT_CNTL/<unnamed-type-bitfields>/<unnamed-type-bitfields>_u_0/<unnamed-type-bitfields>_u_0_s_3/<unnamed-type-bitfields>_u_0_s_3_u_0 */


/* WARNING! conflicting data type names: /iw5mp_xenon.pdb/MC0_STATUS_1/<unnamed-type-bitfields>/<unnamed-type-bitfields>_u_0/<unnamed-type-bitfields>_u_0_s_3/<unnamed-type-bitfields>_u_0_s_3_u_0/<unnamed-type-bitfields>_u_0_s_3_u_0_s_0 - /iw5mp_xenon.pdb/VGT_GROUP_VECT_1_FMT_CNTL/<unnamed-type-bitfields>/<unnamed-type-bitfields>_u_0/<unnamed-type-bitfields>_u_0_s_3/<unnamed-type-bitfields>_u_0_s_3_u_0/<unnamed-type-bitfields>_u_0_s_3_u_0_s_0 */


/* WARNING! conflicting data type names: /iw5mp_xenon.pdb/FSB_TLA_COUNT5_INIT/<unnamed-type-bitfields>/<unnamed-type-bitfields>_u_0 - /iw5mp_xenon.pdb/D1SCL_COEF_RAM_SELECT/<unnamed-type-bitfields>/<unnamed-type-bitfields>_u_0 */


/* WARNING! conflicting data type names: /iw5mp_xenon.pdb/FSB_TLA_COUNT5_INIT/<unnamed-type-bitfields>/<unnamed-type-bitfields>_u_0/<unnamed-type-bitfields>_u_0_s_0 - /iw5mp_xenon.pdb/D1SCL_COEF_RAM_SELECT/<unnamed-type-bitfields>/<unnamed-type-bitfields>_u_0/<unnamed-type-bitfields>_u_0_s_0 */


/* WARNING! conflicting data type names: /iw5mp_xenon.pdb/BC_DUMMY_CRAYRB_ENUMS/<unnamed-type-bitfields>/<unnamed-type-bitfields>_u_0 - /iw5mp_xenon.pdb/D1SCL_COEF_RAM_SELECT/<unnamed-type-bitfields>/<unnamed-type-bitfields>_u_0 */


/* WARNING! conflicting data type names: /iw5mp_xenon.pdb/BC_DUMMY_CRAYRB_ENUMS/<unnamed-type-bitfields>/<unnamed-type-bitfields>_u_0/<unnamed-type-bitfields>_u_0_s_2 - /iw5mp_xenon.pdb/VGT_GROUP_VECT_1_FMT_CNTL/<unnamed-type-bitfields>/<unnamed-type-bitfields>_u_0/<unnamed-type-bitfields>_u_0_s_2 */


/* WARNING! conflicting data type names: /iw5mp_xenon.pdb/BC_DUMMY_CRAYRB_ENUMS/<unnamed-type-bitfields>/<unnamed-type-bitfields>_u_0/<unnamed-type-bitfields>_u_0_s_0 - /iw5mp_xenon.pdb/D1SCL_COEF_RAM_SELECT/<unnamed-type-bitfields>/<unnamed-type-bitfields>_u_0/<unnamed-type-bitfields>_u_0_s_0 */


/* WARNING! conflicting data type names: /iw5mp_xenon.pdb/BC_DUMMY_CRAYRB_ENUMS/<unnamed-type-bitfields>/<unnamed-type-bitfields>_u_0/<unnamed-type-bitfields>_u_0_s_1 - /iw5mp_xenon.pdb/VGT_GROUP_VECT_1_FMT_CNTL/<unnamed-type-bitfields>/<unnamed-type-bitfields>_u_0/<unnamed-type-bitfields>_u_0_s_1 */


/* WARNING! conflicting data type names: /iw5mp_xenon.pdb/BC_DUMMY_CRAYRB_ENUMS/<unnamed-type-bitfields>/<unnamed-type-bitfields>_u_0/<unnamed-type-bitfields>_u_0_s_1/<unnamed-type-bitfields>_u_0_s_1_u_0 - /iw5mp_xenon.pdb/VGT_GROUP_VECT_1_FMT_CNTL/<unnamed-type-bitfields>/<unnamed-type-bitfields>_u_0/<unnamed-type-bitfields>_u_0_s_1/<unnamed-type-bitfields>_u_0_s_1_u_0 */


/* WARNING! conflicting data type names: /iw5mp_xenon.pdb/BC_DUMMY_CRAYRB_ENUMS/<unnamed-type-bitfields>/<unnamed-type-bitfields>_u_0/<unnamed-type-bitfields>_u_0_s_1/<unnamed-type-bitfields>_u_0_s_1_u_0/<unnamed-type-bitfields>_u_0_s_1_u_0_s_0 - /iw5mp_xenon.pdb/VGT_GROUP_VECT_1_FMT_CNTL/<unnamed-type-bitfields>/<unnamed-type-bitfields>_u_0/<unnamed-type-bitfields>_u_0_s_1/<unnamed-type-bitfields>_u_0_s_1_u_0/<unnamed-type-bitfields>_u_0_s_1_u_0_s_0 */


/* WARNING! conflicting data type names: /iw5mp_xenon.pdb/BC_DUMMY_CRAYRB_ENUMS/<unnamed-type-bitfields>/<unnamed-type-bitfields>_u_0/<unnamed-type-bitfields>_u_0_s_2/<unnamed-type-bitfields>_u_0_s_2_u_0 - /iw5mp_xenon.pdb/VGT_GROUP_VECT_1_FMT_CNTL/<unnamed-type-bitfields>/<unnamed-type-bitfields>_u_0/<unnamed-type-bitfields>_u_0_s_2/<unnamed-type-bitfields>_u_0_s_2_u_0 */


/* WARNING! conflicting data type names: /iw5mp_xenon.pdb/BC_DUMMY_CRAYRB_ENUMS/<unnamed-type-bitfields>/<unnamed-type-bitfields>_u_0/<unnamed-type-bitfields>_u_0_s_2/<unnamed-type-bitfields>_u_0_s_2_u_0/<unnamed-type-bitfields>_u_0_s_2_u_0_s_0 - /iw5mp_xenon.pdb/VGT_GROUP_VECT_1_FMT_CNTL/<unnamed-type-bitfields>/<unnamed-type-bitfields>_u_0/<unnamed-type-bitfields>_u_0_s_2/<unnamed-type-bitfields>_u_0_s_2_u_0/<unnamed-type-bitfields>_u_0_s_2_u_0_s_0 */

typedef struct _tiddata _tiddata, *P_tiddata;

typedef struct _tiddata *_ptiddata;

typedef struct threadmbcinfostruct threadmbcinfostruct, *Pthreadmbcinfostruct;

typedef struct threadmbcinfostruct *pthreadmbcinfo;

typedef struct threadlocaleinfostruct threadlocaleinfostruct, *Pthreadlocaleinfostruct;

typedef struct threadlocaleinfostruct *pthreadlocinfo;

typedef struct setloc_struct setloc_struct, *Psetloc_struct;

typedef struct setloc_struct _setloc_struct;

typedef struct localerefcount localerefcount, *Plocalerefcount;

typedef struct localerefcount locrefcount;

typedef struct lconv lconv, *Plconv;

typedef struct __lc_time_data __lc_time_data, *P__lc_time_data;

typedef struct _is_ctype_compatible _is_ctype_compatible, *P_is_ctype_compatible;

struct lconv {
    char *decimal_point;
    char *thousands_sep;
    char *grouping;
    char *int_curr_symbol;
    char *currency_symbol;
    char *mon_decimal_point;
    char *mon_thousands_sep;
    char *mon_grouping;
    char *positive_sign;
    char *negative_sign;
    char int_frac_digits;
    char frac_digits;
    char p_cs_precedes;
    char p_sep_by_space;
    char n_cs_precedes;
    char n_sep_by_space;
    char p_sign_posn;
    char n_sign_posn;
    wchar_t *_W_decimal_point;
    wchar_t *_W_thousands_sep;
    wchar_t *_W_int_curr_symbol;
    wchar_t *_W_currency_symbol;
    wchar_t *_W_mon_decimal_point;
    wchar_t *_W_mon_thousands_sep;
    wchar_t *_W_positive_sign;
    wchar_t *_W_negative_sign;
};

struct _is_ctype_compatible {
    ulong id;
    int is_clike;
};

struct setloc_struct {
    wchar_t *pchLanguage;
    wchar_t *pchCountry;
    int iLocState;
    int iPrimaryLen;
    BOOL bAbbrevLanguage;
    BOOL bAbbrevCountry;
    UINT _cachecp;
    wchar_t _cachein[131];
    wchar_t _cacheout[131];
    struct _is_ctype_compatible _Loc_c[5];
    wchar_t _cacheLocaleName[85];
};

struct _tiddata {
    ulong _tid;
    uintptr_t _thandle;
    int _terrno;
    ulong _tdoserrno;
    uint _fpds;
    ulong _holdrand;
    char *_token;
    wchar_t *_wtoken;
    uchar *_mtoken;
    char *_errmsg;
    wchar_t *_werrmsg;
    char *_namebuf0;
    wchar_t *_wnamebuf0;
    char *_namebuf1;
    wchar_t *_wnamebuf1;
    char *_asctimebuf;
    wchar_t *_wasctimebuf;
    void *_gmtimebuf;
    char *_cvtbuf;
    uchar _con_ch_buf[5];
    ushort _ch_buf_used;
    void *_initaddr;
    void *_initarg;
    void *_pxcptacttab;
    void *_tpxcptinfoptrs;
    int _tfpecode;
    pthreadmbcinfo ptmbcinfo;
    pthreadlocinfo ptlocinfo;
    int _ownlocale;
    ulong _NLG_dwCode;
    void *_terminate;
    void *_unexpected;
    void *_translator;
    void *_purecall;
    void *_curexception;
    void *_curcontext;
    int _ProcessingThrow;
    void *_curexcspec;
    void *_pFrameInfoChain;
    _setloc_struct _setloc_data;
    void *_reserved1;
    void *_reserved2;
    void *_reserved3;
    void *_reserved4;
    void *_reserved5;
    int _cxxReThrow;
    ulong __initDomain;
    int _initapartment;
};

struct __lc_time_data {
    char *wday_abbr[7];
    char *wday[7];
    char *month_abbr[12];
    char *month[12];
    char *ampm[2];
    char *ww_sdatefmt;
    char *ww_ldatefmt;
    char *ww_timefmt;
    int ww_caltype;
    int refcount;
    wchar_t *_W_wday_abbr[7];
    wchar_t *_W_wday[7];
    wchar_t *_W_month_abbr[12];
    wchar_t *_W_month[12];
    wchar_t *_W_ampm[2];
    wchar_t *_W_ww_sdatefmt;
    wchar_t *_W_ww_ldatefmt;
    wchar_t *_W_ww_timefmt;
    wchar_t *_W_ww_locale_name;
};

struct threadmbcinfostruct {
    int refcount;
    int mbcodepage;
    int ismbcodepage;
    ushort mbulinfo[6];
    uchar mbctype[257];
    uchar mbcasemap[256];
    wchar_t *mblocalename;
};

struct localerefcount {
    char *locale;
    wchar_t *wlocale;
    int *refcount;
    int *wrefcount;
};

struct threadlocaleinfostruct {
    int refcount;
    uint lc_codepage;
    uint lc_collate_cp;
    uint lc_time_cp;
    locrefcount lc_category[6];
    int lc_clike;
    int mb_cur_max;
    int *lconv_intl_refcount;
    int *lconv_num_refcount;
    int *lconv_mon_refcount;
    struct lconv *lconv;
    int *ctype1_refcount;
    ushort *ctype1;
    ushort *pctype;
    uchar *pclmap;
    uchar *pcumap;
    struct __lc_time_data *lc_time_curr;
    wchar_t *locale_name[6];
};

typedef ulong u_long;

typedef struct WSAData WSAData, *PWSAData;

typedef struct WSAData WSADATA;

struct WSAData {
    WORD wVersion;
    WORD wHighVersion;
    char szDescription[257];
    char szSystemStatus[129];
    ushort iMaxSockets;
    ushort iMaxUdpDg;
    char *lpVendorInfo;
};

typedef WSADATA *LPWSADATA;

typedef UINT_PTR SOCKET;

typedef ushort u_short;


/* WARNING! conflicting data type names: /winsock.h/sockaddr - /iw5mp_xenon.pdb/sockaddr */

typedef struct fd_set fd_set, *Pfd_set;

typedef uint u_int;

struct fd_set {
    u_int fd_count;
    SOCKET fd_array[64];
};

typedef struct timeval timeval, *Ptimeval;

struct timeval {
    long tv_sec;
    long tv_usec;
};

typedef struct SNDCHANNEL SNDCHANNEL, *PSNDCHANNEL;

struct SNDCHANNEL { /* PlaceHolder Structure */
};

typedef struct BINK BINK, *PBINK;

struct BINK { /* PlaceHolder Structure */
};

typedef struct BINKSND BINKSND, *PBINKSND;

struct BINKSND { /* PlaceHolder Structure */
};

typedef struct _LDBL12 _LDBL12, *P_LDBL12;

struct _LDBL12 {
    uchar ld12[12];
};

typedef struct _CRT_FLOAT _CRT_FLOAT, *P_CRT_FLOAT;

struct _CRT_FLOAT {
    float f;
};

typedef struct _CRT_DOUBLE _CRT_DOUBLE, *P_CRT_DOUBLE;

struct _CRT_DOUBLE {
    double x;
};

typedef int (*_onexit_t)(void);

typedef longlong __time64_t;

typedef uint size_t;

typedef size_t rsize_t;

typedef int errno_t;

typedef struct localeinfo_struct localeinfo_struct, *Plocaleinfo_struct;

struct localeinfo_struct {
    pthreadlocinfo locinfo;
    pthreadmbcinfo mbcinfo;
};

typedef int intptr_t;

typedef struct localeinfo_struct *_locale_t;

