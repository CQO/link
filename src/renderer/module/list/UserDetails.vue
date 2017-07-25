<template>
  <!--右侧栏顶部用户详情组件-->
  <div class="user-details-box">
    <div class="img-box">
      <img :src="imgUrl" onerror="this.src='http://myweb-10017157.cossh.myqcloud.com/2017/0610/20140524012042770.gif';"></img>
    </div>
    <div class="details-box">
      <div class="user">
        <span class="name">{{userName}}</span>
        <span class="icon" v-html="gender"></span>
      </div>
      <div class="id">ID: {{id}}</div>
    </div>
  </div>
</template>

<script>
export default {
  props: {
    imgUrl: String,
    userName: String,
    id: Number,
    gender: String,
    detail: String
  }
}
</script>

<style>
.user-details-box {
  display: flex;
  padding: 5px;
  background-color: white;
  position: relative;
  height: 50px;
}

.img-box img {
  width: 45px;
  height: 45px;
}

.details-box {
  height: 100%;
}

.user-name {
  height: 20px;
  line-height: 20px;
  font-size: 16px;
  text-align: left;
  font-weight: 700;
  color: #555459;
}

.details-box {
  margin-left: 10px;
}

.details-box .user {
  height: 27px;
  line-height: 27px;
  color: #555459;
  font-size: 1rem;
  font-weight: bold;
}

.details-box .icon {
  color: skyblue;
}

.id {
  color: #666;
  height: 20px;
  line-height: 20px;
  font-size: 12px;
}
</style>
