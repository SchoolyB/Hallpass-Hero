# Introduction

This README consists of two sections. The first will contain installation/cloning instructions for developers. The second section will contain the documentation for Hallpass-Hero.


## Cloning & Initial Setup

For developers there a few things to note:

- You must be running some flavor of Linux
- You must have [CMake](https://cmake.org/) installed
- You must have [SQLite3](https://www.sqlite.org/index.html) installed
    - If you run into problems with CMake not finding SQLite3 when compiling, see [Extra Info](#extra-info)

- For contributing to the project, please see the [CONTRIBUTING.md](CONTRIBUTING.md) file for guidelines on making pull requests and issues.


#### Step 1
Clone the repository

```
git clone https://github.com/SchoolyB/Hallpass-Hero.git  
```

#### Step 2
Navigate into the projects root directory

```sh
cd path/to/your/project/Hallpass-hero
```

#### Step 3
Make a new directory called 'build' then navigate into it

```sh
mkdir build && cd build
```
#### Step 4
Use the following commands in order to create the executable

```sh
cmake ..
```

```sh
make
```

#### Step 5
Run the executable
```sh
./Hallpasshero.exe
```

### Running The Program After Initial Setup
After the initial setup is complete you can freely change code. To run the program after you have made changes to the following.

1. Run `make` to build the program and create the executable

2. Run `./Hallpasshero.exe` to run the program

<i>Alternatively</i> you can run `make && ./Hallpasshero.exe` to do both!


## Extra Info

### CMake Cannot Find SQLite3

When trying to build the program you might get thrown the following errors:
```
errorCMake Error at /usr/share/cmake-3.22/Modules/FindPackageHandleStandardArgs.cmake:230 (message):
Could NOT find SQLite3 (missing: SQLite3_INCLUDE_DIR SQLite3_LIBRARY)

Call Stack (most recent call first):
/usr/share/cmake-3.22/Modules/FindPackageHandleStandardArgs.cmake:594 (_FPHSA_FAILURE_MESSAGE)
/usr/share/cmake-3.22/Modules/FindSQLite3.cmake:54 (find_package_handle_standard_args)
CMakeLists.txt:7 (find_package)
```

This happens when CMake is not able to find SQLite3 during the build process.

For Debian-based systems use the following command:
```sh
sudo apt-get install libsqlite3-dev
``` 

For Red Hat-based systems you could try
```sh
sudo yum install sqlite-devel
```

For Arch Linux you could try
```sh
sudo pacman -S sqlite
```

After running your respective command try building and running the program again.