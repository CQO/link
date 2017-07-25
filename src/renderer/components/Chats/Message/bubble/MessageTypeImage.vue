<template>
  <div class="img-message">
    <img class="preview-img" :src="imgPath">
  </div>
</template>
<script>
import vim from '../../../../ipcVIMSDK'
export default {
  data () {
    return {
      MessageBody: JSON.parse(this.message),
      imgPath: null
    }
  },
  created () {
    const image = JSON.parse(this.message)
    let key = image.enc_dec_key
    let targetId = this.targetId
    let decip = this.$store.state.chat.imagePath + key + '.jpg'
    vim.downloadImage(targetId, image.thumbUrl, (resp) => { // 下载图片
      if (resp.code === 0) {
        let url = 'file://' + this.$store.state.chat.imagePath + resp.imgName
        this.imgPath = 'file://' + decip
        vim.decryptFile(key, url, decip)
      }
    })
  },
  props: {
    message: String,
    targetId: Number
  }
}
</script>
<style scoped>
.img-message img {
  width: 100%;
}
</style>
