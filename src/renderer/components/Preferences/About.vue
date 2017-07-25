<template>
  <div id="right">
    <div class="block">
      <dl class="about">
        <dt>
          <h2>关于豆豆</h2>
        </dt>
        <dd>
          豆豆是一款崭新互联网协作平台，解决的普遍问题包括:沟通、任务协同、知识共享和信息同步...
        </dd>
      </dl>
      <div class="doudou_icon">
        版本号: doudou 1.0.0
      </div>
      <ul class="edition">
        <li>豆豆</li>
        <li>
          <a href="javascript:void(0)">北信源</a>&nbsp其他&nbsp
          <a href="javascript:void(0)">开源软件</a>
        </li>
        <li>版权所有2017 北信源软件股份有限公司 保留所有权利。</li>
        <li>北信源豆豆
          <a href="javascript:void(0)">相关条款</a>
        </li>
      </ul>
    </div>
  </div>
</template>
<script>
export default {
  data () {
    return {
    }
  },
  methods: {
  }
}
</script>
<style scoped>
ol,
ul {
  list-style: none;
}

#right {
  height: 100%;
  width: 100%;
}

.block {
  height: 100%;
  width: 100%;
  background: #fff;
}

.about {
  width: 100%;
  float: left;
}

.about dt {
  margin-left: 20px;
  line-height: 60px;
  border-bottom: 1px solid #ccc;
}

.about dt h2 {
  font-weight: normal;
  font-size: 18px;
}

.about dd {
  padding: 0 20px;
  line-height: 40px;
  text-indent: 4em;
  margin: 20px 0 50px 0;
  background: url("../../assets/images/linkdood_logo.png") no-repeat 20px 0/40px 40px;
}

.doudou_icon {
  width: 100%;
  height: 100px;
  float: left;
  background: url("../../assets/images/linkdood_logo.png") no-repeat center/80px auto;
  text-align: center;
  line-height: 220px;
}

.edition {
  width: 100%;
  float: left;
  margin-top: 100px !important;
}

.edition li {
  padding: 0 20px;
  line-height: 25px;
  font-size: 14px;
}
</style>
