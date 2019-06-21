package gui

import javafx.collections.FXCollections
import tornadofx.*

import proxy.*
import drivers.*

class MyApp: App(MyView::class)

class MyView: View() {
    val controller: MyController by inject()

    var usr = User(Driver(HashMap(), BTreeProxy()))

    override val root = vbox {
        button("Press me ${usr.check(2)}")
        label("Waiting")
        listview(controller.values)
    }
}

class MyController: Controller() {
    val values = FXCollections.observableArrayList("Alpha","Beta","Gamma","Delta")
}

class User(val driver : Driver)
{
    fun check(key : Int) : String
    {
        driver.insert(key)
        return when(driver.check_exitence(key))
        {
            true -> "yes"
            false -> "no"
        }
        driver.close()
    }
}
