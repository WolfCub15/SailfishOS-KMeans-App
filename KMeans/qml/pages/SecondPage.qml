import QtQuick 2.0
import Sailfish.Silica 1.0

import MyModule 1.0



Page {
    id: page
    property int i: 0

    allowedOrientations: Orientation.All



    SilicaFlickable {
        anchors.fill: parent
        contentHeight: column.height



        Column {
            id: column

            property string imagePath: "qrc:/images/images/pand.jpg"
            property string original: "qrc:/images/images/pand.jpg"

            KMeansClass{
                id: kMeans
            }

            width: page.width
            spacing: Theme.paddingLarge
            /*PageHeader {
                title: qsTr("UI Template")
            }

            Label {
                x: Theme.horizontalPageMargin
                text: qsTr("Hello Sailors")
                color: Theme.secondaryHighlightColor
                font.pixelSize: Theme.fontSizeExtraLarge
            }
*/


            Image {
                id: img
                width: parent.width
                fillMode: Image.PreserveAspectFit
                source: column.imagePath
            }

            Rectangle {
                 width: parent.width; height: 300
                 color: "transparent"
                 anchors.horizontalCenter: img.horizontalCenter
                 SilicaListView {
                     id: view
                     model: ListModel {
                             id: tasksModel
                             ListElement { name: "Pand"; path:
                            "qrc:/images/images/pand.jpg"; }
                             ListElement { name: "Utya"; path:
                            "qrc:/images/images/utya.jpg"; }
                             ListElement { name: "Dog"; path:
                            "qrc:/images/images/dog.jpg"; }
                             ListElement { name: "Brain"; path:
                            "qrc:/images/images/brain.jpg"; }
                     }
                     anchors.fill: parent
                     delegate: BackgroundItem {
                         width: ListView.view.width
                         height: Theme.itemSizeSmall
                         anchors.horizontalCenter: parent.horizontalCenter
                         Label {
                             text: name
                             anchors.horizontalCenter: parent.horizontalCenter
                        }
                        onClicked: {
                             column.imagePath = tasksModel.get(index).path;
                             column.original = tasksModel.get(index).path;
                         }
                     }
                 }
                 }

                 TextField {
                    id: textField1

                    //width: parent.width/2
                 }
                 Button {
                     id: processedButton
                     text: "Calculate"
                     anchors.horizontalCenter: img.horizontalCenter
                     onClicked: {
                         kMeans.clear();
                         kMeans.n_klaster = textField1.text;
                         kMeans.set(column.imagePath.toString());
                         column.imagePath = kMeans.reciveImage();
                    }
                 }
                 Button {
                     id: originalButton
                     text: "Original"
                     anchors.horizontalCenter: img.horizontalCenter
                     onClicked: column.imagePath = column.original;
                 }
        }
    }
}
