# SPDX-License-Identifier: BSD-3-Clause
# SPDX-FileCopyrightText: %{CURRENT_YEAR} %{AUTHOR} <%{EMAIL}>

add_executable(helloworld
    main.cpp
    about.cpp
    app.cpp
    cli.cpp
    data.cpp
    resources.qrc)

target_link_libraries(helloworld
    Qt5::Core
    Qt5::Gui
    Qt5::Qml
    Qt5::Quick
    Qt5::QuickControls2
    Qt5::Svg
    KF5::I18n
    KF5::CoreAddons
    KF5::ConfigCore
    KF5::ConfigGui
    KF5::AuthCore
    KF5::Auth)

add_subdirectory(tests)
# link_directories(tests)

kconfig_add_kcfg_files(helloworld GENERATE_MOC helloworldconfig.kcfgc)
install(TARGETS helloworld ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})
