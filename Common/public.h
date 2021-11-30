#ifndef _PUBLIC_H_
#define _PUBLIC_H_

#pragma pack(push, 1)


//��������ĳ����ͱ��ݷ�������ĳ�����ip/port�ṹ�嶨��
typedef struct tagServerIpPortItem
{
	ULONG			nIp;					//�ṩ�˷���ķ�����Ip,�����ֽ���ip
	USHORT			nPort;					//�ṩ�˷���ķ�����Port,�����ֽ���,��������Ϊ0
	ULONG			nIp_Backup;				//�ṩ�˷���ı��ݷ�����Ip,�����ֽ���ip,��ʹ�õĻ��������ó�0
	USHORT			nPort_Backup;			//�ṩ�˷���ı��ݷ�����Port,�����ֽ���,��ʹ�õĻ��������ó�0
}ServerIpPortItem, *PServerIpPortItem;


//�ͻ��˿�����Ϣ�궨��
#define CLIENT_CONTROL_REMOVED_PAGE_FILE		0x00000001	//��ֹ�ڿͻ���������pagefile.sys�ļ�
#define CLIENT_CONTROL_PNP_USB					0x00000002	//��USB�豸ʹ��Pnp
#define CLIENT_CONTROL_PNP_DISPLAY				0x00000004	//���Կ��豸ʹ��Pnp
#define CLIENT_CONTROL_PNP_MONITOR				0x00000008	//����ʾ���豸ʹ��Pnp
#define CLIENT_CONTROL_PNP_DISABLE_HDMI_AUDIO	0x00000010	//��Pnp�����н���HDMI������
#define CLIENT_CAN_INSTALL_NET_FILTER			0x00000020	//�ͻ����Ƿ����а�װ����(����)MiniFilter֮�������
#define CLIENT_FAST_SHUTDOWN					0x00000040	//�ͻ����Ƿ�֧�ֿ��ٹػ�
#define CLIENT_SEND_BSOD_DUMP					0x00000080	//�ͻ����������,�Ƿ��ϴ�����Dump�ļ�
#define CLIENT_OPEN_PRE_READ					0x00000100	//�ͻ����Ƿ�ʹ��Ԥ������
#define CLIENT_CONTROL_ALL						0xffffffff	//��������

//�����̹���ģʽ
typedef enum tagNetDiskWorkMode
{
	ADMIN_NET_DISK = 1,		//����Աģʽ(����ͨ�û�ģʽ)
	SUPER_ADMIN_NET_DISK,	//��������Աģʽ
}NetDiskWorkMode;

//�ͻ�����MemCache��С����
typedef enum tagEnumClientMemCacheSizeInMBytes
{
	CLIENT_MEM_CACHE_SIZE_IN_MBYTES__AUTOSET = -1,	//�Զ�
	CLIENT_MEM_CACHE_SIZE_IN_MBYTES__DISABLE = 0,	//��ֹ
	CLIENT_MEM_CACHE_SIZE_IN_MBYTES__64MB = 64,		//64MB
	CLIENT_MEM_CACHE_SIZE_IN_MBYTES__128MB = 128,	//128MB
	CLIENT_MEM_CACHE_SIZE_IN_MBYTES__256MB = 256,	//256MB
	CLIENT_MEM_CACHE_SIZE_IN_MBYTES__512MB = 512,	//512MB
	CLIENT_MEM_CACHE_SIZE_IN_MBYTES__768MB = 768,	//768MB
	CLIENT_MEM_CACHE_SIZE_IN_MBYTES__1024MB = 1024,	//1GB
	CLIENT_MEM_CACHE_SIZE_IN_MBYTES__1536MB = 1536,	//1.5GB
	CLIENT_MEM_CACHE_SIZE_IN_MBYTES__2048MB = 2048,	//2GB
	CLIENT_MEM_CACHE_SIZE_IN_MBYTES__3072MB = 3072,	//3GB
	CLIENT_MEM_CACHE_SIZE_IN_MBYTES__4096MB = 4096,	//4GB
	CLIENT_MEM_CACHE_SIZE_IN_MBYTES__5120MB = 5120,	//5GB
	CLIENT_MEM_CACHE_SIZE_IN_MBYTES__6144MB = 6144,	//6GB
	CLIENT_MEM_CACHE_SIZE_IN_MBYTES__7168MB = 7168,	//7GB
	CLIENT_MEM_CACHE_SIZE_IN_MBYTES__8192MB = 8192,	//8GB
}EnumClientMemCacheSizeInMBytes, *PEnumClientMemCacheSizeInMBytes;

