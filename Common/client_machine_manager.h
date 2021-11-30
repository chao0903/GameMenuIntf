#ifndef _CLIENT_MACHINE_MANAGER_H_
#define _CLIENT_MACHINE_MANAGER_H_

#include <time.h>

#include "public.h"

#include <shlwapi.h>

//����˵��:(�������������һ����ʱ�Ĳ��Գ���,������ǵ�Ŀ��������,��̼�,û��ô���ǳ���Ч������)
//	1.����ʹ��map������ͻ�����Ϣ,map���ؼ����ǿͻ���Ip,����㷨�������Ż���,������Ч�ʺ�ʵ�ּ���ԵĽϺý��
//	2.��������ʹ�õ����ٽ���������,��ʵΪ��Ч������Ӧ���ö�д��,Ϊ�˱�̼򵥵�(���������������ʱ���ڲ����Դ���,����ʽ���߳���),���������������򵥵��ٽ���

//�ͻ���������ʽ����
typedef enum tagClientMachineStartType
{
	CLIENT_MACHINE_START_TYPE_OUR_DISKLESS,
	CLIENT_MACHINE_START_TYPE_OTHER_DISKLESS,
	CLIENT_MACHINE_START_TYPE_SSD_STARTUP,
}ClientMachineStartType, *PClientMachineStartType;

//�ͻ�����Ϣ�ṹ����MAC�ֶκ���
typedef enum tagClientMachineMacDefineType
{
	CLIENT_MACHINE_MAC_DEFINE_TYPE_HAS_SETTED,	//�Ѿ����ù�MAC��ַ
	CLIENT_MACHINE_MAC_DEFINE_TYPE_TEMP_SETTED,	//��ʱ���õ�MAC��ַ(������ĳδ���ù�MAC��ַ�Ŀͻ���������HDCP��ʱ��������ʱ���õ�,������ܻ�д���ͻ����б���)
	CLIENT_MACHINE_MAC_DEFINE_TYPE_IS_NULL,		//δ���ù�MAC��ַ
}ClientMachineMacDefineType, *PClientMachineMacDefineType;


//�ͻ����û��������������ɳ���(��λ:TCHAR)
#define CLIENT_USER_GROUP_MAX_NAME_LEN	32

//�ͻ�����Ϣ�ṹ�嶨��
typedef struct tagClientMachineCfgItem
{
	unsigned short		nClientId;		//�ͻ������

	char				strClientMac[6];//�ͻ�����MAC��ַ
	ClientMachineMacDefineType	eMacStateType;		//MAC��ַ��ǰ��״̬

	unsigned int		nClientIp;		//�ͻ�����IP��ַ,�����ֽ���
	unsigned int		nClientMask;	//�ͻ�������������,�����ֽ���
	unsigned int		nClientGate;	//�ͻ����������������,�����ֽ���
	unsigned int		nClientDns1;	//�ͻ�����DNS1,�����ֽ���
	unsigned int		nClientDns2;	//�ͻ�����DNS2,�����ֽ���

	unsigned int		nImgServerIp;	//���������IP��ַ,�����ֽ���,�����0���ʾ���ݸ��ؾ����Զ�����
	unsigned int		nGameServerIp;	//��Ϸ������IP��ַ,�����ֽ���,�����0���ʾ���ݸ��ؾ����Զ�����

	ClientMachineStartType startupType;	//������ʽ
	BOOL				bMbrBoot;		//�ͻ����Ƿ�ΪMBR������ʽ,����ΪUEFI����

	BOOL				bDisableTheClient;	//�Ƿ������̨����,TRUE��ʾ����,FALSE��ʾ����
	BOOL				bSSDBuildOs;		//�Ƿ�����SSDʩ������(���״̬�����浽�ͻ����б���Ϣ��,�ڷ�����ڴ�����ʱ����,���̷������������״̬������ΪFALSE,��Ĭ��SSDʩ�������ǹرյ�)

	EnumClientMemCacheSizeInMBytes		nClientMemCacheSizeInMBytes;		//ϵͳ���ڿͻ�����������MemCahce��С(��λ:���ֽ�),CLIENT_MEM_CACHE_SIZE_IN_MBYTES__AUTOSET��ʾ�Զ�����,CLIENT_MEM_CACHE_SIZE_IN_MBYTES__DISABLE��ʾ����
	unsigned int		nClientMaxWritebackSizeInMBytes;	//�ͻ��������ɻ�д���ݴ�С(��λ:���ֽ�),������ֵ�ͻ���������,��Ϊ0���ʾ�Ի�д�����κ�����

	unsigned int		nClientControlFlags;	//�ͻ��˿�����Ϣ,CLIENT_CONTROL_REMOVED_PAGE_FILE֮��ĺ������

	WCHAR				strUserGroup[CLIENT_USER_GROUP_MAX_NAME_LEN];	//�û�����
	WCHAR				strUserName[32];	//�û���
	WCHAR				strMachineName[32];	//������

	//˳��λ�Ŀͻ���ʹ�õľ�������
	WCHAR				strImgName1[32];
	WCHAR				strImgName2[32];
	WCHAR				strImgName3[32];
	WCHAR				strImgName4[32];
	WCHAR				strStartImgName[32];	// ����������	
	//�Կ���ʼ����֮���(0��ʾĬ��)
	USHORT				nDefaultSettings_XResolution;	//ˮƽ�ֱ���
	USHORT				nDefaultSettings_YResolution;	//��ֱ�ֱ���
	USHORT				nDefaultSettings_VRefresh;
	USHORT				nDefaultSettings_BitsPerPel;

	time_t				tTempMacSetTime;
}ClientMachineCfgItem, *PClientMachineCfgItem;


