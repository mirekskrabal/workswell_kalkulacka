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

    //calculator button layouts
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
        TextField {
            id: txtInput
            placeholderText: qsTr("Input expression:")
            anchors.fill: parent
            anchors.margins: 4
            //evaluate expression when return is pressed
            Keys.onReturnPressed: {
                txtResult.text += parser.calculate(txtInput.text)
                txtInput.text = parser.res
            }
        }

    }

    Rectangle {
        id: resultFrame
        anchors.left: parent.left; anchors.right: parent.right
        anchors.top: textInputFrame.bottom; anchors.bottom: parent.verticalCenter
        border.color: "grey"
        border.width: 2
        anchors.margins: 3
        ScrollView {
            id: view
            anchors.fill: parent
            TextArea {
               id: txtResult
               anchors.fill: parent
            }
        }
    }
}
