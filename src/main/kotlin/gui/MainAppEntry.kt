package gui

import javafx.collections.FXCollections
import tornadofx.*
import proxy.BTreeProxy


class MyApp: App(MyView::class)

class MyView: View() {
    val controller: MyController by inject()
    val cl : BTreeProxy = BTreeProxy()

    override val root = vbox {
        button("Press me ${1}")
        label("Waiting")
        listview(controller.values)
    }
}

class MyController: Controller() {
    val values = FXCollections.observableArrayList("Alpha","Beta","Gamma","Delta")
}