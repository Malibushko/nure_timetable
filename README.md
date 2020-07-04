
# NURE TimeTable

> Simple application for viewing timetable using NURE API and C++

[![Look of application](https://github.com/Malibushko/nure_timetable/blob/master/gitimg/preview.jpg)]()

## Table of Contents

- [Requirements](#requirements)
- [Build instructions](#build)
- [Plans for future](#plans)
- [Prebuilt packages](#packages)
- [Feedback](#feedback)
- [License](#license)

---

## Requirements  <a name="Requirements"></a>

- C++17 Compiler  
- Conan  
- CMake 3.5 or higher  
- Qt5 version 5.12 or higher  

---

## Build instructions <a name="build"></a>

I highly recommend you to use some IDE for building (especially for mobile devices)  

But there are steps for building by hands:

- 1) Add conan repository for third-party libs as following  
```
conan remote add <some_name> https://api.bintray.com/conan/yehorka9991/magic_get  
```  

 2) Build application with CMake

### Desktop platforms

`cd ~/nure_timetable`  
`mkdir build && cd build`  
`cmake -DCMAKE_BUILD_TYPE=RELEASE ..`  
`cmake --build`  

### Android

To build for android, you should specify some additional   
variables. See all of them <a href="https://doc.qt.io/archives/qt-5.13/android-building.html">here</a>  
You should also install Android SDK and NDK  

`cd ~/nure_timetable`  
`mkdir build && cd build`  
`cmake -DCMAKE_BUILD_TYPE=RELEASE -DANDROID=ON ..`  
`cmake --build`  

 Next you can deploy the application to any platform  
 supported by Qt, using `<mac/win/android/linux>deployqt` tool  

### IOS

To make IOS application you should generate an XCode project  
from CMakeLists.txt. To do this use the following commands  
```
cmake -G Xcode path_to_project  
```
Next build the application as in XCode as usually  

---

## Plans for future  <a name="plans"></a>  

 - Push notifications for Desktop and mobile platforms
 - Extending functionality for particular lessons (e.g showing all groups)
 - Improvements in design and perfomance
 - More prebuilt packages

---

## Prebuilt packages  <a name="packages"></a>  

You can see the list of prebuilt packages <a href="https://github.com/Malibushko/nure_timetable/releases/tag/1">here</a>  
More are coming in the future

---

## Feedback  <a name="feedback"></a>  

I will accept any criticism and comments.  
If you encounter a problem, leave the issue on the GitHub, or write to me in Telegram @Malbu0698  

---

## License  <a name="license"></a>  

I am using Qt so the license is LGPL

- **[LGPL ](http://opensource.org/licenses/mit-license.php)**

---
