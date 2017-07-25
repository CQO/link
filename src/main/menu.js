const os = require('os')
const path = require('path')
const electron = require('electron')

const app = electron.app
const BrowserWindow = electron.BrowserWindow
const shell = electron.shell
const appName = app.getName()

function sendAction (action) {
  const win = BrowserWindow.getAllWindows()[0]

  if (process.platform === 'darwin') {
    win.restore()
  }

  win.webContents.send(action)
}

const viewSubmenu = [
  {
    label: 'Reset Text Size',
    accelerator: 'CmdOrCtrl+0',
    click () {
      sendAction('zoom-reset')
    }
  },
  {
    label: 'Increase Text Size',
    accelerator: 'CmdOrCtrl+Plus',
    click () {
      sendAction('zoom-in')
    }
  },
  {
    label: 'Decrease Text Size',
    accelerator: 'CmdOrCtrl+-',
    click () {
      sendAction('zoom-out')
    }
  }
]

const helpSubmenu = [
  {
    label: `${appName} Website`,
    click () {
      shell.openExternal('https://github.com/sindresorhus/anatine')
    }
  },
  {
    label: 'Report an Issue...',
    click () {
      const body = `
<!-- Please succinctly describe your issue and steps to reproduce it. -->
-
${app.getName()} ${app.getVersion()}
Electron ${process.versions.electron}
${process.platform} ${process.arch} ${os.release()}`

      shell.openExternal(`https://github.com/sindresorhus/anatine/issues/new?body=${encodeURIComponent(body)}`)
    }
  }
]

if (process.platform !== 'darwin') {
  helpSubmenu.push({
    type: 'separator'
  }, {
    role: 'about',
    click () {
      electron.dialog.showMessageBox({
        title: `About ${appName}`,
        message: `${appName} ${app.getVersion()}`,
        detail: 'Created by Sindre Sorhus',
        icon: path.join(__dirname, 'static/Icon.png')
      })
    }
  })
}

const darwinTpl = [
  {
    label: appName,
    submenu: [
      {
        role: 'about'
      },
      {
        role: 'services',
        submenu: []
      },
      {
        type: 'separator'
      },
      {
        role: 'hide'
      },
      {
        role: 'hideothers'
      },
      {
        role: 'unhide'
      },
      {
        type: 'separator'
      },
      {
        role: 'quit'
      }
    ]
  },
  {
    label: '编辑',
    submenu: [
      {
        label: '撤销',
        accelerator: 'Command+Z',
        selector: 'undo:'
      },
      {
        label: '重做',
        accelerator: 'Shift+Command+Z',
        selector: 'redo:'
      },
      {
        type: 'separator'
      },
      {
        label: '剪切',
        accelerator: 'Command+X',
        selector: 'cut:'
      },
      {
        label: '复制',
        accelerator: 'Command+C',
        selector: 'copy:'
      },
      {
        label: '粘贴',
        accelerator: 'Command+V',
        selector: 'paste:'
      },
      {
        label: '全选',
        accelerator: 'Command+A',
        selector: 'selectAll:'
      }
    ]
  },
  {
    label: '窗口',
    role: '',
    submenu: [
      {
        role: 'minimize'
      },
      {
        role: 'close'
      }
    ]
  },
  {
    label: '切换',
    role: '',
    submenu: [
      {
        label: '会话列表',
        accelerator: 'Command+1',
        selector: ''
      },
      {
        label: '联系人列表',
        accelerator: 'Command+2',
        selector: ''
      },
      {
        label: '群列表',
        accelerator: 'Command+3',
        selector: ''
      },
      {
        label: '组织架构',
        accelerator: 'Command+4',
        selector: ''
      },
      {
        type: 'separator'
      },
      {
        label: '移除该会话',
        accelerator: 'Command+D',
        selector: ''
      },
      {
        label: '切换未读'
      },
      {
        label: '切换至上一个会话',
        accelerator: 'Command+Plus',
        selector: ''
      },
      {
        label: '切换至下一个会话',
        accelerator: 'Command+-',
        selector: ''
      }
    ]
  }
]

const otherTpl = [
  {
    label: 'File',
    submenu: [
      {
        label: 'New Tweet...',
        accelerator: 'Ctrl+N',
        click () {
          sendAction('new-tweet')
        }
      },
      {
        label: 'New Direct Message...',
        accelerator: 'Shift+Ctrl+N',
        click () {
          sendAction('new-dm')
        }
      },
      {
        type: 'separator'
      },
      {
        label: 'Next Tab',
        accelerator: 'Ctrl+Tab',
        click () {
          sendAction('next-tab')
        }
      },
      {
        label: 'Previous Tab',
        accelerator: 'Ctrl+Shift+Tab',
        click () {
          sendAction('previous-tab')
        }
      },
      {
        type: 'separator'
      },
      {
        label: 'Toggle Dark Mode',
        accelerator: 'Ctrl+Shift+D',
        click () {
          sendAction('toggle-dark-mode')
        }
      },
      {
        type: 'separator'
      },
      {
        label: 'Log Out',
        click () {
          sendAction('log-out')
        }
      },
      {
        type: 'separator'
      },
      {
        role: 'quit'
      }
    ]
  },
  {
    label: 'Edit',
    submenu: [
      {
        role: 'undo'
      },
      {
        role: 'redo'
      },
      {
        type: 'separator'
      },
      {
        role: 'cut'
      },
      {
        role: 'copy'
      },
      {
        role: 'paste'
      },
      {
        role: 'pasteandmatchstyle'
      },
      {
        role: 'delete'
      },
      {
        type: 'separator'
      },
      {
        role: 'selectall'
      }
    ]
  },
  {
    label: 'View',
    submenu: viewSubmenu
  },
  {
    role: 'help',
    submenu: helpSubmenu
  }
]

// const beforeLogin = [{
//   label: '打开主面板',
//   click: function () {
//     mainWindow.show()
//   }
// },
// {
//   label: '退出',
//   click: function () {
//     app.quit()
//   }
// }
// ]
const tpl = process.platform === 'darwin' ? darwinTpl : otherTpl

module.exports = electron.Menu.buildFromTemplate(tpl)
