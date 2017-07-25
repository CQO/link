<template>
  <div class="expression-box">
    <ul class="expression-item">
      <!--<li class="expression-items" v-for="item in emoji" @click="pushExepression(item)">
        <img :src="'C:/repos/dou/src/renderer/assets/emoji/' + getPureName(item.code)+ '.png'" >
      </li>-->
    </ul>
    <div class="expression-disc">
      expression info
    </div>
  </div>
</template>

<script type="text/javascript">
import { MsgOrder } from '../MsgOrder.js'
import emoji from './Emojis.js'
export default {
  data () {
    return {
      emoji: emoji.CUSTOM_ICON
    }
  },
  methods: {
    pushExepression (item) {
      MsgOrder.$emit('expressionAdd', item.name)
    },
    getPureName (name) {
      name = name.replace(/R.mipmap./g, '').replace(/-]/g, '')
      return name
    }
  }
}
</script>

<style scoped>
.expression-box {
  position: absolute;
  background: #fff;
  right: 0px;
  border: solid 1px #ccc;
  top: -270px;
  z-index: 999;
  border-radius: 10px;
  width: 250px;
  -webkit-box-shadow: 0 0 10px rgba(204, 204, 204, .5);
  -moz-box-shadow: 0 0 10px rgba(204, 204, 204, .5);
  box-shadow: 0 0 10px rgba(204, 204, 204, .5);
  overflow: hidden;
}

.expression-type {
  display: flex;
  list-style: none;
  padding: 0;
  margin: 0;
  border-bottom: solid 1px #ccc;
  background: #f7f7f7;
  line-height: 30px;
}

.expression-type li {
  flex: 1;
  text-align: center
}

.expression-item {
  margin: 0;
  padding: 4px 10px;
  height: 200px;
  overflow-y: auto;
  display: flex;
  flex-wrap: wrap;
  list-style: none
}

.expression-item li {
  text-align: center;
  line-height: 30px;
  height: 30px;
  width: 31px;
  margin-bottom: 6px;
  border-radius: 4px;
}

.expression-item li img {
  width: 100%;
}

.expression-item li:hover {
  cursor: pointer;
  background: #ccc;
}

.expression-disc {
  height: 50px;
  line-height: 50px;
  text-align: center;
  background: #f7f7f7;
  border-top: solid 1px #ccc;
}
</style>
