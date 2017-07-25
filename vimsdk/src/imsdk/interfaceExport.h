/**
 * date: 2015-11-11 ToT~~~
 * author: sharp
 * ＳＤＫ外围插件接口
 */
#ifndef IMSDK_INTERFACEEXPORT
#define IMSDK_INTERFACEEXPORT

#include "config.h"


#ifdef _WIN32
#include <Winsock2.h>
#include <Ws2tcpip.h>
#pragma comment(lib,"Ws2_32.lib")
#pragma comment(lib,"advapi32.lib")
#endif

#include <string>
#ifndef Interface
#define Interface  struct
#endif

///状态
enum  enNetStat {
	enetState_noNet ,      ///< 没有网络连接
	enetState_noLogin,     ///< 没有登录,但是网络联通
	enetState_loging,      ///< 正在登录
	enetState_ReLogin,     ///< 断线重连状态
	enetState_Worker,      ///< 正常状态
};

Interface IPluginSlot;  ///< 插件管理接口
Interface IPlugin ;     ///< 插件接口
Interface ICacheMgr ;   ///< 缓存接口

/**
 * \brief 缓存接口
*/
Interface ICacheMgr  
{
	enum  dataType
	{
		data_user,		  ///< 返回自己的信息 st_userBean
		data_reContact , ///< 最近回话 std::vector<st_reContact>
		data_buddy,		 ///< 根据id获取好友信息st_tinyBuddyBean extra为好友id
		data_buddys,     ///< 好友 std::vector<st_tinyBuddyBean>
		data_group,		 ///< 根据id获取好友信息st_tinyGroupBean extra为群id
		data_groups,     ///< 群 std::vector<st_tinyGroupBean>
		data_groupMem, ///< 群成员是否存在 pOutData为 , extra为std::vecotr<int64>, 分别存放groupId,和memId
		data_P2pUsers,///<获取p2p用户
		data_getFileInfo, ///<获取文件信息
		data_downLoadFileInfo,///<获取下载文件的大小信息
		data_localSetting, ///<  用户本地数据设置与获取
		data_getLoginTicket,  ///<获取登录票据
		data_getPrivateKey,	 ///<获取数据库加密秘钥
		data_loginfo,        ///<获取登录信息
		data_getPostContact,    ///<获取本地通讯录
		data_addPostContact,	///<增加本地通讯录
		data_delPostContact,	///<删除本地通讯录
		data_updatePostContact,	///<更新本地通讯录
	};
	public:
	    virtual bool getCacheData(dataType type , void * pOutData, void *extra = NULL) = 0;
};

namespace imsdk
{
	class IDataMgr;
}

struct st_msgRange {
	uint16  min; ///< 最大消息
	uint16  max; ///< 最小消息
	///< 是否相交
	bool   isCut(st_msgRange & tmp) {
		return !((max < tmp.min) || (min > tmp.max)) ;
	}
	///< 是否包含
	bool   isClude(uint16 & cmd) {
		return ((cmd >= min) && (cmd <= max));
	}
};


/**
 * \brief 插件管理接口,该接口可以通过getConfig获取
 */
