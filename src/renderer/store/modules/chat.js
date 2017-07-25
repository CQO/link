// import Vue from 'vue'
const chat = {
  state: {
    // 好友列表
    ContactList: {},
    // 聊天列表
    ChatList: {
      chats: []
    },
    // 图片路径
    imagePath: '',
    // 文件路径
    UserFilePath: ''
  },

  mutations: {
    SET_IMAGEPATH: (state, data) => {
      state.imagePath = data
    },
    SET_UserFilePath: (state, data) => {
      state.UserFilePath = data
    },
    SET_CONTACTLIST: (state, contactList) => {
      state.ContactList = contactList
    },
    SET_CHATLIST: (state, data) => {
      switch (data.flag) {
        case 16: {
          const chats = state.ChatList.chats
          let newChats = chats.concat(data.chats)
          state.ChatList = {
            chats: newChats
          }
          break
        }
        case 1: {
          state.ChatList = data
          break
        }
        case 5: {
          state.ChatList = data
          break
        }
        case 8: {
          let newArr = state.ChatList
          for (let i = 0; i < data.chats.length; i++) {
            for (let key = 0; key < newArr.chats.length; key++) {
              if (newArr.chats[key].id === data.chats[i].id) {
                newArr.chats[key].unreadCount = data.chats[i].unreadCount
                newArr.chats[key].time = data.chats[i].time
              }
            }
          }
          state.ChatList = {
            chats: newArr.chats
          }
        }
      }
    }
  }
}

export default chat
