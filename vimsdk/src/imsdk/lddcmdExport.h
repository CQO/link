/*
 * lddcmdExport.h
 *
 *  Created on: 2016年4月25日
 *      Author: corning
 */

#ifndef LDDCMDEXPORT_H_
#define LDDCMDEXPORT_H_

#include <cmdReqDef.h>
#include <cmdComDef.h>
#include <string>
#include <vector>
#include <config.h>

using namespace imsdk;

namespace imsdkldd{

/**
 * 导出业务类消息号，各个平台的ＡＰＩ层要统一
 * ** [in] 标识输入参数
 * ** [out] 标识返回参数
 * ** 只有[out]没有[in] 标识该消息ＳＤＫ纯推送，只有reponse没有request
 */
enum {

    /// [in] req_topTaskMsg              [out] resp_commonResult
    ldd_logic_cmd_topTaskMsg = 11001,         /// 任务置顶

    /// [in] _sdkcmd_base                [out] resp_taskMsgCom
    ldd_logic_cmd_getSendTaskMsgs = 11002,    /// 获取指派的任务

    /// [in] _sdkcmd_base                [out] resp_taskMsgCom
    ldd_logic_cmd_getReceiveTaskMsgs = 11003, /// 获取接收的任务

    /// [in] _sdkcmd_base                [out] resp_taskMsgCom
    ldd_logic_cmd_getHistoryTaskMsgs = 11004, /// 获取历史任务

    /// [in] req_taskMsgCom              [out] resp_commonResult(value_str字段标记任务内容)
    ldd_logic_cmd_getBodyTaskMsg = 11005,     /// 获取任务上下文

    /// [in] req_taskMsgCom              [out] resp_commonResult
    ldd_logic_cmd_reCoveryTaskMsg = 11006,    /// 恢复任务

    /// [in] req_taskMsgCom              [out] resp_commonResult
    ldd_logic_cmd_finishTaskMsg = 11007,      /// 完成任务


    /// [in] req_createRoom              [out] resp_commonResult(value_i32字段标记rommID)
    ldd_logic_cmd_createRoom = 11008,         /// 创建房间

    /// [in] req_opRoom                  [out] resp_commonResult
    ldd_logic_cmd_opRoom = 11009,             /// 修改房间

    /// [in] req_roomCom                 [out] resp_commonResult
    ldd_logic_cmd_delRoom = 11010,            /// 删除房间

    /// [in] req_roomCom                 [out] resp_getAllRooms
    ldd_logic_cmd_getRoom = 11011,            /// 获取一个房间

    /// [in] NULL                        [out] resp_getAllRooms
    ldd_logic_cmd_getAllRooms = 11012,        /// 获取所有房间

    /// [in] req_topRoom                 [out] resp_commonResult
    ldd_logic_cmd_topRoom = 11013,            /// 房间置顶


	/// [in] NULL						[out] resp_getEnterpriseListByUser
    ldd_logic_cmd_getEnterpriseListByUser = 11014,  /// 获取一个用户拥有的企业列表

	/// [in] req_getVisibleOrgUsers		[out] resp_getOrgUserAndChildNodes
    ldd_logic_cmd_getVisibleOrgUsers = 11015,       /// 获取某个用户所在组织下可见的子组织和用户

    /// [in] req_queryEnterpriseUserlist	[out] resp_queryEnterpriseUserlist
    ldd_logic_cmd_queryEnterpriseUserlist = 11016,  /// 根据条件查询企业用户列表


	/// [in] req_getMsgCount 			[out] resp_commonResult
	ldd_cmd_getMsgCount = 11017,			  /// 根据targetId获取消息数量
	
	/// [in] req_getMsgCountTopN 		[out] resp_getMsgCountTopN
	ldd_cmd_getMsgCountTopN = 11018,		  /// 获取聊天数目最多的topN或@自己最多的topN


	/// [in] req_AddNote 				[out] resp_commonResult
	ldd_logic_cmd_AddNote = 11019, 			  /// 添加记事本
	
	/// [in] req_getNotes 				[out] resp_getNotes 
	ldd_logic_cmd_getNotes = 11020 ,		  /// 记事本查询
	
	/// [in] req_delNotes 				[out] resp_commonResult
	ldd_logic_cmd_delNotes = 11021, 		  /// 删除记事本
	
	/// [in] req_editNotes 				[out] resp_commonResult
	ldd_logic_cmd_editNotes = 11022,		  /// 修改记事本

