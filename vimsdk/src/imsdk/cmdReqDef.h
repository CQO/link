#ifndef CMD_REQ_DEF_H
#define CMD_REQ_DEF_H

#include "cmdComDef.h"

//=================================<所有请求消息结构体>============================================//

typedef bool(*MatchFunc)(const char *msg, const char *key);

namespace imsdk
{
	/**
	  * \struct  req_setHeadImg
	  * \brief   设置头像 , logic_cmd_setHeadImg 消息携带
	  */
	struct req_setHeadImg : public _sdkcmd_base
	{
		std::string thumb_path;    ///< 缩略图本地地址 thumb_path.
		std::string orgi_path;	   ///< 大图本地地址 orgi_path.
		CMD_SIZE();
	};

	/**
	  * \struct req_getBuddyVerifyType 消息号: logic_cmd_getBuddyVerifyType
	  * \ brief 获取好友验证类型
	  */
	struct req_getBuddyVerifyType : public _sdkcmd_base
	{
		int64 userid; ///< 要加为好友的人ID userid.
		CMD_SIZE();
	};

	/**
	  * \struct req_getBuddyInfo
	  * \brief 获取好友详细信息  消息号：logic_cmd_getBuddyInfo
	  */
	struct req_getBuddyInfo : public _sdkcmd_base
	{
		int8 flag;			///< 0为读取本地，1为获取网络最新 flag.
		int64 userID;		///< 好友的ＩＤ号 userID.
		CMD_SIZE();
	};

	/**
	  * \struct  req_downloadFile
	  * \brief   文件接收请求 消息号: logic_cmd_downloadFile
	  * 		 接收进度消息使用logic_cmd_progress消息返回
	  */
	struct req_downloadFile : public _sdkcmd_base
	{
		std::string localPath;	 ///< 文件本地保存路径 localPath.
		std::string remotePath;	 
		///< 0:remotePath为全路径，1.远程路径,从消息JSON中解析出来的路径  或上0xF0，代表只判断文件有效性，不下载 remotePath
		int8 flag;              					 
		CMD_SIZE();
	};

	/**
	  * \struct  req_recvFiles
	  * \brief   文件接收请求 消息号: logic_cmd_recvFiles
	  * 		 接收进度消息使用logic_cmd_progress消息返回
	  */
	struct req_recvFiles : public _sdkcmd_base
	{
		req_recvFiles () {
			bisP2P = false;
            msgID = 0;
            targetID = 0;
            fromID = 0;
			bisResume = true;
		}
		/**
		 * \brief 1. 当bisP2P=false, 传该文件所属的消息ID
		 *        2. 当bisP2P=true, 传下层请求中带的TASKID
		 */
		int64       msgID;        ///如果是p2p传输, msgid将被用作taskID
		int64       targetID;	 ///< 目标ID	有可能是群或者人 targetID.
		std::string encryptKey;  ///< 暂时不用.
		std::string localPath;	 ///< 文件本地保存路径 localPath.
		std::string remotePath;	 ///< 远程路径,从消息JSON中解析出来的路径 remotePath.
		int64       fromID;      ///< 发送者ID fromID.
		std::string fileName;    ///< 真实的文件名 fileName.
		bool        bisP2P ;	 ///< 是否接收局域网直传文件.
		bool        bisResume;   ///<是否断点下载
		CMD_SIZE();
		
		void  init()
		{
			bisP2P = false ;
		}
	};

	/**
	  * \struct req_sendFiles 消息号: logic_cmd_sendFiles
	  * \brief  发送文件。单个文件接收进度消息使用logic_cmd_progress消息返回
	  */
	struct req_sendFiles : public _sdkcmd_base
	{
		int64 targetID;					///< 目标ID,有可能是群或者人 targetID.
        uint16 type;                    ///< 消息类型, 同req_sendMessage的messageType.
                                        ///< 3:音频 5:图片 6:文件.
		/**
		 * \brief 加密KEY，为空不进行加密
		 */
		std::string encryptKey;			///< 加密KEY encryptKey.
        std::string message;            ///< 附加信息由上层定义和使用.
		std::vector<st_sendFile> _vt;	///< 发送文件数组 _vt.
		int8      isP2p ;               ///< 是否点对点 0 否， 1 是.
		int8      isJS;                 ///< 是否JS上传图片,0 否， 1 是

		req_sendFiles() {
			init();
		}

		void  init()
		{
			_vt.clear();
			isP2p = 0 ;
			isJS = 0;
		}
		CMD_SIZE();
	};

	/**
	  * \struct req_getHistoryMsg 消息号: logic_cmd_getHistoryMsg
	  * \brief 获取历史消息,
	  *
	  */
	struct req_getHistoryMsg : public _sdkcmd_base
	{
		req_getHistoryMsg() :targetID(0), msgBeginID(0), msgOffset(0)
		{
			msgType.clear();
			fromUserID.clear();
		}
		void init()
		{
			targetID = 0;
			msgBeginID = 0;
			msgOffset = 0;
			msgType.clear();
			fromUserID.clear();
		}
		int64    targetID;				///< 目标ID,群号或者用户号 targetID.
        /**
         * \brief 起始消息ID
         *        返回的结果会包含这条消息
         *        offsetFlag = 0 msgBeginID = 0时，代表从最大的消息Id进行查找
         */
		int64    msgBeginID;
		int32    msgOffset;			     ///< 消息偏移量 msgOffset.
		int8     offsetFlag;		     ///< 偏移标志；0.消息Id由大到小偏移 1.消息Id由小到大偏移 offsetFlag.
		std::vector<int16>    msgType;   ///< 消息类型,表示查询某一些类型的消息，默认为空,表示查询所有类型的消息,最多查询5种类型的消息。
		std::vector<int64>    fromUserID; ///< 发送者ID,表示只查询某一些人发的消息，默认为空，表示查询所有人的消息，最多查询10个人的消息。
		CMD_SIZE();
	};

	/**
	* \struct req_getAppointMsg 消息号: logic_cmd_getAppointMsg
	* \brief 获取指定消息,
	*
	*/
	struct req_getAppointMsg : public _sdkcmd_base
	{
		int64 targetID;				///< 目标ID,   群号或者用户号 targetID.
		std::vector<int64> _vt;     ///< msg ID
		void  init()
		{
			_vt.clear();
		}
		CMD_SIZE();
	};

	/**
	  * \struct req_setMsgReaded  消息号: logic_cmd_setMsgReaded
	  * \brief 设置消息已读
	  */
	struct  req_setMsgReaded : public _sdkcmd_base
	{
		int64    targetID;  ///< 目标ID，群或者人ID， targetID.
		/**
		 * \brief 需要设置的消息ID
		 * 比如设置为100, 比100小的所有消息都为已读
		 */
		int64    msgID;
		CMD_SIZE();
	};

	/**
	  * \struct req_sendMessage 	消息号： logic_cmd_sendMessage
	  * \brief  发送消息结构体
	  */
	struct req_sendMessage : public _sdkcmd_base
	{
		req_sendMessage() :userID(0), targetID(0),
				isBurn(0), noStore(0), activeType(0),
                relatedMsgID(0)
		{
		}
		int64 userID;   		///< TODO,要删除,不用设置自己的用户ID.
		int64 targetID; 		///< 接受者，可以为群或者人 targetID.
		std::string message; 	///< 消息体 message.
		int8 isBurn;         	///< 是否阅后即焚 isBurn.
		int8 noStore;			///< 本地不存储消息  0为存储，1为不存储.			
		/**
		 * \brief 消息类型
		 * 1:html 2:文本，3:音频 4: 位置 5:图片6:文件 7:名片 8:弱提示
		 */
		int16 messageType;
		std::string maccode; 	///< mac地址 maccode.
		std::string format;  	///< 文本格式，可以不用 format.
		std::vector<int64>  limitRange; ///< 限制消息用户范围 limitRange.
		std::string msgProperties; 	///< 消息属性, 可定义为json串  msgProperties.
		int8  activeType;    	///< 消息事件属性 activeType.
		std::vector<int64>  relatedUsers; ///< @功能使用，填入@人员的用户ID relatedUsers.
		int64 relatedMsgID;  	///< 关联消息ID relatedMsgID.
		std::string sourceID;   ///< 陌生人单聊时的来源ID，1_groupID表示传的群ID，2_orgID表示传的组织ID sourceID.
		std::string loginfoPath; ///发送消息，登录信息路径
		void init() {
			userID = 0;
			targetID = 0;
			isBurn = 0;
			noStore = 0;
			activeType = 0;
			relatedMsgID = 0;
			limitRange.clear();
			relatedUsers.clear();
			msgProperties = "";
			loginfoPath = "";
		}
		CMD_SIZE();
	};

	/**
	* \struct req_fileMessage 	消息号： logic_cmd_fileMessage
	* \brief  发送消息(包含附件消息)结构体
	*/
	struct req_fileMessage : public req_sendMessage
	{
		req_fileMessage() :isP2p(0){}
		int8        isP2p;                ///< 是否点对点 0 否， 1 是.
		std::string encryptKey;			  ///< 加密KEY encryptKey.
		st_sendFile file;			      ///< 发送的文件
		CMD_SIZE();
	};

	/**
	* \struct req_imgMessage 	消息号： logic_cmd_imgMessage
	* \brief  发送消息(包含附件消息)结构体
	*/
	struct req_imgMessage : public req_sendMessage
	{
		void init()
		{
			thumbPaths.clear();
			orgPaths.clear();
		}
		std::string encryptKey;					///< 加密KEY encryptKey.
		///< 缩略图和原图一一对应，一对图推一次进度
		std::vector<std::string> thumbPaths;	///< 缩略图路径
		std::vector<std::string> orgPaths;		///< 原图路径
		CMD_SIZE();
	};

    /**
     * \struct req_sendMessageExt 	消息号： logic_cmd_sendMessageExt
     * \brief  发送消息(包含附件消息)结构体
     */
    struct req_sendMessageExt : public req_sendMessage
    {
        st_sendFilesExt attachment; ///附件消息
        CMD_SIZE();
    };

	/**
	  * \struct   req_simpleSearch
	  * \brief    搜索
	  */
	struct req_simpleSearch : public _sdkcmd_base
	{

