<template>
  <div class="template">
    <div class="title" :style="{ color: messageBody.titleColor, background: messageBody.titleBgColor }">{{messageBody.title}}</div>
    <div class="body" v-html="messageBody.content"></div>
    <div class="link">
      <a :href="messageBody.detailUrl" target="_blank">阅读更多</a>
    </div>
  </div>
</template>

<script>
// import vim from '../../../../ipcVIMSDK'
export default {
  props: {
    message: String
  },
  data () {
    return {
      messageBody: JSON.parse(this.message)
    }
  }
}
</script>

<style scoped>
.template {
  width: 280px;
}

.title {
  height: 30px;
  line-height: 30px;
  padding: 0 5px;
  border-radius: 5px 5px 0 0;
}

.body {
  background-color: ghostwhite;
  padding: 5px 10px;
}

.link {
  height: 30px;
  line-height: 30px;
  border-radius: 0 0 5px 5px;
  background-color: ghostwhite;
  padding: 0 5px;
  text-align: right;
}
</style>