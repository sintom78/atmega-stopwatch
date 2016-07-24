import QtQuick 2.0
import QtQuick.Controls 1.1

Rectangle {
    id: mainRect
    objectName: "mainRect"
    width: 600
    height: 300

    signal qmlResetButton(bool pressed)
    signal qmlStartStopButton(bool pressed)
    signal qmlModeButton(bool pressed)
    signal qmlSetupButton(bool pressed)

    Rectangle
    {
        id: segmentDisplay
        objectName: "segmentDisplay"
        width:300
        height:250
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.margins: 20
        color: "black"
        property bool segAEnabled
        property bool segBEnabled
        property bool segCEnabled
        property bool segDEnabled
        property bool segEEnabled
        property bool segFEnabled
        property bool segGEnabled
        property int activeDisplayIndex
        property Rectangle activeDispl

        function setSegmentColor(seg)
        {

            if (!seg) {
                console.log("seg unset: ",seg)
                return;
            }

            if (seg.objectName === "segA") {
                seg.color = segAEnabled ? "red" : "darkred";
            } else if (seg.objectName === "segB"){
                seg.color = segBEnabled ? "red" : "darkred";
            } else if (seg.objectName === "segC"){
                seg.color = segCEnabled ? "red" : "darkred";
            } else if (seg.objectName === "segD"){
                seg.color = segDEnabled ? "red" : "darkred";
            } else if (seg.objectName === "segE"){
                seg.color = segEEnabled ? "red" : "darkred";
            } else if (seg.objectName === "segF"){
                seg.color = segFEnabled ? "red" : "darkred";
            } else if (seg.objectName === "segG"){
                seg.color = segGEnabled ? "red" : "darkred";
            }
        }

        function setColors(displ)
        {
            if (displ) {
                if (displ!==activeDispl) {
                    //console.log("off colors for: ",displ.objectName);
                    for (var i in displ.children)
                    {
                        displ.children[i].color = "darkred";
                    }
                } else {
                    //console.log("on colors for: ",displ.objectName);
                    for (var i in displ.children)
                    {
                        setSegmentColor(displ.children[i]);
                    }
                }
            }
        }

        function getSegmentforActiveDisplay(segname)
        {
            if (!activeDispl) {
                console.log("activedispl not set");
                return null;
            }
            for (var i in activeDispl.children)
            {
                if (activeDispl.children[i].objectName === segname) {
                    return activeDispl.children[i];
                }
            }
            return null;
        }

        onSegAEnabledChanged: {
            var seg = getSegmentforActiveDisplay("segA");
            setSegmentColor(seg);
        }

        onSegBEnabledChanged: {
            var seg = getSegmentforActiveDisplay("segB");
            setSegmentColor(seg);
        }

        onSegCEnabledChanged: {
            var seg = getSegmentforActiveDisplay("segC");
            setSegmentColor(seg);
        }

        onSegDEnabledChanged: {
            var seg = getSegmentforActiveDisplay("segD");
            setSegmentColor(seg);
        }

        onSegEEnabledChanged: {
            var seg = getSegmentforActiveDisplay("segE");
            setSegmentColor(seg);
        }

        onSegFEnabledChanged: {
            var seg = getSegmentforActiveDisplay("segF");
            setSegmentColor(seg);
        }

        onSegGEnabledChanged: {
            var seg = getSegmentforActiveDisplay("segG");
            setSegmentColor(seg);
        }


        onActiveDisplayIndexChanged: {
            var displ;
            if (activeDisplayIndex===0) {
                displ = digit0;
            } else if (activeDisplayIndex===1) {
                displ = digit1;
            } else if (activeDisplayIndex===2) {
                displ = digit2;
            } else if (activeDisplayIndex===3) {
                displ = digit3;
            } else {
                console.info("QML: Invalid DisplayIndex");
            }
            setColors(displ);
            activeDispl = displ;
            setColors(displ);
        }


        Flow {
            spacing: 5
            height: 200
            anchors.verticalCenter: parent.verticalCenter
            anchors.horizontalCenter: parent.horizontalCenter

            Rectangle {
                id: digit0
                objectName: "digit0"
                height: parent.height
                width: 60
                color: "black"
                //segments
                Rectangle {
                    id: digit0SegA
                    objectName: "segA"
                   // color: { return segmentDisplay.getColor(objectName,digit0); }
                    height: 5
                    x: 2
                    y: 2
                    width: { parent.width - 4 }
                }
                Rectangle {
                    id: digit0SegB
                    objectName: "segB"
                    color: "red"
                    width: 5
                    y: { digit0SegA.y + digit0SegA.height + 2 }
                    anchors.right: parent.right
                    anchors.rightMargin: 2
                    height: { (digit0.height/2) - 10}
                }

                Rectangle {
                    id: digit0SegC
                    objectName: "segC"
                    color: "red"
                    width: digit0SegB.width
                    height: { digit0SegB.height }
                    anchors.right: parent.right
                    anchors.rightMargin: 2
                    anchors.bottom: digit0SegD.top
                    anchors.bottomMargin: 2
                }

                Rectangle {
                    id: digit0SegD
                    objectName: "segD"
                    color: "red"
                    height: digit0SegA.height
                    width: digit0SegA.width
                    anchors.bottom: parent.bottom
                    x: digit0SegA.x
                }

                Rectangle {
                    id: digit0SegE
                    objectName: "segE"
                    color: "red"
                    width: digit0SegB.width
                    height: digit0SegB.height
                    anchors.left: parent.left
                    anchors.leftMargin: 2
                    anchors.bottom: digit0SegD.top
                    anchors.bottomMargin: 2
                }
                Rectangle {
                    id: digit0SegF
                    objectName: "segF"
                    color: "red"
                    width: digit0SegB.width
                    height: digit0SegB.height
                    y: digit0SegB.y
                    anchors.left: parent.left
                    anchors.leftMargin: 2
                }
                Rectangle {
                    id: digit0SegG
                    objectName: "segG"
                    color:"red"
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.margins: 10
                    height: digit0SegA.height
                    y: {(parent.height/2)-2}

                }/*
                Rectangle {
                    id: digit0dot
                }*/
            }

            //HOURS digit1
            Rectangle {
                id: digit1
                objectName: "digit1"
                color: "black"
                width: 60
                height: parent.height
                //segments
                Rectangle {
                    id: digit1SegA
                    objectName: "segA"
                    color: "red"
                    height: 5
                    x: 2
                    y: 2
                    width: { parent.width - 4 }
                }
                Rectangle {
                    id: digit1SegB
                    objectName: "segB"
                    color: "red"
                    width: 5
                    y: { digit1SegA.y + digit1SegA.height + 2 }
                    anchors.right: parent.right
                    anchors.rightMargin: 2
                    height: { (digit1.height/2) - 10}
                }

                Rectangle {
                    id: digit1SegC
                    objectName: "segC"
                    color: "red"
                    width: digit1SegB.width
                    height: { digit1SegB.height }
                    anchors.right: parent.right
                    anchors.rightMargin: 2
                    anchors.bottom: digit1SegD.top
                    anchors.bottomMargin: 2
                }

                Rectangle {
                    id: digit1SegD
                    objectName: "segD"
                    color: "red"
                    height: digit1SegA.height
                    width: digit1SegA.width
                    anchors.bottom: parent.bottom
                    x: digit1SegA.x
                }

                Rectangle {
                    id: digit1SegE
                    objectName: "segE"
                    color: "red"
                    width: digit1SegC.width
                    height: digit1SegC.height
                    anchors.left: parent.left
                    anchors.leftMargin: 2
                    anchors.bottom: digit1SegD.top
                    anchors.bottomMargin: 2
                }
                Rectangle {
                    id: digit1SegF
                    objectName: "segF"
                    color: "red"
                    width: digit1SegB.width
                    height: digit1SegB.height
                    y: digit1SegB.y
                    anchors.left: parent.left
                    anchors.leftMargin: 2
                }
                Rectangle {
                    id: digit1SegG
                    objectName: "segG"
                    color:"red"
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.margins: 10
                    height: digit1SegA.height
                    y: {(parent.height/2)-2}

                }
            }

            //DIODES
            Rectangle {
                id: diodes
                objectName: "diodes"
                color: "black"
                width: 10
                height: parent.height

                Rectangle {
                    id: diode0
                    width: 5
                    height: 5
                    color: "red"
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.verticalCenterOffset: -10
                }

                Rectangle {
                    id: diode1
                    color: "red"
                    width:5
                    height: 5
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.verticalCenterOffset: 10

                }
            }

            //MINUTES - digit2
            Rectangle {
                id: digit2
                objectName: "digit2"
                color: "black"
                width: 60
                height: parent.height
                //segments
                Rectangle {
                    id: digit2SegA
                    objectName: "segA"
                    color: "red"
                    height: 5
                    x: 2
                    y: 2
                    width: { parent.width - 4 }
                }
                Rectangle {
                    id: digit2SegB
                    objectName: "segB"
                    color: "red"
                    width: 5
                    y: { digit2SegA.y + digit2SegA.height + 2 }
                    anchors.right: parent.right
                    anchors.rightMargin: 2
                    height: { (digit2.height/2) - 10}
                }

                Rectangle {
                    id: digit2SegC
                    objectName: "segC"
                    color: "red"
                    width: digit2SegB.width
                    height: { digit2SegB.height }
                    anchors.right: parent.right
                    anchors.rightMargin: 2
                    anchors.bottom: digit2SegD.top
                    anchors.bottomMargin: 2
                }

                Rectangle {
                    id: digit2SegD
                    objectName: "segD"
                    color: "red"
                    height: digit2SegA.height
                    width: digit2SegA.width
                    anchors.bottom: parent.bottom
                    x: digit2SegA.x
                }

                Rectangle {
                    id: digit2SegE
                    objectName: "segE"
                    color: "red"
                    width: digit2SegC.width
                    height: digit2SegC.height
                    anchors.left: parent.left
                    anchors.leftMargin: 2
                    anchors.bottom: digit2SegD.top
                    anchors.bottomMargin: 2
                }
                Rectangle {
                    id: digit2SegF
                    objectName: "segF"
                    color: "red"
                    width: digit2SegB.width
                    height: digit2SegB.height
                    y: digit2SegB.y
                    anchors.left: parent.left
                    anchors.leftMargin: 2
                }
                Rectangle {
                    id: digit2SegG
                    objectName: "segG"
                    color:"red"
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.margins: 10
                    height: digit2SegA.height
                    y: {(parent.height/2)-2}
                }
            }
            //MINUTES - digit3
            Rectangle {
                id: digit3
                objectName: "digit3"
                color: "black"
                width: 60
                height: parent.height
                //segments
                Rectangle {
                    id: digit3SegA
                    objectName: "segA"
                    color: "red"
                    height: 5
                    x: 2
                    y: 2
                    width: { parent.width - 4 }
                }
                Rectangle {
                    id: digit3SegB
                    objectName: "segB"
                    color: "red"
                    width: 5
                    y: { digit3SegA.y + digit3SegA.height + 2 }
                    anchors.right: parent.right
                    anchors.rightMargin: 2
                    height: { (digit3.height/2) - 10}
                }

                Rectangle {
                    id: digit3SegC
                    objectName: "segC"
                    color: "red"
                    width: digit3SegB.width
                    height: { digit3SegB.height }
                    anchors.right: parent.right
                    anchors.rightMargin: 2
                    anchors.bottom: digit3SegD.top
                    anchors.bottomMargin: 2
                }

                Rectangle {
                    id: digit3SegD
                    objectName: "segD"
                    color: "red"
                    height: digit3SegA.height
                    width: digit3SegA.width
                    anchors.bottom: parent.bottom
                    x: digit3SegA.x
                }

                Rectangle {
                    id: digit3SegE
                    objectName: "segE"
                    color: "red"
                    width: digit3SegC.width
                    height: digit3SegC.height
                    anchors.left: parent.left
                    anchors.leftMargin: 2
                    anchors.bottom: digit3SegD.top
                    anchors.bottomMargin: 2
                }
                Rectangle {
                    id: digit3SegF
                    objectName: "segF"
                    color: "red"
                    width: digit3SegB.width
                    height: digit3SegB.height
                    y: digit3SegB.y
                    anchors.left: parent.left
                    anchors.leftMargin: 2
                }
                Rectangle {
                    id: digit3SegG
                    objectName: "segG"
                    color:"red"
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.margins: 10
                    height: digit3SegA.height
                    y: {(parent.height/2)-2}
                }
            }
        }
    }

    Flow {
        spacing: 5
        height: 25

        anchors.bottom: parent.bottom
        anchors.left: parent.left
        //anchors.margins: 10

        Button {
            width: 80
            text: qsTr("Mode")
            onPressedChanged:
            {
                qmlModeButton(pressed);
            }
        }

        Button {
            width: 120
            text: qsTr("StartStop")
            onPressedChanged:
            {
                qmlStartStopButton(pressed);
            }
        }

        Button {
            width: 80
            text: qsTr("Reset")
            onPressedChanged:
            {
                qmlResetButton(pressed);
            }
        }

        Button {
            width: 80
            text: qsTr("Setup")
            onPressedChanged:
            {
                qmlSetupButton(pressed);
            }
        }
    }

}
