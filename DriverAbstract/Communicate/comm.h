#pragma once
#include "commStruct.h"

typedef NTSTATUS(NTAPI* CommCallback)(PCommPackage package);

NTSTATUS RegistCommunicate(PDRIVER_OBJECT DriverObject, CommCallback callBack);

VOID DestoryCommunicate(PDRIVER_OBJECT DriverObject);
