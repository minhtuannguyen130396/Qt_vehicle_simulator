import QtQuick

Item {
    id: root
    width: 120
    height: 160

    // public API
    property string label: "SOC"
    property real value: 0   // 0..100

    Column {
        spacing: 8

        Text { text: root.label + ": " + Math.round(root.value) + "%" }

        Rectangle {
            width: 40
            height: 120
            radius: 6
            color: "#2b2b2b"
            border.width: 1
            border.color: "#555"

            Rectangle {
                width: parent.width
                height: parent.height * root.value / 100.0
                anchors.bottom: parent.bottom
                radius: 6
                color: root.value < 30 ? "red"
                     : root.value <= 80 ? "green"
                     : "blue"

                Behavior on height { NumberAnimation { duration: 400 } }
            }
        }
    }
}
