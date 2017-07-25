<template>
    <label :class="classObject">
        <span class="vue-switcher__label" v-if="shouldShowLabel">
            <span v-if="label" v-text="label"></span>
            <span v-if="!label && value" v-text="textEnabled"></span>
            <span v-if="!label && !value" v-text="textDisabled"></span>
        </span>
        <input type="checkbox" :disabled="disabled" @change="trigger" :checked="value">
        <div></div>
    </label>
</template>

<script>
export default {
  name: 'switches',
  props: {
    typeBold: {
      default: false
    },
    value: {
      default: false
    },
    disabled: {
      default: false
    },
    label: {
      default: ''
    },
    textEnabled: {
      default: ''
    },
    textDisabled: {
      default: ''
    },
    color: {
      default: 'default'
    },
    emitOnMount: {
      default: true
    }
  },
  mounted () {
    if (this.emitOnMount) {
      this.$emit('input', this.value)
    }
  },
  methods: {
    trigger (e) {
      this.$emit('input', e.target.checked)
    }
  },
  computed: {
    classObject () {
      const { color, value, typeBold, disabled } = this
      return {
        'vue-switcher': true,
        ['vue-switcher--unchecked']: !value, // eslint-disable-line
        ['vue-switcher--disabled']: disabled, // eslint-disable-line
        ['vue-switcher--bold']: typeBold, // eslint-disable-line
        ['vue-switcher--bold--unchecked']: typeBold && !value, // eslint-disable-line
        [`vue-switcher-theme--default`]: color,
        [`vue-switcher-color--${color}`]: color
      }
    },
    shouldShowLabel () {
      return this.label !== '' || this.textEnabled !== '' || this.textDisabled !== ''
    }
  }
}
</script>


<style>
.vue-switcher {
  position: relative;
  display: inline-block
}
.vue-switcher__label {
  display: block;
  font-size: 10px;
  margin-bottom: 5px
}
.vue-switcher input {
  opacity: 0;
  width: 100%;
  height: 100%;
  position: absolute;
  z-index: 1;
  cursor: pointer
}
.vue-switcher div {
  height: 10px;
  width: 40px;
  position: relative;
  border-radius: 30px;
  display: -ms-flex;
  display: -webkit-box;
  display: -ms-flexbox;
  display: flex;
  -webkit-box-align: center;
  -ms-flex-align: center;
  align-items: center;
  -webkit-box-pack: start;
  -ms-flex-pack: start;
  justify-content: flex-start;
  cursor: pointer;
  transition: all .4s ease
}
.vue-switcher div:after {
  content: "";
  height: 18px;
  width: 18px;
  border-radius: 100px;
  display: block;
  transition: all .3s ease;
  position: absolute;
  left: 100%;
  margin-left: -17px;
  cursor: pointer;
  top: -4px
}
.vue-switcher--unchecked div {
  -webkit-box-pack: end;
  -ms-flex-pack: end;
  justify-content: flex-end
}
.vue-switcher--unchecked div:after {
  left: 15px
}
.vue-switcher--disabled div {
  opacity: .3
}
.vue-switcher--disabled input {
  cursor: not-allowed
}
.vue-switcher--bold div {
  top: -8px;
  height: 26px;
  width: 51px
}
.vue-switcher--bold div:after {
  margin-left: -22px;
  top: 4px
}
.vue-switcher--bold--unchecked div:after {
  left: 26px
}
.vue-switcher--bold .vue-switcher__label span {
  padding-bottom: 7px;
  display: inline-block
}
.vue-switcher-theme--default.vue-switcher-color--default div {
  background-color: #b7b7b7
}
.vue-switcher-theme--default.vue-switcher-color--default div:after {
  background-color: #9d9d9d
}
.vue-switcher-theme--default.vue-switcher-color--default.vue-switcher--unchecked div {
  background-color: #aaa
}
.vue-switcher-theme--default.vue-switcher-color--default.vue-switcher--unchecked div:after {
  background-color: #c4c4c4
}
.vue-switcher-theme--default.vue-switcher-color--blue div {
  background-color: #77b0c8
}
.vue-switcher-theme--default.vue-switcher-color--blue div:after {
  background-color: #539bb9
}
.vue-switcher-theme--default.vue-switcher-color--blue.vue-switcher--unchecked div {
  background-color: #c0dae5
}
.vue-switcher-theme--default.vue-switcher-color--blue.vue-switcher--unchecked div:after {
  background-color: #77b0c8
}
.vue-switcher-theme--default.vue-switcher-color--red div {
  background-color: #c87777
}
.vue-switcher-theme--default.vue-switcher-color--red div:after {
  background-color: #b95353
}
.vue-switcher-theme--default.vue-switcher-color--red.vue-switcher--unchecked div {
  background-color: #e5c0c0
}
.vue-switcher-theme--default.vue-switcher-color--red.vue-switcher--unchecked div:after {
  background-color: #c87777
}
.vue-switcher-theme--default.vue-switcher-color--yellow div {
  background-color: #c9c377
}
.vue-switcher-theme--default.vue-switcher-color--yellow div:after {
  background-color: #bab353
}
.vue-switcher-theme--default.vue-switcher-color--yellow.vue-switcher--unchecked div {
  background-color: #e6e3c0
}
.vue-switcher-theme--default.vue-switcher-color--yellow.vue-switcher--unchecked div:after {
  background-color: #c9c377
}
.vue-switcher-theme--default.vue-switcher-color--orange div {
  background-color: #c89577
}
.vue-switcher-theme--default.vue-switcher-color--orange div:after {
  background-color: #b97953
}
.vue-switcher-theme--default.vue-switcher-color--orange.vue-switcher--unchecked div {
  background-color: #e5cec0
}
.vue-switcher-theme--default.vue-switcher-color--orange.vue-switcher--unchecked div:after {
  background-color: #c89577
}
.vue-switcher-theme--default.vue-switcher-color--green div {
  background-color: #77c88d
}
.vue-switcher-theme--default.vue-switcher-color--green div:after {
  background-color: #53b96e
}
.vue-switcher-theme--default.vue-switcher-color--green.vue-switcher--unchecked div {
  background-color: #c0e5ca
}
.vue-switcher-theme--default.vue-switcher-color--green.vue-switcher--unchecked div:after {
  background-color: #77c88d
}
</style>