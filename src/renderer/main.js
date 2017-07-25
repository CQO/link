import Vue from 'vue'
import './assets/themes/index.css'
import ElementUI from 'element-ui'
import App from './App.vue'
import store from './store'
import router from './router'

Vue.use(ElementUI)

/* eslint-disable no-new */
new Vue({
  router,
  store,
  el: '#app',
  render: h => h(App)
})
