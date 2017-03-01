#pragma once

#pragma warning(push)
#pragma warning(disable:4635) 
// Turn off doc comment warnings in this file when compiling with /doc

#include "IConfiguration.h"

namespace autopilot
{
namespace configuration
{
	/**
	 * The mutable configuration interface.
	 *
	 * This interface allows configuration to be modified.
	 */
	class IMutableConfiguration : public IConfiguration
	{
	protected:

		/**
		 * This is an interface with a protected constructor.
		 */
		IMutableConfiguration()
		{
		}

	protected:

		/**
		 * We don't need a copy constructor.
		 */
		IMutableConfiguration(const IMutableConfiguration&);

		/**
		 * We don't need an assignment operator.
		 */
		IMutableConfiguration& operator=(const IMutableConfiguration&);

	public:

		/**
		 * Adds an empty section.
		 *
		 * Returns true if the section was added or false if the section
		 * was already present.
		 */
		virtual bool AddSection(
			const char* sectionName) = 0;

		/**
		 * Erases a parameter.
		 *
		 * Returns true if the parameter was erased.
		 */
		virtual bool EraseParameter(
			const char* sectionName,
			const char* parameterName) = 0;

		/**
		 * Sets a parameter value.
		 *
		 * If the parameter exists and has the same value and flags,
		 * no action is taken. This method ignores the "no override"
		 * flag of existing parameters.
		 *
		 * Returns true if the parameter was set and the value or
		 * flags were changed.
		 */
		virtual bool SetParameter(
			const char* sectionName,
			const char* parameterName,
			const char* value,
			ParameterFlags flags = PARAM_DEFAULT) = 0;

		/**
		 * Sets a parameter value.
		 *
		 * If the parameter exists and has the same value and flags,
		 * no action is taken. This method ignores the "no override"
		 * flag of existing parameters.
		 *
		 * Returns true if the parameter was set and the value or
		 * flags were changed.
		 */
		inline bool SetParameter(
			const char* sectionName,
			const char* parameterName,
			const Parameter* parameter)
		{
			if (parameter)
			{
				return (SetParameter(
					sectionName,
					parameterName,
					parameter->m_value.c_str(),
					parameter->m_flags));
			}
			return (false);
		}

		/**
		 * Sets a parameter value.
		 *
		 * If the parameter exists and has the same value and flags,
		 * no action is taken. This method ignores the "no override"
		 * flag of existing parameters.
		 *
		 * Returns true if the parameter was set and the value or
		 * flags were changed.
		 */
		inline bool SetParameter(
			const char* sectionName,
			const char* parameterName,
			const Parameter& parameter)
		{
			return (SetParameter(
				sectionName,
				parameterName,
				parameter.m_value.c_str(),
				parameter.m_flags));
		}

		/**
		 * Overrides a parameter value.
		 *
		 * If the parameter cannot be overridden, no action is taken.
		 * If the parameter exists and has the same value and flags,
		 * no action is taken.
		 *
		 * Returns true if the parameter was set and the value or
		 * flags were changed.
		 */
		virtual bool OverrideParameter(
			const char* sectionName,
			const char* parameterName,
			const char* value,
			ParameterFlags flags = PARAM_DEFAULT) = 0;

		/**
		 * Overrides a parameter value.
		 *
		 * If the parameter cannot be overridden, no action is taken.
		 * If the parameter exists and has the same value and flags,
		 * no action is taken.
		 *
		 * Returns true if the parameter was set and the value or
		 * flags were changed.
		 */
		inline bool OverrideParameter(
			const char* sectionName,
			const char* parameterName,
			const Parameter* parameter)
		{
			if (parameter)
			{
				return (OverrideParameter(
					sectionName,
					parameterName,
					parameter->m_value.c_str(),
					parameter->m_flags));
			}
			return (false);
		}

		/**
		 * Overrides a parameter value.
		 *
		 * If the parameter cannot be overridden, no action is taken.
		 * If the parameter exists and has the same value and flags,
		 * no action is taken.
		 *
		 * Returns true if the parameter was set and the value or
		 * flags were changed.
		 */
		inline bool OverrideParameter(
			const char* sectionName,
			const char* parameterName,
			const Parameter& parameter)
		{
			return (OverrideParameter(
				sectionName,
				parameterName,
				parameter.m_value.c_str(),
				parameter.m_flags));
		}

