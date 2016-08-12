import QtQuick 2.4
import QtQuick.Controls 1.4
import QtQuick.Dialogs 1.2
import QtQuick.Window 2.0

import Logic_CPP 1.0

ApplicationWindow {
    id: wnd

    minimumHeight: 600
    minimumWidth: 400
    visible: true

    title: "Barley break"

    GridView {
        id: grid

        signal victory();

        function refreshModel() {
            //Logic.refresh();  // commented for testing save/load JSON
            newGame.text = "Best result: " + Logic.BestScore;
            newGame.visible = Logic.BestScore == 0 ? false : true;
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
        delegate: Delegate {
            width: grid.cellWidth
            height: grid.cellHeight

            onVictory: gameOverDialog.visible = true;
        }

        move: Transition {
            NumberAnimation {
                properties: "x, y"
                easing.type: Easing.InOutSine
                duration: 300
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
    Component.onCompleted: grid.refreshModel();
}
