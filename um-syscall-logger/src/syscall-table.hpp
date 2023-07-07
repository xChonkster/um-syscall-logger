/*
*	syscall_table.hpp
* 
*	static table of syscalls
*/

#pragma once

const char* syscall_id_name_table[] =
{
    "NtAccessCheck",
    "NtWorkerFactoryWorkerReady",
    "NtAcceptConnectPort",
    "NtMapUserPhysicalPagesScatter",
    "NtWaitForSingleObject",
    "NtCallbackReturn",
    "NtReadFile",
    "NtDeviceIoControlFile",
    "NtWriteFile",
    "NtRemoveIoCompletion",
    "NtReleaseSemaphore",
    "NtReplyWaitReceivePort",
    "NtReplyPort",
    "NtSetInformationThread",
    "NtSetEvent",
    "NtClose",
    "NtQueryObject",
    "NtQueryInformationFile",
    "NtOpenKey",
    "NtEnumerateValueKey",
    "NtFindAtom",
    "NtQueryDefaultLocale",
    "NtQueryKey",
    "NtQueryValueKey",
    "NtAllocateVirtualMemory",
    "NtQueryInformationProcess",
    "NtWaitForMultipleObjects32",
    "NtWriteFileGather",
    "NtSetInformationProcess",
    "NtCreateKey",
    "NtFreeVirtualMemory",
    "NtImpersonateClientOfPort",
    "NtReleaseMutant",
    "NtQueryInformationToken",
    "NtRequestWaitReplyPort",
    "NtQueryVirtualMemory",
    "NtOpenThreadToken",
    "NtQueryInformationThread",
    "NtOpenProcess",
    "NtSetInformationFile",
    "NtMapViewOfSection",
    "NtAccessCheckAndAuditAlarm",
    "NtUnmapViewOfSection",
    "NtReplyWaitReceivePortEx",
    "NtTerminateProcess",
    "NtSetEventBoostPriority",
    "NtReadFileScatter",
    "NtOpenThreadTokenEx",
    "NtOpenProcessTokenEx",
    "NtQueryPerformanceCounter",
    "NtEnumerateKey",
    "NtOpenFile",
    "NtDelayExecution",
    "NtQueryDirectoryFile",
    "NtQuerySystemInformation",
    "NtOpenSection",
    "NtQueryTimer",
    "NtFsControlFile",
    "NtWriteVirtualMemory",
    "NtCloseObjectAuditAlarm",
    "NtDuplicateObject",
    "NtQueryAttributesFile",
    "NtClearEvent",
    "NtReadVirtualMemory",
    "NtOpenEvent",
    "NtAdjustPrivilegesToken",
    "NtDuplicateToken",
    "NtContinue",
    "NtQueryDefaultUILanguage",
    "NtQueueApcThread",
    "NtYieldExecution",
    "NtAddAtom",
    "NtCreateEvent",
    "NtQueryVolumeInformationFile",
    "NtCreateSection",
    "NtFlushBuffersFile",
    "NtApphelpCacheControl",
    "NtCreateProcessEx",
    "NtCreateThread",
    "NtIsProcessInJob",
    "NtProtectVirtualMemory",
    "NtQuerySection",
    "NtResumeThread",
    "NtTerminateThread",
    "NtReadRequestData",
    "NtCreateFile",
    "NtQueryEvent",
    "NtWriteRequestData",
    "NtOpenDirectoryObject",
    "NtAccessCheckByTypeAndAuditAlarm",
    "NtQuerySystemTime",
    "NtWaitForMultipleObjects",
    "NtSetInformationObject",
    "NtCancelIoFile",
    "NtTraceEvent",
    "NtPowerInformation",
    "NtSetValueKey",
    "NtCancelTimer",
    "NtSetTimer",
    "NtAccessCheckByType",
    "NtAccessCheckByTypeResultList",
    "NtAccessCheckByTypeResultListAndAuditAlarm",
    "NtAccessCheckByTypeResultListAndAuditAlarmByHandle",
    "NtAcquireCrossVmMutant",
    "NtAcquireProcessActivityReference",
    "NtAddAtomEx",
    "NtAddBootEntry",
    "NtAddDriverEntry",
    "NtAdjustGroupsToken",
    "NtAdjustTokenClaimsAndDeviceGroups",
    "NtAlertResumeThread",
    "NtAlertThread",
    "NtAlertThreadByThreadId",
    "NtAllocateLocallyUniqueId",
    "NtAllocateReserveObject",
    "NtAllocateUserPhysicalPages",
    "NtAllocateUserPhysicalPagesEx",
    "NtAllocateUuids",
    "NtAllocateVirtualMemoryEx",
    "NtAlpcAcceptConnectPort",
    "NtAlpcCancelMessage",
    "NtAlpcConnectPort",
    "NtAlpcConnectPortEx",
    "NtAlpcCreatePort",
    "NtAlpcCreatePortSection",
    "NtAlpcCreateResourceReserve",
    "NtAlpcCreateSectionView",
    "NtAlpcCreateSecurityContext",
    "NtAlpcDeletePortSection",
    "NtAlpcDeleteResourceReserve",
    "NtAlpcDeleteSectionView",
    "NtAlpcDeleteSecurityContext",
    "NtAlpcDisconnectPort",
    "NtAlpcImpersonateClientContainerOfPort",
    "NtAlpcImpersonateClientOfPort",
    "NtAlpcOpenSenderProcess",
    "NtAlpcOpenSenderThread",
    "NtAlpcQueryInformation",
    "NtAlpcQueryInformationMessage",
    "NtAlpcRevokeSecurityContext",
    "NtAlpcSendWaitReceivePort",
    "NtAlpcSetInformation",
    "NtAreMappedFilesTheSame",
    "NtAssignProcessToJobObject",
    "NtAssociateWaitCompletionPacket",
    "NtCallEnclave",
    "NtCancelIoFileEx",
    "NtCancelSynchronousIoFile",
    "NtCancelTimer2",
    "NtCancelWaitCompletionPacket",
    "NtCommitComplete",
    "NtCommitEnlistment",
    "NtCommitRegistryTransaction",
    "NtCommitTransaction",
    "NtCompactKeys",
    "NtCompareObjects",
    "NtCompareSigningLevels",
    "NtCompareTokens",
    "NtCompleteConnectPort",
    "NtCompressKey",
    "NtConnectPort",
    "NtContinueEx",
    "NtConvertBetweenAuxiliaryCounterAndPerformanceCounter",
    "NtCreateCrossVmEvent",
    "NtCreateCrossVmMutant",
    "NtCreateDebugObject",
    "NtCreateDirectoryObject",
    "NtCreateDirectoryObjectEx",
    "NtCreateEnclave",
    "NtCreateEnlistment",
    "NtCreateEventPair",
    "NtCreateIRTimer",
    "NtCreateIoCompletion",
    "NtCreateJobObject",
    "NtCreateJobSet",
    "NtCreateKeyTransacted",
    "NtCreateKeyedEvent",
    "NtCreateLowBoxToken",
    "NtCreateMailslotFile",
    "NtCreateMutant",
    "NtCreateNamedPipeFile",
    "NtCreatePagingFile",
    "NtCreatePartition",
    "NtCreatePort",
    "NtCreatePrivateNamespace",
    "NtCreateProcess",
    "NtCreateProfile",
    "NtCreateProfileEx",
    "NtCreateRegistryTransaction",
    "NtCreateResourceManager",
    "NtCreateSectionEx",
    "NtCreateSemaphore",
    "NtCreateSymbolicLinkObject",
    "NtCreateThreadEx",
    "NtCreateTimer",
    "NtCreateTimer2",
    "NtCreateToken",
    "NtCreateTokenEx",
    "NtCreateTransaction",
    "NtCreateTransactionManager",
    "NtCreateUserProcess",
    "NtCreateWaitCompletionPacket",
    "NtCreateWaitablePort",
    "NtCreateWnfStateName",
    "NtCreateWorkerFactory",
    "NtDebugActiveProcess",
    "NtDebugContinue",
    "NtDeleteAtom",
    "NtDeleteBootEntry",
    "NtDeleteDriverEntry",
    "NtDeleteFile",
    "NtDeleteKey",
    "NtDeleteObjectAuditAlarm",
    "NtDeletePrivateNamespace",
    "NtDeleteValueKey",
    "NtDeleteWnfStateData",
    "NtDeleteWnfStateName",
    "NtDirectGraphicsCall",
    "NtDisableLastKnownGood",
    "NtDisplayString",
    "NtDrawText",
    "NtEnableLastKnownGood",
    "NtEnumerateBootEntries",
    "NtEnumerateDriverEntries",
    "NtEnumerateSystemEnvironmentValuesEx",
    "NtEnumerateTransactionObject",
    "NtExtendSection",
    "NtFilterBootOption",
    "NtFilterToken",
    "NtFilterTokenEx",
    "NtFlushBuffersFileEx",
    "NtFlushInstallUILanguage",
    "NtFlushInstructionCache",
    "NtFlushKey",
    "NtFlushProcessWriteBuffers",
    "NtFlushVirtualMemory",
    "NtFlushWriteBuffer",
    "NtFreeUserPhysicalPages",
    "NtFreezeRegistry",
    "NtFreezeTransactions",
    "NtGetCachedSigningLevel",
    "NtGetCompleteWnfStateSubscription",
    "NtGetContextThread",
    "NtGetCurrentProcessorNumber",
    "NtGetCurrentProcessorNumberEx",
    "NtGetDevicePowerState",
    "NtGetMUIRegistryInfo",
    "NtGetNextProcess",
    "NtGetNextThread",
    "NtGetNlsSectionPtr",
    "NtGetNotificationResourceManager",
    "NtGetWriteWatch",
    "NtImpersonateAnonymousToken",
    "NtImpersonateThread",
    "NtInitializeEnclave",
    "NtInitializeNlsFiles",
    "NtInitializeRegistry",
    "NtInitiatePowerAction",
    "NtIsSystemResumeAutomatic",
    "NtIsUILanguageComitted",
    "NtListenPort",
    "NtLoadDriver",
    "NtLoadEnclaveData",
    "NtLoadKey",
    "NtLoadKey2",
    "NtLoadKeyEx",
    "NtLockFile",
    "NtLockProductActivationKeys",
    "NtLockRegistryKey",
    "NtLockVirtualMemory",
    "NtMakePermanentObject",
    "NtMakeTemporaryObject",
    "NtManageHotPatch",
    "NtManagePartition",
    "NtMapCMFModule",
    "NtMapUserPhysicalPages",
    "NtMapViewOfSectionEx",
    "NtModifyBootEntry",
    "NtModifyDriverEntry",
    "NtNotifyChangeDirectoryFile",
    "NtNotifyChangeDirectoryFileEx",
    "NtNotifyChangeKey",
    "NtNotifyChangeMultipleKeys",
    "NtNotifyChangeSession",
    "NtOpenEnlistment",
    "NtOpenEventPair",
    "NtOpenIoCompletion",
    "NtOpenJobObject",
    "NtOpenKeyEx",
    "NtOpenKeyTransacted",
    "NtOpenKeyTransactedEx",
    "NtOpenKeyedEvent",
    "NtOpenMutant",
    "NtOpenObjectAuditAlarm",
    "NtOpenPartition",
    "NtOpenPrivateNamespace",
    "NtOpenProcessToken",
    "NtOpenRegistryTransaction",
    "NtOpenResourceManager",
    "NtOpenSemaphore",
    "NtOpenSession",
    "NtOpenSymbolicLinkObject",
    "NtOpenThread",
    "NtOpenTimer",
    "NtOpenTransaction",
    "NtOpenTransactionManager",
    "NtPlugPlayControl",
    "NtPrePrepareComplete",
    "NtPrePrepareEnlistment",
    "NtPrepareComplete",
    "NtPrepareEnlistment",
    "NtPrivilegeCheck",
    "NtPrivilegeObjectAuditAlarm",
    "NtPrivilegedServiceAuditAlarm",
    "NtPropagationComplete",
    "NtPropagationFailed",
    "NtPssCaptureVaSpaceBulk",
    "NtPulseEvent",
    "NtQueryAuxiliaryCounterFrequency",
    "NtQueryBootEntryOrder",
    "NtQueryBootOptions",
    "NtQueryDebugFilterState",
    "NtQueryDirectoryFileEx",
    "NtQueryDirectoryObject",
    "NtQueryDriverEntryOrder",
    "NtQueryEaFile",
    "NtQueryFullAttributesFile",
    "NtQueryInformationAtom",
    "NtQueryInformationByName",
    "NtQueryInformationEnlistment",
    "NtQueryInformationJobObject",
    "NtQueryInformationPort",
    "NtQueryInformationResourceManager",
    "NtQueryInformationTransaction",
    "NtQueryInformationTransactionManager",
    "NtQueryInformationWorkerFactory",
    "NtQueryInstallUILanguage",
    "NtQueryIntervalProfile",
    "NtQueryIoCompletion",
    "NtQueryLicenseValue",
    "NtQueryMultipleValueKey",
    "NtQueryMutant",
    "NtQueryOpenSubKeys",
    "NtQueryOpenSubKeysEx",
    "NtQueryPortInformationProcess",
    "NtQueryQuotaInformationFile",
    "NtQuerySecurityAttributesToken",
    "NtQuerySecurityObject",
    "NtQuerySecurityPolicy",
    "NtQuerySemaphore",
    "NtQuerySymbolicLinkObject",
    "NtQuerySystemEnvironmentValue",
    "NtQuerySystemEnvironmentValueEx",
    "NtQuerySystemInformationEx",
    "NtQueryTimerResolution",
    "NtQueryWnfStateData",
    "NtQueryWnfStateNameInformation",
    "NtQueueApcThreadEx",
    "NtRaiseException",
    "NtRaiseHardError",
    "NtReadOnlyEnlistment",
    "NtRecoverEnlistment",
    "NtRecoverResourceManager",
    "NtRecoverTransactionManager",
    "NtRegisterProtocolAddressInformation",
    "NtRegisterThreadTerminatePort",
    "NtReleaseKeyedEvent",
    "NtReleaseWorkerFactoryWorker",
    "NtRemoveIoCompletionEx",
    "NtRemoveProcessDebug",
    "NtRenameKey",
    "NtRenameTransactionManager",
    "NtReplaceKey",
    "NtReplacePartitionUnit",
    "NtReplyWaitReplyPort",
    "NtRequestPort",
    "NtResetEvent",
    "NtResetWriteWatch",
    "NtRestoreKey",
    "NtResumeProcess",
    "NtRevertContainerImpersonation",
    "NtRollbackComplete",
    "NtRollbackEnlistment",
    "NtRollbackRegistryTransaction",
    "NtRollbackTransaction",
    "NtRollforwardTransactionManager",
    "NtSaveKey",
    "NtSaveKeyEx",
    "NtSaveMergedKeys",
    "NtSecureConnectPort",
    "NtSerializeBoot",
    "NtSetBootEntryOrder",
    "NtSetBootOptions",
    "NtSetCachedSigningLevel",
    "NtSetCachedSigningLevel2",
    "NtSetContextThread",
    "NtSetDebugFilterState",
    "NtSetDefaultHardErrorPort",
    "NtSetDefaultLocale",
    "NtSetDefaultUILanguage",
    "NtSetDriverEntryOrder",
    "NtSetEaFile",
    "NtSetHighEventPair",
    "NtSetHighWaitLowEventPair",
    "NtSetIRTimer",
    "NtSetInformationDebugObject",
    "NtSetInformationEnlistment",
    "NtSetInformationJobObject",
    "NtSetInformationKey",
    "NtSetInformationResourceManager",
    "NtSetInformationSymbolicLink",
    "NtSetInformationToken",
    "NtSetInformationTransaction",
    "NtSetInformationTransactionManager",
    "NtSetInformationVirtualMemory",
    "NtSetInformationWorkerFactory",
    "NtSetIntervalProfile",
    "NtSetIoCompletion",
    "NtSetIoCompletionEx",
    "NtSetLdtEntries",
    "NtSetLowEventPair",
    "NtSetLowWaitHighEventPair",
    "NtSetQuotaInformationFile",
    "NtSetSecurityObject",
    "NtSetSystemEnvironmentValue",
    "NtSetSystemEnvironmentValueEx",
    "NtSetSystemInformation",
    "NtSetSystemPowerState",
    "NtSetSystemTime",
    "NtSetThreadExecutionState",
    "NtSetTimer2",
    "NtSetTimerEx",
    "NtSetTimerResolution",
    "NtSetUuidSeed",
    "NtSetVolumeInformationFile",
    "NtSetWnfProcessNotificationEvent",
    "NtShutdownSystem",
    "NtShutdownWorkerFactory",
    "NtSignalAndWaitForSingleObject",
    "NtSinglePhaseReject",
    "NtStartProfile",
    "NtStopProfile",
    "NtSubscribeWnfStateChange",
    "NtSuspendProcess",
    "NtSuspendThread",
    "NtSystemDebugControl",
    "NtTerminateEnclave",
    "NtTerminateJobObject",
    "NtTestAlert",
    "NtThawRegistry",
    "NtThawTransactions",
    "NtTraceControl",
    "NtTranslateFilePath",
    "NtUmsThreadYield",
    "NtUnloadDriver",
    "NtUnloadKey",
    "NtUnloadKey2",
    "NtUnloadKeyEx",
    "NtUnlockFile",
    "NtUnlockVirtualMemory",
    "NtUnmapViewOfSectionEx",
    "NtUnsubscribeWnfStateChange",
    "NtUpdateWnfStateData",
    "NtVdmControl",
    "NtWaitForAlertByThreadId",
    "NtWaitForDebugEvent",
    "NtWaitForKeyedEvent",
    "NtWaitForWorkViaWorkerFactory",
    "NtWaitHighEventPair",
    "NtWaitLowEventPair",
    "NtLoadKey3",
}; // array is sorted

const size_t syscall_id_name_table_size = sizeof( syscall_id_name_table );

