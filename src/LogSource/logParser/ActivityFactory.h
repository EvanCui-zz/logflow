///////////////////////////////////////////////////////////////////
//
// Copyright (c) Microsoft Corporation.
//
// SYNOPSIS
//
//  ActivityFactory - class in charge of generating ActivityIds
//
///////////////////////////////////////////////////////////////////

#pragma once

//
// The activity ID is a 16-byte number that has a specific structure. The structure ensures relative
// uniqueness of the activity IDs while also carrying additional information.
//
#pragma pack(push, 1)
typedef struct _LOGGER_ACTIVITY_ID
{
    UINT32 SequenceNumber;      // Monotonically increasing sequence number
    UINT16 SourceId;            // Number that uniquely identifies the activity source (XFE, TS, EN, etc.)
    UINT16 InstanceId;          // Instance number that created the activity (10 if source was ENPN_IN_10)
    BYTE Discriminator;         // 0-128 number that is used for selective logging.
    UINT16 Time;                // Bits 32-47 of CsTimeStamp at time of creation:
                                //   Bit 32: 7.16 minutes; Bit 47: 162.9 days
                                //   Wrapping interval is 325.8 days
    UINT16 TenantId;            // CRC of tenant name
    BYTE ChildSourceId;         // Child source identifier or zero if root activity (just low byte)
    UINT16 ChildSequenceNumber; // Child activity sequence or zero if root activity

} LOGGER_ACTIVITY_ID;
#pragma pack(pop)

C_ASSERT(sizeof(LOGGER_ACTIVITY_ID) == sizeof(GUID));

class ActivityFactory
{
public:

    ActivityFactory(
        __in INT16 sourceId,
        __in INT16 instanceId)
    {
        m_SourceId = sourceId;
        m_InstanceId = instanceId;
        m_SequenceNumber = 0;
        m_ChildActivitySequenceNumber = 0;
        m_Discriminator = 0;
        m_TenantId = 0;
        m_TenantName = "";
    }

public:

    void
    Initialize();

    void
    CreateActivityId(
        __in bool logDiscriminator,
        __out GUID * pActivityId);

    void
    CreateActivityId(
        __in GUID * parentActivityId,
        __out GUID * pActivityId);

    void
    SetSourceId(
        __in INT16 sourceId)
    {
        m_SourceId = sourceId;
    }

    INT16
    GetSourceId()
    {
        return m_SourceId;
    }

    void
    SetInstanceId(
        __in INT16 instanceId)
    {
        m_InstanceId = instanceId;
        
        UpdateTenantId();
    }

    INT16
    GetInstanceId()
    {
        return m_InstanceId;
    }
    
    void
    SetTenantName(
        __in PCSTR tenantName)
    {
        m_TenantName = tenantName;
        
        UpdateTenantId();
    }
    
    PCSTR
    GetTenantName()
    {
        return m_TenantName;
    }
    
    static void
    ParseActivityId(
        __in  GUID*  activityId,
        __out INT16* sourceId,
        __out INT16* instanceId,
        __out FILETIME* startTime,
        __out FILETIME* endTime,
        __out INT16* tenantId
        );

    static void
    ConvertSourceId(
        __in  INT16  sourceId,
        __out char * source,
        __in  int    size,
        __in_opt const char * configIniPath
        );

    static void
    ConvertTenantId(
        __in  INT16  tenantId,
        __in  INT16  instanceId,
        __out char * tenant,
        __in  int    size,
        __in_opt const char * configIniPath
        );
        
    static INT16 GetTenantCrc(const char* tenantName, INT16 instanceId);
        
    static void
    FiletimeFromActivityId(LOGGER_ACTIVITY_ID activityId,
                           FILETIME* startTime,
                           FILETIME* endTime);

private:

    void CreateActivityId(
        __in BYTE discriminator,
        __out GUID * pActivityId);

    UINT32 GetRandomUInt32();
    
    void UpdateTenantId();

    static void ConvertSourceIdRoleConfig(
        __in  INT16  sourceId,
        __out char * source,
        __in  int    size
        );

    static void ConvertSourceIdFromFile(
        __in  INT16  sourceId,
        __out char * source,
        __in  int    size,
        __in_opt const char * configIniPath
        );

    static void
    ConvertTenantIdRoleConfig(
        __in  INT16  tenantId,
        __in  INT16  instanceId,
        __out char * tenant,
        __in  int    size
        );

    static void
    ConvertTenantIdFromFile(
        __in  INT16  tenantId,
        __in  INT16  instanceId,
        __out char * tenant,
        __in  int    size,
        __in_opt const char * configIniPath
        );

private:

    // Activity Id fields
    INT16                   m_SourceId;
    INT16                   m_InstanceId;
    volatile ULONG          m_SequenceNumber;
    volatile ULONG          m_ChildActivitySequenceNumber;
    volatile LONG           m_Discriminator;
    INT16                   m_TenantId;
    PCSTR                   m_TenantName;
};

