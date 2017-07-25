#ifndef CMD_TYPE_DEF_H_
#define CMD_TYPE_DEF_H_
////////////////////////////////////////////////////////////////////////////////
/// Copyright (C), 2015-2016
/// \file   	cmdTypeDef.h
/// \brief  	SDK消息号定义头文件
/// \author 	sharp.young
/// \version    0.2.1
/// \data       2016/1/10
/// \warning    只有[out]没有[in]是纯推送消息
/// \Function List
/// \History:
///  	<author> 		<time>    	   <version >    	<desc>
///     sharp		    2016/1/14        0.2.1         增加 logic_cmd_localSetting 消息定义
////////////////////////////////////////////////////////////////////////////////

namespace imsdk
{

//////////////////////////////////////////////////
/// \enum  路径定义
/// 使用中的路径定义
//////////////////////////////////////////////////
enum  enUserPath 
{
	user_dbPath,		///< enum 数据库路径 user_dbPath
	user_audioPath,		///< enum 音频路径 user_dbPath
	user_cachePath,		///< 缓存路径
	user_imgPath,		///< 图片路径
	user_filePath,		///< 文件路径
	user_videoPath,		///< 视频路径
	user_emotionPath,	///< 表情路径
	user_rootPath		///< 应用程序根路径
};

///设备类型
enum eDeviceType
{
	client_dev_unknown = 0,		///< 未知
	client_dev_pc,				///< PC端
	client_dev_phone,			///< 移动端
	client_dev_html				///< 网页端
};

/**
* \brief 业务类消息号，各个平台的ＡＰＩ层要统一
* [in]  此消息的请求结构体
* [out] 此消息的响应结构体
* \warning (1)只有[out]没有[in]的消息是纯推送消息！！！(2) 实际API中使用的参数都是下面描述的结构体指针。
*/

enum  
{
	/**
	* \brief 一些耗时操作，由SDK下面推送的进度
	* [out] resp_Progress
	*/
	logic_cmd_progress = 0x00, 
	/**
	* \brief 升级提示
	* [out] resp_upgrade
	*/
	logic_cmd_upgrade = 0x01, 
	/**
	*  \brief 登录消息
	* [in]  req_loginParamBean
	* [out] resp_loginResultBean
	*/
	logic_cmd_login = 0x02,
	/**
	* \brief 注销消息
	* [in]  NULL
	* [out] resp_commonResult
	*/
	logic_cmd_logout = 0x03,
	/**
	* \brief 连续错误几次后
	* [in]  req_verfiyimg
	* [out] resp_verifyImg
	*/
	logic_cmd_verifyImg = 0x04,   
	/**
	 * \brief 自动登录
	 * [in]  req_loginParamBean(不需要密码)
	 * [out] resp_loginResultBean
	 */
	logic_cmd_loginAuto = 0x05,
	/**
	 * \brief 获取好友列表
	 * [in]  NULL
	 * [out] resp_buddyList
	 */
	logic_cmd_buddylist = 0x06,
	/**
	 * \brief 离线登录,
	 * [in]  req_offlineLogin
	 * [out] resp_commonResult
	 */
	logic_cmd_offlineLogin = 0x07,
	/**
	 * \brief 获取服务器时间
	 * [in] NULL
	 * [out] resp_commonResult i64 返回毫秒时间
	 */
	logic_cmd_getServerTime = 0x08, 
	/**
	 * \brief 注册第一步
	 * [in]  req_regParam1
	 * [out] resp_regResult1
	 */
	logic_cmd_reg1 = 0x09,
	/**
	 * \brief 注册第二步
	 * [in]  req_regParam２
	 * [out] resp_regResult２
	 */
	logic_cmd_reg2 = 0x0A,
	/**
	 * \brief 重置密码第一步
	 * [in]  req_regParam1
	 * [out] resp_regResult1
	 */
	logic_cmd_resetPw1 = 0x0B,     
	/**
	 * \brief 重置密码第二步
	 * [in]  req_regParam２
	 * [out] resp_regResult２
	 */
	logic_cmd_resetPw2 = 0x0C, 
	/**
	 * \brief 设置自己个人信息
	 * [in]  req_setMySelf
	 * [out] resp_commonResult
	 */
	logic_cmd_setMyself = 0x0D, 
	/**
	 * \brief 获取自己个人信息
	 * [in]  NULL
	 * [out] resp_getMyself
	 */
	logic_cmd_getMyself = 0x0E ,   
	/**
	 * \brief 操作好友, 主动请求操作好友
	 * [in]  req_opUser
	 * [out] resp_commonResult
	 */	
	logic_cmd_opuser = 0x0F, 
	/**
	 * \brief 修改密码
	 * [in]  req_changePw
	 * [out] resp_commonResult
	 */	
	logic_cmd_changePw = 0x10, 
	/**
	 * \brief 获取好友在线状态
	 * [in]  req_getBuddyOnline
	 * [out] resp_getBuddyOnlineResult
	 */	
	logic_cmd_getBuddyOnline = 0x11,
	/**
	 * \brief 更新自己在线状态
	 * [in]  req_updateOnlineState
	 * [out] NULL
	 */	
	logic_cmd_onlineState = 0x12, 
	/**
	 * \brief 发送消息
	 * [in]  req_sendMessage
	 * [out] resp_sendMessage
	 */
	logic_cmd_sendMessage = 0x13,   
	/**
	 * \brief 转发消息，服务器不保留，暂时未用
	 * [in]  req_sendMessage
	 * [out] resp_transferMsg
	 */
	logic_cmd_transferMessage = 0x14, 
	/**
	 * \brief 设置消息已读
	 * [in]  req_setMsgReaded
	 * [out] resp_commonResult
	 */
	logic_cmd_setMsgReaded = 0x15,  
	/**
	 * \brief 得到保存文件路径
	 * [in]  req_getFileInfos
	 * [out] resp_getFileInfos
	 */
	//logic_cmd_getBuddyReqVerifyBox = 0x16, 
	logic_cmd_getFileInfos = 0x16,
	/**
	 * \brief 得到隐藏好友
	 * [in]  req_getHiddenAccountNew
	 * [out] resp_getHiddenAccountNew
	 */
	logic_cmd_getHiddenAccountNew = 0x17,
	//logic_cmd_getBuddyRespVerifyBox = 0x17, 
	/**
	 * \brief 添加好友请求
	 * [in]  req_addBuddyRequest
	 * [out] resp_commonResult
	 */
	logic_cmd_addBuddyRequest = 0x18, 
	/**
	 * \brief 添加好友请求响应
	 * [in]  req_addBuddyResponse
	 * [out] resp_commonResult
	 */
	logic_cmd_addBuddyResponse = 0x19,  
	/**
	 * \brief 好友请求验证框已读
	 * [in]  req_reqVerifyBoxforRead
	 * [out] resp_commonResult
	 */
	logic_cmd_buddyReqVerForead = 0x1A, 
	/**
	 * \brief 根据消息类型获得聊天消息
	 * [in]  req_getMsgByType
	 * [out] resp_getMsgByType
	 */
	logic_cmd_getMsgByType = 0x1B,
	//logic_cmd_buddyRespVerForead = 0x1B, 
	/**
	 * \brief 获取指定用户信息
	 * [in]  req_getUserFromID
	 * [out] resp_getUserFromID
	 */
	logic_cmd_getUser = 0x1C,
	/**
	 * \brief 简单搜索好友
	 * [in]  req_simpleSearch
	 * [out] resp_searchResult
	 */
	logic_cmd_simpleSearch = 0x1D, 
	/**
	 * \brief 最近联系人列表
	 * [in]  NULL
	 * [out] resp_reContactList
	 */
	logic_cmd_reContactList = 0x1E,
	/**
	 * \brief 聊天界面获取聊天历史消息
	 * [in]  req_getHistoryMsg
	 * [out] resp_getHistoryMsg
	 */
	logic_cmd_getHistoryMsg = 0x1F, 
	/**
	 * \brief 获取群列表
	 * [in]  NULL
	 * [out] resp_getGroupList
	 */
	logic_cmd_getGroups = 0x20,  
	/**
	 * \brief 创建群
	 * [in]  req_createGroup
	 * [out] resp_createGroup
	 */
	logic_cmd_createGroup = 0x21,
	/**
	 * \brief 申请加入群
	 * [in]  req_groupVerifyRequest
	 * [out] resp_commonResult
	 */
	logic_cmd_groupVerifyReq = 0x22, 
	/**
	 * \brief 邀请加入群
	 * [in]  req_groupInviteRequest
	 * [out] resp_commonResult
	 */
	logic_cmd_groupInviteReq = 0x23, 
	/**
	 * \brief 推送外部群组信息
	 * [in]  NULL
	 * [out] NULL
	 */
	logic_cmd_PullOutSideGroupInfo = 0x24,
	/**
	 * \brief 发送文件
	 * [in]  req_sendFiles
	 * [out] resp_sendFiles
	 */
	logic_cmd_sendFiles = 0x25,
	/**
	 * \brief 接收文件，
	 * [in]  req_recvFiles
	 * [out] resp_recvFiles
	 */
	logic_cmd_recvFiles = 0x26, 

