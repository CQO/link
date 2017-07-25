<template>
	<div id="right">
		<div class="block">
			<ul>
				<li>
					<span>声 音</span>
					<el-checkbox v-model="checked">开启消息声音提示</el-checkbox>
				</li>
				<li>
					<span>铃 音</span>
					<el-select v-model="value2" placeholder="铃音1">
						<el-option v-for="sound in music" :key="sound.id" :label="sound.label" :value="sound.value"></el-option>
					</el-select>
				</li>
				<li>
					<span>免打扰</span>
					<el-checkbox-group v-model="checkList">
						<el-checkbox label="开启消息声音提醒"></el-checkbox>
					</el-checkbox-group>
				</li>
			</ul>
		</div>
	</div>
</template>
<script>
export default {
  data () {
    return {
      checked: true,
      checkList: '',
      value4: [new Date(2016, 9, 10, 8, 40), new Date(2016, 9, 10, 9, 40)],
      options: [{
        value: '选项1',
        label: '所有'
      }, {
        value: '选项2',
        label: 'VIP'
      }, {
        value: '选项3',
        label: '无'
      }],
      value1: '所有',
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
      value2: '铃音2',
      form: [{
        value: '选项1',
        label: '头像'
      }, {
        value: '选项2',
        label: '发送者'
      }, {
        value: '选项3',
        label: '内容'
      }],
      value3: '头像'
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

ul {
	padding: 30px 0;
}

ul li {
	padding: 14px 60px;
}

ul li span {
	display: inline-block;
	width: 74px;
	height: 36px;
	line-height: 36px;
	text-align: left;
	font-size: 16px;
	padding-right: 10px;
}

ul li .el-checkbox-group {
	display: inline;
}
</style>
