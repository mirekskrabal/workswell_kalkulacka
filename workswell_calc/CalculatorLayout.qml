import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Layouts 1.14

RowLayout {
    anchors.fill: parent
    property int col: 0
    property int  row: 0
    //stores button text values
    property var buttonVals: [["7","4","1", "0"], ["8","5","2", "."],
                              ["9","6","3", "+"], ["(", "*", "/", "-"],
                              [")", "C", "sci mode", "="],
                              ["sin()", "cos()", "tg()", "cotg()"],
                               ["log()", "^", "pi", "e"]];
    Repeater {
        model: col
        ColumnLayout {
            id: currentCol
            property int colIndex: index
            Layout.alignment: maximumHeight
            Repeater {
                model: row
                RoundButton {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    radius: 20
                    text: buttonVals[currentCol.colIndex][index]

                    palette {
                        button: (colIndex == 4 && index == 3) ? "limegreen" :
                                (colIndex == 4 && index == 2) ?  "pink" :
                                (colIndex == 4 && index == 1) ?  "lightcoral" : "lightsteelblue";
                    }
                    onClicked: {
                        if (colIndex == 4 && index == 3) {
                            txtResult.text += parser.calculate(txtInput.text)
                            txtInput.text = parser.res
                        }
                        else if (colIndex == 4 && index == 2){
                            layoutNoSci.visible = !layoutNoSci.visible
                            layoutWithSci.visible = !layoutWithSci.visible
                        }
                        else if (colIndex == 4 && index == 1) {
                            txtInput.text = ""
                        }

                        else{
                            txtInput.text += buttonVals[currentCol.colIndex][index]
                        }
                    }
                }
            }
        }
    }
}
