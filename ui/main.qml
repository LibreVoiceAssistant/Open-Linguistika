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

Kirigami.AbstractApplicationWindow {
    id: linguistikaRoot
    width: 960
    height: 720
    visible: true
    title: qsTr("Open Linguistika")
    signal translationsGenerated()
    signal lReleaseStarted()
    signal lReleaseFinished()

    function translateAnotherSkill() {
        linguistika.restart()
        applicationSwipeView.currentIndex = 0
    }

    LinguistikaProvider {
        id: linguistika

        onGenerateTranslationsFinished: {
            console.log("Generate translations finished")
            linguistikaRoot.translationsGenerated()
        }
        onLreleaseStarted: {
            console.log("Lrelease started")
            linguistikaRoot.lReleaseStarted()
        }
        onLreleaseFinished: {
            console.log("Lrelease finished")
            linguistikaRoot.lReleaseFinished()
        }
    }

    FolderMenu {
        id: skillSelectorDialog
    }

    Rectangle {
        id: applicationHeader
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        height: Kirigami.Units.gridUnit * 2
        Kirigami.Theme.inherit: false
        Kirigami.Theme.colorSet: Kirigami.Theme.View
        color: Kirigami.Theme.backgroundColor

        Item {
            id: applicationHeaderLayout
            anchors.fill: parent
            anchors.margins: 1

            Kirigami.Icon {
                id: applicationHeaderIcon
                source: "applications-education-language"
                width: Kirigami.Units.iconSizes.medium
                height: Kirigami.Units.iconSizes.medium
                anchors.verticalCenter: parent.verticalCenter
                anchors.right: applicationHeaderTitle.left
                anchors.rightMargin: Kirigami.Units.gridUnit
            }

            Kirigami.Heading {
                id: applicationHeaderTitle
                level: 1
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignLeft
                text: qsTr("Open Linguistika")
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

    SwipeView {
        id: applicationSwipeView        
        anchors.top: applicationHeader.bottom
        anchors.topMargin: Kirigami.Units.largeSpacing
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        interactive: false

        SkillsPage {
            id: page1
        }

        LanguagePage {
            id: page2
        }

        TranslationsViewPage {
            id: page3
        }

        GeneratedViewPage {
            id: page4
        }
    }
}