#pragma once

#pragma warning(disable : 4995)

#include <stdio.h>
#include <stdlib.h>
#pragma warning(push,3)
#include <string>
#include <vector>
#pragma warning(pop)

#include "RefCount.h"

namespace autopilot
{
namespace configuration
{
	/**
	 * The configuration interface.
	 *
	 * All the configuration information may be accessed via this interface
	 * by using parameter or section access methods.
	 *
	 * A configuration is divided into any number of sections, each section
	 * may have any number of parameters (name value pairs).
	 *
	 * A configuration section is qualified by its name. A section name is
	 * unique within a configuration.
	 *
	 * A parameter is qualified by the section name and the parameter name.
	 * A parameter name is unique within a section.
	 */
	class IConfiguration : public RefCount
	{
	public:

		/**
		 * Section names.
		 */
		typedef std::vector<std::string> SectionNames;

		/**
		 * Parameter names.
		 */
		typedef std::vector<std::string> ParameterNames;

		/**
		 * Parameter flags.
		 */
		enum ParameterFlags
		{
			/**
			 * The default parameter flag.
			 */
			PARAM_DEFAULT = 0,

			/**
			 * The parameter should not be overridden.
			 *
			 * Parameters with this flag are preserved from being overriden
			 * by the override (BRS) configurations or "Override" methods.
			 */
			PARAM_NO_OVERRIDE = 0x01,
		};

		/**
		 * Parameter descriptor.
		 */
		class Parameter
		{
		public:

			/**
			 * The parameter string value.
			 */
			std::string m_value;

			/**
			 * The parameter flags.
			 */
			ParameterFlags m_flags;

		public:

			/**
			 * The default constructor.
			 */
			inline Parameter()
			:
				m_flags(PARAM_DEFAULT)
			{
			}

			/**
			 * The full constructor.
			 */
			inline Parameter(
				const char* value,
				ParameterFlags flags)
			:
				m_value(value ? value : ""),
				m_flags(flags)
			{
			}

			/**
			 * The full constructor.
			 */
			inline Parameter(
				const std::string& value,
				ParameterFlags flags)
			:
				m_value(value),
				m_flags(flags)
			{
			}

			/**
			 * The copy constructor.
			 */
			inline Parameter(const Parameter& other)
			:
				m_value(other.m_value),
				m_flags(other.m_flags)
			{
			}

			/**
			 * The assignment operator.
			 */
			inline Parameter& operator=(const Parameter& other)
			{
				m_value = other.m_value;
				m_flags = other.m_flags;
				return (*this);
			}

			/**
			 * Compare this parameter to parameter values.
			 */
			inline bool IsEqual(const char* value) const
			{
				if (!value)
				{
					return (false);
				}
				return (strcmp(value,m_value.c_str()) == 0);
			}

			/**
			 * Compare this parameter to parameter values.
			 */
			inline bool IsEqual(const std::string& value) const
			{
				return (strcmp(value.c_str(),m_value.c_str()) == 0);
			}

			/**
			 * Compare this parameter to parameter flags.
			 */
			inline bool IsEqual(ParameterFlags flags) const
			{
				return (flags == m_flags);
			}

			/**
			 * Compare this parameter to parameter values.
			 */
			inline bool IsEqual(
				const char* value,
				ParameterFlags flags) const
			{
				return (IsEqual(flags) && IsEqual(value));
			}

			/**
			 * Compares this parameter to another.
			 *
			 * Returns false if the parameters are not equal.
			 */
			inline bool IsEqual(const Parameter* other) const
			{
				if (other != this)
				{
					if (!other)
					{
						return (false);
					}
					return (IsEqual(other->m_value.c_str(),other->m_flags));
				}
				return (true);
			}

			/**
			 * Compares this parameter to another.
			 */
			inline bool operator==(const Parameter& other) const
			{
				return (IsEqual(&other));
			}
		};

		/**
		 * String value collection.
		 */
		typedef std::vector<std::string> StringValues;

	protected:

		/**
		 * This is an interface with a protected constructor.
		 */
		IConfiguration()
		{
		}

		/**
		 *   A simple implementation of IsParameterEqual that works for most implementations.
		 *   Uses GetParameter and does a direct comparison.
		 */
		bool IsParameterEqual_Base(
			const char* sectionName,
			const char* parameterName,
			const Parameter* parameter) const;

		/**
		 *   A simple implementation of IsEqual that works for most implementations.
		 *   Walks the sections/parameters and does a direct comparison.
		 */
		bool IsEqual_Base(
			const IConfiguration* other) const;

	public:

		/**
		 * The destructor is virtual.
		 */
		virtual ~IConfiguration()
		{
		}

	protected:

