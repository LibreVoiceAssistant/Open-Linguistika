# Open Linguistika

Open Linguistika Tool allows Mycroft skills developers to easily add support for newer languages to their skill GUI user interface.

### Requirements:
 - Qt Version: 5.12
 - Qt Labs Platform QML Module
 - KDE Kirigami
 - Cmake
 - Extra Cmake Modules

### Build Instructions:
- dev_setup.sh script coming soon :)
- To manually build and run Open Linguistika:

```
git clone https://github.com/OpenVoiceOS/Open-Linguistika
cd Open-Linguistika
mkdir build
cd build
cmake .. -DCMAKE_INSTALL_PREFIX=/usr -DCMAKE_BUILD_TYPE=Release -DKDE_INSTALL_LIBDIR=lib -DKDE_INSTALL_USE_QT_SYS_PATHS=ON
make
sudo make install
```

### Running:
- From Launcher: Open Linguistika
- Any Terminal: OpenLinguistika

### Add More Languages:
- Start OpenLinguistika once
- Edit ~/config/linguistika.json
- Under Languages List Add For Example:

```
{
    "code": "nl",
    "name": "Dutch"
}
```

- Save the json file
- Restart OpenLinguistika
