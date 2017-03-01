#pragma once

#include <string.h>
#pragma warning(push,3)
#include <map>
#include <string>
#pragma warning(pop)

#include "IMutableConfiguration.h"

namespace autopilot
{
namespace configuration
{
	/**
	 * The configuration map.
	 */
	class ConfigurationMap : public IMutableConfiguration
	{
	private:

		/**
		 * Names are case insensitive.
		 */
		struct NameCmp
		{
			bool operator()(const std::string& s1,const std::string& s2) const
			{
				return (_stricmp(s1.c_str(),s2.c_str()) < 0);
			}
		};

		/**
		 * The map of parameters.
		 */
		typedef std::map<const std::string,Parameter,NameCmp> ParamMap;

		/**
		 * The parameters.
		 */
		struct Params
		{
			ParameterNames m_paramNames;
			ParamMap m_paramMap;
		};

		/**
		 * The map of sections.
		 */
		typedef std::map<const std::string,Params,NameCmp> SectionMap;

		/**
		 * The list of sections.
		 */
		typedef std::vector<const std::string> SectionList;

		/**
		 * The sections.
		 */
		struct Sections
		{
			SectionNames m_sectionNames;
			SectionMap m_sectionMap;
		};

	private:

		/**
		 * The map of sections.
		 */
		Sections m_sections;

	public:

		/**
		 * Constructs the map.
		 */
		ConfigurationMap()
		{
		}

		/**
		 * Constructs a map from a configuration.
		 *
		 * If section prefix is not NULL, the source sections that
		 * do not have the prefix will not be used, also the source
		 * sections that have the prefix will be renamed to contain no
		 * prefix (i.e. the prefix will be removed from the section name).
		 */
		ConfigurationMap(
			const IConfiguration* other,
			const char* sectionPrefix = 0);

	protected:

		/**
		 * We don't need a copy constructor.
		 */
		ConfigurationMap(const ConfigurationMap& other);

		/**
		 * We don't need an assignment operator.
		 */
		ConfigurationMap& operator=(const ConfigurationMap&);

	public:

		//
		// Implementation of IConfiguration.
		//

		bool DoesSectionExist(
			const char* sectionName) const;

		void GetSectionNames(
			SectionNames& sectionNames) const;

		unsigned int GetSectionCount() const;

		bool DoesParameterExist(
			const char* sectionName,
			const char* parameterName) const;

		void GetParameterNames(
			const char* sectionName,
			ParameterNames& parameterNames) const;

		virtual unsigned int GetParameterCount(
			const char* sectionName) const;

		bool GetParameter(
			const char* sectionName,
			const char* parameterName,
			Parameter& parameter) const;

		bool GetStringParameter(
			const char* sectionName,
			const char* parameterName,
			std::string& value) const;

		bool GetStringParameter(
			const char* sectionName,
			const char* parameterName,
			char* value,
			size_t valueSize) const;

		bool IsParameterEqual(
			const char* sectionName,
			const char* parameterName,
			const Parameter* parameter) const;

		/* Use the default implementation of IsEqual
		bool IsEqual(
			const IConfiguration* other) const;
		*/

		/* Use the default implementation of IsEmpty
		bool IsEmpty() const;
		*/

	public:

		//
		// Implementation of IMutableConfiguration.
		//

		bool AddSection(
			const char* sectionName);

		bool EraseParameter(
			const char* sectionName,
			const char* parameterName);

		bool SetParameter(
			const char* sectionName,
			const char* parameterName,
			const char* value,
			ParameterFlags flags = PARAM_DEFAULT);

		inline bool SetParameter(
			const char* sectionName,
			const char* parameterName,
			const Parameter* parameter)
		{
			return (IMutableConfiguration::SetParameter(
				sectionName,
				parameterName,
				parameter));
		}

		inline bool SetParameter(
			const char* sectionName,
			const char* parameterName,
			const Parameter& parameter)
		{
			return (IMutableConfiguration::SetParameter(
				sectionName,
				parameterName,
				parameter));
		}

		bool OverrideParameter(
			const char* sectionName,
			const char* parameterName,
			const char* value,
			ParameterFlags flags = PARAM_DEFAULT);

		inline bool OverrideParameter(
			const char* sectionName,
			const char* parameterName,
			const Parameter* parameter)
		{
			return (IMutableConfiguration::OverrideParameter(
				sectionName,
				parameterName,
				parameter));
		}

		inline bool OverrideParameter(
			const char* sectionName,
			const char* parameterName,
			const Parameter& parameter)
		{
			return (IMutableConfiguration::OverrideParameter(
				sectionName,
				parameterName,
				parameter));
		}

		bool MergeParameter(
			const char* sectionName,
			const char* parameterName,
			const char* value,
			ParameterFlags flags = PARAM_DEFAULT);

		inline bool MergeParameter(
			const char* sectionName,
			const char* parameterName,
			const Parameter* parameter)
		{
			return (IMutableConfiguration::MergeParameter(
				sectionName,
				parameterName,
				parameter));
		}

		inline bool MergeParameter(
			const char* sectionName,
			const char* parameterName,
			const Parameter& parameter)
		{
			return (IMutableConfiguration::MergeParameter(
				sectionName,
				parameterName,
				parameter));
		}

		bool MergeSection(
			const char* fromSection,
			const char* toSection);

		void Copy(
			const IConfiguration* other,
			const char* sectionPrefix = 0);

		bool Override(
			const IConfiguration* other,
			const char* sectionPrefix = 0);

		bool Merge(
			const IConfiguration* other,
			const char* sectionPrefix = 0);

		IMutableConfiguration* Clone(
			const char* sectionPrefix = 0) const;

		bool Clear();

	public:

		/**
		 * A very dangerous method that is available for backward
		 * compatibility. Please do not use.
		 */
		const char* GetInternalStringParameter(
			const char* sectionName,
			const char* parameterName,
			const char* defaultValue) const;

	protected:

		/**
		 * Finds a parameter.
		 *
		 * Returns NULL if the parameter could not be found.
		 */
		const Parameter* FindParameter(
			const char* sectionName,
			const char* parameterName) const;

		/**
		 * Finds a mutable parameter.
		 *
		 * Returns NULL if the parameter could not be found.
		 */
		Parameter* FindMutableParameter(
			const char* sectionName,
			const char* parameterName);
	};
};
};

