package drivers

import proxy.BTreeProxy
import tornadofx.getProperty

data class Server(val id : Int, val name : String, val rack : Int)
data class Name(val name: String)
data class Id (val id : Int)
data class Rack(var rack: Int)

/*
* manage creation of a table
* doesn't mater what types the objects have
* they all are data classes
* stores a table
* */


// there is no need for a driver. it is just to hode the ugly
// java interface - but still it needs to be refactored the fuck out
 class Driver ( val table : HashMap<Int, Int>,
                private val clib: BTreeProxy)
{
    init { clib.init() }
    fun insert(key : Int) : Unit = clib.insert_key(key)
    fun remove(key: Int) : Unit = clib.remove_key(key)
    fun check_exitence(key: Int) : Boolean = clib.is_inside(key)
    fun close() : Unit = clib.release()
}