<template>
  <div class="webpage">
    <div v-if="show" class="title" :style="{ fontSize: textSize }">{{messageBody.body}}</div>
  </div>
</template>

<script>
export default {
  props: {
    message: String,
    msgProperties: String
  },
  data () {
    return {
      messageBody: JSON.parse(this.message),
      textSize: 'inherit',
      show: true
    }
  },
  created () {
    // console.log(this.messageBody)
    if (this.msgProperties) {
      console.log(this.msgProperties)
      const properties = JSON.parse(this.msgProperties)
      if (properties.textSize && properties.textSize !== 0 && properties.textSize < 50) {
        this.textSize = properties.textSize + 'px'
      }
    }
  }
}
</script>