Interface IPluginSlot  {
	/**
	 * \brief 设置插件
	 */
	virtual bool     	regPlugin(IPlugin * plugin) = 0 ;

    /**
     * \brief 注销插件
     */
    virtual bool     	unregPlugin(IPlugin * plugin) = 0;

	/**
	 * \brief 获取插件接口
	 */
	virtual IPlugin * 		getPlugin(std::string & name) = 0 ;
	/**
	 * \brief 获取最大内部消息ID, 如果新加的消息号,网络消息也可在此基础上增加,在此基础上增加
	 */
	virtual uint16   	getMaxCmd() = 0 ;

	/**
	 * \brief 获取SessionID
	 */
	virtual int64    	getSessionID() = 0 ;

	/**
	 * \brief 获取ticket
	 */
	virtual std::string getTicket() = 0 ;

    /**
     * \brief 获取ticket
     */
    virtual int64 getMyselfID() = 0;

	/**
	 *  \brief 获取网络状态
	 */
	virtual enNetStat 	getNetStat() = 0 ;

	/**
	 * \brief 获取登录地址
	 */
	virtual void       getLoginAddress(std::string & ip , uint16 & port)=0;
	/**
	 * \brief 获取数据库管理指针
	 */
	virtual imsdk::IDataMgr  * getIDataMgr() = 0;

	/**
	 * \brief   发送消息到Reponse处理通道
	 * @param1  信令号
	 * @param2  携带数据
	 * @param3  数据长度
	 * @param4  是否同步
	 * @param5  附加参数，暂时不用
	 * @return  调用成功异步值返回非灵消息号,同步返回1. 失败都返回0
	 */
	virtual   uint32     sendtoReponse(int16 cmd , void * pData, int32 len , bool bsync = false) = 0;

	/**
	 * \brief 发送消息到耗时操作处理通道,
	 * 目的是不堵塞住request通道
	 */
	virtual   uint32     sendtoOther(int16 cmd , void * pData, int32 len , bool bsync = false) = 0;

	/**
	 *\brief  发送消息到耗时操作处理通道,
	 * 目的是不堵塞住request通道
	 */
	virtual   uint32     sendtoDb(int32 idx,int16 cmd , void * pData, int32 len , bool bsync = false) = 0;

	/**
	 * \brief 发送消息到耗时操作处理通道,
	 * 目的是不堵塞住request通道
	 */
	virtual   uint32     sendtoNet(int16 cmd , void * pData, int32 len , bool bsync = false) = 0;

	/**
	 *\brief   @interval 间隔，毫秒
	 * @bloop 是否循环
	 */
	virtual   int16      setTimer(int32 interval, bool bloop = true) = 0;
	virtual   void       killTimer(int16 timerID) = 0;

    virtual uint32       sendtoProgress(int64 id, int64 dlnow) = 0;

	/**
	 * \brief 发送到网络,调用ＡＰ接口.
	 */
	virtual   bool       sendToServer(uint16 cmd , void * pIn , void * pOut) = 0 ;
	/**
	 * \brief 记录日志
	 */
	virtual   void       loglog(enlogLvl lvl, enLogModel model, const char * tag, const char * fmt, ...) = 0;
    /**
	 * \brief 获取业务数据结构，
	 * @msg  消息类型　cmdExport.h里面有定义
	 * @bisReq 是否是该消息的请求数据结构　 true为请求结构，false为响应结构
	 * ＠return 指向相应结构体的指针
	 */
	virtual void     *       	getCommonCmdData(uint16 msg , bool bisReq = true) = 0;
	/**
	 * \brief 使用完毕，回收数据结构
	 * @msg 同getCmdData
	 * ＠bisReq　同getCmdData
	 * ＠pData　getCmdData返回值
	 * ＠returu 　true是成功，false失败
	 */
	virtual bool               freeCommonCmdata(uint16 msg , void * pData ,bool bisReq = true)=0;

    ///获取插件接口数据结构
	virtual  void   *          getPluginCmdData(uint16 msg,bool bisReq=true) = 0 ;

	///释放插件数据结构体
	virtual  bool              freePluginData(uint16 msg , void * pData,bool bisReq = true)=0;

	///语言可以混编的平台，转换临时结构体到动态内存申请结构体,比如PC，IOS
	virtual   void   *         transData(uint16 cmd,void * pData , int len) = 0 ;

	///安卓请求转换
	virtual   void   *         trans_reqAndroid(void * pEnv , uint16 msg , void * jobj) = 0;
	///安卓响应转换
	virtual   void   *         trans_respAndroid(void * pEnv , void * jobj, uint16 msg, void * pData,bool sync = false) = 0;

    /**
     * \brief 获取根目录地址
     */
    virtual std::string   		getRootPath() = 0;

    /**
     * \brief 获取DB目录地址
     */
    virtual std::string         getDbPath() = 0;

    /**
     * \brief 获取DB key
     */
    virtual std::string         getDbKey() = 0;

    virtual std::string         getappName() = 0;

    /// 插件数据库是否需要升级
    virtual bool          		isNeedUpdataPluginDb() = 0;

    /// 插件数据库升级
    virtual void          		updataPluginDb() = 0;

    /// 初始化数据库
    virtual void          		initPluginDb(bool enCryptDb) = 0;

    /// 反初始化数据库
    virtual void          		unInitPluginDb() = 0;
	
    /// 获取ID
    virtual   int32       		getClientID() = 0;

    /// 释放缓存
    virtual   void       		unInitPluginCache() = 0;
    /**
     * \brief 增加数据库通道，返回值作为sendtoDb第一个参数
     */
	virtual   int32             addDbChannle(IPlugin * Plugin) = 0;
	///获取私信密钥,空为不存在
	virtual   char *            getPrivatePw(int64 targetID) = 0;
};

