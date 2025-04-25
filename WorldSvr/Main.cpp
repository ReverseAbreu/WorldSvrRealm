#include <iostream>
#include <Core.h>
#include <Proc/Proc.h>
#include "Memory/Memory.h"
#include "Game/MacroBM3/MacroBM3.h"
#include "Game/EventAlert/EventAlert.h"
#include "Game/Warp/Warp.h"
#include "Game/WindowList/WindowList.h"

void LoadConfigs()
{
    INIT(Proc);
    INIT(Warp);
    EventAlert::Initialize();
    MacroBM3::Initialize();
    WindowList::Initialize();
}

extern "C" void Init()
{
    LoadConfigs();
}