<template>
<div class="head">
  <div class="left-column">
    <router-link tag="div" class="auth" to="/preferences">
      <img v-if="user_account.avatar" :src="'file://' + user_account.avatar">
    </router-link>
    <router-link class="item" tag="div" to="/chats">聊天</router-link>
    <router-link class="item" tag="div" to="/GroupProfile">联系人</router-link>
    <router-link class="item" tag="div" to="/Work">应用市场</router-link>
  </div>
  <div class="search right-column">
    <input type="text" class="text" v-model="fuzzySearch"></input>
    <div class="button icon" @click="search">&#xe607;</div>
  </div>
  <div class="search-result" v-if="searchResult">
    <div class="close-bar">
      <div class="close" @click="clearSearch">x</div>
    </div>
    <div v-for="item in searchResult" :key="item.userId">
      <UserDetails :imgUrl="typeof (item.userId) === 'undefined'?'file://'+item.icon:'file://'+item.avatar" :userName="item.name" :gender="item.gender" :id="typeof (item.userId) === 'undefined' ? item.groupId : item.userId"></UserDetails>
    </div>
  </div>
</div>
</template>

<script>
import { mapGetters } from 'vuex'
import UserDetails from './module/list/UserDetails'
import vim from './ipcVIMSDK'
export default {
  data () {
    return {
      fuzzySearch: '',
      searchResult: null
    }
  },
  components: {
    UserDetails
  },
  computed: {
    ...mapGetters([
      'user_account'
    ])
  },
  methods: {
    search () {
      vim.searchFromLocal(this.fuzzySearch, (resp) => {
        console.log('本地搜索结果:', resp)
        this.searchResult = resp.buddys
      })
    },
    clearSearch () {
      this.searchResult = null
    }
  }
}
</script>

<style scoped>
.head {
  height: 40px;
  line-height: 40px;
  position: relative;
  display: flex;
  background-color: #2d3a4b;
  color: #baaed7;
  font-size: 14px;
  justify-content: space-between;
}

.left-column {
  display: flex;
}

.right-column {
  font-size: 16px;
  display: flex;
}

.search {
  margin: 2px 10px;
  display: flex;
}

.search .text {
  height: 26px;
  margin: 3px 0;
}

.search .button {
  width: 30px;
  height: 30px;
  text-align: center;
  line-height: 30px;
  background-color: lightslategrey;
  color: white;
  margin: 3px 0;
}

.auth {
  height: 40px;
  display: flex;
  font-size: 12px;
  font-family: arial;
  font-style: normal;
}

.auth img {
  height: 30px;
  width: 30px;
  margin: 5px;
  border-radius: 50%;
}

.auth span {
  margin: 0 5px;
}

.head .item {
  width: 70px;
  text-align: center;
  display: inline-block;
  position: relative;
}

.head .item:after {
  content: '';
  display: block;
  height: 0;
  width: 100%;
  background: transparent;
  transition: height .5s ease, background-color .5s ease;
}

.head .router-link-active {
  color: white;
}

.head .item:hover:after,
.head .router-link-active:after {
  height: 2px;
  background: white;
  position: absolute;
  bottom: 0;
}

.head .search-result {
  position: absolute;
  max-height: 200px;
  overflow-x: hidden;
  overflow-y: auto;
  width: 157px;
  right: 10px;
  top: 35px;
  border: 1px solid #ccc;
}

.head .search-result .close-bar {
  height: 25px;
  background-color: lavenderblush;
  border-bottom: solid 1px #ccc;
  display: flex;
  justify-content: flex-end;
}

.head .search-result .close {
  background-color: gainsboro;
  width: 20px;
  height: 20px;
  border-radius: 50%;
  margin: 3px;
  line-height: 20px;
  text-align: center;
  color: brown;
  font-size: 17px;
}
</style>