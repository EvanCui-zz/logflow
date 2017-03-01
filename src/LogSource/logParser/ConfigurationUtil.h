#pragma once

#include <stdlib.h>
#pragma warning(push,3)
#include <string>
#pragma warning(pop)

namespace autopilot
{
namespace configuration
{
	// A wrapper for a search path string. Contains a semicolon-delimited list of directories to be searched in
	// order for a particular file.
	class FileSearchPath
	{
	public:

		// Constructs a new search path. The list is initially empty.
		FileSearchPath();
		~FileSearchPath();

		// Resets the search path to an empty list.
		void Clear();

		// Returns the semicolon-delimited list of fully-qualified directory names.
		const char *GetSearchPath() const;

		// returns true if and only if there are no directories in the list.
		bool IsEmpty() const;

		// Appends a single directory or a semicolon-delimited list of directories to the end of this search path.
		// If pszPath is NULL, does nothing.
		// If fExpandRelative is true, canonicalizes all directories in the appended search path relative to pszBaseDir. If
		// pszBaseDir==NULL, the working directory is used for relative paths.
		// If false is returned, GetLastError() has been set.
		// Not thread safe
		bool AppendSearchPath(const char *pszPath, bool fExpandRelative = true, const char *pszBaseDir = NULL);
		
		// Sets the search path to a single directory or a semicolon-delimited list of directories.
		// Is pszPath is NULL, seats the search path to an empty list.
		// If fExpandRelative is true, canonicalizes all directories in the appended search path relative to pszBaseDir. If
		// pszBaseDir==NULL, the working directory is used for relative paths.
		// If false is returned, GetLastError() has been set.
		// Not thread safe
		bool SetSearchPath(const char *pszPath, bool fExpandRelative = true, const char *pszBaseDir = NULL);

		// Appends the value of a named environment variable to the end of this search path. 
		// If the environment variable doesn't exist, does nothing.
		// If false is returned, GetLastError() has been set.
		bool AppendEnvVarSearchPath(const char *pszEnvVarName, bool fExpandRelative = true, const char *pszBaseDir = NULL);

		// Sets this search path to the value of a named environment variable. 
		// If the environment variable doesn't exist, sets the search path to an empty list.
		// If false is returned, GetLastError() has been set.
		bool SetEnvVarSearchPath(const char *pszEnvVarName, bool fExpandRelative = true, const char *pszBaseDir = NULL);

		// Locates a file in this search path (or its own path if fully qualified), and constructs a fully qualified pathname
		//
		// Returns false with GetLastError() if the specified file could not be located or an error occurred.
		bool Search(
			std::string& fullPathnameOut,
			const char *pszFilename) const;

		// Locates a file in this search path (or its own path if fully qualified), and constructs a fully qualified pathname.
		//
		// If the file cannot be located using the search path, then a fully qualified pathname is constructed by
		// normalizing the filename against the provided default directory. This is useful when you want to "find or create"
		// a file that may or may not already exist.
		//
		// If defaultDir is NULL, the current working directory is used
		//
		// Returns false with GetLastError() if an error occured.
		bool SearchWithDefaultDir(
			std::string& fullPathnameOut,
			const char *pszFilename,
			const char *defaultDir=NULL) const;

		// Thin wrapper around Windows SearchPathA
		// Locates a file in this search path (or its own path if fully qualified)
		// If the function succeeds, the value returned is the length, in chars,
		// of the string copied to the buffer, not including the terminating null character.
		// If the return value is greater than nBufferLength, the value returned is the size
		// of the buffer required to hold the path (including the terminating null character). In
		// addition, SetLastError(ERROR_BUFFER_OVERFLOW) is done in this case (an extension to the Win32 spec).
		// The return value should never return equal to nBufferLength unless nBufferLength is 0.
		// If the function fails, the return value is zero. To get extended error information, call GetLastError().
		// See documentation for WIN32 SearchPathA.
		size_t WinSearch(
			const char * pszFilename, // file to locate. If it contains a path, that is used instead of search path
			char * pBufferOut, // Buffer to received canonical path
			size_t cbBufferLength, // length of pBufferOut in chars, including place for null terminator
			char **ppszFilePartOut = NULL// returned pointer into lpBuffer to just the tail name of the file. If NULL, not returned
		) const;

