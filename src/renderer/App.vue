<template>
  <div class="app">
    <HeadBar v-if="auth_hasLogin"></HeadBar>
    <router-view class="body"></router-view>
  </div>
</template>

<script>
import { mapGetters } from 'vuex'
import UserDetails from './module/list/UserDetails'
import HeadBar from './HeadBar'
import vim from './ipcVIMSDK'
import { MsgOrder } from './components/Chats/MsgOrder.js'
const notifier = require('node-notifier') // 桌面通知模块
export default {
  data () {
    return {
      hasLogin: vim.getHasLogin()
    }
  },
  components: {
    UserDetails,
    HeadBar
  },
  computed: {
    ...mapGetters([
      'auth_hasLogin',
      'condition_chatWith'
    ])
  },
  created () {
    vim.init(this.$store)
    // 注册消息通知回调，有消息到达，会自动调用传入的回调函数
    vim.regMsgNoticeCb((resp) => {
      console.log('收到新对话推送:', resp)
      const message = JSON.parse(resp.message) // 解析消息内容
      const userData = this.$store.state.chat.ContactList[resp.senderId]
      // 判断是否为好友
      if (userData && resp.targetId !== this.condition_chatWith.userData.id) {
        let showMessage = '发来一条消息'
        // 如果是纯文本消息 桌面通知显示文本内容
        if (resp.messageType === 2) showMessage = message.body
        // 调用通知模块 显示桌面通知
        notifier.notify({
          title: userData.name,
          message: showMessage,
          icon: userData.avatar,
          sound: true,
          reply: true
        })
      }
      MsgOrder.$emit('newMessage', resp)
    })
    // 注册 账户信息推送
    vim.regGetMyselfCb((resp) => {
      console.log('账户信息推送:', resp)
      this.$store.commit('SET_ACCOUNT', resp)
    })
    // 注册 最近联系人推送推送
    vim.regRecontactCb((resp) => {
      console.log('收到最近联系人推送:', resp)
      this.$store.commit('SET_CHATLIST', resp)
    })
    vim.regContactListCb((resp) => {
      console.log('收到联系人推送', resp)
      let contacts = {}
      resp.contacts.forEach((element) => {
        contacts[element.userId] = element
      })
      this.$store.commit('SET_CONTACTLIST', contacts)
    })
  }
}
</script>

<style>
/*全局样式*/

body,h1,h2,h3,h4,h5,h6,hr,p,blockquote,dl,dt,dd,ul,ol,li,pre,form,fieldset,legend,button,input,textarea,th,td {
  margin: 0;padding: 0
}

body,html,.app {
  width: 100%;
  height: 100%;
  font-size: 16px;
  overflow: hidden;
  font-family: Helvetica, Tahoma, Arial, STXihei, “华文细黑”, “Microsoft YaHei”, “微软雅黑”, sans-serif;
}

body,button,input,select,textarea {
  font: 12px arial
}

.body {
  cursor: default;
  height: calc(100% - 40px);
}

h1,
h2,
h3,
h4,
h5,
h6 {
  font-size: 100%
}

a {
  text-decoration: none
}

a:hover {
  text-decoration: underline
}

ul,
ol {
  list-style: none
}

img {
  border: 0
}

table {
  border-collapse: collapse;
  border-spacing: 0
}

/*滚动条整体部分*/

::-webkit-scrollbar {
  width: 7px;
  height: 7px;
  border-radius: 4px;
  background-color: #f0f0f0;
}

::-webkit-scrollbar-button {
  display: none;
}


/*滚动条两端的箭头*/

::-webkit-scroll-track {
  display: none;
}


/*经测试好像并不能控制什么*/

::-webkit-scrollbar-track-piece {
  display: none;
}


/*滚动条内侧部分 去掉*/

::-webkit-scrollbar-thumb {
  background-color: skyblue;
  border-radius: 4px;
}


/*滚动条中可以拖动的那部分*/

::-webkit-scrollbar-corner {
  display: none;
}


/*变角部分*/

::-webkit-resizer {
  display: none;
}

@font-face {
  font-family: 'iconfont';
  /* project id 307699 */
  src: url('//at.alicdn.com/t/font_b7t6uwb9ssda38fr.eot');
  src: url('//at.alicdn.com/t/font_b7t6uwb9ssda38fr.eot?#iefix') format('embedded-opentype'),
  url('//at.alicdn.com/t/font_b7t6uwb9ssda38fr.woff') format('woff'),
  url('//at.alicdn.com/t/font_b7t6uwb9ssda38fr.ttf') format('truetype'),
  url('//at.alicdn.com/t/font_b7t6uwb9ssda38fr.svg#iconfont') format('svg');
}

.icon {
  font-family: "iconfont";
  font-style: normal;
  font-weight: normal;
  display: inline-block;
}
</style>
