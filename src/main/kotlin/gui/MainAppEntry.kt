package gui

import javafx.collections.FXCollections
import tornadofx.*
import proxy.BTreeProxy


class MyApp: App(MyView::class)

class MyView: View() {
    val controller: MyController by inject()
    val cl : BTreeProxy = BTreeProxy()
    val re = cl.call()

    override val root = vbox {
        button("Press me ${re}")
        label("Waiting")
        listview(controller.values)
    }
}

class MyController: Controller() {
    val values = FXCollections.observableArrayList("Alpha","Beta","Gamma","Delta")
}