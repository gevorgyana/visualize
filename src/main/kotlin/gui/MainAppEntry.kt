package gui

import javafx.collections.FXCollections
import tornadofx.*
import proxy.*
import drivers.*

class MyApp: App(MyView::class)

class MyView: View() {
    val controller: MyController by inject()

    var usr = User(Driver(HashMap(), BTreeProxy()))

    override val root = tabpane {
        tab("Set")
        {
            vbox {
                val status = textfield("STATUS BAR")


                label("enter the numbers below")

                val f = textfield("") {
                    filterInput { it.controlNewText.isInt() }
                }

                button("Insert")
                {
                    action {
                        val vlue = f.textProperty().get()
                        try {
                            val vlue_int = vlue.toInt()
                            usr.insert(vlue_int)
                        } catch (e : NumberFormatException)
                        {
                            println("invalid format for a number!!!")
                            status.text = "Invalid?"
                        }
                    }
                }

                button("Remove")
                {
                    action {
                        val vlue = f.textProperty().get()
                        try {
                            val vlue_int = vlue.toInt()
                            usr.remove(vlue_int)
                        } catch (e : NumberFormatException)
                        {
                            println("invalid format for a number!!!")
                            status.text = "Invalid?"
                        }
                    }
                }

                button ("Check")
                {
                    action {
                        val vlue = f.textProperty().get()
                        try {
                            val vlue_int = vlue.toInt()
                            val ans = usr.check(vlue_int)
                            status.text = ans
                        } catch (e : NumberFormatException)
                        {
                            println("invalid format for a number!!!")
                            status.text = "Invalid?"
                        }
                    }
                }
            }
        }
        tab("List")
        {

        }
    }
}

class MyController: Controller() {
    val values = FXCollections.observableArrayList("Int")
    fun insert(key : Int, usr : User) = usr.insert(key)
    fun remove(key: Int, usr : User) = usr.remove(key)
    fun check(key: Int, usr: User) : String {
        return usr.check(key)
    }
}

class User(val driver : Driver)
{
    fun check(key : Int) : String
    {
        return when(driver.check_exitence(key))
        {
            true -> "yes"
            false -> "no"
        }
        driver.close()
    }

    fun insert(key: Int) = driver.insert(key)
    fun remove(key: Int) = driver.remove(key)
}