	/// [in] req_archiveNotes			[out] resp_commonResult
	ldd_logic_cmd_archiveNotes = 11023,       /// 归档


    /// [in] req_qEnUS	                [out] resp_qEnUS
    ldd_logic_cmd_qEnUS = 11024,              /// 查询组织机构的用户

    /// [in] req_getOrgInfo	            [out] resp_getOrgInfo
    ldd_logic_cmd_getOrgInfo = 11025,         /// 获取组织的信息


    /// [in] req_archiveNotes            [out] resp_commonResult
    ldd_logic_cmd_topNote = 11026,            /// 记事本置顶

    /// [in] req_searchNotes             [out] resp_getNotes
    ldd_logic_cmd_searchNote = 11027,         /// 记事本搜索


    /// [in] req_taskMsgCom              [out] resp_commonResult
    ldd_logic_cmd_readTaskMsg = 11028,        /// 设置任务已读

    /// [in] req_getReceiveMsg           [out] resp_getReceiveMsg
    ldd_logic_cmd_getReceiveMsg = 11029,      /// 获取任务回复消息

    /// [in] req_updataTask              [out] resp_commonResult
    ldd_logic_cmd_updataTask = 11030,         /// 修改任务属性，包括 1：是否完成 2：是否标记 3：是否已读 4：是否置顶

    /// [in] req_taskMsgCom              [out] resp_commonResult
    ldd_logic_cmd_delTaskMsg = 11031,         /// 删除任务


	ldd_logic_cmd_max	                      /// 最大的命令标识，标识命令个数，无特殊业务意义
};

/// =========================================================================================

/// 任务属性
struct st_msgProperties {
    int32 timeZone;             ///< 时区
    std::string timeTask;       ///< 任务时间
    std::string isFinish;       ///< 是否完成     0：未完成 1：完成
    std::string isTask;         ///< 任务标记     0：非任务 1：任务
    std::string isRead;         ///< 是否已读     0：未读   1：已读
};

/// task
struct st_task {
    st_task() {
        fromeUserID = 0;
        msgID = 0;
        top = 0;
        targetID = 0;
    }

    int64 fromeUserID;                  ///< 自己发送的这个字段填0
    int64 sendTime;                     ///< 发送时间
    std::string body;                   ///< 消息体
    int64 msgID;                        ///< 任务ID
    st_msgProperties msgProperties;
    std::string relatedUsers;           ///< 如果是针对群全体成员，就是群ID，如果针对部分人，就是人的ID数组
    std::vector<int64> vt_relatedUsers; ///< 返回的时候用
    int8 top;                           ///< 是否置顶   0：不置顶 1：置顶
    int64 targetID; 		            ///< 接受者，可以为群或者人
};

/// room
struct st_room {
    st_room() {
        roomID = 0;
        top = 0;
    }

    int roomID;                         ///< 房间ID
    std::string roomName;               ///< 房间名
    std::string roomMember;             ///< 房间成员
    std::vector<int64> vt_roomMember;   ///< 返回的时候用
    int8 top;                           ///< 是否置顶   0：不置顶 1：置顶
    std::string portraitURL;            ///< 头像
};

/// note
struct st_baseNoteinfo
{
    st_baseNoteinfo() :id(0), lastChgTime(0), sendUserId(0), targetId(0), type(0), isReminder(0), rmdTime(0), isArchive(0) {
        INIT_ISSET(lastChgTime)
            INIT_ISSET(sendUserId)
            INIT_ISSET(targetId)
            INIT_ISSET(type)
            INIT_ISSET(content)
            INIT_ISSET(title)
            INIT_ISSET(key)
            INIT_ISSET(isReminder)
            INIT_ISSET(rmdTime)
            INIT_ISSET(isArchive)
    }

