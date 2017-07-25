<template>
  <div id="personal">
    <el-row :gutter="24">
      <ul class="left">
        <el-col :span="4">
          <li @click="tabToggle(index ,tab.path)" v-for="(tab,index) in tabs" :class="{active:active===index}">
            {{tab.name}}
          </li>
        </el-col>
      </ul>
      <el-col :span="20">
        <component :is="currentView"></component>
      </el-col>
    </el-row>
  </div>
</template>

<script>
import About from './About.vue'
import Account from './Account.vue'
import System from './System.vue'
import Notice from './Notice.vue'
import Privacy from './Privacy.vue'
import Security from './Security.vue'
import Theme from './Theme.vue'
export default {
  components: {
    About,
    Account,
    Security,
    Theme,
    Privacy,
    System,
    Notice
  },
  data () {
    return {
      currentView: 'Account',
      active: 0,
      tabs: [
        {
          name: '账  号',
          path: 'Account'
        },
        {
          name: '通  用',
          path: 'System'
        },
        {
          name: '隐  私',
          path: 'Privacy'
        },
        {
          name: '主  题',
          path: 'Theme'
        },
        {
          name: '安  全',
          path: 'Security'
        },
        {
          name: '关  于',
          path: 'About'
        }
      ]
    }
  },
  methods: {
    handleOpen (key, keyPath) {
      console.log(key, keyPath)
      this.infor = false
    },
    handleClose (key, keyPath) {
      console.log(key, keyPath)
    },
    handleClick (tab, event) {
      console.log(tab, event)
    },
    tabToggle (i, v) {
      this.active = i
      this.currentView = v
    }
  }
}
</script>
<style scoped>
body {
  font-family: Helvetica, sans-serif;
}

* {
  padding: 0;
  margin: 0 auto;
  box-sizing: border-box;
}

a {
  text-decoration: none;
}

.left {
  overflow: hidden;
  float: left;
}

.left li {
  line-height: 50px;
  text-align: center;
}

.left li:hover {
  cursor: pointer;
}



/*.left li:last-child {
  margin-top: 20px;
}*/

.left li a {
  color: #000;
  display: block;
}

.left li:hover {
  background: #f3f3f3;
}

.left li.active {
  background: #fff;
  transform: none;
}

.el-row {
  height: 100%;
  width: 100%;
  margin: 0 !important;
}

.el-col-4 {
  padding: 0 !important;
  position: absolute;
  top: 0;
  /*bottom: 0;*/
  background: #dcdcdc;
}

.el-col-20 {
  float: right;
  padding: 0 !important;
}
</style>