/**
 * \brief 插件接口
 */
Interface IPlugin {
	/**
	 * 设置插件槽指针
	 * 执行插件初始化等工作可以在这里进行，比如注册数据库通
	 */
	virtual void     setPlugSlot(IPluginSlot * pSlot) = 0;
	/**
	 * \brief 获取证书内容
	 * @buffer 存放证书的缓存
	 * @len buffer的长度, 调用完毕, 返回证书的实际长度
	 * @return true为成功,false为失败
	 */
	virtual  bool    getCert( uint8 * buffer ,uint16 & len ) = 0 ;

	/**
	 * \brief 获取插件名称
	 */
	virtual  std::string  getName() = 0 ;
	/**
	 * \brief 请求响应
	 * @param1   消息号
	 * @param2   消息携带数据
	 * @param3   消息长度
	 * @return   true为消息不在向下执行,false为消息会到下层执行
	 */
	virtual   bool   onRequest(uint16 msg , void * pData , uint32 len) = 0 ;

	/**
	 * \brief notify响应
	 * @param1   消息号
	 * @param2   消息携带数据
	 * @param3   消息长度
	 * @return   true为消息不在向下执行,false为消息会到下层执行
	 */
	virtual   bool   onNotify(uint16 msg , void * pData , uint32 len) = 0 ;

	/**
	 * \brief 数据库操作
	 * 同上
	 * @return   true为消息不在向下执行,false为消息会到下层执行
	 */
	virtual   bool   onDb(uint8 idx,uint16 msg , void * pData , uint32 len) = 0;

	/**
	 * \brief 耗时操作
	 * 同上
	 * @return   true为消息不在向下执行,false为消息会到下层执行
	 */
	virtual   bool   onOther(uint16 msg , void * pData , uint32 len) = 0;

	/**
	 * \brief 网络操作
	 * 同上
	 */
	virtual   bool   onNetRequest(uint16 msg , void * pData , uint32 len) = 0 ;

	/**
	 * \brief 定时器响应
	 * @timeID   为定时器ID
	 * @return   true为消息不在向下执行,false为消息会到下层执行
	 */
	virtual   bool   onTimer(int16 timeID) = 0;

	/**
	 * \brief reponse操作前
	 */
	virtual   void   onpreResponse(uint16 msg, void * pData , uint32 len) = 0 ;

	/**
	 * \brief reponse操作后, plugin特有的消息内存可以在此释放
	 */
	virtual   void   onpostRespnse(uint16 msg, void * pData , uint32 len) = 0;

 	/**
     * \brief request之后
     */
    virtual   void   onpostServer(uint16 msg, void * pData, uint32 len) = 0;

	/**
	 * \brief 发送到网络响应
	 * 调用IPluginSlot::sendToServer会响应到这里
	 */
	virtual   bool   onSendServer(uint16 cmd , void * pIn , void * pOut) = 0 ;

 	///语言可以混编的平台，转换临时结构体到动态内存申请结构体,比如PC，IOS
	virtual   void   *  transData(uint16 cmd,void * pData , int len) = 0 ;

	/**
	 * \brief 获取该消息号对应的内存
	 */
	virtual   void *    getCmdData(int16 cmd,bool bisReq=true)=0;
	
	///释放transData　返回的数据
	virtual   void      freeData(uint16 cmd,void * pData,bool bisReq=true) = 0;

    ///插件数据库是否需要升级
    virtual   bool      isNeedUpdataPluginDb() = 0;

    ///数据库升级
    virtual   bool      updataPluginDb() = 0;
	
	///初始化数据库
    virtual   bool      initPluginDb(bool enCryptDb) = 0;

    ///反初始化数据库
    virtual   bool      unInitPluginDb() = 0;
	
    ///获取消息区间
    virtual   void      getCmdRange(st_msgRange & range) = 0;
	
	///安卓请求转换
	virtual   void   *  trans_reqAndroid(void * pEnv , uint16 msg , void * jobj) = 0;
	
	///安卓响应转换
	virtual   void   *  trans_respAndroid(void * pEnv , void * jobj, uint16 msg, void * pData,bool bsync=false) = 0;

    ///释放插件中的缓存
    virtual   void      unInitPluginCache() = 0;
};

