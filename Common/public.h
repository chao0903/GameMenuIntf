#ifndef _PUBLIC_H_
#define _PUBLIC_H_

#pragma pack(push, 1)


//服务器上某服务和备份服务器上某服务的ip/port结构体定义
typedef struct tagServerIpPortItem
{
	ULONG			nIp;					//提供此服务的服务器Ip,网络字节序ip
	USHORT			nPort;					//提供此服务的服务器Port,网络字节序,不能设置为0
	ULONG			nIp_Backup;				//提供此服务的备份服务器Ip,网络字节序ip,不使用的话必须设置成0
	USHORT			nPort_Backup;			//提供此服务的备份服务器Port,网络字节序,不使用的话必须设置成0
}ServerIpPortItem, *PServerIpPortItem;


//客户端控制信息宏定义
#define CLIENT_CONTROL_REMOVED_PAGE_FILE		0x00000001	//禁止在客户机上生成pagefile.sys文件
#define CLIENT_CONTROL_PNP_USB					0x00000002	//对USB设备使用Pnp
#define CLIENT_CONTROL_PNP_DISPLAY				0x00000004	//对显卡设备使用Pnp
#define CLIENT_CONTROL_PNP_MONITOR				0x00000008	//对显示器设备使用Pnp
#define CLIENT_CONTROL_PNP_DISABLE_HDMI_AUDIO	0x00000010	//在Pnp过程中禁用HDMI类声卡
#define CLIENT_CAN_INSTALL_NET_FILTER			0x00000020	//客户机是否运行安装网络(网卡)MiniFilter之类的驱动
#define CLIENT_FAST_SHUTDOWN					0x00000040	//客户机是否支持快速关机
#define CLIENT_SEND_BSOD_DUMP					0x00000080	//客户机如果蓝屏,是否上传蓝屏Dump文件
#define CLIENT_OPEN_PRE_READ					0x00000100	//客户机是否使用预读技术
#define CLIENT_CONTROL_ALL						0xffffffff	//启动所有

//网络盘工作模式
typedef enum tagNetDiskWorkMode
{
	ADMIN_NET_DISK = 1,		//管理员模式(即普通用户模式)
	SUPER_ADMIN_NET_DISK,	//超级管理员模式
}NetDiskWorkMode;

//客户机上MemCache大小定义
typedef enum tagEnumClientMemCacheSizeInMBytes
{
	CLIENT_MEM_CACHE_SIZE_IN_MBYTES__AUTOSET = -1,	//自动
	CLIENT_MEM_CACHE_SIZE_IN_MBYTES__DISABLE = 0,	//禁止
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

//如果客户机MemCache的值是非法值,则设置为自动
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

//BootLdr引导时,客户机向服务器请求的引导过程中的配置信息结构体
typedef struct tagBootLdrClientCfgItem
{
	char				strMagic[32];	//特殊用途,***请不要设置也不要篡改***

	unsigned int		nSize;			//此结构体的大小,回调函数内部不需要设置此值

	NetDiskWorkMode		nWorkMode;		//客户机工作模式(超级管理员模式或者普通模式)

	//下面4个字段是客户机有线网卡设备相关Id,在线Pnp使用(这是一个PCI设备的Id)
	//	这4个字段回调函数不必填充(但是回调函数里必须检测这个硬件Id对应的PNP配置是否存在)
	USHORT				nNetAdapterVenId;
	USHORT				nNetAdapterDevId;
	ULONG				nNetAdapterSubsysId;
	UCHAR				nNetAdapterRevId;

	//下面字段是提供客户机系统盘磁盘服务的ip/port
	ServerIpPortItem	osServerIpPort;	//提供客户机OS镜像服务的服务器Ip/port信息***此字段回调函数里可以不填充,也可以填充,BootLdr服务会填充一次***

	char				strClientMac[6];//客户机的MAC地址

	//无盘启动的时候,设置服务器上OS镜像上传服务的服务ip/port
	unsigned int		nUpOsServerIp;			//上传OS镜像服务器的IP地址,网络字节序
	USHORT				nUpOsServerPort;		//上传OS镜像服务器的Port,网络字节序,如果使用此功能这里不能设置为0

	//当需要向服务器发送客户机蓝屏Dump信息,定义服务器上的接收Dump数据的Ip和Port信息
	char				strDumpServerMac[6];	//收集客户机蓝屏Dump服务器的Mac地址
	unsigned int		nDumpServerIp;			//收集客户机蓝屏Dump的IP地址,网络字节序
	USHORT				nDumpServerPort;		//收集客户机蓝屏Dump的Port,网络字节序,如果使用此功能这里不能设置为0

	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	/////////我是分割线,回调函数里上面字段不用填充,但是下面字段必须填充///////
	//////////////////////////////////////////////////////////////////////////

	//***下面的字段在回调函数里必须填充,其它模块不会填充的***
	unsigned int		nClientIp;		//客户机的IP地址,网络字节序
	unsigned int		nClientMask;	//客户机的子网掩码,网络字节序
	unsigned int		nClientGate;	//客户机所在网络的网关,网络字节序
	unsigned int		nClientDns1;	//客户机的DNS1,网络字节序
	unsigned int		nClientDns2;	//客户机的DNS2,网络字节序

	EnumClientMemCacheSizeInMBytes		nClientMemCacheSizeInMBytes;		//系统盘在客户机上最大许可MemCahce大小(单位:兆字节)
	unsigned int		nClientMaxWritebackSizeInMBytes;	//客户机最大许可回写数据大小(单位:兆字节),超过此值客户机将重启,如为0则表示对回写不做任何限制

	unsigned int		nClientControlFlags;	//客户端控制信息,CLIENT_CONTROL_REMOVED_PAGE_FILE之类的宏的联合

	WCHAR				strUserName[32];	//用户名
	WCHAR				strMachineName[32];	//机器名

	//显卡初始像素之类的
	USHORT				nDefaultSettings_XResolution;	//水平分辨率
	USHORT				nDefaultSettings_YResolution;	//垂直分辨率
	USHORT				nDefaultSettings_VRefresh;
	USHORT				nDefaultSettings_BitsPerPel;
}BootLdrClientCfgItem, *PBootLdrClientCfgItem;

#pragma pack(pop)

#endif	//_PUBLIC_H_