#pragma once
#include "../../DriverAbstract/Communicate/commStruct.h"

BOOLEAN DriverInit();
BOOLEAN DriverComm(ULONG code, PVOID inData, ULONG inSize, PVOID outData, ULONG outSize);
