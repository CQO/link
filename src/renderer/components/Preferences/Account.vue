<template>
  <div id="right">
    <div class="block">
      <el-form class="demo-ruleForm" :rules="rules" ref="ruleForm" label-width="100px" :label-position="labelPosition" :model="ruleForm">
        <el-form-item prop="userName" label="姓名">
          <el-input v-model="ruleForm.userName" placeholder="请输入"></el-input>
        </el-form-item>
        <el-form-item label="出生日期" required>
          <el-col :span="11" style="padding-left: 0;">
            <el-form-item prop="birthDate">
              <el-date-picker type="date" placeholder="选择日期" v-model="ruleForm.birthDate" style="width: 100%;"></el-date-picker>
            </el-form-item>
          </el-col>
        </el-form-item>
        <el-form-item label="性别" required>
          <el-radio-group v-model="radioSex">
            <el-radio class="radio" :label="1">男</el-radio>
            <el-radio class="radio" :label="2">女</el-radio>
            <el-radio class="radio" :label="0">保密</el-radio>
          </el-radio-group>
        </el-form-item>
        <el-form-item label="邮箱地址" prop="email">
          <el-input v-model="ruleForm.email" placeholder="请输入"></el-input>
        </el-form-item>
        <el-form-item label="电话号码" prop="phone">
          <el-input v-model="ruleForm.phone" placeholder="请输入"></el-input>
        </el-form-item>
        <el-form-item label="工作职位" prop="position">
          <el-input v-model="ruleForm.position" placeholder="请输入"></el-input>
        </el-form-item>
        <el-form-item label="所属部门" prop="department">
          <el-input v-model="ruleForm.department" placeholder="请输入"></el-input>
        </el-form-item>
      </el-form>
    </div>
    <div class="picture">
      <el-upload class="avatar-uploader" action="https://jsonplaceholder.typicode.com/posts/" :show-file-list="false" :on-success="handleAvatarSuccess" :before-upload="beforeAvatarUpload">
        <img v-if="imageUrl" :src="imageUrl" title="点击更换头像" class="avatar">
        <i v-else class="avatar-uploader-icon" title="点击更换头像"></i>
      </el-upload>
    </div>
  </div>
</template>
<script>
import { mapGetters } from 'vuex'
export default {
  data () {
    return {
      imageUrl: '',
      radioSex: '',
      labelPosition: 'left',
      ruleForm: {
        userName: '',
        birthDate: '',
        email: '',
        phone: '',
        position: '',
        department: ''
      },
      rules: {
        userName: [
          { required: true, message: '请输入姓名', trigger: 'blur' },
          { min: 3, max: 5, message: '长度在 3 到 5 个字符', trigger: 'blur,change' }
        ],
        birthDate: [
          { type: 'date', required: true, message: '请输入正确格式日期，如2011-01-01', trigger: 'blur,change' }
        ],
        email: [
          { required: true, message: '请输入邮箱地址', trigger: 'blur' },
          { message: '请输入正确的邮箱地址', trigger: 'blur,change' }
        ],
        phone: [
          { required: true, pattern: /^1[34578]\d{9}$/, message: '请输入中国国内的手机号码', trigger: 'blur' },
          { min: 11, max: 11, message: '手机号码长度应为11', trigger: 'blur,change' }
        ],
        position: [
          { message: '请输入工作职位', trigger: 'blur' }
        ],
        department: [
          { message: '请输入所属部门', trigger: 'blur' }
        ]
      }
    }
  },
  computed: {
    ...mapGetters([
      'user_account'
    ])
  },
  created () {
    this.handleData()
  },
  methods: {
    handleData () {
      console.log('我的账号：', this.user_account)
      if (typeof (this.user_account.avatar) !== 'undefined') {
        this.imageUrl = this.user_account.avatar
      }
      this.radioSex = this.user_account.sex
      this.ruleForm.userName = this.user_account.name
      this.ruleForm.birthDate = this.user_account.birthday
      this.ruleForm.email = this.user_account.email
      this.ruleForm.phone = this.user_account.phone
      this.ruleForm.position = this.user_account.job
      this.ruleForm.department = this.user_account.department
    },
    handleAvatarSuccess (res, file) {
      this.imageUrl = URL.createObjectURL(file.raw)
    },
    beforeAvatarUpload (file) {
      const isJPG = file.type === 'image/jpeg'
      const isLt2M = file.size / 1024 / 1024 < 2
      if (!isJPG) {
        this.$message.error('上传头像图片只能是 JPG 格式!')
      }
      if (!isLt2M) {
        this.$message.error('上传头像图片大小不能超过 2MB!')
      }
      return isJPG && isLt2M
    },
    submitForm (formName) {
      this.$refs[formName].validate((valid) => {
        if (valid) {
          alert('submit!')
        } else {
          console.log('error submit!!')
          return false
        }
      })
    },
    resetForm (formName) {
      this.$refs[formName].resetFields()
    }
  }
}
</script>
<style scoped>
.avatar-uploader .el-upload {
  border: 1px dashed #d9d9d9;
  border-radius: 6px;
  cursor: pointer;
  position: relative;
  overflow: hidden;
}

.avatar-uploader .el-upload:hover {
  border-color: #20a0ff;
}

.avatar-uploader-icon {
  font-size: 28px;
  color: #8c939d;
  width: 178px;
  height: 178px;
  line-height: 178px;
  text-align: center;
}

.avatar {
  width: 178px;
  height: 178px;
  display: block;
}

#right {
  height: 100%;
  width: 100%;
  font-size: 18px;
}

.block {
  height: 100%;
  width: 60%;
  background: #fff;
  float: left;
}

.el-form {
  margin: 40px 0 0 40px;
}

.el-form div:nth-of-type(6) {
  margin-left: 15px;
}

.el-form div:nth-of-type(7) {
  padding-left: 15px;
}

.el-form div:nth-of-type(6) div {
  margin-left: -15px;
}

.el-form div:nth-of-type(7) div {
  margin-left: -15px;
}

.picture {
  height: 100%;
  width: 40%;
  background: #fff;
  float: left;
  position: relative;
}

.el-upload i {
  position: absolute;
  left: 50%;
  top: 40px;
  margin-left: -89px;
}

.el-upload img {
  position: absolute;
  left: 50%;
  top: 40px;
  margin-left: -89px;
  z-index: 9999;
}

.el-col-11 {
  width: 200px;
}
</style>