	/**
	 * \brief 聊天消息 主动推送
	 *
	 * [out] resp_msgBean
	 */
	logic_cmd_msgStore = 0x27, 
	/**
	 * \brief 上下线 主动推送
	 *
	 * [out] resp_Present
	 */
	logic_cmd_present = 0x28,  
	/**
	 * \brief 对方请求添加好友消息 主动推送
	 *
	 * [out] resp_respBuddy
	 */
	logic_cmd_buddyreqVerBox = 0x29, 
	/**
	 * \brief 对方响应添加好友消息 主动推送
	 *
	 * [out] resp_respBuddy
	 */
	logic_cmd_buddyRespVerifybox = 0x2A,	
	/**
	 * \brief 推送消息（复杂应用） 主动推送
	 *
	 * [out] NULL
	 */	
	logic_cmd_pushMessage = 0x2B, 
	/**
	 * \brief 操作好友 主动推送
	 *
	 * [out] resp_operBuddy
	 */	
	logic_cmd_operateBuddy = 0x2C,     
	/**
	 * \brief 修改自己的信息 主动推送
	 *
	 * [out] resp_operUser
	 */	
	logic_cmd_operateUser = 0x2D, 
	/**
	 * \brief 群管理员收到请求，邀请添加群成员等请求 主动推送
	 *
	 * [out] resp_sysMsg
	 */	
	logic_cmd_reqGroup = 0x2E,
	/**
	 * \brief 群请求响应 主动推送
	 *
	 * [out] resp_sysMsg
	 */	
	logic_cmd_respGroup=0x2F,  
	/**
	 * \brief 群管理员收到其他管理员对群的操作通知 主动推送
	 *
	 * [out] resp_sysMsg
	 */	
	logic_cmd_adminRespGroupDone = 0x30,
	/**
	 * \brief 刷新群列表 主动推送
	 *
	 * [out] resp_groupNotify
	 */
	logic_cmd_groupRefresh = 0x31,  
	/**
	 * \brief 转让群 主动推送
	 *
	 * [out] resp_notifyTransGroup
	 */
	logic_cmd_transferGroup = 0x32,   
	/**
	 * \brief 刷新群成员资料 主动推送
	 *
	 * [out] resp_groupMemberInfoRefresh
	 */
	logic_cmd_groupMemberInfoRefresh = 0x33,
	/**
	 * \brief 刷新群成员列表 主动推送
	 *
	 * [out] resp_groupMembersRefresh
	 */
	logic_cmd_groupMembersRefresh = 0x34, 
	/**
	 * \brief 企业注册消息 主动推送
	 *
	 * [out] resp_enterpriseRegisterMessage
	 */
	logic_cmd_enterpriseRegisterMessage = 0x35,
	/**
	 * \brief 暂时未用 主动推送
	 *
	 * [out] NULL
	 */
	logic_cmd_reqResource = 0x36,
	/**
	 * \brief 错误信息，服务器检测 主动推送
	 *
	 * [out] NULL
	 */
	logic_cmd_error = 0x37, 
	/**
	 * \brief 未读消息列表，登录后ＳＤＫ会主动推送到界面 主动推送
	 *
	 * [out] resp_msgList
	 */
	logic_cmd_unReadmsgList = 0x38,
	/**
	 * \brief 设备消息，登录后ＳＤＫ会主动推送到界面 主动推送
	 *
	 * [out] resp_msgList
	 */
	logic_cmd_devicemsgList = 0x39, 
	/**
	 * \brief 获取好友详细信息
	 * [in]  req_getBuddyInfo
	 * [out] resp_getBuddyInfo
	 */
	logic_cmd_getBuddyInfo = 0x3A,
	/**
	 * \brief 更新头像, SDK下载完毕 ,推送到上层.
	 *
	 * [out] resp_updateHeadImg
	 */
	logic_cmd_updateHeadImg = 0x3B,
	/**
	* \brief 设置自己的头像
	* [in]  req_setHeadImg
	* [out] resp_setHeadImg
	*/
	logic_cmd_setHeadImg = 0x3C, 
	/**
	* \brief 获取好友验证方式
	* [in]  req_getBuddyVerifyType
	* [out] resp_getBuddyVerifyType
	*/
	logic_cmd_getBuddyVerifyType = 0x3D,
	/**
	* \brief 解散/退群
	* [in]  req_groupDelete
	* [out] resp_commonResult
	*/
	logic_cmd_groupDelete = 0x3E,
	/**
	* \brief 转让群
	* [in]  req_groupTrans
	* [out] resp_commonResult
	*/
	logic_cmd_groupTrans = 0x3F,
	/**
	* \brief 设置群资料
	* [in]  req_groupInfoUpdate
	* [out] resp_commonResult
	*/
	logic_cmd_groupUpdateInfo = 0x40,
	/**
	* \brief 获取群成员列表
	* [in]  req_groupCommonData
	* [out] resp_getGroupMemList
	*/
	logic_cmd_getGroupMemList = 0x41,
	/**
	* \brief 删除群成员
	* [in]  req_removeGroupMember
	* [out] resp_commonResult
	*/
	logic_cmd_removeGroupMember = 0x42,
	/**
	* \brief 获取群详细信息
	* [in]  req_groupCommonData
	* [out] resp_getGroupInfo
	*/
	logic_cmd_getGroupInfo = 0x43,
	/**
	* \brief 获取群验证方式
	* [in]  req_groupCommonData
	* [out] resp_getGroupSet
	*/
	logic_cmd_getGroupSet = 0x44, 
	/**
	* \brief 获取群成员详细信息
	* [in]  req_getMemberInfo
	* [out] resp_getGroupMemberInfo
	*/
	logic_cmd_getGroupMemberInfo = 0x45,
	/**
	* \brief 设置群成员详细信息
	* [in]  req_setGroupMemberInfo
	* [out] resp_commonResult
	*/
	logic_cmd_setGroupMemberInfo = 0x46,
	/**
	* \brief 设置群消息提示方式
	* [in]  req_setGroupShieldInfo
	* [out] resp_commonResult
	*/
	logic_cmd_setShieldGroupMessage = 0x47,
	/**
	* \brief 获取网址缩略图。
	* [in]  req_getUrlInfo
	* [out] resp_getUrlInfo
	*/
	logic_cmd_getUrlInfo = 0x48,
	/**
	* \brief 近距离加好友、群
	* [in]  req_faceToFaceJoin
	* [out] resp_faceToFaceJoin
	*/
	logic_cmd_faceToFaceJoin = 0x49, 
	/**
	* \brief 发送图片
	* [in]  req_sendImg
	* [out] resp_sendImg
	*/
	logic_cmd_sendImg = 0x4A,
	/**
	* \brief 接收图片
	* [in]  req_recvImg
	* [out] resp_recvImg
	*/
	logic_cmd_recvImg = 0x4B, 
	/**
	* \brief 设置群验证方式
	* [in]  req_setGroupSet
	* [out] resp_commonResult
	*/
	logic_cmd_setGroupSet = 0x4C,
	/**
	* \brief 最后未读一条系统消息(好友验证，响应，群申请等消息)
	* [in]  NULL
	* [out] resp_SysMsg
	*/
	logic_cmd_lastSysMsg = 0x4D, 
	/**
	* \brief 获取系统消息列表
	* [in]  req_getSysMsgList
	* [out] resp_getSysMsgList
	*/
	logic_cmd_getSysMsgList = 0x4E, 
	/**
	* \brief 设置系统消息已读
	* [in]  req_setSysMsgReaded
	* [out] resp_commonResult
	*/
	logic_cmd_setSysMsgReaded = 0x4F, 

