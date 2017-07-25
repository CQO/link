<template>
  <div class="file-msg">
    <div class="left-area">
      <div class="icon" v-html="icon"></div>
      <div class="size">{{fileSize}} KB</div>
    </div>
    <div class="right-area">
      <div class="file-name" v-html="fileName"></div>
      <div class="file-progress" v-if="showProgress" style="width:0px" ref="fileProgress"></div>
      <div class="download-button" v-if="showProgress" @click="downLoad">下载</div>
      <div class="download-button" v-if="!showProgress" @click="openFile">打开文件夹</div>
    </div>
  </div>
</template>
<script>
import vim from '../../../../ipcVIMSDK'
const { shell } = require('electron')
export default {
  data () {
    return {
      fileName: null,
      icon: null,
      fileSize: null,
      showProgress: true,
      fullPath: null
    }
  },
  methods: {
    downLoad () {
      let msgId = this.msg.messageId
      const message = JSON.parse(this.msg.message)
      this.fullPath = this.$store.state.chat.UserFilePath + this.fileName
      let downInfo = {
        targetId: this.msg.targetId,
        messageId: msgId,
        localPath: this.fullPath,
        remotePath: message.mediaUrl,
        fromId: this.msg.senderId
      }
      vim.downloadFile(downInfo, (res) => {
        if (res.code === 0) {
          this.showProgress = false
        }
      }, (resp) => {
        this.$refs.fileProgress.style.width = resp.progess + 70 + 'px'
      })
    },
    openFile () {
      shell.showItemInFolder(this.fullPath)
    }
  },
  mounted () {
    console.log(this.msg)
    const message = JSON.parse(this.msg.message)
    const isText = /.*\.(txt|doc|xml|)$/
    // 判断传来的文件是文本文件吗
    if (isText.test(message.fileName)) {
      this.icon = '&#xe602;'
    }
    const isPicture = /.*\.(png|pdf|bmp|jpg|svg|ppsx|)$/
    if (isPicture.test(message.fileName)) {
      this.icon = '&#xe646;'
    }
    const isCompress = /.*\.(zip|rar|7z|)$/
    if (isCompress.test(message.fileName)) {
      this.icon = '&#xe612;'
    }
    this.fileName = message.fileName
    this.fileSize = Math.floor(message.fileSize / 1024)
  },
  props: ['msg']
}
</script>
<style scoped>
.file-msg {
  display: flex;
  height: 90px;
  border: 1px solid #f1f1f7;
}

.left-area {
  width: 80px;
  height: 90px;
}

.icon {
  height: 70px;
  width: 70px;
  font-size: 3rem;
  text-align: center;
  line-height: 70px;
  color: skyblue;
}

.size {
  height: 20px;
  width: 70px;
  text-align: center;
  color: #ccc;
}

.right-area {
  padding: 10px;
  width: 170px;
}

.file-name {
  height: 50px;
  font-size: 1rem;
}

.download-button {
  height: 30px;
  line-height: 30px;
  text-align: right;
}

.file-progress {
  width: 120px;
  height: 4px;
  background: #87ceeb;
}

.download-button:hover {
  color: #87ceeb;
}
</style>
