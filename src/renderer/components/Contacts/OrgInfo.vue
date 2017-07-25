<template>
  <!--组织详情页面-->
  <div class="org-details">
    <div class="org-info-title">
      <div class="avatar item"></div>
      <div class="name item">用户名</div>
      <div class="sex item">性别</div>
      <div class="sign item">个性签名</div>
    </div>
    <div class="chat-list">
      <div class="item" v-for="item in chat_contactList" v-on:click="jump(item)">
        <Contact :userImg="'file://'+item.avatar" :name="item.name" :sex="item.sex" :sign="item.sign" :userId="item.userId" :isStar="item.isStar"></Contact>
      </div>
    </div>
  </div>
</template>

<script >
import Contact from './Brick/List/Contact'
import { mapGetters } from 'vuex'
import vim from '../../ipcVIMSDK'
export default {
  computed: {
    ...mapGetters([
      'condition_chatWith',
      'chat_contactList',
      'user_account'
    ])
  },
  created () {
    // 将对话者类型置为-1 表示在程序其他界面
    this.condition_chatWith.userData.chatType = null
  },
  methods: {
    jump: function (item) {
      item.id = item.userId
      let newArr = {}
      // 如果发现这个目标是群的话 并且 右侧栏是展开的情况 获取群成员
      if (item.chatType === 2) {
        vim.getMemberList(item.id, (resp) => {
          console.log('获取到群成员', resp)
          resp.members.forEach((element) => {
            newArr[element.userId] = element
          })
          console.log('保存群成员', newArr)
          this.$store.commit('SET_CHAT_WITH_MEMBER', newArr)
        })
      } else {
        newArr[item.id] = item
        newArr[this.user_account.userId] = this.user_account
        this.$store.commit('SET_CHAT_WITH_MEMBER', newArr)
      }
      this.$store.commit('SET_CHAT_WITH_USER', item)
      this.$router.push({ path: '/MsgList', query: { 'index': item.id } })
    }
  },
  components: {
    Contact
  }
}
</script>

<style scoped>
.org-details {
  background-color: white;
}

.chat-list {
  height: calc(100% - 70px);
  overflow-x: hidden;
  overflow-y: auto;
}

.org-info-title {
  display: flex;
  background-color: #f8f9fc;
  height: 25px;
  line-height: 25px;
  font-size: 14px;
}
.org-info-title .avatar {
  width: 25px;
  margin: 0 10px;
}
.org-info-title .name {
  width: 150px;
}
.org-info-title .sex {
  width: 60px;
}
.org-info-title .sign {
  width: 200px;
}
.chat-list .item:nth-child(odd) {
  background-color: white;
}
.chat-list .item:nth-child(even) {
  background-color: #fafdfb;
}
</style>