    int64 id;						///< 记事本唯一标识
    IS_SET(int64, lastChgTime) 		///< 最后修改时间,毫秒
    IS_SET(int64, sendUserId)		///< 发送的人
    IS_SET(int64, targetId)		    ///< 来源 人或者群
    /**
     * 低4位 类型 1. 文本，2，图片，3，音频，4，视频，5，URL
     * 高4位 0. 本地， 1. 网络
     */
    IS_SET(int8, type)
    /**
     * 根据文件类型
     * 1. 文本、URL 直接存储内容。
     * 2. 非文本类型，存储附件地址，网络地址和本地地址均可。
     */
    IS_SET(std::string, content)	///< 内容,最长4096个字符	
    IS_SET(std::string, title)		///< 标题,最长128字符
    IS_SET(std::string, key)		///< 关键字(标签),检索用 最长128字符 ,可以为空
    IS_SET(int8, isReminder)		///< 是否是提醒
    IS_SET(int64, rmdTime)			///< 提醒时间
    IS_SET(int8, isArchive)			///< 是否归档
};

/// 企业信息
struct st_entpriseInfo
{
	st_entpriseInfo() :
	    enterpriseID(0),
	    sector(0),
	    sectorDetail(0),
	    type(0),
	    provinceID(0),
	    cityID(0),
	    areaID(0),
	    status(0),
	    creationDate(0),
	    updatedDate(0){}

	int64 enterpriseID;         ///< 企业ID enterpriseID.
	std::string fullName;       ///< 企业全称 fullName.
	std::string shortName;      ///< 企业简称 shortName.
	std::string keyWords;       ///< 企业关键字 keyWords.
	int64 sector;               ///< 企业所属行业 sector.
	int64 sectorDetail;         ///< 企业所属行业的具体哪一类 sectorDetail.
	int8 type;                  ///< 企业类型 type.
	int64 provinceID;           ///< 企业所属省份 provinceID.
	int64 cityID;               ///< 企业所属市 cityID.
	int64 areaID;               ///< 企业所属地区 areaID.
	std::string address;        ///< 企业地址 address.
	std::string postcode;       ///< 企业邮编 postcode.
	std::string phone;          ///< 企业电话 phone.
	std::string fax;            ///< 企业传真 fax.
	std::string email;          ///< 企业邮箱 email.
	std::string homepage;       ///< 企业主业 homepage.
	std::string introduction;   ///< 企业简介 introduction.
	std::string logUrl;         ///< 企业logURL地址 logUrl.
	int8 status;                ///< 企业状态：1:审核通过；2：未审核；3：审核未通过 status.
	int64 creationDate;         ///< 注册时间 creationDate.
	int64 updatedDate;          ///< 修改时间 updatedDate.
};

/// 组织信息
struct st_organizationInfo
{
	st_organizationInfo() :
	    orgID(0),
	    enterpriseID(0),
	    leaf(0),
	    parentOrgID(0),
	    orderNum(0),
	    branchID(0),
	    departLevel(0),
	    subOrgNum(0),
	    subUserNum(0),
	    isHidden(0),
	    provinceID(0),
	    cityID(0),
	    areaID(0) {}

	int64 orgID;                ///< 组织机构ID orgID.
	int64 enterpriseID;         ///< 企业ID enterpriseID.
	std::string orgName;        ///< 组织机构名称 orgName.
	std::string orgCode;        ///< 组织机构内码 orgCode.
	int8 leaf;                  ///< 是否叶节点 leaf.
	int64 parentOrgID;          ///< 父节点 parentOrgID.
	std::string remark;         ///< 备注 remark.
	int64 orderNum;             ///< 排序 orderNum.
	int64 branchID;             ///< 办公地ID branchID.
	std::string departID;       ///< 删除 departID.
	int8 departLevel;           ///< 组织级别 departLevel.
	int64 subOrgNum;            ///< 递归计算所有下级组织数 subOrgNum.
	int64 subUserNum;           ///< 递归计算所有下级成员数 subUserNum.
	int8 isHidden;              ///< 是否隐藏, 1对本部门公开 2对所有部门公开，3对部分部门公开 isHidden.
	int64 provinceID;           ///< 省份ID provinceID.
	std::string provinceName;   ///< 省份名称 provinceName.
	int64 cityID;               ///< 市ID cityID.
	std::string cityName;       ///< 市名称 cityName.
	int64 areaID;               ///< 县ID areaID.
	std::string areaName;       ///< 县名称 areaName.
   std::string orgNamePinyin;   ///< 拼音 orgNamePinyin.
};

/// 企业成员信息
struct st_entpriseUserInfo
{
	st_entpriseUserInfo() :
	    userID(0),
	    enterpriseID(0),
	    orgID(0),
	    roleID(0),
	    status(0),
	    orderNum(0),
	    newOrgID(0){}

