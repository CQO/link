#ifndef CMD_RESP_DEF_H
#define CMD_RESP_DEF_H

#include "cmdComDef.h"

//=================================<所有响应消息结构体>============================================//

namespace imsdk
{

/**
 * \struct  resp_setHeadImg  消息号：logic_cmd_setHeadImg
 * \brief   设置头像
 */
struct resp_setHeadImg : public _sdkrespcmd_base  
{
	std::string  thumb_json;    ///< 缩略图本地地址 thumb_json.
	std::string  orgi_json;     ///< 大图本地地址 orgi_json.
	CMD_SIZE();
	COPY_OTHER(resp_setHeadImg);
};

/**
 * \struct  resp_getBuddyVerifyType  消息号：logic_cmd_getBuddyVerifyType
 * \brief   获得好友验证信息
 */
struct resp_getBuddyVerifyType : public _sdkrespcmd_base  
{
	int64 userid;             ///< 好友ＩＤ userid.
	int8 type;                ///< value: 1：需要验证信息,2:不允许任何人添加,3:允许任何人添加，默认1 
	CMD_SIZE();
	COPY_OTHER(resp_getBuddyVerifyType);
};

/**
 * \struct  resp_UpdateHeadImg  消息号：logic_cmd_updateHeadImg
 * \brief   更新头像
 */
struct resp_UpdateHeadImg : public _sdkrespcmd_base  
{
	int64 userid; 	          ///< 用户ＩＤ,群ＩＤ，组织ＩＤ,APPID userid.
	std::string fileName;     ///< 文件名 不包含路径 fileName.
	CMD_SIZE();
	COPY_OTHER(resp_UpdateHeadImg);
};

/**
 * \struct  resp_getBuddyInfo  消息号：logic_cmd_getBuddyInfo
 * \brief   获取好友详细信息
 */
struct resp_getBuddyInfo : public _sdkrespcmd_base 
{
	st_buddyBean info;  ///< 好友信息 info.
	void  init() 
	{
		info.info.phones.clear();
		info.info.emails.clear();
	}
	CMD_SIZE();
	COPY_OTHER(resp_getBuddyInfo);
};

/**
 * \struct  resp_recvFiles  消息号：logic_cmd_recvFiles
 * \brief   文件接收
 */
struct resp_recvFiles : public _sdkrespcmd_base
{
	int64       targetID;    ///< 对方ID,有可能是群或者人 targetID.
	std::string localPath;   ///< 接收成功后文件路径 localPath.
	CMD_SIZE();
	COPY_OTHER(resp_recvFiles);
};

/**
 * \struct  resp_sendFiles  消息号：logic_cmd_sendFiles
 * \brief   文件发送
 */
struct resp_sendFiles : public _sdkrespcmd_base
{
	int64       targetID;            ///< 目标ID targetID.
	std::vector<std::string> jsonVt; ///< JSON字符串数组 jsonVt.
	CMD_SIZE();
	COPY_OTHER(resp_sendFiles);
	void  init() 
	{
		jsonVt.clear();
	}
};

/**
 * \struct  resp_getHistoryMsg  消息号：logic_cmd_getHistoryMsg
 * \brief   历史聊天消息
 */
struct resp_getHistoryMsg : public _sdkrespcmd_base 
{
	int64   targetID;			///< 目标ID,群号或者用户号 targetID.
	std::vector<st_msgBean>  _vt;
	void  init() 
	{
		_vt.clear();
	}
	CMD_SIZE();
	COPY_OTHER(resp_getHistoryMsg);
};

/**
* \struct  resp_getAppointMsg  消息号：logic_cmd_getAppointMsg
* \brief   指定聊天消息
*/
struct resp_getAppointMsg : public _sdkrespcmd_base
{
    int64 targetID;			///< 目标ID,群号或者用户号 targetID.
    std::vector<st_msgBean>  _vt;
    void  init()
    {
        _vt.clear();
    }
    CMD_SIZE();
	COPY_OTHER(resp_getAppointMsg);
};

/**
 * \struct  resp_reContactList   消息号: logic_cmd_reContactList
 * \brief   联系人列表
 */
struct   resp_reContactList : public _sdkrespcmd_base 
{
	/**
	 * \brief 数据包flag
	 * 全部更新:
	*	0x01　第一个包,　　 需要清理原来的数据
	*	0x02  中间的包，　　在原来的数据后面追加
	*	0x04  最后的包，　　最近会话发送完毕
	* 部分更新:
	*   0x08  更新
	*	0x10  新增
	*	0x20  删除
	* 全部删除
	*   0x40 删除所有最近联系人
	 */
	int8 flag;               //包含0x01(第一个包), 0x02(后面还有数据包),0x04(最后一个数据包) flag.
	std::vector<st_reContact>  _vt;
	void init() 
	{
		_vt.clear();
	}
	CMD_SIZE();
	COPY_OTHER(resp_reContactList);
};

/**
 * \struct  resp_transferMsg   消息号: logic_cmd_transMessage
 * \brief   转发信息
 */
struct  resp_transferMsg : public _sdkrespcmd_base
{
	std::string what;
	int64  sendTime;
	CMD_SIZE();
	COPY_OTHER(resp_transferMsg);
};

/**
 * \struct  resp_sendMessage   消息号: logic_cmd_sendMessage
 * \brief   发送信息
 */
struct  resp_sendMessage : public _sdkrespcmd_base
{
	resp_sendMessage()
	{
		init();
	}
	std::string what;
	int64 messageID;       ///< 服务端发送消息ID
	int64 sendTime;        ///< 服务端发送消息时间
	int64 lastMessageID;   ///< 服务端上次发送消息ID
	std::vector<std::string> badwordList; ///< 如果发送的消息包含敏感词，返回敏感词的集合。用于一步发送文件消息时，为文件的远程url,按原图-缩略图-原图。。排列
	int8 badwordType;      ///< 0不过滤 1使用*号 2禁止发送
	void init()
	{
		messageID = 0;
		sendTime = 0;
		lastMessageID = 0;
		badwordType = 0;
		badwordList.clear();
	}
	CMD_SIZE();
	COPY_OTHER(resp_sendMessage);
};

/**
 * \struct  resp_sendMessageExt   消息号: logic_cmd_sendMessageExt
 * \brief   发送信息
 */
struct  resp_sendMessageExt : public resp_sendMessage
{
    /* nothing */
};

/**
 * \struct  resp_SearchResult   消息号: logic_cmd_simpleSearch
 * \brief   搜索
 */
struct resp_SearchResult : public _sdkrespcmd_base
{
	int8 type;										///< 1 搜索联系人 2搜索群 3搜索团队 type.
	std::string what;
	std::vector<st_smallUserBean> vtub;       		///< 用户列表 vtub.
	std::vector<st_groupBean> vtgroup;  		    ///< 群列表 vtgroup.
    std::vector<st_entInfo> vtent;     	            ///< 企业列表 vtent.
	std::vector<st_orGroupinfo> vtOrgroup;     	    ///< 企业组织列表 vtOrgroup.
	std::vector<st_orgUserinfo> vtorgUser;      	///< 组织用户列表 vtorgUser
	std::vector<st_entAppinfo> vtEntApp;       	    ///< 企业应用列表 vtEntApp
	void init() 
	{
		vtub.clear();
		vtgroup.clear();
		vtent.clear();
		vtOrgroup.clear();
		vtorgUser.clear();
		vtEntApp.clear();
	}
	CMD_SIZE();
	COPY_OTHER(resp_SearchResult);
};

/**
 * \struct  resp_getBuddyOnlineResult   消息号: logic_cmd_getBuddyOnline
 * \brief  
 */
struct resp_getBuddyOnlineResult : public _sdkrespcmd_base 
{
	std::string   what;
	std::vector<st_onlineState> obs;
	void init() 
	{
		obs.clear();
	}
	CMD_SIZE();
	COPY_OTHER(resp_getBuddyOnlineResult);
};

/**
 * \struct  resp_getMyself   消息号: logic_cmd_getMyself
 * \brief  获取自身信息
 */
struct resp_getMyself : public _sdkrespcmd_base
{
	std::string what;
	st_userBean user;
	void init() 
	{
		user.info.phones.clear();
		user.info.emails.clear();
	}
	CMD_SIZE();
	COPY_OTHER(resp_getMyself);
};

/**
 * \struct  resp_getUserFromID   消息号: logic_cmd_getUser
 * \brief  通过ID获得用户
 */
struct   resp_getUserFromID : public _sdkrespcmd_base
{
	std::string       entExtend;
	st_smallUserBean  user;
	void init()
	{
		user.emails.clear();
		user.phoneNums.clear();
		user.srvName = "";
	}
	CMD_SIZE();
	COPY_OTHER(resp_getUserFromID);
};

/**
 * \struct  resp_buddyList   消息号: logic_cmd_buddylist
 * \brief  好友列表
 */
struct resp_buddyList : public _sdkrespcmd_base
{
	/**
	  * \brief
	  * 0x01 包含此标识，清空ＵＩ列表插入下面列表项
	  * 0x02　包含此标识，ＵＩ列表尾部添加下面列表项
	  * 0x04 包含此标识，所有好友添加完毕
	  * 0x08 包含此标识，从当前ＵＩ列表中更新下面列表项　不和其他标识组合
	  * 0x10 包含此标识，从当前ＵＩ列表中删除下面列表项　不和其他标识组合
	  */
	int32  flag;
	std::vector<st_tinyBuddyBean>  tub;		 ///< 用户列表 tub.
	void  init() 
	{
		tub.clear();
	}
	CMD_SIZE();
	COPY_OTHER(resp_buddyList);
};

/**
 * \struct  resp_getHiddenAccountNew   消息号: logic_cmd_getHiddenAccountNew
 * \brief  通过密码得到隐藏的好友或群Id
 */
struct resp_getHiddenAccountNew : public _sdkrespcmd_base
{
	resp_getHiddenAccountNew() :flag(0){}
	std::vector<int64> hiddenIds;			    ///< 隐藏好友或群ID集合.
	int8 flag;								///< 1代表存在此隐藏好友密码，0代表不存在.

