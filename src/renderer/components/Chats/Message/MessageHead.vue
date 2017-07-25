<template>
  <div class="message-head">
    <!--<div class="time">{{getTimes(mark.sendTime)}}</div>-->
    <div class="avatar">
      <img v-if="condition_member && condition_member[mark.senderId]" :src="'file://' + condition_member[mark.senderId].avatar" onerror="this.src='http://myweb-10017157.cossh.myqcloud.com/2017/0610/20140524012042770.gif';">
    </div>
  </div>
</template>
<script type="text/javascript">
export default {
  data () {
    return {

    }
  }
}
</script>

<style scoped>

</style>
