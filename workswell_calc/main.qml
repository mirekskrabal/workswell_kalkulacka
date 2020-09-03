import QtQuick 2.14
import QtQuick.Window 2.14
import QtQuick.Controls 2.14
import QtQuick.Layouts 1.14
import QtQuick.Controls.Styles 1.4


ApplicationWindow {
    width: 640
    height: 480
    visible: true
    title: qsTr("Calculator")

    Rectangle {
            id: appMenu
            anchors.top: parent.top;
            anchors.left: parent.left; anchors.right: parent.right
            height: 20
    }

    //stores button text values
    Item{
        id: buttonValues
        property var buttonVals: [["7","4","1", "0"], ["8","5","2", ","],
                                  ["9","6","3", "%"], ["/", "*", "-", "+"],
                                  ["()", "C", "sci mode", "="],
                                  ["sin", "cos", "tg", "cotg"],
                                   ["exp", "log", "pi", "e"]];
    }

    //creating buttons
    Item {
        id: buttons
        anchors.left: parent.left; anchors.bottom: parent.bottom
        anchors.right: parent.right; anchors.top: parent.verticalCenter
        anchors.margins: 4

        /*2 layouts - one with scientific functions, one without
          using visibility of the layouts it is possible to switch between them*/
        CalculatorLayout {
            id: layoutWithSci
            col: 7
            row: 4
            visible: false
        }

        CalculatorLayout {
            id: layoutNoSci
            col: 5
            row: 4
            visible: true
        }
    }

    Rectangle {
        id: textInputFrame
        height: 50
        anchors.left: parent.left; anchors.right: parent.right
        anchors.top: appMenu.top
        border.color: "grey"
        border.width: 2
        anchors.margins: 3

        TextArea {
            id: plainTxtInput
            anchors.fill: parent
            anchors.margins: 4
        }
    }

    Rectangle {
        id: resultFrame
        anchors.left: parent.left; anchors.right: parent.right
        anchors.top: textInputFrame.bottom; anchors.bottom: parent.verticalCenter
        border.color: "grey"
        border.width: 2
        anchors.margins: 3

        TextArea {
            id: txtResult
            anchors.fill: parent
        }
    }
}
