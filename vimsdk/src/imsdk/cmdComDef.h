#ifndef CMD_COM_DEF_H_
#define CMD_COM_DEF_H_

////////////////////////////////////////////////////////////////////////////////
/// Copyright (C), 2015-2016
/// \file   	cmdComDef.h
/// \brief  	SDK通用结构体头文件
/// \author 	sharp.young
/// \version    0.2.1
/// \data       2016/1/10
/// \warning    消息号携带的数据结构，必须继承此结构体，如果结构体中有容器类的成员变量，“必须”重载init方法，完成清理工作
///             比如： 
///             struct test : public _sdkcmd_base {
///					std::vector<x> prop1 ;
///					void init() {
///						prop1.clear();
///					}
///				}
/// \warning    成员变量结构体(比如"st_"开头的结构体)里面含有容器类数据的话，该成员变量结构体须提供清理方法 需要将容器清理
/// \Function List
/// \History:
///  	<author> 		<time>    	   <version >    	<desc>
///
////////////////////////////////////////////////////////////////////////////////

#include <string>
#include <vector>
#include <map>
#include <sstream>
#include "config.h"

namespace imsdk
{
	/**
	*  \def 结构体长度宏定义
	*/
#define CMD_SIZE() \
	virtual int32 getSize() { return sizeof(*this); }
#define COPY_OTHER(T) \
	virtual bool copyfrom(_sdkrespcmd_base * pOther) { *this = *((T*)pOther); return true; }
	/**
	 * 定义结构体:
	 *  struct Example {
	 *  	Example(){
	 *  		INIT_ISSET(test)
	 *  	}
	 *  	IS_SET(std::sting,test);
	 *  }
	 *
	 *  //使用
	 *  Example  myTest;
	 *  myTest._set_test("this is test");
	 *  printf("myTest.test = %s,",myTest.test.c_str());
	 *
	 *  //输出
	 *  myTest.test = this is test",
	 */
#define  IS_SET(type,x) \
		type  x;		 \
		bool isSet_##x ; \
		void _set_##x(type & val) {   \
			x = val ;                  \
			isSet_##x = true ;        \
		}
#define INIT_ISSET(x) \
		isSet_##x = false;
#define INIT_ISSETEX(x,y) \
		isSet_##x = y;

/**
* \struct _sdkcmd_base
* \biref  所有消息的基类
* 
*/
struct _sdkcmd_base 
{
	_sdkcmd_base():localID(0) {
		
	}
	virtual ~_sdkcmd_base(){}
	virtual void init() { localID = 0; }
	///本地唯一编号
	int64  localID;
	CMD_SIZE();
	};

/**
 * \struct _sdkrespcmd_base
 * \biref 所有返回消息基类
*/
struct _sdkrespcmd_base : public _sdkcmd_base 
{
	_sdkrespcmd_base() :code(0) {}
	virtual ~_sdkrespcmd_base(){}
	virtual void init() 
	{
		code = 0;
		_sdkcmd_base::init();
	}
	virtual bool copyfrom(_sdkrespcmd_base * pOther) {
		*this = *pOther;
		return true ;
	}
	///响应返回值
	int32  code;
	CMD_SIZE();	
};

