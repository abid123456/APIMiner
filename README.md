
           d8888 8888888b. 8888888 888b     d888 d8b
          d88888 888   Y88b  888   8888b   d8888 Y8P
         d88P888 888    888  888   88888b.d88888
        d88P 888 888   d88P  888   888Y88888P888 888 88888b.   .d88b.  888d88
       d88P  888 8888888P"   888   888 Y888P 888 888 888  88b d8P  Y8b 888P
      d88P   888 888         888   888  Y8P  888 888 888  888 88888888 888
     d8888888888 888         888   888   "   888 888 888  888 Y8b.     888
    d88P     888 888       8888888 888       888 888 888  888  "Y8888  888

APIMiner v1.0.0: API Logger for Windows Executables

Original developers: Anoop Saldanha, Abhijit Mohanta

Forker: abid123456

Prerequisites:
- Windows x64 machine with MSYS2, MinGW32, MinGW64, and Python 2 installed
- Add MinGW binary and library directories to PATH
- Install pip by doing
```
curl https://bootstrap.pypa.io/pip/2.7/get-pip.py -o get-pip.py
python2 get-pip.py
```
- Install setuptools in MSYS by doing
```
pacman -S mingw-w64-x86_64-python-setuptools
```
- Install docutils, jinja2, and pyyaml (in MSYS2 MINGW64 mode):
```
python2 -m pip install docutils jinja2 pyyaml
```

## How to compile:
### Windows
1. Start by executing `mingw32-make` command on Windows command prompt (cmd) in the project directory
2. In the same directory, execute `make` on MSYS2 MINGW64
3. Now execute `make` on MSYS2 MINGW32
4. Repeat steps 1 and 2 once

The binaries can then be found in `./bin/` folder.

Have fun!

### Linux (Debian/Ubuntu)
Make sure you have python2 installed

1. Install the cross compiler
```bash
sudo apt install gcc-mingw-w64-i686 gcc-mingw-w64-x86-64
```
2. Now build the program 
```bash
make
```