		/**
		 * We don't need a copy constructor.
		 */
		IConfiguration(const IConfiguration&);

		/**
		 * We don't need an assignment operator.
		 */
		IConfiguration& operator=(const IConfiguration&);

	public:

		/**
		 * Determines if a section exists.
		 *
		 * Returns false if the section does not exist.
		 */
		virtual bool DoesSectionExist(
			const char* sectionName) const = 0;

		/**
		 * Returns a collection of all the section names.
		 */
		virtual void GetSectionNames(
			SectionNames& sectionNames) const = 0;

		/**
		 * Returns the number of sections.
		 */
		virtual unsigned int GetSectionCount() const = 0;

		/**
		 * Determines if a parameter exists.
		 *
		 * Returns false if the parameter does not exist.
		 */
		virtual bool DoesParameterExist(
			const char* sectionName,
			const char* parameterName) const = 0;

		/**
		 * Returns a collection of all the parameter names.
		 */
		virtual void GetParameterNames(
			const char* sectionName,
			ParameterNames& parameterNames) const = 0;

		/**
		 * Returns the number of parameters.
		 */
		virtual unsigned int GetParameterCount(
			const char* sectionName) const = 0;

		/**
		 * Retrieves a parameter.
		 *
		 * Returns false if the parameter could not be found.
		 */
		virtual bool GetParameter(
			const char* sectionName,
			const char* parameterName,
			Parameter& parameter) const = 0;

		/**
		 * Retrieves the parameter value.
		 *
		 * If the value is too large to fit into the value buffer parameter,
		 * the value is treated as "not found" since it violates the expected
		 * maximum size condition.
		 *
		 * Returns false if the value could not be found.
		 */
		virtual bool GetStringParameter(
			const char* sectionName,
			const char* parameterName,
			char* value,
			size_t valueSize) const = 0;

		/**
		 * Retrieves the parameter value.
		 *
		 * Returns false if the value could not be found.
		 */
		virtual bool GetStringParameter(
			const char* sectionName,
			const char* parameterName,
			std::string& value) const = 0;

		/**
		 * Determines if this configuration's parameter is equal to another.
		 *
		 * Returns true if the parameters are equal.
		 *
		 * The default implementation fetches the parameter and directly compares it.
		 */
		virtual bool IsParameterEqual(
			const char* sectionName,
			const char* parameterName,
			const Parameter* parameter) const
		{
			return IsParameterEqual_Base(sectionName, parameterName, parameter);
		}

		/**
		 * Determines if this configuration has the same content as another.
		 *
		 * Returns true if the configurations are equal.
		 *
		 * The default implementation walks the sections/parameters and compares them.
		 */
		virtual bool IsEqual(
			const IConfiguration* other) const
		{
			return IsEqual_Base(other);
		}

		/**
		 * Determines if this configuration is empty.
		 *
		 * Returns true if the configurations is empty.
		 *
		 * The default implementation just checks for the section count being zero
		 */
		virtual bool IsEmpty() const
		{
			return (GetSectionCount() == 0);
		}

	public:

		/**
		 * Retrieves the parameter value.
		 *
		 * If the value could not be found in the configuration and the
		 * default value is not NULL, the default value is used.
		 *
		 * Returns false if the value could not be found in the configuration.
		 */
		bool GetStringParameter(
			const char* sectionName,
			const char* parameterName,
			std::string& value,
			const char* defaultValue) const;

		/**
		 * Retrieves the parameter value.
		 *
		 * If the value could not be found in the configuration,
		 * the default value is used.
		 *
		 * Returns false if the value could not be found in the configuration.
		 */
		inline bool GetStringParameter(
			const char* sectionName,
			const char* parameterName,
			std::string& value,
			const std::string& defaultValue) const
		{
			return (GetStringParameter(sectionName,parameterName,value,defaultValue.c_str()));
		}

		/**
		 * Retrieves the parameter value.
		 *
		 * If the value is too large to fit into the value buffer parameter,
		 * the value is treated as "not found" since it violates the expected
		 * maximum size condition.
		 *
		 * Returns false if the value could not be found in the configuration.
		 */
		bool GetStringParameter(
			const char* sectionName,
			const char* parameterName,
			char* value,
			size_t valueSize,
			const char* defaultValue) const;
		
		/**
		 * Retrieves the parameter value.
		 *
		 * Returns the default value if the value could not be found in the
		 * configuration.
		 */
		inline std::string GetStringParameter(
			const char* sectionName,
			const char* parameterName,
			const char* defaultValue) const
		{
			std::string value;
			GetStringParameter(sectionName,parameterName,value,defaultValue);
			return (value);
		}

