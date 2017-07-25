<template>
  <div id="right">
    <div class="block">
      <span class="wrapper">
        <el-button type="success" @click="up">经典</el-button>
        <el-button type="warning" @click="up1">现代</el-button>
      </span>
    </div>
  </div>
</template>

<script>
export default {
  data () {
    return {}
  },
  methods: {
    up: function () {
      localStorage.setItem('sty', 'Cont')
      localStorage.setItem('nav', 'el-menu--dark1')
      location.reload()
    },
    up1: function () {
      localStorage.setItem('nav', 'el-menu--dark2')
      localStorage.setItem('sty', 'Cont1')
      location.reload()
    }
  }

}
</script>

<style scoped>
#right {
  height: 100%;
  width: 100%;
}

.block {
  height: 100%;
  width: 100%;
  background: #fff;
}

.el-tag {
  padding: 40px 88px;
  font-size: 20px;
}

.el-button {
  margin: 40px 0 0 40px;
  ;
  display: block;
  color: #000;
}

.wrapper {
  overflow: hidden;
  display: block;
}

.wrapper button {
  color: #fff;
}

.wrapper button:first-child {
  background: #50411e;
}

.el-button--success {
  border-color: #50411e;
}

.wrapper button:nth-child(2) {
  background: #00b925;
}

.el-button--warning {
  border-color: #00b925;
}
</style>
