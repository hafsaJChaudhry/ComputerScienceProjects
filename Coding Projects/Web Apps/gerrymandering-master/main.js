const {app, BrowserWindow, Menu, ipcMain} = require('electron');
const path = require('path');
const url = require('url');
const shell = require('electron').shell;
const child = require('child_process')

// Keep a global reference of the window object, if you don't, the window will
// be closed automatically when the JavaScript object is garbage collected.
let mainWindow;
/*
require('electron-reload')(__dirname, {
   electron: path.join(__dirname, 'node_modules', '.bin', 'electron')
 });*/

function createWindow(){
  // Create the browser window.
    mainWindow = new BrowserWindow({
        width: 1920,
        height: 1080,
        webPreferences: {
          nodeIntegration: true
        }
    });
    Menu.setApplicationMenu(null);
    mainWindow.maximize();
    //mainWindow.webContents.openDevTools();

    //display the index webpage
    mainWindow.loadURL(url.format({
        pathname: path.join(__dirname, 'src/index.html'),
        protocol: 'file:',
        slashes: true
    }));

    mainWindow.on('closed', function () {
        // Dereference the window object, usually you would store windows
        // in an array if your app supports multi windows, this is the time
        // when you should delete the corresponding element.
        mainWindow = null
    });
}

// This method will be called when Electron has finished
// initialization and is ready to create browser windows.
// Some APIs can only be used after this event occurs.
app.on('ready', createWindow)

// Quit when all windows are closed.
app.on('window-all-closed', function () {
  // On macOS it is common for applications and their menu bar
  // to stay active until the user quits explicitly with Cmd + Q
  if (process.platform !== 'darwin'){
      app.quit();
  }
})

app.on('activate', function () {
  // On macOS it's common to re-create a window in the app when the
  // dock icon is clicked and there are no other windows open.
  if (mainWindow === null) {
      createWindow();
  }
})

ipcMain.handle("call_algo", async () =>{
  //EXE version:
  return app.getAppPath()

  //NPM RUN START version
  //return path.join(__dirname, "src/data_files/")
})
