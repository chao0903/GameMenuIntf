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
		CODE_SUCCESS=0,		//�ɹ�
		CODE_FAILED=1001,	//ʧ��
		CODE_NO_CONNECTED,	//������δ����
		CODE_INVALID,		//��Ч
	}EnumResultCode;

	typedef struct tagResultCode
	{
		tagResultCode(EnumResultCode _code)
		{
			code = _code;
			switch (_code)
			{
			case EnumResultCode::CODE_SUCCESS:
				desc = L"�����б�ɹ�";
				break;
			case EnumResultCode::CODE_FAILED:
				desc = L"�����б�ʧ��";
				break;
			case EnumResultCode::CODE_NO_CONNECTED:
				desc = L"������δ����";
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
	//��ȡ�ͻ�������JSON�ַ�����������pcid, placeid, domain, ����̨�汾��
	char* GetClientCfgJsonString();
	//��ȡ�����б�
	char* GetAllDomain();

private:
	static std::string m_jsonstr;
};


