#pragma once

#pragma warning(push,3)
#pragma warning (disable:4365) // signed/unsigned mismatch
#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>
#include <vector>
#pragma warning(pop)

#pragma warning(push)
#pragma warning(disable:4634) 
// Turn off doc comment warnings in this file when compiling with /doc

#include "IConfiguration.h"
#include "IMutableConfiguration.h"
#include "IConfigurationListener.h"
namespace autopilot
{
namespace configuration
{
    class ConfigurationManager;

    extern bool g_slimModeConfiguration;
    extern int g_initialInstanceNumber;
    extern bool g_useNewInstanceNumbers;

	/**
	 * Base class for a simple intrinsic macro expander.
	 *
	 */
	class IIntrinsicMacroExpander : public RefCount
	{
	public:
		/**
		 *   Fetches the value of a single macro invocation.
		 *
		 *   macroName contains the name of the macro, either "!macroname" or "%envvarname".
		 *   If argsString is NULL, there are no arguments to the macro. Otherwise, it points to a whitespace-trimmed string containing the macro arguments.
		 *
		 *   The current supported intrinsic macros are:
		 *	   exedir - The directory in which the current running EXE is located
		 *	   bootstrapdir - The bootstrap directory (Normally ".." or whatever is in environment variable APENVIRONMENTDIR)
		 *	   environmentdir - The bootstrap directory (Normally ".." or whatever is in environment variable APENVIRONMENTDIR). Deprecated, retained for compatibility.
		 *	   configfiledir - The directory in which the default configuration file resides	   
		 *	   autopilotinidir - The directory in which the bootstrap config file resides
		 *
		 *   Returns false if the expansion failed.
		 *
		 */
		virtual bool ExpandMacro(std::string& valueOut, const char *macroName, const char *argsString = NULL, const IConfiguration *pConfiguration = NULL) const = 0;

		virtual ~IIntrinsicMacroExpander() {}
	};

	/**
	 * The global configuration accessor.
	 *
	 * This class accesses a single static collection of configurations to
	 * simplify configuration operations.
	 */
	class Configuration
	{
	public:

		/**
		 * The list of configuration override (BRS) pathnames.
		 *
		 * The configuration overrides (BRS) are used to override
		 * application configurations. (See BRS specifications.)
		 *
		 * The list must contain the overrides in the decreasing
		 * order of "strength" - the strongest override must come
		 * first in the list, the weakest override must come last
		 * in the list. There is no limit to the number of overrides.
		 *
		 * For example, the global override that overrides all other
		 * overrides must come first, the cluster override next, the
		 * environment override last.
		 *
		 * If the pathnames are not fully qualified, they are treated
		 * as relative to the data directory path.
		 */
		typedef std::vector<std::string> OverridePathnames;

		/**
		 * The configuration types.
		 *
		 * The configuration types control the type of configuration
		 * obtained with the GetConfiguration() method.
		 *
		 * CONFIG_PATH_XXX types may be used to obtain configurations
		 * that are located in application, environment, cluster,
		 * or global configuration directories. Note that only one
		 * of the CONFIG_PATH_XXX types may be used at one time.
		 * The CONFIG_PATH_XXX types are ignored if the configuration
		 * name is fully qualified (not relative).
		 */
		enum Types
		{
			/**
			 * The configuration does not use overrides (BRS).
			 *
			 * Applications should never use this flag for normal
			 * configurations since it disables overrides for that
			 * configuration.
			 *
			 * Applications should use this override if they take
			 * care of the overrides for special processing such
			 * as reading the override first and applying special
			 * transforms to the configuration file.
			 *
			 * NOTE: do not use this flag unless there is a good reason for it!
			 */
			CONFIG_NO_OVERRIDES = 0x01,

			/**
			 * The configuration is not filtered for the current
			 * cluster and environment.
			 *
			 * Applications should never use this flag for normal
			 * configurations since it disables filtering by cluster
			 * and environment. If filtering is disabled, applications
			 * will have to explicitly attempt to use various combinations
			 * of cluster and environment to qualify parameter names.
			 *
			 * Some applications that work across multiple environments
			 * may use this flag but provide only the cluster name to
			 * the configuration; this will enable filtering by cluster
			 * and the application can do the filtering by environment.
			 *
			 * NOTE: do not use this flag unless there is a good reason for it!
			 */
			CONFIG_NO_FILTERING = 0x02,

