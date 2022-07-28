// SPDX-FileCopyrightText: 2021 Aditya Mehra <aix.m@outlook.com>
//
// SPDX-License-Identifier: Apache-2.0

import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import Qt.labs.platform 1.1
import org.kde.kirigami 2.11 as Kirigami

import Linguistika 1.0

Page {
    id: page1

    Item {
        id: topArea
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        height: Kirigami.Units.gridUnit * 5 
        
        RowLayout {
            anchors.fill: parent
            anchors.margins: Kirigami.Units.largeSpacing

            Button {
                Layout.preferredWidth: Kirigami.Units.gridUnit * 8
                Layout.fillHeight: true
                Kirigami.Theme.inherit: true
                Kirigami.Theme.colorSet: Kirigami.Theme.Button
                icon.name: "folder-open-symbolic"
                text: qsTr("Select Skill")

                onClicked: {
                    skillSelectorDialog.open()
                }
            }

            Rectangle {
                Layout.fillWidth: true
                Layout.preferredHeight: Kirigami.Units.gridUnit * 2.5
                Layout.alignment: Qt.AlignVCenter
                Kirigami.Theme.inherit: false
                Kirigami.Theme.colorSet: Kirigami.Theme.View
                color: Kirigami.Theme.backgroundColor
                radius: 12
                border.width: 1
                border.color: Kirigami.Theme.disabledTextColor

                Label {
                    anchors.fill: parent
                    text: linguistika.skillPath != "" ? linguistika.skillPath : qsTr("No Skill Folder Selected") 
                    color: Kirigami.Theme.textColor
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    elide: Text.ElideMiddle
                    maximumLineCount: 1
                }
            }

            Button {
                Layout.preferredWidth: Kirigami.Units.gridUnit * 8
                Layout.fillHeight: true
                Kirigami.Theme.inherit: true
                Kirigami.Theme.colorSet: Kirigami.Theme.Button
                icon.name: "folder-language"
                text: qsTr("Select Languages")
                enabled: skillTranslatableStringsListView.count > 0

                onClicked: {
                    applicationSwipeView.currentIndex = 1
                }
            }
        }

        Kirigami.Separator {
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            height: 1
            color: Kirigami.Theme.disabledTextColor
        }
    }

    Rectangle {
        id: noSkillSelectedLabelItem
        anchors.centerIn: parent
        width: parent.width * 0.8
        height: Kirigami.Units.gridUnit * 3
        Kirigami.Theme.inherit: false
        Kirigami.Theme.colorSet: Kirigami.Theme.View
        color: Kirigami.Theme.backgroundColor
        radius: 12
        border.width: 1
        border.color: Kirigami.Theme.disabledTextColor
        visible: !skillDescriptionItemUiFilesArea.visible

        RowLayout {
            anchors.fill: parent

            Kirigami.Icon {
                Layout.preferredWidth: Kirigami.Units.gridUnit * 2
                Layout.preferredHeight: Kirigami.Units.gridUnit * 2
                Layout.leftMargin: Kirigami.Units.largeSpacing
                source: "folder-language"
                color: Kirigami.Theme.textColor
            }

            Kirigami.Heading {
                Layout.fillWidth: true
                Layout.fillHeight: true                        
                font.bold: true
                font.weight: Font.ExtraBold
                text: qsTr("Select a skill to start translating")
                color: Kirigami.Theme.textColor
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                elide: Text.ElideMiddle
                level: 2
                maximumLineCount: 1
            }
        }
    }

    RowLayout {
        anchors.top: topArea.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom

        Rectangle {
            id: skillDescriptionItemUiFilesArea
            Layout.fillWidth: true
            Layout.fillHeight: true
            Kirigami.Theme.inherit: false
            Kirigami.Theme.colorSet: Kirigami.Theme.View
            color: Kirigami.Theme.backgroundColor
            visible: linguistika.skillPath != "" ? true : false

            ColumnLayout {
                anchors.fill: parent
                spacing: 10

                Rectangle {
                    Layout.fillWidth: true
                    Layout.preferredHeight: Kirigami.Units.gridUnit * 3
                    color: Kirigami.Theme.backgroundColor

                    Kirigami.Heading {
                        text: qsTr("Skill UI Files")
                        anchors.centerIn: parent
                        level: 2
                        color: Kirigami.Theme.textColor
                    }
                }

                ListView {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    model: linguistika.skillFileModel
                    ScrollBar.vertical: ScrollBar {}
                    delegate: Kirigami.BasicListItem {
                        text: model.skillFileName
                    }
                }
            }
        }

        Rectangle {
            Layout.fillWidth: true
            Layout.fillHeight: true
            Kirigami.Theme.inherit: false
            Kirigami.Theme.colorSet: Kirigami.Theme.View
            color: Kirigami.Theme.backgroundColor
            visible: linguistika.skillPath != "" ? true : false

            ColumnLayout {
                anchors.fill: parent
                spacing: 10

                Rectangle {
                    Layout.fillWidth: true
                    Layout.preferredHeight: Kirigami.Units.gridUnit * 3
                    color: Kirigami.Theme.backgroundColor

                    Kirigami.Heading {
                        text: qsTr("Translatable Strings")
                        anchors.centerIn: parent
                        level: 2
                        color: Kirigami.Theme.textColor
                    }
                }

                ListView {
                    id: skillTranslatableStringsListView
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    ScrollBar.vertical: ScrollBar {}
                    model: linguistika.skillFileModel.translatableItemsModel

                    delegate: Kirigami.BasicListItem {
                        text: model.text
                    }
                }
            }
        }
    }
}