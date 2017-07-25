<template>
  <div id="right">
    <div class="block">
      <div class="select">
        <p>系统选项</p>
        <el-checkbox-group v-model="systemOption">
          <el-checkbox label="开机时自启动"></el-checkbox>
          <el-checkbox label="输入信息语法检查"></el-checkbox>
        </el-checkbox-group>
        <p>语言</p>
        <el-radio-group v-model="language">
          <el-radio :label="3">中文</el-radio>
          <el-radio :label="6">English</el-radio>
        </el-radio-group>
      </div>
      <ul>
        <li>
          <span>声 音</span>
          <el-checkbox v-model="voiceWarn" class="checkbox">开启消息声音提示</el-checkbox>
        </li>
        <li>
          <span>免打扰</span>
          <el-checkbox-group v-model="systemOption">
            <el-checkbox label="开启消息声音提醒"></el-checkbox>
          </el-checkbox-group>
        </li>
        <li>
          <span>铃 音</span>
          <el-select v-model="ring" placeholder="铃音1">
            <el-option v-for="sound in music" :key="sound.id" :label="sound.label" :value="sound.value"></el-option>
          </el-select>
        </li>
        <li>
          <span>下载位置</span>
          <el-input v-model="download"></el-input>
          <el-upload class="upload-demo" action="https://jsonplaceholder.typicode.com/posts/" :on-change="handleChange">
            <el-button size="middle" type="primary">点击上传</el-button>
          </el-upload>
        </li>
        <li>
          <span>清空聊天记录</span>
          <el-button size="middle" type="primary" :loading="loading" @click.native.prevent="clearMsgs">开始清空</el-button>
        </li>
      </ul>
    </div>
  </div>
</template>
<script>
import vim from '../../ipcVIMSDK'
export default {
  data () {
    return {
      loading: false,
      systemOption: ['开机时自启动', '简体中文'],
      download: '',
      voiceWarn: true,
      language: 3,
      music: [{
        value: '选项1',
        label: '铃音1'
      }, {
        value: '选项2',
        label: '铃音2'
      }, {
        value: '选项3',
        label: '铃音3'
      }],
      ring: '铃音2'
    }
  },
  methods: {
    handleChange (file, fileList) {
    },
    clearMsgs () {
      this.loading = true
      vim.deleteAllMessage(true, (resp) => {
        console.log('删除所有消息', resp)
        if (resp.code === 0) {
          this.loading = false
          this.$message.success('删除成功')
        }
      })
    }
  }
}
</script>
<style scoped>
#right {
  height: 100%;
  width: 100%;
  font-size: 18px;
}

.block {
  height: 100%;
  width: 60%;
  background: #fff;
}

p {
  padding: 14px 60px;
  font-size: 16px;
}

.select .el-checkbox {
  width: 100%;
  display: inline-block;
  padding: 10px;
  margin-left: 90px;
}

ul li {
  font-size: 16px;
  padding: 10px 60px;
}

ul li span {
  padding-right: 10px;
}

ul li .upload-demo {
  display: inline;
}

.el-checkbox-group {
  display: inline;
}

.el-radio__inner {
  width: 18px;
  height: 18px;
  display: inline-block;
}

.el-radio+.el-radio {
  margin-left: 0;
}

.el-radio-group {
  margin-left: 90px;
}

.el-select {
  margin-left: 26px;
}

.checkbox {
  margin-left: 10px;
}
</style>