			/**
			 * The configuration is not to be automatically macro-expanded.
			 *
			 * If this flag is used, the configuration returned by
			 * GetConfiguration() will not have embedded macros expanded by
			 * GetParameter, etc.
			 *
			 * If the flag is omitted, the underling configuration will be wrapped
			 * with a thin wrapper that expands macro invocations each time
			 * a parameter is fetched.
			 *
			 * If a macro's result value changes independently of changes to the
			 * configuration, configuration change notification will not automatically occur, but the
			 * next time change notification happens, the new values will be identified
			 * as having changed.
			 *
			 */
			CONFIG_NO_MACROS = 0x04,

			/**
			 * The configuration is raw.
			 *
			 * No filtering or overrides - raw configuration.
			 */
			CONFIG_RAW = CONFIG_NO_OVERRIDES | CONFIG_NO_FILTERING | CONFIG_NO_MACROS,

			/**
			 * The configuration is to be automatically updated.
			 *
			 * If this flag is used, the configuration returned by
			 * GetConfiguration() will be automatically updated with
			 * new values if the values change. The caveat is that the
			 * access to the configuration will be synchronized and
			 * may cause some performance drain (due to the use of
			 * a critical section for every call).
			 *
			 * Configuration change listeners are always notified after
			 * the configuration values are updated for configurations
			 * that were obtained using this flag.
			 */
			CONFIG_AUTOUPDATE = 0x10,

			/**
			 * The configuration is to be read in a fallback mode.
			 *
			 * If this flag is used, the configuration returned by
			 * GetConfiguration() will have it's parameter names
			 * resolved in a fallback mode. Any call to get a configuration
			 * parameter will attempt to get the most qualified parameter
			 * first, and, failing that, get the next most qualified parameter.
			 *
			 * This flag is useful for non-filtered configurations
			 * (see CONFIG_NO_FILTERING) when qualified parameter names
			 * are used by the calling application.
			 *
			 * For example, if the application requsts to get a parameter
			 * using a name like cluster#environment$name, this flag will
			 * cause the configuration to do the following until it finds
			 * the parameter:
			 *		look up cluster#environment$name
			 *		look up environment$name
			 *		look up cluster#name
			 *		look up name
			 *
			 * For example, for cluster#name parameter:
			 *		look up cluster#name
			 *		look up name
			 *
			 * For example, for environment$name parameter:
			 *		look up environment$name
			 *		look up name
			 *
			 * For example, for name parameter:
			 *		look up name
			 *
			 * The actual separator characters used to separate the cluster
			 * and environment names are CLUSTER_SEPARATOR and
			 * ENVIRONMENT_SEPARATOR.
			 *
			 * NOTE: This flag should only be used by mangement applications
			 * that work with multiple clusters and/or environments at the
			 * same time.
			 *
			 * NOTE: Do not use this flag unnecessarily as there will be
			 * a performance drain due to multiple lookups.
			 */
			CONFIG_FALLBACK = 0x20,

			/**
			 * The configuration's override.
			 *
			 * If an application needs to access the override applied
			 * to a specific configuration, the application needs to
			 * request the override using the configuration's name and
			 * this flag.
			 *
			 * The override obtained using this flag will contain only
			 * the sections applicable to the configuration with the
			 * section names already resolved (i.e. if the override
			 * section name is [app$config$section], the returned
			 * section name will be [section]).
			 *
			 * The only reason to use this flag is to do some special
			 * override processing.
			 *
			 * The application would get the configuraiton using the
			 * CONFIG_NO_OVERRIDE flag to get the configuration without
			 * the applied override. Then the application would get
			 * the override ising this CONFIG_UNDERLYING_OVERRIDE flag
			 * to get the configuration's override. Then the application
			 * would do some special transformations to change the
			 * normal override behavior.
			 *
			 * NOTE: do not use this flag unless there is a good reason for it!
			 */
			CONFIG_UNDERLYING_OVERRIDE = 0x40,

			/**
			 * The configuration is encrypted.
			 *
			 * The configuration is in an an encrypted file.
			 * If this flag is not specified, the configuration is
			 * assumed to be in a plain file.
			 */
			CONFIG_ENCRYPTED = 0x80,

