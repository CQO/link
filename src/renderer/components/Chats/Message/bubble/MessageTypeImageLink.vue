<template>
  <div class="image-link">
    <div class="title">{{messageBody.title}}</div>
    <div class="picUrl">
      <img :src="imgUrl"></img>
    </div>
    <div class="url">
      <a :href="messageBody.url" target="_blank">阅读更多</a>
    </div>
  </div>
</template>

<script>
import vim from '../../../../ipcVIMSDK'
export default {
  props: {
    message: String,
    targetId: Number
  },
  data () {
    return {
      messageBody: JSON.parse(this.message)[0],
      imgUrl: null
    }
  },
  created () {
    const messageBody = JSON.parse(this.message)[0]
    console.log('下载图片', this.targetId, messageBody.picUrl)
    vim.downloadImage(this.targetId, messageBody.picUrl, (resp) => { // 下载图片
      if (resp.code === 0) {
        this.imgUrl = `file://${this.$store.state.chat.imagePath}${resp.imgName}`
      }
    })
  }
}
</script>

<style scoped>
.image-link {
  width: 380px;
  border: 1px solid #d4d4d4;
  overflow: hidden;
  background-color: ghostwhite;
  border-radius: 5px;
}

.title {
  height: 35px;
  line-height: 35px;
  font-size: 1rem;
  font-weight: bold;
  margin: 0 10px;
  border-bottom: 1px solid #ccc;
}

.picUrl {
  height: 180px;
  overflow: hidden;
  margin: 5px 10px;
}

.picUrl img {
  width: 360px;
  height: 180px;
}

.url {
  height: 30px;
  line-height: 30px;
  border-top: 1px solid #f1f1f7;
  padding: 0 10px;
  text-align: right;
}
</style>