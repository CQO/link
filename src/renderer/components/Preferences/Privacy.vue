<template>
  <div id="right">
    <div class="block">
      <dl>
        <dt>好友验证</dt>
        <dd>
          <el-radio-group v-model="radio2">
            <el-radio :label="3">需要身份验证</el-radio>
            <el-radio :label="6">允许任何人添加</el-radio>
            <el-radio :label="9">不允许任何人添加</el-radio>
          </el-radio-group>
        </dd>
      </dl>
      <dl>
        <dt>搜索可见性</dt>
        <dd>
          <el-radio-group v-model="radio3">
            <el-radio :label="3">所有人可见</el-radio>
            <el-radio :label="6">不所有人可见</el-radio>
          </el-radio-group>
        </dd>
      </dl>
      <div class="blacklist">
        <el-menu default-active="2" class="el-menu-vertical-demo" @open="handleOpen" @close="handleClose">
          <el-submenu index="1-4">
            <template slot="title">黑名单</template>
            <el-menu-item index="1-4-1">好友1</el-menu-item>
            <el-menu-item index="1-4-2">好友2</el-menu-item>
            <el-menu-item index="1-4-3">好友3</el-menu-item>
          </el-submenu>
        </el-menu>
      </div>
    </div>
  </div>
</template>
<script>
export default {
  data () {
    return {
      radio2: 3,
      radio3: 3
    }
  },
  methods: {
    handleOpen (key, keyPath) {
      console.log(key, keyPath)
    },
    handleClose (key, keyPath) {
      console.log(key, keyPath)
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
  font-size: 16px;
}

.block dl {
  overflow: hidden;
}

.block dt {
  line-height: 40px;
  padding-left: 20px;
  margin: 10px 20px;
}




/*.el-radio-group {
	overflow: hidden;
}*/

.el-radio-group label {
  width: 18px;
  display: block;
  margin-left: 80px;
  height: 40px;
  line-height: 40px;
}

.blacklist {
  background: #fff;
  overflow: hidden;
  width: 15%;
  margin-left: 20px;
}

.el-menu {
  background: #fff;
}

.el-menu-item {
  line-height: 40px;
  height: 40px;
  text-align: left;
}
</style>
