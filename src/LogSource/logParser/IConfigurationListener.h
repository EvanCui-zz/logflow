#pragma once

#include "IConfiguration.h"

namespace autopilot
{
namespace configuration
{
	/**
	 * The interface that must be implemented by all configuration listeners
	 * to receive configuration change notifications.
	 *
	 * A Listener must remember to remove itself from being notified if
	 * the listener is being deleted or a crash will occur.
	 */
	class IConfigurationListener
	{
	public:

		/**
		 * The constructor.
		 */
		IConfigurationListener()
		{
		}

		/**
		 * The virtual destructor.
		 */
		virtual ~IConfigurationListener()
		{
		}

	protected:

		/**
		 * The copy constructor.
		 */
		IConfigurationListener(const IConfigurationListener&)
		{
		}

		/**
		 * The assignment operator.
		 */
		IConfigurationListener& operator=(IConfigurationListener&)
		{
			return (*this);
		}
		
	public:

		/**
		 * Receives the notification when a configuration changes.
		 *
		 * name - the original name used to register the listener
		 * pathname - the fully qualified pathname of the conifguration
		 * cookie - the original cookie used to register the listener
		 * configuration - the new (changed) configuration instance
		 *
		 * The cookie is the same pointer that was used to register
		 * the configuration listener.
		 *
		 * The pathname is the fully qualified name of the configuration.
		 * If the configuration is in memory, the pathname will be
		 * the same as the name.
		 *
		 * The configuration reference counted pointer points to the new
		 * configuration. The listener may elect to preserve the pointer
		 * and use it.
		 */
		virtual void ConfigurationChanged(
			const char* name,
			const char* pathname,
			void* cookie,
			Ptr<const IConfiguration> configuration) = 0;
	};
};
};