	private:
		char *m_pBuffer; // on heap
		size_t m_nb;
		size_t m_nbAlloc;
	};

	/**
	 * Configuration utility class.
	 */
	class ConfigurationUtil
	{
	private:

		ConfigurationUtil();

	public:

		/**
		 * Normalizes a file pathname.
		 *
		 * The provided pathname is normalized (fully qualified).
		 * All the slashes (/) are converted to backslashes (\).
		 *
		 * Returns false if there was an error.
		 */
		static bool NormalizePathname(
			char* buffer,
			size_t bufferSize,
			const char* pathname);

		/**
		 * Normalizes a file pathname.
		 *
		 * The provided pathname is normalized (fully qualified).
		 * All the slashes (/) are converted to backslashes (\).
		 *
		 * Returns false if there was an error.
		 */
		inline static bool NormalizePathname(
			std::string& buffer,
			const char* pathname)
		{
			char tmp[_MAX_PATH];
			if (NormalizePathname(tmp,sizeof(tmp),pathname))
			{
				buffer = tmp;
				return (true);
			}
			return (false);
		}

		/**
		 * Normalizes a file pathname.
		 *
		 * The provided pathname is combined with the directory to provide
		 * a normalized (fully qualified) file pathname.
		 * If pathname is already fully qualified, the directory is ignored.
		 * Unless pathname is already fully qualified and directory is NULL,
		 * the current directory is used.
		 *
		 * Returns false if there was an error.
		 */
		static bool NormalizePathname(
			char* buffer,
			size_t bufferSize,
			const char* directory,
			const char* pathname);

		/**
		 * Normalizes a file pathname.
		 *
		 * The provided pathname is normalized (fully qualified).
		 * All the slashes (/) are converted to backslashes (\).
		 *
		 * Returns false if there was an error.
		 */
		inline static bool NormalizePathname(
			std::string& buffer,
			const char* directory,
			const char* pathname)
		{
			char tmp[_MAX_PATH];
			if (NormalizePathname(tmp,sizeof(tmp),directory,pathname))
			{
				buffer = tmp;
				return (true);
			}
			return (false);
		}

		/**
		 * Returns true if the specified fully qualified pathname exists
		 */
		static bool FilePathnameExists(const char *fullpathname);
		
		/**
		 * Extracts a fle name from a file pathname.
		 *
		 * The provided pathname may or may not contian directory parts.
		 * Only the file name and file extension are extracted.
		 *
		 * Returns false if there was an error.
		 */
		static bool GetFilename(
			char* buffer,
			size_t bufferSize,
			const char* pathname);

		/**
		 * Extracts a fle name from a file pathname.
		 *
		 * The provided pathname may or may not contian directory parts.
		 * Only the file name and file extension are extracted.
		 *
		 * Returns false if there was an error.
		 */
		static bool GetFilename(
			std::string& buffer,
			const char* pathname)
		{
			char tmp[_MAX_PATH];
			if (GetFilename(tmp,sizeof(tmp),pathname))
			{
				buffer = tmp;
				return (true);
			}
			return (false);
		}

		/**
		 * Extracts a directory name from a pathname.
		 *
		 * The provided pathname must contan the directory parts, or "" and false is returned.
		 *
		 * Returns false if there was an error.
		 */
		static bool GetDirectoryName(
			char* buffer,
			size_t bufferSize,
			const char* pathname);

		/**
		 * Extracts a directory name from a pathname.
		 *
		 * The provided pathname must contan the directory parts, or "." is returned.
		 *
		 * Returns false if there was an error.
		 */
		static bool GetDirectoryName(
			std::string& buffer,
			const char* pathname)
		{
			char tmp[_MAX_PATH];
			if (GetDirectoryName(tmp,sizeof(tmp),pathname))
			{
				buffer = tmp;
				return (true);
			}
			return (false);
		}

		/**
		 * Converts '/' to '\\' and removes trailing backslashes from pathnames
		 */
		 static void NormalizeSlashes(
			std::string& buffer,
			const char *pathname);

		/**
		 * Works just like GetCurrentDirectory, except gets the directory that the running EXE
		 * resides in rather than the current working directory.
		 *
		 * See Windows documentation for GetCurrentDirectory
		 */
		static DWORD GetCurrentExeDirectoryA(DWORD nBufferLength, LPSTR lpBuffer);
		static DWORD GetCurrentExeDirectoryW(DWORD nBufferLength, LPWSTR lpBuffer);
		