		/**
		 * \brief 搜索类型
		 * type 与上 0x0F:
		 * 1: 搜索联系人
		 * 2: 搜索群
		 * 3: 搜索团队
		 * 0: 全部搜索
		 * type 与上 0xF0:
		 * 0x10: 标识本地搜索
		 */
		int8 type;
		/**
		 * \brief 搜索范围
		 * 1: 本公司搜索
		 * 2: 互联搜索
		 */
		int8 area;                 ///< 地区 1 本公司搜索 2 互联搜索 area.
		std::string keyword;       ///< 关键字 keyword.
		CMD_SIZE();
	};

	/**
	 * \struct req_updateOnlineState
	 * \brief  更新在线状态
	 */
	struct req_updateOnlineState : public _sdkcmd_base
	{
		int8 flag; ///< 1:在线,2:隐身,3:忙碌,4:离开,5:请勿打扰 flag.
		CMD_SIZE();
	};

	/**
	  * \struct req_getBuddyOnline 消息号:  logic_cmd_getBuddyOnline
	  * \brief 获取好友在线状态
	  */
	struct req_getBuddyOnline : public _sdkcmd_base
	{
		/**
		 * \biref 用户列表 , 如果列表为空，
		 * 表示获取全部好友状态，否则获取指定成员状态。
		 */
		std::vector<int64> member;
		void init()
		{
			member.clear();
		}
		CMD_SIZE();
	};

	/**
	 * \struct req_Changepw  ， 消息号: logic_cmd_changePw
	 * \brief 修改密码
	 */
	struct req_Changepw : public _sdkcmd_base
	{
		std::string  oldPw;   ///< 旧密码 oldPw.
		std::string  newpw;   ///< 新密码 newpw.
		CMD_SIZE();
	};

	/**
	 * \struct req_opUser , 消息号: logic_cmd_opuser
	 * \brief 操作好友
	 */
	struct req_opUser : public _sdkcmd_base
	{
		req_opUser()
		{
			INIT_ISSET(isDeleteOtherSide)
			INIT_ISSET(tinyUserType)
			INIT_ISSET(starBuddy)
			INIT_ISSET(groupID)
			INIT_ISSET(remark)
			INIT_ISSET(backgroundURL)
		}

		int8 operType;	///< 2:更新，3：删除 operType.
		/**
		* \brief 好友ＩＤ
		*/
		int64 userID;
		/**
		* \brief 对方列表中 1： 删除， 2： 不删除
		*/
		IS_SET(int8, isDeleteOtherSide)
		/**
		* \brief 好友类型 1: 好友， 2：关注
		*/
		IS_SET(int8, tinyUserType)
		/**
		* \brief 星标好友 1：星标好友，　２：非星标好友
		*/
		IS_SET(int8, starBuddy)

		/**
		* \brief 操作到组
		* 1.我的好友 2陌生人 3黑名单，其他用户自定义分组
		*/
		IS_SET(int64, groupID)
		IS_SET(std::string, remark)	///< 好友备注 remark.
		IS_SET(std::string, backgroundURL)	///< 聊天背景 backgroundURL.		

		void init()
		{
			operType = 0;
			isDeleteOtherSide = 0;
			tinyUserType = 0;
			starBuddy = 0;
			userID = 0;
			groupID = 0;
			INIT_ISSET(isDeleteOtherSide)
			INIT_ISSET(tinyUserType)
			INIT_ISSET(starBuddy)
			INIT_ISSET(groupID)
			INIT_ISSET(remark)
			INIT_ISSET(backgroundURL)
		}

		CMD_SIZE();
	};

	/**
	* \struct req_getHiddenAccountNew , 消息号: logic_cmd_getHiddenAccountNew
	* \brief 通过密码得到隐藏的好友或群Id
	*/
	struct req_getHiddenAccountNew : public _sdkcmd_base
	{
		std::string pwd;				///< 之前设置过的隐藏密码 pwd.
		CMD_SIZE();
	};

	/**
	 * \struct req_setHiddenAccountNew , 消息号: logic_cmd_setHiddenAccountNew
	 * \brief 设置隐藏
	 */
	struct req_setHiddenAccountNew : public _sdkcmd_base
	{
		std::string pwd;				///< 设置隐藏的密码 pwd.
		std::vector<int64> hiddenIDs;   ///< 需要隐藏的好友或群ID hiddenIDs
		CMD_SIZE();
	};

	/**
	 * \struct req_deleteHiddenAccountNew , 消息号: logic_cmd_deleteHiddenAccountNew
	 * \brief 删除隐藏
	 */
	struct req_deleteHiddenAccountNew : public _sdkcmd_base
	{
		std::string pwd;				///< 删除需要的密码 pwd.
		std::vector<int64> hiddenIDs;   ///< 需要删除的好友或群ID hiddenIDs
		CMD_SIZE();
	};

	/**
	 * \struct req_verifyHiddenInfo , 消息号: logic_cmd_verifyHiddenInfo
	 * \brief 找回隐藏密码，验证聊天信息
	 */
	struct req_verifyHiddenInfo : public _sdkcmd_base
	{
		std::vector<st_hiddenAccountInfo> hiddenAccountInfos;	///< 验证以前的聊天记录，三句大于5字节以上的聊天信息,可以是多个人的,也可以是一个人的,优先多个人
		CMD_SIZE();
	};

	/**
	* \struct req_resetHiddenPWD , 消息号: logic_cmd_resetHiddenPWD
	* \brief 重置隐藏密码
	*/
	struct req_resetHiddenPWD : public _sdkcmd_base
	{
		std::string oldpwd;				///< 旧密码  oldpwd
		std::string newpwd;				///< 新密码  newpwd
		CMD_SIZE();
	};

	/**
	* \struct  req_setMySelf 消息号: logic_cmd_setMyself
	* \brief 设置用户请求
	*/
	struct req_setMySelf : public _sdkcmd_base
	{
		st_userBean user; ///< 用户信息结构 user.
		void init() {
			user.info.emails.clear();
			user.info.phones.clear();
			user.init();
		}
		CMD_SIZE();
	};

	/**
	 * \struct  req_getUserFromID 消息号: logic_cmd_getUser
	 * \brief   根据ＩＤ获取用户信息
	 */
	struct  req_getUserFromID : public _sdkcmd_base
	{
		int64    userid;  ///< 用户ＩＤ userid.
		CMD_SIZE();
	};

	/**
	 * \struct req_LoginParamBean
	 * \brief 如果只进行域登录，只填写 :
	 * 1. area
	 * 2. usrid 填0
	 * 3. version
	 * 4. device_info
	 */
	struct req_LoginParamBean : public _sdkcmd_base
	{
		req_LoginParamBean()
		{
			device_type = 0;
			tick = 0;
			user = "";
			pwd = "";
			mac = device_info = version = net_type = local = area = nationalCode = "";
			sdkid = 0;
		}
		/**
		 * \brief  用户类型
		 * 手机 1，  qq 2 ， 邮箱 3 ，豆豆号 4， 身份证 5 , 6 豆豆账号
		 */
		int8 userType;
		/**
		 * \brief 设备类型
		 * 设备类型 1：PC；2：手机；3：pad；4：网页
		 */
		int32 device_type;
		/**
		 * \brief 踢人策略
		 * 1: 强制踢(登录) 2: 不踢人(重连)
		 */
		int32 tick;
		int64 usrid;                	        ///< 用户id　usrid.
		int64 sdkid;                            ///< SDKid,标识唯一APP sdkid.
		std::string mark;						///< 版本标识，mark.
		std::string user;                  	    ///< 用户名, user.
		std::string pwd;                   	    ///< 密码 pwd.
		std::string mac;                   	    ///< mac 地址 统一格式 XX:XX:XX:XX:XX:XX mac.
		/**
		 * \brief 设备信息
		 * pc-window7,an-xiaomei2,ios-iphone4s
		 */
		std::string device_info;
		std::string version;				    ///< 客户端版本,如1.23,1.2281(点后面超过2位为测试版) version.
		std::string net_type;				    ///< 网络类型,wifi或2g/3g或httpproxy net_type.
		std::string local;               	    ///< 语言及国家,zh_CN local.
		/**
		 * \brief
		 * 分两种
		 * 1. 使用ＶＲＶ服务器的二级域名
		 * 		比如传入 vrv 连接的服务器为 vrv.linkdood.cn
		 * 		传入 wechat 的连接服务器为 wechat.linkdood.cn;
		 * 2. IP地址　格式为　地址:端口 比如: 192.168.0.1:80 ，或者 163.com:80。
		 *
		 *
		 */
		std::string area;               	    ///< 多服务器的情况下,是服务器地址 area.
		std::string nationalCode;         		///< 国家代码 nationalCode.
		std::string captcha;                    ///< 验证码 captcha.
		CMD_SIZE();
	};

	/**
	*  开放登陆
	*/
	struct req_getLoginAuthCode : public _sdkcmd_base
	{
		req_getLoginAuthCode()
		{
			appID.clear();
			userID.clear();
			account.clear();
			INIT_ISSET(account);
			INIT_ISSET(password);
		}
		void init()
		{
			appID.clear();
			userID.clear();
			account.clear();
			INIT_ISSET(account);
			INIT_ISSET(password);
		}

		std::string appID;            ///< 应用ID
		std::string userID;           ///< 用户ID
		std::string accountType;      ///< 账户类型
		IS_SET(std::string, account)  ///< 账户
		IS_SET(std::string, password) ///< 密码
		CMD_SIZE();
	};
	/**
	 * \struct req_addBuddyResponse 消息号: logic_cmd_addBuddyResponse
	 * \brief 　加好友响应
	 */
	struct  req_addBuddyResponse : public _sdkcmd_base
	{
		int64 userID; 			  ///< 将要添加好友ＩＤ userID.
		std::string refuseReason; ///< 拒绝理由 refuseReason.
		std::string remark;       ///< 备注　remark.
		int8 isAgree;             ///< 是否同意 1：同意，2：不同意，3：以后不允许添加，4：忽略 isAgree.
		int64 reqVerifyboxID;     ///< 验证请求唯一编号 reqVerifyboxID.
		CMD_SIZE();
	};

