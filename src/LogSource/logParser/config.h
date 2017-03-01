#pragma once

/**
 * DEPRECATED
 * No new code should be written using this set of classes.
 * Please use the new configuration implementation using Configuration.h.
 */
#pragma message(__FILE__ " is DEPRECATED; use Configuration.h instead")

#include <string.h>
#include <stdlib.h>
#pragma warning(push,3)
#include <vector>
#include <map>
#pragma warning(pop)

#include "Configuration.h"
#include "ConfigurationUtil.h"
#include "ConfigurationMap.h"

/**
 * Disable warning C4996: 'strncpy' was declared deprecated.
 */
#pragma warning(push)
#pragma warning(disable: 4996)

#pragma warning(disable:4634) 
// Turn off doc comment warnings in this file when compiling with /doc

/**
 * DEPRECATED
 * A synonym for the mutable configuration.
 */
#define ConfigFile autopilot::configuration::ConfigurationMap

/**
 * DEPRECATED
 * The old maximum section name size.
 */
const int c_configMaxSectionNameSize = 128;

/**
 * DEPRECATED
 * The old maximum parameter name size.
 */
const int c_configMaxParameterNameSize = 256;

/**
 * DEPRECATED
 * The old maximum parameter value size.
 */
const int c_configMaxParameterValueSize = 8192;

/**
 * DEPRECATED
 * Represents a single name-value pair defined in the config file.
 *
 * This is a very restricted representation since it limits the size of the
 * configuration name and value.
 *
 * The user of this deprecated class may elect to manipulate the public
 * members directly.
 */
class ConfigItem
{
private:

	ConfigItem(const ConfigItem&);
	ConfigItem& operator=(const ConfigItem&);

public:

	char m_name[c_configMaxParameterNameSize];
	char m_value[c_configMaxParameterValueSize];

	ConfigItem(const char* name = 0,const char* value = 0)
	{
		SetName(name);
		SetValue(value);
	}

	void SetName(const char* name)
	{
		if (name)
		{
			strncpy(m_name,name,sizeof(m_name));
			m_name[sizeof(m_name) - 1] = '\0';
		}
		else
		{
			m_name[0] = '\0';
		}
	}

	void SetValue(const char* value)
	{
		if (value)
		{
			strncpy(m_value,value,sizeof(m_value));
			m_value[sizeof(m_value) - 1] = '\0';
		}
		else
		{
			m_value[0] = '\0';
		}
	}
};


/**
 * DEPRECATED
 * Represents a single name-value pair defined in the config file.
 *
 * The user of this deprecated class may elect to manipulate the public
 * members directly.
 */
class ConfigNameValuePair
{
private:

	ConfigNameValuePair(const ConfigNameValuePair&);
	ConfigNameValuePair& operator=(const ConfigNameValuePair&);

public:

	const char* m_pName;
	size_t m_cbName;
	const char* m_pValue;
	size_t m_cbValue;

	ConfigNameValuePair(const char* name = 0,const char* value = 0)
	{
		SetName(name);
		SetValue(value);
	}

	void SetName(const char* name)
	{
		if (name)
		{
			m_pName = name;
			m_cbName = strlen(name);
		}
		else
		{
			m_pName = "";
			m_cbName = 0;
		}
	}

	void SetValue(const char* value)
	{
		if (value)
		{
			m_pValue = value;
			m_cbValue = strlen(value);
		}
		else
		{
			m_pValue = "";
			m_cbValue = 0;
		}
	}

	void SetItem(const ConfigItem& item)
	{
		SetName(item.m_name);
		SetValue(item.m_value);
	}
};

/**
 * DEPRECATED
 * Allows iteration over all the members of a configuration section.
 *
 * Note that the Next and NextPair methods return a pointer to internal
 * fields.
 */
class ConfigSectionEnumerator
{
private:

	ConfigSectionEnumerator(const ConfigSectionEnumerator&);
	ConfigSectionEnumerator& operator=(const ConfigSectionEnumerator&);

private:

	const Ptr<const ConfigurationMap> m_configuration;
	const std::string m_section;
	IConfiguration::ParameterNames m_parameters;
	IConfiguration::ParameterNames::const_iterator m_iterator;
	ConfigItem m_item;
	ConfigNameValuePair m_nameValue;

public:

    ConfigSectionEnumerator(
    	const Ptr<const ConfigurationMap>& configuration,
    	const char* section)
    :
    	m_configuration(configuration),
		m_section(section)
	{
		m_configuration->GetParameterNames(m_section.c_str(),m_parameters);
		m_iterator = m_parameters.begin();
	}

