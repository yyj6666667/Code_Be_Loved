/*用来singleton的教学 */

//eager

public class EagerSingleton {
    private static EagerSingleton instance = new EagerSingleton(); //
    private EagerSingleton() {} //关闭全局创建的可能
    public static EagerSingleton getInstance() {
        return instance;
    }
}

//lazy

public class LazySingleton {
    private static Lazysingleton instance = null;
    private LazySingleton() {};
    public static synchronized LazySingleton getInstance () {
        if (instance == null) {
            instance = new LazySingleton();
        }
        return instance;
    }
}

public class Singleton {
    private volatile static Singleton instance = null;
    private Singleton() {}
    public static Singleton getSingleton() {
        if (instance == null) {
            synchronized(Singleton.class) { //必须选择一个所有线程都能访问到的、并且是唯一的对象来作为锁。对于一个静态方法来说，这个类本身的 Class 对象 (Singleton.class) 是最完美、最自然的选择
                if (instance == null)       //开始把二次检查写漏了，是必须的
                    instance = new Singleton();
            }
        }
        return instance;
    }
}