			/**
			 * The configuration is application specific.
			 *
			 * This configuration is located in or relative to the
			 * application directory or search path.
			 *
			 * This flag is ignored if the configuration path is
			 * fully qualified (not relative).
			 *
			 * Other CONFIG_PATH_XXX values may be used to access
			 * configurations that are not located in the application
			 * specific directory.
			 */
			CONFIG_PATH_APPLICATION = 0x0000,

			/**
			 * The configuration is environment-wide.
			 *
			 * This flag is ignored if the configuration path is
			 * fully qualified (not relative).
			 *
			 * The configuration is resolved against the environment
			 * configuration location(s). This configuration is shared by all
			 * the applications running within the environment.
			 */
			CONFIG_PATH_ENVIRONMENT = 0x0100,

			/**
			 * The configuration is cluster-wide.
			 *
			 * This flag is ignored if the configuration path is
			 * fully qualified (not relative).
			 *
			 * The configuration is resolved against the cluster-wide
			 * configuration location(s). This configuration is shared by all
			 * the environments within the cluster.
			 */
			CONFIG_PATH_CLUSTER = 0x0200,

			/**
			 * The configuration is global.
			 *
			 * This flag is ignored if the configuration path is
			 * fully qualified (not relative).
			 *
			 * The configuration is resolved against the global configuration
			 * location(s). This configuration is shared by all the clusters.
			 */
			CONFIG_PATH_GLOBAL = 0x0400,

			/**
			 * The configuration is in the old BRS locaiton.
			 *
			 * This location will be deprecated once the old BRS.INI file
			 * is migrated to use the cluster and environment overrides.
			 *
			 * NOTE: Do not use this flag - it is DEPRECATED!
			 */
			CONFIG_PATH_BRS = 0x0800,

			/**
			 * The configuration is in the bootstrap location (either
			 * the bootstrap search path or the bootstrap directory).
			 *
			 * This flag is ignored if the configuration path is
			 * fully qualified (not relative).
			 *
			 * The configuration is resolved against the bootstrap
			 * location(s). For example, the bootstrap location may be
			 * the parent directory of the application.
			 */
			CONFIG_PATH_BOOTSTRAP = 0x1000,

			/**
			 * The configuration is in the data location.
			 *
			 * This flag is ignored if the configuration path is
			 * fully qualified (not relative).
			 *
			 * The configuration is resolved against the data
			 * location. The data location is specified in the bootstrap
			 * configuration.
			 */
			CONFIG_PATH_DATA = 0x2000,

			/**
			 * The configuration is cluster-wide.
			 *
			 * This flag is ignored if the configuration path is
			 * fully qualified (not relative).
			 *
			 * The configuration is resolved against the cluster-wide
			 * configuration location. This configuration is the root of all
			 * the environments within the cluster i.e. all the environment
			 * directories as well as the cluster shared directory are rooted
			 * in this directory.
			 */
			CONFIG_PATH_CLUSTER_ROOT = 0x4000,

			/**
			 * The configuration path mask.
			 *
			 * This mask may be applied to the configuration type to
			 * isolate just the CONFIG_PATH_XXX type.
			 */
			CONFIG_PATH_MASK = 0xff00,

			/**
			 * The default configuration type.
			 *
			 * By default, the configuration is filtered for the current
			 * cluster and environment and uses override configurations (BRS)
			 * as specified in the bootstrap configuration.
			 */
			CONFIG_DEFAULT = CONFIG_PATH_APPLICATION,
		};

		/**
		 * The separator used for the cluster qualifier in parameter names.
		 *
		 * This cluster name is recognized by being immediately followed
		 * by the cluster name separator in parameter names.
		 */
		static const char CLUSTER_SEPARATOR;

		/**
		 * The separator used for the cluster qualifier in parameter names.
		 *
		 * This is just a string representation of the @a CLUSTER_SEPARATOR.
		 */
		static const char CLUSTER_SEPARATOR_STR[2];

		/**
		 * The separator used for the environment qualifier in parameter names.
		 *
		 * This environment name is recognized by being immediately followed
		 * by the environment name separator in parameter names.
		 */
		static const char ENVIRONMENT_SEPARATOR;

		/**
		 * The separator used for the environment qualifier in parameter names.
		 *
		 * This is just a string representation of the @a ENVIRONMENT_SEPARATOR.
		 */
		static const char ENVIRONMENT_SEPARATOR_STR[2];

