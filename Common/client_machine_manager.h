#ifndef _CLIENT_MACHINE_MANAGER_H_
#define _CLIENT_MACHINE_MANAGER_H_

#include <time.h>

#include "public.h"

#include <shlwapi.h>

//两点说明:(我们这个仅仅是一个临时的测试程序,因此我们的目的是能用,编程简单,没怎么考虑程序效率问题)
//	1.这里使用map来保存客户机信息,map主关键字是客户机Ip,这个算法不是最优化的,但是是效率和实现简洁性的较好结合
//	2.这里我们使用的是临界区来互斥,其实为了效率这里应该用读写锁,为了编程简单点(反正我们这个是临时的内部测试代码,非正式上线程序),我们这里用来更简单的临界区

//客户机启动方式定义
typedef enum tagClientMachineStartType
{
	CLIENT_MACHINE_START_TYPE_OUR_DISKLESS,
	CLIENT_MACHINE_START_TYPE_OTHER_DISKLESS,
	CLIENT_MACHINE_START_TYPE_SSD_STARTUP,
}ClientMachineStartType, *PClientMachineStartType;

//客户机信息结构体中MAC字段含义
typedef enum tagClientMachineMacDefineType
{
	CLIENT_MACHINE_MAC_DEFINE_TYPE_HAS_SETTED,	//已经设置过MAC地址
	CLIENT_MACHINE_MAC_DEFINE_TYPE_TEMP_SETTED,	//临时设置的MAC地址(可能是某未设置过MAC地址的客户机在请求HDCP的时候我们临时设置的,这个不能回写到客户机列表里)
	CLIENT_MACHINE_MAC_DEFINE_TYPE_IS_NULL,		//未设置过MAC地址
}ClientMachineMacDefineType, *PClientMachineMacDefineType;


//客户机用户分组名称最大许可长度(单位:TCHAR)
#define CLIENT_USER_GROUP_MAX_NAME_LEN	32

//客户机信息结构体定义
typedef struct tagClientMachineCfgItem
{
	unsigned short		nClientId;		//客户机编号

	char				strClientMac[6];//客户机的MAC地址
	ClientMachineMacDefineType	eMacStateType;		//MAC地址当前的状态

	unsigned int		nClientIp;		//客户机的IP地址,网络字节序
	unsigned int		nClientMask;	//客户机的子网掩码,网络字节序
	unsigned int		nClientGate;	//客户机所在网络的网关,网络字节序
	unsigned int		nClientDns1;	//客户机的DNS1,网络字节序
	unsigned int		nClientDns2;	//客户机的DNS2,网络字节序

	unsigned int		nImgServerIp;	//镜像服务器IP地址,网络字节序,如果是0则表示根据负载均衡自动分配
	unsigned int		nGameServerIp;	//游戏服务器IP地址,网络字节序,如果是0则表示根据负载均衡自动分配

	ClientMachineStartType startupType;	//启动方式
	BOOL				bMbrBoot;		//客户机是否为MBR引导方式,否则为UEFI引导

	BOOL				bDisableTheClient;	//是否禁用这台机器,TRUE表示禁用,FALSE表示启用
	BOOL				bSSDBuildOs;		//是否开启了SSD施工功能(这个状态不保存到客户机列表信息里,在服务的内存里临时存在,无盘服务重启后这个状态被设置为FALSE,即默认SSD施工功能是关闭的)

	EnumClientMemCacheSizeInMBytes		nClientMemCacheSizeInMBytes;		//系统盘在客户机上最大许可MemCahce大小(单位:兆字节),CLIENT_MEM_CACHE_SIZE_IN_MBYTES__AUTOSET表示自动分配,CLIENT_MEM_CACHE_SIZE_IN_MBYTES__DISABLE表示禁用
	unsigned int		nClientMaxWritebackSizeInMBytes;	//客户机最大许可回写数据大小(单位:兆字节),超过此值客户机将重启,如为0则表示对回写不做任何限制

	unsigned int		nClientControlFlags;	//客户端控制信息,CLIENT_CONTROL_REMOVED_PAGE_FILE之类的宏的联合

	WCHAR				strUserGroup[CLIENT_USER_GROUP_MAX_NAME_LEN];	//用户分组
	WCHAR				strUserName[32];	//用户名
	WCHAR				strMachineName[32];	//机器名

	//顺次位的客户机使用的镜像名称
	WCHAR				strImgName1[32];
	WCHAR				strImgName2[32];
	WCHAR				strImgName3[32];
	WCHAR				strImgName4[32];
	WCHAR				strStartImgName[32];	// 启动镜像名	
	//显卡初始像素之类的(0表示默认)
	USHORT				nDefaultSettings_XResolution;	//水平分辨率
	USHORT				nDefaultSettings_YResolution;	//垂直分辨率
	USHORT				nDefaultSettings_VRefresh;
	USHORT				nDefaultSettings_BitsPerPel;

	time_t				tTempMacSetTime;
}ClientMachineCfgItem, *PClientMachineCfgItem;


