export default {
  state: {
    hasLogin: false
  },

  mutations: {
    SET_HASLOGIN: (state, hasLogin) => {
      state.hasLogin = hasLogin
    }
  }
}
