import QtQuick 2.12

Item {
  id: root
  width: 24
  height: 24

  Rectangle {
    id: bar1
    x: 2
    y: 5
    width: 20
    height: 2
    antialiasing: true
  }

  Rectangle {
    id: bar2
    x: 2
    y: 10
    width: 20
    height: 2
    antialiasing: true
  }

  Rectangle {
    id: bar3
    x: 2
    y: 15
    width: 20
    height: 2
    antialiasing: true
  }

  property int animationDuration: 350

  state: "menu"
  states: [
    State {
      name: "menu"
    },

    State {
      name: "back"
      PropertyChanges { target: root; rotation: 180 }
      PropertyChanges { target: bar1; rotation: 45; width: 13; x: 9.5; y: 8 }
      PropertyChanges { target: bar2; width: 17; x: 3; y: 12 }
      PropertyChanges { target: bar3; rotation: -45; width: 13; x: 9.5; y: 16 }
    }
  ]

  transitions: [
    Transition {
      RotationAnimation { target: root; direction: RotationAnimation.Clockwise; duration: animationDuration; easing.type: Easing.InOutQuad }
      PropertyAnimation { target: bar1; properties: "rotation, width, x, y"; duration: animationDuration; easing.type: Easing.InOutQuad }
      PropertyAnimation { target: bar2; properties: "rotation, width, x, y"; duration: animationDuration; easing.type: Easing.InOutQuad }
      PropertyAnimation { target: bar3; properties: "rotation, width, x, y"; duration: animationDuration; easing.type: Easing.InOutQuad }
    }
  ]
}
