import QtQuick 2.0
import Sailfish.Silica 1.0



Page {
    id: page

    allowedOrientations: Orientation.All

    SilicaFlickable {
        anchors.fill: parent

        PullDownMenu {
            MenuItem {
                text: qsTr("KMeans")
                onClicked: pageStack.push(Qt.resolvedUrl("SecondPage.qml"))
            }

        }

        contentHeight: column.height

        Column {
            id: column

            width: page.width
            spacing: Theme.paddingLarge
            PageHeader {
                title: qsTr("Начальная страница")
            }
            Label {
                x: Theme.horizontalPageMargin
                text: qsTr("Hello!")
                color: Theme.secondaryHighlightColor
                font.pixelSize: Theme.fontSizeExtraLarge
            }
        }
    }
}
