#pragma once

// for offsetof macro
#include <stddef.h>

#include "shared.h"

// Max # of people who may subscribe to any particular file
const int c_changeNotifyMaxCallbacks = 20;
 
typedef void *FileChangeNotifyCookie;
typedef void (*FileChangeNotifyFunc)(char *filename, FileChangeNotifyCookie cookie);
class ChangeNotifyEntry;
class ChangeNotifier;


// A callback function and its associated cookie
struct ChangeNotifyCallbackInfo
{
    FileChangeNotifyFunc    m_callback;
    void *                  m_cookie;
};

// One of these exists for each file we are interested in
// It contains a list of callback functions for all people interested in changes to this file
class ChangeNotifyEntry
{
public:
    ChangeNotifyEntry();

    // Add a notify function for when this file changes
    BOOL AddNotify(FileChangeNotifyFunc func, FileChangeNotifyCookie cookie);

    // Remove a notification for when the file changes
    BOOL RemoveNotify(FileChangeNotifyFunc func, FileChangeNotifyCookie cookie);

    // Call all callback functions for this file
    void DoAllCallbacks();

    // File we're interested in
    char                    m_filename[MAX_PATH];
    ChangeNotifyEntry *     m_next;

    // The date of the file when we made the last callback for it
    FILETIME                m_fileDateLastCallback;

    // When we last scanned this file for changes.  This is a GetTickCount() value
    DWORD                   m_lastScanTime;

    // This is how often we should scan this file
    DWORD                   m_scanIntervalMillisecs;

    // List of callbacks to call when this file changes
    ChangeNotifyCallbackInfo m_callbacks[c_changeNotifyMaxCallbacks];
};


// There are two pieces to this class; the static part, which contains a critical section,
// a linked list of files of interest, and a few other things.  The non-static part is
// instance data; each time a file is subscribed to in a directory that hasn't been 
// seen before, a new instance is launched to listen for directory changes on that directory.
class ChangeNotifier
{
private:
    // Protects the list (m_firstEntry)
    static CRITSEC* s_criticalSection;
    static int s_initCount;

    // Linked list of files of interest
    // This is protected by critical section
    static ChangeNotifyEntry *     m_firstEntry;

private:
    static unsigned int __stdcall MainLoop(void*);
    static BOOL SpinThread();

public:
    static void ScanFiles();
    static void ScanFile(const char* filename);
    static BOOL GetFileLastWriteDate(char *filename, FILETIME *date);

public:
    // Static initializer
    static BOOL Init();
    static void UnInit(bool force = false);

    // used to pause scaning
    static void Pause();
    static void Resume();

    // Callable on any thread; subscribe to a file.  
    // NOTE: Before the callback func is called, the ChangeNotifier
    // class opens the file with GENERIC_READ access with READ_SHARE
    // permissions to ensure that the file is not locked already. This
    // means that the file can only be opened with GENERIC_READ access
    // inside the callback func. Trying to open the file with any
    // other access will fail.
    // This function will use the default poll interval (about 10 seconds)
    static BOOL Subscribe(char *filename, FileChangeNotifyFunc func, FileChangeNotifyCookie cookie);

    // Same as above, but you can specify the poll interval in milliseconds
    static BOOL Subscribe(char *filename, FileChangeNotifyFunc func, FileChangeNotifyCookie cookie, DWORD pollIntervalMilliseconds);

    // Same as above, but you can specify the last known file write time (time of the last change that subscriber is already aware of)
    static BOOL Subscribe(
        char *filename, 
        FileChangeNotifyFunc func, 
        FileChangeNotifyCookie cookie, 
        DWORD pollIntervalMilliseconds, 
        FILETIME lastKnownWriteTime
        );

    // Callable on any thread; unsubscribe to notifications on a file
    // func and cookie must be the same values that you used to subscribe
    // Returns TRUE for success, and FALSE if the API thinks you were not subscribed
    static BOOL Unsubscribe(char *filename, FileChangeNotifyFunc func, FileChangeNotifyCookie cookie);

    // Callable on any thread; trigger any callbacks for the named file.
    // Does not attempt to lock the file, or update any of the
    // timestamps. This is primarily used by the in-memory config file
    // mechanism to trigger callbacks when the in-memory data
    // structure is changed.
    static void ReScanEntry(const char* filename);

private:
    // Find the ChangeNotifyEntry for a given filename, or NULL if not found
    static ChangeNotifyEntry *FindNotifyEntry(const char *filename);
    
    static void CALLBACK StopThread(ULONG_PTR dwParam);

    static HANDLE m_hThread;
    static DWORD m_dwThreadID;
    static volatile bool m_bStopped;
    static volatile LONG s_nPaused;
};