	 /**
	 * \brief 推送接收
	 * [in]  req_setIosNoticeInfo
	 * [out] resp_commonResult
	 */
	logic_cmd_setUpiosRecvNotice = 0x50,
	/**
	* \brief 设置TOKEN
	* [in]  req_setTokenAndOlState
	* [out] resp_commonResult
	*/
	logic_cmd_setTokenAndOlState = 0x51,
	/**
	* \brief 1:ios 端切换到后台调用，2:ios 端切换到前台调用
	* [in]  req_setOLtype
	* [out] resp_commonResult
	*/
	logic_cmd_setOLtype = 0x52,
	/**
	* \brief 获取IOS推送设置
	* [in]  NULL
	* [out] resp_getIosNoticeInfo
	*/
	logic_cmd_getIosRecvNoticeSettings = 0x53, 
	/**
	* \brief 获取文件列表
	* [in]  req_getFileList
	* [out] resp_getFileList
	*/
	logic_cmd_getFileList = 0x54,
	/**
	* \brief 删除文件
	* [in]  req_delGroupFiles
	* [out] resp_commonResult
	*/
	logic_cmd_delGroupFiles = 0x55,
	/**
	* \brief 申请加入群响应
	* [in]  req_groupVerifyRespons
	* [out] resp_commonResult
	*/
	logic_cmd_groupVerifyResp = 0x56,
	/**
	* \brief 获取服务器密码规则
	* [in]  _sdkcmd_base
	* [out] resp_pwRuleResult
	*/
	logic_cmd_getPwRule = 0x57,
	/**
	* \brief 获取群消息提示方式
	* [in]  req_getGroupShieldInfo
	* [out] resp_getGroupShieldInfo
	*/
	logic_cmd_getShieldGroupMessage = 0x58,
	/**
	* \brief 获取clientKey , key在value_str里
	* [in]  NULL
	* [out] resp_commonResult
	*/
	logic_cmd_getClientKey = 0x59,
	/**
	* \brief 性能报告消息
	* [in]  NULL
	* [out] resp_perf
	*/
	logic_cmd_perf = 0x5A,

