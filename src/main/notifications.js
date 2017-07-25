const ms = require('ms')
const fetch = require('node-fetch')

const {version} = require('./package')

const NEWS_URL = 'https://hyper-news.now.sh'

module.exports = function fetchNotifications (win) {
  const {rpc} = win
  const retry = err => {
    setTimeout(() => fetchNotifications(win), ms('30m'))
    if (err) {
      console.error('Notification messages fetch error', err.stack)
    }
  }

  console.log('Checking for notification messages')
  fetch(NEWS_URL, {
    headers: {
      'X-Hyper-Version': version,
      'X-Hyper-Platform': process.platform
    }
  })
  .then(res => res.json())
  .then(data => {
    const {message} = data || {}
    if (typeof message !== 'object' && message !== '') {
      throw new Error('Bad response')
    }
    if (message === '') {
      console.log('No matching notification messages')
    } else {
      rpc.emit('add notification', message)
    }

    retry()
  })
  .catch(retry)
}

const {resolve} = require('path')

const {app, BrowserWindow} = require('electron')
const isDev = require('electron-is-dev')

let win

// the hack of all hacks
// electron doesn't have a built in notification thing,
// so we launch a window on which we can use the
// HTML5 `Notification` API :'(

let buffer = []

app.on('ready', () => {
  const win_ = new BrowserWindow({
    show: false
  })
  const url = 'file://' + resolve(
    isDev ? __dirname : app.getAppPath(),
    'notify.html'
  )
  win_.loadURL(url)
  win_.webContents.on('dom-ready', () => {
    win = win_
    buffer.forEach(([title, body]) => {
      notify(title, body)
    })
    buffer = null
  })
})

function notify (title, body) {
  console.log(`[Notification] ${title}: ${body}`)
  if (win) {
    win.webContents.send('notification', {title, body})
  } else {
    buffer.push([title, body])
  }
}

module.exports = notify
