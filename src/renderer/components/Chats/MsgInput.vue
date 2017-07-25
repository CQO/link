<template>
  <div class="input-box">
    <div class="input-btn icon" @click.stop="switchs">&#xe6df;</div>
    <textarea class="input-message" placeholder="请输入内容" @keyup.ctrl.enter="sendInfo" v-model="message" @onfocus='onFocus'></textarea>
    <div class="emoji-btn icon" @click.stop="showExpression">&#xe6b3;</div>
    <ul class="choose-tab" v-show="visibleOpen">
      <li class="choose-style" @click="changeStyle">切换风格</li>
      <li class='choose-file'>选择文件
        <input type="file" ref="inputFile" @change='uploadFile'>
      </li>
    </ul>
    <Emoji v-show="expressionOpen"></Emoji>
  </div>
</template>
<script>
import { mapGetters } from 'vuex'
import vim from '../../ipcVIMSDK'
import Emoji from './Message/Emoji'
import { MsgOrder } from './MsgOrder.js'

export default {
  data () {
    return {
      visibleOpen: false,
      expressionOpen: false,
      message: '',
      messageType: 2
    }
  },
  components: {
    Emoji
  },
  computed: {
    ...mapGetters([
      'condition_chatWith',
      'chat_imagePath',
      'user_account'
    ])
  },
  created () {
    this.$nextTick(() => {
      MsgOrder.$on('expressionAdd', (item) => { // 监听表情
        this.message += item
      })
      let abody = document.querySelector('body') // 点击空白处关闭所有菜单
      abody.onclick = () => {
        this.closeMenu()
      }
    })
  },
  methods: {
    showExpression () { // 显示表情
      this.expressionOpen = !this.expressionOpen
      this.visibleOpen = false
    },
    switchs () {
      this.visibleOpen = !this.visibleOpen
      this.expressionOpen = false
    },
    closeMenu () {
      this.visibleOpen = false
      this.expressionOpen = false
    },
    changeStyle () { // 改变气泡风格
      MsgOrder.$emit('changeBubble')
    },
    onFocus () { // 获取焦点后关闭其他菜单
      this.expressionOpen = false
      this.visibleOpen = false
    },
    getMsg (id, msg, type, sendTime, messageId, senderId) {
      let messages = {
        targetId: id,
        message: msg,
        messageType: type,
        sendTime: sendTime,
        messageId: messageId,
        senderId: senderId
      }
      return messages
    },
    sendInfo () { // 发送文本信息
      const id = this.condition_chatWith.userData.id
      let msg = '{"body":' + JSON.stringify(this.message) + '}'
      vim.sendMessage(id, msg, '', 2, '', (res) => {
        if (res.code === 0) {
          let message = this.getMsg(id, msg, 2, res.sendTime, res.messageId, this.user_account.userId)
          MsgOrder.$emit('newMessage', message)
        }
      })
      this.message = ''
      this.expressionOpen = false
      this.visibleOpen = false
    },
    mathRandow () { // 获取16位数字（加密使用）
      let num = ''
      for (let i = 0; i < 16; i++) {
        num += Math.floor(Math.random() * 10)
      }
      return num
    },
    uploadFile () { // 上传文件
      const files = this.$refs.inputFile.files[0].path
      const isJPG = files.match(/jpg|png|gif/g) // 判断是否为图片
      const isfile = files.match(/txt|doc|xls|pdf/g)
      const name = this.$refs.inputFile.files[0].name // 上传文件的名字
      const size = this.$refs.inputFile.files[0].size // 上传文件的大小
      const id = this.condition_chatWith.userData.id // targetId
      if (isfile) {
        let fileProperty = {
          targetId: this.condition_chatWith.userData.id,
          encryptkey: this.mathRandow(),
          localPath: files
        }
        vim.uploadFile(fileProperty, (resp) => { // 上传文件SDK
          if (resp.code === 0) {
            let message = '{"fileName":' + JSON.stringify(name) + ',' + '"fileSize":' + JSON.stringify(size) + '}'
            vim.sendMessage(fileProperty.targetId, message, '', 6, '', (res) => { // 发送文件消息
              if (res.code === 0) {
                let fileMsg = this.getMsg(id, message, 6, res.sendTime, res.messageId, this.user_account.userId)
                MsgOrder.$emit('newMessage', fileMsg)
              }
            })
          }
        })
      } else if (isJPG) { // 图片类型
        let encryptkey = this.mathRandow() // 加密密码
        let imageSavePath = this.chat_imagePath + encryptkey + '.jpg' // 图片保存路径
        let ImageInfo = vim.thumbImage(files) // 图片压缩后的信息
        vim.uploadImage(id, files, ImageInfo.originBuffer, imageSavePath, ImageInfo.width, ImageInfo.height, encryptkey, (resp) => {
          if (resp.code === 0) {
            let thumbUrl = JSON.parse(resp.thumbJson).files[0].url // 返回缩略图地址
            let srcUrl = JSON.parse(resp.srcJson).files[0].url // 返回大图地址
            let fileName = JSON.parse(resp.srcJson).files[0].filename // 返回文件名字
            let message = '{"fileName":' + JSON.stringify(fileName) + ',"thumbUrl":' + JSON.stringify(thumbUrl) + ',"mediaUrl":' + JSON.stringify(srcUrl) + ',"docid":1' + ',"encryptUser":' + id + ',"enc_dec_key":' + JSON.stringify(encryptkey) + ',"width":' + ImageInfo.width + ',"height":' + ImageInfo.height + '}'
            vim.sendMessage(id, message, '', 5, imageSavePath, (res) => { // 发送图片消息
              if (res.code === 0) {
                let imgMsg = this.getMsg(id, message, 5, res.sendTime, res.messageId, this.user_account.userId)
                MsgOrder.$emit('newMessage', imgMsg)
              }
            })
          }
        })
      }
    }
  }
}
</script>
<style scoped>
.choose-tab {
  position: absolute;
  top: -105px;
  background: #fff;
  border-radius: 4px;
  border: 1px solid #ccc;
  text-align: center;
  width: 80px;
  padding: 0;
  margin: 0;
  padding: 10px;
  -webkit-box-shadow: 0 0 10px rgba(204, 204, 204, .5);
  -moz-box-shadow: 0 0 10px rgba(204, 204, 204, .5);
  box-shadow: 0 0 10px rgba(204, 204, 204, .5);
}