    const char* NextName(bool includeQualified = false);

    const ConfigItem* Next(bool includeQualified = true)
    {
		for (const char* name = NextName(includeQualified); name; name = NextName(includeQualified))
		{
			const char* value = m_configuration->GetInternalStringParameter(
				m_section.c_str(),
				name,
				0);
			if (value)
			{
				m_nameValue.SetName(name);
				m_nameValue.SetValue(value);
				m_item.SetName(name);
				m_item.SetValue(value);
				return (&m_item);
			}
		}
		return (0);
    }

    const ConfigNameValuePair* NextPair()
    {
		if (Next())
		{
			return (&m_nameValue);
		}
		return (0);
    }

    void Reset()
    {
		m_iterator = m_parameters.begin();
    }
};

/**
 * DEPRECATED
 * The configuration file.
 */
class ConfigFileNames
{
public:

	char m_relativeFilename[MAX_PATH];
	char m_filenamePart[MAX_PATH];

public:

	ConfigFileNames(const char* filename)
	{
		if (!filename)
		{
			filename = autopilot::configuration::Configuration::GetDefaultConfigName();
		}
		strncpy(m_relativeFilename,filename,sizeof(m_relativeFilename) - 1);
		m_relativeFilename[sizeof(m_relativeFilename) - 1] = '\0';
		ConfigurationUtil::GetFilename(
			m_filenamePart,sizeof(m_filenamePart),filename);
	}
};

/**
 * DEPRECATED
 * The configuration parser.
 */
class ConfigParser
{
private:

	const ConfigFileNames m_configFileNames;
	Ptr<const ConfigurationMap> m_configurationMap;
	Ptr<const IConfiguration> m_configuration;
	ConfigParser* m_override;
	IConfiguration::SectionNames m_sectionNames;
	std::vector<ConfigSectionEnumerator*> m_sectionEnumerators;

	struct NameCmp
	{
		bool operator()(const std::string& s1,const std::string& s2) const
		{
			return (_stricmp(s1.c_str(),s2.c_str()) < 0);
		}
	};
	typedef std::map<std::string,std::string,NameCmp> TempValues;
	typedef std::map<std::string,TempValues> TempMap;
	TempValues m_tempValues;

public:

	const ConfigFileNames* m_configFile;

public:

	/**
	 * Constructs the parser.
	 */
	ConfigParser(
		const char* filename,
		const Ptr<const IConfiguration>& configuration,
		ConfigParser* override = 0)
	:
		m_configFileNames(filename),
		m_configFile(&m_configFileNames),
		m_configurationMap(new ConfigurationMap(configuration)),
		m_configuration(m_configurationMap),
		m_override(override)
	{
		m_configuration->GetSectionNames(m_sectionNames);
	}

	~ConfigParser()
	{
		while (!m_sectionEnumerators.empty())
		{
			delete m_sectionEnumerators.back();
			m_sectionEnumerators.pop_back();
		}
		if (m_override)
		{
			delete m_override;
		}
	}

private:

	ConfigParser(const ConfigParser&);
	ConfigParser& operator=(const ConfigParser&);

public:

	// PLEASE don't use this!
	const char* GetTempParameter(const char* section,const char* parameter) const
	{
		return (m_configurationMap->GetInternalStringParameter(section,parameter,0));
	}

	// PLEASE don't use this!
	const char* GetTempParameter(const char* section,const ConfigNameValuePair* pNameValuePair) const
	{
		return (m_configurationMap->GetInternalStringParameter(section,pNameValuePair->m_pName,pNameValuePair->m_pValue));
	}

	BOOL GetParameter(const char *section,const ConfigNameValuePair* pNonBRSedNameValue,char* value,int valueSize) const
	{
		return (m_configuration->GetStringParameter(section,pNonBRSedNameValue->m_pName,value,(size_t)valueSize,pNonBRSedNameValue->m_pValue));
	}

	BOOL GetParameter(const char* section,const char* parameter,char* value,int valueSize) const
	{
		return (m_configuration->GetStringParameter(section,parameter,value,(size_t)valueSize));
	}

	BOOL GetParameter(const char* section,const char* parameter,char* value,int valueSize,const char* defaultValue) const
	{
		return (m_configuration->GetStringParameter(section,parameter,value,(size_t)valueSize,defaultValue));
	}