		/**
		 * The separator used in the override configuration section names.
		 *
		 * This override files have their section names qualified by the
		 * applicaiton name, configuration name, etc. All the name parts
		 * are separated by the same separator character.
		 */
		static const char OVERRIDE_SEPARATOR;

		/**
		 * The separator used in the override configuration section names.
		 *
		 * This is just a string representation of the @a OVERRIDE_SEPARATOR.
		 */
		static const char OVERRIDE_SEPARATOR_STR[2];

	private:

		/**
		 * We do not need a constructor for this class.
		 */
		Configuration();

	public:
		
		/**
		 * Initializes the global configuration.
		 *
		 * If the application directory is NULL, the directory of the
		 * current application module is used.
		 *
		 * If the default configuration name is NULL, the pre-defined default
		 * configuration name is used. If the default configuration name
		 * is not NULL, it replaces the pre-defined default configuration
		 * name.
		 *
		 * If the default configuration is not NULL, the default configuration
		 * will not be read from a file; rather the "in memory" default
		 * conifguration is used. This is useful for unit tests or
		 * in situations when the default configuration must be dynamically
		 * generated.
		 *
		 * If the bootstrap configuration directory is not NULL, it is
		 * used as the base directory for the bootstrap configuration.
		 * If the bootstrap configuration directory is not NULL, it is
		 * assumed to be either fully qualified or relative to the
		 * application directory.
		 *
		 * If the bootstrap configuration is not NULL, it is used to
		 * determine the cluster, environment, and data directory.
		 * If the bootstrap configuration is NULL, the default mechanism
		 * is used to determine the bootstrap configuration. If the
		 * bootstrap configuration is NULL and the bootstrap configuration
		 * cannot be read from a file or determined by some other means,
		 * the initialization will fail.
		 *
		 * If the cluster and environment are both missing from
		 * the bootstrap configuration, no filtering is performed and
		 * no cluster or environment overrides will be used.
		 *
		 * If the data directory is not specified in the bootstrap
		 * configuration, no overrides of configuration will be used.
		 *
		 * If the override pathnames is NULL, the default override pathnames
		 * are used.
		 *
		 * This function can be called after PreInitialize (see below) which
		 * allows you to subclass ConfigurationManager and set basic
		 * parameters before initialization. CompleteInitialize() is called
		 * by this function, so no further initialization can be performed
		 * after this call.
		 *
		 * Returns false if the initialization failed and configuration
		 * cannot be used.
		 */
		static bool Initialize(
			const char* applicationDir = 0,
			const char* defaultConfigName = 0,
			const IConfiguration* defaultConfiguration = 0,
			const char* bootstrapConfigDir = 0,
			const IConfiguration* bootstrapConfiguration = 0,
			const OverridePathnames* overridePathnames = 0);

			
		/**
		 * Alternate way to Initialize the global configuration.
		 *
		 * The application can call PreInitialize with a newly created, uninitialized
		 * subclass of ConfigurationManager to begin using that configuration manager
		 * for initialization. If NULL is provided, a simple uninitialized instance of the base class is created.
		 *
		 * Then the application can set up the ConfigurationManager as desired.
		 * After the configuration manager has been fully initialized, the application
		 * should call CompleteInitialize.
		 *
		 * returns true if preinitialization succeeded.
		 */
		static bool PreInitialize(ConfigurationManager *configurationManager = NULL);

		/**
		 * Completes the alternate way to Initialize the global configuration (see PreInitialize).
		 *
		 *
		 * Returns false if the initialization failed and configuration
		 * cannot be used.
		 */
		static bool CompleteInitialize();

		/**
		 * Determines if the configuration has been initialized.
		 *
		 * The configuration MUST be initialized before most of the
		 * methods may be used.
		 */
		static bool IsInitialized();

		/**
		 * Returns the applicaiton name.
		 *
		 * The Initialize() is required to be called before this method.
		 *
		 * The application name is the last component in the fully
		 * qualified application directory. This is used for configuration
		 * overrides.
		 *
		 * Returns NULL if the method failed.
		 */
		static const char* GetApplicationName();

		/**
		 * Returns the default configuration name.
		 *
		 * The Initialize() is required to be called before this method.
		 *
		 * Note that the returned name is *not* necessarily fully qualified, but will be resolved against
		 * the application dir and application search path when the configuration is
		 * looked up. If new files and/or registered in-memory configurations are added
		 * earlier in the search path, then the resultant path can change.
		 *
		 * Returns NULL if the method failed.
		 */
		static const char* GetDefaultConfigName();

