import java.util.*;
import java.util.concurrent.*; //注意，* 只导入当前包下的类，不会recursively导入子包

public class ProductorConsumer {
    public static void main(String[] args) {
        LinkedList<? super Integer> buffer = new LinkedList<Integer>();
        ExecutorService pool = Executors.newFixedThreadPool(15);
        for (int i = 0; i < 10; i++) {
            pool.submit(new Productor(buffer, 12));
        }
        for (int i = 0; i < 5; i++) {
            pool.submit(new Consumer(buffer));
        }
    }

    private static class Productor implements Runnable {
        private List<Integer> buffer;
        private int maxLength;
        public Productor(List list, int maxLength) { //注意，java中的数组、链表名字都是引用哦，可以理解为传入了指针
            this.buffer = list;
            this.maxLength = maxLength;
        }
        @Override
        public void run() {
            while (true) {
                synchronized (buffer) {
                    String curName = Thread.currentThread().getName();
                    while (buffer.size() == maxLength) {
                        System.out.println("满了，当前线程wait,苦了" + curName);
                        try{
                        buffer.wait();//wait() 的作用就是让已经拿到锁的线程主动暂停，并且释放锁,进入对应锁的专属的等待队列
                        //wait过后，thread重新竞争上锁了，会从之前wait的地方继续执
                        } catch (InterruptedException e) {}
                        System.out.println("我胡汉三又拿到锁开始霍霍了");
                    }
                    int addSize = 1;
                    System.out.print("加了 ");
                    buffer.add(1);
                    buffer.notifyAll();
                }
            }
        }
        //productor end
    }

    private static class Consumer implements Runnable {
        private List<Integer> buffer;
        public Consumer(List buffer) {
            this.buffer = buffer;
        }
        @Override
        public void run() {
            while(true) {
                int takeSize = 3;
                synchronized (buffer) {
                    while(buffer.size() < takeSize) {
                        System.out.println("空了没法拿了");
                        try{
                        buffer.wait();} catch (InterruptedException e) {}
                        System.out.println("又添了些，可以开始拿了");
                    }
                    System.out.print("拿！ |");
                    for (int i = 0; i < takeSize; i++) {
                        buffer.remove(0);
                    }
                    //拿了过后通知等候队列中的潜在对象
                    buffer.notifyAll();
                }
            }
        }

     }
}