	void init()
	{
		hiddenIds.clear();
	}
	CMD_SIZE();
	COPY_OTHER(resp_getHiddenAccountNew);
};

/**
 * \struct  resp_buddyList   消息号: 
 * \brief  隐藏好友或群组
 */
struct resp_HiddenPWDResult : public _sdkrespcmd_base
{
	std::string what;			///< 出错信息 tub.
	int64 timestamp;			///< 时间戳 timestamp.
	std::map<std::string, std::vector<int64> >  haccounts;  ///< 隐藏的ID,key为密码，value为隐藏的id集合 hacounts.

	void init()
	{
		haccounts.clear();
	}
	CMD_SIZE();
	COPY_OTHER(resp_HiddenPWDResult);
};

/**
 * \struct  resp_Progress   消息号: logic_cmd_progress
 * \brief  进度消息结构,主要用于sdk库执行期间返回一些描述信息
 *         该消息的localID为执行命令的消息localID
 */
struct resp_Progress : public _sdkrespcmd_base
{
	int32 extra_req;       ///< 额外的请求信息 extra_req.
	int32 progess;         ///< 进度百分比 progess.
	std::string info;      ///< 描述信息 info.
	CMD_SIZE();
	COPY_OTHER(resp_Progress);
};

/**
 * \struct  resp_upgrade   消息号: logic_cmd_upgrade
 * \brief  系统升级
 */
struct resp_upgrade : public _sdkrespcmd_base 
{
	std::string hasCode;	///< 升级包的HAXI值.
	int8 gradeType;			///< 升级类型 0建议升级,1强制升级 gradeType.
	std::string version;    ///< 服务器版本号 version.
	std::string updateAddr; ///< 升级的地址.
	std::string desc;		///< 升级描述信息.
	CMD_SIZE();
	COPY_OTHER(resp_upgrade);
};


/**
 * \struct  resp_regResult1   消息号: logic_cmd_reg1，logic_cmd_resetPw1
 * \brief  注册1，找回密码1
 */
struct resp_regResult1 : public _sdkrespcmd_base 
{
	std::string  	what;
	int64         	registryID;
	std::string  	sendNumber;
	int32           reregistryTimeout;
	std::string  	nation;
	CMD_SIZE();
	COPY_OTHER(resp_regResult1);
};

/**
 * \struct  resp_regResult2   消息号: logic_cmd_reg2，logic_cmd_resetPw2
 * \brief  注册2，找回密码2
 */
struct resp_regResult2 : public _sdkrespcmd_base
{
	int64         	registryID; ///< 第二步传入的注册编号 registryID.
	int64           userID;     ///< 用户ID   userID.
	std::string  	what;       ///< 错误描述，当前无用 what.
	std::string  	usr;        ///< 注册的用户名 usr.
	CMD_SIZE();
	COPY_OTHER(resp_regResult2);
};

/**
 * \struct  resp_verifyImg   消息号: logic_cmd_Verfiyimg
 * \brief  验证码返回
 */
struct  resp_verifyImg : public _sdkrespcmd_base 
{
	int8 type;		  ///< 类型 type.
	std::string what; ///< 错误描述 what.
	std::string img;  ///< 图片地址 img.
	CMD_SIZE();
	COPY_OTHER(resp_verifyImg);
};

/**
 * \struct  resp_LoginResultBean   消息号: logic_cmd_login
 * \brief  登录
 * 如果在登录状态下收到该结构体的CODE不为零，要跳回到登录界面
 */
struct resp_LoginResultBean : public _sdkrespcmd_base
{
	int64 userid;					  ///< 用户ID userid.
	std::string what;                 ///< code非零的情况下有效 what.
	std::string verImgPath;           ///< 验证码 verImgPath.
	std::string area;                 ///< 地区 area.
	int32 sessionTimeout;             ///< 超时时间 sessionTimeout.
	int64 serverTime;                 ///< 服务器时间 serverTime.
	int64 remainLockTime;             ///< 账号锁定剩余时长,只有在账号被锁定时才有效
	void init() 
	{
		what="";
		verImgPath="";
		area="";
	}
	CMD_SIZE();
	COPY_OTHER(resp_LoginResultBean);
};

/**
 * \struct  resp_msgBean   消息号: logic_cmd_msgStore
 * \brief  聊天消息
 */
struct resp_msgBean : public _sdkrespcmd_base
{
	st_msgBean msg;
	void init() 
	{
		msg.limitRange.clear();
		msg.relatedUsers.clear();
	}
	CMD_SIZE();
	COPY_OTHER(resp_msgBean);
};

/**
 * \struct  resp_present   消息号: 
 * \brief  上下线消息
 */
struct resp_present : public _sdkrespcmd_base
{
	int64 userID; ///< 上下线的用户ID userID.
	/**
	 * \brief 设备类型
	 * 1：PC；2：手机；3：pad；4：网页
	 */
	int8  deviceType;
	/**
	 * brief 设备标识
	 * -1：离线，1:在线；2:隐身,3:忙碌,4:离开,5:请勿打扰,6:想聊天
	 */
	int8  flag;
	std::string deviceInfo;
	CMD_SIZE();
	COPY_OTHER(resp_present);
};

//=================================<推送消息>============================================//

/**
 * \struct  resp_groupNotify   消息号: logic_cmd_groupRefresh
 * \brief  群刷新
 */
struct resp_groupNotify : public _sdkrespcmd_base
{