		/**
		 * Returns the fully qualified bootstrap configuration name.
		 *
		 * The Initialize() is required to be called before this method.
		 *
		 * Returns NULL if the method failed.
		 */
		static const char* GetBootstrapConfigName();

		/**
		 * Returns the fully qualified override pathnames.
		 *
		 * The Initialize() is required to be called before this method.
		 *
		 * The override pathnames parameter will be cleared before being set.
		 * The override pathnames collection will be empty if there are
		 * no override pathnames.
		 *
		 * Returns false if the method fails.
		 */
		static bool GetOverridePathnames(
			OverridePathnames& overridePathnames);

		/**
		 * Returns the cluster name.
		 *
		 * The Initialize() is required to be called before this method.
		 *
		 * Returns an empty string if there is no cluster filtering.
		 *
		 * Returns NULL if the method failed.
		 */
		static const char* GetClusterName();

		/**
		 * Returns the environment name.
		 *
		 * The Initialize() is required to be called before this method.
		 *
		 * Returns an empty string if there is no environment filtering.
		 *
		 * Returns NULL if the method failed.
		 */
		static const char* GetEnvironmentName();

		/**
		 * Returns the fully qualified directory for a given configuration type.
		 *
		 * The Initialize() is required to be called before this method.
		 *
		 * One of the CONFIG_PATH_XXX flag must be used to specify the
		 * type of the configuration directory to be returned.
		 *
		 * Note that this function does not make use of the search path for the
		 * given configuration type; it always uses the "default" directory
		 * for the config type. For this reason it is better to use
		 * MakeConfigPathname when the configuration name is known.
		 *
		 * Note that depending on the bootstrap configuration some of the
		 * configuration types may not be available. For example, if the
		 * cluster name is not set in the bootstrap configuration, the
		 * cluster and environment configuration directories are not available.
		 *
		 * Note that some environments do not have the cluster name set.
		 * (Applications must function correctly even under these conditions
		 * to be backward compatible.)
		 *
		 * Returns NULL if this type of a configuration directory is
		 * not available or if the method encountered an error.
		 */
		static const char* GetConfigDir(
			Types configType);

		/**
		 * Returns the search path for a given configuration type.
		 *
		 * One of the CONFIG_PATH_XXX flag must be used to specify the
		 * type of the configuration search path to be returned.
		 *
		 * Note that depending on the bootstrap configuration some of the
		 * configuration types may not be available. For example, if the
		 * cluster name is not set in the bootstrap configuration, the
		 * cluster and environment configuration search paths are not available.
		 *
		 * Note that some environments do not have the cluster name set.
		 * (Applications must function correctly even under these conditions
		 * to be backward compatible.)
		 *
		 * Note also that if a directory search path for a configuration type is not in effect,
		 * this method will return an empty search path. This does not mean that configurations cannot be found;
		 * it simply means that they are always normalized against the "default" directory where files
		 * of that configuration type live.
		 *
		 * Returns NULL if this type of a configuration search path is
		 * not available or if the method encountered an error.
		 */
		static const char* GetConfigSearchPath(
			Types configType);

		/**
		 * Returns a fully qualified file name for a given configuration type.
		 *
		 * The Initialize() is required to be called before this method.
		 *
		 * If the configuration name is NULL, the default configuration
		 * is used.
		 *
		 * This method will generate a fully qualified path name for a
		 * configuration file relative to the directory specified by the
		 * CONFIG_PATH_XXX.
		 *
		 * If the configuration name is already fully qualified, it will not
		 * be changed.
		 *
		 * One of the CONFIG_PATH_XXX flag may be used to specify the
		 * type of the configuration directory to be used.
		 *
		 * Note that depending on the bootstrap configuration some of the
		 * configuration types may not be available. For example, if the
		 * cluster name is not set in the bootstrap configuration, the
		 * cluster and environment configuration directories are not available.
		 *
		 * Note that some environments do not have the cluster name set.
		 * (Applications must function correctly even under these conditions
		 * to be backward compatible.)
		 *
		 * Returns false if this type of a configuration directory is
		 * not available or if the method encountered an error.
		 */
		static bool MakeConfigPathname(
			const char* configName,
			Types configType,
			std::string& configPathname);