	/**
	 * \struct  req_addBuddyRequest 消息号: logic_cmd_addBuddyRequest
	 * \brief 发送好友请求
	 */
	struct req_addBuddyRequest : public _sdkcmd_base
	{
		/**
		 * \brief １是加好友，　２是关注
		 */
		int8 requestType;
		int64 userID;                         ///< 被请求好友的ＩＤ userID.
		std::vector<std::string>  verifyInfo; ///< 验证信息 verifyInfo.
		std::string remark;                   ///< 好友备注 remark.

		void init()
		{
			verifyInfo.clear();
		}
		CMD_SIZE();
	};

	/**
	 * \struct req_regParam1  消息号: logic_cmd_reg1 logic_cmd_resetPw1
	 * \brief 注册账号第一步，找回密码第一步
	 */
	struct req_regParam1 : public _sdkcmd_base{
		int8                usertype;       ///<  用户类型 usertype.
		std::string   	    number;			///<  手机号或者电子邮箱 number.
		std::string         domain;			///<  企业域 domain.
		/**
		 * \brief 做预登录用,
		 * 请查看req_LoginParamBean　预登录填写规则
		 */
		req_LoginParamBean  login;
		CMD_SIZE();
	};

	/**
	* \struct req_regParam2   消息号: logic_cmd_reg2,logic_cmd_resetPw2
	* \brief  注册账号第二步，找回密码第二步
	*	     这条消息有2种使用模式
	*	     1: 一步走完，code、name和pwd都赋值
	*	     2: 两步走完，如下：
	*	     第一步：code赋值，name、pwd不赋值。
	*	     返回 resp_regResult2
	*	     第二步：name、pwd赋值，code不赋值。
	*		 返回 resp_regResult2
	*	     *registryID， sdkid都要赋值
	*/
	struct req_regParam2 : public _sdkcmd_base
	{
		int64          	registryID;       ///<  注册第一步返回的注册编号ID registryID.
		int64           sdkid;            ///<  使用SDK标识唯一APP sdkid.
		std::string  	code;             ///<  验证码 code.
		std::string  	name;             ///<  用户名 name.
		std::string  	pwd;              ///<  密码 pwd.
		CMD_SIZE();
	};

	/**
	 * \struct req_verifyImg  消息号：  logic_cmd_verifyImg
	 * \brief  验证码操作
	 */
	struct  req_verifyImg : public _sdkcmd_base
	{
		int8        next;		///< 1，更换下一张，2，不更换下一张 next.
		int8        type;		///< 验证码类型，默认填１ type.
		std::string usrName;	///< 用户名 usrName.
		std::string info;		///< 从验证码获得的检验信息 info.
		std::string macAddr;	///< MAC地址，必选参数 macAddr.
		CMD_SIZE();
	};

	/**
	 * \struct req_createGroup 消息号： logic_cmd_createGroup
	 * \brief 创建群
	 */
	struct req_createGroup : public _sdkcmd_base
	{
		req_createGroup()
		{
			init();
		}
		std::string serverInfo;		       ///< 本地服务器地址,当前没有使用 serverInfo.
		int8 level;			               ///< 群等级 1、2、3、4 level.
		std::string extend;	               ///< 群扩展字段 extend.
		std::vector<int64> inviteMembers;  ///< 邀请用户 inviteMembers.
		int8 groupSearch;			       ///< 群是否可以被检索，1.可以 2.不可以 默认1 groupSearch.
		int8 groupActive;			       ///< 群是否激活 1.激活 2.不激活 默认1 groupActive.
		int8  receiveMsgType;              ///< 1：提示并接收消息；2：不提示，接收仅显示数目；3：屏蔽消息 receiveMsgType.
		std::string receiveTimePeriod;    
		std::string name ;                 ///< 群名称.
		std::string brief ;                ///< 公告.
		int64       relatedGroupID ;       ///< 关联群，默认为零.
		int64       relatedEntID ;         ///< 关联企业 默认为零.

		void init() {
			serverInfo = "";
			level = 1;
			extend = "";
			inviteMembers.clear();
			groupSearch = 1;
			groupActive = 1;
			receiveMsgType = 1;
			receiveTimePeriod = "";
			name = "";
			brief = "";
			relatedGroupID =  0;
			relatedEntID = 0 ;
		}
		CMD_SIZE();
	};

	/**
	 * \struct req_groupVerifyRequest 消息号: logic_cmd_groupVerifyReq
	 * \brief 申请加入群请求
	 */
	struct req_groupVerifyRequest : public _sdkcmd_base
	{
		int64 groupID;				///< 群ID groupID.
		std::string verifyInfo;		///< 验证信息 verifyInfo.
		CMD_SIZE();
	};

	/**
	 * \struct req_groupInviteRequest　 消息号  logic_cmd_groupInviteReq
	 * \brief   邀请加入群请求命令
	 */
	struct req_groupInviteRequest : public _sdkcmd_base
	{
		int64 groupID;				///< 群ID groupID.
		std::string verifyInfo;		///< 验证信息 verifyInfo.
		std::vector<int64> users;	///< 邀请的用户ID集合　users.
		void init() {
			users.clear();
		}
		CMD_SIZE();
	};

	/**
	 *  \struct req_groupDelete 消息号 logic_cmd_groupDelete
	 *  \brief   删除群
	 */
	struct req_groupDelete : public _sdkcmd_base
	{
		int8  operType;				///< 1 群主解散群，2 群成员退群 operType.
		int64 groupid;				///< 群ID groupid.
		void init()
		{
			operType = 0;
			groupid = 0;
		}
		CMD_SIZE();
	};

	/**
	 *  \struct req_groupTrans 消息号 logic_cmd_groupTrans
	 *  \brief   群转移
	 */
	struct req_groupTrans : public _sdkcmd_base
	{
		int64 groupid;
		int64 targetUserid;
		void init()
		{
			groupid = 0;
			targetUserid = 0;
		}
		CMD_SIZE();
	};

	/**
	 *  \struct req_groupInfoUpdate 消息号 logic_cmd_groupUpdateInfo
	 *  \brief   群信息更新
	 */
	struct req_groupInfoUpdate : public _sdkcmd_base
	{
		req_groupInfoUpdate()
		{
			INIT_ISSET(groupName)
			INIT_ISSET(groupIcon)
			INIT_ISSET(groupBrief)
			INIT_ISSET(groupBulletin)
			INIT_ISSET(groupBackgroundURL)
			INIT_ISSET(groupSearch)
			INIT_ISSET(extendProperties)
			INIT_ISSET(groupMessageContentMode)
		}
		int64 groupID;
		IS_SET(std::string, groupName)				///< 群名称.
		IS_SET(std::string, groupIcon)				///< 群图标.
		IS_SET(std::string, groupBrief)				///< 群简介.
		IS_SET(std::string, groupBulletin)			///< 群公告.
		IS_SET(std::string, groupBackgroundURL)		///< 群背景图片.
		IS_SET(int8, groupSearch);					///< 群是否可以被外部检索 1可以 2不可以.
		/**
		 * \brief群扩展字段,以json形式,目前：  ;
		 * 警种ID： policeTypeID
		 * 地域---省份ID：provinceID
		 * 地域---市ID： cityID
		 * 私信模式：privateMsg：1为开启私信   0表示不允许私信
		 */
		IS_SET(std::string, extendProperties)	
		IS_SET(int8, groupMessageContentMode)		 ///< 群消息通知详情: 1、开启(显示详情)  2、关闭(显示通知源，隐藏内容)  默认开启.

		void init()
		{
			groupID = 0;
			groupSearch = 0;
			groupName = "";
			groupIcon = "";
			groupBrief = "";
			groupBulletin = "";
			groupBackgroundURL = "";
			INIT_ISSET(groupName)
			INIT_ISSET(groupIcon)
			INIT_ISSET(groupBrief)
			INIT_ISSET(groupBulletin)
			INIT_ISSET(groupBackgroundURL)
			INIT_ISSET(groupSearch);
			INIT_ISSET(extendProperties)
			INIT_ISSET(groupMessageContentMode)
		}
		CMD_SIZE();
	};

	/**
	 *  \struct req_groupCommonData 消息号
	 *  \brief   群请求通用接口体
	 */
	struct req_groupCommonData : public _sdkcmd_base
	{
		int64 groupid;
		void init()
		{
			groupid = 0;
		}
		CMD_SIZE();
	};

	/**
	 *  \struct req_removeGroupMember 消息号 logic_cmd_removeGroupMember
	 *  \brief   删除群成员
	 */
	struct  req_removeGroupMember : public _sdkcmd_base
	{
		int64   groupID;
		int64   userID;
		std::vector<int64> rmIDs;
		void init()
		{
			rmIDs.clear();
		}
		CMD_SIZE();
	};

	/**
	 *  \struct req_setGroupSet 消息号 logic_cmd_setGroupSet
	 *  \brief   设置群验证方式
	 */
	struct req_setGroupSet : public _sdkcmd_base
	{
		int64 groupID;
		st_groupSet gs;
		void init()
		{
			groupID = 0;
			gs.verifyType = 0;
			gs.isAllow = 0;
		}
		CMD_SIZE();
	};

	/**
	 *  \struct req_getMemberInfo
	 *  \brief
	 */
	struct req_getMemberInfo : public _sdkcmd_base{
		int64 groupid;
		int64 memberid;
		CMD_SIZE();
	};

	/**
	 *  \struct req_getGroupMemberInfo 消息号 logci_cmd_getGroupMemberInfo
	 *  \brief   获取群成员详细信息
	 */
	struct req_getGroupMemberInfo : public _sdkcmd_base
	{
		int64 groupid;
		int64 memberid;
		CMD_SIZE();
	};

	/**
	 * \struct req_setGroupMemberInfo  消息号 logic_cmd_setGroupMemberInfo
	 * \brief 设置群成员信息
	 */
	struct req_setGroupMemberInfo : public _sdkcmd_base
	{
		req_setGroupMemberInfo() {
			INIT_ISSET(memberName)
			INIT_ISSET(chatContext)
			INIT_ISSET(memType)
			INIT_ISSET(vSign)
			INIT_ISSET(groupMessageContentMode)
		}