//客户机列表文件名
#define CLIENT_MACHINE_XML_CFG_FILE_NAME	_T("client_machine.xml")

//客户机列表Hash数组大小
#define CLIENT_MACHINE_USER_ONE_BYTE_HASH_SIZE	256

//客户机XML配置文件关键字定义
//	client表示是一条客户机记录
//	id表示客户机编号,我们目前不以编号来标识一台机器,而是以mac地址来标识一台机器,但客户机的编号也一定不能有相同的啊
//	mac/ip/gate/mask/dns1/dns2表示客户机的mac地址、ip地址、网关、子网掩码和DNS配置
//	macok表示mac地址是否有效被设置,ok表示已设置,其它值未设置
//	clientdisable表示这台机器是否被禁用,on表示禁用,其它值(或者无此字段)表示使用这台机器
//	imgserverip/resourceserverip表示客户机对应的镜像服务、资源盘服务服务器ip地址,字符串为空表示根据负载均衡自动分配
//	startup表示客户机启动方式,diskless表示使用我们的无盘启动,other_diskless表示使用第三方无盘或有盘启动,ssd_startup表示SSD启动
//	boot表示客户机引导方式,mbr表示MBR引导,否则表示uefi引导
//	memcache表示客户机本地的内存缓存大小(单位：M)，如果是0表示不使用本地内存缓存功能
//	maxwb表示客户机最大可以回写的字节数量(单位：M)，如果是0表示不限制
//	controlflag是一个对客户机的控制字段,其含义暂时不表
//	x/y/refresh/bitsperpel表示客户机显示器的分辨率、刷新率和色深
//	imgname1/imgname2/imgname3/imgname4表示客户机要使用的镜像,空字符串表示不使用
//	groupname/username/machinename分别表示客户机的组名、用户名和机器名
#define CLIENT_MACHINE_XML_CFG_CLIENT				"client"
#define CLIENT_MACHINE_XML_CFG_ID					"id"
#define CLIENT_MACHINE_XML_CFG_MAC					"mac"
#define CLIENT_MACHINE_XML_CFG_MAC_OK				"macok"
	#define CLIENT_MACHINE_XML_CFG_MAC_OK_OK		"ok"			//mac已被设置
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
	#define CLIENT_MACHINE_XML_CFG_OUR_DISKLESS_STARTUP_TYPE	"diskless"			//我们自己的无盘启动
	#define CLIENT_MACHINE_XML_CFG_OTHER_DISKLESS_STARTUP_TYPE	"other_diskless"	//第三方无盘或有盘启动
	#define CLIENT_MACHINE_XML_CFG_SSD_STARTUP_TYPE				"ssd_startup"		//SSD启动
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
#define CLIENT_MACHINE_XML_CFG_START_IMG_NAME		"startimgname"   // 客户机最终的启动镜像名
#define CLIENT_MACHINE_XML_CFG_GROUP_NAME			"groupname"
#define CLIENT_MACHINE_XML_CFG_USER_NAME			"username"
#define CLIENT_MACHINE_XML_CFG_MACHINE_NAME			"machinename"


//功能：初始化客户机列表模块
//参数：无
//返回：成功TRUE,否则FALSE
BOOL init_client_machine_list_module();


//功能：销毁客户机列表模块
//参数：无
//返回：无
//说明：即使没有初始化成功调用本函数也是可以的
VOID destroy_client_machine_list_module();


