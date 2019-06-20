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
    }

    public int call()
    {
        CLibrary.SINSTANCE.btree_init(2);
        CLibrary.SINSTANCE.btree_insert_key(1);
        CLibrary.SINSTANCE.btree_insert_key(2);
        CLibrary.SINSTANCE.btree_insert_key(3);
        CLibrary.SINSTANCE.btree_insert_key(4);
        CLibrary.SINSTANCE.btree_insert_key(5);
        CLibrary.SINSTANCE.btree_insert_key(6);
        CLibrary.SINSTANCE.btree_insert_key(7);
        CLibrary.SINSTANCE.btree_remove_key(1);
        CLibrary.SINSTANCE.btree_remove_key(2);
        CLibrary.SINSTANCE.btree_remove_key(7);
        boolean res = CLibrary.SINSTANCE.btree_is_inside_of(4);
        if (res)
        {
            System.out.println("YES");
        } else
        {
            System.out.println("NO");
        }
        return 0;
    }
}
