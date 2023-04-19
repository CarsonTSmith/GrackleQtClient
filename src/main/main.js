const {app, BrowserWindow, ipcMain} = require('electron');
const path = require('path');
const net  = require('net');

let win;

let client = new net.Socket();

//const primary_screen     = screen.getPrimaryDisplay();
//const dimensions         = primary_screen.size;

const create_connect_win = () => {
  const connect_win_width  = 0.5 * 1920;
  const conenct_win_height = 0.7 * 1080;
  win = new BrowserWindow({
    webPreferences: {
      nodeIntegration: true,
      preload: path.join(__dirname + '/preloads/', 'connect_preload.js')
    },
    width: connect_win_width,
    height: conenct_win_height,
    //autoHideMenuBar: true,
  });

  ipcMain.on('app: connect_to_server', function(event, socket_info) {
    console.log(socket_info);
    connect_to_server(socket_info.ip, socket_info.port);
  });

  win.loadFile('src/components/connect/index.html');
}

app.whenReady().then(() => {
  create_connect_win();

  app.on('activate', () => {
      if (BrowserWindow.getAllWindows().length === 0)
        create_connect_win()
  });

})

function connect_to_server(ip, port) 
{
  client.connect(port, ip);

  client.on('error', function(e) {
    win.webContents.send('cant_connect_to_server');
    console.log('cant connect to server');
  });

  client.on('connect', function(e) {
    console.log('connected to server');
    win.setSize(1920, 1080);
    win.loadFile('src/components/chatroom/html/index.html');
  });

  client.on('data', function() {
  });
}