	///安卓专用 安卓事件通知
	logic_cmd_wakeUp = 0x5B,

	/**
	* \brief 发送九宫格图片
	* [in]  req_sendNineBoxImg
	* [out] resp_sendNineBoxImg
	*/
	logic_cmd_sendNineBoxImg = 0x5C,
	/**
	* \brief 接收九宫格图片
	* [in]  req_recvNineBoxImg
	* [out] resp_recvNineBoxImg
	*/
	logic_cmd_recvNineBoxImg = 0x5D,
	/**
	* \brief 设置全局勿扰模式开关信息
	* [in]  req_setNoDisturbMode
	* [out] resp_commonResult
	*/
	logic_cmd_setNoDisturbMode = 0x5E,
	/**
	* \brief 获取全局勿扰模式开关信息
	* [in]  _sdkcmd_base
	* [out] resp_getNoDisturbMode
	*/
	logic_cmd_getNoDisturbMode = 0x5F,
	/**
	* \brief 设置个人对目标用户的勿扰模式
	* [in]   req_setUserToTargetSwitch
	* [out]  resp_commonResult
	*/
	logic_cmd_setUserToTargetSwitch = 0x60,
	/**
	* \brief 获取个人对目标用户的勿扰模式
	* [in]  req_getUserToTargetSwitch
	* [out] resp_getUserToTargetSwitch
	*/
	logic_cmd_getUserToTargetSwitch = 0x61,
	/**
	* \brief 根据账号名获取ID
	* [in]  req_getIdByAccount
	* [out] resp_getIdByAccount
	*/
	logic_cmd_getIdByAccount = 0x62,
	/**
	* \brief
	* [in]  req_queryMarketApplication
	* [out] resp_queryMarketApplication
	*/
	 logic_cmd_queryMarketApplication = 0x63,
	/**
	* \brief 添加或删除应用
	* [in]  req_addOrDeleteApplication
	* [out] resp_commonResult
	*/
	 logic_cmd_addOrDeleteApplication = 0x64,
	/**
	* \brief 获取用户已安装列表信息
	* [in]  req_getInstalledApplication
	* [out] resp_getInstalledApplication
	*/
	 logic_cmd_getInstalledApplication = 0x65,
	/**
	* \brief 设置私信密码
	* [in]  req_setPrivateMsgPwd
	* [out] resp_commonResult
	*/
	logic_cmd_setPrivateMsgPwd = 0x66,
	/**
	* \brief 根据条件查询企业用户列表
	* [in]  req_queryEnterpriseUserlist
	* [out] resp_queryEnterpriseUserlist
	*/
	// logic_cmd_queryEnterpriseUserlist = 0x67,

