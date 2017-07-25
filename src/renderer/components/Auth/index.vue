<template>
  <div class="login-container">
    <el-form autoComplete="on" :model="loginForm" :rules="loginRules" ref="loginForm" label-position="left" label-width="0px" class="card-box login-form">
      <img src="../../assets/images/linkdood_logo.png" class="img"></img>
      <h3 class="title">豆豆</h3>
      <el-form-item prop="user">
        <el-input name="user" type="text" v-model="loginForm.user" autoComplete="on" placeholder="用户名"></el-input>
      </el-form-item>
      <el-form-item prop="password">
        <el-input name="password" type="password" @keyup.enter.native="handleLogin" v-model="loginForm.password" autoComplete="on" placeholder="密码"></el-input>
      </el-form-item>
      <el-checkbox v-model="checked">自动登录</el-checkbox>
      <el-form-item>
        <el-button type="primary" style="width:100%;" :loading="loading" @click.native.prevent="handleLogin">
          登录
        </el-button>
      </el-form-item>
      <router-link to="/signup" class="toRegist">
        首次登录？
      </router-link>
    </el-form>
    <el-dialog title="第三方验证" :visible.sync="showDialog">
      邮箱登录成功,请选择第三方验证
      <!--<socialSign></socialSign>-->
    </el-dialog>
  </div>
</template>

<script>
// import { mapGetters } from 'vuex'
import vim from '../../ipcVIMSDK'

export default {
  data () {
    const validateUser = (rule, value, callback) => {
      if (value === '') {
        callback(new Error('请输入用户名'))
      } else {
        callback()
      }
    }

    const validatePass = (rule, value, callback) => {
      if (value.length < 6) {
        callback(new Error('密码不能小于6位'))
      } else {
        callback()
      }
    }

    return {
      checked: false,
      loginForm: {
        user: localStorage.getItem('userName'),
        password: localStorage.getItem('password')
      },
      loginRules: {
        user: [
          { required: true, trigger: 'blur', validator: validateUser }
        ],

        password: [
          { required: true, trigger: 'blur', validator: validatePass }
        ]
      },
      loading: false,
      showDialog: false
    }
  },
  methods: {
    handleLogin () {
      const self = this
      this.$refs.loginForm.validate(valid => {
        if (valid) {
          vim.login('0086' + self.loginForm.user, self.loginForm.password, 1, 'vrv', (resp) => {
            if (vim.getHasLogin()) {
              const userBean = {
                'account': '0086' + self.loginForm.user,
                'pwd': self.loginForm.password,
                'server': 'vrv',
                'userId': resp.userId,
                'autoLogin': self.checked,
                'userType': 1,
                'time': new Date()
              }
              const result = vim.setLoginInfo(userBean)
              console.log('设置登录信息', result)
              const results = vim.getLoginInfos()
              console.log('获取登录信息', results)
              // const localSetting = {
              //   name: '0086' + self.loginForm.user,
              //   pswd: self.loginForm.password,
              //   autoLogin: self.checked
              // }
              // const loginConfig = {
              //   'key': 'config',
              //   'value': JSON.stringify(localSetting)
              // }
              // vim.updateLocalSetting([loginConfig], (resp) => {
              //   console.log('>>>>>>>>>>>>>>>>>>>>>>更新本地配置：', resp)
              //   vim.getLocalSetting(['config'], (resp) => {
              //     console.log('>>>>>>>>>>>>>>>>>>>>>>获取本地配置：', resp)
              //   })
              // })
              // vim.addLocalSetting([obj], (resp) => {
              //   console.log('>>>>>>>>>>>>>>>>>>>>>>添加本地配置：', resp)
              //   vim.getLocalSetting(['config'], (resp) => {
              //     console.log('>>>>>>>>>>>>>>>>>>>>>>获取本地配置：', resp)
              //   })
              // })
              localStorage.setItem('checked', self.checked)
              localStorage.setItem('userName', self.loginForm.user)
              localStorage.setItem('password', self.loginForm.password)
              this.$router.push('/chats')
            } else {
              if (resp.code === 101) {
                this.$message.error('没有此用户！')
              } else if (resp.code === 112) {
                this.$message.error('帐号密码错误！')
              } else if (resp.code === 113) {
                this.$message.error('帐号已经登录！')
              } else if (resp.code === -1) {
                this.$message.error('请检查网络连接是否正常！')
              } else if (resp.code === 11100) {
                this.$message.error('服务无法找到！')
              }
              return false
            }
          })
        }
      })
    }
  }
}
</script>

<style scoped>
.tips {
  font-size: 14px;
  color: #fff;
  margin-bottom: 5px;
}

.login-container {
  height: 100vh;
  background-color: #2d3a4b;
  z-index: 999999;
}

input:-webkit-autofill {
  -webkit-box-shadow: 0 0 0px 1000px #293444 inset !important;
  -webkit-text-fill-color: #fff !important;
}

input {
  background: transparent;
  border: 0px;
  -webkit-appearance: none;
  border-radius: 0px;
  padding: 12px 5px 12px 15px;
  color: #eeeeee;
  height: 47px;
}

.el-input {
  display: inline-block;
  height: 47px;
  width: 85%;
}

.svg-container {
  padding: 6px 5px 6px 15px;
  color: #889aa4;
}

.title {
  font-size: 26px;
  font-weight: 400;
  color: #eeeeee;
  margin: 0px auto 40px auto;
  text-align: center;
  font-weight: bold;
}

.login-form {
  position: absolute;
  left: 0;
  right: 0;
  width: 400px;
  padding: 35px 35px 15px 35px;
  margin: 120px auto;
}
.el-form-item {
  border: 1px solid rgba(255, 255, 255, 0.1);
  background: rgba(0, 0, 0, 0.1);
  border-radius: 5px;
  color: #454545;
}

.toRegist {
  color: #fff;
}
.img {
  position: relative;
  margin-top: -60px;
  clear: both;
  display: block;
  margin-left: auto;
  margin-right: auto;
  width: 120px;
}
.main {
  background: #fff;
}
.el-checkbox__input.is-checked .el-checkbox__inner {
  background-color: #666
}
.el-checkbox__label {
  display: inline;
  color: #fff;
}
.el-form-item .el-form-item__content .el-input {
  width: 100%;
  height: 100%;
}
</style>
