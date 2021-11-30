#include "RegistryConfigMgr.h"
#include "logfile.h"
#include "ATW.h"


std::shared_ptr<RegistryConfigMgr> _instance = nullptr;
static std::once_flag _flag;

std::shared_ptr<RegistryConfigMgr> RegistryConfigMgr::instance()
{
	try
	{
		std::call_once(_flag, [&]() {
			_instance.reset(new RegistryConfigMgr());
			});
	}
	catch (...)
	{
		_instance = nullptr;
	}

	return _instance;
}

RegistryConfigMgr::RegistryConfigMgr()
	: m_uqPnpBoot(new WinRegistryKey(HKEY_LOCAL_MACHINE, PNPBOOL_PARAMETERS_REG_PATH))
	, m_domainRegKey(new WinRegistryKey(HKEY_LOCAL_MACHINE, CLINET_DOMAINS_REG_PATH))
{
#if defined ClientReg
	g_logger->debug("[RegistryConfigMgr] Game menu type: CLIENT.");
#else
	g_logger->debug("[RegistryConfigMgr] Game menu type: LOCAL.");
#endif
}

RegistryConfigMgr::~RegistryConfigMgr()
{
}

void RegistryConfigMgr::convert_nip_to_string(DWORD nIp, PTCHAR strIp)
{
	_stprintf(strIp, L"%d.%d.%d.%d", (DWORD)((unsigned char *)&nIp)[0], (DWORD)((unsigned char *)&nIp)[1], (DWORD)((unsigned char *)&nIp)[2], (DWORD)((unsigned char *)&nIp)[3]);
}

bool RegistryConfigMgr::convert_string_to_nip(PTCHAR strIp, unsigned int *nIp)
{
	bool bResult;

	unsigned int n1, n2, n3, n4;

	bResult = (EOF != _stscanf(strIp, _T("%d.%d.%d.%d"), &n1, &n2, &n3, &n4));

	((unsigned char *)nIp)[0] = (unsigned char)n1;
	((unsigned char *)nIp)[1] = (unsigned char)n2;
	((unsigned char *)nIp)[2] = (unsigned char)n3;
	((unsigned char *)nIp)[3] = (unsigned char)n4;

	return bResult;
}

DWORD RegistryConfigMgr::GetPlaceId()
{
	DWORD placeid = 0;
	m_uqPnpBoot->GetInt(g_PlaceId, placeid);
	return placeid;
}

bool RegistryConfigMgr::GetAuditType()
{
	DWORD auditType = 0;
	m_uqPnpBoot->GetInt(g_AuditType, auditType);
	return (bool)auditType;
}
std::wstring RegistryConfigMgr::GetServerIp()
{
#if defined ClientReg
	DWORD dwIp = 0;
	TCHAR strIp[24] = { 0 };

	if (m_uqPnpBoot->GetInt(g_OsServerIp_Backup, dwIp))
	{
		convert_nip_to_string(dwIp, strIp);
		g_logger->debug("[RegistryConfigMgr] [GetServerIp] ip: {}", WtoA(strIp));
	}
	else
	{
		g_logger->debug("[RegistryConfigMgr] [GetServerIp] Get OsServerIp_Backup failed.");
	}
	return std::wstring(strIp);
#else
	std::wstring _ip = m_uqPnpBoot->GetString(g_OsServerIp_Backup);
	g_logger->debug("[RegistryConfigMgr] [GetServerIp] ip: {}", WtoA(_ip));
	return _ip;
#endif
	//unsigned int iIp = 0;
	//TCHAR tIp[24] = { L"192.168.1.101" }; //2583800000
	//convert_string_to_nip(tIp, &iIp);
	//return _strIp;
}

std::string RegistryConfigMgr::GetClientMac()
{
	BYTE macByte[8] = { 0 };
	DWORD len = sizeof(macByte);
	char			mac[18];
	if (m_uqPnpBoot->GetBirary(g_ClientMac, (char*)macByte, len))
	{
		sprintf(mac, "%02X-%02X-%02X-%02X-%02X-%02X",
			macByte[0], macByte[1], macByte[2], macByte[3], macByte[4], macByte[5]);
		g_logger->debug("[RegistryConfigMgr] [GetClientMac] mac: {}", mac);
	}
	else
	{
		g_logger->debug("[RegistryConfigMgr] [GetClientMac] GetClientMac failed.");
	}
	
	return mac;
}

