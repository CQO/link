<template>
  <div class="contact-list">
    <img :src="userImg" onerror="this.src='http://myweb-10017157.cossh.myqcloud.com/2017/0610/20140524012042770.gif';"></img>
    <div class="name">{{name}}</div>
    <div class="sex" v-if="sex === 1">男</div>
    <div class="sex" v-else>女</div>
    <div class="sign">{{sign}}</div>
    <div class="icon" :class="{ star: showStart }" v-on:click.stop="sart">&#xe616;</div>
  </div>
</template>

<script>
import vim from '../../../../ipcVIMSDK'
export default {
  data () {
    return {
      showStart: this.isStar
    }
  },
  props: {
    userImg: String,
    name: String,
    sign: String,
    sex: Number,
    isStar: Boolean,
    userId: Number
  },
  methods: {
    sart: function () {
      const boo = !this.showStart
      const data = {
        isStar: boo,
        userId: this.userId
      }
      vim.updateContactInfo(data, (resp) => {
        if (resp.code === 0) {
          console.log(`ID:${this.userId}星标好友设置成功`)
          this.showStart = boo
        }
      })
    }
  }
}
</script>

<style scoped>
.contact-list {
  display: flex;
  height: 29px;
  line-height: 29px;
  text-align: left;
  padding: 2px 0;
  font-size: 13px;
}
img {
  height: 25px;
  width: 25px;
  margin: 2px 10px;
}
.name {
  width: 150px;
  text-overflow: ellipsis;
  overflow: hidden;
  white-space: nowrap;
}
.sex {
  width: 60px;
}
.sign {
  width: 200px;
  overflow-x: auto;
  overflow-y: hidden;
}
.star {
  color: gold;
}
</style>