	void init()
	{
		group.init();
	}

	int8  operType;	            ///< 操作类型 1：添加 2：更新，31：解散 32：移除 33：退出 operType.
	int64 userID; 	 			///< 操作人信息 userID.
	std::string  userName;      ///< 操作人姓名 userName.
	st_groupBean group;         ///< 群信息 group.
	CMD_SIZE();
	COPY_OTHER(resp_groupNotify);
};

/**
 * \struct  resp_reqBuddy   消息号: logic_cmd_buddyreqVerBox
 * \brief  服务器主动推送消息
 */
struct resp_reqBuddy : public _sdkrespcmd_base
{
	int64  			 ts  ;   ///< UI不用 ts.
	st_buddyReqBean  brq ;   ///< 添加好友结构 brq.
	void init() 
	{
		brq.reqUserBean.phoneNums.clear();
		brq.reqUserBean.emails.clear();
		brq.verifyInfo.clear();
	}
	CMD_SIZE();
	COPY_OTHER(resp_reqBuddy);
};

/**
 * \struct  resp_respBuddy   消息号: logic_cmd_buddyRespVerifybox
 * \brief  
 */
struct  resp_respBuddy : public _sdkrespcmd_base
{
	int64            ts;    ///< UI不用 ts.
	st_buddyRespBean brq;   ///< 好友响应结构 brq.
	void init() {
		brq.respUserBean.phoneNums.clear();
		brq.respUserBean.emails.clear();
	}
	CMD_SIZE();
	COPY_OTHER(resp_respBuddy);
};

/**
 * \struct  resp_pushMessage   消息号: logic_cmd_pushMessage
 * \brief  推送消息（复杂应用）
 */
struct  resp_pushMessage : public _sdkrespcmd_base 
{
    resp_pushMessage() :
        previousMsgID(0),
        msgType(0),
        msgID(0),
        createUserID(0),
        fromTeamID(0),
        originalMsgID(0),
        status(0),
        createTime(0),
        operType(0),
        havVaild(0) {}

