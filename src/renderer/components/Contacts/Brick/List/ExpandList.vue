<template>
  <!--下拉展开模块-->
  <div class="expand-list-box">
    <div class="list" v-on:click="spread">
      <div class="left">
        <div class="icon" v-html="icon"></div>
        <span>{{title}}</span>
      </div>
      <div class="right" v-on:click="listClick" v-bind:class="{ active: showExpand }">
        <svg t="1495644719748" class="icon" style="" viewBox="0 0 1024 1024" version="1.1" xmlns="http://www.w3.org/2000/svg" p-id="1677" xmlns:xlink="http://www.w3.org/1999/xlink" width="12" height="12">
          <path d="M218.703 908.422v-799.42c0-44.239 35.503-80.023 79.743-80.022 20.57 0 39.17 7.892 53.261 20.569l527.503 395.339c34.941 26.487 42.271 76.366 15.781 111.585-4.51 6.201-9.868 11.555-15.782 15.782l-533.131 399.857c-34.945 26.199-85.090 19.156-111.303-16.064-10.991-14.375-16.064-30.997-16.064-47.619v0z" p-id="1678" fill="#c8c9cc"></path>
        </svg>
      </div>
    </div>
    <div class="expand" v-show="showExpand">
      <slot></slot>
    </div>
  </div>
</template>

<script>
export default {
  props: {
    icon: String,
    title: String,
    text: String,
    to: String
  },
  methods: {
    listClick: function () {
      this.showExpand = !this.showExpand
    },
    spread: function () {
      if (this.to) {
        location.href = this.to
      }
    }
  },
  data () {
    return {
      showExpand: true
    }
  }
}
</script>

<style scoped>
.expand-list-box{
  position: relative;
  overflow: hidden;
  user-select: none;
  cursor: default;
  padding-bottom: 1px;
}
.list{
  display: flex;
  justify-content: Space-between;
  height: 35px;
  line-height: 35px;
  padding-left: 5px;
  font-size: 0.9rem;
  background-color: #f8f8f8;
  color: #666;
  border-bottom: 1px solid #ebebeb;
}
.left{
  display: flex;
}
.left .icon{
  height: 40px;
  width: 20px;
  text-align: center;
}
.icon{
  color: #666;
}
svg{
  margin: 11px;
}
.expand-list-box span{
  color: #666;
}
.expand{
  text-align: left;
  line-height: 13px;
  color: #717274;
  font-size: 13px;
  max-height: 395px;
  overflow-x: hidden;
  overflow-y: auto;
}
.right{
  transition: All 0.4s ease-in-out;
  height: 35px;
  width: 35px;
}
.active{
  transform: rotate(90deg);
}
.expand p {
  padding: 0px 10px;
  font-size: 0.8rem;
}
</style>