//�ͻ����б��ļ���
#define CLIENT_MACHINE_XML_CFG_FILE_NAME	_T("client_machine.xml")

//�ͻ����б�Hash�����С
#define CLIENT_MACHINE_USER_ONE_BYTE_HASH_SIZE	256

//�ͻ���XML�����ļ��ؼ��ֶ���
//	client��ʾ��һ���ͻ�����¼
//	id��ʾ�ͻ������,����Ŀǰ���Ա������ʶһ̨����,������mac��ַ����ʶһ̨����,���ͻ����ı��Ҳһ����������ͬ�İ�
//	mac/ip/gate/mask/dns1/dns2��ʾ�ͻ�����mac��ַ��ip��ַ�����ء����������DNS����
//	macok��ʾmac��ַ�Ƿ���Ч������,ok��ʾ������,����ֵδ����
//	clientdisable��ʾ��̨�����Ƿ񱻽���,on��ʾ����,����ֵ(�����޴��ֶ�)��ʾʹ����̨����
//	imgserverip/resourceserverip��ʾ�ͻ�����Ӧ�ľ��������Դ�̷��������ip��ַ,�ַ���Ϊ�ձ�ʾ���ݸ��ؾ����Զ�����
//	startup��ʾ�ͻ���������ʽ,diskless��ʾʹ�����ǵ���������,other_diskless��ʾʹ�õ��������̻���������,ssd_startup��ʾSSD����
//	boot��ʾ�ͻ���������ʽ,mbr��ʾMBR����,�����ʾuefi����
//	memcache��ʾ�ͻ������ص��ڴ滺���С(��λ��M)�������0��ʾ��ʹ�ñ����ڴ滺�湦��
//	maxwb��ʾ�ͻ��������Ի�д���ֽ�����(��λ��M)�������0��ʾ������
//	controlflag��һ���Կͻ����Ŀ����ֶ�,�京����ʱ����
//	x/y/refresh/bitsperpel��ʾ�ͻ�����ʾ���ķֱ��ʡ�ˢ���ʺ�ɫ��
//	imgname1/imgname2/imgname3/imgname4��ʾ�ͻ���Ҫʹ�õľ���,���ַ�����ʾ��ʹ��
//	groupname/username/machinename�ֱ��ʾ�ͻ������������û����ͻ�����
#define CLIENT_MACHINE_XML_CFG_CLIENT				"client"
#define CLIENT_MACHINE_XML_CFG_ID					"id"
#define CLIENT_MACHINE_XML_CFG_MAC					"mac"
#define CLIENT_MACHINE_XML_CFG_MAC_OK				"macok"
	#define CLIENT_MACHINE_XML_CFG_MAC_OK_OK		"ok"			//mac�ѱ�����
