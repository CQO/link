<template>
  <div class="contact" :class="{ ismini: mini }">
    <!--聊天界面左侧联系人列表界面-->
    <transition-group class="group" name="flip-list" tag="div">
      <div v-for="(item, index) in evenNumbers" v-on:click="jump(item, index)" v-bind:key="item.id">
        <Contact :userImg="item.avatar" :name="item.name" :unreadCount="item.unreadCount"></Contact>
      </div>
    </transition-group>
    <router-link tag="div" class="addFriend" to="AddFriend">+添加好友</router-link>
    <div class="icon shrink" v-on:click="miniSwitch">&#xe61b;</div>
  </div>
</template>

<script>
import Contact from './Brick/List/Contact_modern'
import ExpandList from './Brick/List/ExpandList'
import { mapGetters } from 'vuex'
import vim from '../../ipcVIMSDK'
export default {
  components: {
    Contact,
    ExpandList
  },
  data () {
    return {
      friend: {
        code: 0
      },
      mini: false
    }
  },
  computed: {
    ...mapGetters([
      'chat_chatList',
      'user_account'
    ]),
    evenNumbers: function () {
      const chatList = this.chat_chatList.chats
      const filter = chatList.sort((a, b) => {
        return b.time - a.time
      })
      return filter
    }
  },
  methods: {
    jump: function (item, index) {
      if (item.unreadCount > 0) { // 如果未读数 大于1 则清空未读数
        vim.setMessageRead(item.id, item.lastMsgId, (resp) => {
          console.log(resp)
        })
      }
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
      item.index = index
      console.log('设置当前聊天对象:', item)
      this.$store.commit('SET_CHAT_WITH_USER', item) // 保存目标用户信息
      this.$router.push({ path: '/MsgList', query: { 'index': item.id } })
    },
    miniSwitch: function (item) {
      this.mini = !this.mini
    },
    test: function () {
      console.log(this.chat_chatList.chats)
      var arrSimple2 = this.chat_chatList.chats
      const lalla = arrSimple2.sort((a, b) => {
        return b.time - a.time
      })
      console.log(lalla)
    }
  }
}
</script>

<style>
.ismini .contact-list .name {
  display: none;
}

.ismini .contact-list img {
  height: 45px;
  width: 45px;
  border: 1px solid #ccc;
  margin: 0 5px;
}
</style>


<style scoped>
.contact {
  width: 200px;
  height: 100%;
  background: white;
  overflow-x: hidden;
  overflow-y: auto;
  border-right: 1px solid #eee;
  position: relative;
  transition: width 0.5s;
}

.ismini {
  width: 57px;
}

.ismini ::-webkit-scrollbar {
  display: none;
}
.ismini .addFriend {
  font-size: 12px;
}

.ismini .group {
  width: 58px;
}

.router-link-exact-active .contact-list {
  background-color: #d6f0fd;
}

.addFriend {
  display: flex;
  height: 35px;
  line-height: 35px;
  font-size: 0.9rem;
  position: absolute;
  bottom: 0;
  width: 100%;
  justify-content: center;
  color: #9e9ea1;
}

.flip-list-move {
  transition: transform 0.5s;
}

.shrink {
  position: fixed;
  left: 185px;
  top: calc(50% - 20px);
  line-height: 100px;
  width: 16px;
  font-size: 16px;
  color: cadetblue;
  z-index: 999;
  height: 100px;
}

.shrink:hover {
  color: skyblue;
  background-color: aliceblue;
}

.ismini .shrink {
  transform: rotate(180deg);
  left: 58px;
}

.group {
  height: calc(100% - 35px);
  overflow-x: hidden;
  overflow-y: auto;
  width: 200px;
}
</style>
