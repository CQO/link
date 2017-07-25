import { app, BrowserWindow, Menu } from 'electron'
const appMenu = require('./menu')
// const EventEmitter = require('events')

let mainWindow = null
let isQuitting = false

/**
 * Set `__static` path to static files in production
 * https://simulatedgreg.gitbooks.io/electron-vue/content/en/using-static-assets.html
 */
if (process.env.NODE_ENV !== 'development') {
  global.__static = require('path').join(__dirname, '/static').replace(/\\/g, '\\\\') // eslint-disable-line
}

const winURL = process.env.NODE_ENV === 'development'
  ? 'http://localhost:9080'
  : `file://${__dirname}/index.html`

// 启动前先关闭上次未退出的窗口
const isAlreadyRunning = app.makeSingleInstance(() => {
  if (mainWindow) {
    if (mainWindow.isMinimized()) {
      mainWindow.restore()
    }
    mainWindow.show()
  }
})

if (isAlreadyRunning) {
  app.quit()
}

function createMainWindow () {
  const win = new BrowserWindow({
    title: app.getName(),
    width: 800,
    height: 600,
    webPreferences: {
      webSecurity: false
    }
  })
  if (process.platform === 'darwin') {
    win.setSheetOffset(40)
  }
  win.loadURL(winURL)
  win.on('close', e => {
    if (!isQuitting) {
      e.preventDefault()
      if (process.platform === 'darwin') {
        app.quit()
      } else {
        app.quit()
      }
    }
  })
  win.on('page-title-updated', e => {
    e.preventDefault()
  })
  return win
}
let appIcon = null
app.on('ready', () => {
  Menu.setApplicationMenu(appMenu)
  mainWindow = createMainWindow()
})

app.on('hasLogin', (event, arg) => {
  console.log(arg + '$$$$$$$$$$$$$$$$$$$$$$$$')
  const contextMenu = Menu.buildFromTemplate([
    {
      label: '在线',
      click: function () {
        mainWindow.show()
      }
    },
    {
      label: '离线',
      click: function () {
        mainWindow.show()
      }
    },
    {
      label: '打开主面板',
      click: function () {
        mainWindow.show()
      }
    },
    {
      label: '退出',
      click: function () {
        app.quit()
      }
    }
  ])
  appIcon.setContextMenu(contextMenu)
})

app.on('activate', () => {
  mainWindow.show()
})

app.on('before-quit', () => {
  isQuitting = true
})