//功能：回写客户机XML文件
//参数：无
//返回：成功TRUE,否则FALSE
BOOL write_back_client_machine_user_xml();


//功能：添加一个客户机信息
//参数：
//		pClientMachineCfgItem 要添加的客户机信息节点
//返回：成功TRUE,否则FALSE
BOOL add_one_client_machine_cfg_item(IN PClientMachineCfgItem pClientMachineCfgItem);


//功能：修改一条客户机信息
//参数：
//		nClientIp 被修改的客户机原来的ip地址,网络字节序
//		pClientMachineCfgItem 修改后的客户机信息节点
//返回：成功TRUE,否则FALSE
BOOL modify_one_client_machine_cfg_item_by_ip(IN unsigned int nClientIp, IN PClientMachineCfgItem pClientMachineCfgItem);


//功能：修改一条客户机信息(从TempMac到HasMac列表里)(先判定是不是TempMac)
//参数：
//		pClientMachineCfgItem 客户机信息节点
//返回：成功TRUE,否则FALSE
BOOL client_machine_move_temp_mac_to_true_mac(IN PClientMachineCfgItem pClientMachineCfgItem);


//功能：按批次获取客户机信息供服务发送给UI进程展示
//参数：
//		dwFromIndex 从这个Index开始获取客户机信息列表(从0开始计数)
//		pReturnItem 用来保存获取到的客户机信息
//		pReturnItemBufEnd 用来保存获取到的客户机信息的缓冲区的尾部(不含)
//		dwReturnItemCount 返回获取到客户机信息数量
//返回：还有未遍历的项目TRUE,否则FALSE(无法遍历也返回FALSE)
BOOL get_client_machine_list_by_batch(IN DWORD dwFromIndex, IN PClientMachineCfgItem pReturnItem, IN char *pReturnItemBufEnd, IN PDWORD dwReturnItemCount);
BOOL get_client_machine_list_by_batch(IN DWORD dwFromIndex, IN DWORD dwMaxSaveItemCount, IN PClientMachineCfgItem pReturnItem, IN PDWORD dwReturnItemCount); // add by lizhi 20191216

//功能：根据mac地址删除一个客户机信息
//参数：
//		strClientMac 要删除的客户机MAC地址
//返回：成功TRUE,否则FALSE
BOOL del_one_client_machine_cfg_item_by_mac(IN char *strClientMac);


//功能：根据ip地址删除一个客户机信息
//参数：
//		nClientIp 要删除的客户机ip地址(网络字节序)
//返回：成功TRUE,否则FALSE
BOOL del_one_client_machine_cfg_item_by_ip(IN unsigned int nClientIp);


//功能：根据mac地址查询客户信息
//参数：
//		strClientMac 客户机Mac地址
//		pCfgItem 如果找到则把配置信息写入此结构体中
//返回：成功TRUE,否则FALSE
BOOL query_client_machine_cfg_by_mac(IN char *strClientMac, OUT PClientMachineCfgItem pCfgItem OPTIONAL);


//功能：根据客户机Id查询客户信息
//参数：
//		nClientId 客户机编号
//		pCfgItem 如果找到则把配置信息写入此结构体中
//返回：成功TRUE,否则FALSE
BOOL query_client_machine_cfg_by_client_id(IN unsigned short nClientId, IN PClientMachineCfgItem pCfgItem);


//功能：根据客户机网络字节序ip地址查询客户信息
//参数：
//		nClientIp 客户机网络字节序ip地址
//		pCfgItem 如果找到则把配置信息写入此结构体中
//返回：成功TRUE,否则FALSE
BOOL query_client_machine_cfg_by_client_ip(IN unsigned int nClientIp, IN PClientMachineCfgItem pCfgItem);


//功能：查找一个未分配的IP地址(即没有对应MAC的IP地址)
//参数：
//		strClientMac 客户机Mac地址
//		pCfgItem 如果找到则把配置信息写入此结构体中
//返回：成功TRUE,否则FALSE
BOOL query_temp_client_machine_cfg(IN char *strClientMac, OUT PClientMachineCfgItem pCfgItem);


#endif	//_CLIENT_MACHINE_MANAGER_H_