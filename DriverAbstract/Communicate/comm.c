#include "comm.h"

#define DEVICE_NAME L"\\Device\\Airshelf"

CommCallback gCommCallback = NULL;

NTSTATUS DefaultDispatch(PDEVICE_OBJECT DeviceObject, PIRP Irp)
{
	Irp->IoStatus.Status = STATUS_SUCCESS;
	IoCompleteRequest(Irp, 0);
	return STATUS_SUCCESS;
}


NTSTATUS WriteDispatch(PDEVICE_OBJECT DeviceObject, PIRP Irp)
{
	PIO_STACK_LOCATION ioStack = IoGetCurrentIrpStackLocation(Irp);
	NTSTATUS status = STATUS_UNSUCCESSFUL;
	int Length = ioStack->Parameters.Write.Length;

	if(Length == sizeof(CommPackage) && gCommCallback)
	{
		PCommPackage package = Irp->AssociatedIrp.SystemBuffer;
		if (package->id == COMM_ID && MmIsAddressValid(package))
			status = gCommCallback(package);
	}
	Irp->IoStatus.Information = 0;
	Irp->IoStatus.Status = status;
	IoCompleteRequest(Irp, 0);
	return STATUS_SUCCESS;
}

NTSTATUS RegistCommunicate(PDRIVER_OBJECT DriverObject, CommCallback callBack)
{
	UNICODE_STRING uName = { 0 };
	RtlInitUnicodeString(&uName, DEVICE_NAME);
	PDEVICE_OBJECT pDevice = NULL;
	NTSTATUS status = IoCreateDevice(DriverObject, NULL, &uName, FILE_DEVICE_UNKNOWN, FILE_DEVICE_SECURE_OPEN, FALSE, &pDevice);
	if (!NT_SUCCESS(status))
	{
		KdPrintEx((77, 0, "[x]:IoCreateDevice Fail %x!!!\r\n", status));
		return status;
	}

	UNICODE_STRING symName = { 0 };
	RtlInitUnicodeString(&symName, SYM_NAME);

	status = IoCreateSymbolicLink(&symName, &uName);
	if (!NT_SUCCESS(status))
	{
		KdPrintEx((77, 0, "[x]:IoCreateSymbolicLink Fail %x!!!\r\n", status));
		return status;
	}

	pDevice->Flags &= ~DO_DEVICE_INITIALIZING;
	pDevice->Flags |= DO_BUFFERED_IO;

	DriverObject->MajorFunction[IRP_MJ_CREATE] = DefaultDispatch;
	DriverObject->MajorFunction[IRP_MJ_CLOSE] = DefaultDispatch;
	DriverObject->MajorFunction[IRP_MJ_WRITE] = WriteDispatch;

	if(NT_SUCCESS(status))
	{
		gCommCallback = callBack;
	}
	KdPrintEx((77, 0, "[+]: RegistCommunicate Successed !!!\r\n"));
	return status;
}

VOID DestoryCommunicate(PDRIVER_OBJECT DriverObject)
{
	UNICODE_STRING symName = { 0 };
	RtlInitUnicodeString(&symName, SYM_NAME);
	IoDeleteSymbolicLink(&symName);
	if (DriverObject->DeviceObject) IoDeleteDevice(DriverObject->DeviceObject);
}