		int64 groupId;
		int64 memberId;
		/**
		 * \brief 群成员类型
		 * 1：普通用户 2：管理员 3：超级管理员
		 */
		IS_SET(int8, memType)  
		IS_SET(std::string, memberName)  ///<群名片.
		IS_SET(std::string, chatContext) ///<暂时不用.
		IS_SET(int8, vSign) ///< v标群,0为非v标群，1为v标群.
		IS_SET(int8, groupMessageContentMode) ///< 群通知消息内容模式: 1、通知详情  2、通知源，隐藏内容  3、完全隐藏 , 默认开启模式1.
		CMD_SIZE();

		void init()
		{
			INIT_ISSET(memberName)
			INIT_ISSET(chatContext)
			INIT_ISSET(memType)
			INIT_ISSET(vSign)
			INIT_ISSET(groupMessageContentMode)
		}
	};

	/**
	 * \struct req_getGroupShieldInfo  消息号 logic_cmd_getShieldGroupMessage
	 * \brief 获取群消息提示方式
	 */
	struct req_getGroupShieldInfo : public _sdkcmd_base
	{
		int8  deviceType;		///< 1：pc；2：phone；3：html deviceType.
		int64 groupid;
		void init()
		{
			deviceType = 0;
			groupid = 0;
		}
		CMD_SIZE();
	};

	/**
	 * \struct req_setGroupShieldInfo  消息号 logic_cmd_setShieldGroupMessage
	 * \brief 设置群消息提示方式
	 */
	struct req_setGroupShieldInfo : public _sdkcmd_base
	{
		int8  receiveMsgType;			///< 1：提示并接收消息；2：不提示，接收仅显示数目；3：屏蔽消息 receiveMsgType.
		int64 groupid;
		std::string receiveTimePeriod;
		CMD_SIZE();
	};

	/**
	 * \struct req_setGroupReqForRead  消息号 logic_cmd_setGroupReqForRead
	 * \brief 设置群验证请求消息已读
	 */
	struct req_setGroupReqForRead : public _sdkcmd_base
	{
		int8 operType;                                   ///< 1 邀请  2 请求 operType.
		int64 groupID;
		std::vector<int64> inviteIDs;					 ///< 邀请 或者 请求 inviteIDs.
		//std::map<int64,std::vector<int64> > reqIDs;	 ///< 请求消息ID，KEY-群ID value-请求ID集合
		void init()
		{
			inviteIDs.clear();
			//reqIDs.clear();
		}
		CMD_SIZE();
	};

	/**
	 * \struct req_sendImg  消息号 logic_cmd_sendImg
	 * \brief 发送图片
	 */
	struct req_sendImg : public _sdkcmd_base
	{
		int64        targetID;      ///< 发送方ID targetID.
		std::string  encryptKey;    ///< 加密ＫＥＹ　不为空的话会对文件进行加密 encryptKey.
		std::string  thumb_path;    ///< 缩略图本地地址 thumb_path.
		std::string  orgi_path;     ///< 大图本地地址 orgi_path.
		void init()
		{

		}
		CMD_SIZE();
	};

	/**
	 * \struct req_recvImg  消息号 logic_cmd_recvImg
	 * \brief 接收图片
	 */
	struct req_recvImg : public _sdkcmd_base
	{
		int64   	targetID; 	///< 发送方对方ＩＤ targetID.
		int64       msgID;      ///< 消息ID msgID.
		std::string imgName;  	///< 文件名称 imgName.
		std::string rPath;  	///< 不带ＵＲＬ的远程路径 rPath.
		bool        isP2P;		///< 如果为true的话,
		CMD_SIZE();
	};

	/**
	* \struct req_sendNineBoxImg  消息号 logic_cmd_sendNineBoxImg
	* \brief 发送九宫格
	*/
	struct req_sendNineBoxImg : public _sdkcmd_base
	{
		req_sendNineBoxImg() :targetID(0){}
		int64        targetID;      ///< 发送方ID targetID.
		std::string  encryptKey;    ///< 加密ＫＥＹ　不为空的话会对文件进行加密 encryptKey.
		std::vector<std::string>  thumb_path;    ///< 缩略图本地地址 thumb_path.
		std::vector<std::string>  orgi_path;     ///< 大图本地地址 orgi_path. 缩略图数量需要和原图一致
		void init()
		{
			thumb_path.clear();
			orgi_path.clear();
			targetID = 0;
		}
		CMD_SIZE();
	};

	/**
	* \struct req_recvNineBoxImg  消息号 logic_cmd_recvNineBoxImg
	* \brief 接收九宫格图片
	*/
	struct req_recvNineBoxImg : public _sdkcmd_base
	{
		req_recvNineBoxImg() :targetID(0), msgID(0){}
		int64   	targetID; 	///< 发送方对方ＩＤ targetID.
		int64       msgID;      ///< 消息ID msgID.
		std::vector<std::string> rPath;  	///< 不带ＵＲＬ的远程路径 rPath.
		void init()
		{
			rPath.clear();
			targetID = 0;
			msgID = 0;
		}
		CMD_SIZE();
	};

	/**
	 * \struct req_getSysMsgList  消息号 logic_cmd_getSysMsgList
	 * \brief 获取系统消息列表
	 */
	struct req_getSysMsgList : public _sdkcmd_base
	{
		int8 type;                   ///< 0:全部 1:加好友请求 2:加好友响应 3:加群请求 4:加群响应 type.
		/* 
		  offsetFlag为0 以传入的time起始，向下偏移，拉取小于time的消息 如果time = 0，代表从最新收到的系统消息开始拉取
	      offsetFlag为1 以传入的time起始，向上偏移，拉取大于time的消息.
		*/
		int64 time;                  
		int32 msgOffset;			 ///< 消息列偏移量 msgOffset.
		int8 offsetFlag;             ///< 偏移标志，向上偏移 0；向下偏移 1 offsetFlag.
		CMD_SIZE();
	};

	/**
	* \struct req_deleteSysMsgList  消息号 logic_cmd_deleteSysMsgList
	* \brief 删除系统消息列表
	*/
	struct req_deleteSysMsgList : public _sdkcmd_base
	{
		int8 type;       ///< 0:表示删除所有系统消息， 1:删除加好友请求 2:删除加好友响应 3:删除加群请求 4:删除加群响应 type.
		std::vector<int64> msgIDs;   ///< 当type不为0时，存放需要删除的msgid.
		void init(){
			msgIDs.clear();
		}
		CMD_SIZE();
	};

	/**
	 * \struct req_setSysMsgReaded  消息号 logic_cmd_setSysMsgReaded
	 * \brief 设置系统消息已读
	 *	      1 好友请求验证框已读
	 *	      2 好友请求返回框已读
	 *	      3 设置群验证请求消息已读
	 *	      4 设置群验证响应消息已读
	 */
	struct req_setSysMsgReaded : public _sdkcmd_base {
		int8    type;      			///< 类型 type.
		std::vector<int64> msgIDs;
		void init(){
			msgIDs.clear();
		}
		CMD_SIZE();
	};

	/**
	 * \struct req_setIosNoticeInfo  消息号 logic_cmd_setUpiosRecvNotice
	 * \brief
	 */
	struct req_setIosNoticeInfo : public _sdkcmd_base{
		st_iosNoticeInfo info;
		CMD_SIZE();
	};

	/**
	 * \struct req_getFileList  消息号 logic_cmd_getFileList
	 * \brief  获取文件列表
	 */
	struct req_getFileList : public _sdkcmd_base
	{
		int64     targetID;		///< 目标ID targetID.
		int64     msgBeginID;	///< 起始ＩＤ号 msgBeginID.
		int32     msgOffset;	///< 消息列偏移量 msgOffset.
		int8      offsetFlag;	///< 偏移标志，向上偏移 0；向下偏移 1 offsetFlag.
		CMD_SIZE();
	};

	/**
	 * \struct req_delGroupFiles  消息号 logic_cmd_delGroupFiles
	 * \brief  删除群文件
	 */
	struct req_delGroupFiles : public _sdkcmd_base
	{
		std::vector<int64>  _vt;  ///< 删除文件的ＩＤ列表 _vt.
		void init() {
			_vt.clear();
		}
		CMD_SIZE();
	};

	/**
	 * \struct req_groupVerifyRespons  消息号 logic_cmd_groupVerifyResp
	 * \brief 群主或管理员收到进群请求时做出的响应，或用户被邀请进群时做出的响应
	 */
	struct req_groupVerifyRespons : public _sdkcmd_base
	{
		int64 reqID; 				///< 请求信息ID reqID.
		int64 groupID;				///< 群ID groupID.
		int8 respResult; 			///< 处理结果 1：忽略 2：同意 3： 拒绝  4:拒绝并不允许再次请求; 被邀请进群时，只能同意或拒绝 respResult.
		std::string refuseReason; 	///< 拒绝理由 refuseReason.
		CMD_SIZE();
	};

	/**
	 * \struct req_setOLtype  消息号
	 * \brief
	 */
	struct req_setOLtype : public _sdkcmd_base
	{
		bool type;
		CMD_SIZE();
	};

	/**
	 * \struct req_setTokenAndOlState  消息号 logic_cmd_setTokenAndOlState
	 * \brief IOS设置Token和云后台在线
	 */
	struct req_setTokenAndOlState : public _sdkcmd_base
	{
		/**
		 * \brief type 类型
		 *   0, 默认值，客户端可以不用设置该值，
		 *      设置token和appleID；需要参数appleID，token；返回code
		 *   1. 设置云后台在线状态 . 需要字段vb1设置云后台在线(true 在线, false 不在线),vb2设置应用在线(true在线), 返回code
		 */
		int8 type;
		std::string appleID;	///< 应用id， 不同的应用所对应的证书是不一样的 appleID.
		std::string token;		///< ios token token.
		bool vb1;				///< 设置云后台在线 vb1.
		bool vb2;				///< 设置应用在线 vb2.
		CMD_SIZE();
	};

	/**
	 * \struct req_delReContact  消息号 logic_cmd_delReContact
	 * \brief
	 */
	struct req_delReContact : public _sdkcmd_base
	{
		int64 targetID; ///< 删除聊天对象得ID， 如果为零，则删除全部最近聊天.
		CMD_SIZE();
	};

