# Pamac-Qt (WIP)
This repo contains a list of linked projects for implementation of libalpm and AUR Qt GUI via libpamac

### libqpamac

Wrapper library for libpamac interaction with Qt/QML

### NotificationService

Classes that expose various DE-specific notification/progress features to QML

### Pamac-Qt App

Qt GUI Frontend for libpamac that supports managing ALPM packages and AUR building pipeline

## Building

To build the application from source you need CMake, Qt5 and libpamac (contained in `pamac-common` package)

```
mkdir build
cd build
cmake ..
make
```