	int64 userID;               ///< 用户ID userID.
	int64 enterpriseID;         ///< 企业ID enterpriseID.
	int64 orgID;                ///< 组织机构ID orgID.
	int64 roleID;               ///< 角色ID roleID.
	std::string enName;         ///< 企业内用户名称 enName.
	std::string duty;           ///< 企业内用户职位 duty.
	std::string enMail;         ///< 企业内用户email enMail.
	std::string enMobile;       ///< 企业内用户手机号 enMobile.
	int8 status;                ///< 企业账号是否被停用 1：正常 2：停用 status.
	int64 orderNum;             ///< 人员排序 orderNum.
	int64 newOrgID;             ///< 用户修改组织机构使用，表示用户新组织机构，在修改时给用户增加组织机构时，使用newOgrID而不是orgID newOrgID.
	std::string orgName;        ///< 组织名 orgName.
	std::string extend;         ///< 扩展信息 extend.
	std::string accountName;    ///< 账号名称 accountName.
	std::string enNamePy;       ///< 拼音 enNamePy.
    int8  bizStatus;            ///< 业务状态 业务状态：1：初始状态，2：在高管群
    std::string userHead;       ///< 头像
};

/// 团队用户查询参数
struct st_enterpriseUserQueryInfo
{
    st_enterpriseUserQueryInfo() :
        entID(0),
        entOrgID(0),
        pageNo(0),
        pageSize(100),
        entUserID(0),
        bizStatus(0),
        status(0) {}

    std::string username; 	    ///< 用户名称 username.
    int64 entID; 			    ///< 企业ID entID.
    int64 entOrgID; 		    ///< 组织架构ID entOrgID.
    int32 pageNo; 			    ///< 分页页码 pageNo.
    int32 pageSize; 		    ///< 分页每页大小 pageSize.
    int64 entUserID; 		    ///< 企业用户ID.
    int8 bizStatus;             ///< 业务状态，1：默认，2：在高管层.
    int8 status;                ///< 企业用户状态.
};

/// 团队用户查询返回
struct st_enUserStruct
{
    st_enUserStruct() :
        total(0),
        totalPage(0){}

    int64 total;
    int64 totalPage;
    std::vector<st_entpriseUserInfo> vtEnter;
};

/// ===============================================================================================

/// task
struct req_taskMsgCom : public _sdkcmd_base
{
    int64 msgID;
    CMD_SIZE();
};

struct resp_taskMsgCom : public _sdkrespcmd_base {
    std::vector<st_task> vtTaskMsgs;
    CMD_SIZE();
    COPY_OTHER(resp_taskMsgCom);
};

/// ldd_logic_cmd_topTaskMsg
struct req_topTaskMsg : public _sdkcmd_base
{
    int64 msgID;
    int8 top;               ///< 是否置顶   0：不置顶 1：置顶
    CMD_SIZE();
};

/// ldd_logic_cmd_getReceiveMsg
struct req_getReceiveMsg : public _sdkcmd_base
{
    req_getReceiveMsg() {
        type = 0;
        msgID = 0;
        count = 30;
    }

    int8 type;              ///< 1:根据msgID获取; 2:根据timeTask获取
    int64 msgID;            ///< type = 1时赋值
    std::string timeTask;   ///< type = 2时赋值
    int count;              ///< 最多获取的数量(默认值为30)
    CMD_SIZE();
};

struct resp_getReceiveMsg : public _sdkrespcmd_base {
    std::vector<st_task> vtMainTasks;               ///< 查询的任务
    std::vector<imsdk::st_msgBean> vtReceiveMsgs;   ///< 任务的回复
    CMD_SIZE();
    COPY_OTHER(resp_getReceiveMsg);
};


/// ldd_logic_cmd_updataTask
struct req_updataTask : public _sdkcmd_base
{
    req_updataTask() {
        isSetFinish = false;
        isSetTask = false;
        isSetRead = false;
        isSetTop = false;
        msgID = 0;
        isTop = 0;
    }

    int64 msgID;

    bool isSetFinish;       ///< 标记是否设置isFinish字段
    std::string isFinish;   ///< 是否完成     0：未完成 1：完成

    bool isSetTask;         ///< 标记是否设置isTask字段
    std::string isTask;     ///< 任务标记     0：非任务 1：任务

    bool isSetRead;         ///< 标记是否设置isRead字段
    std::string isRead;     ///< 是否已读     0：未读 1：已读

    bool isSetTop;          ///< 标记是否设置isTop字段
    int8 isTop;             ///< 是否置顶   0：不置顶 1：置顶

