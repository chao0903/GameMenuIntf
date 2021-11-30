#include "RapidJsonMgr.h"
#include "document.h"
#include "stringbuffer.h"
#include "writer.h"
#include "ATW.h"
#include "GlobalInt.h"
#include "RegistryConfigMgr.h"
#include "Constants.h"

static std::shared_ptr<CRapidJsonMgr> s_instance = nullptr;
static std::mutex s_mutex;
std::string CRapidJsonMgr::m_jsonstr = "";


std::shared_ptr<CRapidJsonMgr>& CRapidJsonMgr::instance()
{
	if (nullptr == s_instance)
	{
		std::lock_guard<std::mutex> lock(s_mutex);
		if (nullptr == s_instance) {
			s_instance.reset(new CRapidJsonMgr);
		}
	}

	return s_instance;
}

CRapidJsonMgr::CRapidJsonMgr()
{
}

CRapidJsonMgr::~CRapidJsonMgr()
{
}

char* CRapidJsonMgr::GetClientCfgJsonString()
{
	std::wstring pcid = RegistryConfigMgr::instance()->GetPcid();
	DWORD placeId = RegistryConfigMgr::instance()->GetPlaceId();
	std::wstring domain = RegistryConfigMgr::instance()->GetPlatDomain();
	std::wstring version = RegistryConfigMgr::instance()->GetVersion();
	bool auditType = RegistryConfigMgr::instance()->GetAuditType();
	g_logger->debug("[GetClientCfgJsonString] PCID: {}, PLACEID:{}, TRACKDOMAIN:{}, VERSION:{}, AUDITTYPE:{}", 
		WtoA(pcid).c_str(), placeId, WtoA(domain).c_str(), WtoA(version).c_str(), auditType);

	std::lock_guard<std::mutex> lock(s_mutex);
	rapidjson::Document document;
	document.SetObject();
	rapidjson::Document::AllocatorType& allocator = document.GetAllocator();

	//添加结果码和结果描述
	document.AddMember(rapidjson::StringRef("placeId"), (int)placeId, allocator);
	document.AddMember(rapidjson::StringRef("auditType"), (int)auditType, allocator);
	document.AddMember(rapidjson::StringRef("pcId"), rapidjson::Value().SetString(WtoA(pcid).c_str(), allocator).Move(), allocator);
	document.AddMember(rapidjson::StringRef("domain"), rapidjson::Value().SetString(WtoA(domain).c_str(), allocator).Move(), allocator);
	document.AddMember(rapidjson::StringRef("version"), rapidjson::Value().SetString(WtoA(version).c_str(), allocator).Move(), allocator);
	document.AddMember(rapidjson::StringRef("productId"), rapidjson::Value().SetString(GM_PRODUCE_ID.c_str(), allocator).Move(), allocator);

	//获取JSON字符串
	rapidjson::StringBuffer buffer;
	rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
	document.Accept(writer);
	m_jsonstr = AtoUTF8(buffer.GetString());
	return &m_jsonstr[0];
}

char* CRapidJsonMgr::GetAllDomain()
{
	m_jsonstr = RegistryConfigMgr::instance()->GetAllDomain();
	return &m_jsonstr[0];
}