	int64 previousMsgID;
	int8  msgType;
	int64 msgID;
	int64 createUserID;
	int64 fromTeamID;
	std::vector<int64>  targetUserIDs;
	std::string title;
	std::string body;
	std::string format;
	int64 originalMsgID;
	int8 status;
	int64 createTime;
	int8 operType;
	std::string maccode;
	int8 havVaild;
	void init() 
	{
		targetUserIDs.clear();
	}
	CMD_SIZE();
	COPY_OTHER(resp_pushMessage);
};

/**
 * \struct  resp_operBuddy   消息号: logic_cmd_operateBuddy
 * \brief  操作好友
 */
struct resp_operBuddy : public _sdkrespcmd_base 
{
	int8 operType;			///< 1: add 2: update，3: delete operType.
	st_tinyBuddyBean  tub;
	void init() 
	{
		operType = 0;
		tub.init();
	}
	CMD_SIZE();
	COPY_OTHER(resp_operBuddy);
};

/**
 * \struct  resp_operUser   消息号: logic_cmd_operateUser
 * \brief  操作自己
 */
struct resp_operUser : public _sdkrespcmd_base  
{
	st_userBean  user;
	void init() 
	{
		user.info.phones.clear();
		user.info.emails.clear();
	}
	CMD_SIZE();
	COPY_OTHER(resp_operUser);
};

/**
 * \struct  resp_reqGroup   消息号: logic_cmd_reqGroup
 * \brief  群管理收到请求，添加等请求
 */     
struct resp_reqGroup : public _sdkrespcmd_base {
	int64 reqID;
	int64 reqUserID;
	int64 groupID;
	std::string verifyInfo;
	int8  reqType;
	int64 reqTime;
	int8  status;
	std::string groupName;
	std::string groupIcon;
	std::string groupType;
	std::vector<int64>  inviteUsers;
	void init() 
	{
		inviteUsers.clear();
	}
	CMD_SIZE();
	COPY_OTHER(resp_reqGroup);
};

/**
 * \struct  resp_createGroup   消息号: logic_cmd_createGroup
 * \brief  创建群
 */  
struct resp_createGroup : public _sdkrespcmd_base
{
	int64 groupid;   ///< 群ＩＤ groupid.
	CMD_SIZE();
	COPY_OTHER(resp_createGroup);
};

/**
 * \struct  resp_getGroupList   消息号: logic_cmd_getGroups
 * \brief  获取群列表返回
 */
struct resp_getGroupList : public _sdkrespcmd_base
{
	std::vector<st_tinyGroupBean> groups;
	void init() 
	{
		groups.clear();
	}
	CMD_SIZE();
	COPY_OTHER(resp_getGroupList);
};

/**
 * \struct  resp_groupResponseResult   消息号: 
 * \brief  
 */
struct resp_groupResponseResult : public _sdkrespcmd_base
{
	int32 result;				///< 多个管理员处理的时候，如果已经处理了，返回上一个处理结果 result.
	st_tinyUserBean user;		///< 响应人信息 user.
	resp_commonResult ret;
	CMD_SIZE();
	COPY_OTHER(resp_groupResponseResult);
};

/**
 * \struct  resp_msgList   消息号: logic_cmd_unReadmsgList
 * \brief   获取系统消息列表
 */
struct resp_msgList : public _sdkrespcmd_base
{
	std::vector<st_ureadMsgList> msglist;
	void init() 
	{
		msglist.clear();
	}
	CMD_SIZE();
	COPY_OTHER(resp_msgList);
};

/**
 * \struct  resp_getGroupMemList   消息号: logic_cmd_getGroupMemList
 * \brief   群成员列表
 */
struct resp_getGroupMemList : public _sdkrespcmd_base
{
	int64   groupID;
	std::vector<st_tinyGroupMemberBean>  _vt;
	void init()
	{
		groupID = 0;
		_vt.clear();
	}
	CMD_SIZE();
	COPY_OTHER(resp_getGroupMemList);
};

/**
 * \struct  resp_getGroupInfo   消息号: logic_cmd_getGroupInfo
 * \brief   获取群详细信息
 */
struct resp_getGroupInfo : public _sdkrespcmd_base
{
	st_groupBean group;
	void init()
	{
		_sdkrespcmd_base::init();
	}
	CMD_SIZE();
	COPY_OTHER(resp_getGroupInfo);
};

/**
 * \struct  resp_getGroupSet   消息号: logic_cmd_getGroupSet
 * \brief   获取群验证方式
 */
struct resp_getGroupSet : public _sdkrespcmd_base
{
	st_groupSet gs;
	void init()
	{
		_sdkrespcmd_base::init();
	}
	CMD_SIZE();
	COPY_OTHER(resp_getGroupSet);
};

/**
 * \struct  resp_getGroupMemberInfo   消息号: logci_cmd_getGroupMemberInfo
 * \brief   获取群成员信息
 */
struct resp_getGroupMemberInfo : public _sdkrespcmd_base
{
	st_groupMemberBean member;
	CMD_SIZE();
	COPY_OTHER(resp_getGroupMemberInfo);
	void init()
	{
		member.birthday = 0;
		member.name = "";
		member.nickID = "";
		member.sign = "";
		member.phones.clear();
		member.emails.clear();
		member.info.init();
	}
};

/**
 * \struct  resp_setGroupMemberInfo   消息号: logic_cmd_setGroupMemberInfo
 * \brief   设置群成员信息
 */
struct resp_setGroupMemberInfo : public _sdkrespcmd_base
{
	st_groupMemberBean member;
	CMD_SIZE();
	COPY_OTHER(resp_setGroupMemberInfo);
};

/**
 * \struct  resp_getGroupShieldInfo   消息号: logic_cmd_getShieldGroupMessage
 * \brief   获取群消息提示方式
 */
struct resp_getGroupShieldInfo : public _sdkrespcmd_base
{
	int8  receiveMsgType;     ///< 1：提示并接收消息；2：不提示，接收仅显示数目；3：屏蔽消息 receiveMsgType.
	int64 groupId;			  ///< 群ID.
	std::string receiveTimePeriod;
	void init()
	{
		receiveMsgType = 0;
		receiveTimePeriod = "";
		groupId = 0;
	}
	CMD_SIZE();
	COPY_OTHER(resp_getGroupShieldInfo);
};

/**
 * \struct  resp_notifyTransGroup   消息号: logic_cmd_transferGroup
 * \brief   转移群
 */
struct resp_notifyTransGroup : public _sdkrespcmd_base
{
	int64 groupID;
	std::string groupName;
	std::string userName;   // 暂时不用.
	int64 userID;           // 转让群后，新的群主ID.
	CMD_SIZE();
	COPY_OTHER(resp_notifyTransGroup);
};

/**
 * \struct  resp_groupMemberInfoRefresh   消息号: logic_cmd_groupMemberInfoRefresh
 * \brief   群成员信息刷新
 */
struct resp_groupMemberInfoRefresh : public _sdkrespcmd_base
{
	void init()
	{
		info.init();
	}