		/**
		 * Merges a parameter value.
		 *
		 * If the parameter exists, no action is taken.
		 *
		 * Returns true if the parameter was set.
		 */
		virtual bool MergeParameter(
			const char* sectionName,
			const char* parameterName,
			const char* value,
			ParameterFlags flags = PARAM_DEFAULT) = 0;

		/**
		 * Merges a parameter value.
		 *
		 * If the parameter exists, no action is taken.
		 *
		 * Returns true if the parameter was set.
		 */
		inline bool MergeParameter(
			const char* sectionName,
			const char* parameterName,
			const Parameter* parameter)
		{
			if (parameter)
			{
				return (MergeParameter(
					sectionName,
					parameterName,
					parameter->m_value.c_str(),
					parameter->m_flags));
			}
			return (false);
		}

		/**
		 * Merges a parameter value.
		 *
		 * If the parameter exists, no action is taken.
		 *
		 * Returns true if the parameter was set.
		 */
		inline bool MergeParameter(
			const char* sectionName,
			const char* parameterName,
			const Parameter& parameter)
		{
			return (MergeParameter(
				sectionName,
				parameterName,
				parameter.m_value.c_str(),
				parameter.m_flags));
		}

		/**
		 * Merges one section into another.
		 *
		 * Only parameters from the source section that do not exist in the
		 * destination section are merged.
		 *
		 * Returns true if any the parameter were set.
		 */
		virtual bool MergeSection(
			const char* fromSection,
			const char* toSection) = 0;

		/**
		 * Copy this configuration from another.
		 *
		 * All the parametes from the source configuration are copied into
		 * this configuration. This configuration is cleared before being
		 * copied into.
		 *
		 * If section prefix is not NULL, the source sections that
		 * do not have the prefix will not be used, also the source
		 * sections that have the prefix will be copied without
		 * the prefix (i.e. the prefix will be removed from the section name).
		 */
		virtual void Copy(
			const IConfiguration* other,
			const char* sectionPrefix = 0) = 0;

		/**
		 * Overrides this configuration from another.
		 *
		 * All the parametes from the source configuration are copied into
		 * this configuration with the exception of parameters that cannot
		 * be overridden.
		 *
		 * If the parameter exists and has the same value and flags,
		 * no action is taken. If a parameter with a "stronger" filter
		 * already exists, no action is taken.
		 *
		 * If section prefix is not NULL, the overriding sections that
		 * do not have the prefix will not be used, also the overriding
		 * sections that have the prefix will override sections without
		 * the prefix (i.e. the prefix will be removed from the section name).
		 *
		 * Returns true if any of the parameters were added or modified
		 * in this configuration.
		 */
		virtual bool Override(
			const IConfiguration* other,
			const char* sectionPrefix = 0) = 0;

		/**
		 * Merges another congiguration into this one.
		 *
		 * Only parameters from the source configuration that do not exist in
		 * this configuration are merged.
		 *
		 * If section prefix is not NULL, the merging sections that
		 * do not have the prefix will not be used, also the merging
		 * sections that have the prefix will merge into sections without
		 * the prefix (i.e. the prefix will be removed from the section name).
		 *
		 * Returns true if any the parameter were added to this configuration.
		 */
		virtual bool Merge(
			const IConfiguration* other,
			const char* sectionPrefix = 0) = 0;

		/**
		 * Clones the configuration.
		 *
		 * The returned configuration pointer should be reference counted
		 * using the Ptr<> template to make sure that the cloned configuration
		 * is deleted when no longer used.
		 *
		 * If section prefix is not NULL, the overriding sections that
		 * do not have the prefix will not be used, also the overriding
		 * sections that have the prefix will override sections without
		 * the prefix (i.e. the prefix will be removed from the section name).
		 */
		virtual IMutableConfiguration* Clone(
			const char* sectionPrefix = 0) const = 0;

		/**
		 * Clears the configuration.
		 *
		 * Returns true if anything was cleared.
		 */
		virtual bool Clear() = 0;
	};
};
};

#pragma warning(pop)
