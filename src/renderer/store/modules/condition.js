const condition = {
  state: {
    rightColumnShow: false,
    chatWith: { // 和谁在聊天
      userData: {
        chatType: null,
        member: null
      } // 当前聊天的用户信息
    },
    myself: { // 我(当前登录用户)的用户信息
      avatar: null
    },
    member: null, // 如果是群的话该字段不为空
    relation: { // ID与ID所对应信息 对应表
    },
    showSearch: false
  },

  mutations: {
    RIGHT_COLUMN_SWITCH: (state) => { // 改变右侧栏显示状态
      state.rightColumnShow = !state.rightColumnShow
    },
    RIGHT_COLUMN_SET: (state, isHide) => { // 隐藏右侧栏
      state.rightColumnShow = isHide
    },
    SET_CHAT_WITH_USER: (state, userData) => {
      state.chatWith.userData = userData
    },
    SET_CHAT_WITH_MEMBER: (state, data) => {
      state.member = data
    },
    CHANGE_REMARK: (state, data) => {
      let lalal = state.chatWith.userData
      lalal.name = data.remark
      state.chatWith.userData = lalal
    },
    SET_ACCOUNT: (state, data) => { // 设置当前登录用户信息
      state.myself = data
      state.myself.userId = data.id
    },
    UPDATE_ACCOUNT: (state, data) => { // 更新当前登录用户信息
      state.myself = { ...state.myself,
        phone: data.phone,
        email: data.email,
        name: data.name
      }
    }
  }
}

export default condition
