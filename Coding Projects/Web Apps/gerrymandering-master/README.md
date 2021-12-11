# Hyperion

### Members
+ Michael Sachs	(Trilliante)
+ Christopher Skane	(coderBlitz)
+ Hafsa Chaudry	(hafsaJChaudry)
+ Enzo Walker	(enzow1)
+ Chigozie Ewulum	(Chi-Says-Hi)

### Where's the Code?
Due to the large amount of configuration files required for compiling and running the project, I've provided a guide to show where the important code is:
+ In the root directory, the [main.js](main.js) contains the code required for Electron to run as the application.
+ Inside the src directory:
  + The [index.html](src/index.html), [index.js](src/index.js), and [mainstyles.css](src/mainstyles.css) files contains the code for what is displayed.
  + Inside the data_files directory are the folders for Algorithm 1 and Algorithm 2. These folders contain the python code for running the actual algorithms, and the data required to run them. The "custom" directory is code from a redistricting algorithm attempt that failed.

### How to run the EXE
+ Follow the installation and setup procedures described in the Software Users Manual. The software inventory and environment must be present before the installation steps can be completed.
+ Download the The Gerrymandering App-win32-ia32 folder from the release-builds directory.
+ Copy the downloaded folder into the installation directory.
+ You can either run the executable directly from the folder, or create a desktop shortcut to it by right clicking on the executable and selecting “Send to > Desktop (Create Shortcut)”.

<br>

### How to get the code to work for development:
Download & Install [node.js (12.16.1 LTS)](https://nodejs.org/en/) or whatever the current LTS version is. During the install, on one screen it will ask if you want `npm package manager` and `add to PATH`. Make sure they are selected. They should be installed by default.

Go to the location you want to download this directory, and use git to clone this repository, `git clone https://github.com/Trilliante/gerrymandering` should work.

Go into the directory and run the following commands. `npm install` and `npm install --dev`. Now you should be able to run the application, as it is, via `npm run start`. If you're on windows, you should be able to run `npm run package-win` to create the EXE, but you don't need to to this yet.


**Note:** For development, you must be in `npm run start` mode. For building the exe, you must be in `EXE` mode. **By default, the program is in `EXE` mode.** This is accomplished via commenting out a few file paths in three different files:
+ [main.js](main.js) must be modified. For `EXE` mode, line 67 must be uncommented and line 70 must be commented out. For `npm run start` mode, line 67 must be commented out and line 70 must be uncommented.
+ [redistricter.py](src/data_files/Algorithm1/redistricter.py) must be modified. For `EXE` mode, line 11 must be uncommented and line 13 must be commented out. For `npm run start` mode, line 11 must be commented out and line 70 must be uncommented.
+ [k_means_bfs.py](src/data_files/Algorithm2/k_means_bfs.py) must be modified. For `EXE` mode, line 18 must be uncommented and line 20 must be commented out. For `npm run start` mode, line 18 must be commented out and line 20 must be uncommented.