package dt;

import java.util.concurrent.BlockingQueue;
import java.util.concurrent.LinkedBlockingQueue;
import java.util.concurrent.TimeUnit;

public class RealTimeSerialSignalWindow extends SerialSignalWindow implements Runnable {

    RealTimeSerialSignalWindow() {

    }

    BlockingQueue<byte[]> messageQueue = new LinkedBlockingQueue<>();

    Thread thread;

    @Override
    public SerialSignalWindow connect() {
        super.connect();
        thread = new Thread(this);
        thread.start();
        return this;
    }

    @Override
    protected void processMessage(byte[] messageB) {
        try {
            if (this.messageQueue.size() > 1000) {
                this.messageQueue.clear();
            }
            this.messageQueue.put(messageB);
        } catch (InterruptedException e) {
            throw new RuntimeException(e);
        }
    }

    @Override
    public void run() {
        while (true) {

            byte[] msg = null;
            try {
                msg = this.messageQueue.poll(1, TimeUnit.SECONDS);
            } catch (InterruptedException e) {

            }

            while (msg == null) {
                try {
                    msg = this.messageQueue.take();
                } catch (InterruptedException e) {

                }
            }
            byte[] next = null;
            while (true) {
                next = this.messageQueue.poll();
                if (next == null) {
                    break;
                }
                msg = next;
            }

            super.processMessage(msg);
        }
    }

}
