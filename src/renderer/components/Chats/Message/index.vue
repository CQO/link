<template>
  <div :class='computedClsObj'>
    <div class="avatar">
      <img v-if="condition_member && condition_member[mark.senderId]" :src="'file://' + condition_member[mark.senderId].avatar" onerror="this.src='http://myweb-10017157.cossh.myqcloud.com/2017/0610/20140524012042770.gif';">
    </div>
    <div class="messages-body">
      <p class='receive-name' v-if='condition_member && condition_member[mark.senderId]' v-html="condition_member[mark.senderId].name?condition_member[mark.senderId].name:condition_member[mark.senderId].remark"></p>
      <ImageBubble class="msg-bubble" v-if="mark.messageType===5" :message="mark.message" :targetId="mark.targetId"></ImageBubble>
      <TextBubble class="msg-bubble" v-else-if="mark.messageType===2" :message="mark.message" :msgProperties="mark.msgProperties"></TextBubble>
      <FileBubble class="msg-bubble" v-else-if="mark.messageType===6" :msg='mark'></FileBubble>
      <TemplateBubble v-else-if="mark.messageType === 26" :message="mark.message"></TemplateBubble>
      <WebpageBubble v-else-if="mark.messageType === 9" :message="mark.message"></WebpageBubble>
      <imageLinkBubble v-else-if="mark.messageType === 12" :message="mark.message" :targetId="mark.targetId"></imageLinkBubble>
      <div class="msg-bubble" v-else>暂不支持此类型消息</div>
    </div>
  </div>
</template>
<script>
import FileBubble from './bubble/MessageTypeFile.vue'
import ImageBubble from './bubble/MessageTypeImage'
import imageLinkBubble from './bubble/MessageTypeImageLink'
import TemplateBubble from './bubble/MessageTypeTemplate'
import WebpageBubble from './bubble/MessageTypeWebpage'
import TextBubble from './bubble/MessageTypeText'
import { MsgOrder } from '../MsgOrder.js'
import { mapGetters } from 'vuex'
import emoji from './Emojis.js'
export default {
  data () {
    return {
      emoji: emoji.CUSTOM_ICON,
      active: false,
      msgTagVisible: false,
      memberList: null,
      name: null
    }
  },
  props: ['mark'],
  components: {
    FileBubble,
    imageLinkBubble,
    ImageBubble,
    TemplateBubble,
    WebpageBubble,
    TextBubble
  },
  created () {
    MsgOrder.$on('changeBubble', function () {
      this.active = !this.active
    }.bind(this))
  },
  computed: {
    ...mapGetters([
      'user_account',
      'condition_chatWith',
      'condition_member'
    ]),
    computedClsObj: function () {
      return {
        selfMsg: this.mark.senderId === this.user_account.userId && !this.active,
        otherMsg: this.mark.senderId !== this.user_account.userId && !this.active,
        aside: this.active
      }
    }
  }
}
</script>

<style scoped>
.selfMsg {
  padding: 5px 15px;
  overflow: hidden;
  display: flex;
  justify-content: flex-start;
  flex-direction: row-reverse;
  font-size: 14px;
  margin-bottom: 4px;
}

.selfMsg .avatar {
  background-color: #eef1f6;
  height: 40px;
  width: 40px;
  border-radius: 8px;
  overflow: hidden;
  margin-top: 8px;
}

.selfMsg .avatar img {
  width: 100%;
}

.selfMsg .messages-body {
  flex: 1;
  display: flex;
  align-items: flex-end;
  flex-direction: column;
  margin-right: 20px;
}

.selfMsg .receive-name {
  color: #717274;
  height: 16px;
  font-size: 13px;
}

.selfMsg .msg-bubble {
  max-width: 45%;
  min-width: 22px;
  display: flex;
  align-items: flex-end;
  background-color: skyblue;
  padding: 6px;
  border-radius: 6px;
  color: #48576a;
  word-wrap: break-word;
  word-break: break-all;
  position: relative;
  -webkit-box-shadow: 0 0 2px rgba(204, 204, 204, .7);
  -moz-box-shadow: 0 0 2px rgba(204, 204, 204, .7);
  box-shadow: 0 0 2px rgba(204, 204, 204, .7);
}

.selfMsg .msg-bubble .img-message img,
.otherMsg .msg-bubble .img-message img,
.aside .msg-bubble.img-message img {
  width: 100%;
}

.selfMsg .msg-bubble:before {
  content: '';
  display: inline-block;
  height: 0;
  width: 0;
  border-left: solid 8px #b8dcf8;
  border-top: solid 6px transparent;
  border-bottom: solid 6px transparent;
  position: absolute;
  top: 8px;
  left: 100%;
}

.otherMsg {
  padding: 5px 15px;
  display: flex;
  justify-content: flex-start;
  font-size: 14px;
  margin-bottom: 4px;
}

.otherMsg .avatar {
  background-color: #eef1f6;
  height: 40px;
  width: 40px;
  border-radius: 8px;
  overflow: hidden;
}

.otherMsg .receive-name {
  height: 16px;
  color: #717274;
  font-size: 13px;
}

.otherMsg .avatar img {
  width: 100%;
}

.otherMsg .messages-body {
  display: flex;
  flex-direction: column;
  margin-left: 20px;
}

.otherMsg .msg-bubble {
  max-width: 300px;
  min-width: 22px;
  background-color: ghostwhite;
  padding: 6px;
  border-radius: 6px;
  word-break: break-all;
  position: relative;
  -webkit-box-shadow: 0 0 2px rgba(204, 204, 204, .7);
  -moz-box-shadow: 0 0 2px rgba(204, 204, 204, .7);
  box-shadow: 0 0 2px rgba(204, 204, 204, .7);
}

.otherMsg .msg-bubble:before {
  content: '';
  display: inline-block;
  height: 0;
  width: 0;
  border-right: solid 10px ghostwhite;
  border-top: solid 6px transparent;
  border-bottom: solid 6px transparent;
  position: absolute;
  top: 8px;
  left: -8px;
}

.imgshow {
  max-width: 100%;
  border-radius: 10px
}

.aside {
  padding: 5px 15px;
  display: flex;
  justify-content: flex-start;
  font-size: 13px;
}

.aside .avatar {
  background-color: #eef1f6;
  height: 40px;
  width: 40px;
  border-radius: 8px;
  overflow: hidden;
}

.aside .receive-name {
  height: 20px;
  color: #717274;
  font-size: 12px;
}

.aside .avatar img {
  width: 100%;
}

.aside .messages-body {
  flex: 1;
  display: flex;
  flex-direction: column;
  margin-left: 20px;
}

.aside .msg-bubble {
  border-radius: 6px;
  max-width: 45%;
  min-width: 40px;
  word-break: break-all;
  position: relative;
}

.msg-tag {
  width: 60px;
  background: #ccc;
  color: 12px;
  padding: 2px;
}

.msg-tag li {
  text-align: center;
  background: #fff;
  line-height: 35px;
  color: 10px;
}
</style>