/**
*  \struct resp_commonResult
*  \biref  通用返回结构体, 具体含义各个消息或有不同
*/
struct resp_commonResult : public _sdkrespcmd_base 
{
	resp_commonResult() :value_byte(0),
						 value_i16(0),
	                     value_i32(0),
	                     value_i64(0)
	{}
	std::string 	what;
	int8 			value_byte;
	int16 			value_i16;
	int32 			value_i32;
	int64		    value_i64;
	std::string 	value_str;
	CMD_SIZE();
	COPY_OTHER(resp_commonResult);
	};

	/**
	 * \struct st_tinyUserBean
	 * \biref  用户基础信息，陌生人信息可以使用
	 */
	struct st_tinyUserBean
	{
		st_tinyUserBean() :sex(0),
		timezone(0),
		userid(0),
		birthday(0)
		{
			INIT_ISSET(sex)
			INIT_ISSET(timezone)
			INIT_ISSET(userid)
			INIT_ISSET(birthday)
			INIT_ISSET(name)
			INIT_ISSET(pinyin)
			INIT_ISSET(avatar)
			INIT_ISSET(avatarUrl)
			INIT_ISSET(srcAvatar)
			INIT_ISSET(srcAvatarUrl)
			INIT_ISSET(sign)
			INIT_ISSET(area)
			INIT_ISSET(nickID)
			INIT_ISSET(phones)
			INIT_ISSET(emails)
		}
		///性别
		IS_SET(int, sex)
		///时区
		IS_SET(int, timezone)
		///用户ID
		IS_SET(int64, userid)
		///生日
		IS_SET(int64, birthday)
		///名称
		IS_SET(std::string, name)
		///拼音
		IS_SET(std::string, pinyin)
		///头像
		IS_SET(std::string, avatar)
		IS_SET(std::string, avatarUrl)
		///大图头像
		IS_SET(std::string, srcAvatar)
		IS_SET(std::string, srcAvatarUrl)
		///签名
		IS_SET(std::string, sign)
		///区域
		IS_SET(std::string, area)
		///豆豆号
		IS_SET(std::string, nickID)
		///电话号码清单
		IS_SET(std::vector<std::string>, phones)
		///邮件清单
		IS_SET(std::vector<std::string>, emails)

		void init()
		{
			INIT_ISSET(sex)
			INIT_ISSET(timezone)
			INIT_ISSET(userid)
			INIT_ISSET(birthday)
			INIT_ISSET(name)
			INIT_ISSET(pinyin)
			INIT_ISSET(avatar)
			INIT_ISSET(avatarUrl)
			INIT_ISSET(srcAvatar)
			INIT_ISSET(srcAvatarUrl)
			INIT_ISSET(sign)
			INIT_ISSET(area)
			INIT_ISSET(nickID)
			INIT_ISSET(phones)
			INIT_ISSET(emails)
			phones.clear();
			emails.clear();
		}
		bool operator == (const st_tinyUserBean& rhs) const
		{
			return (sex == rhs.sex &&
				timezone == rhs.timezone &&
				userid == rhs.userid &&
				birthday == rhs.birthday &&
				name == rhs.name &&
				pinyin == rhs.pinyin &&
				avatar == rhs.avatar &&
				avatarUrl == rhs.avatarUrl &&
				srcAvatar == rhs.srcAvatar &&
				srcAvatarUrl == rhs.srcAvatarUrl &&
				sign == rhs.sign &&
				area == rhs.area &&
				nickID == rhs.nickID &&
				phones == rhs.phones &&
				emails == rhs.emails
				);
		}
	};

	/**
	 * \struct st_userBean
	 * \brief 用户的扩展信息
	 * IS_SET 标识需要使用函数来设置该类型，不设置的话，不会发送到服务器
	 * 比如 IS_SET(std::string,extend)
	 *
	 *    st_userBean param;
	 *    std::string newExtent="this is test";
	 *    param._set_extend(newExtent);
	 */
	struct st_userBean
	{
		st_userBean() {
		INIT_ISSET(extend)
		INIT_ISSET(entExtend)
		INIT_ISSET(info)
		}
		void init()
		{
			INIT_ISSET(extend)
			INIT_ISSET(entExtend)
			INIT_ISSET(info)
			info.init();
		}

		///扩展信息
		IS_SET(std::string, extend)
		///企业扩展
		IS_SET(std::string, entExtend)
		///基础信息
		IS_SET(st_tinyUserBean, info)
		bool operator == (const st_userBean& rhs) const
		{
				return (extend == rhs.extend && info == rhs.info  && entExtend == rhs.entExtend);
		}
	};

	/**
	 * \struct st_buddyBean
	 * \brief 好友数据结构
	 */
	struct st_buddyBean
	{
		st_buddyBean() :page(0), isstar(0){}
		int32 page;				///< 好友页 page.
		///是否特别/星标好友 1是，该字段含有0x1000标识为应用，含有0x2000标示为隐藏好友，含有0x4000表示置顶
		int32 isstar;
		std::string remark;			///< 备注 remark.
		std::string remarkPy;		///< 备注拼音 remarkPy.
		std::string chatImage;		///< 聊天背景 chatImage.
		std::string srvName;		///< 服务器名称 srvName,如果是空，则是当前服务器
		std::string privateMsgPwd;	///< 私信密码 privateMsgPwd.
		st_tinyUserBean info;		///< 基础信息 info.
		bool operator == (const st_buddyBean& rhs) const
		{
			return (page == rhs.page &&
				info == rhs.info &&
				isstar == rhs.isstar &&
				remark == rhs.remark &&
				remarkPy == rhs.remarkPy &&
				chatImage == rhs.chatImage &&
				privateMsgPwd == rhs.privateMsgPwd
				);
		}		
	};

	/**
	 *	\struct st_smallUserBean
	 *	\brief  用户信息，好友使用。
	 */
	struct st_smallUserBean
	{
		std::string phoneNum; 	///< 电话号码,可以删除 phoneNum.
		std::string email;	  	///< 电子邮件， 可以删除 email.
		std::string name;	  	///< 用户名   name.
		int8 sex;			  	///< 性别 sex.
		/**
		 * \brief 账号状态
		 * 用户状态 1：正常 2：停用 3: 注册邀请4: 影子用户，5:锁定，6:冻结
		 */
		int8 status;			///< 状态 status.
		/**
		 * \brief 好友头像
		 * 推送上去只是文件名部分
		 */
		std::string portraitURL;
		std::string portraitURLUrl;
		std::string sign;		///< 个性签名 sign.
		int64 userID;			///< 用户ID userID.
		int64 qqNum;			///< QQ号码 qqNum.
		/**
		 * \brief 匹配查询类型
		 * 1:qqNum 2:phoneNum 4:email 8:userID 16:name
		 */
		int8 match;
		std::vector<std::string>  phoneNums; ///< 电话号码清单 phoneNums.
		std::vector<std::string>  emails;	 ///< 电子邮件清单 emails.
		/**
		 * \brief 原始好友头像
		 * 推送上去只是文件名部分
		 */
		std::string oriPortraitURL;	///< 原始好友头像 oriPortraitURL.
		std::string oriPortraitURLUrl;
		int64 birthday;
		int16 year;					///< 生日年 year.
		int8 month;					///< 生日月 month.
		int8 day;					///< 生日天 day.
		std::string accountName;	///< 账号名称 accountName.
		std::string srvName;       ///< 服务器名称 srvName;
	};

	/**
	 * \struct st_reContact
	 * \brief  最近联系人信息
	 */
	struct  st_reContact
	{
		st_reContact() :
		targetID(0),
		lastMsgid(0),
		time(0),
		type(0),
		status(0),
		msgType(0),
		activeType(0),
		unreadCnt(0),
		lastAtMsgID(0),
		msgRemindMode(0){}

		int64 targetID;   		///< 目标用户ID targetID.
		int64 lastMsgid;        ///< 最后一次消息ID lastMsgid.
		int64 time;				///< 消息时间 time
		int8  type;				///< 会话类型 1是个人会话，２是群会话 type.
		/**
		 * \brief 是否置顶
		 * 0x01男，0x02女,0x04置顶,0x08为群，前两位都为0 非男非女为保密
		 */
		int8  status;
		
		/*消息提醒模式 与0x0F 0.免打扰 1.提醒始终有声音  2.提醒始终无声音  3.提醒  4.不提醒，仅显示条目 5.根据免打扰时间段判断是否免打扰
		*与 0xF0, 1、通知详情  2、通知源，隐藏内容  3、完全隐藏*/
		int8 msgRemindMode;
		
		/**
		 * \brief 消息类型
		 * 分为两部分
		 * 1. 基本部分 messageType & 0x00FF
		 *    1: html , 2:文本 , 3: 音频 , 4: 位置 , 5:图片 , 6: 文件 , 7: 名片, 8: 弱提示
		 * 2. 附加部分 messageType & 0FF00
		 *    0x100: 为发送带附件的消息，附件上传失败会带有此标识 如图片，文件，音频等。
		 *    0x200: 为消息体本身发送失败，消息会含有此记录.
		 *    0x400: 为接收到带附件的消息，接收附件失败。
		 */
		int16  msgType;
		int32  activeType;		 ///< 激活类型， 当前未用 activeType.
		int16  unreadCnt;        ///< 未读数量   unreadCnt.
		int16  realUnReadCnt; ///< 真正的未读消息数量 = unReadCnt+弱提示消息数量+其它端同步的设备消息数量，客户端每次拉取未读用这个字段，当realUnReadCnt为0时，表示所有消息在当前设备客户端都已经看过了
		/**
		 * \brief 头像
		 * 推送上去只有文件名部分
		 */
		std::string avatar;
		std::string avatarUrl;  ///< 服务器全路径
		std::string nickName;	///< 显示名称 nickName.
		std::string lastMsg;    ///< 最后一条消息描述 lastMsg.
		std::string whereFrom;  ///< 发送者名称,在群的时候有用 whereFrom.
		std::string msgProp;    ///< 消息属性
		std::string sourceID ;  ///< 消息附加源
		int64       lastAtMsgID; ///< 最后一条@你的消息,如果为零，则标识@的消息已读
		int64       sendUserID;  ///< 发送者ID
	}; 

	/**
	 * \struct st_onlineState
	 * \brief 好友状态
	 */
	struct  st_onlineState
	{
		int64 userID;     ///< 用户ID userID.
		int64 connectID;  ///< 连接ID connectID.
		/**
		 * \brief 设备类型
		 * 1：PC；2：手机；3：pad；4：网页
		 */
		int8 deviceType;
		/**
		 * \brief 设备状态标识
		 * -1：离线，1:在线；2:隐身,3:忙碌,4:离开,5:请勿打扰, 6:想聊天
		 */
		int8 flag;
	};

	/**
	 * \struct  st_PersonalData
	 * \brief   个人信息结构体
	 */
	struct st_personalData
	{
		/**
		 * \brief type:   1 (生日)，２（电话），３（邮件）
		 * 　　　　　　　　value:  1：所有人可见 2：仅好友可见 3：仅自己可见，默认1
		 * type:   4 (好友验证方式)
		 * 　　　　　　　 value:   1：需要验证信息,2:不允许任何人添加,3:允许任何人添加，默认1
		 * type:   5 V标
		 *         value: 1:表示始终有声音提醒，2：表示始终无声音提醒 3:不始终提醒，默认1
		 * type:   6 @相关人提醒模式
		 *         value: 1:表示始终有声音提醒，2：表示始终无声音提醒 3:不始终提醒，默认1
		 * type:   7 全局通知消息内容展现模式
		 *         value: 1:通知详情，2：通知源，隐藏内容 3:完全隐藏，默认2
		 */
		int16 type; ///< 类型 type.
		int16 val; ///< 值 val.
	};

	/**
	 * \struct st_hiddenAccountInfo
	 * \brief 隐藏账户的信息
	 */
	struct st_hiddenAccountInfo
	{
		int64 targetID;			///< 隐藏ID，可以是群,也可以是单个人  targetID.
		int64 sendMsgUserID;		///< 消息发送者ID sendMsgUserID.
		std::string message;		///< 发送的消息 message.
	};

	/**
	 *  \struct st_tinyBuddyBean
	 *  \brief  用户的缩略信息
	 */
	struct st_tinyBuddyBean
	{
		st_tinyBuddyBean() :flag(0), sex(0), userid(0)
		{
			INIT_ISSET(sex)
			INIT_ISSET(flag)
			INIT_ISSET(name)
			INIT_ISSET(avatar)
			INIT_ISSET(avatarUrl)
			INIT_ISSET(remark)
			INIT_ISSET(sign)
			INIT_ISSET(pinyin)
			INIT_ISSET(status)
			INIT_ISSET(srvName)
			INIT_ISSET(area)
			INIT_ISSET(birthday)
		}

		void init()
		{
			INIT_ISSET(sex)
			INIT_ISSET(flag)
			INIT_ISSET(name)
			INIT_ISSET(avatar)
			INIT_ISSET(avatarUrl)
			INIT_ISSET(remark)
			INIT_ISSET(sign)
			INIT_ISSET(pinyin)
			INIT_ISSET(status)
			INIT_ISSET(srvName)
			INIT_ISSET(area)
			INIT_ISSET(birthday)
		}

		int64 userid;			///< 用户ID 	    userid.
		/**
		 * \brief 性别
		 * 1男，2女 其他保密
		 */
		IS_SET(int8, sex)
		/**
		 * \brief 星标好友
		 * 与上0x0001 = 1是星标好友， 该字段含有0x1000为应用, 0x2000标示是否为隐藏好友, 0x4000表示是否置顶
		 */
		IS_SET(int32, flag)
		IS_SET(std::string, name)		///< 用户名称 	name.
		/**
		 * \brief 用户头像
		 * 上传上去只是文件名，需要获取头像路径，拼接出路径
		 */
		IS_SET(std::string, avatar)
		IS_SET(std::string, avatarUrl)  ///< 好友头像服务器全路径
		IS_SET(std::string, remark)		///< 备注      remark.
		IS_SET(std::string, sign)		///< 个性签名   sign. 
		/**
		 * \brief 拼音，remark有值是remark的拼音，remark没值是name的拼音
		 * 格式 简拼$$全拼
		 * 比如 “连豆豆” 这个中文
		 * 简拼：ldd
		 * 全拼: liandoudou
		 * 该字段的值为 ldd$$liandoudou, 使用的时候注意分割
		 */
		IS_SET(std::string, pinyin)
		/**
		 * \brief 用户状态
		 * 1:正常,2:停用,3:注销,8:删除,16:隐藏
		 */
		IS_SET(std::string, status)
		///服务器名称
		IS_SET(std::string, srvName)	
		///地区
		IS_SET(std::string, area)
		///生日
		IS_SET(int64, birthday)
	};

	/**
	 * \struct st_msgBean
	 * \brief 消息结构
	 */
	struct  st_msgBean
	{
		int64 sendUserID;     ///< 消息发送人员ID sendUserID.
		int64 receTargetID;   ///< 消息接收者，有可能是自己，有可能是群 receTargetID.
		int64 sendTime;       ///< 发送时间 　　sendTime.
		int64 targetID;       ///< 肯定不是自己，有可能是别人或者群　targetID.
		std::string message;  ///< 消息内容 message.
		int8   isBurn;		  ///< 0x01为阅后即焚 isBurn.
		/**
		 * \brief 消息类型
		 * 分为两部分
		 * 1. 基本部分 messageType & 0x00FF
		 *    1: html , 2:文本 , 3: 音频 , 4: 位置 , 5:图片 , 6: 文件 , 7: 名片, 8: 弱提示
		 * 2. 附加部分 messageType & 0FF00
		 *    0x100: 为发送带附件的消息，附件上传失败会带有此标识 如图片，文件，音频等。
		 *    0x200: 为消息体本身发送失败，消息会含有此记录.
		 *    0x400: 为接收到带附件的消息，接收附件失败。
		 *    0x1000:  消息正在发送中
		 *    0x2000： 附件正在上传中
		 * Note: 上传附件和对应发送消息的localID必须保持一致
		 */
		int16   messageType;
		int64   messageID;      ///< 消息的整个系统里的唯一ID messageID.
		std::string format;     ///< 消息的格式 format.
		int64 lastMessageID;    ///< 上一条消息ID lastMessageID.
		int64 senderSessionID;  ///< 发送消息人员的会话ID senderSessionID.
		std::vector<int64>  limitRange; ///< limitRange limitRange.
		std::string msgProperties;		///< 消息属性　msgProperties.
		int8 activeType;                ///< 活动使用 ID activeType.
		std::vector<int64>  relatedUsers;///< @的人
		int64 relatedMsgID;             ///< 活动的响应 relatedMsgID.
		int64 localID;
		///陌生人单聊时的来源ID，1_groupID表示传的群ID，2_orgID表示传的组织ID
		std::string sourceID ;
		std::string memberName ;  ///< 群聊的时候值有效, 发送成员的名称. 单聊时也有效表示消息发送人员名称
		int8 msgRemindMode;		///< 0.免打扰 1.提醒始终有声音  2.提醒始终无声音  3.提醒  4.不提醒，仅显示条目 5.根据免打扰时间段判断是否免打扰
	};

	/**
	 * \struct st_reqVerifyParam
	 * \brief  验证消息已读结构体
	 */
	struct st_reqVerifyParam
	{
		int64 reqVerifyboxID; ///< 验证ID reqVerifyboxID.
		int64 reqUserID;      ///< 发送验证请求的用户ID reqUserID.
	};

	/**
	 * \struct st_ureadMsgList
	 * \brief  未读消息推送结构体
	 */
	struct st_ureadMsgList
	{
		int32 unReadCnt;   ///< 未读消息数量  unReadCnt.
		int32 weakTipCnt;  ///< 未读中的弱提示数量 weakTipCnt.
		st_msgBean msg;    ///< 最后一条未读  msg.
	};

	/**
	 * \struct st_iosNoticeInfo
	 * \brief  IOS进入后台设置token结构体
	 */
	struct st_iosNoticeInfo
	{
		/**
		 * \brief 接收类型 1. 免打扰，2. 取消免打扰
		 */
		int8 recvType;
		std::string token; ///< IOS token.
	};

	// TODO:
	struct  st_groupValAddedAttr
	{
		int64 groupValueAddedAttr;
		int32 groupMemberNum;
	};

	/**
	 * \struct st_orgUserinfo
	 * \brief  组织用户信息（搜索中使用）
	 */
	struct st_orgUserinfo
	{
		st_orgUserinfo() :
		userid(0){}

		int64   userid; 	                        ///< 用户ID      userid.
		std::string name;                           ///< 用户名称 	 name.
		std::string orgname;                        ///< orgname 组织名称.
        std::string userHead;                       ///< 头像.
	};

    /**
    * \struct st_entpriseInfo
    * \brief 企业信息(搜索中使用)
    */
    struct  st_entInfo
    {
        st_entInfo() :
        enterpriseID(0){}

        int64 enterpriseID;         ///< 企业ID enterpriseID.
        std::string fullName;       ///< 企业全称 fullName.
        std::string shortName;      ///< 企业简称 shortName.
        std::string keyWords;       ///< 企业关键字 keyWords.
        std::string phone;          ///< 企业电话 phone.
    };

	/**
	 * \struct st_orGroupinfo
	 * \brief brief 组织信息（搜索中使用）
	 */
	struct st_orGroupinfo
	{
		st_orGroupinfo() :
		groupid(0),
		orderid(0){}

		int64    groupid;
		int32    orderid;
		std::string groupname;
		std::string orgname;
	};

	/**
	 * \struct st_noDisturbModeInfo
	 * \brief
	 */
	struct st_noDisturbModeInfo
	{
		int32 startTimer;
		int32 endTimer;
		int8  value;
	};

	/**
	 * \struct st_userToTargetSwitchInfo
	 * \brief
	 */
	struct st_userToTargetSwitchInfo
	{
		int64 targetUserId;
		int8  typeValue;
		int8  modeType;
	};

	/**
	 * \struct st_personalDataSwitchInfo
	 * \brief
	 */
	struct st_personalDataSwitchInfo {
		int8 switchType;
		int8 value;
	};

	/**
	 * \struct st_orgOpenConfig
	 * \brief
	 */
	struct st_orgOpenConfig
	{
		int64 teamID;
		int64 openOrgID;
		std::vector<int64>  vtTargetOrgIDs;
	};

	/**
	 * \struct st_entAppinfo
	 * \brief 应用信息
	 */
	struct  st_entAppinfo
	{
		std::string appSign;///< 应用标识
		std::string appName;///< 应用名称
		std::string appIcon;///< 应用图标
		std::string appIconUrl; ///服务器应用图标全路径
		int64 appID;        ///< 应用ID
		std::string appHomeUrl;///< 应用主页
		int64 entID;           ///< entID
		std::string entName;   ///< 应用名称
		std::map<std::string, std::string>  configItems; ///< 客户端配置
		std::string appSecret; ///< 应用密钥
		std::string appToken;  ///< 应用TOKEN
		std::string appMenus;  ///< 应用菜单
		int8	openIDEncoded; ///< openID是否编码，0:不编码，1:编码
		std::string subAccount;///< 应用子账号
		///子类型  1 H5应用，2 移动应用
		int8 appSubType;
		///推荐 1 是 ，2 否
		int8 recommend;
		/// 默认安装 1 是 ，2 否
		int8 defaultInstall;
		///扩展字段 json 格式，mainUrl H5首页，adminUrl 后台管理地址，myUrl 个人中心地址
		std::string extend;
		///设备类型 1(iPhone_E), 2(iPad_E), 3(iPhone), 4(iPad), 5(androidPhone), 6(androidPad)
		std::string deviceTypes;
		///消息历史地址
		std::string messageHistoryUrl;
		///是否启用自定义菜单   1 启用  0停用
		std::string menuOpen;
		///是否启用自定义菜单响应事件  1启用 0停用
		std::string replyMenuOpen;
		///智能开关 2
		int8 appType;
		///公众号状态
		int8 status;
		///1:订阅号2：企业号
		int8 subOrAppType;
		///应用功能介绍
		std::string appFunctintroduce;
		///公众号介绍
		std::string appDescription;
	};

	/**
	 * \struct  st_tinyGroupBean
	 * \brief   群数据结构
	 */
	struct st_tinyGroupBean
	{
		st_tinyGroupBean() :memberNums(0), level(0)
		{
			INIT_ISSET(memberNums)
			INIT_ISSET(level)
			INIT_ISSET(groupID)
			INIT_ISSET(createrID)
			INIT_ISSET(createTime)
			INIT_ISSET(relatedGroupID)
			INIT_ISSET(relatedEnt)
			INIT_ISSET(name)
			INIT_ISSET(pinyin)
			INIT_ISSET(icon)
			INIT_ISSET(srvName)
		}

		void init()
		{
			INIT_ISSET(memberNums)
			INIT_ISSET(level)
			INIT_ISSET(groupID)
			INIT_ISSET(createrID)
			INIT_ISSET(createTime)
			INIT_ISSET(relatedGroupID)
			INIT_ISSET(relatedEnt)
			INIT_ISSET(name)
			INIT_ISSET(pinyin)
			INIT_ISSET(icon)
			INIT_ISSET(srvName)
		}
		
		IS_SET(int32, memberNums)		///< 群成员数  memberNums.	
		/**
		* \brief 群等级 从低到高,1到4位代表群等级，5到6位不为0代表为互联群，7到8位位代表是否为v标群, 9到10位代表是否置顶(0表示未置顶，1表示置顶)，
		*  11位到12位代表群通知消息内容模式: 1、通知详情  2、通知源，隐藏内容  3、完全隐藏
		*  15位到16位代表是否为隐藏群：0，非隐藏 1，隐藏
		*  17位到18位代表是否允许主动退出, 0:可以主动退出,1:不能主动退出
		*/
		IS_SET(int32, level)					
		IS_SET(int64, groupID)			///< 群ID   groupID.		
		IS_SET(int64, createrID)		///< 创建者ID  createrID.		
		IS_SET(int64, createTime)		///< 创建时间	  createTime.	
		IS_SET(int64, relatedGroupID)	///< 关联群ID，存在关联群ID则属于群内房间 relatedGroupID.	
		IS_SET(int64, relatedEnt)		///< 关联企业ID,	relatedEnt.	
		IS_SET(std::string, name)		///< 群名称 name.
		IS_SET(std::string, pinyin)		///< 拼音名称 pinyin.
		IS_SET(std::string, icon)		///< 群头像	icon.
		IS_SET(std::string, iconUrl)    ///< 群头像服务器全路径
		IS_SET(std::string, srvName)	///< 服务器名称  
	};

	/**
	 * \struct st_groupBean
	 * \brief  群数据结构
	 */
	struct st_groupBean
	{
		st_groupBean() :isActive(1), isSearch(1)
		{
			INIT_ISSET(isActive)
			INIT_ISSET(isSearch)
			INIT_ISSET(serverInfo)
			INIT_ISSET(brief)
			INIT_ISSET(groupType)
			INIT_ISSET(bulletin)
			INIT_ISSET(chatImage)
			INIT_ISSET(extend)
			INIT_ISSET(groupMessageContentMode)
			INIT_ISSET(info)
		}
		void init()
		{
			INIT_ISSET(isActive)
			INIT_ISSET(isSearch)
			INIT_ISSET(serverInfo)
			INIT_ISSET(brief)
			INIT_ISSET(groupType)
			INIT_ISSET(bulletin)
			INIT_ISSET(chatImage)
			INIT_ISSET(extend)
			INIT_ISSET(groupMessageContentMode)
			INIT_ISSET(privateMsgPwd)
			INIT_ISSET(info)
			info.init();
		}
		/**
		 * \brief 激活群
		 * 1. 激活， 2：未激活
		 */
		IS_SET(int32, isActive)
		IS_SET(int32, isSearch)			///< 是否可以被检索 1可以，2不可以 isSearch.
		IS_SET(std::string, serverInfo) ///< 本地服务器地址 serverInfo.
		IS_SET(std::string, brief)		///< 群简介 brief.
		IS_SET(std::string, groupType)	///< 群分类 groupType.
		IS_SET(std::string, bulletin)	///< 群公告 bulletin.
		IS_SET(std::string, chatImage)	///< 群聊背景 chatImage.
		/** 
		 *	\brief 群扩展字段,以json形式,目前： 
		 *  警种ID： policeTypeID
		 *  地域---省份ID：provinceID
		 *  地域---市ID： cityID
		 *  私信模式：privateMsg：1为开启私信   0表示不允许私信 extend.
		 */
		IS_SET(std::string, extend)
		/** 
		 *	\brief 群消息通知详情: 1、开启(显示详情)  2、关闭(显示通知源，隐藏内容)  默认开启 groupMessageContentMode.
		 */
		IS_SET(int8, groupMessageContentMode)

		IS_SET(std::string, privateMsgPwd);	///< 私信密码 privateMsgPwd.

		IS_SET(st_tinyGroupBean, info)	///< 群信息 info.		
	};

	/**
	 * \struct st_groupSet
	 * \brief  群验证方式信息
	 */
	struct st_groupSet
	{
		int8 verifyType;		///< 验证类型:1:不允许任何人添加,2：需要验证信息,3：允许任何人添加 verifyType.
		int8 isAllow;			///< 是否允许群成员邀请好友加入群:1,允许 2,不允许 isAllow.
	};

	/**
	 * \struct st_shieldGroupMessage
	 * \brief  群信息提示方式
	 */
	struct st_shieldGroupMessage {
		st_shieldGroupMessage()
		{
			INIT_ISSET(pcMsgSet)
			INIT_ISSET(phoneMsgSet)
			INIT_ISSET(htmlMsgSet)
			INIT_ISSET(groupMessageContentMode)
			INIT_ISSET(msgRecvTime)
		}
		void init()
		{
			INIT_ISSET(pcMsgSet)
			INIT_ISSET(phoneMsgSet)
			INIT_ISSET(htmlMsgSet)
			INIT_ISSET(groupMessageContentMode)
			INIT_ISSET(msgRecvTime)
		}

		IS_SET(int8, pcMsgSet)				 ///< PC群消息设置 1：提示接收 2：不提示，显示数目 3：屏蔽消息 4：时间段接收 5：插电接收 pcMsgSet.
		IS_SET(int8, phoneMsgSet)			 ///< phone/pad群消息设置 1：提示接收 2：不提示，显示数目 3：屏蔽消息 4：时间段接收 5：插电接收 phoneMsgSet.
		IS_SET(int8, htmlMsgSet)			 ///< HTML群消息设置 1：提示接收 2：不提示，显示数目 3：屏蔽消息 4：时间段接收 5：插电接收 htmlMsgSet.
		int64 groupid;
		IS_SET(int8, groupMessageContentMode) ///< 群通知消息内容模式: 1、通知详情  2、通知源，隐藏内容  3、完全隐藏 , 默认开启模式1
		IS_SET(std::string, msgRecvTime)	 ///< 接收消息时间段 msgRecvTime.
	};

	/**
	 * \struct st_tinyGroupMemberBean
	 * \brief  群成员结构
	 */
	struct st_tinyGroupMemberBean {
		st_tinyGroupMemberBean()
		{
			INIT_ISSET(sex)
			INIT_ISSET(userType)
			INIT_ISSET(userid)
			INIT_ISSET(memberName)
			INIT_ISSET(avatar)
			INIT_ISSET(avatarUrl)
			INIT_ISSET(vSign)
			INIT_ISSET(pinyin)
		}
		void init()
		{
			INIT_ISSET(sex)
			INIT_ISSET(userType)
			INIT_ISSET(userid)
			INIT_ISSET(memberName)
			INIT_ISSET(avatar)
			INIT_ISSET(avatarUrl)
			INIT_ISSET(vSign)
			INIT_ISSET(pinyin)
		}
		IS_SET(int8, sex)			///< 性别 1男 2女 0保密 sex.	 		
		IS_SET(int8, userType)		///< 用户类型复用，与上0x0F  1：普通用户 2：管理员 3：超级管理员; 与上0xF0＝0xF0: 该群成员为应用 userType.			
		IS_SET(int64, userid)		///< 用户ＩＤ，当userType为１００的时候是应用 userid.	
		IS_SET(std::string, memberName)	///< 群名称 memberName.	
		IS_SET(std::string, avatar)		///< 用户头像 avatar.
		IS_SET(std::string, avatarUrl)  ///< 头像服务器全路径
		IS_SET(int8, vSign)				///< 是否为v标群，0不是 1是 只有当群成员为用户自己时使用				
		/**
		 * \brief 拼音，memberName
		 * 格式 简拼$$全拼
		 * 比如 “连豆豆” 这个中文
		 * 简拼：ldd
		 * 全拼: liandoudou
		 * 该字段的值为 ldd$$liandoudou, 使用的时候注意分割
		 */
		IS_SET(std::string, pinyin)
	};

	/**
	 * \struct st_groupMemberBean
	 * \brief
	 */
	struct st_groupMemberBean{
		int64 birthday;
		std::string name;
		std::string nickID;
		std::string sign;
		std::vector<std::string> phones;
		std::vector<std::string> emails;
		st_tinyGroupMemberBean info;
		/**
		 * \brief 如果获取的群成员是自己的话，这个字段的值有效。
		 * 标识这个群
		 */
		st_shieldGroupMessage  shield;
	};

	/**
	 * \struct st_buddyReqBean
	 * \brief 添加好友结构
	 */
	struct  st_buddyReqBean
	{
		int64 reqVerifyboxID;
		st_smallUserBean reqUserBean;
		std::vector<std::string>  verifyInfo;
		int8 requestType;
		int64 requestTime;
		int8 isResponse;
	};

	/**
	 * \struct st_buddyRespBean
	 * \brief 添加好友响应结构体
	 */
	struct st_buddyRespBean
	{
		int64 respVerifyboxID;
		st_smallUserBean respUserBean;
		std::string refuseReason;
		int8   isAgree;
		int64 responseTime;
	};

	//=================================<其它协议>============================================//
	///发送文件结构.
	struct   st_sendFile
	{
		st_sendFile() :isResume(false){};
		int32           fileSign;	///< 文件唯一标识
		std::string     localPath;  ///< 本地地址
		bool            isResume;   ///< 是否断点上传
	};

	///文件结构
	struct st_Fileinfo
	{
		int8    isEncrypt;		///< 是否加密
		int64   fileID;			///< 文件ID
		int64   userID;			///< 发送者
		int64   targetID;		///< 目标对象, 可能是用户,也可能是群
		int64   fileSize;		///< 文件大小
		int64   uploadTime;		///< 上传时间
		std::string filePath;   ///< 本地路径
		std::string fileName;	///< 文件名
		std::string fileUrl;	///< 文件URL
		std::string secretKey;	///< 加密KEY
		std::string fromName;   ///< 发送者名字，如果在本地没找到返回空
	};

	struct  YMD
	{
		int16  year;
		int8   month;
		int8   day;
	};

	///应用信息
	struct st_tinyAppInfo {
		st_tinyAppInfo() : appID(0)
		{
			configKey.clear();
			configVal.clear();
		}
		void init()
		{
			configKey.clear();
			configVal.clear();
		}
		std::string appSign; ///< 应用标识
		std::string appName; ///< 应用名称
		std::string appIcon; ///< 应用图标
		int64       appID;   ///< 应用ＩＤ
		std::string homeUrl; ///< 主页地址
		int64       entID;   ///< 企业ＩＤ
		std::string entName; ///< 企业名称
		///客户端配置ＫＥＹ
		std::vector<std::string> configKey;
		///客户端配置ＶＡＬＵＥ
		std::vector<std::string> configVal;
		///应用密钥
		std::string appSecret;
		///应用令牌
		std::string appToken;
		std::string appMenus;  ///应用菜单
		int8	openIDEncoded; ///openID是否编码，0:不编码，1:编码
		std::string subAccount;///机器人子账户列表 json格式数据

		///子类型  1 H5应用，2 移动应用
		int8 appSubType;
		///推荐 1 是 ，2 否
		int8 recommend;
		/// 默认安装 1 是 ，2 否
		int8 defaultInstall;
		///扩展字段 json 格式，mainUrl H5首页，adminUrl 后台管理地址，myUrl 个人中心地址
		std::string extend;
		///设备类型 1(iPhone_E), 2(iPad_E), 3(iPhone), 4(iPad), 5(androidPhone), 6(androidPad)
		std::string deviceTypes;
		///消息历史地址
		std::string messageHistoryUrl;
		///是否启用自定义菜单   1 启用  0停用
		std::string menuOpen;
		///是否启用自定义菜单响应事件  1启用 0停用
		std::string replyMenuOpen;
		///智能开关 2
		int8 appType;
		///公众号状态
		int8 status;
		///1:订阅号2：企业号
		int8 subOrAppType;
		///应用功能介绍
		std::string appFunctintroduce;
		///公众号介绍
		std::string appDescription;
	};

	///本地配置条目
	struct st_localSetting {
		std::string key; ///标识
		std::string val; ///值
	};

	//搜索返回群或个人的简要信息
	struct st_simpleSearchInfo
	{
		/**
		* \brief 0x01男，0x02女,0x08为群，0x10为app, 0 非男非女为保密
		*/
		int8 targetFlag;
		///对象名称
		std::string name;
		///头像
		std::string avatr;
		std::string avatrUrl; ///头像服务器全路径
	};

	///保存搜索的targetId对象的简要信息和搜索出的消息数量
	struct st_msgGlobalSearch
	{
		///会话对象
		int64 targetID;

		///群或个人的简要信息
		st_simpleSearchInfo info;

		///消息数目
		int amount;
	};

	///近距离加好友、群请求结构
	struct st_faceToFaceBean
	{
		st_faceToFaceBean() :uid(0), longitude(0), latitude(0)
		{
			INIT_ISSET(passwd)
		}
		void init()
		{
			INIT_ISSET(passwd)
		}

		int64  uid;					///< 用户Id
		double longitude;			///< 经度
		double latitude;			///< 纬度
		IS_SET(std::string, passwd) ///< 房间密码
		int8 operation;				///< 操作类型 1.建群 2.加好友
	};

	/// 第三步 做真正的操作
	struct st_userAccountOperate
	{
		st_userAccountOperate()
		{
			INIT_ISSET(operation)
			INIT_ISSET(registryID)
			INIT_ISSET(code)
		}

		IS_SET(int8, operation)    ///< 1: 绑定 2: 解绑
		IS_SET(int64, registryID)  ///< 需要验证的那些都需要填写
		IS_SET(std::string, code)  ///< 验证码
	};

	/// 通信录信息
	struct st_contactBean
	{
		std::string contactNum;   ///< 联系人号码
		std::string name;         ///< 联系人姓名
		int8 source;              ///< 数据来源 1：手机通讯录； 2：qq好友；
	};

	struct st_autoAttentionBean
	{
		st_contactBean contact;
		st_tinyUserBean userBean;
	};
	/// 推荐信息
	struct st_recommendContact
	{
		st_contactBean contact;     ///< 通信录信息
		st_smallUserBean sub;       ///< 联系人信息
		st_smallUserBean recommend; ///< 推荐者信息
		int8 isBuddy;               ///< 是否是好友标识，1表示是好友关系，2表示不是好友关系, 3表示等待验证
	};

	///分享设置选项
	struct st_shareOptionBean
	{
		int8 shareType;          ///< 1:不允许他看我的分享，2：不看他的分享 
		int64 targetUserID;      ///< 目标用户ID
	};

	//用户自定义信息
	struct st_personDefinition
	{
		int8 defType;         ///< 1表示推送通知提示音
		std::string value;    ///< defType=1的时候表示提示音的名字
	};
	
	///数据字典
	struct st_enterpriseDictionary
	{
		int32 id;                ///< 组织ID
		int32 parentId;          ///< 父组织ID
		std::string dicValue;    ///< 字段的名字
		std::string dickey;      ///< 字段的键值
		std::string extend;      ///< 拓展字段，内部json结构
	};
	
	///应用市场信息
	struct st_smallMarketAppInfo
	{
		int64 appID;                   ///< 应用ID
		int64 entID;                   ///< 企业ID
		int64 ownerId;                 ///< 所有者id
		int8 status;                   ///< 状态 1启用，2停用，3删除
		int8 appType;                  ///< 类型：1(H5应用), 2(IOS应用), 3(Android应用), 4(内部应用), 5(快捷应用)
		int8 defaultInstall;           ///< 默认安装 1 是 ，2 否
		int8 showInMy;                 //< 个人安全中心是否展示 1 是 ，2 否
		std::string appSign;           ///< 应用标识
		std::string appName;           ///< 应用名称
		std::string appIcon;           ///< 应用图标		
		std::string appFunctintroduce; ///< 应用功能介绍
		std::string version;           ///< 版本号	
		std::string mainUrl;           ///< H5首页
		std::string adminUrl;          ///< 后台管理地址
		std::string myUrl;             ///< 个人中心地址
		std::string packageName;       ///< 包名
		std::string downloadUrl;       ///< 下载地址
		std::string urlScheme;         ///< urlScheme
		std::string quickUrl;          ///< 快捷应用地址
		std::string extend;            ///< 扩展字段
		std::string activityName;      ///< activity
	};

	///应用市场分页查询结果
	struct st_smallMarketAppPage
	{
		st_smallMarketAppPage() : pageNum(0), pageSize(0), count(0), totalPage(0)
		{
			appInfoList.clear();
		}
		int32 pageNum;                              ///< 页码
		int32 pageSize;                             ///< 页长
		int32 count;                                ///< 总数
		int32 totalPage;                            ///< 总页数
		std::vector<st_smallMarketAppInfo> appInfoList; ///< 应用列表
	};
	
	struct st_emoticon
	{
		st_emoticon()
		{

		}
		std::string mdCode;         ///< 整个文件的MD5hashcode码
		std::string depict;         ///< 表情描述
		int64 userID;               ///< 表情创建人
		int64 createTime;           ///< 创建时间
		std::string mainUrl;        ///< 表情Url
		int8 property;              ///< 表情属性 1：系统自带2：用户上传
		int8 emoStatus;             ///< 表情状态 1：草稿 2：发布 3：删除
		std::string packageCode;    ///< 关联表情包Code
		int8 type;                  ///< 表情类型 1：静态表情2：动态表情
		///表情类目 1人物 2情侣 3动物 4食物 5节日 6网络热点 7征稿活动 8其他
		int8 category;
		///表情标签  多个标签用分隔符分开，web端自己定义自己解析，服务只做存储
		std::string label;
		std::string thumbnailUrl;     ///< 缩略图url
		std::string meaning;          ///< 表情含义
		std::string orderNum;         ///< 排序字段
		std::string extendFiled;      ///< 拓展字段1
		std::string extendColumn;     ///< 拓展字段2
	};

	struct st_singleEmoticon
	{
		st_singleEmoticon()
		{

		}
		std::string mdCode;               ///< 单个表情MdCode
		std::string thumbnailUrl;
		std::string mainUrl;              ///< 大图地址
		std::string meaning;              ///< 含义	
	};
	struct st_emoticonPackage
	{
		std::string mdCode;          ///< 整个文件的MD5hashcode码
		std::string depict;          ///< 表情描述
		int64 userID;                ///< 表情创建人
		int64 createTime;            ///< 创建时间
		std::string mainUrl;         ///< 表情Url
		int8 property;               ///< 表情属性 1：系统自带2：用户上传
		int8 packStatus;             ///< 表情状态 1：草稿 2：发布 3：删除
		std::string name;            ///< 表情包名称
		std::string bannerUrl;       ///< 详细页横幅URL
		int8 type;                   ///< 表情类型 1：静态表情2：动态表情
		///表情类目 1人物 2情侣 3动物 4食物 5节日 6网络热点 7征稿活动 8其他
		int8 category;
		///表情标签  多个标签用分隔符分开，web端自己定义自己解析，服务只做存储
		std::string label;
		std::string iconUrl;             ///< 图标url
		std::string coverUrl;            ///< 封面URL
		std::string instruction;         ///< 表情包说明
		std::string orderNum;            ///< 排序字段
		std::string extendFiled;         ///< 拓展字段1
		std::string extendColumn;        ///< 拓展字段2
		std::vector<st_singleEmoticon> emoticonList; //单个表情列表
		//std::string emoticonListJson;    ///< 关联表情 格式json-list<map>
	};

	struct st_emoticonResult
	{
		int resultCode;                  ///< 结果编码 801:md5Code不存在，802：md5Code文件已存在
		std::string resultMessage;       ///< 结果信息
		std::string mdCode;              ///< 表情code
		int orderNum;                    ///< 排序
	};
	
	///局域网用户
	struct st_p2pUser {
		int64 userid ; ///< 用户ID
		int8  flag   ; ///< PC 0x01，移动 0x02
	};

	///更新的解密消息
	/*struct st_updateMsg
	{
		int64 targetId;
		///msgId,messsage和msgProperty一一对应
		std::vector<int64> msgId;
		std::vector<std::string> message;		
		std::vector<std::string> msgProperty;
	};*/

	struct st_sysMsg
	{
		int64 msgID;									///< 消息ID msgID.
		int64 userID;									///< 用户ID userID.
		int64 groupID;									///< 群ID groupID.
		/**
		* \brief 1:加好友请求
		* 2:加好友响应
		* 3:加群请求
		* 4:加群响应
		*/
		int32 msgType;
		int64 time;										///< 时间秒 time.
		int32 isResponse;								///< 是否需要回复:	1:需要应答，	2：不需要应答 isResponse.
		int32 isRead;									///< ０未读，１已读 isRead.
		/**
		* \brief #msgType=1  1: 通过，２：拒绝，３：永久拒绝 4:忽略 0:没有操作
		*  #msgType=2  1: 通过，２：拒绝，３：永久拒绝
		*  #msgType=3  1: 忽略，２：同意，３：拒绝，４：永久拒绝，５：解散，６：移出，７主动退出  0:没有操作
		*  #msgType=4  1: 忽略，２：同意，３：拒绝，４：永久拒绝，５：解散，６：移出，７主动退出
		*/
		int32 opType;
		/**
		* \brief #msgType=1	 * 1:　添加好友，２：关注
		* #msgType=3	 * 1: 邀请, 2: 申请加入
		*/
		int32 subType;									///< 请求子类型 subType.
		std::string name;								///< 用户名 name.
		std::string groupName;							///< 群名称 groupName.
		std::string portraitUrl;						///< 头像   portraitUrl
		std::string addInfo;							///< 附加消息 addInfo
	};
    /**
     * \struct st_sendFilesExt 消息号: logic_cmd_sendFilesExt
     * \brief  发送文件。单个文件接收进度消息使用logic_cmd_progress消息返回
     */
    typedef bool (parseJsonCallback_t)(int32 type, std::vector<std::string> jsons,
            std::string &body, std::string &prop);

    struct st_sendFilesExt
    {
        int64 targetID;					///< 目标ID,有可能是群或者人 targetID.
        /**
         * \brief 加密KEY，为空不进行加密
         */
        std::string encryptKey;			///< 加密KEY encryptKey.
        std::string message;            ///< 附加信息由上层定义和使用
        std::vector<st_sendFile> _vt;	///< 发送文件数组 _vt.
        int8      isP2p ;               ///< 是否点对点 0 否， 1 是

        /*
         * \brief 解析上传附件返回的json
         * type:  附件类型
         * jsons: 上传附件返回的json vector
         * body:  解析完成后对应的消息body
         * prop:  解析完成后对应的消息属性
         * return: true for success, false for fail
         */
        parseJsonCallback_t *parseJsonCB;

        st_sendFilesExt() {
            init();
        }

        void  init() {
            _vt.clear();
            isP2p = 0 ;
            parseJsonCB = NULL;
        }
    };

	///p2p对象
	struct p2pUser{
		p2pUser() :userid(0), flag(0){}
		int64 userid; ///<用户ID
		int8  flag; ///<PC 0x01，移动 0x02
	};
	///局域网设备用户列表
	struct p2pUsers {
		int32 code;
		std::vector<p2pUser> _vt; ///< 用户列表
	};

	struct tfFileInfo {
		tfFileInfo() {
			fileType = 1;
			size = 0;
			finished = 0;
		}

		char   fileType; /* 1:文件, 2:文件夹 */
		int64  size;    /* 当前文件大小, http上传时是总分片数 */
		int64  finished; /* 完成量, http上传时是已上传的分片数 */
		std::string   remotePath; /* 远端路径 */
		std::string   localPath;  /* 本地路径 */
	};
		///保存的上传通讯录字段
	struct st_localPostContact
	{
		st_localPostContact() :userId(0), flag(0)
		{
		}
		std::string phone;			//手机号
		int64 userId;				//用户ID
		int8 flag;					//标示
		std::string phoneName ;     ///手机通讯录里面的名称
		std::string userName ;      ///用户名称
		std::string userHead ;      ///用户头像
	};
}

#endif   //CMD_COM_DEF_H_
