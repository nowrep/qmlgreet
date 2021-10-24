import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.4

import cz.nowrep.qmlgreet 1.0

ApplicationWindow {
    id: window
    width: 1920
    height: 1080
    visible: true

    Image {
        anchors.fill: parent
        source: Backend.backgroundSrc

        Pane {
            anchors.centerIn: parent
            width: 600
            height: 300
            Material.elevation: 5
            Material.background: "#bbffffff"

            ColumnLayout {
                anchors.centerIn: parent

                Label {
                    id: timeLabel
                    Layout.alignment: Qt.AlignCenter
                    font.bold: true
                    font.pointSize: 28
                    Component.onCompleted: update()

                    function update() {
                        text = Qt.formatTime(new Date(), "hh:mm");
                    }

                    Timer {
                        interval: 1000
                        repeat: true
                        running: true
                        onTriggered: timeLabel.update()
                    }
                }

                Label {
                    Layout.alignment: Qt.AlignCenter
                    Layout.topMargin: 30
                    font.pointSize: 12
                    font.capitalization: Font.AllUppercase
                    text: Backend.user
                }

                TextField {
                    Layout.alignment: Qt.AlignCenter
                    Layout.preferredWidth: 200
                    echoMode: TextInput.Password
                    horizontalAlignment: TextInput.AlignHCenter
                    Component.onCompleted: forceActiveFocus()
                    onAccepted: {
                        if (text) {
                            Backend.login(text);
                        }
                    }
                }
            }
        }

        Button {
            anchors {
                top: parent.top
                right: parent.right
                margins: 20
            }
            text: "x"
            onClicked: Qt.quit()
        }

        Connections {
            target: Backend

            function onSessionSuccess() {
                Qt.quit();
            }
        }
    }
}