	/**
	* \brief 删除最近联系人
	* [in]  req_delReContact
	* [out] resp_commonResult
	*/
	logic_cmd_delReContact = 0x68,
	/**
	* \brief 取消已经提交的请求，耗时的操作比较有用，不耗时的操作取消基本都会失败。
	* [in]  req_cancleExec
	* [out] resp_commonResult
	*/
	logic_cmd_cancleExec = 0x69,

	/**
	 *　\brief 发送错误日志
	 *　[in]   req_sendErrLog
	 *　[out]  resp_commonResult
	 */
	logic_cmd_sendErrLog = 0x6A,

	/**
	 *　\brief 获取应用信息
	 *　[in]   req_getAppinfo
	 *　[out]  resp_getAppInfo
	 */
	logic_cmd_getAppInfo = 0x6B,

	/**
	 * \brief　本地与用户相关的配置存储，只有登录后才能使用。
	 * [in] req_localSetting
	 * [out] resp_localSetting
	 */
	logic_cmd_localSetting = 0x6C,

	/**
	 * \brief 网络通知,纯推送
	 * [out] resp_netNotifyer
	 */
	logic_cmd_netNotifyer = 0x6D,

	/**
	 * \brief 获取安全中心页面
	 * [in] req_LoginParamBean ,
	 *      １：　如果没有登录，需要填写该结构体。
	 *      ２：　如果登录后，不用填写该结构体。
	 * [out] resp_commonResult 返回值存在 value_str
	 */
	logic_cmd_getSecUrl = 0x6E,
	
	/**
	 * \brief 　SDK数据库更新推送，升级期间，所有消息不提供服务. 发送请求会收到err_sdk_dbUpdate错误码
	 * [out] resp_commonResult  value_byte = 0 开始，value_byte = １ 成功结束 value_byte = 2 升级失败，
	 * 其它的值失败结束，错误值查看errorExport.h
	 */
	logic_cmd_dbUpdateNotifyer = 0x6F,
	
	/**
	 * \brief 查询组织机构的用户
	 * [in] req_qEnUS
	 * [out] resp_qEnUS
	 */
	// logic_cmd_qEnUS = 0x70,

	/**
	 * \brief 设置隐藏
	 * [in] req_setHiddenAccountNew
	 * [out] resp_HiddenPWDResult
	 */
	logic_cmd_setHiddenAccountNew = 0x71,

	/**
	 * \brief 删除隐藏
	 * [in]  req_setHiddenAccountNew
	 * [out] resp_HiddenPWDResult
	 */
	logic_cmd_deleteHiddenAccountNew = 0x72,

	/**
	 * \brief 找回隐藏密码
	 * [in] req_verifyHiddenInfo
	 * [out] resp_HiddenPWDResult
	 */
	logic_cmd_verifyHiddenInfo = 0x73,

	/**
	 * \brief 重置隐藏密码
	 * [in] req_resetHiddenPWD
	 * [out] resp_commonResult
	 */
	logic_cmd_resetHiddenPWD = 0x74,