	int64 groupID;
	st_tinyGroupMemberBean info;
	CMD_SIZE();
	COPY_OTHER(resp_groupMemberInfoRefresh);
};

/**
 * \struct  resp_groupMembersRefresh   消息号: logic_cmd_groupMemberInfoRefresh
 * \brief   所有群成员信息刷新
 */
struct resp_groupMembersRefresh : public _sdkrespcmd_base
{
	int8  operType;                         ///< 1 添加， 32 移除， 33 退出 operType.
	int64 groupID;
	std::vector<st_tinyGroupMemberBean> infos;
	void init()
	{
		infos.clear();
	}
	CMD_SIZE();
	COPY_OTHER(resp_groupMembersRefresh);
};

/**
 * \struct  resp_sendImg   消息号: logic_cmd_sendImg
 * \brief   发送图片
 */
struct resp_sendImg : public _sdkrespcmd_base 
{
	int64   targetID;			///< 发送方ID targetID.
	std::string  thumb_Json;    ///< 返回缩略图JSON串 thumb_Json.
	std::string  orgi_json;     ///< 返回大图JSON orgi_json.
	CMD_SIZE();
	COPY_OTHER(resp_sendImg);
};

/**
 * \struct  resp_recvImg   消息号: logic_cmd_recvImg
 * \brief   接收图片
 */
struct resp_recvImg : public _sdkrespcmd_base 
{
	int64  targetID; 		    ///< 发送方对方ＩＤ targetID.
	std::string imgName;  	    ///< 本地文件路径 imgName.
	CMD_SIZE();
	COPY_OTHER(resp_recvImg);
};

/**
* \struct  resp_sendNineBoxImg   消息号: logic_cmd_sendNineBoxImg
* \brief   发送九宫格图片
*/
struct resp_sendNineBoxImg : public _sdkrespcmd_base
{
	int64   targetID;			///< 发送方ID targetID.
	std::string  thumb_Json;    ///< 返回缩略图JSON串 thumb_Json.
	std::string  orgi_json;     ///< 返回大图JSON orgi_json.
	int8 imgIndex;				///< 图片索引 0-8
	CMD_SIZE();
	COPY_OTHER(resp_sendNineBoxImg);
};

/**
* \struct  resp_recvNineBoxImg   消息号: logic_cmd_recvNineBoxImg
* \brief   接收九宫格图片，接收几张，会推送几次消息
*/
struct resp_recvNineBoxImg : public _sdkrespcmd_base
{
	int64  targetID; 		    ///< 发送方对方ＩＤ targetID.
	std::string imgName;  	    ///< 本地文件路径 imgName.
	int8 imgIndex;				///< 图片索引 0-8
	int8 imgCount;				///< 总图片数
	CMD_SIZE();
	COPY_OTHER(resp_recvNineBoxImg);
};

/**
 * \struct  resp_SysMsg   消息号: logic_cmd_lastSysMsg
 * \brief   最后一条系统消息
 */
struct  resp_SysMsg : public _sdkrespcmd_base 
{
	st_sysMsg sysMsg;
	CMD_SIZE();
	COPY_OTHER(resp_SysMsg);		
};

/**
* \struct  resp_unReadSysMsg   消息号: logic_cmd_lastUnReadSysMsg
* \brief   最后一条系统消息，刚登陆上时推送
*/
struct  resp_unReadSysMsg : public _sdkrespcmd_base
{
	st_sysMsg sysMsg;                               ///< 系统消息.
	int32 unReadCount;                              ///< 未读数量.
	CMD_SIZE();
	COPY_OTHER(resp_unReadSysMsg);
};

/**
 * \struct  resp_getSysMsgList   消息号: logic_cmd_getSysMsgList
 * \brief   获取系统消息列表
 */
struct resp_getSysMsgList : public _sdkrespcmd_base 
{
	int8      type;
	std::vector<resp_SysMsg>  sysMsgVt;
	void init()
	{
		sysMsgVt.clear();
	}
	CMD_SIZE();
	COPY_OTHER(resp_getSysMsgList);
};

/**
 * \struct  resp_getIosNoticeInfo   消息号: logic_cmd_getIosRecvNoticeSettings
 * \brief   获取Ios通知信息
 */
struct resp_getIosNoticeInfo : public _sdkrespcmd_base
{
	st_iosNoticeInfo info;
	CMD_SIZE();
	COPY_OTHER(resp_getIosNoticeInfo);
};

/**
 * \struct  resp_getFileList   消息号: logic_cmd_getFileList
 * \brief   得到文件列表
 */
struct resp_getFileList : public _sdkrespcmd_base 
{
	std::vector<st_Fileinfo> _vt;
	void init() 
	{
		_vt.clear();
	}
	CMD_SIZE();
	COPY_OTHER(resp_getFileList);
};

/**
 * \struct  resp_pwRuleResult   消息号: logic_cmd_getPwRule
 * \brief   得到密码规则
 *		高8位代表最小长度
 *		低8位，按照最低位开始，一次代表:(1代表必须，0 表示可选)
 *		1. 是否必须有数字
 *		2. 是否必须有小写字母
 *		3. 是否必须有大写字母
 *		4. 是否必须有英文字母
 *		5. 是否必须有字符(特殊字符)
 *		6. 是否允许注册(1允许，0不允许)
 */
struct resp_pwRuleResult : public _sdkrespcmd_base
{
	int32 code;
	std::string what;
	int16 stipulate;
	CMD_SIZE();
	COPY_OTHER(resp_pwRuleResult);
};

/**
 * \struct  resp_perf   消息号:
 * \brief  
 */
struct resp_perf : public _sdkcmd_base 
{
	uint16  cmd;
	int8    type;	///< 0 请求通道处理时间,1:DB通道处理时间,2:网络通道处理时间,3,other处理时间 type.
	int32   Perf;
	void init() 
	{
		Perf = 0;
	}
	CMD_SIZE();
	COPY_OTHER(resp_perf);
};

/**
 * \struct  resp_getAppInfo   消息号: logic_cmd_getAppInfo
 * \brief   获取应用详细信息返回
 */
struct resp_getAppInfo : public _sdkrespcmd_base
{
	st_tinyAppInfo info ;
	CMD_SIZE();
	COPY_OTHER(resp_getAppInfo);
};

/**
 * \struct  resp_localSetting   消息号: logic_cmd_localSetting
 * \brief   本地配置请求返回
 */
struct resp_localSetting : public _sdkrespcmd_base
{
	int8    type ;  ///< 0 : 增加，１：查询 , 2: 修改，３：删除 type.
	/**
	 * \type 的值不同，items有不同的意义
	 * 0: items里面为空
	 * 1: items里面带查询的后的输出.
	 * 2: items里面为空
	 * 3: items里面为空
	 */
	std::vector<st_localSetting> items ;
	void init() {
		items.clear();
	}
	CMD_SIZE();
	COPY_OTHER(resp_localSetting);
};

/**
 * \struct  resp_netNotifyer   消息号: logic_cmd_netNotifyer
 * \brief   网络状态通知
 */
struct resp_netNotifyer : public _sdkrespcmd_base
{
	int8 type ;  ///< 0是　网络ＯＫ，非零是网络异常  type.
	CMD_SIZE();
	COPY_OTHER(resp_netNotifyer);
};

/**
 * \struct  resp_PersonalData   消息号: logic_cmd_getPersonalData
 * \brief   获取用户个人设置
 */
struct resp_PersonalData : public _sdkrespcmd_base
{
	std::vector<st_personalData> vt; ///< 获取用户数据
    void init()
    {
    	vt.clear();
    }
    CMD_SIZE();
    COPY_OTHER(resp_PersonalData);
};

/**
 * \struct  resp_msgAmountSearch   消息号: logic_cmd_msgGlobalSearch 
 * \brief   本地消息全局搜索，只返回每个匹配到的群或个人的消息的数目。
 */
struct resp_msgGlobalSearch : public _sdkrespcmd_base
{
	std::string key;    ///< 查找的关键字.
	int64 nextTargetId; ///< 后续查找的targetId，0代表已搜索全部消息.
	std::vector<st_msgGlobalSearch> results; ///< 搜索到的targetId对象简要信息集合.
	COPY_OTHER(resp_msgGlobalSearch);
	void init()
	{
		results.clear();
	}
	CMD_SIZE();
};

/**
 * \struct  resp_msgDetailSearch  消息号: logic_cmd_msgDetailSearch 
 * \brief   在某个群或个人的聊天记录中搜索关键字。
 */
struct resp_msgDetailSearch : public _sdkrespcmd_base
{
	int64 targetId; ///< 搜索的目标Id.
	int64 nextMsgId; ///< 后续查找的消息ID，0代表已搜索全部消息.
	std::string key; ///< 查找的关键字.
	std::map<int64, st_simpleSearchInfo> senderInfos; ///< 存放此次搜索结果中的所有发送者信息,key为senderUserId.
	std::vector<st_msgBean> msgs; ///< 保存所有消息内容信息.

