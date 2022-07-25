// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: %{CURRENT_YEAR} %{AUTHOR} <%{EMAIL}>

import QtQuick 2.15
import QtQuick.Controls 2.15 as Controls
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.15
import org.kde.kirigami 2.19 as Kirigami
import org.kde.grubeditor 1.0
Kirigami.ApplicationWindow {

    id: root

    title: i18n("Grub Editor")

    minimumWidth: Kirigami.Units.gridUnit * 40
    minimumHeight: Kirigami.Units.gridUnit * 20

    height:Kirigami.Units.gridUnit * 22
    width:Kirigami.Units.gridUnit * 45


    onClosing: Controller.saveWindowGeometry(root)

    onWidthChanged: saveWindowGeometryTimer.restart()
    onHeightChanged: saveWindowGeometryTimer.restart()
    onXChanged: saveWindowGeometryTimer.restart()
    onYChanged: saveWindowGeometryTimer.restart()


    // This timer allows to batch update the window size change to reduce
    // the io load and also work around the fact that x/y/width/height are
    // changed when loading the page and overwrite the saved geometry from
    // the previous session.
    Timer {
        id: saveWindowGeometryTimer
        interval: 1000
        onTriggered: Controller.saveWindowGeometry(root)
    }

    property int counter: 0

    globalDrawer: Kirigami.GlobalDrawer {
        title: i18n("grubeditor")
        titleIcon: "applications-graphics"
        isMenu: !root.isMobile
        actions: [

            Kirigami.Action {
                text: i18n("About Grub Editor")
                icon.name: "help-about"
                onTriggered: pageStack.layers.push('qrc:About.qml')
            },
            Kirigami.Action {
                text: i18n("Quit")
                icon.name: "application-exit"
                onTriggered: Qt.quit()
            }
        ]
    }

    contextDrawer: Kirigami.ContextDrawer {
        id: contextDrawer
    }

    pageStack.initialPage: page

    Kirigami.Page {
        id: page

        Layout.fillWidth: true

//         title: i18n("Main Page")



            ColumnLayout{
                width:page.width

                Kirigami.FormLayout {

                    width: page.width

                    Layout.alignment:Qt.AlignHCenter
    //                 anchors.centerIn: parent
                    Controls.ComboBox{

                        Kirigami.FormData.label:"Loaded configuration from:"
                        model:["/etc/default/grub","snapshot1"]
                    }

                    Kirigami.Separator{
                        Kirigami.FormData.isSection:true
                    }




                    RowLayout{
                        Kirigami.FormData.label:"default entry:"

                        Controls.RadioButton{
                            id:rb_pre_defined
                            checked:Data.defaultEntryType==Data.Predefined
                            text:"predefined"

                            onClicked: {
                            if(checked){
                                rb_previously_booted.checked=false
                            }
                        }
                        }
                        Controls.ComboBox{
                            id:cob_os_entries

                            model:Data.osEntries

                        }

                    }

                    Controls.RadioButton{
                        id:rb_previously_booted
                        text:"previously booted entry"
                        checked:Data.defaultEntryType==Data.PreviouslyBooted
                        onClicked: {
                            if(checked){
                                rb_pre_defined.checked=false
                            }
                        }

                    }
                    Item{
                        Layout.preferredHeight:Kirigami.Units.largeSpacing
                    }

                    RowLayout{

                        Kirigami.FormData.label:"Visibility:"

                        Controls.CheckBox{
                            id:chb_bootDefault

                            text:"Boot default entry after"
                            checked:Data.bootDefault
                        }
                        Controls.SpinBox{
                            value:Data.grubTimeout


                        }
                        Controls.Label{
                            text:"seconds"
                        }
                    }
                    Controls.CheckBox{
                        id:chb_showMenu
                        text:"show menu"
                        checked:Data.showMenu

                        Component.onCompleted :{
                            console.log("done")
                            console.log(Data.showMenu)

                        }

                    }
                    Item{

                        height:Kirigami.Units.largeSpacing

                    }

                    Controls.CheckBox{
                        id:chb_lookForOtherOs
                        text:"look for other Operating Systems"
                        checked:Data.lookForOtherOs
                    }





                }//FormLayout

                Item{

                        height:Kirigami.Units.largeSpacing

                    }


                RowLayout{

                        //Layout.alignment:Qt.AlignRight
                        Layout.alignment:Qt.AlignHCenter

    //                     Layout.fillWidth:true
                        width:page.width
                        Layout.rightMargin:Kirigami.Units.largeSpacing*5





                        Controls.Button {
                            //Layout.alignment: Qt.AlignHCenter
                            text: "Reset"

                        }//Button

                        Controls.Button {
                            //Layout.alignment: Qt.AlignHCenter
                            text: "Set"
                            onClicked:{Data.set(cob_os_entries.currentText,chb_showMenu.checked)}

                        }//Button

                }//RowLayout
                //}//Rectangle
            }//ColumnLayout

    }//Kirigami.Page
}//Kirigami.Application
