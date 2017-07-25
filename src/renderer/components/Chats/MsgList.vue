<template>
  <div class="chats-box">
    <div class="chats-area left">
      <div class="slide-content">
        <div class="title">{{condition_chatWith.userData.name}}</div>
        <div class="bubble-box" v-if="chatContent">
          <div class="bubble-area" v-for="(item,index) in chatContent" :key="item.sendTime">
            <p class="time">{{getTimes(item.sendTime)}}</p>
            <msgBubble :mark='item'></msgBubble>
          </div>
        </div>
      </div>
      <InputMsg></InputMsg>
      <div class="shrink icon" @click="rightColumnClick">&#xe61b;</div>
    </div>
    <SidebarRight v-if="rightColumn_show"></SidebarRight>
  </div>
</template>
<script>
import InputMsg from './MsgInput'
import msgBubble from './Message'
import SidebarRight from '../Contacts/Right'
import { mapGetters } from 'vuex'
import vim from '../../ipcVIMSDK'
import { MsgOrder } from './MsgOrder.js'
export default {
  data () {
    return {
      retractNotice: false,
      chatContent: null
    }
  },
  computed: {
    ...mapGetters([
      'condition_chatWith',
      'rightColumn_show'
    ])
  },
  components: {
    msgBubble,
    InputMsg,
    SidebarRight
  },
  created () {
    MsgOrder.$on('newMessage', (resp) => {
      if (resp.targetId === this.condition_chatWith.userData.id) {
        return this.chatContent.push(resp)
      }
    })
    const targetId = this.condition_chatWith.userData.id
    vim.getMessages(targetId, 0, 10, 0, (resp) => {
      if (resp.code === 0) {
        console.log('获取历史聊天记录成功：', resp)
        this.chatContent = resp.messages
      } else {
        console.log('获取历史聊天记录失败：', resp)
      }
    })
  },
  methods: {
    getTimes (time) { // 根据时间戳来计算当前时间
      return new Date(parseInt(time)).toLocaleString().replace(/:\d{1,2}$/, ' ')
    },
    rightColumnClick () {
      this.$store.commit('RIGHT_COLUMN_SWITCH')
    }
  },
  watch: {
    chatContent () {
      this.$nextTick(() => {
        let list = document.querySelector('.bubble-box')
        list.scrollTop = list.scrollHeight
      })
    },
    '$route' (to, from) {
      const targetId = this.condition_chatWith.userData.id
      vim.getMessages(targetId, 0, 10, 0, (resp) => {
        if (resp.code === 0) {
          console.log('获取历史聊天记录成功：', resp)
          this.chatContent = resp.messages
        } else {
          console.log('获取历史聊天记录失败：', resp)
        }
      })
    }
  }
}
</script>
<style scoped>
.chats-area {
  background: #fff;
  width: 100%;
  height: 100%;
  display: flex;
  flex-direction: column;
  position: relative;
}

.left {
  position: relative;
  display: flex;
  flex: 1;
}

.slide-content {
  flex: 1;
  display: flex;
  flex-direction: column
}

.slide-content .bubble-box {
  overflow-y: auto;
}

.time {
  text-align: center;
  font-size: 12px;
  margin: 2px 0;
  line-height: 30px;
  color: #717274;
}

.slide-content .title {
  width: 100%;
  height: 35px;
  line-height: 35px;
  border-bottom: solid 1px #ccc;
  font-weight: normal;
  margin: 0 12 0 12px;
  text-align: center;
}

.el-select .el-input {
  width: 110px;
}

.retract {
  text-align: center;
  color: 14px;
  display: flex;
  justify-content: center;
}

.retract span {
  width: 200px;
  background: rgba(0, 0, 0, .2);
  border-radius: 8px;
}

.shrink {
  position: absolute;
  right: 0;
  height: 100px;
  line-height: 100px;
  top: calc(50% - 50px);
  color: cadetblue;
}

.shrink:hover {
  color: skyblue;
  background-color: aliceblue;
}
</style>
