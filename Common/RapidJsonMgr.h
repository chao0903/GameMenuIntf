#pragma once

#include <string>
#include <memory>
#include <stdint.h>
#include <vector>
#include <mutex>
#include "WyGameServerMsgDef.h"


class CRapidJsonMgr
{
public:
	static std::shared_ptr<CRapidJsonMgr>& instance();
	~CRapidJsonMgr();

	typedef enum tagEnumResultCode
	{
		CODE_SUCCESS=0,		//成功
		CODE_FAILED=1001,	//失败
		CODE_NO_CONNECTED,	//服务器未链接
		CODE_INVALID,		//无效
	}EnumResultCode;

	typedef struct tagResultCode
	{
		tagResultCode(EnumResultCode _code)
		{
			code = _code;
			switch (_code)
			{
			case EnumResultCode::CODE_SUCCESS:
				desc = L"请求列表成功";
				break;
			case EnumResultCode::CODE_FAILED:
				desc = L"请求列表失败";
				break;
			case EnumResultCode::CODE_NO_CONNECTED:
				desc = L"服务器未连接";
				break;
			default:
				desc = L"";
				break;
			}
		}

		EnumResultCode code = CODE_INVALID;
		std::wstring desc = L"";
	}ResultCode;

protected:
	CRapidJsonMgr();

public:
	//获取客户机配置JSON字符串（包含：pcid, placeid, domain, 控制台版本）
	char* GetClientCfgJsonString();
	//获取域名列表
	char* GetAllDomain();

private:
	static std::string m_jsonstr;
};