#define CLIENT_MACHINE_XML_CFG_DISABLE_THIS_CLIENT	"clientdisable"
	#define CLIENT_MACHINE_XML_CFG_DISABLE_THIS_CLIENT_ON	"on"
	#define CLIENT_MACHINE_XML_CFG_DISABLE_THIS_CLIENT_OFF	"off"
#define CLIENT_MACHINE_XML_CFG_IP					"ip"
#define CLIENT_MACHINE_XML_CFG_GATE					"gate"
#define CLIENT_MACHINE_XML_CFG_MASK					"mask"
#define CLIENT_MACHINE_XML_CFG_DNS1					"dns1"
#define CLIENT_MACHINE_XML_CFG_DNS2					"dns2"
#define CLIENT_MACHINE_XML_CFG_IMG_SERVER_IP		"imgserverip"
#define CLIENT_MACHINE_XML_CFG_GAME_SERVER_IP		"resourceserverip"
#define CLIENT_MACHINE_XML_CFG_STARTUP_TYPE			"startup"
	#define CLIENT_MACHINE_XML_CFG_OUR_DISKLESS_STARTUP_TYPE	"diskless"			//�����Լ�����������
	#define CLIENT_MACHINE_XML_CFG_OTHER_DISKLESS_STARTUP_TYPE	"other_diskless"	//���������̻���������
	#define CLIENT_MACHINE_XML_CFG_SSD_STARTUP_TYPE				"ssd_startup"		//SSD����
#define CLIENT_MACHINE_XML_CFG_BOOT					"boot"
	#define CLIENT_MACHINE_XML_CFG_MBR_BOOT			"mbr"
	#define CLIENT_MACHINE_XML_CFG_UEFI_BOOT		"uefi"
#define CLIENT_MACHINE_XML_CFG_MEM_CACHE			"memcache"
#define CLIENT_MACHINE_XML_CFG_MAX_WB				"maxwb"
#define CLIENT_MACHINE_XML_CFG_CONTROL_FLAG			"controlflag"
#define CLIENT_MACHINE_XML_CFG_X					"x"
#define CLIENT_MACHINE_XML_CFG_Y					"y"
#define CLIENT_MACHINE_XML_CFG_REFRESH				"refresh"
#define CLIENT_MACHINE_XML_CFG_BITS_PER_PEL			"bitsperpel"
#define CLIENT_MACHINE_XML_CFG_IMG1_NAME			"imgname1"
#define CLIENT_MACHINE_XML_CFG_IMG2_NAME			"imgname2"
#define CLIENT_MACHINE_XML_CFG_IMG3_NAME			"imgname3"
#define CLIENT_MACHINE_XML_CFG_IMG4_NAME			"imgname4"
#define CLIENT_MACHINE_XML_CFG_START_IMG_NAME		"startimgname"   // �ͻ������յ�����������
#define CLIENT_MACHINE_XML_CFG_GROUP_NAME			"groupname"
#define CLIENT_MACHINE_XML_CFG_USER_NAME			"username"
#define CLIENT_MACHINE_XML_CFG_MACHINE_NAME			"machinename"


//���ܣ���ʼ���ͻ����б�ģ��
//��������
//���أ��ɹ�TRUE,����FALSE
BOOL init_client_machine_list_module();


//���ܣ����ٿͻ����б�ģ��
//��������
//���أ���
//˵������ʹû�г�ʼ���ɹ����ñ�����Ҳ�ǿ��Ե�
VOID destroy_client_machine_list_module();


//���ܣ���д�ͻ���XML�ļ�
//��������
//���أ��ɹ�TRUE,����FALSE
BOOL write_back_client_machine_user_xml();


//���ܣ����һ���ͻ�����Ϣ
//������
//		pClientMachineCfgItem Ҫ��ӵĿͻ�����Ϣ�ڵ�
//���أ��ɹ�TRUE,����FALSE
BOOL add_one_client_machine_cfg_item(IN PClientMachineCfgItem pClientMachineCfgItem);