	/**
	 * \struct req_cancleExec  消息号 logic_cmd_cancleExec
	 * \brief
	 */
	struct req_cancleExec : public _sdkcmd_base
	{
		int64 cmdLocalID;		    ///< 被取消的消息localID cmdLocalID.
		int16 cmdType;			///< 消息类型 cmdType.
		CMD_SIZE();
	};

	/**
	 * \struct req_sendErrLog  消息号 logic_cmd_sendErrLog
	 * \brief
	 */
	struct req_sendErrLog : public _sdkcmd_base
	{
		int8  osType; 		///< 操作系统类型 osType.
		std::string path; 	///< 错误日志路径 path.
		std::string desc;	    ///< 描述 desc.
	};

	/**
	 * \struct req_getAppinfo  消息号 logic_cmd_getAppInfo
	 * \brief
	 */
	struct req_getAppinfo : public _sdkcmd_base
	{
		int64  appID;  ///< APPID appID.
		CMD_SIZE();
	};

	/**
	 * \struct req_localSetting  消息号 logic_cmd_localSetting
	 * \brief 本地设置请求结构
	 */
	struct req_localSetting : public _sdkcmd_base
	{
		int8   type;  			///< 0 : 增加，１：查询 , 2: 修改，３：删除 type.
		/**
		 * \brief type 的值不同，items有不同的意义
		 * 	0: items里面是新插入的值
		 * 	1: items里面带要查询的值，st_localSetting只给key赋值即可
		 * 	2: items带最新的值
		 * 	3: items里面带要删除的值，st_localSetting只给key赋值即可
		 */
		std::vector<st_localSetting> items;
		void init() {
			items.clear();
		}
		CMD_SIZE();
	};

	/**
	 * \struct req_transferOuterMsgDb  消息号 logic_cmd_transferOuterMsgDb
	 * \brief  迁移外部聊天数据库
	 */
	struct req_transferOuterMsgDb : public _sdkcmd_base
	{
		std::string outerDbPath;	///< 外部数据库路径 outerDbPath.
		CMD_SIZE();
	};

	/**
	 * \struct  req_PersonalData 消息号　logic_cmd_setPersonalData
	 * \brief 个人信息设置,
	 */
	struct  req_PersonalData : public _sdkcmd_base
	{
		std::vector<st_personalData> items;
		void init()
		{
			items.clear();
		}
		CMD_SIZE();
	};

	/**
	* \struct  req_msgGlobalSearch 消息号　logic_cmd_msgGlobalSearch
	* \brief 本地消息全局搜索，只返回每个匹配到的群或个人的消息的数目。
	*/
	struct req_msgGlobalSearch : public _sdkcmd_base
	{
		req_msgGlobalSearch() :targetId(0), count(1), msgType(2),
		startTime(0), endTime(0), func(NULL)
		{}

		/**
		 * 　\brief targetId为用户Id或群Id，与count字段共同决定搜索的方式，有以下几种组合：
		 *   targetId !=0, count = 1:搜索指定的targetId的聊天记录数目。
		 * 　targetId = 0,count > 1: 从最小的targetId开始向上搜索记录，共搜索到count个匹配的targetId为止。
		 *   targetId !=0, count >1: 从指定的targetId开始向上搜索记录，共搜索到count个匹配的targetId为止。
		 *   count = 0为匹配全部消息，最多匹配10000条
		 */
		int64 targetId;
		int count;	
		int16  msgType;   ///< 搜索消息的类型，1:html 2:文本，3:音频 4: 位置 5:图片6:文件 7:名片 8:弱提示 (当前只支持文本).
		std::string key;  ///< 搜索的关键字.
		int64 startTime;	///< 搜索消息的最小时间,不能大于endTime,单位秒.
		int64 endTime;	///< 搜索消息的最大时间,0为搜索全部时间段,单位秒.
		MatchFunc func;	///< 传入的用户自定义的匹配字符串的函数,为空时使用默认匹配方式
		CMD_SIZE();
	};

	/**
	 * \struct  req_msgDetailSearch 消息号　logic_cmd_msgDetailSearch
	 * \brief 在某个群或个人的聊天记录中搜索关键字。
	 */
	struct req_msgDetailSearch : public _sdkcmd_base
	{
		req_msgDetailSearch() :targetId(0), msgType(2), startTime(0), endTime(0), msgId(0), count(0), func(NULL)
		{}
		int64 targetId;	   	 ///< 目标Id.
		int16  msgType;      ///< 搜索消息的类型，1:html 2:文本，3:音频 4: 位置 5:图片6:文件 7:名片 8:弱提示 (当前只支持文本).
		int64  msgId;		 ///< 起始msgId，0为从最早的消息就行搜索.
		int count;			 ///< 一次匹配到最大的消息数量,0为匹配全部消息,最多匹配100000条.
		std::string key;	 ///< 搜索的关键字.
		int64 startTime;	 ///< 搜索消息的最小时间,不能大于endTime，单位秒.
		int64 endTime;	     ///< 搜索消息的最大时间,0为搜索全部时间段, 单位秒.
		MatchFunc func;	///< 传入的匹配字符串的函数,为空时使用默认匹配方式
		CMD_SIZE();
	};

	/**
	 * \struct  req_addToBWlist 消息号　logic_cmd_addToBWlist
	 * \brief 添加用户到黑名单或白名单
	 */
	struct req_addToBWlist : public _sdkcmd_base
	{
		req_addToBWlist() :type(1), enterpriseId(0)
		{}
		int8 type;					 ///< 操作类型，1.黑名单 2.白名单.
		std::vector<int64> memList;	 ///< 添加到名单的成员ID集合.
		int64 enterpriseId;			 ///< 企业ID，当type为2时设置.

		void init()
		{
			memList.clear();
		}
		CMD_SIZE();
	};

	/**
	 * \struct  req_removeFromBWlist 消息号　logic_cmd_removeFromBWlist
	 * \brief 把用户从黑名单或白名单中移除
	 */
	struct req_removeFromBWlist : public _sdkcmd_base
	{
		req_removeFromBWlist() :type(1), enterpriseId(0)
		{}
		int8 type;					 ///< 操作类型，1.黑名单 2.白名单.
		std::vector<int64> memList;	 ///< 移除的成员ID集合,为空时，代表清空名单中的所有用户.
		int64 enterpriseId;			 ///< 企业ID，当type为2时设置.

		void init()
		{
			memList.clear();
		}
		CMD_SIZE();
	};


	/**
	 * \struct  req_getBWlist 消息号　logic_cmd_getBWlist
	 * \brief 获取黑名单或白名单列表
	 */
	struct req_getBWlist : public _sdkcmd_base
	{
		req_getBWlist() :type(1), enterpriseId(0)
		{}

		int8 type;					 ///< 操作类型，1.黑名单 2.白名单.
		int64 enterpriseId;			 ///< 企业ID，当type为2时设置.
		CMD_SIZE();
	};

	/**
	 * \struct  req_delMsg 消息号: logic_cmd_delMessage
	 * \brief 删除消息结构体 时间上的单位为毫秒
	 * 使用示例:
	 * (1) 删除所有用户的所有时间的消息
	 * 　　　　req_delMsg　param ; ///使用默认值，targetID = 0 ;startTime = endTime = 0;
	 *     bSyncSrv 有效，true为同步删除服务器数据，false为只删除客户端数据.
	 * (2) 删除某个用户(ID=10000)所有的消息
	 *     req_delMsg　param ;
	 *     param.targetID = 10000 ;
	 *	　　　param.startTime = param.endTime = 0 ;
	 *	   bSyncSrv 有效，true为同步删除服务器数据，false为只删除客户端数据.
	 * (3) 删除所有用户某段时间内（st，et）的消息
	 *     param.targetID = 0 ;
	 *     param.startTime = st ;
	 *     param.endTime = et ;
	 * (4) 删除某个用户(ID=10000)下的一些消息(消息号为；10000:10001),如果bSyncSrv为true，传入的msgId对应的消息必须为阅后即焚消息
	 *     param.targetID = 10000 ;
	 *     param.idVt.push_back(10000);
	 *	   param.idVt.push_back(10001);
	 *　(5) 删除某个用户(ID=10000)某段时间(st,et)内的消息
	 *     param.targetID = 10000 ;
	 *     param.startTime = st ;
	 *     param.endTime = et ;
	 *　(6) 删除某个用户(ID=10000)某段时间(st,et)内的消息
	 *     param.targetID = 10000 ;
	 *     param.startTime = st ;
	 *     param.endTime = et ;
	 *
	 * 就以上５种组合方式，其他组合方式暂不支持。
	 */
	struct req_delMsg : public  _sdkcmd_base
	{
		req_delMsg() {
			targetID = 0;
			startTime = endTime = 0;
		}
		int64   targetID;     ///< 删除消息的目标ＩＤ,targetID = 0 为删除所有用户.
		int64 startTime;		///< 起始时间,不能大于endTime，单位毫秒.
		int64 endTime;		///< 结束时间,搜索全部时间段, 单位毫秒,endTime＝０为删除所有时间段的消息.
		std::vector<int64>      idVt; ///< 删除消息的ＩＤ列表.		
		bool     bSyncSrv ;   ///< 是否同步到服务器. 如果是阅后即焚设置为TRUE.
		bool     bDelContact; ///< 当targetID=0时，表示是否删除所有的最近联系人，为ture表示删除.
		void init()
		{
			idVt.clear();
			bSyncSrv = false;
		}
		CMD_SIZE();
	};

	/**
	 * \struct  req_chatTop 消息号　logic_cmd_chatTop
	 * \brief 聊天置顶
	 */
	struct req_chatTop : public  _sdkcmd_base
	{
		req_chatTop() :targetId(0), type(0)
		{}

		int64 targetId;		///< 操作的目标ID.
		int8 type;			///< 操作类型 0不置顶 1置顶.

		CMD_SIZE();
	};