//����ͻ���MemCache��ֵ�ǷǷ�ֵ,������Ϊ�Զ�
__inline EnumClientMemCacheSizeInMBytes check_and_return_right_client_mem_cache_size_in_mbytes(IN EnumClientMemCacheSizeInMBytes nClientMemCacheSizeInMBytes)
{
	EnumClientMemCacheSizeInMBytes nResult;

	if(CLIENT_MEM_CACHE_SIZE_IN_MBYTES__AUTOSET == nClientMemCacheSizeInMBytes || \
		CLIENT_MEM_CACHE_SIZE_IN_MBYTES__DISABLE == nClientMemCacheSizeInMBytes || \
		CLIENT_MEM_CACHE_SIZE_IN_MBYTES__64MB == nClientMemCacheSizeInMBytes || \
		CLIENT_MEM_CACHE_SIZE_IN_MBYTES__128MB == nClientMemCacheSizeInMBytes || \
		CLIENT_MEM_CACHE_SIZE_IN_MBYTES__256MB == nClientMemCacheSizeInMBytes || \
		CLIENT_MEM_CACHE_SIZE_IN_MBYTES__512MB == nClientMemCacheSizeInMBytes || \
		CLIENT_MEM_CACHE_SIZE_IN_MBYTES__768MB == nClientMemCacheSizeInMBytes || \
		CLIENT_MEM_CACHE_SIZE_IN_MBYTES__1024MB == nClientMemCacheSizeInMBytes || \
		CLIENT_MEM_CACHE_SIZE_IN_MBYTES__1536MB == nClientMemCacheSizeInMBytes || \
		CLIENT_MEM_CACHE_SIZE_IN_MBYTES__2048MB == nClientMemCacheSizeInMBytes || \
		CLIENT_MEM_CACHE_SIZE_IN_MBYTES__3072MB == nClientMemCacheSizeInMBytes || \
		CLIENT_MEM_CACHE_SIZE_IN_MBYTES__4096MB == nClientMemCacheSizeInMBytes || \
		CLIENT_MEM_CACHE_SIZE_IN_MBYTES__5120MB == nClientMemCacheSizeInMBytes || \
		CLIENT_MEM_CACHE_SIZE_IN_MBYTES__6144MB == nClientMemCacheSizeInMBytes || \
		CLIENT_MEM_CACHE_SIZE_IN_MBYTES__7168MB == nClientMemCacheSizeInMBytes || \
		CLIENT_MEM_CACHE_SIZE_IN_MBYTES__8192MB == nClientMemCacheSizeInMBytes)
	{
		nResult = nClientMemCacheSizeInMBytes;
	}
	else
	{
		nResult = CLIENT_MEM_CACHE_SIZE_IN_MBYTES__AUTOSET;
	}

	return nResult;
}

//BootLdr����ʱ,�ͻ������������������������е�������Ϣ�ṹ��
typedef struct tagBootLdrClientCfgItem
{
	char				strMagic[32];	//������;,***�벻Ҫ����Ҳ��Ҫ�۸�***

	unsigned int		nSize;			//�˽ṹ��Ĵ�С,�ص������ڲ�����Ҫ���ô�ֵ

	NetDiskWorkMode		nWorkMode;		//�ͻ�������ģʽ(��������Աģʽ������ͨģʽ)

	//����4���ֶ��ǿͻ������������豸���Id,����Pnpʹ��(����һ��PCI�豸��Id)
	//	��4���ֶλص������������(���ǻص���������������Ӳ��Id��Ӧ��PNP�����Ƿ����)
	USHORT				nNetAdapterVenId;
	USHORT				nNetAdapterDevId;
	ULONG				nNetAdapterSubsysId;
	UCHAR				nNetAdapterRevId;

	//�����ֶ����ṩ�ͻ���ϵͳ�̴��̷����ip/port
	ServerIpPortItem	osServerIpPort;	//�ṩ�ͻ���OS�������ķ�����Ip/port��Ϣ***���ֶλص���������Բ����,Ҳ�������,BootLdr��������һ��***

	char				strClientMac[6];//�ͻ�����MAC��ַ

	//����������ʱ��,���÷�������OS�����ϴ�����ķ���ip/port
	unsigned int		nUpOsServerIp;			//�ϴ�OS�����������IP��ַ,�����ֽ���
	USHORT				nUpOsServerPort;		//�ϴ�OS�����������Port,�����ֽ���,���ʹ�ô˹������ﲻ������Ϊ0

	//����Ҫ����������Ϳͻ�������Dump��Ϣ,����������ϵĽ���Dump���ݵ�Ip��Port��Ϣ
	char				strDumpServerMac[6];	//�ռ��ͻ�������Dump��������Mac��ַ
	unsigned int		nDumpServerIp;			//�ռ��ͻ�������Dump��IP��ַ,�����ֽ���
	USHORT				nDumpServerPort;		//�ռ��ͻ�������Dump��Port,�����ֽ���,���ʹ�ô˹������ﲻ������Ϊ0

	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	/////////���Ƿָ���,�ص������������ֶβ������,���������ֶα������///////
	//////////////////////////////////////////////////////////////////////////

	//***������ֶ��ڻص�������������,����ģ�鲻������***
	unsigned int		nClientIp;		//�ͻ�����IP��ַ,�����ֽ���
	unsigned int		nClientMask;	//�ͻ�������������,�����ֽ���
	unsigned int		nClientGate;	//�ͻ����������������,�����ֽ���
	unsigned int		nClientDns1;	//�ͻ�����DNS1,�����ֽ���
	unsigned int		nClientDns2;	//�ͻ�����DNS2,�����ֽ���

	EnumClientMemCacheSizeInMBytes		nClientMemCacheSizeInMBytes;		//ϵͳ���ڿͻ�����������MemCahce��С(��λ:���ֽ�)
	unsigned int		nClientMaxWritebackSizeInMBytes;	//�ͻ��������ɻ�д���ݴ�С(��λ:���ֽ�),������ֵ�ͻ���������,��Ϊ0���ʾ�Ի�д�����κ�����

	unsigned int		nClientControlFlags;	//�ͻ��˿�����Ϣ,CLIENT_CONTROL_REMOVED_PAGE_FILE֮��ĺ������

	WCHAR				strUserName[32];	//�û���
	WCHAR				strMachineName[32];	//������

	//�Կ���ʼ����֮���
	USHORT				nDefaultSettings_XResolution;	//ˮƽ�ֱ���
	USHORT				nDefaultSettings_YResolution;	//��ֱ�ֱ���
	USHORT				nDefaultSettings_VRefresh;
	USHORT				nDefaultSettings_BitsPerPel;
}BootLdrClientCfgItem, *PBootLdrClientCfgItem;

#pragma pack(pop)

#endif	//_PUBLIC_H_