	/**
	 * \brief 迁移外部聊天数据库 PC专用
	 * [in] req_transferOuterMsgDb
	 * [out] resp_commonResult
	 */
	logic_cmd_transferOuterMsgDb = 0x75,

	 /**
	 * \brief 外部文件下载命令
	 * [in] req_downloadFile
	 * [out] resp_commonResult
	 */
	logic_cmd_downloadFile = 0x76,

	 /**
	  * \brief 设置用户自身配置
	  * 生日，电话，邮件，好友，Ｖ标，提醒模式，全局消息通知模式
	  * [in]  req_PersonalData
	  * [out] resp_commonResult , value_i16 放置设置成功的条数
	  */
	logic_cmd_setPersonalData = 0x77,

	 /**
	  * \brief 获取用户自身配置
	  * 生日，电话，邮件，好友，Ｖ标，提醒模式，全局消息通知模式
	  * [in]  req_PersonalData
	  *       需要获取那些类型的数据，把类型添加进列表,要获取全部的话，列表可置为空。
	  * [out] resp_PersonalData
	  */
	logic_cmd_getPersonalData = 0x78,

	/**
	* \brief 本地消息全局搜索，只返回每个匹配到的群或个人的消息的数目。
	* [in] req_msgGlobalSearch
	* [out] resp_msgGlobalSearch
	*/
	logic_cmd_msgGlobalSearch = 0x79,

	/**
	* \brief 在某个群或个人的聊天记录中搜索关键字。
	* [in] req_msgDetailSearch
	* [out] resp_msgDetailSearch
	*/
	logic_cmd_msgDetailSearch = 0x7A,
	   
	 /**
      * \brief 通知界面更新完成
      * [out] resp_commonResult
      */
     logic_cmd_updatafinish = 0x7B,

	/**
	* \brief 添加用户到黑名单或白名单
	* [in] req_addToBWlist
	* [out] resp_commonResult
	*/
	logic_cmd_addToBWlist = 0x7C,
	 
	/**
	* \brief 把用户从黑名单或白名单中移除
	* [in] req_removeFromBWlist
	* [out] resp_commonResult
	*/
	logic_cmd_removeFromBWlist = 0x7D,

	/**
	* \brief 获取黑名单或白名单列表
	* [in] req_getBWlist
	* [out] resp_getBWlist
	*/
	logic_cmd_getBWlist = 0x7E,
	  
	/**
     * \brief 获取指定消息
     * [in] req_getAppointMsg
     * [out] resp_getAppointMsg
     */
    logic_cmd_getAppointMsg = 0x7F,

	/**
	* \brief 删除消息
	* [in]  req_delMsg
	* [out] resp_commonResult
	*/
	logic_cmd_delMessage = 0x80,

	/**
	* \brief 聊天置顶
	* [in]  req_chatTop
	* [out] resp_commonResult
	*/
	logic_cmd_chatTop = 0x81,

   /**
	* \brief 特定用户信息设置(是否显示在线，设置用户被查找方式(豆豆号，手机号等),新消息提醒方式等)
	* [in]  req_setUserSetting
	* [out] resp_commonResult
	*/
	logic_cmd_setUserSetting = 0x82,

	/**
	 * \brief 得到特定用户信息设置(是否显示在线，设置用户被查找方式(豆豆号，手机号等),新消息提醒方式等)
	 * 返回值为 resp_commonResult的value_i64字段。
	 * [in]  req_getUserSetting
	 * [out] resp_commonResult
	 */
	logic_cmd_getUserSetting = 0x83,

	/**
	 * \brief 外部聊天消息导入 
	 * [in]  req_msgImport
	 * [out] resp_commonResult
	 */
	logic_cmd_msgImport = 0x84,

	/**
	 * \brief 进入群房间推送 
	 * [out] resp_faceJoinGroup
	 */
	logic_cmd_faceJoinGroup = 0x85,
	/**
	 * \brief 退出群房间推送
	 * [out] resp_exitFaceJoinGroup
	 */
	logic_cmd_exitFaceJoinGroup = 0x86,
	/**
	 * \brief 创建群成功后的推送
	 * [out] resp_createFaceGroupSuccess
	 */
	logic_cmd_createFaceGroupSuccess = 0x87,
	/**
	 * \brief 进入好友房间推送
	 * [out] resp_faceJoinGroup
	 */
	logic_cmd_faceAddBuddy = 0x88,
	/**
	 * \brief 退出好友房间推送
	 * [out] resp_exitFaceJoinGroup
	 */
	logic_cmd_exitFaceAddBuddy = 0x89,
	
	/**
	 * \brief 批量加好友成功后的推送
	 * [out] resp_createFaceGroupSuccess
	 */
	logic_cmd_faceAddBuddyConfirm = 0x8A,

	/**
	 * \brief 删除延迟消息
	 * [in] req_delDelayMsg
	 * [out] resp_commonResult
	 */
	logic_cmd_delDelayMsg = 0x8B,

