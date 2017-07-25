<template>
  <div class="regist-container">
    <el-form autoComplete="on" :model="registForm" :rules="resetRules" ref="registForm" label-position="left" label-width="0px" class="card-box regist-form">
      <div>
        <router-link to="/auth" class="back-icon">
          <i class="el-icon-arrow-left"></i>
        </router-link>
        <h3 class="title">注册</h3>
      </div>
      <el-form-item prop="phone">
        <el-input name="phone" type="text" v-model="registForm.phone" placeholder="手机号"></el-input>
      </el-form-item>
      <el-form-item prop="code">
        <el-input name="code" type="text" style="width:70%" v-model="registForm.code" placeholder="验证码"></el-input>
        <el-button type="primary" style="width:30%,text-align:right" @click.native.prevent="getCode">
          获取验证码
        </el-button>
      </el-form-item>
      <el-form-item prop="password">
        <el-input name="password" :type="passwordType" v-model="registForm.password" placeholder="密码"></el-input>
      </el-form-item>
      <el-form-item prop="checkPass">
        <el-input name="checkPass" :type="passwordType" v-model="registForm.checkPass" placeholder="确认密码"></el-input>
        <i @click="togglePasswordType" class="el-icon-information"></i>
      </el-form-item>
      <el-form-item style="width:100%;">
        <el-button type="primary" style="width:100%;" :loading="loading" @click.native.prevent="backToLogin">
          注册
        </el-button>
      </el-form-item>
    </el-form>
  </div>
</template>

<script>
import vim from '../../ipcVIMSDK'
export default {
  name: 'regist',
  data () {
    const validateAccount = (rule, value, callback) => {
      const pattern = /^1[34578]\d{9}$/
      if (pattern.test(value)) {
        callback()
      } else if (value === '') {
        callback(new Error('请输入手机号'))
      } else {
        callback(new Error('请输入正确的手机号'))
      }
    }
    const validaePass = (rule, value, callback) => {
      if (value.length < 6) {
        callback(new Error('密码不能小于6位'))
      } else {
        callback()
      }
    }
    const validatePass2 = (rule, value, callback) => {
      if (value === '') {
        callback(new Error('请再次输入密码'))
      } else if (value !== this.registForm.password) {
        callback(new Error('两次输入密码不一致!'))
      } else {
        callback()
      }
    }
    return {
      registForm: {
        phone: '',
        password: '',
        checkPass: '',
        code: ''
      },
      passwordType: 'password',
      resetRules: {
        phone: [
          { required: true, trigger: 'blur', validator: validateAccount }
        ],
        password: [
          { required: true, trigger: 'blur', validator: validaePass }
        ],
        checkPass: [
          { required: true, trigger: 'blur', validator: validatePass2 }
        ],
        code: [
          { required: true, message: '必填项', trigger: 'blur' }
        ]
      },
      loading: false
    }
  },
  methods: {
    getCode () {
      vim.getRegCode('0086' + this.registForm.phone, 1, 'vrv', (resp) => {
        if (resp.code === 0) {
          this.$message.success('成功')
        } else if (resp.code === 182) {
          this.$message.error('该手机账号已经存在！')
        }
      })
    },
    backToLogin () {
      this.loading = true
      const _this = this
      this.$refs.registForm.validate(valid => {
        if (valid) {
          vim.signup('62140486', '测试账号', _this.registForm.checkPass, (resp) => {
            console.log('注册成功回调：', resp)
            setTimeout(() => {
              _this.$router.push({ path: '/auth' })
              this.loading = false
            }, 3 * 1000)
          })
        } else {
          console.log('error submit!!')
          this.loading = false
        }
      })
    },
    togglePasswordType () {
      if (this.passwordType === 'text') {
        this.passwordType = 'password'
      } else {
        this.passwordType = 'text'
      }
    }
  }
}
</script>

<style scoped>
.regist-container {
  height: 100vh;
  background-color: #2d3a4b;
}
input:-webkit-autofill {
  -webkit-box-shadow: 0 0 0px 1000px #293444 inset !important;
  -webkit-text-fill-color: #3E3E3E !important;
}


.back-icon {
  float: left;
  margin-top: 5px;
}
.el-icon-information {
  position: absolute;
  right: -18px;
  top: 10px;
}
.regist-form {
  position: absolute;
  left: 0;
  right: 0;
  width: 350px;
  padding: 35px 35px 15px 35px;
  margin: 120px auto;
}

.card-box {
  padding: 20px;
  box-shadow: 0 0px 8px 0 rgba(0, 0, 0, 0.06), 0 1px 0px 0 rgba(0, 0, 0, 0.02);
  -webkit-border-radius: 5px;
  border-radius: 5px;
  -moz-border-radius: 5px;
  background-clip: padding-box;
  margin-bottom: 20px;
  background-color: #F9FAFC;
  width: 400px;
  border: 2px solid #8492A6;
}

.title {
  margin: 0px auto 40px auto;
  text-align: center;
  color: #505458;
}
</style>