.choose-file,
.choose-style {
  padding: 6px;
  height: 30px;
  line-height: 40px;
  position: relative;
  cursor: pointer;
  color: #fff;
  font-size: 14px;
  background: #2d3a4b;
  border: 1px solid #ddd;
  border-radius: 4px;
  overflow: hidden;
  display: inline-block;
  display: inline;
  zoom: 1
}

.choose-file input {
  position: absolute;
  right: 0;
  top: 0;
  opacity: 0;
  filter: alpha(opacity=0);
  cursor: pointer
}

.choose-tab li:hover {
  cursor: pointer;
}

.input-box {
  display: flex;
  border: 2px solid #E0E0E0;
  position: relative;
  margin: 5px;
  border-radius: 5px;
}

.input-box .input-btn {
  width: 34px;
  height: 34px;
  line-height: 34px;
  text-align: center;
  border: none;
  border-right: 2px solid #E0E0E0;
  font-size: 30px;
  color: #E0E0E0;
}

.input-box .input-btn:hover {
  background: #56B68B;
  color: #FFF;
  border-color: #56B68B;
}

.input-box .input-message {
  flex: 1;
  border: none;
  display: flex;
  align-self: center;
  resize: none;
  height: 34px;
  line-height: 34px;
  font-size: 13px;
  padding-left: 5px;
}

.input-box .emoji-btn {
  width: 34px;
  height: 34px;
  line-height: 34px;
  font-size: 20px;
  text-align: center;
  color: #ccc;
}
</style>
