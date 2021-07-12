#include "Communicate/comm.h"
#include "Communicate/commStruct.h"

NTSTATUS NTAPI Dispatch(PCommPackage package)
{
	NTSTATUS status = STATUS_UNSUCCESSFUL;
	switch (package->code)
	{
		case TEST:
		{
			//DbgBreakPoint();
			PTest OnePackage = (PTest)package->inData;
			KdPrintEx((77,0,"%s\r\n", OnePackage->myStr));
			*((OnePackage->myStr) + 6) = 'B';
			*((OnePackage->myStr) + 7) = 'o';
			*((OnePackage->myStr) + 8) = 'b';
			*((OnePackage->myStr) + 9) = '!';
			*((OnePackage->myStr) + 10) = '!';
			status = STATUS_SUCCESS;
		}
		break;
	}
	return status;
}


VOID DrivUnload(PDRIVER_OBJECT DriverObject)
{
	DestoryCommunicate(DriverObject);
	KdPrintEx((77, 0, "[+]: Driver Unloaded!!!\r\n"));
}


NTSTATUS DriverEntry(PDRIVER_OBJECT pDriver, PUNICODE_STRING pReg)
{
	//DbgBreakPoint();

	NTSTATUS status = RegistCommunicate(pDriver, Dispatch);
	if (!NT_SUCCESS(status)) 
	{
		KdPrintEx((77, 0, "[+]: Driver Failed!!!\r\n"));
		return status;
	}
	pDriver->DriverUnload = DrivUnload;
	KdPrintEx((77, 0, "[+]: Driver Loaded!!!\r\n"));
	return status;
}