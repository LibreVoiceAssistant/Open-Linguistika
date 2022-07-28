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
    id: page2
    
    Connections {
        target: linguistikaRoot
        onTranslationsGenerated: {
            viewBusyOverlay.visible = false
            applicationSwipeView.currentIndex = 2
        }
    }
    
    Rectangle {
        id: viewBusyOverlay
        z: 300
        anchors.fill: parent
        visible: false
        enabled: visible
        color: Qt.rgba(0, 0, 0, 0.8)

        BusyIndicator {
            id: viewBusyIndicator
            visible: viewBusyOverlay.visible
            anchors.centerIn: parent
            running: viewBusyOverlay.visible
            enabled: viewBusyOverlay.visible

            Label {
                id: viewBusyIndicatorLabel
                visible: viewBusyOverlay.visible
                enabled: viewBusyOverlay.visible
                anchors.top: parent.bottom
                anchors.horizontalCenter: parent.horizontalCenter
                text: "Loading Translations..."
            }
        }
    }


    Item {
        id: topAreaPage2
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
                    applicationSwipeView.currentIndex = 0
                }
            }

            Button {
                Layout.preferredWidth: Kirigami.Units.gridUnit * 10
                Layout.fillHeight: true
                Layout.alignment: Qt.AlignRight
                Kirigami.Theme.inherit: true
                Kirigami.Theme.colorSet: Kirigami.Theme.Button
                icon.name: "svn-update"
                text: qsTr("Generate Translations")
                enabled: providersButtonGroup.checkedButton != null

                onClicked: {
                    viewBusyOverlay.visible = true
                    linguistika.generateTranslations()
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
        anchors.top: topAreaPage2.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom

        Rectangle {
            Layout.fillWidth: true
            Layout.fillHeight: true
            Kirigami.Theme.inherit: false
            Kirigami.Theme.colorSet: Kirigami.Theme.View
            color: Kirigami.Theme.backgroundColor

            Rectangle {
                id: selectLanguagesHeaderItemRowLayout
                anchors.top: parent.top
                anchors.left: parent.left
                anchors.right: parent.right
                height: Kirigami.Units.gridUnit * 3
                Kirigami.Theme.inherit: false
                color: Kirigami.Theme.backgroundColor

                Kirigami.Heading {
                    text: qsTr("Available Languages")
                    anchors.centerIn: parent
                    level: 2
                    color: Kirigami.Theme.textColor
                }
            }
            
            ColumnLayout {
                id: languageSelectorsLayout
                anchors.top: selectLanguagesHeaderItemRowLayout.bottom
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.margins: Kirigami.Units.largeSpacing
                spacing: Kirigami.Units.largeSpacing

                Repeater {
                    id: languageRepeater
                    model: linguistika.supportedLanguagesModel
                    delegate: CheckBox {
                        text: model.languageName
                        Layout.fillWidth: true
                        Layout.alignment: Qt.AlignHCenter

                        onCheckedChanged: {
                            if (checked) {
                                linguistika.setTargetLanguage(model.languageCode)
                            } else {
                                linguistika.removeTargetLanguage(model.languageCode)
                            }
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

            ButtonGroup {
                id: providersButtonGroup
                buttons: providersColumnLayout.children
            }

            Rectangle {
                id: selectProvidersHeaderItemRowLayout
                anchors.top: parent.top
                anchors.left: parent.left
                anchors.right: parent.right
                height: Kirigami.Units.gridUnit * 3
                Kirigami.Theme.inherit: false
                color: Kirigami.Theme.backgroundColor

                Kirigami.Heading {
                    text: qsTr("Available Providers")
                    anchors.centerIn: parent
                    level: 2
                    color: Kirigami.Theme.textColor
                }
            }

            ColumnLayout {
                id: providersColumnLayout
                anchors.top: selectProvidersHeaderItemRowLayout.bottom
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.margins: Kirigami.Units.largeSpacing
                spacing: Kirigami.Units.largeSpacing

                RadioButton {
                    id: radioButtonGoogle
                    text: qsTr("G Translate Service")
                    onCheckedChanged: {
                        if (checked) {
                            linguistika.setTranslationProvider("gTranslate")
                        } 
                    }
                }

                RadioButton {
                    id: radioButtonMicrosoft
                    text: qsTr("Libre Translate Service")
                    onCheckedChanged: {
                        if (checked) {
                            linguistika.setTranslationProvider("libreTranslate")
                        } 
                    }
                }
                RadioButton {
                    id: radioButtonYandex
                    text: qsTr("Manual Translation")
                    onCheckedChanged: {
                        if (checked) {
                            linguistika.setTranslationProvider("manualTranslate")
                        } 
                    }
                }
            }
        }
    }
}