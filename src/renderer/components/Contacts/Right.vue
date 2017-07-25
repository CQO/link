<template>
  <div class="sidebar-right">
    <UserDetails v-if="condition_chatWith.userData.chatType" class="item" :imgUrl="'file://'+condition_chatWith.userData.avatar" :userName="condition_chatWith.userData.name" :gender="gender" :id="condition_chatWith.userData.id"></UserDetails>
    <ExpandList class="item" v-if="this.condition_chatWith.userData.chatType === 2 && condition_member" icon="#" title="群成员">
      <Contact v-for="item in condition_member" :key="item.userId" :userImg="'file://'+item.avatar" :name="item.remark" :unreadCount="0"></Contact>
    </ExpandList>
    <div class="shield sidebar-right-item">
      <div>屏蔽消息</div>
      <SwitchItem class="switch-item" color="blue" v-model="isShield"></SwitchItem>
    </div>
  </div>
</template>

<script>
import SwitchItem from './Brick/Switch'
import UserDetails from '../../module/list/UserDetails'
import ExpandList from './Brick/List/ExpandList'
import Contact from './Brick/List/Contact_modern'
import vim from '../../ipcVIMSDK'
import { mapGetters } from 'vuex'
export default {
  components: {
    UserDetails,
    ExpandList,
    Contact,
    SwitchItem
  },
  computed: {
    ...mapGetters([
      'condition_chatWith',
      'condition_member'
    ])
  },
  data () {
    return {
      gender: '&#xe600;',
      remark: '',
      isShield: false
    }
  },
  methods: {
    toChat: function (item) {
      const id = typeof (item.userId) === 'undefined' ? item.groupId : item.userId
      this.$store.commit('SET_CHAT_WITH_USER', item)
      this.$router.push({ path: '/MsgList', query: { 'index': id } })
    },
    setRemark: function () { // 设置备注功能
      const data = {
        remark: this.remark,
        index: this.condition_chatWith.userData.index,
        userId: this.condition_chatWith.userData.id
      }
      vim.updateContactInfo(data, (resp) => {
        if (resp.code === 0) {
          this.$store.commit('CHANGE_REMARK', data)
          console.log(`ID:${this.condition_chatWith.userData.id}更改备注成功`)
        }
      })
    }
  }
}
</script>

<style scoped>
.sidebar-right {
  border-left: 1px solid #e8e8e8;
  width: 175px;
  top: 60px;
  height: 100%;
  right: 0;
  background-color: white;
}

.sidebar-right .item {
  border-bottom: 1px solid #e8e8e8;
}

.sidebar-right-item {
  height: 30px;
  line-height: 30px;
  background-color: whitesmoke;
  padding: 0 5px;
  display: flex;
  justify-content: space-between;
  color: lightslategray;
}

.switch-item {
  height: 10px;
  margin: 10px 0;
}
</style>
