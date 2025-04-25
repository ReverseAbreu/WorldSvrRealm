#pragma once
#include <iostream>
#include <fstream>
#include <functional>
#include <dlfcn.h>
#include <cstring>
#include <map>
#include <unistd.h>
#include <sys/mman.h>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstddef>
#include <memory>
#include <sstream>
#include <fstream>

//common
#include "Common/typedefs.h"
#include "Common/Singleton.h"
#include "Common/XorString.h"

//Memory
#include "Memory/Memory.h"

//Packets
#include "PacketManager/Protodefs.h"
#include "PacketManager/C2SPackets.h"
#include "PacketManager/S2CPackets.h"
#include "PacketManager/PacketManager.h"

//Macros
#define INIT(classe)						classe::GetInstance()->Init() 
#define Instance(classe)					classe::GetInstance()  