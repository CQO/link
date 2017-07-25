const fs = require('fs')
const path = require('path')

// 导入nodeService库,需要设置为本地nodeService所在路径
const service = require(`../renderer/assets/VIMSDK/${process.platform}/VIMSDK_${process.platform}`)

// 创建一个client对象
/* eslint new-cap: ["error", { "newIsCap": false }] */
const client = new service.client()
const auth = client.authService()
const chats = client.chatService()
const groups = client.groupService()
const contacts = client.contactService()
const users = client.userService()
const file = client.fileService()
const config = client.configService()
const search = client.searchService()

module.exports = {
  init: () => {
    // 启动client需要的初始化信息
    const info = {
      version: '1.5.0',
      deviceType: 2,
      deviceInfo: 'pc-windows',
      appName: 'nodeImsdk',
      netType: 'wifi',
      mac: '12345',
      local: 'zh_CN'
    }
    const dbPath = path.join(__dirname, '../renderer/assets/dataBase/') // 数据库目录
    const crtPath = path.join(__dirname, '../renderer/assets/vimsdk/linkdood.crt')
    // 判断数据路文件夹是否存在
    if (!fs.existsSync(dbPath)) {
      fs.mkdir(dbPath, (e) => {
        console.log(`创建目录${dbPath}`)
      })
    }
    const result = client.startup(dbPath, crtPath, info)
    return result
  },

  // 登录
  login: (user, pwd, userType, server, callback) => {
    auth.login(user, pwd, userType, server, function (res) {
      callback(res)
    })
  },
  // 修改账户信息后推送
  regGetMyselfCb: (callback) => {
    users.regGetMyselfCb(function (res) {
      callback(res)
    })
  },
  // 推送账户信息
  regOperateUserCb: (callback) => {
    users.regOperateUserCb(function (res) {
      callback(res)
    })
  },
  // 获取账户信息
  getAccountInfo: (callback) => {
    users.getAccountInfo(function (res) {
      callback(res)
    })
  },

  // 获取本地配置
  getLocalSetting (localSetting, callback) {
    users.getLocalSetting(localSetting, function (res) {
      callback(res)
    })
  },
  // 增加本地配置
  addLocalSetting: (addConfig, callback) => {
    users.addLocalSetting(addConfig, (resp) => {
      callback(resp)
    })
  },
  // 更新本地配置
  updateLocalSetting: (updateConfig, callback) => {
    users.updateLocalSetting(updateConfig, (resp) => {
      callback(resp)
    })
  },
  // 删除本地配置
  deleteLocalSetting: (deleteConfig, callback) => {
    users.deleteLocalSetting(deleteConfig, (resp) => {
      callback(resp)
    })
  },
  // 删除聊天记录
  deleteAllMessage: (delChatList, callback) => {
    chats.deleteAllMessage(delChatList, (resp) => {
      callback(resp)
    })
  },
  // 获取用户登录记录
  getLoginInfos: () => {
    return config.getLoginInfos()
  },
  // 设置用户登录信息
  setLoginInfo: (userBean) => {
    return config.setLoginInfo(userBean)
  },
  // 获取图片路径
  userImgPath: () => {
    return config.userImgPath()
  },
  // 获取文件路径
  userFilePath: () => {
    return config.userFilePath()
  },
  // 获取个人设置项
  getPersonalData: (callback) => {
    users.getPersonalData((resp) => {
      console.log('获取个人设置项', resp)
      callback(resp)
    })
  },
  // 发送消息
  sendMessage: (targetId, message, property, type, callback) => {
    const msg = {
      targetId: targetId, // 目标的targetId,人或群
      message: message, // 消息体
      msgProperty: property, // 消息属性
      messageType: type // 消息类型  1:html 2:文本，3:音频 4: 位置 5:图片6:文件 7:名片 8:弱提示
    }
    console.log('发送消息:', msg)
    chats.sendMessage(msg, function (resp) {
      callback(resp)
    })
  },
  // 获取联系人列表
  getContactList: (cb) => {
    return contacts.getContactList(cb)
  },
  // 好友资料更新的回调
  regOperateContactCb: (cb) => {
    return contacts.regOperateContactCb(cb)
  },
  // 好友更新的回调
  regContactListCb: (cb) => {
    return contacts.regContactListCb(cb)
  },
  // 获取消息
  getMessages: (targetID, msgBeginID, msgOffset, offsetFlag, cb) => {
    return chats.getMessages(targetID, msgBeginID, msgOffset, offsetFlag, cb)
  },
  // 更新联系人信息（星标&V标&备注
  updateContactInfo: (json, cb) => {
    return contacts.updateContactInfo(json, cb)
  },
  // 获取未读消息
  regRecontactCb: (cb) => {
    return chats.regRecontactCb(cb)
  },
  // 设置消息已读
  setMessageRead: (targetID, msgID, cb) => {
    return chats.setMessageRead(targetID, msgID, cb)
  },
  // 图片上传
  uploadImage: (targetId, srcimg, originBuffer, filename, width, height, encryptkey, callback) => {
    console.log(targetId, srcimg, originBuffer, filename, width, height, encryptkey)
    fs.writeFile(filename, originBuffer)
    file.uploadImage(targetId, encryptkey, filename, srcimg, 0, (res) => {
      console.log(res)
      callback(res)
    }, (resp) => {
      console.log(resp)
      callback(resp)
    })
  },
  // 图片的下载
  downloadImage: (targetId, url, callback) => {
    return file.downloadImage(targetId, url, (resp) => {
      callback(resp)
    })
  },
  // 文件上传
  uploadFile: (fileProperty, callback) => {
    console.log(fileProperty)
    return file.uploadFile(fileProperty, (resp) => {
      console.log('文件信息', resp)
      callback(resp)
    })
  },
  // 文件下载
  downloadFile: (downProp, callback, pro) => {
    return file.downloadFile(downProp, (resp) => {
      callback(resp)
      console.log(resp)
    }, (res) => {
      pro(res)
      console.log(res)
    })
  },
  // 解密文件
  decryptFile: (key, srcPath, destPath) => {
    return file.decryptFile(key, srcPath, destPath)
  },
  // 接收消息
  regMsgNoticeCb: function (cb) {
    return chats.regMsgNoticeCb(cb)
  },
  // 获取群信息
  getGroupInfo: (groupid, cb) => {
    return groups.getGroupInfo(groupid, cb)
  },
  // 获取群成员
  getMemberList: (groupid, cb) => {
    return groups.getMemberList(groupid, cb)
  },
  // 获取会话列表
  getChatList: (cb) => {
    return chats.getChatList(cb)
  },
  // 获取验证码
  getRegCode: (user, userType, server, cb) => {
    return auth.getRegCode(user, userType, server, cb)
  },
  // 注册
  signup: (code, name, pswd, callback) => {
    auth.signup(code, name, pswd, (resp) => {
      callback(resp)
    })
  },
  // 更新用户信息
  updateAccountInfo: (name, email, phone, cb) => {
    const userInfo = {
      name: name,
      email: email,
      phone: phone
    }
    return users.updateAccountInfo(userInfo, cb)
  },
  // 搜索用户
  searchFromNet: (keyWord, cb) => {
    // console.log('搜索:', keyWord)
    return search.searchFromNet(keyWord, cb)
  },
  // 本地搜索
  searchFromLocal: (keyWord, callback) => {
    search.searchFromLocal(keyWord, (resp) => {
      // console.log('搜索:', resp)
      callback(resp)
    })
  },
  // 全局搜索消息
  searchGlobalMessage: (key, msgDetailProperty, callback) => {
    search.searchGlobalMessage(key, msgDetailProperty, (resp) => {
      // console.log('搜索:', resp)
      callback(resp)
    })
  },
  // 搜索详细信息
  searchDetailMessage: (key, msgDetailProperty, callback) => {
    search.searchDetailMessage(key, msgDetailProperty, (resp) => {
      console.log('搜索:', resp)
      callback(resp)
    })
  }
}