    CMD_SIZE();
};

/// room
struct req_roomCom : public _sdkcmd_base
{
    int roomID;             ///< 房间ID
    CMD_SIZE();
};

/// ldd_logic_cmd_createRoom
struct req_createRoom : public _sdkcmd_base
{
    std::string roomName;   ///< 房间名
    std::vector<int64> vt;  ///< 成员集合
    int8 top;               ///< 是否置顶   0：不置顶 1：置顶
    std::string portraitURL;///< 头像
    CMD_SIZE();
};

/// ldd_logic_cmd_opRoom
struct req_opRoom : public _sdkcmd_base
{
    req_opRoom() {
        isSetroomName = false;
        isSetportraitURL = false;
        isSetmember = false;
    }
    int roomID;             ///< 房间ID
    std::string roomName;   ///< 名字（修改了传值，没修改不传值）
    bool isSetroomName;     ///< 修改名字置为true，没有修改置为false
    std::string portraitURL;///< 头像（修改了传值，没修改不传值）
    bool isSetportraitURL;  ///< 修改头像置为true，没有修改置为false
    std::vector<int64> vt;  ///< 成员集合(有新增的或者删除的就传值，保留最终人员，没有就不传)
    bool isSetmember;       ///< vt有值时设置为true,没有值时为false
    CMD_SIZE();
};

/// ldd_logic_cmd_topRoom
struct req_topRoom : public _sdkcmd_base
{
    int roomID;		        ///< 房间ID
    int8 top;               ///< 是否置顶   0：不置顶 1：置顶
    CMD_SIZE();
};

/// ldd_logic_cmd_getAllRooms
struct resp_getAllRooms : public _sdkrespcmd_base {
    std::vector<st_room> vt;
    CMD_SIZE();
    COPY_OTHER(resp_getAllRooms);
};

/// ldd_logic_cmd_getEnterpriseListByUser
struct resp_getEnterpriseListByUser : public _sdkrespcmd_base {
	std::vector<st_entpriseInfo> vtEntpriseInfo;
	CMD_SIZE();
    COPY_OTHER(resp_getEnterpriseListByUser);
};

/// ldd_logic_cmd_getVisibleOrgUsers
struct req_getVisibleOrgUsers : public _sdkcmd_base
{
    req_getVisibleOrgUsers() {
        type = 0;
    }