		/**
		 * Retrieves the parameter value.
		 *
		 * Returns the default value if the value could not be found in the
		 * configuration.
		 */
		inline std::string GetStringParameter(
			const char* sectionName,
			const char* parameterName,
			const std::string& defaultValue) const
		{
			std::string value;
			GetStringParameter(sectionName,parameterName,value,defaultValue);
			return (value);
		}

		/**
		 * Retrieves the parameter value.
		 *
		 * The returned value list is always cleared even if the value
		 * could not be found in the configuration.
		 *
		 * Returns false if the value could not be found in the configuration.
		 */
		bool GetStringListParameter(
			const char* sectionName,
			const char* parameterName,
			StringValues& values,
			const char* separators = ",") const;

		/**
		 * Retrieves the parameter value.
		 *
		 * If the value could not be found in the configuration,
		 * the default value is used.
		 *
		 * Returns false if the value could not be found in the configuration.
		 */
		bool GetBoolParameter(
			const char* sectionName,
			const char* parameterName,
			bool* value,
			bool defaultValue) const;
		
		/**
		 * Retrieves the parameter value.
		 *
		 * Returns the default value if the value could not be found in the
		 * configuration.
		 */
		inline bool GetBoolParameter(
			const char* sectionName,
			const char* parameterName,
			bool defaultValue) const
		{
			bool value;
			GetBoolParameter(sectionName,parameterName,&value,defaultValue);
			return (value);
		}

		/**
		 * Retrieves the parameter value.
		 *
		 * If the value could not be found in the configuration,
		 * the default value is used.
		 *
		 * Returns false if the value could not be found in the configuration.
		 */
		inline bool GetIntParameter(
			const char* sectionName,
			const char* parameterName,
			int* value,
			int defaultValue) const
		{
			__int32 result;
			bool ok = GetInt32Parameter(sectionName,parameterName,&result,defaultValue);
			*value = (int)result;
			return (ok);
		}
		
		/**
		 * Retrieves the parameter value.
		 *
		 * Returns the default value if the value could not be found in the
		 * configuration.
		 */
		inline int GetIntParameter(
			const char* sectionName,
			const char* parameterName,
			int defaultValue) const
		{
			return ((int)GetInt32Parameter(sectionName,parameterName,defaultValue));
		}

		/**
		 * Retrieves the parameter value.
		 *
		 * If the value could not be found in the configuration,
		 * the default value is used.
		 *
		 * Returns false if the value could not be found in the configuration.
		 */
		inline bool GetUIntParameter(
			const char* sectionName,
			const char* parameterName,
			unsigned int* value,
			unsigned int defaultValue) const
		{
			unsigned __int32 result;
			bool ok = GetUInt32Parameter(sectionName,parameterName,&result,defaultValue);
			*value = (unsigned int)result;
			return (ok);
		}
		
		/**
		 * Retrieves the parameter value.
		 *
		 * Returns the default value if the value could not be found in the
		 * configuration.
		 */
		inline unsigned int GetUIntParameter(
			const char* sectionName,
			const char* parameterName,
			unsigned int defaultValue) const
		{
			return ((unsigned int)GetUInt32Parameter(sectionName,parameterName,defaultValue));
		}

		/**
		 * Retrieves the parameter value.
		 *
		 * If the value could not be found in the configuration,
		 * the default value is used.
		 *
		 * Returns false if the value could not be found in the configuration.
		 */
		bool GetInt16Parameter(
			const char* sectionName,
			const char* parameterName,
			__int16* value,
			__int16 defaultValue) const;

		/**
		 * Retrieves the parameter value.
		 *
		 * Returns the default value if the value could not be found in the
		 * configuration.
		 */
		inline __int16 GetInt16Parameter(
			const char* sectionName,
			const char* parameterName,
			__int16 defaultValue) const
		{
			__int16 value;
			GetInt16Parameter(sectionName,parameterName,&value,defaultValue);
			return (value);
		}

		/**
		 * Retrieves the parameter value.
		 *
		 * If the value could not be found in the configuration,
		 * the default value is used.
		 *
		 * Returns false if the value could not be found in the configuration.
		 */
		bool GetUInt16Parameter(
			const char* sectionName,
			const char* parameterName,
			unsigned __int16* value,
			unsigned __int16 defaultValue) const;

		/**
		 * Retrieves the parameter value.
		 *
		 * Returns the default value if the value could not be found in the
		 * configuration.
		 */
		inline unsigned __int16 GetUInt16Parameter(
			const char* sectionName,
			const char* parameterName,
			unsigned __int16 defaultValue) const
		{
			unsigned __int16 value;
			GetUInt16Parameter(sectionName,parameterName,&value,defaultValue);
			return (value);
		}

