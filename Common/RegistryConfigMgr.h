#pragma once
#include <memory>
#include <tchar.h>
#include <mutex>
#include "WinRegistryKey.h"
#include "ReportHttpInterface.h"
//---------------------------------------------------------------------------
#define ClientReg			//�ͻ�����ʹ��

#if defined ClientReg
//ע���Keyֵ
const TCHAR* const g_OsServerIp_Backup = L"OsServerIp_Backup";
const TCHAR* const g_ClientMac = L"ClientMac";

const TCHAR* const g_PlaceId = L"PlaceId";
const TCHAR* const g_AuditType = L"AuditType";
const TCHAR* const g_PcId = L"Pcid";
const TCHAR* const g_PlatformDomainName = L"PlatformDomainName";
const TCHAR* const g_DisplayVersion = L"DisplayVersion";

const TCHAR* const g_ResDomain = L"res1Domain";
const TCHAR* const g_PassportDomain = L"passportDomain";
const TCHAR* const g_FileDomain = L"fileDomain";
const TCHAR* const g_CmsDomain = L"cmsDomain";
const TCHAR* const g_TrackDomain = L"trackDomain";
const TCHAR* const g_IpDomain = L"ipDomain";
const TCHAR* const g_ClientDomain = L"clientDomain";
const TCHAR* const g_GatewayDomain = L"gatewayDomain";
const TCHAR* const g_MgrDomain = L"mgrDomain";
const TCHAR* const g_MainDomain = L"mainDomain";
const TCHAR* const g_DataDomain = L"dataDomain";

#define PNPBOOL_PARAMETERS_REG_PATH			_T("SYSTEM\\CurrentControlSet\\services\\PnpBoot\\Parameters")
#define CLINET_DOMAINS_REG_PATH             _T("SYSTEM\\CurrentControlSet\\services\\PnpBoot\\Parameters\\Domains")
#else
//ע���Keyֵ
const TCHAR* const g_OsServerIp_Backup = L"masterIp";
const TCHAR* const g_ClientMac = L"ClientMac";

const TCHAR* const g_PlaceId = L"placeId";
const TCHAR* const g_AuditType = L"AuditType";
const TCHAR* const g_PcId = L"pcid";
const TCHAR* const g_PlatformDomainName = L"PlatformDomainName";
const TCHAR* const g_DisplayVersion = L"DisplayVersion";

const TCHAR* const g_ResDomain = L"res1Domain";
const TCHAR* const g_PassportDomain = L"passportDomain";
const TCHAR* const g_FileDomain = L"fileDomain";
const TCHAR* const g_CmsDomain = L"cmsDomain";
const TCHAR* const g_TrackDomain = L"trackDomain";
const TCHAR* const g_IpDomain = L"ipDomain";
const TCHAR* const g_ClientDomain = L"clientDomain";
const TCHAR* const g_GatewayDomain = L"gatewayDomain";
const TCHAR* const g_MgrDomain = L"mgrDomain";
const TCHAR* const g_MainDomain = L"mainDomain";
const TCHAR* const g_DataDomain = L"dataDomain";

#ifndef _WIN64
#define PNPBOOL_PARAMETERS_REG_PATH			_T("SOFTWARE\\ChiYanYunWei\\cfg")
#define CLINET_DOMAINS_REG_PATH             _T("SOFTWARE\\ChiYanYunWei\\cfg\\Domains")
#else	//_WIN64
#define PNPBOOL_PARAMETERS_REG_PATH			_T("SOFTWARE\\WOW6432Node\\ChiYanYunWei\\cfg")	//64λ������ʹ��64_32λ�����ע���·��,��������ʹ����64λ������32λ/64λ���������ͬ��ע���·��
#define CLINET_DOMAINS_REG_PATH             _T("SOFTWARE\\WOW6432Node\\ChiYanYunWei\\cfg\\Domains")
#endif	//_WIN64

#endif
//---------------------------------------------------------------------------

class RegistryConfigMgr
{
public:
	~RegistryConfigMgr();
	static std::shared_ptr<RegistryConfigMgr> instance();

protected:
	RegistryConfigMgr();

public:
	std::wstring GetServerIp();
	std::string GetClientMac();

	DWORD GetPlaceId();
	bool GetAuditType();
	std::wstring  GetPlatDomain();
	std::wstring  GetPcid();
	std::wstring  GetVersion();
	void          SetAllDomain(RequestDynamicDomainResponseObject responceObject);
	std::string   GetAllDomain();
	std::wstring  GetGateWayDomain();
private:
	void convert_nip_to_string(DWORD nIp, PTCHAR strIp);
	bool convert_string_to_nip(PTCHAR strIp, unsigned int *nIp);
	
private:
	std::unique_ptr<WinRegistryKey>  m_uqPnpBoot;
	std::unique_ptr<WinRegistryKey>  m_domainRegKey;
};