std::wstring RegistryConfigMgr::GetPlatDomain()
{
	std::wstring domainName = m_domainRegKey->GetString(g_TrackDomain);
	if (domainName.empty())
	{
		g_logger->debug("[RegistryConfigMgr] [GetPlatDomain] Platform domain name does not exist.");
	}
	return domainName;
}

std::wstring RegistryConfigMgr::GetPcid()
{
	std::wstring pcid = m_uqPnpBoot->GetString(g_PcId);
	if (pcid.empty())
	{
		g_logger->debug("[RegistryConfigMgr] [GetPcid] Pcid does not exist.");
	}
	return pcid;
}

void  RegistryConfigMgr::SetAllDomain(RequestDynamicDomainResponseObject responceObject)
{
	if (m_domainRegKey == NULL)
	{
		return;
	}

	/*m_domainRegKey->SetString(g_ResDomain, AtoW(responceObject.body.domains.res1Domain));
	m_domainRegKey->SetString(g_PassportDomain, AtoW(responceObject.body.domains.passportDomain));
	m_domainRegKey->SetString(g_FileDomain, AtoW(responceObject.body.domains.fileDomain));

	m_domainRegKey->SetString(g_CmsDomain, AtoW(responceObject.body.domains.cmsDomain));
	m_domainRegKey->SetString(g_TrackDomain, AtoW(responceObject.body.domains.trackDomain));
	m_domainRegKey->SetString(g_IpDomain, AtoW(responceObject.body.domains.ipDomain));

	m_domainRegKey->SetString(g_ClientDomain, AtoW(responceObject.body.domains.clientDomain));
	m_domainRegKey->SetString(g_GatewayDomain, AtoW(responceObject.body.domains.gatewayDomain));
	m_domainRegKey->SetString(g_MgrDomain, AtoW(responceObject.body.domains.mgrDomain));
	m_domainRegKey->SetString(g_MainDomain, AtoW(responceObject.body.domains.mainDomain));
	m_domainRegKey->SetString(g_DataDomain, AtoW(responceObject.body.domains.dataDomain));*/
	return;
}

std::wstring  RegistryConfigMgr::GetGateWayDomain()
{
	if (m_domainRegKey == NULL)
	{
		return L"";
	}
	return m_domainRegKey->GetString(_T("gatewayDomain"));
}

std::string RegistryConfigMgr::GetAllDomain()
{
	if (m_domainRegKey == NULL)
	{
		return "";
	}

	/*RequestDynamicDomainResponseBody reqDomains;
	reqDomains.domains.res1Domain = WtoA(m_domainRegKey->GetString(g_ResDomain));
	reqDomains.domains.passportDomain = WtoA(m_domainRegKey->GetString(g_PassportDomain));
	reqDomains.domains.fileDomain = WtoA(m_domainRegKey->GetString(g_FileDomain));
	reqDomains.domains.cmsDomain = WtoA(m_domainRegKey->GetString(g_CmsDomain));
	reqDomains.domains.trackDomain = WtoA(m_domainRegKey->GetString(g_TrackDomain));
	reqDomains.domains.ipDomain = WtoA(m_domainRegKey->GetString(g_IpDomain));
	reqDomains.domains.clientDomain = WtoA(m_domainRegKey->GetString(g_ClientDomain));
	reqDomains.domains.gatewayDomain = WtoA(m_domainRegKey->GetString(g_GatewayDomain));
	reqDomains.domains.mgrDomain = WtoA(m_domainRegKey->GetString(g_MgrDomain));
	reqDomains.domains.mainDomain = WtoA(m_domainRegKey->GetString(g_MainDomain));
	reqDomains.domains.dataDomain = WtoA(m_domainRegKey->GetString(g_DataDomain));*/

	std::string domainJson = "";
	//JsonHelper::ObjectToJson(reqDomains, domainJson);

	g_logger->debug("[RegistryConfigMgr] [GetAllDomain] {}", domainJson.c_str());
	return domainJson;
}


std::wstring RegistryConfigMgr::GetVersion()
{
	std::wstring ver = m_uqPnpBoot->GetString(g_DisplayVersion);
	if (ver.empty())
	{
		g_logger->debug("[RegistryConfigMgr] [GetVersion] App version does not exist.");
	}
	return ver;
}