	/**
	 * \struct  req_setUserSetting 消息号　logic_cmd_setUserSetting
	 * \brief 特定用户信息设置(是否显示在线，设置用户被查找方式(豆豆号，手机号等),新消息提醒方式等)
	 */
	struct req_setUserSetting : public  _sdkcmd_base
	{
		req_setUserSetting() :type(1), flag(0)
		{}
		/**
		 * \brief
		 *  type = 1: 设置是否显示在线信息 flag: 0显示 1不显示 默认0 
		 *  type = 3: 设置豆豆号查找 flag: 0允许 1不允许 默认0 
		 *  type = 4: 设置手机号查找 flag: 0允许 1不允许 默认0 
		 *  type = 5: 设置邮箱号查找 flag: 0允许 1不允许 默认0 
		 *  type = 6: 设置分享更新   flag: 0提示更新 1不提示更新 默认0 
		 *  type = 7: 新消息通知是否提醒 flag: 0提醒 1不提醒 默认0 
		 *  type = 12: 多服务新消息通知是否提醒 flag: 0不始终提示 1始终提示 默认0
		 *  type = 13: 多服务设置V标好友始终提醒 flag: 0不始终提示 1始终提示 默认0
		 *  type = 14: 多服务设置设置@相关人始终提醒 flag: 0不始终提示 1始终提示 默认0
		 */
		int32 type;	
		int8 flag;
		CMD_SIZE();
	}; 

	/**
	* \struct  req_getUserSetting 消息号　logic_cmd_getUserSetting
	* \brief 获取特定用户信息(是否显示在线，设置用户被查找方式(豆豆号，手机号等),新消息提醒方式等)
	*/
	struct req_getUserSetting : public  _sdkcmd_base
	{
		req_getUserSetting() :type(0)
		{}
		/**
		* \brief
		* 通过类型得到设置信息，返回值为resp_commonResult的value_i64字段.
		* 如果type = 0 ,返回所有字段，每个字段所占的位于type向对应。(如是否在线.
		* 信息type为1，则返回值为value_i64第一位. 
		*/
		int32 type;
		CMD_SIZE();
	};

	/**
	 * \struct  req_msgImport 消息号　logic_cmd_msgImport
	 * \brief 导入外部聊天消息
	 */
	struct req_msgImport : public  _sdkcmd_base
	{
		std::vector<st_msgBean> messages;	///< 需要导入的消息.

		void init()
		{
			messages.clear();
		}

		CMD_SIZE();
	};

	/**
	 * \struct  req_getFileInfos 消息号　logic_cmd_getFileInfos
	 * \brief 得到保存文件信息
	 */
	struct req_getFileInfos : public  _sdkcmd_base
	{
		std::vector<int64> fileMsgIds;	 ///< 需要获取的文件消息ID.

		void init()
		{
			fileMsgIds.clear();
		}

		CMD_SIZE();
	};


	/**
	 * \struct req_offlineLogin  消息号: logic_cmd_offlineLogin
	 * \brief 离线登录结构体
	 */
	struct req_offlineLogin : public _sdkcmd_base
	{
		int64        userID;  ///< 用户ＩＤ 如果为零，获取最近一次登录成功的用户ＩＤ.
		std::string  pwd ;    ///< 密码.
		void init(){;}
		CMD_SIZE();
	};

	/**
	 * \struct  req_getMsgByType 消息号　logic_cmd_getMsgByType
	 * \brief 根据消息类型获得聊天消息
	 */
	struct req_getMsgByType : public _sdkcmd_base
	{
		req_getMsgByType() :targetId(0), msgType(0){}
		int64 targetId;			///< 个人或群ID.
		int8 msgType;			    ///< 获取的消息类型.
		CMD_SIZE();
	};

	/**
	 * \struct  req_getUrlInfo 消息号　logic_cmd_getUrlInfo
	 * \brief 获取网址缩略图信息
	 */
	struct req_getUrlInfo : public _sdkcmd_base
	{
		std::string url;			///< 网址url.
		std::string loginfoPath ;       ///< 登录文件路径
		CMD_SIZE();
	}; 

	/**
	 * \struct  req_faceToFaceJoin 消息号　logic_cmd_faceToFaceJoin
	 * \brief 近距离加好友、群
	 * 
	 * 根据type取值不同进行不同操作:
	 * 1: 创建近距离建群/加好友的房间。根据传入经度, 纬度, 创建房间。 
	 *	  传入参数： st_faceToFaceBean  房间信息,不填password，由服务器生成
	 *    返回：
	 *		code:　0:成功，100:参数错误，11100:内部错误
	 *		resp_faceToFaceJoin 含有randomCode,即服务器产生的password
	 *　		
	 * 2: 加入近距离建群房间。根据传入经度、纬度、房间密码，搜索缓存队列是否存在房间。
	 *    存在：加入房间人员队列，推送通知，通知房间其他人，此人加入此房间返回房间信息和人员队列 
	 *    不存在：返回房间不存在
	 *	  传入参数: st_faceToFaceBean 房间信息,需要填password
	 *	   返回：
	 *	 	code:　0:成功，100:参数错误，11100:内部错误
	 *		resp_faceToFaceJoin
	 *
	 * 3: 退出近距离建群房间。根据房间ID，检索房间信息，看人员是否是创建者
 	 *	  是：删除此房间，推送通知，通知人员队列人员，房间删除
	 *    否：删除此队列中此人员，推送通知，通知其他人员，此人退出房间
   	 *    传入参数： userId, roomId
	 *	  返回:
	 *　	    code:　0:成功，100:参数错误，11100:内部错误
	 *
	 * 4: 近距离建群。根据房间ID，判断房间队列是否存在。
	 *    传入参数： userId，roomId
 	 *				groupLevel等级，userList加入群的用户列表
	 *
	 *		 返回：
	 *　　	code: 0:成功，100:参数错误，302:没有权限创建群（非管理员）
	 *	          344:创建群失败，345:房间不存在，11100:内部错误
	 *
	 * 5: 加入近距离加好友房间。根据传入经度、纬度、房间密码，搜索缓存队列是否存在房间。
	 *    存在：检测与房间主是否是好友，如果是返回已经是好友关系
     *  	　　加入房间人员队列，推送通知给房间创建者，此人加入此房间,返回房间信息和房主信息
     *  不存在：创建房间人员队列,返回房间信息,人员队列的第一个人为创建者。
   	 * 传入参数： st_faceToFaceBean 房间信息,需要填password
	 *	  返回：
	 *		code:　0:成功，100:参数错误，11100:内部错误
	 *　　			resp_faceToFaceJoin
   	 *
	 * 6: 退出近距离加好友房间。根据房间ID，检索房间信息，看人员是否是创建者
	 *	  是：删除此房间，推送通知，通知人员队列人员，房间删除
	 *    否：删除此队列中此人员，推送通知，通知房主，此人退出房间
	 *    传入参数：userId , roomId
	 *    返回:
	 *		code: 0:成功，100:参数错误，11100:内部错误
	 * 
	 * 7: 近距离加好友。根据房间ID，判断房间队列是否存在，
	 *	  调用消息盒子服务，批量给成员发添加好友验证请求。
	 * 传入参数: userId ,roomId, verifyInfo，userList
	 *   返回：
	 *   	code: 0:成功，100:参数错误，340:没有权限操作（非管理员）
	 *           556:操作失败，345:房间不存在，11100:内部错误
	 */
	struct req_faceToFaceJoin : public _sdkcmd_base
	{
		req_faceToFaceJoin() :type(0), userId(0), groupLevel(0)
		{
			INIT_ISSET(userId)
			INIT_ISSET(roomId)
			INIT_ISSET(verifyInfo)
			INIT_ISSET(userList)
			INIT_ISSET(faceToFace)
			INIT_ISSET(groupLevel)
			INIT_ISSET(groupName)
		}
		void init()
		{
			userList.clear();
			faceToFace.init();
			INIT_ISSET(userId)
			INIT_ISSET(roomId)
			INIT_ISSET(verifyInfo)
			INIT_ISSET(userList)
			INIT_ISSET(faceToFace)
			INIT_ISSET(groupLevel)
			INIT_ISSET(groupName)
		}

		int8 type;							///< 操作类型.
		IS_SET(int64, userId)					///< 用户ID.
		IS_SET(std::string, roomId)				///< 房间ID.
		IS_SET(std::string, verifyInfo)			///< 加好友验证信息.
		IS_SET(std::vector<int64>, userList)   	///< 用户Id列表.
		IS_SET(st_faceToFaceBean, faceToFace)	///< 详细信息.
		IS_SET(int8, groupLevel)				///< 群等级，创建群时使用.必须传 
		IS_SET(std::string, groupName)			///< 群等级，创建群时使用.必须传 
		
		CMD_SIZE();
	};

	/**
	* \struct req_delDelayMsg
	* \brief 删除延时消息使用 消息号 logic_cmd_delDelayMsg
	*
	*/
	struct  req_delDelayMsg : public _sdkcmd_base{
		int64   targetID;   ///< 该消息所属用户/群ID.
		int64  	msgID;     ///< 删除消息号.
		CMD_SIZE();
	};

	/**
	* \struct req_updateDelayMsg
	* \brief 更新自己的消息 消息号 logic_cmd_updateDelayMsg
	*
	*/
	struct  req_updateDelayMsg : public _sdkcmd_base {
		int64   targetID;  ///< 该消息所属用户/群ID.
		st_msgBean  msg;   ///< 更新消息信息.
		CMD_SIZE();
	};


	/**
	* \struct  req_authenticationParam 消息号　logic_cmd_bindEmailOrPhoneNumber1
	* \brief 绑定手机号第1步 请求绑定手机号
	* 返回：code 0: 操作成功/验证码正确（发送成功）	 100:参数不正确
	* 381: 未指定接收者			                   382: 未指定接收者类型
	* 383: 验证码已过期			                   384: 验证码不正确
	* 385: 发送间隔时间太短			               386: 发送失败
	* 387: 未发送过验证码
	*/
	struct req_authenticationParam : public _sdkcmd_base
	{
		req_authenticationParam() :operation(0)
		{

		}
		int8 operation;			///< 1. 绑定.
		std::string phone;		    ///< 手机号.
		std::string language;      ///< 语言.

		CMD_SIZE();
	};

