<template>
  <div class="chats-box">
    <ContactList></ContactList>
    <router-view class="message-box"></router-view>
  </div>
</template>

<script>
import ContactList from '../Contacts/Chatlist'
export default {
  components: {
    ContactList
  }
}
</script>

<style>
.chats-box {
  width: 100%;
  display: flex;
  background-image: url('../../assets/images/linkdood_logo.png');
  background-repeat: no-repeat;
  background-position: 409px 40%;
  background-size: 256px;
  overflow: hidden;
}

.message-box {
  flex: 1;
}
</style>
