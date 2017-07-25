<template>
  <div class="contact-list">
    <img v-if="userImg" :src="'file://'+userImg" onerror="this.src='http://myweb-10017157.cossh.myqcloud.com/2017/0610/20140524012042770.gif';"></img>
    <img v-else src="http://myweb-10017157.cossh.myqcloud.com/2017/0610/20140524012042770.gif"></img>
    <div class="body">
      <div class="name">{{name}}</div>
      <div class="unreadCount" v-if="unreadCount > 0">{{unreadCount}}</div>
    </div>
  </div>
</template>

<script>
export default {
  props: {
    userImg: String,
    name: String,
    time: String,
    unreadCount: Number,
    message: String
  }
}
</script>

<style scoped>
.contact-list{
  display: flex;
  padding: 5px 0;
  overflow: hidden;
  position: relative;
  background-color: ghostwhite;
  font-size: 14px;
}
.contact-list:hover {
  background-color: #cee9f9;
}
img{
  height: 25px;
  width: 25px;
  margin: 0 5px;
  margin-left: 10px;
}
.body{
  height: 100%;
  width: calc(100% - 50px);
  display: flex;
  justify-content: space-between;
}
.name{
  line-height: 25px;
}
.message{
  height: 20px;
  line-height: 20px;
  color: #a1aaab;
  font-size: 12px;
}
.time{
  color: #cfd1d9;
  font-size: 12px;
  width: 30px;
  text-align: center;
}
.unreadCount {
  background-color: red;
  width: 17px;
  height: 17px;
  text-align: center;
  line-height: 17px;
  color: white;
  border-radius: 50%;
  margin: 4px;
}
</style>