	void init()
	{
		senderInfos.clear();
		msgs.clear();
	}
	CMD_SIZE();
	COPY_OTHER(resp_msgDetailSearch);
};

/**
 * \struct  resp_getBWlist  消息号: logic_cmd_getBWlist 
 * \brief   获取黑名单或白名单列表。
 */
struct resp_getBWlist : public _sdkrespcmd_base
{
	std::vector<int64> memList;

	void init()
	{
		memList.clear();
	}
	CMD_SIZE();
	COPY_OTHER(resp_getBWlist);
};

/**
* \struct  resp_getFileInfos 消息号　logic_cmd_getFileInfos
* \brief 得到保存文件信息
*/
struct resp_getFileInfos : public  _sdkrespcmd_base
{
	std::map<int64, st_Fileinfo> fileInfos;	 ///< 获取的文件详细信息,key为文件消息ID.

	void init()
	{
		fileInfos.clear();
	}

	CMD_SIZE();
	COPY_OTHER(resp_getFileInfos);
};

/**
* \struct  resp_getMsgByType 消息号　logic_cmd_getMsgByType
* \brief 根据消息类型获得聊天消息
*/
struct resp_getMsgByType : public  _sdkrespcmd_base
{
	std::vector<st_msgBean> msgs;		///< 返回的消息集合.

	void init()
	{
		msgs.clear();
	}

	CMD_SIZE();
	COPY_OTHER(resp_getMsgByType);
};

/**
* \struct  resp_getUrlInfo 消息号　logic_cmd_getUrlInfo
* \brief 获取网址缩略图信息
*/
struct resp_getUrlInfo : public _sdkrespcmd_base
{
	std::string url;		///< 网址.
	std::string title;		///< 解析的标题.
	std::string img;		///< 解析的图片地址.
	std::string desc;		///< 解析的摘要.

	CMD_SIZE();
	COPY_OTHER(resp_getUrlInfo);
};

/**
* \struct  resp_faceToFaceJoin 消息号　logic_cmd_faceToFaceJoin
* \brief 近距离加好友，群
*/
struct resp_faceToFaceJoin : public _sdkrespcmd_base
{	
	int8 result; ///< 1：加入或者创建成功，2：创建失败，精度、纬度、房间密码不符合要求 3：已与房主是好友关系, 4:房间不存在.
	std::string roomId; ///< 房间ID.
	std::vector<st_smallUserBean> userList; ///< 返回此房间的用户列表，主要信息用户ID,用户名，图像，如果是近距离加好友只返回房主用户信息.
	std::string randomCode; ///< 随机码,创建房间时返回.

	void init()
	{
		userList.clear();
	}
	
	CMD_SIZE();
	COPY_OTHER(resp_faceToFaceJoin);
};

/**
* \struct  resp_faceJoinGroup 消息号　logic_cmd_faceJoinGroup, logic_cmd_faceAddBuddy
* \brief 进入房间推送
*/
struct resp_faceJoinGroup : public _sdkrespcmd_base
{
	std::string roomId;			///< 房间ID.
	st_smallUserBean userInfo;	    ///< 进入房间的用户的信息.

	void init()
	{
		userInfo.emails.clear();
		userInfo.phoneNums.clear();
	}
	
	CMD_SIZE();
	COPY_OTHER(resp_faceJoinGroup);
};

/**
* \struct  resp_exitFaceJoinGroup 消息号　logic_cmd_exitFaceJoinGroup, logic_cmd_exitFaceAddBuddy
* \brief 退出房间推送
*/
struct resp_exitFaceJoinGroup : public _sdkrespcmd_base
{
	std::string roomId;			///< 房间ID.
	int64 userId;				    ///< 退出人员ID.
	int8 deleteRoom;			    ///< 1：退出并解散房间，其他值退出房间.

	CMD_SIZE();
	COPY_OTHER(resp_exitFaceJoinGroup);
};

/**
 *\struct resp_MessageCursor 消息号 logic_cmd_MessageCursor
 *\消息游标推送
 */
struct resp_MessageCursor :public _sdkrespcmd_base
{
	int64 userID;  ///<本人用户ID
	int64 targetID;///<目标ID(单聊人ID,群聊群组ID)
	int64 maxReadID;///<所有设备中最大的消息已读ID

	CMD_SIZE();
	COPY_OTHER(resp_MessageCursor);
};
/**
* \struct  resp_createFaceGroupSuccess 消息号　logic_cmd_createFaceGroupSuccess, logic_cmd_faceAddBuddyConfirm
* \brief 创建群或加好友成功后的推送
*/
struct resp_createFaceGroupSuccess : public _sdkrespcmd_base
{
	std::string roomId;			///< 房间ID.
	int64 groupID;				///< 群ID.
	int8 joinFlag;				///< 加入标识, 1:加入成功, 2:取消加入.

	CMD_SIZE();
	COPY_OTHER(resp_createFaceGroupSuccess);
};

/**
* \struct  resp_authenticationResult 消息号　logic_cmd_bindEmailOrPhoneNumber1
* \brief 绑定手机号第1步返回值
*/
struct resp_authenticationResult : public _sdkrespcmd_base
{
	resp_authenticationResult()
	{
	}

	int32  reregistryTimeout;			    ///< 重新发短信或邮件的间隔, 默认 60秒.
	int64  registryID;					///< 豆豆ID.
	std::string sendNumber;				///< 发送短信或者邮件的号码或者邮箱.
	std::string nation;				    ///< 国家编码, 默认是0086.

	CMD_SIZE();
	COPY_OTHER(resp_authenticationResult);
};

/**
* \struct  resp_getNoDisturbMode 消息号　logic_cmd_setNoDisturbMode
* \brief 设置全局勿扰模式开关信息
*/
struct  resp_getNoDisturbMode : public _sdkrespcmd_base
{
	resp_getNoDisturbMode() :startTime(0), endTime(0), flag(0)
	{
	}

	int32 startTime;	    ///< 起始时间.
	int32 endTime;		///< 结束时间.
	int8  flag;			///< 开关是否打开，0关闭，1打开.

	CMD_SIZE();
	COPY_OTHER(resp_getNoDisturbMode);
};

/**
* \struct  resp_getUserToTargetSwitch 消息号　logic_cmd_getUserToTargetSwitch
* \brief 获取个人对目标用户的勿扰模式
*/
struct  resp_getUserToTargetSwitch : public _sdkrespcmd_base
{
	resp_getUserToTargetSwitch() :targetId(0), type(0), value1(0),value2(0)
	{
	}