	/**
	 * \brief 修改延迟消息
	 * [in] req_updateDelayMsg
	 * [out] resp_commonResult
	 */
	logic_cmd_updateDelayMsg = 0x8C,

	/**
	* \brief 绑定手机号第一步
	* [in]  req_authenticationParam
	* [out] resp_authenticationResult
	*/
	logic_cmd_bindEmailOrPhoneNumber1 = 0x8D,
	/**
	* \brief 绑定手机号第2步
	* [in]  req_authenticationServerCode
	* [out] resp_commonResult
	*/
	logic_cmd_bindEmailOrPhoneNumber2 = 0x8E,
	/**
	* \brief 绑定手机号第3步 或者为绑定邮箱（共1步）
	* [in]  req_MultiOpSTParam
	* [out] resp_commonResult
	*/
	logic_cmd_bindEmailOrPhoneNumber3 = 0x8F,

	/**
	* \brief 上传通讯录
	* [in]  req_postContact
	* [out] resp_postContactResult
	*/
	logic_cmd_postContact = 0x90,
	/**
	* \brief 前端用户设置分享列表
	* [in]  req_shareOption
	* [out] resp_commonResult
	*/
	logic_cmd_updateShareOption = 0x91,
	/**
	* \brief 前端用户得到分享列表
	* [in]  req_sharedType
	* [out] resp_commonListResult
	*/
	logic_cmd_getShareOption = 0x92,
	/**
	* \brief 用户自定义一些信息， 目前只表示声音
	* [in]  req_personDefinitionParam
	* [out] req_personDefinitionParam
	*/
	logic_cmd_setPersonDefinition = 0x93,
	/**
	* \brief 得到用户自定义一些信息
	* [in]  req_getPersonDef
	* [out] resp_getPersonDefinitionResult
	*/
	logic_cmd_getPersonDefinition = 0x94,
	/**
	* \brief 根据条件查询拓展字段信息
	* [in]  req_queryExtendedField
	* [out] resp_queryExtendedField
	*/
	logic_cmd_queryExtendedField = 0x95,
	/**
	* \brief 设置群成员背景图片
	* [in]  req_setUserGroupImage
	* [out] resp_commonResult  内容在value_str里面
	*/
	logic_cmd_setUserGroupImage = 0x96,
	/**
	* \brief 获取群成员背景图片
	* [in]  req_getUserGroupImage
	* [out] resp_commonResult  
	*/
	logic_cmd_getUserGroupImage = 0x97,

	/**
	* \brief 背景修改推送
	* [out] resp_bkImgUpdate
	*/
	logic_cmd_bkImgUpdate = 0x98,
	
	/**
	* \brief 单个表情查询、根据表情包标识查询表情，查询所有用户自定义表情
	* [in]  req_queryEmoticon
	* [out] resp_queryEmoticon
	*/
	logic_cmd_queryEmoticon = 0x99,
	/**
	* \brief 单个查询表情包
	* [in]  req_singleQueryEmoticonPackage
	* [out] resp_singleQueryEmoticonPackage
	*/
	logic_cmd_singleQueryEmoticonPackage = 0x9A,
	/**
	* \brief 分页查询表情包
	* [in]  req_pageQueryEmoticon
	* [out] resp_pageQueryEmoticon
	*/
	logic_cmd_pageQueryEmoticon = 0x9B,
	/**
	* \brief 增加或删除用户自定义表情
	* [in]  req_addOrDeleteCustomEmoticon
	* [out] resp_addOrDeleteCustomEmoticon
	*/
	logic_cmd_addOrDeleteCustomEmoticon = 0x9C,
	/**
	* \brief 删除系统消息
	* [in]  req_deleteSysMsgList
	* [out] resp_commonResult
	*/
	logic_cmd_deleteSysMsgList = 0x9D,
	
	/**
	 * \brief 获取局域网可以P2P通讯的用户
	 * [in] NULL
	 * [out] resp_getP2pUser
	 */
	logic_cmd_getP2pUsers=0x9E,

	/**
	 * \brief 局域网用户状态修改推送
	 * [out] resp_P2pUserStatusChg
	 */
	logic_cmd_P2pUserStatusChg=0x9F,

	/**
	 * \brief 局域网用户传输文件请求。
	 * [out] resp_P2pSendFileReq
	 */
	logic_cmd_p2pSendFilereq=0xA0,

	/**
	 * \brief 局域网用户传输文件操作
	 * [out] resp_P2pFileTransOp
	 */
	logic_cmd_p2pFileTransOps=0xA1,

	/**
	* \brief 更新解密后的消息到数据库
	* [in]  req_updateDecryptedMsg
	* [out] resp_commonResult
	*/
	logic_cmd_updateDecryptedMsg=0xA2,

	/**
	 * \brief 快速注册
	 * [in] req_quickRegister
	 * [out] resp_commonResult
	 */
	logic_cmd_quickRegister=0xA3,
	/**
	* \brief 得到最后一条未读系统消息河数量
	* [in] NULL
	* [out] resp_unReadSysMsg
	*/
	logic_cmd_lastUnReadSysMsg=0xA4,

