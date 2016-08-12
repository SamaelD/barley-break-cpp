import QtQuick 2.0

import Logic_CPP 1.0

Rectangle {
    id: rect

    signal victory();

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
        }
    }
}