		/**
		 * Returns the fully qualified path to the default configuration file.
		 */
		static const char *GetDefaultConfigPathname();

		/**
		 * Returns the fully qualified directory name containing the default configuration file.
		 */
		static const char *GetDefaultConfigDirName();
		
		/**
		 * Determines if a raw configuration exists for a given config name
		 *
		 * If the configuration name is NULL, the default configuration
		 * is used.
		 *
		 */
		static bool RawConfigurationExists(
			const char* name,
			Types configType = CONFIG_DEFAULT);
		
		/**
		 * Gets a configuration by a configuration name.
		 *
		 * The Initialize() is required to be called before this method.
		 *
		 * If the configuration name is NULL, the default configuration
		 * is returned.
		 *
		 * If CONFIG_AUTOUPDATE flag is used, the returned configuration
		 * will be automatically updated with new values if the values change.
		 * The caveat is that the access to the configuration will be
		 * synchronized and may cause some performance drain.
		 *
		 * The returned configuration is reference counted - do not delete
		 * the returned configuration. When done, simply let the
		 * returned reference go out of scope or set it to NULL.
		 *
		 * Returns NULL if the configuration is not available.
		 */
		static Ptr<const IConfiguration> GetConfiguration(
			const char* configName,
			Types configType = CONFIG_DEFAULT);

		/**
		 * Sets an "in-memory" configuration.
		 *
		 * The Initialize() is required to be called before this method.
		 *
		 * If the configuration name is NULL, the default configuration
		 * is replaced by this configuration.
		 *
		 * If the configuraiton is NULL, the configuration is cleared.
		 * If the configuration is not NULL, a copy of the configuration
		 * is made; any updates to the original configuration subsequent
		 * to this call will not be reflected in the stored copy.
		 *
		 * The configuration being set is assumed to be raw and without macro expansion;
		 * macros, overrides, and filtering are applied to it once it is set.
		 *
		 * The configuration type is used only to resolve the configuration
		 * name. This method may be used to replace the values of an
		 * already loaded configuration; this happens when the fully
		 * qualified configuration name that's being set matches an
		 * already loaded configuration.
		 *
		 * Configuration notification listeners registered for
		 * this name are called as needed. Note that the configuraiton
		 * listeners will not be called if the newly set configuration
		 * has the same contents as the old configuration.
		 *
		 * Returns false if the method fails.
		 */
		static bool SetConfiguration(
			const char* configName,
			const IConfiguration* configuration,
			Types configType = CONFIG_DEFAULT);

		/**
		 * Signals a configuration change.
		 *
		 * The Initialize() is required to be called before this method.
		 *
		 * This method is used to signal all the configuration listeners
		 * registered for this name. This method may be used by the
		 * application to signal a change even if the configuration has
		 * not changed.
		 *
		 * If the configuration name is NULL, the default configuration
		 * listeners will be signalled.
		 *
		 * The configuration type is used to resolve the configuration
		 * name.
		 *
		 * Returns false if the method fails.
		 */
		static bool SignalListeners(
			const char* configName,
			Types configType = CONFIG_DEFAULT);

		/**
		 * Registers a configuration change listener.
		 *
		 * The Initialize() is required to be called before this method.
		 *
		 * The name and cookie pointers are preserved and are used to
		 * signal the newly registered listener when configuration contents
		 * change. The name pointer may be NULL for a default configuration;
		 * if not NULL, the name pointer must point to a string. The cookie
		 * pointer does not need to be valid.
		 *
		 * Returns the listener handle that may be used to unregister the
		 * listener. Returns NULL if the listener could not be registered.
		 */
		static void* RegisterListener(
			const char* configName,
			void* cookie,
			IConfigurationListener* listener,
			Types configType = CONFIG_DEFAULT);

		/**
		 * Unregisters a configuration change listener.
		 *
		 * The Initialize() is required to be called before this method.
		 *
		 * Returns false if the method fails.
		 */
		static bool UnregisterListener(
			void* handle);