	/**
	 * \brief 发送消息(消息类型可以是附件类型)
	 * [in]  req_sendMessage
	 * [out] resp_transferMsg
	 */
	logic_cmd_sendMessageExt=0xA5,

	/**
	* \brief 解密消息并更新到数据库
	* [in]  req_decryptMsg
	* [out] resp_getHistoryMsg
	*/
	logic_cmd_decryptMsg=0xA6,
	/**
	* \brief 单个查询表情包
	* [in]  req_labelQueryEmoticonPackage
	* [out] resp_labelQueryEmoticonPackage
	*/
	logic_cmd_labelQueryEmoticonPackage = 0xA7,
	/**
	* \brief 取消正在进行的传输或拒绝尚未开始的p2p传输
	* [in]  req_p2pTransferCancle
	*/
    logic_cmd_p2pFileTransCancle = 0xA8,
	/**
	*\brief 导入导出本地数据
	*[in] req_transferLocalData
	*[out] resp_commonresult
	*/
	logic_cmd_transferLocalData = 0xA9,

	/**
	* \brief 设置未读数量（IOS 专用接口）
	* [in]  req_setUnReadCount
	* [out] resp_setUnReadCount
	*/
	logic_cmd_setUnReadCount = 0xAA,

	/**
	* \brief 设置最后一条已读为未读（android)
	* [in]  req_setMsgUnRead
	* [out] resp_setMsgUnRead
	*/
	logic_cmd_setMsgUnRead = 0xAB,

	/**
	 * \brief  服务器推送错误 主动推送,下面会自动重联
	 *
	 * [out] NULL
	 */
	logic_cmd_NotifyError = 0xAC, 

	/**
	* \brief 上传消息计数
	*[in] req_cmd_upMsgCount
	*[out] resp_commonresult
	*/
	logic_cmd_upMsgCount = 0xAD,

	/**
	* \brief 消息游标推送
	*[in] NULL
	*[out] resp_MessageCursor
	*/
	logic_cmd_MessageCursor = 0xAE,

	/**
	* \brief 一步发送文件接口
	*[in] req_fileMessage
	*[out] resp_commonResult
	*/
	logic_cmd_fileMessage = 0xAF,

	/**
	* \brief 一步发送图片接口
	*[in] req_imgMessage
	*[out] resp_commonResult
	*/
	logic_cmd_imgMessage = 0xB0,
	/**
	* \brief 查询单个表情
	*[in] req_querySingleEmoticon
	*[out] resp_querySingleEmoticon
	*/
	logic_cmd_querySingleEmoticon = 0xB1,
	/**
	* \brief  查询机器人（公众号）时间戳，查询数据字典时间戳
	*[in] req_queryAppOrDictTimestamp
	*[out] resp_queryAppOrDictTimestamp
	*/
	logic_cmd_queryAppOrDictTimestamp = 0xB2,
	
	/**
	* \brief 检查URL的是否有效
	*[in] req_checkUrlValid
	*[out] resp_commonResult
	*/
	logic_cmd_checkUrlValid = 0xB3,

	/**
	* \brief 转发消息
	*[in] req_forwardMessage
	*[out] resp_sendMessage
	*/
	logic_cmd_forwardMessage = 0xB4,
	/**
	* \brief 增加最近联系人
	*[in] req_addRecontact
	*[out] resp_commonResult
	*/
	logic_cmd_addRecontact = 0xB5,
	/**
	* \brief 获取OAUTH登陆授权码
	*[in] req_getLoginAuthCode
	*[out] resp_commonResult
	*/
	logic_cmd_getLoginAuthCode = 0xB6,

	/**
	* \brief 获取对象聊天时间数组
	*[in] req_getMsgDays
	*[out] resp_getMsgDays
	*/
	logic_cmd_getMsgDays = 0xB7,

	/**
	* \brief 根据日期获取消息
	*[in] req_getMsgsFromDay
	*[out] resp_getMsgsFromDay
	*/
	logic_cmd_getMsgFromDay = 0xB8,

	logic_cmd_getInstallDays = 0xB9,

	logic_cmd_getAllMsgCount = 0xba,
	
	/**
	* \brief 获取打洞服务地址
	*[in] NULL
	*[out] resp_stunServerInfo
	*/
    logic_cmd_getStunServer = 0xBB,

	/**
	* \brief 获取声网服务功能
	*[int] req_agoraFunc
	*[out] resp_agoraFunc
	*/
	logic_cmd_getAgoraFunc = 0xBC,
	logic_cmd_transferMsg = 0xBD,
	/**
	* \brief 根据用户id分页获取企业号
	* [in]  req_getEntAppInfo
	* [out] resp_getEntAppInfo
	*/
	logic_cmd_getEntAppInfo = 0xBE,
	logic_cmd_max
};

}

#endif  //CMD_TYPE_DEF_H_