	BOOL GetParameter(const char* section,const char* parameter,std::string& value) const
	{
		return (m_configuration->GetStringParameter(section,parameter,value));
	}

	BOOL GetParameter(const char* section,const char* parameter,std::string& value,const char* defaultValue) const
	{
		return (m_configuration->GetStringParameter(section,parameter,value,defaultValue));
	}

	BOOL GetParameter(const char* section,const char* parameter,std::string& value,const std::string& defaultValue) const
	{
		return (m_configuration->GetStringParameter(section,parameter,value,defaultValue));
	}

	// PLEASE don't use this!
	char* GetParameter(const char *section,const char *parameter) const
	{
		std::string value;
		if (m_configuration->GetStringParameter(section,parameter,value))
		{
			return (_strdup(value.c_str()));
		}
		return (0);
	}

	// PLEASE don't use this!
	char* GetParameter(const char *section,const ConfigNameValuePair* pNonBRSedNameValue) const
	{
		std::string value;
		if (m_configuration->GetStringParameter(section,pNonBRSedNameValue->m_pName,value,pNonBRSedNameValue->m_pValue))
		{
			return (_strdup(value.c_str()));
		}
		return (0);
	}

	BOOL GetOverrideParameter(const char* overrideSection,const char* defaultSection,const char* parameter,char* value,int valueSize) const
	{
		if (GetParameter(overrideSection,parameter,value,valueSize))
		{
			return (TRUE);
		}
		return (GetParameter(defaultSection,parameter,value,valueSize));
	}

	BOOL GetIntParameter(const char* section,const char* parameter,int *value) const
	{
		return (m_configuration->GetIntParameter(section,parameter,value,*value));
	}

	BOOL GetIntParameter(const char* section,const char* parameter,int *value,int defaultValue) const
	{
		return (m_configuration->GetIntParameter(section,parameter,value,defaultValue));
	}

	BOOL GetUInt32Parameter(const char* section,const char* parameter,unsigned int *value) const
	{
		return (m_configuration->GetUIntParameter(section,parameter,value,*value));
	}

	BOOL GetUInt32Parameter(const char* section,const char* parameter,unsigned int *value,unsigned int defaultValue) const
	{
		return (m_configuration->GetUIntParameter(section,parameter,value,defaultValue));
	}

	BOOL GetInt64Parameter(const char* section,const char* parameter,__int64 *value) const
	{
		return (m_configuration->GetInt64Parameter(section,parameter,value,*value));
	}

	BOOL GetInt64Parameter(const char* section,const char* parameter,__int64 *value,__int64 defaultValue) const
	{
		return (m_configuration->GetInt64Parameter(section,parameter,value,defaultValue));
	}

	BOOL GetUInt64Parameter(const char* section,const char* parameter,unsigned __int64 *value) const
	{
		return (m_configuration->GetUInt64Parameter(section,parameter,value,*value));
	}

	BOOL GetUInt64Parameter(const char* section,const char* parameter,unsigned __int64 *value,unsigned __int64 defaultValue) const
	{
		return (m_configuration->GetUInt64Parameter(section,parameter,value,defaultValue));
	}

	BOOL GetOverrideIntParameter(const char *overrideSection,const char *defaultSection,const char *parameter,int *value) const
	{
		if (GetIntParameter(overrideSection,parameter,value))
		{
			return (TRUE);
		}
		return (GetIntParameter(defaultSection,parameter,value));
	}

	BOOL GetFloatParameter(const char* section,const char* parameter,float *value) const
	{
		return (m_configuration->GetFloatParameter(section,parameter,value,*value));
	}

	BOOL GetDoubleParameter(const char* section,const char* parameter,double* value) const
	{
		return (m_configuration->GetDoubleParameter(section,parameter,value,*value));
	}

	BOOL GetBoolParameter(const char* section,const char* parameter,bool *value) const
	{
		return (m_configuration->GetBoolParameter(section,parameter,value,*value));
	}

	BOOL DoesSectionExist(const char* section)
	{
		return (m_configuration->DoesSectionExist(section));
	}

	ConfigSectionEnumerator* EnumerateSection(
		const char *section,
		ConfigSectionEnumerator** brsEnumerator = 0)
	{
		if (brsEnumerator)
		{
			if (m_override)
			{
				*brsEnumerator = m_override->EnumerateSection(section,0);
			}
			else
			{
				*brsEnumerator = 0;
			}
		}
		ConfigSectionEnumerator* enumerator = 0;
		if (m_configuration->DoesSectionExist(section))
		{
			enumerator = new ConfigSectionEnumerator(m_configurationMap,section);
		}
		if (enumerator)
		{
			m_sectionEnumerators.push_back(enumerator);
		}
		return (enumerator);
	}

