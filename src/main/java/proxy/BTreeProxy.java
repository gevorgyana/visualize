package proxy;

import com.sun.jna.Library;
import com.sun.jna.Native;
import com.sun.jna.Platform;

/*
public class BTreeProxy {
    public static void run(){

    }
}
*/

/** Simple example of JNA interface mapping and usage. */
public class BTreeProxy {

    // This is the standard, stable way of mapping, which supports extensive
    // customization and mapping of Java to native types.

    public interface CLibrary extends Library {
        CLibrary INSTANCE = (CLibrary)
                Native.loadLibrary("clib",
                        CLibrary.class);

        //void printf(String format, Object... args);
        Boolean is_inside_of(int key);
        void insert_key(int key);
    }

    public int call()
    {
        Boolean b = CLibrary.INSTANCE.is_inside_of(1);
        if (b)
        {
            System.out.println("!");
        }
        CLibrary.INSTANCE.insert_key(1);
                CLibrary.INSTANCE.insert_key(3);
        return 0;
    }
}
