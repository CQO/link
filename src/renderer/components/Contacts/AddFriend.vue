<template>
  <div class="query-user">
    <!--添加好友界面-->
    <div class="title">添加好友</div>
    <div class="query-box">
      <input type="text" v-model="searchText"></input>
      <div class="btn" v-on:click="search">查找用户</div>
    </div>
    <div class="searchResult">
      <div class="item" v-for="item in searchData.buddys" :key="item.userId">
        <img :src="'file://' + item.avatar">
        <div class="information">
          <div class="name">{{item.name}}</div>
          <div class="addFriend">加为好友</div>
          <div class="userId" v-if="item.userId">ID: {{item.userId}}</div>
          <div class="userId" v-if="item.groupId">群ID: {{item.groupId}}</div>
        </div>
      </div>
      <div class="clear"></div>
    </div>
  </div>
</template>

<script >
import vim from '../../ipcVIMSDK'
export default {
  data () {
    return {
      searchText: null,
      searchData: {
        buddys: null
      }
    }
  },
  methods: {
    search: function () {
      if (this.searchText) {
        vim.searchFromNet(this.searchText, (resp) => {
          console.log(resp)
          this.searchData = resp
        })
      } else {
        alert('需要输入至少一个字符')
      }
    }
  }
}
</script>

<style scoped>
.query-user {
  background-color: white;
}
input {
  width: calc(100% - 100px);
  font-size: 1rem;
}
.title {
  height: 35px;
  line-height: 35px;
  font-size: 1rem;
  font-weight: bold;
  text-align: center;
  background-color: #CCFFFF;
}
.query-box {
  display: flex;
  justify-content: space-between;
  height: 40px;
  margin: 10px;
}
.btn {
  line-height: 40px;
  background-color: skyblue;
  padding: 0 5px;
  color: white;
  border-radius: 5px;
  font-size: 0.9rem;
  cursor: pointer;
}
.searchResult {
  height: calc(100% - 100px);
  background-color: white;
  overflow-y: auto;
  overflow-x: hidden;
}
.searchResult .item {
  height: 45px;
  width: 200px;
  float: left;
  overflow: hidden;
  margin: 5px;
  border: 1px solid #ccc;
  padding: 5px;
  display: flex;
  border-radius: 5px;
  position: relative;
}
.searchResult .item img {
  height: 45px;
  width: 45px;
}
.information {
  margin-left: 10px;
}
.information .name {
  height: 25px;
  line-height: 25px;
  font-weight: bold;
  color: #6699CC;
}
.information .userId {
  line-height: 20px;
  height: 20px;
  font-size: 14px;
}
.clear {
  clear: both;
}
.addFriend {
  position: absolute;
  top: 6px;
  right: 5px;
  font-size: 12px;
  background-color: skyblue;
  border-radius: 5px;
  padding: 2px;
  color: white;
  width: 50px;
  height: 14px;
  line-height: 14px;
  text-align: center;
}
</style>
