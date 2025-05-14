import QtQuick 2.0
import Sailfish.Silica 1.0
import org.sailfishos.nfcshare 1.0

Item {
    id: thisItem

    property var shareAction

    property var _content: (shareAction && 'resources' in shareAction && shareAction.resources.length > 0) ? shareAction.resources[0] : undefined
    property string _text: ('data' in _content) ? _content.data : ('status' in _content) ? _content.status : ""

    NfcShare {
        id: nfcShare

        text: thisItem.visible ? _text : ""
        onDone: shareAction.done()
    }

    Item {
        anchors {
            left: parent.left
            right: parent.right
            top: parent.top
            bottom: progress.top
        }

        Image {
            readonly property int _size: Theme.itemSizeHuge

            anchors.centerIn: parent
            source: "image://theme/icon-m-nfc"
            sourceSize: Qt.size(_size, _size)
            width: _size
            height: _size
        }
    }

    ProgressBar {
        id: progress

        anchors {
            left: parent.left
            right: parent.right
            bottom: parent.bottom
            bottomMargin: Theme.paddingLarge
        }
        indeterminate: !nfcShare.bytesTransferred
        maximumValue: nfcShare.bytesTotal
        value: nfcShare.bytesTransferred
        opacity: (nfcShare.ready && !nfcShare.done) ? 1 : 0
    }
}
