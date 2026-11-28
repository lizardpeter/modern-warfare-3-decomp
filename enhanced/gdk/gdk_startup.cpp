#include "enhanced/gdk.h"

// Store engine task groups live in the executable and are also used at shutdown.
struct TaskGroup;
extern TaskGroup g_gdkTasks;
extern TaskGroup g_gdkGameSaveTasks;
void TaskGroup_Init(TaskGroup *group, const char *name);
void Gdk_InitUserTasks();
void Gdk_InitPartyTasks();
void Gdk_InitStoreTasks();

extern "C" HRESULT InitializeApiImpl(u64 version, u64 sdk);

// The original makes Xbox Live SDK initialization a prerequisite for the engine's
// startup. The enhanced build only needs local bootstrap and task-group bookkeeping.
bool Gdk_InitEngineTasks()
{
	if (InitializeApiImpl(0, 0) < 0)
		return false;

	TaskGroup_Init(&g_gdkTasks, "GDK tasks.");
	TaskGroup_Init(&g_gdkGameSaveTasks, "GDK gamesave tasks.");
	Gdk_InitUserTasks();
	Gdk_InitPartyTasks();
	Gdk_InitStoreTasks();
	return true;
}
