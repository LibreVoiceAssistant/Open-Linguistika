// SPDX-FileCopyrightText: 2021 Aditya Mehra <aix.m@outlook.com>
//
// SPDX-License-Identifier: Apache-2.0

import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Layouts 1.12
import org.kde.kirigami 2.11 as Kirigami
import QtQuick.Controls 2.12 as Controls
import Qt.labs.folderlistmodel 2.12

Controls.Popup {
    id: menuPopupArea
    width: parent.width * 0.9
    height:  parent.height * 0.9
    x: (parent.width - width) / 2
    y: (parent.height - height) / 2
    dim: true

    onOpened: {
        if(testModelView.count > 0){
            testModelView.forceActiveFocus()
        }
    }

    Controls.Overlay.modeless: Rectangle {
        color: Qt.rgba(Kirigami.Theme.backgroundColor.r, Kirigami.Theme.backgroundColor.g, Kirigami.Theme.backgroundColor.b, 0.77)
    }

    background: Rectangle {
        Kirigami.Theme.colorSet: Kirigami.Theme.View
        color: Kirigami.Theme.backgroundColor
        border.color: "black"
    }

    FolderListModel {
        id: folderModel
        folder: Qt.resolvedUrl("file:///home/")
        showDirs: true
        showFiles: false
        showHidden: true
        nameFilters: [".*"]
        showDotAndDotDot: false
    }

    FolderListModel {
        id: fileModel
        folder: folderModel.folder
        showDirs: false
        showDotAndDotDot: false
        showOnlyReadable: true
    }

    Item {
        width: parent.width
        anchors.top: parent.top
        anchors.bottom: footerMenuArea.top

        RowLayout {
            id: headerAreaBMLPg1
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: parent.right
            height: Kirigami.Units.gridUnit * 3

            Rectangle {
                color: Kirigami.Theme.backgroundColor
                radius: 10
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.margins: Kirigami.Units.smallSpacing

                Controls.TextField {
                    id: folderModelSearchField
                    anchors.fill: parent
                    anchors.leftMargin: Kirigami.Units.largeSpacing
                    anchors.topMargin: Kirigami.Units.smallSpacing
                    anchors.bottomMargin: Kirigami.Units.smallSpacing
                    anchors.rightMargin: Kirigami.Units.largeSpacing
                    color: Kirigami.Theme.textColor
                    text: folderModel.folder.toString().replace("file://", "")

                    background: Rectangle {
                        color: "transparent"
                    }

                    onAccepted: {
                        folderModel.folder = Qt.resolvedUrl("file://" + text)
                    }
                }
            }

            Controls.Button {
                Layout.preferredWidth: Kirigami.Units.gridUnit * 3
                Layout.fillHeight: true
                Layout.margins: Kirigami.Units.smallSpacing
                icon.name: "arrow-left"
                onClicked: {
                    folderModel.folder = folderModel.parentFolder
                }
            }
        }

        Kirigami.Separator {
            id: headrSeptBml
            anchors.top: headerAreaBMLPg1.bottom
            width: parent.width
            height: 1
        }

        RowLayout {
            anchors.top: headrSeptBml.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            anchors.margins: Kirigami.Units.largeSpacing

            GridView {
                id: testModelView
                model: folderModel
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.margins: Kirigami.Units.largeSpacing
                cellWidth: parent.width / 6
                cellHeight: cellWidth
                keyNavigationEnabled: true
                highlightFollowsCurrentItem: true
                highlightRangeMode: GridView.ApplyRange
                snapMode: GridView.SnapToRow
                cacheBuffer: width
                highlightMoveDuration: Kirigami.Units.longDuration
                clip: true
                enabled: folderModel.count > 0 ? 1 :0
                Controls.ScrollBar.vertical: Controls.ScrollBar {
                    anchors.right: parent.right
                }

                delegate: FileItemDelegate {
                    onClicked: {
                        folderModel.folder = fileUrl
                    }                    
                }

                onCountChanged: {
                    if(testModelView.count > 0){
                        testModelView.forceActiveFocus()
                        testModelView.currentIndex = 0
                    }
                }

                move: Transition {
                    SmoothedAnimation {
                        property: "x"
                        duration: Kirigami.Units.longDuration
                    }
                }
            }
        }
    }

    Item {
        id: footerMenuArea
        width: parent.width
        height: Kirigami.Units.gridUnit * 3
        anchors.bottom: parent.bottom

        Kirigami.Separator {
            anchors.top: parent.top
            anchors.leftMargin: Kirigami.Units.largeSpacing
            anchors.rightMargin: Kirigami.Units.largeSpacing
            height: 1
        }

        RowLayout {
            anchors.top: footerMenuArea.top
            anchors.topMargin: Kirigami.Units.smallSpacing
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: parent.bottom

            Controls.Button {
                Layout.fillWidth: true
                Layout.fillHeight: true
                text: "Cancel"
                onClicked: {
                    menuPopupArea.close()
                }
            }

            Controls.Button {
                Layout.fillWidth: true
                Layout.fillHeight: true
                text: "Select Folder"
                onClicked: {
                    linguistika.setSkillPath(folderModel.folder.toString().replace("file://", ""))
                    menuPopupArea.close()
                }
            }
        }
    }
}
