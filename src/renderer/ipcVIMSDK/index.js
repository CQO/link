const path = require('path')
const sdkPath = path.resolve(__dirname, '../../main/vimsdk.js')
const ipcMain = require('electron').remote.require(sdkPath)
const nativeImage = require('electron').nativeImage

let store
let hasLogin = false
export default {
  init: function (vueStore) {
    store = vueStore
    ipcMain.init()
    return 'vim.init OK'
  },
  login: function (user, pwd, userType, server, callback) {
    ipcMain.login(user, pwd, userType, server, (resp) => {
      hasLogin = (resp.code === 0)
      store.commit('SET_HASLOGIN', hasLogin)
      let msgDetailProperty = {
        'targetId': 4328635930,
        'messageType': 2
      }
      if (hasLogin) {
        ipcMain.searchDetailMessage('1', msgDetailProperty, resp => {
          console.log('搜索详情', resp)
        })
        // const localSetting = {
        //   name: '008618392395339',
        //   pswd: 'zy920503',
        //   autoLogin: true
        // }
        // const obj = {
        //   'key': 'config',
        //   'value': JSON.stringify(localSetting)
        // }
        // console.log('obj---->：', obj)
        // ipcMain.addLocalSetting([obj], (resp) => {
        //   console.log('添加本地配置：', resp)
        //   ipcMain.getLocalSetting(['config'], (resp) => {
        //     console.log('获取本地配置：', resp)
        //   })
        // })
        // ipcMain.deleteLocalSetting(['config', ''], (resp) => {
        //   console.log('删除本地配置：', resp)
        //   ipcMain.getLocalSetting(['config'], (resp) => {
        //     console.log('获取本地配置：', resp)
        //   })
        // })
        // 获取图片路径
        const result = ipcMain.userImgPath()
        console.log('获取图片路径', result)
        store.commit('SET_IMAGEPATH', result)
        // 获取文件存放路径
        const filePath = ipcMain.userFilePath()
        console.log('获取文件路径', filePath)
        store.commit('SET_UserFilePath', filePath)
        // store.commit('SET_IMAGEPATH', result)
        // 获取个人设置项
      }
      console.log('>>>>>>>>>>>code:' + resp.code + ',haslogin:' + hasLogin)
      callback(resp)
    })
  },
  getHasLogin: function () {
    return hasLogin
  },
  // 注册新通知推送
  regMsgNoticeCb: ipcMain.regMsgNoticeCb,
  // 注册最近联系人回掉
  regRecontactCb: ipcMain.regRecontactCb,
  // 推送账户信息
  regOperateUserCb: ipcMain.regOperateUserCb,
  // 好友更新推送
  regContactListCb: ipcMain.regContactListCb,
  // 修改账户信息后推送
  regGetMyselfCb: ipcMain.regGetMyselfCb,
  // 发送消息
  sendMessage: (targetId, message, property, type, filePath, callback) => {
    ipcMain.sendMessage(targetId, message, property, type, callback)
  },
  // 获取联系人列表
  getContactList: ipcMain.getContactList,
  // 联系人更新回掉
  regOperateContactCb: ipcMain.regOperateContactCb,
  // 获取账户信息
  getAccountInfo: ipcMain.getAccountInfo,
  // 设置消息已读
  setMessageRead: ipcMain.setMessageRead,
  // 获取群信息
  getGroupInfo: ipcMain.getGroupInfo,
  // 获取群成员
  getMemberList: ipcMain.getMemberList,
  // 获取对话列表
  getChatList: ipcMain.getChatList,
  // 获取注册验证码
  getRegCode: ipcMain.getRegCode,
  // 注册
  signup: ipcMain.signup,
  // 获取本地配置
  getLocalSetting: ipcMain.getLocalSetting,
  // 增加本地配置
  addLocalSetting: ipcMain.addLocalSetting,
  // 更新本地配置
  updateLocalSetting: ipcMain.updateLocalSetting,
  // 更新用户信息
  updateAccountInfo: ipcMain.updateAccountInfo,
  // 获取登陆记录
  getLoginInfos: ipcMain.getLoginInfos,
  // 设置登录记录
  setLoginInfo: ipcMain.setLoginInfo,
  // 删除消息记录
  deleteAllMessage: ipcMain.deleteAllMessage,
  // 获取用户图片信息路径
  userImgPath: ipcMain.userImgPath,
  // 更新联系人信息（星标&V标&备注
  updateContactInfo: ipcMain.updateContactInfo,
  // 搜索用户
  searchFromNet: ipcMain.searchFromNet,
  // 本地搜索
  searchFromLocal: ipcMain.searchFromLocal,
  // 全局搜索消息
  searchGlobalMessage: (key, msgDetailProperty, callback) => {
    ipcMain.searchGlobalMessage(key, msgDetailProperty, (resp) => {
      console.log('搜索', resp)
      callback(resp)
    })
  },
  // 搜索详细信息
  searchDetailMessage: (key, msgDetailProperty, callback) => {
    ipcMain.searchDetailMessage(key, msgDetailProperty, (resp) => {
      console.log('搜索:', resp)
      callback(resp)
    })
  },
  // 获取历史消息
  getMessages: ipcMain.getMessages,
  // 压缩图片
  thumbImage: (srcImg) => {
    var image = nativeImage.createFromPath(srcImg)
    let originBuffer = image.toJPEG(60)
    let images = {
      originBuffer: originBuffer,
      width: image.getSize().width,
      height: image.getSize().height
    }
    return images
  },
  // 上传图片
  uploadImage: ipcMain.uploadImage,
  // 图片下载
  downloadImage: ipcMain.downloadImage,
  // 上传文件
  uploadFile: ipcMain.uploadFile,
  // 下载文件
  downloadFile: ipcMain.downloadFile,
  // 解密文件
  decryptFile: ipcMain.decryptFile
}