/**
 * name 为不同插件的名称
 */
#define   CREATE_PLUGIN(name) \
		IPlugin * name##_createPlugin()
#define   RELEASE_PLUGIN(name) \
		void name##_releasePlugin(IPlugin * plugin)

#define  CALL_CREATE_PLUGIN(name) \
        name##_createPlugin()
#define  CALL_RELEASE_PLUGIN(name, plugin) \
		name##_releasePlugin(plugin)


/*******************数据管理类，通过此类可以得到逻辑层需要的各种用户本地数据*****************/

namespace imsdk
{
class dbUtils;
class DataStore;


typedef DataStore* DataStorePtr;

///定义数据存储类型
enum eStoreType
{
	eDatabase,		///< 数据库
	eNone			///< 不使用存储
};

///存储文件的句柄(暂时未数据库句柄，目前有四个数据库)
enum eStoreHandle
{
	eUserHandle,
	eGroupMemHandle,
	eMsgHanle,
	eBackMsgHandle
};

///存储的数据分类
enum eDataType
{
	eBuddy,				///< 好友
	eEnterprise,		///< 企业
	eEntUser,			///< 企业成员
	eGroup,				///< 群组
	eGroupMember,		///< 群成员
	eGroupFile,			///< 群文件表
	eGroupMemTsp,		///< 群成员时间戳
	eFileInfo,			///< 文件信息
	eFileTsp,			///< 文件时间戳
	eLocalSetting,		///< 本地设置
	eSysMsg,			///< 系统消息
	eMessage,			///< 消息
	eMsgHelper,         ///< 消息已读帮助
	eMessageBack,		///< 备份消息
	eOrg,				///< 组织
	eOrgtimestamp,		///< 组织时间戳
	ePushMsg,			///< 推送消息
	eRecontact,			///< 最近联系人
	eServer,			///< 服务器
	eStranger,			///< 陌生人
	eTimestamp,			///< 时间戳
	eUser,				///< 用户
	eSysConfig,			///< 系统配置
	eExtendedField,		///< 扩展字段
	eAppInfo,           ///< 机器人应用信息
	eEmoticon,          ///< 自定义表情信息
	eLocalPostContact,	///< 本地通讯录
	etableCount,        ///< 表的数目
	/**
	* \brief 初始化，初始化主要做下面几件事
	*	1. 打开数据库连接
	*/
	einit,
	///读取数据到缓存
	einitCache,
	/*
	* \brief 反初始化，关闭数据库连接
	*/
	euninit,
	///数据库升级 ,都是同步执行
	edbUpdatePatch,
	eMax
};

///定义数据操作的方法类型
enum eFunctType
{
	/**************************通用方法*******************************/