		/**
		 * Fetches an environment variable (UTF8 encoding).
		 *
		 * If the variable does not exist, uses defaultValue defaultValue it is not NULL, or fails with GetLastError() == ERROR_ENVVAR_NOT_FOUND otherwise. 
		 *
		 * If not supplied, an empty string is used as defaultValue, which is unambiguous since existent environment variables cannot be empty.
		 *
		 * returns false with GetLastError() if the operation fails.
		 */
		static bool GetEnvironmentVar(std::string& valueOut, const char *varname, const char *defaultValue="");

		/**
		 * Converts a WCHAR unicode (UTF16) string to UTF8. Cannot fail (Asserts if windows complains)
		 *
		 * If the input string is NULL, produces an empty string (too bad stl strings can't be NULL).
		 *
		 * If the length is not given, it is obtained with wstrlen.
		 */
		static void WideToUtf8(std::string& strOut, const WCHAR *strIn, size_t lengthIn = ((size_t)-1));

		/**
		 * Converts a UTF8 string to WCHAR unicode (UTF16). Cannot fail (Asserts if windows complains)
		 *
		 * If the input string is NULL, produces an empty string (too bad stl strings can't be NULL).
		 *
		 * If the length is not given, it is obtained with strlen.
		 */
		static void Utf8ToWide(std::wstring& strOut, const char *strIn, size_t lengthIn = ((size_t)-1));

		/**
		 * Separates a parameter name into component names.
		 *
		 * The parameter name is broken down into the cluster, environment,
		 * and plain parameter name components.
		 *
		 * @return @c false if the original parameter name has a bad format.
		 */
		static bool BreakParameterName(
			const char* parameterName,
			std::string& clusterPart,
			std::string& environmentPart,
			std::string& namePart);

		/**
		 * Composes a parameter name from component names.
		 *
		 * If the @a clusterPart or the @a environmentPart is NULL or
		 * empty, it is not used.
		 */
		static void ComposeParameterName(
			const char* clusterPart,
			const char* environmentPart,
			const char* namePart,
			std::string& parameterName);

		/**
		 * Composes an override section prefix.
		 *
		 * If the @a applicationName or the @a configurationName is NULL or
		 * empty, it is not used.
		 */
		static void ComposeOverridePrefix(
			const char* applicationName,
			const char* configurationName,
			std::string& overridePrefix);

		/**
		 * Composes an override section name.
		 *
		 * If the @a applicationName or the @a configurationName is NULL or
		 * empty, it is not used.
		 */
		static void ComposeOverrideSection(
			const char* applicationName,
			const char* configurationName,
			const char* sectionName,
			std::string& overrideSection);
	};

	/*
	  * Temporary object classes used by macros to help simplify ephemeral UTF8/WCHAR conversions
	  */
	class TempWideToUtf8String
	{
	public:
		TempWideToUtf8String(const WCHAR *s)
		{
			m_isNull = (s == NULL);
			if (!m_isNull) {
				ConfigurationUtil::WideToUtf8(m_str, s);
			}
		}

		const char *get()
		{
			return (m_isNull ? NULL : m_str.c_str());
		}

	private:
		std::string m_str;
		bool m_isNull;
	};

	// The following macro returns a UTF16 representation of a null-terminated UTF8 string (NULL pointers are preserved). The representation
	// becomes invalid after the current C++ statement, so it can only safely be used, e.g., to pass the converted value as a parameter to a method call.
	#define WSTRINGTOUTF8(s) (TempWideToUtf8String(s).get())

	class TempUtf8ToWideString
	{
	public:
		TempUtf8ToWideString(const char *s)
		{
			m_isNull = (s == NULL);
			if (!m_isNull) {
				ConfigurationUtil::Utf8ToWide(m_str, s);
			}
		}

		const WCHAR *get()
		{
			return (m_isNull ? NULL : m_str.c_str());
		}

	private:
		std::wstring m_str;
		bool m_isNull;
	};

	// The following macro returns a UTF8 representation of a null-terminated UTF16 string (NULL pointers are preserved). The representation
	// becomes invalid after the current C++ statement, so it can only safely be used, e.g., to pass the converted value as a parameter to a method call.
	#define UTF8TOWSTRING(s) (TempUtf8ToWideString(s).get())

	
};
};

