# Generated from YAC_Commander.pro.

cmake_minimum_required(VERSION 3.16)
project(YAC_Commander VERSION 1.0 LANGUAGES CXX)



set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

if(NOT DEFINED INSTALL_EXAMPLESDIR)
    set(INSTALL_EXAMPLESDIR "examples")
endif()


find_package(QT NAMES Qt5 REQUIRED COMPONENTS Core)
find_package(Qt5 REQUIRED COMPONENTS Gui Network Widgets)

message("${CMAKE_SOURCE_DIR}")
set(CMAKE_AUTOUIC_SEARCH_PATHS ${CMAKE_SOURCE_DIR})

set(PROJECT_SOURCES
    Delegates/FileMoverDelegate.cpp
    Delegates/FileMoverDelegate.hpp
    Delegates/TableItemDelegate.cpp
    Delegates/TableItemDelegate.hpp
    Dialogs/FileProgressDialog.cpp
    Dialogs/FileProgressDialog.hpp
    Dialogs/FileSearchDlg.cpp
    Dialogs/FileSearchDlg.hpp
    Dialogs/NewDirDlg.cpp
    Dialogs/NewDirDlg.hpp
    Dialogs/NewFileDlg.cpp
    Dialogs/NewFileDlg.hpp
    Dialogs/TextViewerDlg.cpp
    Dialogs/TextViewerDlg.hpp
    MainWindow.cpp
    MainWindow.hpp
    Menus/ItemContextMenu.cpp
    Menus/ItemContextMenu.hpp
    Models/FileFindingsModel.cpp
    Models/FileFindingsModel.hpp
    Models/OrderedFileSystemModel.cpp
    Models/OrderedFileSystemModel.hpp
    Views/FileTableView.cpp
    Views/FileTableView.hpp
    Views/QFastView.cpp
    Views/QFastView.hpp
    Views/QHexView.cpp
    Views/QHexView.hpp
    Widgets/CommandDropdown.cpp
    Widgets/CommandDropdown.hpp
    Widgets/EditableDropdown.cpp
    Widgets/EditableDropdown.hpp
    Widgets/FileTabSelector.cpp
    Widgets/FileTabSelector.hpp
    Widgets/QuickAccessBar.cpp
    Widgets/QuickAccessBar.hpp
    main.cpp
    mainwindow.ui
    progressDialog.ui
    searchdialog.ui
    textviewer.ui
)

add_executable(YAC_Commander
     ${PROJECT_SOURCES}
)
set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "yc")
if(UNIX)
    find_package(KF5KIO)
    find_package(KF5Archive)
    find_package(KF5CoreAddons)

    target_link_libraries(YAC_Commander PUBLIC
        KF5Archive
        KF5CoreAddons
        KF5::KIOCore
        KF5::KIOFileWidgets
        KF5::KIOWidgets
        KF5::KIONTLM
        Qt::Core
        Qt::Gui
        Qt::Network
        Qt::Widgets
    )

set (DESKTOP
        yc.desktop)

    set (ICONS
            TC_QT.png)
else()
    target_link_libraries(YAC_Commander PUBLIC
        Qt::Core
        Qt::Gui
        Qt::Network
        Qt::Widgets
    )
endif()

if(CMAKE_BUILD_TYPE STREQUAL Debug AND LINUX)
    target_compile_options(YAC_Commander
        -fsanitize=address
    )
endif()

install(TARGETS YAC_Commander
    RUNTIME DESTINATION "/usr/local/bin"
)

install(FILES "${CMAKE_SOURCE_DIR}/yc.desktop"
     DESTINATION "/usr/share/applications"
)

install(FILES "${CMAKE_SOURCE_DIR}/TC_QT.png"
     DESTINATION "/usr/share/icons/hicolor/32x32/apps/"
)

#execute_process(COMMAND ${XDG-DESKTOP-MENU_EXECUTABLE} install
# yc.desktop "/usr/share/applications")
