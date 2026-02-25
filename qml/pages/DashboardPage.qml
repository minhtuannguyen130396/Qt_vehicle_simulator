import QtQuick
import QtQuick.Controls
import "../components"

Item {
    id: root
    anchors.fill: parent

    Column {
        anchors.centerIn: parent
        spacing: 14
        width: Math.min(parent.width - 40, 720)

        // Motor
        Text {
            text: "Motor speed: " + controller.speed + " | I=" + controller.motorCurrent.toFixed(1) + "A | T=" + controller.motorTemp.toFixed(1) + "°C"
            font.pixelSize: 16
        }

        SpeedGauge {
            width: 240
            height: 180
            value: controller.speed
            lineW: 10
            minV: 0
            maxV: 100
        }

        Slider {
            width: parent.width
            from: 0
            to: 100
            value: controller.speed
            onMoved: controller.setSpeed(Math.round(value))
        }

        Row {
            spacing: 10

            Button {
                text: "Refresh (GetSnapshot)"
                onClicked: controller.refresh()
            }

            Text {
                text: controller.lastMessage
                color: "#666666"
                verticalAlignment: Text.AlignVCenter
                elide: Text.ElideRight
                width: parent.width - 200
            }
        }

        // Batteries
        Row {
            spacing: 18
            width: parent.width

            Frame {
                width: parent.width/2 - 9
                padding: 12

                Column {
                    spacing: 8
                    width: parent.width

                    Text { text: "Battery 1"; font.pixelSize: 16 }

                    Row {
                        spacing: 8
                        width: parent.width
                        TextField {
                            id: sn1
                            width: parent.width - btn1.width - 10
                            text: controller.b1Serial
                            placeholderText: "Serial number"
                        }
                        Button {
                            id: btn1
                            text: "Set"
                            onClicked: controller.setBattery1Serial(sn1.text)
                        }
                    }

                    Text { text: "V=" + controller.b1Voltage.toFixed(2) + "V   I=" + controller.b1Current.toFixed(2) + "A   T=" + controller.b1Temp.toFixed(1) + "°C" }

                    SocBar {
                        label: "SOC"
                        value: controller.b1Soc
                    }
                }
            }

            Frame {
                width: parent.width/2 - 9
                padding: 12

                Column {
                    spacing: 8
                    width: parent.width

                    Text { text: "Battery 2"; font.pixelSize: 16 }

                    Row {
                        spacing: 8
                        width: parent.width
                        TextField {
                            id: sn2
                            width: parent.width - btn2.width - 10
                            text: controller.b2Serial
                            placeholderText: "Serial number"
                        }
                        Button {
                            id: btn2
                            text: "Set"
                            onClicked: controller.setBattery2Serial(sn2.text)
                        }
                    }

                    Text { text: "V=" + controller.b2Voltage.toFixed(2) + "V   I=" + controller.b2Current.toFixed(2) + "A   T=" + controller.b2Temp.toFixed(1) + "°C" }

                    SocBar {
                        label: "SOC"
                        value: controller.b2Soc
                    }
                }
            }
        }
    }
}
