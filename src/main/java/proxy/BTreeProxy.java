package proxy;

import com.sun.jna.Library;
import com.sun.jna.Native;
import com.sun.jna.Platform;

public class BTreeProxy {

    public interface CLibrary extends Library {
        CLibrary INSTANCE = (CLibrary)
                Native.loadLibrary("clib", CLibrary.class);
        CLibrary SINSTANCE = (CLibrary) Native.synchronizedLibrary
                (INSTANCE);

        // declarations
        void insert_key(int key);
    }

    public int call()
    {
        CLibrary.SINSTANCE.insert_key(1);
        CLibrary.SINSTANCE.insert_key(2);
        return 0;
    }
}