	/**
	* \struct  req_authenticationServerCode 消息号　logic_cmd_bindEmailOrPhoneNumber2
	* \brief 绑定手机号第2步，验证验证码
	* 返回：code　0: 操作成功/验证码正确（发送成功）	100:参数不正确
	*　　120: 帐号不存在			                  381: 未指定接收者
	*    382: 未指定接收者类型            	         383: 验证码已过期
	*    384: 验证码不正确 			              385: 发送间隔时间太短
	*    386: 发送失败 			                   387：未发送过验证码
	*/
	struct req_authenticationServerCode : public _sdkcmd_base
	{
		req_authenticationServerCode() :operation(0), registryID(0)
		{
		}
		int8 operation;   ///< 1: 绑定 2: 解绑.
		int64 registryID; ///< 第一步返回的　registryID （豆豆ID）.
		std::string code; ///< 验证码.
		CMD_SIZE();
	};

	/**
	* \struct  req_MultiOpSTParam 消息号　logic_cmd_bindEmailOrPhoneNumber3
	* \brief 绑定手机号第3步，绑定手机号或邮箱
	返回：code　0:成功，100:参数错误，120: 帐号不存在，510:帐号已被使用
	*/
	struct req_MultiOpSTParam : public _sdkcmd_base
	{
		req_MultiOpSTParam() :type(0), operation(0), registryID(0)
		{
		}
		int8 type;         ///< 1：绑定手机 2：绑定邮箱.
		int8 operation;    ///< 1: 绑定 2: 解绑.
		int64 registryID;  ///< 需要验证的那些都需要填写.
		std::string code;
		CMD_SIZE();
	};

	/**
	* \struct  req_setNoDisturbMode 消息号　logic_cmd_setNoDisturbMode
	* \brief 设置全局勿扰模式开关信息
	*/
	struct req_setNoDisturbMode : public _sdkcmd_base
	{
		req_setNoDisturbMode() :startTime(0), endTime(0), flag(0)
		{
		}
	
		int32 startTime;	    ///< 起始时间.
		int32 endTime;		///< 结束时间.
		int8  flag;			///< 开关是否打开，0关闭，1打开.

		CMD_SIZE();
	};

	/**
	* \struct  req_setUserToTargetSwitch 消息号　logic_cmd_setUserToTargetSwitch
	* \brief 设置个人对目标用户的勿扰模式
	*/
	struct req_setUserToTargetSwitch : public _sdkcmd_base
	{
		req_setUserToTargetSwitch() :targetId(0), type(1), value(1)
		{
		}

		int64 targetId;	    ///< 用户ID.
		int8  type;		    ///< 设置类型 1:个人设置项 2:个人通知消息详情模式.
		/** 
		 * \brief 类型值
		 * 1:个人设置项, value: 1为接收提醒 2表示不提醒仅显示数字 3:为免打扰, 默认1
		 * 2:个人通知消息详情模式, value:1、通知详情 2、通知源，隐藏内容 3、完全隐藏 默认开启模式1
		 */
		int8  value;	

		CMD_SIZE();
	}; 
	
		/**
		* \struct  req_getUserToTargetSwitch 消息号　logic_cmd_getUserToTargetSwitch
		* \brief 获取个人对目标用户的勿扰模式
		*/
	struct req_getUserToTargetSwitch : public _sdkcmd_base
	{
		req_getUserToTargetSwitch() :targetId(0), type(0)
		{
		}

		int64 targetId;	    ///< 获取的用户ID.
		int8 type;		    ///< 获取的设置类型 0:获取全部 1:个人设置项 2:个人通知消息详情模式.

		CMD_SIZE();
	};

	/**
	* \struct  req_postContact 消息号　logic_cmd_postContact
	* \brief 上传通讯录
	*/
	struct req_postContact : public _sdkcmd_base
	{
		req_postContact() :flag(1)
		{
		}

		void init()
		{
			postContact.clear();
		}

		std::vector<st_contactBean> postContact;
		int8 flag;		  ///< 1 服务器推荐逻辑 2 添加通讯录好友.

		CMD_SIZE();
	};

	/**
	* \struct  req_shareOption 消息号　net_cmd_updateShareOption
	* \brief 前端用户设置分享列表
	*/
	struct req_shareOption : public _sdkcmd_base
	{
		req_shareOption()
		{
			shareOpt.clear();
		}
		std::vector<st_shareOptionBean> shareOpt;

		CMD_SIZE();
	};

	/**
	* \struct  req_sharedType 消息号　logic_cmd_getShareOption
	* \brief 前端用户设置分享列表
	*/
	struct req_sharedType : public _sdkcmd_base
	{
		int8 sharedType;  ///< 要查询的类型：1为不让他人看我的分享，2为不看他的分享.

		CMD_SIZE();
	};

	/**
	* \struct  req_personDefinitionParam 消息号　logic_cmd_setPersonDefinition
	* \brief   用户自定义一些信息， 目前只表示声音
	*/
	struct req_personDefinitionParam : public _sdkcmd_base
	{
		req_personDefinitionParam()
		{
			personDef.defType = 0;
			INIT_ISSET(userID)
		}
		IS_SET(int64, userID) ///< 可以设置其他用户， 不设置默认为自己.
		st_personDefinition personDef;

		CMD_SIZE();
	};

	/**
	* \struct  req_getPersonDef 消息号　 logic_cmd_getPersonDefinition
	* \brief   得到用户自定义一些信息
	*/
	struct req_getPersonDef : public _sdkcmd_base
	{
		req_getPersonDef() : defType(0)
		{
			INIT_ISSET(userID)
		}
		IS_SET(int64, userID) ///< 可以设置其他用户， 不设置默认为自己.
		int8 defType;         
		
		CMD_SIZE();
	};
	
		/**
	* \struct  req_queryExtendedField 消息号　logic_cmd_queryExtendedField
	* \brief  根据条件查询拓展字段信息
	*/
	struct req_queryExtendedField : public _sdkcmd_base
	{
		req_queryExtendedField() : type(0)
		{
			INIT_ISSET(dicKey)
		}
		int8 type;                 ///< 1. type 类型 根据查询条件查询出所有的拓展字段信息.
		IS_SET(std::string, dicKey)  ///< 当不给dicKey赋值时表示查询所有扩展字段.

		CMD_SIZE();
	}; 

	/**
	* \struct  req_setUserGroupImage 消息号　logic_cmd_setUserGroupImage
	* \brief  设置成员群背景图片
	*/
	struct req_setUserGroupImage : public _sdkcmd_base
	{
		req_setUserGroupImage() :groupID(0)
		{

		}
		int64 groupID;
		std::string userBackgroundURL;

		CMD_SIZE();
	};

	/**
	* \struct  req_getUserGroupImage 消息号　logic_cmd_getUserGroupImage
	* \brief  获取成员群背景图片
	*/
	struct req_getUserGroupImage : public _sdkcmd_base
	{
		req_getUserGroupImage() :groupID(0)
		{

		}
		int64 groupID;

		CMD_SIZE();
	};
	

	/**
	 * \struct req_p2pTransferCancle 消息号 logic_cmd_p2pFileTransCancle
	 * \brief P2P文件传输操作 , 如果选择接收，接收端推送上来的进度标识就为本条消息的LOCALid
	 */
	struct req_p2pTransferCancle : public _sdkcmd_base
	{
		req_p2pTransferCancle() {

		}
		int64  taskID ;	///< 接收到请求推送里面的.
		CMD_SIZE();
	};


	/**
	* \struct req_queryMarketApplication 消息号 logic_cmd_queryMarketApplication
	* \brief  应用市场分页查询条件
	*/
	struct req_queryMarketApplication : public _sdkcmd_base
	{
		req_queryMarketApplication() :pageNum(0), pageSize(0), deviceType(0), userID(0)
		{
			INIT_ISSET(appName)
		}

		void init()
		{
			INIT_ISSET(appName)
		}
		int32 pageNum;                 ///< 页码.
		int32 pageSize;                ///< 页长.
		int32 deviceType;              ///< 类型 2（IOS），3（Android）.
		int64 userID;                 ///< 用户ID，预留类型，暂时不用输入.
		IS_SET(std::string, appName)    ///< 应用名称 模糊查询用.

		CMD_SIZE();
	};

	/**
	* \struct req_addOrDeleteApplication 消息号 logic_cmd_addOrDeleteApplication
	* \brief 添加或删除应用
	*/
	struct req_addOrDeleteApplication : public _sdkcmd_base
	{
		req_addOrDeleteApplication() :type(0), appID(0)
		{

		}
		int8 type;    ///< 2.添加应用  4.删除应用.
		int64 appID;

		CMD_SIZE();
	};

	/**
	* \struct getInstalledApplication 消息号 logic_cmd_getInstalledApplication
	* \brief  获取用户已安装列表信息
	*/
	struct req_getInstalledApplication : public _sdkcmd_base
	{
		req_getInstalledApplication() :deviceType(0)
		{

		}
		int32 deviceType; ///< 2.IOS 3.Android.

		CMD_SIZE();
	};

	
	/**
	* \struct req_getEntAppInfo 消息号 logic_cmd_getEntAppInfo
	* \brief  根据用户id分页获取企业号
	*/
	struct req_getEntAppInfo : public _sdkcmd_base
	{
		req_getEntAppInfo() :pageNum(0), pageSize(0), status(0), userID(0)
		{
			INIT_ISSET(entID)
		}
		int32 pageNum;          ///< 页码
		int32 pageSize;         ///< 每页大小
		int8 status;            ///< 状态 1待审核，2可用，3审核不通过，4停用，5锁定，6删除
		int64 userID;           ///< 登录用户id
		IS_SET(int64, entID);   ///< 企业ID，能获取到就传获取到的值  获取不到 就传0
		CMD_SIZE();
	};
	/**
	* \struct req_queryEmotion 消息号 logic_cmd_queryEmoticon
	* \brief  单个表情查询、根据表情包标识查询表情，查询所有用户自定义表情
	*/
	struct req_queryEmoticon : public _sdkcmd_base
	{
		req_queryEmoticon() : type(0)
		{
			INIT_ISSET(mdCode)
		}
		/**
		 *1, 单个查询表情 mdCode表示单个表情的md5, 6,根据表情包标识查询所有表情 mdCode表示表情包的md5, 7,查询用户所有自定义表情 这里mdCode不传值
		 */
		int8 type;               
		IS_SET(std::string, mdCode);      ///< 单个表情或者表情包的md5Code.