//���ܣ��޸�һ���ͻ�����Ϣ
//������
//		nClientIp ���޸ĵĿͻ���ԭ����ip��ַ,�����ֽ���
//		pClientMachineCfgItem �޸ĺ�Ŀͻ�����Ϣ�ڵ�
//���أ��ɹ�TRUE,����FALSE
BOOL modify_one_client_machine_cfg_item_by_ip(IN unsigned int nClientIp, IN PClientMachineCfgItem pClientMachineCfgItem);


//���ܣ��޸�һ���ͻ�����Ϣ(��TempMac��HasMac�б���)(���ж��ǲ���TempMac)
//������
//		pClientMachineCfgItem �ͻ�����Ϣ�ڵ�
//���أ��ɹ�TRUE,����FALSE
BOOL client_machine_move_temp_mac_to_true_mac(IN PClientMachineCfgItem pClientMachineCfgItem);


//���ܣ������λ�ȡ�ͻ�����Ϣ�������͸�UI����չʾ
//������
//		dwFromIndex �����Index��ʼ��ȡ�ͻ�����Ϣ�б�(��0��ʼ����)
//		pReturnItem ���������ȡ���Ŀͻ�����Ϣ
//		pReturnItemBufEnd ���������ȡ���Ŀͻ�����Ϣ�Ļ�������β��(����)
//		dwReturnItemCount ���ػ�ȡ���ͻ�����Ϣ����
//���أ�����δ��������ĿTRUE,����FALSE(�޷�����Ҳ����FALSE)
BOOL get_client_machine_list_by_batch(IN DWORD dwFromIndex, IN PClientMachineCfgItem pReturnItem, IN char *pReturnItemBufEnd, IN PDWORD dwReturnItemCount);
BOOL get_client_machine_list_by_batch(IN DWORD dwFromIndex, IN DWORD dwMaxSaveItemCount, IN PClientMachineCfgItem pReturnItem, IN PDWORD dwReturnItemCount); // add by lizhi 20191216

//���ܣ�����mac��ַɾ��һ���ͻ�����Ϣ
//������
//		strClientMac Ҫɾ���Ŀͻ���MAC��ַ
//���أ��ɹ�TRUE,����FALSE
BOOL del_one_client_machine_cfg_item_by_mac(IN char *strClientMac);


//���ܣ�����ip��ַɾ��һ���ͻ�����Ϣ
//������
//		nClientIp Ҫɾ���Ŀͻ���ip��ַ(�����ֽ���)
//���أ��ɹ�TRUE,����FALSE
BOOL del_one_client_machine_cfg_item_by_ip(IN unsigned int nClientIp);


//���ܣ�����mac��ַ��ѯ�ͻ���Ϣ
//������
//		strClientMac �ͻ���Mac��ַ
//		pCfgItem ����ҵ����������Ϣд��˽ṹ����
//���أ��ɹ�TRUE,����FALSE
BOOL query_client_machine_cfg_by_mac(IN char *strClientMac, OUT PClientMachineCfgItem pCfgItem OPTIONAL);


//���ܣ����ݿͻ���Id��ѯ�ͻ���Ϣ
//������
//		nClientId �ͻ������
//		pCfgItem ����ҵ����������Ϣд��˽ṹ����
//���أ��ɹ�TRUE,����FALSE
BOOL query_client_machine_cfg_by_client_id(IN unsigned short nClientId, IN PClientMachineCfgItem pCfgItem);


//���ܣ����ݿͻ��������ֽ���ip��ַ��ѯ�ͻ���Ϣ
//������
//		nClientIp �ͻ��������ֽ���ip��ַ
//		pCfgItem ����ҵ����������Ϣд��˽ṹ����
//���أ��ɹ�TRUE,����FALSE
BOOL query_client_machine_cfg_by_client_ip(IN unsigned int nClientIp, IN PClientMachineCfgItem pCfgItem);


//���ܣ�����һ��δ�����IP��ַ(��û�ж�ӦMAC��IP��ַ)
//������
//		strClientMac �ͻ���Mac��ַ
//		pCfgItem ����ҵ����������Ϣд��˽ṹ����
//���أ��ɹ�TRUE,����FALSE
BOOL query_temp_client_machine_cfg(IN char *strClientMac, OUT PClientMachineCfgItem pCfgItem);


#endif	//_CLIENT_MACHINE_MANAGER_H_