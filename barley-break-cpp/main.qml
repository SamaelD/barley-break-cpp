import QtQuick 2.4
import QtQuick.Controls 1.4
import QtQuick.Dialogs 1.2
import QtQuick.Window 2.0

import Logic_CPP 1.0

ApplicationWindow {
    id: wnd

    property var score: null

    minimumHeight: 600
    minimumWidth: 400
    visible: true

    title: "Barley break"

    GridView {
        id: grid

        signal victory();

        function refreshModel() {
            Logic.refresh();
            var res = JSON.parse(wnd.score);
            newGame.text = "Best result: " + res;
            newGame.visible = true;
        }

        header: Button {
            id: refresh

            width: wnd.width
            height: wnd.height * 0.1

            text: "refresh"
            onClicked: grid.refreshModel();
        }

        footer: Rectangle {
            x:0
            y: 100

            width: wnd.width
            height: wnd.height * 0.1

            border.color: "black"
            color: "lightgray"

            Text {
                anchors.centerIn: parent

                font.pixelSize: parent.height * 0.85
                font.bold: true

                text: "Moves: " + Logic.moveCounter
            }
        }

        anchors.fill: parent

        interactive: false;

        cellWidth: width / 4
        cellHeight: (parent.height - parent.height * 0.2) / 4

        model: Logic.list
        delegate: component

        move: Transition {
            NumberAnimation {
                properties: "x, y"
                easing.type: Easing.InOutSine
                duration: 300
            }
        }
    }

    Component {
        id: component
        Rectangle {
            id: rect

            width: grid.cellWidth
            height: grid.cellHeight

            opacity: Logic.identifier(index) == 0 ? 0 : 1
            border.color: "black"
            color: "green"
            radius: 10

            Text {
                anchors.centerIn: rect

                text: Logic.identifier(index)

                font.bold: true
                font.pixelSize: 48

                color: mouse.containsMouse ? "black" : "darkred"
            }

            transform: Scale {
                origin.x: rect.x;
                origin.y: rect.y;
                xScale: mouse.containsMouse ? 1.015 : 1
                yScale: mouse.containsMouse ? 1.015 : 1
            }

            MouseArea {
                id: mouse

                anchors.fill: rect

                hoverEnabled: true
                acceptedButtons: Qt.LeftButton
                onClicked: {
                    if (Logic.identifier(index) == 0) {
                        return;
                    }
                    Logic.move(index);
                    if (Logic.checkWin()) {
                        wnd.score = JSON.stringify(Logic.moveCounter);
                        gameOverDialog.visible = true;
                    }
                }
            }
        }
    }

    MessageDialog {
        id: gameOverDialog

        visible: false

        title: "Winner"
        text: "You're win the game!\nCongratulations!!\nRestart?"

        standardButtons: StandardButton.Yes | StandardButton.No
        onYes: grid.refreshModel();
        onNo: Qt.quit()
    }

    MessageDialog {
        id: newGame

        visible: false

        title: "New game"
    }
}