		CMD_SIZE();
	};

	/**
	* \struct req_querySingleEmoticon 消息号 logic_cmd_querySingleEmoticon
	* \brief  查询某个表情包里单个表情信息
	*/
	struct req_querySingleEmoticon : public _sdkcmd_base
	{
		req_querySingleEmoticon()
		{

		}
		std::string packMdCode;    ///< 表情包mdCode
		std::string mdCode;        ///< 包内单个表情mdCode

		CMD_SIZE();
	};

	/**
	* \struct req_singleQueryEmoticonPackage 消息号 logic_cmd_singleQueryEmoticonPackage
	* \brief  单个查询表情包
	*/
	struct req_singleQueryEmoticonPackage : public _sdkcmd_base
	{
		req_singleQueryEmoticonPackage()
		{

		}
		std::string mdCode;            ///< 单个查询表情包的mdCode.

		CMD_SIZE();
	};

	/**
	* \struct req_pageQueryEmoticon 消息号 logic_cmd_pageQueryEmoticon
	* \brief  分页查询表情包
	*/
	struct req_pageQueryEmoticon : public _sdkcmd_base
	{
		req_pageQueryEmoticon() :pageNum(0), pageSize(0)
		{

		}
		int32 pageNum;                 ///< 页码.
		int32 pageSize;                ///< 页长.
		CMD_SIZE();
	};

	/**
	* \struct req_addOrDeleteCustomEmoticon 消息号 logic_cmd_addOrDeleteCustomEmoticon
	* \brief  增加或删除用户自定义表情
	*/
	struct req_addOrDeleteCustomEmoticon : public _sdkcmd_base
	{
		req_addOrDeleteCustomEmoticon() : type(0)
		{

		}
		int8 type;            ///< 4 增加， 5删除.
		st_emoticon emot;
		CMD_SIZE();
	};

	/**
	* \struct req_labelQueryEmoticonPackage 消息号 logic_cmd_labelQueryEmoticonPackage
	* \brief  根据表情包标签查询相关表情包集合
	*/
	struct req_labelQueryEmoticonPackage : public _sdkcmd_base
	{
		req_labelQueryEmoticonPackage()
		{

		}
		std::string label;     ///< 表情包标签.
		CMD_SIZE();
	};

	/**
	*
	* \brief 查询机器人（公众号）时间戳，查询数据字典时间戳 消息号 logic_cmd_queryAppOrDictTimestamp
	1, 单个查询机器人（公众号）时间戳
	2, 查询数据字典时间戳 查询字典时间戳appID不用赋值
	*/
	struct req_queryAppOrDictTimestamp : public _sdkcmd_base
	{
		req_queryAppOrDictTimestamp() : type(0), appID(0)
		{

		}
		int8 type;    //1.机器人（公众号） 2.字典
		int64 appID;  //机器人ID
		CMD_SIZE();
	};

	/**
	* \struct resp_setUnReadCount 消息号 logic_cmd_setUnReadCount
	* \brief 设置客户端消息未读数量
	*/
	struct req_setUnReadCount : public _sdkcmd_base
	{
		req_setUnReadCount()
		{

		}
		int8  type;             ///< 0,设置未读数，默认值，客户端可以不用设置该值.
		int32 count;            ///< 未读数量.
		std::string appleID;     ///< 应用id，不同的应用所对应的证书是不一样的.
		std::string token;       ///< ios token.
		CMD_SIZE();
	};

	/**
	 *
	 */

	/**
	* \struct req_setMsgUnRead 消息号： logic_cmd_setMsgUnRead
	* \brief 设置最后一条已读为未读
	*/
	struct req_setMsgUnRead : public _sdkcmd_base
	{
		req_setMsgUnRead() : targetID(0)
		{

		}
		int64 targetID;        ///目标ID,如果targetID等于0表示设置的是系统消息
		CMD_SIZE();
	};

	/**
	* \struct req_updateDecryptedMsg 消息号 logic_cmd_updateDecryptedMsg
	* \brief  更新解密后的消息到数据库
	*/
	struct req_updateDecryptedMsg : public _sdkcmd_base
	{
		int64 targetId;
		int64 msgId;
		std::string message;
		std::string msgProperty;
		CMD_SIZE();
	};

	/**
	 * \struct req_quickRegister 消息 logic_cmd_quickRegister
	 * \brief 快速注册
	 */
	struct req_quickRegister : public _sdkcmd_base
	{
		int8        type ;      ///< 1：电话, 3：邮箱, 4：userID, 5:身份证, 6：豆豆号, 7:自定义帐号(不做限制) .
		std::string name ; 	   ///< 名称/昵称.
		std::string account ;    ///< 账号.
		std::string password ;   ///< 密码.
		/**
 		   只填写 :
		* 1. area
		* 2. usrid 填0
		* 3. version
		* 4. device_info
		*/
		req_LoginParamBean login ; ///< 注册信息.
		
		CMD_SIZE();
	};


	/**
	* \struct req_setPrivateMsgPwd 消息 logic_cmd_setPrivateMsgPwd
	* \brief 设置私信密码
	*/
	struct req_setPrivateMsgPwd : public _sdkcmd_base
	{
		int64 targetId;		    ///< 好友或群ID.
		std::string passward;		///< 私信密码  为空代表清除私信密码.
		CMD_SIZE();
	};

	/**
	* \struct req_getIdByAccount 消息 logic_cmd_getIdByAccount
	* \brief 通过账号名获取用户ID
	*/
	struct req_getIdByAccount : public _sdkcmd_base
	{
		void init()
		{
			accounts.clear();
		}
		std::vector<std::string> accounts;				///< 用户accountName集合.
		CMD_SIZE();
	};

	/**
	* \struct req_decryptMsg 解密消息 logic_cmd_decryptMsg
	*  brief 解密消息并更新到数据库
	*/
	struct req_decryptMsg : public _sdkcmd_base
	{
		int64 targetId;				///< 人或群ID.
		std::vector<int64> msgIds;     ///< 要解密的消息集合.
		void init()
		{
			msgIds.clear();
		}
		CMD_SIZE();
	};
	/**
	* \struct req_transferLocalData 导入导出数据 logic_cmd_transferLocalData
	*  brief 导入导出数据
	*/
	struct req_transferLocalData : public _sdkcmd_base{

		req_transferLocalData() :version("0.0.1"), year(0), mon(0), day(0){};
		std::string filePath;  ///< 要导入或导出文件的存放路径.

		std::string version;
		///导出时选择从那天开始. 
		int year;
		int mon;
		int day;
		int8 type;///< 0:导出;1:导入.
		CMD_SIZE();
	};
	/**
	* \struct req_cmd_upMsgCount 上传消息计数  logic_cmd_upMsgCount
	*  brief 上传消息计数
	*/
	struct req_upMsgCount: public _sdkcmd_base{
		req_upMsgCount() :type(0){};
		int8 type;
		std::string PackName;///<包名
		std::string UserName;///<用户名
		std::string MsgNum;///<消息数
		std::string devtype;///<设备类型
		CMD_SIZE();
	};

	/**
	* \struct req_checkUrlValid 检查URL的是否有效  logic_cmd_checkUrlValid
	*  brief 检查URL的是否有效
	*/
	struct req_checkUrlValid : public _sdkcmd_base
	{
		std::string url;	///< 要检查的url，如果为相对路径，下层会自己拼接为完整路径
		CMD_SIZE();
	};

	/**
	* \struct req_forwardMessage 转发消息  logic_cmd_forwardMessage
	*  brief 转发消息
	*/
	struct req_forwardMessage : public _sdkcmd_base
	{
		int64 fromTargetId;		///< 转发的消息的当前targetId
		int64 toTargetId;		///< 转发的消息的接收targetId
		int64 messageId;    ///< 转发的消息ID
		CMD_SIZE();
	};

	/**
	* \struct req_addRecontact 转发消息  logic_cmd_addRecontact
	*  brief 转发消息
	*/
	struct req_addRecontact : public _sdkcmd_base
	{
		st_reContact recontact;	///< 添加的最近联系人
		CMD_SIZE();
	};
	
	/**
	* \struct req_getAllMsgCount 获取全部消息  logic_cmd_getAllMsgCount
	*  brief 获取全部消息
	*/
	struct req_getAllMsgCount : public _sdkcmd_base
	{
		int64 targetId;  ///< 为0代表获取所有聊天消息，不为0代表获取相应targetId的全部消息
		CMD_SIZE();
	};
	
	
	/**
	* \struct req_getMsgDays 获取消息日期数组  logic_cmd_getMsgDays
	*  brief 获取消息日期数组
	*/
	struct req_getMsgDays : public _sdkcmd_base
	{
		///对象ID
		int64   targetID ;
		///是否最近 0是最近 非零是要查询的年份
		int16    bLately  ; 
		void init() {
			bLately = 0 ;
		} 
		CMD_SIZE();
	};
	/**
	* \struct req_getMsgsFromDay 获取消息日期数组  logic_cmd_getMsgFromDay
	*  brief 获取消息日期数组
	*/
	struct req_getMsgsFromDay : public _sdkcmd_base
	{
		///对象ID
		int64   targetID ;
		///是否最近 0 是最近，非零的话要设置 YMD
		int8    bLately  ; 
		int64   msgID;          ///< 如果msgID为0则从大到小拉取消息
		int32   count;          ///< 拉取的消息数量,如果count为0，则拉取所有的
		void 	init() {
			bLately = 0 ;
		} 
		///查询的天数
		YMD     ymd ;
		CMD_SIZE();
	};	
	/**
	* \struct req_agoraFunc  logic_cmd_getAgoraFunc
	*  brief  获取声网服务功能
	*/
	struct req_agoraFunc : public _sdkcmd_base
	{
		req_agoraFunc() :channelID(0), chatTime(0), type(0){}
		///channelID
		int64   channelID;
		int64   chatTime;/// 通话时间 type = 5时 填
		int64   uuid;///声网id type = 6时 填
		int8    type;///1,认证 2.呼叫 3.接听 4.重新申请subKey,5.outChat,6,updataID
		CMD_SIZE();
	};
}
#endif  //CMD_REQ_DEF_H
