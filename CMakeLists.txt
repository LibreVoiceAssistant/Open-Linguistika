cmake_minimum_required(VERSION 3.16.0)

project(OpenLinguistika LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(QT_MIN_VERSION "5.12.0")

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(AUTORCC_OPTIONS "ui/qml.qrc")

find_package(ECM REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH} ${ECM_KDE_MODULE_DIR} "${CMAKE_SOURCE_DIR}/cmake")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings)
include(ECMInstallIcons)
include(FeatureSummary)

find_package(Qt5 ${QT_MIN_VERSION} REQUIRED COMPONENTS 
    Core
    Qml
    Quick 
    WebEngine 
    Test
    Widgets
    QuickControls2
    Multimedia
    Xml
)

find_package(KF5 ${KF5_MIN_VERSION} REQUIRED 
    Kirigami2
)

qt5_add_resources(open_linguistika_sources "ui/qml.qrc")

add_executable(OpenLinguistika ${open_linguistika_sources}
    main.cpp
    plugins/LibreTranslateProvider.cpp
    plugins/GTranslateProvider.cpp
    plugins/LinguistikaProvider.cpp
    plugins/SkillFileModel.cpp
    plugins/TranslatableItemsModel.cpp
    plugins/SupportedLanguagesModel.cpp
    plugins/GlobalSettings.cpp
    plugins/TranslatedStringsModel.cpp
)

target_link_libraries(OpenLinguistika 
    Qt5::Quick
    Qt5::Widgets
    Qt5::Core 
    Qt5::Qml
    Qt5::QuickControls2
    Qt5::Multimedia
    Qt5::Network
    Qt5::Xml
    KF5::Kirigami2
)

install(TARGETS OpenLinguistika ${INSTALL_TARGETS_DEFAULT_ARGS})
install(PROGRAMS openvoiceos.openlinguistika.desktop DESTINATION ${KDE_INSTALL_APPDIR})