		/**
		 * Retrieves the parameter value.
		 *
		 * If the value could not be found in the configuration,
		 * the default value is used.
		 *
		 * Returns false if the value could not be found in the configuration.
		 */
		bool GetInt32Parameter(
			const char* sectionName,
			const char* parameterName,
			__int32* value,
			__int32 defaultValue) const;

		/**
		 * Retrieves the parameter value.
		 *
		 * Returns the default value if the value could not be found in the
		 * configuration.
		 */
		inline __int32 GetInt32Parameter(
			const char* sectionName,
			const char* parameterName,
			__int32 defaultValue) const
		{
			__int32 value;
			GetInt32Parameter(sectionName,parameterName,&value,defaultValue);
			return (value);
		}

		/**
		 * Retrieves the parameter value.
		 *
		 * If the value could not be found in the configuration,
		 * the default value is used.
		 *
		 * Returns false if the value could not be found in the configuration.
		 */
		bool GetUInt32Parameter(
			const char* sectionName,
			const char* parameterName,
			unsigned __int32* value,
			unsigned __int32 defaultValue) const;

		/**
		 * Retrieves the parameter value.
		 *
		 * Returns the default value if the value could not be found in the
		 * configuration.
		 */
		inline unsigned __int32 GetUInt32Parameter(
			const char* sectionName,
			const char* parameterName,
			unsigned __int32 defaultValue) const
		{
			unsigned __int32 value;
			GetUInt32Parameter(sectionName,parameterName,&value,defaultValue);
			return (value);
		}

		/**
		 * Retrieves the parameter value.
		 *
		 * If the value could not be found in the configuration,
		 * the default value is used.
		 *
		 * Returns false if the value could not be found in the configuration.
		 */
		bool GetInt64Parameter(
			const char* sectionName,
			const char* parameterName,
			__int64* value,
			__int64 defaultValue) const;

		/**
		 * Retrieves the parameter value.
		 *
		 * Returns the default value if the value could not be found in the
		 * configuration.
		 */
		inline __int64 GetInt64Parameter(
			const char* sectionName,
			const char* parameterName,
			__int64 defaultValue) const
		{
			__int64 value;
			GetInt64Parameter(sectionName,parameterName,&value,defaultValue);
			return (value);
		}

		/**
		 * Retrieves the parameter value.
		 *
		 * If the value could not be found in the configuration,
		 * the default value is used.
		 *
		 * Returns false if the value could not be found in the configuration.
		 */
		bool GetUInt64Parameter(
			const char* sectionName,
			const char* parameterName,
			unsigned __int64* value,
			unsigned __int64 defaultValue) const;

		/**
		 * Retrieves the parameter value.
		 *
		 * Returns the default value if the value could not be found in the
		 * configuration.
		 */
		inline unsigned __int64 GetUInt64Parameter(
			const char* sectionName,
			const char* parameterName,
			unsigned __int64 defaultValue) const
		{
			unsigned __int64 value;
			GetUInt64Parameter(sectionName,parameterName,&value,defaultValue);
			return (value);
		}

		/**
		 * Retrieves the parameter value.
		 *
		 * If the value could not be found in the configuration,
		 * the default value is used.
		 *
		 * Returns false if the value could not be found in the configuration.
		 */
		bool GetFloatParameter(
			const char* sectionName,
			const char* parameterName,
			float* value,
			float defaultValue) const;

		/**
		 * Retrieves the parameter value.
		 *
		 * Returns the default value if the value could not be found in the
		 * configuration.
		 */
		inline float GetFloatParameter(
			const char* sectionName,
			const char* parameterName,
			float defaultValue) const
		{
			float value;
			GetFloatParameter(sectionName,parameterName,&value,defaultValue);
			return (value);
		}

		/**
		 * Retrieves the parameter value.
		 *
		 * If the value could not be found in the configuration,
		 * the default value is used.
		 *
		 * Returns false if the value could not be found in the configuration.
		 */
		bool GetDoubleParameter(
			const char* sectionName,
			const char* parameterName,
			double* value,
			double defaultValue) const;

		/**
		 * Retrieves the parameter value.
		 *
		 * Returns the default value if the value could not be found in the
		 * configuration.
		 */
		inline double GetDoubleParameter(
			const char* sectionName,
			const char* parameterName,
			double defaultValue) const
		{
			double value;
			GetDoubleParameter(sectionName,parameterName,&value,defaultValue);
			return (value);
		}

		/**
		 * Dumps the configuration for debugging.
		 *
		 * If the section name is not NULL, only one section is dumped.
		 */
		void DumpToLog(const char* sectionName = 0) const;

		/**
		 * Dumps the configuration for debugging.
		 *
		 * If the section name is not NULL, only one section is dumped.
		 */
		void Dump(FILE* file,const char* sectionName = 0) const;
	};
};
};

