package gui

import tornadofx.*
import proxy.BTreeProxy

class MyApp: App(MyView::class)

class MyView: View() {

    val cl : BTreeProxy = BTreeProxy()
    val re = cl.call()

    override val root = vbox {
        button("Press me")
        label("Waiting")
    }
}