	int GetSectionCount(void) const
	{
		return ((int)m_sectionNames.size());
	}

	const char* GetSectionName(int nSection) const
	{
		if (nSection < 0 || nSection >= (int)m_sectionNames.size())
		{
			return (0);
		}
		return (m_sectionNames[(IConfiguration::SectionNames::size_type)nSection].c_str());
	}

	void GetParameterNames(
		const char* sectionName,
		IConfiguration::ParameterNames& parameterNames) const
	{
		m_configuration->GetParameterNames(sectionName,parameterNames);
	}


	Ptr<const IConfiguration> GetConfiguration() const
	{
		return (m_configuration);
	}
};

/**
 * DEPRECATED
 * The configuration change notify cookie.
 */
typedef void* ConfigChangeNotifyCookie;

/**
 * DEPRECATED
 * The configuration change notify function.
 */
class ConfigSubscribeInfo;
typedef void (*ConfigChangeNotifyFunc)(char *filename,ConfigParser *config,ConfigChangeNotifyCookie cookie,ConfigSubscribeInfo *si);

/**
 * DEPRECATED
 * The configuration change notify subscription information.
 */
class ConfigSubscribeInfo : public IConfigurationListener
{
private:

	ConfigSubscribeInfo(const ConfigSubscribeInfo&);
	ConfigSubscribeInfo& operator=(const ConfigSubscribeInfo&);

public:

	char m_pathName[MAX_PATH + 1];
	ConfigChangeNotifyFunc m_callbackFunc;
	ConfigChangeNotifyCookie m_cookie;
	void* m_listenerHandle;

	ConfigSubscribeInfo(
		ConfigChangeNotifyFunc callbackFunc,
		ConfigChangeNotifyCookie cookie)
	:
		m_callbackFunc(callbackFunc),
		m_cookie(cookie),
		m_listenerHandle(0)
	{
		m_pathName[0] = '\0';
	}

public:

	void ConfigurationChanged(
		const char* name,
		const char* pathname,
		void* /* cookie */,
		Ptr<const IConfiguration> configuration)
	{
		if (!name)
		{
			name = autopilot::configuration::Configuration::GetDefaultConfigName();
		}
		if (!pathname)
		{
			pathname = name;
		}
		strncpy(m_pathName,pathname,sizeof(m_pathName));
		m_pathName[sizeof(m_pathName) - 1] = '\0';
		ConfigParser parser(pathname,configuration);
		(*m_callbackFunc)(m_pathName,&parser,m_cookie,this);
	}
};

/**
 * DEPRECATED
 * The configuration class.
 */
class Config
{
private:

	Config();
	Config(const Config&);
	Config& operator=(const Config&);

public:
	static BOOL Init(const char *directory, const char *defaultConfigFilename)
	{
		return (autopilot::configuration::Configuration::Initialize(directory,defaultConfigFilename));
	}

	static BOOL Init(
		const char *directory,
		const char *defaultConfigFilename,
		const ConfigFile* inMemoryAutoPilot,
		const ConfigFile* inMemoryDefaultConfig)
	{
		return (autopilot::configuration::Configuration::Initialize(
			directory,
			defaultConfigFilename,
			inMemoryDefaultConfig,
			0,
			inMemoryAutoPilot));
	}

	static ConfigSubscribeInfo* Subscribe(
		const char *filename,
		ConfigChangeNotifyFunc notify,
		ConfigChangeNotifyCookie cookie,
		autopilot::configuration::Configuration::Types type = autopilot::configuration::Configuration::CONFIG_DEFAULT);

	static void Unsubscribe(
		ConfigSubscribeInfo *handle);

	static void ConfigFileChangeNotify(
		char *filename,
		autopilot::configuration::Configuration::Types type = autopilot::configuration::Configuration::CONFIG_DEFAULT);

	static ConfigParser* OpenConfig(
		const char *filename,
		autopilot::configuration::Configuration::Types type = autopilot::configuration::Configuration::CONFIG_DEFAULT);

	static void CloseConfig(ConfigParser *parser);

	static void RelativeToFullPath(
		const char *relative,
		char *fullpath,
		autopilot::configuration::Configuration::Types type = autopilot::configuration::Configuration::CONFIG_DEFAULT);

	static const char* GetDefaultFilename();
};

#pragma warning(pop)
