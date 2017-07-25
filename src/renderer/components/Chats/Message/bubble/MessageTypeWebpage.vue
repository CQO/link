<template>
  <div class="webpage">
    <div class="title" v-if="messageBody.title">{{messageBody.title}}</div>
    <div class="body">
      <img :src="messageBody.img"></img>
      <a class="body" :href="messageBody.url" target="_blank" v-html="messageBody.description"></a>
    </div>
  </div>
</template>

<script>
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
.webpage {
  width: 270px;
  overflow: hidden;
  background-color: ghostwhite;
  padding: 5px;
  border-radius: 5px;
}

.webpage img {
  width: 270px;
}

.title {
  height: 25px;
  line-height: 25px;
  color: azure;
}
</style>