	int64 targetId;	///< 用户ID.
	int8  type;		///< 设置类型 0.全部获取 1:个人设置项 2:个人通知消息详情模式.
	int8  value1;		///< 个人设置项, type为0和1时赋值， value1: 1为接收提醒 2表示不提醒仅显示数字 3:为免打扰, 默认1.
	int8  value2;		///< 个人通知消息详情模式, type为0和2时赋值, value2:1、通知详情 2、通知源，隐藏内容 3、完全隐藏 默认开启模式1.

	CMD_SIZE();
	COPY_OTHER(resp_getUserToTargetSwitch);
};

/**
* \struct  resp_postContactResult 消息号　logic_cmd_postContact
* \brief 上传通讯录返回信息
*/

struct resp_postContactResult : public _sdkrespcmd_base
{
	void init()
	{
		recommends.clear();
	}

	std::vector<st_autoAttentionBean>  autoAttents;
	std::vector<st_recommendContact>   recommends;   ///< 推荐者,那些无法自动加好友的人.                    

	CMD_SIZE();
	COPY_OTHER(resp_postContactResult);
};

/**
* \struct  resp_commonListResult 消息号　logic_cmd_getShareOptions
* \brief 前端用户得到分享列表
*/
struct resp_commonListResult : public _sdkrespcmd_base
{
	resp_commonListResult()
	{
		memList.clear();
	}
	void init()
	{
		memList.clear();
	}
	
	std::vector<int64> memList;   ///< ID列表.
	CMD_SIZE();
	COPY_OTHER(resp_commonListResult);
};

/**
* \struct  resp_getPersonDefinitionResult 消息号　logic_cmd_getPersonDefinition
* \brief 得到用户自定义一些信息
*/
struct resp_getPersonDefinitionResult : public _sdkrespcmd_base
{
	resp_getPersonDefinitionResult()
	{
		personDef.clear();
	}
	void init()
	{
		personDef.clear();
	}
	std::vector<st_personDefinition> personDef;

	CMD_SIZE();
	COPY_OTHER(resp_getPersonDefinitionResult);
};

/**
* \struct  resp_queryExtendedField 消息号　logic_cmd_queryExtendedField
* \brief  根据条件查询拓展字段信息
*/
struct resp_queryExtendedField : public _sdkrespcmd_base
{
	void init()
	{
		listEnDic.clear();
	}
	std::vector<st_enterpriseDictionary>  listEnDic;

	CMD_SIZE();
	COPY_OTHER(resp_queryExtendedField);
};

/**
* \struct resp_bkImgUpdate 消息号 logic_cmd_bkImgUpdate
* \brief 背景图片修改推送
*/
struct resp_bkImgUpdate : public _sdkrespcmd_base {
	void init() {
		;
	}
	int64   targetID ;
	std::string img ;
};

/**
* \struct resp_queryMarketApplication 消息号 logic_cmd_queryMarketApplication
* \brief 分页查询应用市场应用信息
*/
struct resp_queryMarketApplication : public _sdkrespcmd_base
{
	st_smallMarketAppPage  marketApp;
	void init() {
		marketApp.appInfoList.clear();;
	}
	CMD_SIZE();
	COPY_OTHER(resp_queryMarketApplication);
};

/**
* \struct resp_bkImgUpdate 消息号 logic_cmd_getInstalledApplication
* \brief 获取用户已安装列表信息
*/
struct resp_getInstalledApplication : public _sdkrespcmd_base
{
	void init() {
		appInfoList.clear();
	}
	std::vector<st_smallMarketAppInfo> appInfoList;  ///< 应用列表.
	CMD_SIZE();
	COPY_OTHER(resp_getInstalledApplication);
};
/**
* \struct resp_getEntAppInfo 消息号 logic_cmd_getEntAppInfo
* \brief 根据用户id分页获取企业号
*/
struct resp_getEntAppInfo : public _sdkrespcmd_base
{
	void init() {
		appList.clear();
	}
	int64 totalCount;                         ///< 总数量
	int64 totalPage;                          ///< 总页数
	std::vector<st_tinyAppInfo> appList;      ///< 企业号集合

	CMD_SIZE();
	COPY_OTHER(resp_getEntAppInfo);
};

/**
* \struct resp_queryEmoticon 消息号 logic_cmd_queryEmoticon
* \brief  单个表情查询、根据表情包标识查询表情，查询所有用户自定义表情
* \	返回：code:　0:成功，100:参数错误，11100:内部错误
*/
struct resp_queryEmoticon : public _sdkrespcmd_base
{
	void init() {
		emotlist.clear();
	}
	std::vector<st_emoticon> emotlist;

	CMD_SIZE();
	COPY_OTHER(resp_queryEmoticon);
};

/**
* \struct resp_querySingleEmoticon 消息号 logic_cmd_querySingleEmoticon
* \brief  查询某个表情包里单个表情信息
*/
struct resp_querySingleEmoticon : public _sdkrespcmd_base
{
	resp_querySingleEmoticon()
	{

	}
	std::string packMdCode;           ///< 表情包MdCode
	std::string mainUrl;              ///< 大图地址
	std::string meaning;              ///< 含义
	std::string mdCode;               ///< 单个表情MdCode
	int8 type;                        ///< 表情类型 1：静态表情2：动态表情
};
/**
* \struct resp_singleQueryEmoticonPackage 消息号 logic_cmd_singleQueryEmoticonPackage
* \brief  单个查询表情包
*/
struct resp_singleQueryEmoticonPackage : public _sdkrespcmd_base
{
	st_emoticonPackage emotPackage;
	CMD_SIZE();
	COPY_OTHER(resp_singleQueryEmoticonPackage);
};

/**
* \struct req_pageQueryEmoticon 消息号 logic_cmd_pageQueryEmoticon
* \brief  分页查询表情包
*/
struct resp_pageQueryEmoticon : public _sdkrespcmd_base
{
	void init()
	{
		emotItem.clear();
	}
	int32 pageNum;     ///< 请求页码.
	int16 pageSize;    ///< 每页大小.
	int64 totalPage;   ///< 总页数.
	int64 totalItems;  ///< 总条数.
	std::vector<st_emoticonPackage> emotItem; //当前页数据

	CMD_SIZE();
	COPY_OTHER(resp_pageQueryEmoticon);
};

/**
* \struct req_addOrDeleteCustomEmoticon 消息号 logic_cmd_addOrDeleteCustomEmoticon
* \brief  增加或删除用户自定义表情
* \返回值：	添加：code　0:成功，100:参数错误，801:md5Code不正确，802:md5Code文件已存在，11100:内部错误
* \	         删除：code　0:成功，100:参数错误，801:md5Code不正确，11100:内部错误
*/
struct resp_addOrDeleteCustomEmoticon : public _sdkrespcmd_base
{
	void init()
	{
		successList.clear();
		failList.clear();
	}

