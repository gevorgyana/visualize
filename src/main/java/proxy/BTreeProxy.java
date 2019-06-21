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
        void btree_remove_key(int key);
        boolean btree_is_inside_of(int key);
        void btree_release();
    }

    public void insert_key(int key)
    {
        CLibrary.SINSTANCE.btree_insert_key(key);
    }

    public void remove_key(int key)
    {
        CLibrary.SINSTANCE.btree_remove_key(key);
    }

    public boolean is_inside(int key)
    {
        return CLibrary.SINSTANCE.btree_is_inside_of(key);
    }

    public void init() {
            CLibrary.SINSTANCE.btree_init(2);
    }

    public void release() {
        CLibrary.SINSTANCE.btree_release();
    }
}
