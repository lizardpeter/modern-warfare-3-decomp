#pragma once
#include "enhanced/gdk.h"
HRESULT LocalQueueCreate(GdkInterface *, u32, u32, XTaskQueueHandle *);
HRESULT LocalQueueComposite(GdkInterface *, XTaskQueuePortHandle, XTaskQueuePortHandle, XTaskQueueHandle *);
HRESULT LocalQueuePort(GdkInterface *, XTaskQueueHandle, u32, XTaskQueuePortHandle *);
HRESULT LocalQueueDuplicate(GdkInterface *, XTaskQueueHandle, XTaskQueueHandle *);
bool LocalQueueDispatch(GdkInterface *, XTaskQueueHandle, u32, u32);
void LocalQueueClose(GdkInterface *, XTaskQueueHandle);
HRESULT LocalQueueSubmit(GdkInterface *, XTaskQueueHandle, u32, void *, XTaskQueueCallback *);
HRESULT LocalQueueDelay(GdkInterface *, XTaskQueueHandle, u32, u32, void *, XTaskQueueCallback *);
HRESULT LocalQueueTerminate(GdkInterface *, XTaskQueueHandle, bool, void *, XTaskQueueTerminatedCallback *);
bool LocalQueueCurrent(GdkInterface *, XTaskQueueHandle *);
void LocalQueueSetCurrent(GdkInterface *, XTaskQueueHandle);