	eGetData,			///< 得到一条数据
	eGetDatas,			///< 根据条件得到一组数据
	eGetDataBySql,		///< 根据sql语句得到一组数据
	eGetAllDatas,		///< 得到全部数据
    eGetMore,
	eUpdateData,		///< 更新数据
	eUpdateDatas,		///< 批量更新数据
	eInsertData,		///< 插入数据
	eInsertDatas,		///< 批量插入数据
	eSetData,			///< 设置数据
	eSetDatas,			///< 批量设置数据
	eDeleteData,		///< 根据数据结构删除单条数据
	eDeleteDatas,		///< 根据数据结构批量删除数据
	eDeleteDataBySql,   ///< 根据sql语句删除一组数据
	eDeleteAll,         ///< 删除所有
    eIsExist,           ///< 是否存在
    eSearch,            ///< 搜索

	/**************************系统消息相关*******************************/

	/**
	* \brief 得到最后一条系统消息
	* [data]  st_dbSysMsgBean *msg
	* [extra] NULL
	*/
	eGetLastSysMsg,
	/**
	* \brief 得到某一类系统消息最近接收的消息ID
	* [data]  int64 *msgId
	* [extra] int16 *msgType
	*/
	eGetLastSysMsgId,
	/**
	* \brief 得到最后一条系统消息
	* [data]  vector<st_dbSysMsgBean>*
	* [extra] vector<void*>*. 其中第一个元素为vector<int64> *msgIds, 第二个元素为int16 *msgType;
	*/
	eSetSysMsgRead,
	/**
	* \brief 设置系统消息的操作类型
	* [data]  int8 *operaType
	* [extra] vector<int64>*. 其中第一个元素为消息ID,第二个元素为消息类型
	*/
	eSetSysMsgOpera,
	/**
	* \brief 设置最后一条系统消息为未读
	* [data] NULL
	* [extra] NULL
	*/
	eSetLastSysMsgUnRead,

	/**************************文件*******************************/

	/**
	* \brief 通过文件的发送者或接受者ID的到文件的消息ID
	* [data]  vector<int64> *msgIds(得到的消息ID集合)
	* [extra] int64 targetID(发送者或接受者Id)
	*/
	eGetFileIdByTarget,

	/**************************时间戳*******************************/

	/**
	* \brief 对比stampName的时间戳与timestamp是否相同，相同返回true
	* [data]  string *stampName(本地时间戳的名字)
	* [extra] int64 *timestamp(对比的时间戳)
	*/
	eCmpTimestamp,

	/**************************聊天消息*******************************/