	int32 resultCode;         ///< 0操作成功 801:md5Code不存在 802 : md5文件已存在 803 : 批量新增表情失败
	std::string resultMessage; ///< 结果信息.
	std::vector<st_emoticonResult> successList; ///< 批量成功ID记录.
	std::vector<st_emoticonResult> failList;    ///< 批量失败ID记录.

	CMD_SIZE();
	COPY_OTHER(resp_addOrDeleteCustomEmoticon);
};

/**
* \struct resp_labelQueryEmoticonPackage 消息号 logic_cmd_labelQueryEmoticonPackage
* \brief  单个查询表情包
*/
struct resp_labelQueryEmoticonPackage : public _sdkrespcmd_base
{
	void init()
	{
		emotPackageList.clear();
	}
	std::vector<st_emoticonPackage> emotPackageList;
	CMD_SIZE();
	COPY_OTHER(resp_labelQueryEmoticonPackage);
};

/**
* \struct resp_queryAppOrDictTimestamp 消息号 logic_cmd_queryAppOrDictTimestamp
* \brief 查询机器人（公众号）时间戳，查询数据字典时间戳
*/
struct resp_queryAppOrDictTimestamp : public _sdkrespcmd_base
{
	resp_queryAppOrDictTimestamp() : timeStamp(0)
	{
	}
	int64 timeStamp;       //时间戳
	CMD_SIZE();
	COPY_OTHER(resp_queryAppOrDictTimestamp);
};

/**
* \struct resp_setUnReadCount 消息号 logic_cmd_setUnReadCount
* \brief 设置客户端消息未读数量 
*/
struct resp_setUnReadCount : public _sdkrespcmd_base
{
	resp_setUnReadCount()
	{

	}
	CMD_SIZE();
	COPY_OTHER(resp_setUnReadCount);
};

/**
* \struct resp_setMsgUnRead 消息号： logic_cmd_setMsgUnRead
* \brief 设置最后一条已读为未读
*/
struct resp_setMsgUnRead : public _sdkrespcmd_base
{
	resp_setMsgUnRead()
	{

	}
	CMD_SIZE();
	COPY_OTHER(resp_setMsgUnRead);
};

/**
* \struct  resp_getP2pUser 消息号　logic_cmd_getP2pUsers
* \brief 得到可以P2P通讯的用户
*/
struct resp_getP2pUser : public _sdkrespcmd_base
{
	resp_getP2pUser()
	{
		users.clear();
	}
	std::vector<st_p2pUser> users;

	CMD_SIZE();
	COPY_OTHER(resp_getP2pUser);
};

/**
* \struct  resp_P2pUserStatusChg 消息号　logic_cmd_P2pUserStatusChg
* \brief p2p用户的状态改变
*/
struct resp_P2pUserStatusChg : public _sdkrespcmd_base
{
	resp_P2pUserStatusChg()
	{
		userID = 0 ;
		device = 0 ;
		status = 0 ;
	}
	int64  userID ; ///< 用户ID.
	int8   device ; ///< 0x01 PC , 0x02 移动终端.
	int8   status ; ///< 上线 0 ， 下线， 1.

	CMD_SIZE();
	COPY_OTHER(resp_P2pUserStatusChg);
};

/**
 *	\struct resp_P2pSendFileReq 消息号 logic_cmd_p2pSendFilereq
 *	\brief P2P用户发送文件请求
 */
struct resp_P2pSendFileReq: public _sdkrespcmd_base
{
	resp_P2pSendFileReq() {

	}
    int8        fileType; ///< 1:文件, 2:目录.
	std::string fileName ; ///< 文件名称.
	int64       fileSize ; ///< 文件大小.
	/**
	 * 发送文件的标识,响应的时候需要回传回去
	 */
	int64       fileSign ; ///< 发送标识.
	int64       sendUserID;///< 发送者ID.
	CMD_SIZE();
	COPY_OTHER(resp_P2pSendFileReq);
};

/**
 *	\struct resp_P2pFileTransOp 消息号 logic_cmd_p2pFileTransOps
 *	\brief P2P用户文件传输操作
 */
struct resp_P2pFileTransOp: public _sdkrespcmd_base
{
	resp_P2pFileTransOp() {
	}
	int8    opType; /// <接收 0,  取消 1.
	int64   fileSign; ///< 发送标识taskID.

	CMD_SIZE();
	COPY_OTHER(resp_P2pFileTransOp);
};

/**
*	\struct resp_getIdByAccount 消息号 logic_cmd_getIdByAccount 
*	\brief 通过账号名获取用户ID
*/
struct resp_getIdByAccount : public _sdkrespcmd_base
{
	resp_getIdByAccount() {
	}
	
	void init()
	{
		userIds.clear();
	}
	std::map<std::string, int64> userIds;	//用户ID集合，key为account

	CMD_SIZE();
	COPY_OTHER(resp_getIdByAccount);
};

/**
   \struct resp_getMsgDays 
   \brief 获取聊天消息日期
*/
struct resp_getMsgDays : public _sdkrespcmd_base{
	///聊天日期数组
	std::vector<YMD> vt_days ;
	///每天的消息数 数组下标同 vt_days 一一对应
	std::vector<int32> vt_cnt  ;
    void init() {
		vt_days.clear();
		vt_cnt.clear();
	}
	CMD_SIZE();
	COPY_OTHER(resp_getMsgDays);
};

/**
   \struct resp_getMsgsFromDay 
   \brief 根据日期获取聊天消息
*/
struct resp_getMsgsFromDay: public _sdkrespcmd_base {
	std::vector<st_msgBean>  _vt;
	void init() {
		_vt.clear();
	}
	CMD_SIZE();
	COPY_OTHER(resp_getMsgsFromDay);
};

/**
\struct resp_stunServerInfo
\brief 获取打洞服务地址
*/
struct resp_getStunServer: public _sdkrespcmd_base {
	std::string stunAddr;
	std::string stunPort;
	std::string turnAddr;
	std::string turnPort;
	std::string account;
	std::string password;
	CMD_SIZE();
	COPY_OTHER(resp_getStunServer);
};
/**
\struct resp_agoraFunc
\brief 声网服务相关接口
*/
struct resp_agoraFunc :public _sdkrespcmd_base{
	int retCode;
	std::string vendorKey;
	std::string subKey;
	CMD_SIZE();
	COPY_OTHER(resp_agoraFunc);
};
}

#endif   //CMD_RESP_DEF_H