	public:

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
		 * Returns the fully qualified data directory (modified for the current instance # if necessary).
		 */
		static const char* GetDataDir();

		/**
		 * Returns the fully qualified data directory (not modified for the current instance #).
		 * Advanced! Most applications should use GetDataDir().
		 */
		static const char* GetDataDirNoInstance();

		/**
		 * Returns the UNC path to the local data directory (modified for the current instance # if necessary).
		 */
		static const char *GetLocalUncDataDir();

		/**
		 * Returns the UNC path to the local data directory (not modified for the current instance #).
		 * Advanced! Most applications should use GetLocalUncDataDir().
		 */
		static const char *GetLocalUncDataDirNoInstance();
		
		/**
		 * Returns the network data dir without the \\servername\ prefix (modified for the current instance # if necessary).
		 */
		static const char *GetNetworkDataDirNoServerName();

		/**
		 * Returns the network data dir without the \\servername\ prefix (not modified for the current instance).
		 */
		static const char *GetNetworkDataDirNoInstanceNoServerName();

		/**
		 * Returns the directory in which Configuration was launched.
		 */
		static const char *GetInitialWorkingDir();
		
		/**
		 * Returns the directory in which the currently running EXE resides.
		 */
		static const char *GetExeDir();
		
		/**
		 * If running multi-instance, returns the instance #.
		 */
		static int GetInstanceNum();

		/**
		 * Returns the multi-instance instance ID.
		 */
		static const char *GetInstanceId();

		/**
		 * Returns the Multi-Instance root.
		 */
		static const char *GetInstanceRoot();

		/**
		 * Returns the Multi-Instance subdir name.
		 */
		static  const char *GetInstanceSubdirName();

		/**
		 * Returns the Multi-Instance machine name. In single-instance mode, this is the computer name. In multi-instance
		 * mode it is virtualized.
		 */
		static const char *GetMachineName() ;

		/**
		 * Returns the real machine name, not modified by multi-instance. May be overridden with APMACHINENAME.
		 */
		static const char *GetRawMachineName();

		/**
		 * Returns the real machine name, modified by multi-instance. Not overridden with APMACHINENAME.
		 */
		static const char *GetHostName();

		/**
		 * Returns the real machine name, not modified by multi-instance. Not overridden with APMACHINENAME.
		 */
		static const char *GetRawHostName();

		/**
		 * Returns the fully qualified machinelist file
		 */
		static const char* GetMachinelistFile();
		
		/**
		 * Returns the fully qualified podlist file
		 */
		static const char* GetPodlistFile();
		
		/**
		 * Returns true if new-style instance numbering is being used.
		 */
		static bool IsUsingNewInstanceNumbers();

		/**
		 * Strips off any instancing alteration from the machine name and constructs a UNC path to the machine without regard to the instance
		 * number. Use this method when you want to get to a well-known base share on a server.
		 *
		 * This method is generally not very useful for multi-instance, since all instances on a particular machine will resolve
		 * to the same path.
		 *
		 * If machine is NULL, uses the current machine.
		 * If shareNameAndSubpath is NULL or empty, simply constructs "\\rawmachinename".
		 *
		 * Returns false with GetLastError() if the path could not be constructed.
		 *
		 */
		static bool GetUncPathNoInstance(char * szBuf, size_t cchBuf, const char *machine=NULL, const char*shareNameAndSubpath=NULL);
		static bool GetUncPathNoInstance(std::string& buffer, const char *machine=NULL, const char*shareNameAndSubpath=NULL);

		

		/**
		 * Fetches a UNC path within a specific machine's data dir, taking instancing into account.
		 *
		 * If machine is NULL, uses the current machine.
		 * If subpath is NULL or empty, simply constructs a path to the data directory itself.
		 *
		 * Returns false with GetLastError() if the path could not be constructed.
		 *
		 */
		static bool GetUncDataDir(char *szBuf, size_t cchBuf, const char *machine=NULL, const char *subpath=NULL);
		static bool GetUncDataDir(std::string& buffer, const char *machine=NULL, const char *subpath=NULL);

		/**
		 * look up the IP address and port number of instance port #basePort running on the specified machine.
		 * Instance numbering in the machine name is taken into account.
		 * The desired address and port are filled in to pAddrOut and pPortOut.
		 * Either pAddrOut or pPortOut may be NULL to omit determination of that value.
		 * If machine is NULL, values for the local instance are returned.
		 *
		 * Returns false on failure.
		 */
		static bool GetAddressAndPort(struct in_addr* pAddrOut, unsigned short* pPortOut, unsigned short basePort=0, const char* machine=NULL);

		/**
		 * look up the port number of instance port# basePort running on the specified machine.
		 * Instance numbering in the machine name is taken into account.
		 * The desired port is filled in to pPortOut.
		 * If machine is NULL, the value for the local instance is returned.
		 *
		 * Returns false on failure.
		 */
		static bool GetPort(unsigned short *pPortOut, unsigned short basePort, const char *machine=NULL);

	public:

		/**
		 * Generates a default configuration.
		 *
		 * This method may be called before the Initialize()is called.
		 *
		 * This is useful to pre-create a default configuration when one
		 * is not available for some reason. The default configuration
		 * will set up some values that are useful during the initialization.
		 *
		 * The returned configuration is mutable and may be extended with
		 * other values.
		 *
		 * The configuration returned by this method is independent of the
		 * Initialize() and will always return the same configuration.
		 *
		 * Returns NULL if the configuration could not be allocated.
		 */
		static Ptr<IMutableConfiguration> GenerateDefaultConfig();

		/**
		 * Generates a default bootstrap configuration.
		 *
		 * This method may be called before the Initialize()is called.
		 *
		 * This is useful to pre-create a default bootstrap configuration when
		 * one is not available for some reason. The default bootstrap
		 * will set up some values that are useful during the initialization.
		 *
		 * The returned configuration is mutable and may be extended with
		 * other values.
		 *
		 * If the data directory is NULL, the application directory is used
		 * as the data directory.
		 *
		 * If the network data directory is NULL, a probably icorrect data
		 * directory name is made up.
		 *
		 * If the environment or cluster name is NULL, the corresponding
		 * entries are not made in the bootstrap configuration.
		 *
		 * Some parameters that are required in the bootstrap configuration
		 * are added.
		 *
		 * Returns NULL if the configuration could not be allocated.
		 */
		static Ptr<IMutableConfiguration> GenerateDefaultBootstrapConfig(
			const char* dataDir,
			const char* networkDataDir,
			const char* environment,
			const char* cluster);

		/**
		 *   Fetches the value of a single macro invocation.
		 *
		 *   macroName contains the name of the macro, either "!macroname" or "%envvarname".
		 *   If argsString is NULL, there are no arguments to the macro. Otherwise, it points to a whitespace-trimmed string containing the macro arguments.
		 *
		 *   Returns false if the expansion failed.
		 *
		 */
		static bool ExpandMacro(std::string& valueOut, const char *macroName, const char *argsString = NULL, const IConfiguration *pConfiguration = NULL);

		/**
		 *   Replaces zero or more occurrences of simple intrinsic macro strings of the form {!macroname args} or {%macroname} within a string.
		 *
		 *   See ExpandMacro for a list of supported macros.
		 *   The default implementation calls ExpandMacro repeatedly (through InternalExpandMacroInPlace) to replace invocations within the string.
		 *
		 * Returns false if there was an error expanding the macros.
		 */
		static bool ExpandMacros(std::string& valueInOut, const IConfiguration *pConfiguration = NULL);

		/*
		 * Registers a handler that will dynamically expand a macro value at usage time. The expander is refcounted.
		 * Replaces any existing expander.
		 */
		static bool RegisterMacroExpander(const char *macroName, IIntrinsicMacroExpander *expander);
		
		/*
		 * Registers a macro that always returns a specific string.
		 * Replaces any existing expander.
		 */
		static bool RegisterMacroString(const char *macroName, const char *macroValue);

		/*
		 * Removes the registration for a macro.
		 */
		static bool UnregisterMacro(const char *macroName);

		/*
		 * Turns on/off verbose config logging.  THe default is off.
		 */
        static bool SetVerboseConfigurationLogging(bool fVerbose = true);
        
	private:

		/**
		 * Validates initialization. Initializes a default configuration manager if initialization has not yet happened
		 *
		 * This method will assert if initialization was not performed
		 * or failed.
		 *
		 * Returns true if initialization was performed OK.
		 */
		static bool ValidateInitialized();

		/**
		 * Validates initialization started. Initializes a default configuration manager if initialization has not yet begun
		 *
		 * This method will assert if initialization was not performed
		 * or failed.
		 *
		 * Returns true if initialization was performed OK.
		 */
		static bool ValidateInitializeStarted();
	};
};
};

/**
 * Make the namespace transparent to simplify the use of the
 * configuration.
 */
using namespace autopilot::configuration;

#pragma warning(pop)
