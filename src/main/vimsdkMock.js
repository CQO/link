// let msgNoticeCb
// let recontactCb

module.exports = {
  init: () => {
    // msgNoticeCb = msgNoticeCallback
    // recontactCb = recontactCallback
    console.log('!MOCK init')
    return 'OK'
  },

  login: (user, pwd, userType, server, callback) => {
    callback({
      code: 0,
      userId: 0
    })
    recontactCb([])
  },

  sendMessage (targetId, msg, msgProperty, isBurn, messageType, callback) {
    callback({
      code: 0
    })
    msgNoticeCb(msg)
  },

  logout (callback) {
    callback({
      code: 0
    })
  },

  getContactList (callback) {
    callback({
      code: 0,
      contacts: [{
        id: 1,
        gender: 1,
        timeZone: 8,
        isStar: 1,
        isApp: 1,
        isHidden: 2,
        isTop: 0,
        name: 'lindood',
        avatar: require('../app/assets/images/t1.png'),
        thumbAvatar: '/2.png',
        avatarUrl: '/0.png',
        extends: '',
        pinyin: 'pinyin',
        acountName: 'acountName',
        remark: 'remark',
        server: 'server',
        birthday: 12837715647512,
        chatImg: 'chatImg',
        sign: 'build.biu.biu.',
        area: 'cn',
        privateMsgPwd: '',
        emails: ['link@vrvmail.com.cn', 'dood@vrvmail.com.cn'],
        phones: ['18101009888', '13990100011']
      }, {
        id: 2,
        gender: 2,
        timeZone: 8,
        isStar: 1,
        isApp: 1,
        isHidden: 2,
        isTop: 0,
        name: 'lindood',
        avatar: require('../app/assets/images/t1.png'),
        thumbAvatar: '/2.png',
        avatarUrl: '/0.png',
        extends: '',
        pinyin: 'pinyin',
        acountName: 'acountName',
        remark: 'remark',
        server: 'server',
        birthday: 12837715647512,
        chatImg: 'chatImg',
        sign: 'build.biu.biu.',
        area: 'cn',
        privateMsgPwd: '',
        emails: ['link@vrvmail.com.cn', 'dood@vrvmail.com.cn'],
        phones: ['18101009888', '13990100011']
      }]
    })
  },

  getAccountInfo: (userName, email, phone, callback) => {
    callback({
      code: 0,
      account: {
        id: '0',
        Name: userName,
        email: email,
        header: 'http://myweb-10017157.cossh.myqcloud.com/2017/0610/20140524012042770.gif',
        nickId: 0,
        phone: phone,
        sign: 'hahahah',
        sex: '0',
        birthday: '1992-05-03',
        address: '陕西省西安市雁塔区',
        department: '西安研发',
        job: '职位1'
      }
    })
  }
}
