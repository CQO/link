import Vue from 'vue'
import VueRouter from 'vue-router'

// 引入组件
import Auth from '../components/Auth'
import Signup from '../components/Auth/Signup'

import Preferences from '../components/preferences'
import Chats from '../components/Chats'
import MsgList from '../components/Chats/MsgList'
import AddFriend from '../components/Contacts/AddFriend'
import GroupProfile from '../components/Contacts/OrgInfo'

Vue.use(VueRouter)

export const routes = [
  { path: '/', redirect: '/auth', isTab: false },
  { path: '/auth', component: Auth, isTab: false },
  { path: '/signup', component: Signup, isTab: false },
  {
    path: '/preferences',
    name: 'Preferences',
    component: Preferences,
    isTab: true,
    isIcon: true,
    notSearch: true,
    children: []
  },
  {
    path: '/chats',
    name: 'chats',
    chName: '聊天',
    icon: 'el-icon-date',
    component: Chats,
    isTab: true,
    notSearch: true,
    children: [
      { path: '/MsgList', name: 'MsgList', component: MsgList },
      { path: '/GroupProfile', name: 'GroupProfile', component: GroupProfile },
      { path: '/AddFriend', name: 'AddFriend', component: AddFriend, isMenu: true }
    ]
  },
  { path: '', name: 'search', chName: '搜索', icon: 'el-icon-document', isTab: true, notSearch: false }
]
export default new VueRouter({
  routes
})
