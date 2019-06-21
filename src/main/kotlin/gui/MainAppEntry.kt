package gui

import javafx.collections.FXCollections
import tornadofx.*
import proxy.*
import drivers.*
import kotlin.system.measureNanoTime
import kotlin.system.measureTimeMillis

class MyApp: App(MyView::class)

class MyView: View() {
    val controller: MyController by inject()

    var usr = User(Driver(HashMap(), BTreeProxy()))

    override val root = tabpane {
        tab("Set")
        {
            vbox {
                val status = textfield("STATUS BAR")
                val benchmark = textfield("BENCHMARK")
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
                            status.text = "Inserted"
                            var time_spent = measureNanoTime{usr.insert(vlue_int)}
                            benchmark.text = time_spent.toString()
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
                            status.text = "Removed"
                            var time_spent = measureNanoTime{usr.remove(vlue_int)}
                            benchmark.text = time_spent.toString()
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
                        } catch (e: NumberFormatException) {
                            println("invalid format for a number!!!")
                            status.text = "Invalid?"
                        }
                    }
                }
            }
        }
    }
}

class MyController: Controller() {
    val values = FXCollections.observableArrayList("Int")
    fun insert(key : Int, usr : User) : Unit {
        usr.insert(key)
    }
    fun remove(key: Int, usr : User) : Unit {
        usr.remove(key)
    }
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

    fun insert(key: Int) : Unit = driver.insert(key)
    fun remove(key: Int) : Unit = driver.remove(key)
}