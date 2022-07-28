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
    id: page4
    
    Connections {
        target: linguistikaRoot
        onLReleaseStarted: {
            viewBusyOverlayGeneratedPage.visible = true
        }
        onLReleaseFinished: {
            viewBusyOverlayGeneratedPage.visible = false
            generatedPageFinalMessage.visible = true
        }
    }
    
    Rectangle {
        id: viewBusyOverlayGeneratedPage
        z: 300
        anchors.fill: parent
        visible: false
        enabled: visible
        color: Qt.rgba(0, 0, 0, 0.8)

        BusyIndicator {
            id: viewBusyIndicatorGeneratedPage
            visible: viewBusyOverlayGeneratedPage.visible
            anchors.centerIn: parent
            running: viewBusyOverlayGeneratedPage.visible
            enabled: viewBusyOverlayGeneratedPage.visible

            Label {
                id: viewBusyIndicatorLabelGeneratedPage
                visible: viewBusyOverlayGeneratedPage.visible
                enabled: viewBusyOverlayGeneratedPage.visible
                anchors.top: parent.bottom
                anchors.horizontalCenter: parent.horizontalCenter
                text: "Generating Translation Release Files..."
            }
        }
    }

    Rectangle {
        id: generatedPageFinalMessage
        anchors.centerIn: parent
        width: parent.width * 0.8
        height: Kirigami.Units.gridUnit * 3
        Kirigami.Theme.inherit: false
        Kirigami.Theme.colorSet: Kirigami.Theme.View
        color: Kirigami.Theme.backgroundColor
        radius: 12
        border.width: 1
        border.color: Kirigami.Theme.disabledTextColor
        visible: false

        RowLayout {
            anchors.fill: parent

            Kirigami.Icon {
                Layout.preferredWidth: Kirigami.Units.gridUnit * 2
                Layout.preferredHeight: Kirigami.Units.gridUnit * 2
                Layout.leftMargin: Kirigami.Units.largeSpacing
                source: "checkmark"
                color: Kirigami.Theme.textColor
            }

            Kirigami.Heading {
                Layout.fillWidth: true
                Layout.fillHeight: true                        
                font.bold: true
                font.weight: Font.ExtraBold
                text: qsTr("Success! See the files in the 'ui/translations' folder in your skill directory.")
                color: Kirigami.Theme.textColor
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                elide: Text.ElideMiddle
                level: 2
                maximumLineCount: 1
            }
        }
    }

    Button {
        id: translateAnotherSkillButton
        anchors.top: generatedPageFinalMessage.bottom
        anchors.topMargin: Kirigami.Units.largeSpacing
        anchors.horizontalCenter: parent.horizontalCenter
        width: Kirigami.Units.gridUnit * 12
        height: Kirigami.Units.gridUnit * 3
        icon.name: "view-refresh-symbolic"
        text: qsTr("Translate Another Skill")
        visible: generatedPageFinalMessage.visible ? 1 : 0
        onClicked: {
            linguistikaRoot.translateAnotherSkill()
        }
    }
}