	/**
	* \brief  得到发送成功的最大消息ID
	* [data]  空
	* [extra] 空
	*/
	eGetMaxMsgId,
	/**
	* \brief  得到发送成功的最大消息ID
	* [data]  st_dbMessageBean*
	* [extra] 空
	*/
	eGetLastMsg,
	/**
	* \brief  得到不是某位用户发送的发送成功的最大消息ID
	* [data]  st_dbMessageBean*
	* [extra] int64 myselfID
	*/
	eGetNotUserIDLastMsg,
	/**
	* \brief  根据msgId更新之前未发送成功的消息
	* [data]  st_dbMessageBean*
	* [extra] int64 *oldId
	*/
	eUpdateMsgId,
	/**
	* \brief  根据消息Id更新消息类型
	* [data]  int32 *type
	* [extra] int64 *msgId
	*/
	eUpdateMsgType,
	/**
	* \brief  获取可设置最大已读消息ID
	* [data]  int64 *maxId
	* [extra] int64 *baseId
	*/
	eGetMaxCansetReadedId,
	/**
	* \brief 获取本地未读数量
	* [data]  int32 *msgCount
	* [extra] vector<int64 myUserId, int64 minMsgId, int64 maxMsgId>*
	*/
	eGetMsgCount,
	/**
	* \brief 获取实际的本地未读数量
	* [data]  int32 *msgCount
	* [extra] vector<int64 minMsgId, int64 maxMsgId>*
	*/
	eGetRealUnReadCnt,
	/**
	* \brief 批量获取本地未读数量
	* [data]  int32 *msgCount
	* [extra] vector<int64 myUserId, int64 minMsgId, int64 maxMsgId>*
	*/
	eGetMsgCounts,
	/**
	* \brief 获取本地未读消息数量
	* [data]  int32 *msgCount
	* [extra] 空
	*/
	eGetUnReadMsgCount,
	/**
	* \brief 通过条件语句得到消息数量
	* [data]
	* [extra]
	*/
	eGetMsgCountByCondition,
	/**
	* \brief 通过类型的到聊天消息
	* [data]  int32 *msgCount
	* [extra] vector<int64 myUserId, int64 minMsgId, int64 maxMsgId>*
	*/
	eGetMsgByType,
	/**
	* \brief 搜索本地消息，只返回对应targetId的消息的数目
	* [data]  vector<st_msgGlobalSearch>* msgs
	* [extra] vector<void*>*，元素依次为:string* key, int64 *targetId, int count, int64 startTime, int64 endTime.
	*/
	eMsgGlobalSearch,
	/**
	* \brief 得到对应targetId的详细聊天消息
	* [data]  vector<void*>*，元素依次为:vector<st_msgBean> *msgs, map<int64, st_simpleSearchInfo> *senderInfos
	* [extra] vector<void*>*，元素依次为:string* key, int64 *targetId, int *count, int64 *startTime, int64 *endTime.
	*/
	eMsgDetailSearch,

	/**
	* \brief 更新解密后的消息，
	* [data]  vector<void*>*,元素依次为: vecotr<std::string>* message, vecotr<std::string>*msgProperty
	* [extra] vector<int64>* msgIds
	*/
	eUpdateDecryptMsg,

    eGetMsgCountTopN,

    eGetMessageByID,

	/**************************配置消息*******************************/
	eIsFindSysConfig,

	/**************************陌生人*******************************/
	eGetIdByAcountName,
	///更新群成员类型
	eupdateGMBType,
	///根据消息更新最近联系人
	updateDatafromMsg,
	///获取文件列表
	egetFileList,
	///消息表localID是否存在
	elocalIDExsit,
	///android设置已读为未读用，更新最大已读消息ID
	eupdateMaxReadID,
	///更新所有msgHelper表，设置maxCansetReaded等于maxMsgID
	eupdateAllMaxCansetReaded,

	/***************************群成员**************************/
	//删除页群成员
	eDeletePageGroupMember,
};

///定义可检索的数据类型
enum eKeyType
{
	eKeyUserId,			///< 用户Id
	eKeyGroupId,		///< 群Id
	eKeyGrpCreaterId,	///< 群创建者Id
	eKeyEntId,			///< 企业Id
	eKeyOrgId,			///< 组织Id
	eKeyMsgId,			///< 消息Id
	eKeyMsgType,		///< 消息类型
	eKeyTime,			///< 时间
	eKeyUserType,		///< 用户类型
	ekeyPage,			///< 页
	eKeyMainName,		///< 名字
	ekeyLocalId,		///< localId

	eKeyAll				///< 全部数据
};

class IDataMgr
{
public:

	///根据数据类型创建响应数据，返回空代表没有使用此类型存储，或存储中每次此类数据
	virtual DataStorePtr create(eDataType type, int64 targetId = 0) = 0;

};

}

#endif
