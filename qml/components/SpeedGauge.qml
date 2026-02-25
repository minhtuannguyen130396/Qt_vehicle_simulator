import QtQuick

Item {
    id: root
    width: 220
    height: 170

    // public API
    property real value: 0          // 0..100
    property real lineW: 10
    property real startDeg: 135     // start of 3/4 ring
    property real sweepDeg: 270     // 3/4 circle sweep
    property real minV: 0
    property real maxV: 100

    Canvas {
        id: gauge
        anchors.centerIn: parent
        width: parent.width
        height: parent.height

        onPaint: {
            var ctx = getContext("2d");
            ctx.reset();

            var w = width;
            var h = height;

            var cx = w / 2;
            var cy = h * 0.62;

            var r = Math.min(w, h) * 0.42;
            var lw = root.lineW;

            function deg2rad(d) { return d * Math.PI / 180.0; }

            var start = deg2rad(root.startDeg);
            var endBg = deg2rad(root.startDeg + root.sweepDeg);

            var v = Math.max(root.minV, Math.min(root.maxV, root.value));
            var t = (v - root.minV) / (root.maxV - root.minV);
            var endFg = deg2rad(root.startDeg + root.sweepDeg * t);

            // background arc (black)
            ctx.beginPath();
            ctx.lineWidth = lw;
            ctx.lineCap = "round";
            ctx.strokeStyle = "#000000";
            ctx.arc(cx, cy, r, start, endBg, false);
            ctx.stroke();

            // foreground arc (green)
            ctx.beginPath();
            ctx.lineWidth = lw;
            ctx.lineCap = "round";
            ctx.strokeStyle = "#00c853";
            ctx.arc(cx, cy, r, start, endFg, false);
            ctx.stroke();
        }
    }

    // repaint triggers
    onValueChanged: gauge.requestPaint()
    onWidthChanged: gauge.requestPaint()
    onHeightChanged: gauge.requestPaint()
    Component.onCompleted: gauge.requestPaint()

    Text {
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        y: 10
        text: Math.round(root.value)
        font.pixelSize: 34
        font.bold: true
        color: "#eaeaea"
    }

    Text {
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        anchors.topMargin: 2
        text: "SPEED"
        font.pixelSize: 12
        color: "#9a9a9a"
    }
}
