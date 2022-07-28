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
    id: page3

    Item {
        id: topAreaPage3
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
                icon.name: "arrow-left"
                text: qsTr("Back")

                onClicked: {
                    applicationSwipeView.currentIndex = 1
                }
            }

            Button {
                Layout.preferredWidth: Kirigami.Units.gridUnit * 8
                Layout.fillHeight: true
                Layout.alignment: Qt.AlignRight
                Kirigami.Theme.inherit: true
                Kirigami.Theme.colorSet: Kirigami.Theme.Button
                icon.name: "folder-language"
                text: qsTr("Generate Release")

                onClicked: {
                    applicationSwipeView.currentIndex = 3
                    linguistika.runLupdate()
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

    RowLayout {
        anchors.top: topAreaPage3.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom

        Rectangle {
            id: stringDescriptionItemsArea
            Layout.fillWidth: true
            Layout.fillHeight: true
            Kirigami.Theme.inherit: false
            Kirigami.Theme.colorSet: Kirigami.Theme.View
            color: Kirigami.Theme.backgroundColor

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
                    id: translatedStringsModelListView
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    model: linguistika.translatedStringsModel
                    property var secondaryModel
                    ScrollBar.vertical: ScrollBar {}

                    delegate: Kirigami.BasicListItem {
                        text: model.source

                        onClicked: {
                            translatedStringsModelListView.secondaryModel = model.translation
                        }
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

            ColumnLayout {
                anchors.fill: parent
                spacing: 10

                Rectangle {
                    Layout.fillWidth: true
                    Layout.preferredHeight: Kirigami.Units.gridUnit * 3
                    color: Kirigami.Theme.backgroundColor

                    Kirigami.Heading {
                        text: qsTr("Translated Strings")
                        anchors.centerIn: parent
                        level: 2
                        color: Kirigami.Theme.textColor
                    }
                }

                ListView {
                    id: stringsTranslationsModelView
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    Layout.margins: Kirigami.Units.largeSpacing
                    spacing: Kirigami.Units.largeSpacing
                    model: translatedStringsModelListView.secondaryModel
                    ScrollBar.vertical: ScrollBar {}
                    clip: true
                    
                    delegate: Kirigami.AbstractListItem {
                        width: stringsTranslationsModelView.width
                        height: Kirigami.Units.gridUnit * 4

                        background: Rectangle {
                            color: Kirigami.Theme.backgroundColor
                            border.color: Kirigami.Theme.disabledTextColor
                            border.width: 1
                            Kirigami.Theme.inherit: false
                            radius: 6
                        }

                        contentItem: ColumnLayout {
                            Label {
                                text: modelData.code
                                font.capitalization: Font.Capitalize
                                Layout.fillWidth: true                  
                                Layout.fillHeight: true
                            }
                            TextField {
                                id: stringTranslationTextField
                                text: modelData.translatedString
                                Layout.fillWidth: true
                                Layout.fillHeight: true

                                onTextChanged: {
                                    var stringToChange = linguistika.translatedStringsModel.get(translatedStringsModelListView.currentIndex).source
                                    linguistika.translatedStringsModel.updateString(stringToChange, modelData.code, stringTranslationTextField.text)
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}