    /**
     * 0:获取登陆时刻的组织机构，如果登陆后对后台组织机构进行了修改，本次登陆拉取不到，下次登陆才可以拉取到
     * 1:获取当前时刻的组织机构
     * =1 比 =0 速度上慢了很多，建议使用 =0
     */
    int8 type;        
    int64 enterpriseID;
	int64 orgID;            ///< 传 0 表示获取根组织
	CMD_SIZE();
};

struct resp_getOrgUserAndChildNodes : public _sdkrespcmd_base
{
	std::vector<st_organizationInfo> vtOrgInfo;
	std::vector<st_entpriseUserInfo> vtEntUser;
	CMD_SIZE();
    COPY_OTHER(resp_getOrgUserAndChildNodes);
};

/// ldd_logic_cmd_queryEnterpriseUserlist
struct req_queryEnterpriseUserlist : public _sdkcmd_base
{
    st_enterpriseUserQueryInfo enterpriseUserQueryInfo;
    CMD_SIZE();
};

struct resp_queryEnterpriseUserlist : public _sdkrespcmd_base
{
    st_enUserStruct enUserStruct;
    CMD_SIZE();
    COPY_OTHER(resp_queryEnterpriseUserlist);
};

/// ldd_logic_cmd_qEnUS
struct req_qEnUS : public _sdkcmd_base
{
    int8  type;             ///< 0:获取orgID下面的所有用户信息; 1:orgID输入userID,获取这个用户信息; 2:输入用户名称，获取用户信息
    int64 orgID;            ///< type=0时为组织ID, type=1时为用户ID,type=2时不用赋值
    int64 entID;            ///< 企业ID，不用赋值
    std::string userName;   ///< type=0 或 1 时，不用传值，type=2时，传用户名称
    CMD_SIZE();
};

struct resp_qEnUS : public _sdkrespcmd_base
{
    std::vector<st_entpriseUserInfo> vtEntUser;
    CMD_SIZE();
    COPY_OTHER(resp_qEnUS);
};

/// ldd_logic_cmd_getOrgInfo
struct req_getOrgInfo : public _sdkcmd_base
{
    int64 orgID;      ///< 组织ID orgID.
    CMD_SIZE();
};

struct resp_getOrgInfo : public _sdkrespcmd_base
{
    st_organizationInfo orgInfo;
    CMD_SIZE();
    COPY_OTHER(resp_getOrgInfo);
};

/// ldd_cmd_getMsgCount
struct req_getMsgCount : public _sdkcmd_base
{
	req_getMsgCount() :targetId(0){}
	int64 targetId;		///< 个人或群Id
	CMD_SIZE();
};

/// ldd_cmd_getMsgCountTopN
struct req_getMsgCountTopN : public _sdkcmd_base
{
	req_getMsgCountTopN() :type(1){}
	int8 type;			///< 1.获取topN群. 2.获取topN单聊 3.获取@自己最多的topN群
	int topN;			///< 获取的数量
	CMD_SIZE();
};

struct resp_getMsgCountTopN : public _sdkrespcmd_base
{
	std::vector<int64> targetIds;		///< 返回群或个人的ID集合
	std::vector<int> counts;			///< 对应每个targetId的数目
	CMD_SIZE();
    COPY_OTHER(resp_getMsgCountTopN);
};

/// ldd_logic_cmd_AddNote
struct req_AddNote :  public _sdkcmd_base
{
    /// 添加的时候，本地附件填写全路径
	st_baseNoteinfo info ;
	CMD_SIZE();
};

/// ldd_logic_cmd_getNotes
struct req_getNotes :  public _sdkcmd_base {
	 int64   beginID ;      ///< 起始消息ID offsetFlag = 0 beginID = 0时，代表从最大的消息Id进行查找
	 int32   offset ;       ///< 查询的数量(赋值为0代表查询全部)
	 int8    offsetFlag ;   ///< 偏移标志；0.消息Id由大到小偏移 1.消息Id由小到大偏移 offsetFlag.
	 CMD_SIZE();
};

struct resp_getNotes : public _sdkrespcmd_base {
	std::vector<st_baseNoteinfo>  _vt ;
	CMD_SIZE();	
    COPY_OTHER(resp_getNotes);
};

/// ldd_logic_cmd_delNotes 
struct req_delNotes : public _sdkcmd_base {
	std::vector<int64> _vt;    ///< 删除ID列表
	CMD_SIZE();
};

/// ldd_logic_cmd_editNotes 
struct req_editNotes : public _sdkcmd_base {
	/**
	 *  附件记事本只能修改标题，关键字（标签）,
	 *  文本类可以修改内容
	 *  归档字段并不能修改，需要使用归档接口
	 */
	st_baseNoteinfo  info;
	CMD_SIZE();
};

/// ldd_logic_cmd_archiveNotes
/// ldd_logic_cmd_topNote
struct req_archiveNotes : public _sdkcmd_base {
	/**
	 * 只能修改归档字段
	 */
	int64   id;         ///< 进行 归档 或 置顶 的note id
    /**
     *  低4位：表示是否归档 0. 不归档，1. 归档
     *  高4位：表示是否置顶 0. 不置顶，1. 置顶
     */
	int8    isArchive;  ///< 是否归档 或 置顶
	CMD_SIZE();
};

/// ldd_logic_cmd_searchNote 
struct req_searchNotes : public _sdkcmd_base {
    req_searchNotes() :
        beginID(0),
        offset(0),
        offsetFlag(0),
        type(0),
        is_all(0), 
        is_archive(0) {}

    /**
     * 下面3个字代表查找范围
     */
    int64 beginID;      ///< 起始消息ID, offsetFlag = 0 beginID = 0时，代表从最大的消息Id进行查找
    int32 offset;       ///< 查询的数量(赋值为0代表查询全部)
    int8  offsetFlag;   ///< 偏移标志；0.消息Id由大到小偏移 1.消息Id由小到大偏移 offsetFlag.

    int8 type;          ///< 1:只需给is_all赋值; 2:只需给is_archive赋值; 3:只需给key赋值;
    /**
     * 下面3个字段每次只能给一个字段赋值，剩余2个字段不用赋值
     */
    int8 is_all;        ///< 1:获取全部
    int8 is_archive;    ///< 1:获取未归档 2:获取已经归档
    std::string key;    ///< 根据关键词查找
    CMD_SIZE();
};

}

#endif