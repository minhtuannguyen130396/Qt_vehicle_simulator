import QtQuick
import QtQuick.Controls
import "qml/pages"
ApplicationWindow {
    visible: true
    width: 640
    height: 420
    title: "Qt Device App (Fixed QRC)"

    // Page root
    DashboardPage {
        id: dashboard
        anchors.fill: parent
    }

    Component.onCompleted: controller.refresh()
}
