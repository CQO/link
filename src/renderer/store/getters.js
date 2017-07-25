const getters = {
  user_account: state => state.condition.myself,
  chat_contactList: state => state.chat.ContactList,
  chat_chatList: state => state.chat.ChatList,
  chat_imagePath: state => state.chat.imagePath,
  chat_UserFilePath: state => state.chat.UserFilePath,
  auth_hasLogin: state => state.auth.hasLogin,
  rightColumn_show: state => state.condition.rightColumnShow,
  condition_chatWith: state => state.condition.chatWith,
  condition_member: state => state.condition.member
}
export default getters
