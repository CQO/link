# doudou

> Linkdood 以协作为目的企业级即时通讯软件

## Build Setup

``` bash
# 安装依赖项
npm install

# 启动electron, 热加载刷新
npm run dev

# 构建产品包体积优化
npm run build

```

## 项目文件说明

``` bash
dou/
    ├── app/               # 源码根目录
    │   ├── filters/       # vue数据过滤器
    │   └── store/         # vuex，Flux风格的数据通讯框架
    │   │    ├── index.js  # vuex入口
    │   ├── components/    # 组件根目录
    │   │    ├── Auth      # 认证 登录 注册等
    │   │    ├── Chats     # 聊天
    │   │    ├── Contacts  # 通讯录
    │   │    ├── Me        # 账号
    │   │    ├── Perferences  # 系统偏好设置
    │   │    ├── Search    # 智能搜索
    │   │    ├── Work      # 应用市场 面向企业办公
    │   ├── App.vue        # 根组件
    │   ├── index.html.ejs # 网页入口
    │   ├── index.js       # 网页逻辑入口
    │   ├── app.html       # 网页入口
    │   ├── router.js      # 路由
    │   ├── main/          # electron
    │   │    ├── index.js  # electron入口
    │   │    ├── index.dev.js  # electron debug入口
    └── test/
    │    ├── e2e/          # 端到端模拟真实环境的测试 nightwatch
    │    └── unit/         # 单元测试 karma
    ├── assets/            # 资源目录，各客户端icon及其他资源
    ├── .babelrc           # js语法转换到ES6的工具
    ├── .editconfig        # 统一编辑器的配置工具
    ├── .eslintignore      # js语法检查中忽略的部分
    ├── .eslintrc.js       # js语法检查工具，不合法会报错
    ├── .gitignore         # git版本控制时忽略的文件和目录
    ├── package.json       # nodejs的npm配置工具
    ├── CHANGELOG.md       # 更改的历史记录
    ├── README.md          # 项目说明入口
    ├── tasks.js           # 常用运行命令入口
    ├── config.js          # 项目配置文件
    ├── webpack.config.js  # webpack配置文件
```

## tips

### 安装elecron
Electron-prebuilt使用 electron-download 来下载 electron 二进制文件并默认缓存在 HOME/.electron/ 目录下。
# .npmrc
registry=https://registry.npm.taobao.org/
disturl=https://npm.taobao.org/dist

export ELECTRON_MIRROR="https://npm.taobao.org/mirrors/electron/"
