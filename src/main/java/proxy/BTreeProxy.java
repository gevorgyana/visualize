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
        void btree_insert_key(int key);
        void btree_init(int expansion_factor);
    }

    public int call()
    {
        CLibrary.SINSTANCE.btree_init(2);
        CLibrary.SINSTANCE.btree_insert_key(1);
        CLibrary.SINSTANCE.btree_insert_key(2);
        return 0;
    }
}
