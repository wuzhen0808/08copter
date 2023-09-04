package dt;

import java.util.ArrayList;
import java.util.List;

import com.fazecast.jSerialComm.SerialPort;
import com.fazecast.jSerialComm.SerialPortEvent;
import com.fazecast.jSerialComm.SerialPortMessageListener;

public class SerialSignalSource implements SignalWindow, SerialPortMessageListener {

    SerialPort port;
    int nextIdx = 0;
    int length = 0;
    float[] window = new float[100];
    List<Listener> listeners = new ArrayList<>();

    public SerialSignalSource() {
    }

    public SerialSignalSource connect() {
        SerialPort port = SerialPort.getCommPort("COM4");
        port.setComPortParameters(9600, 8, 1, 0);
        port.setComPortTimeouts(SerialPort.TIMEOUT_NONBLOCKING, 0, 0);
        port.openPort(20);
        port.addDataListener(this);
        this.port = port;
        return this;
    }

    public static void main(String[] args) throws Exception {
        SerialSignalSource sss = new SerialSignalSource().connect();
        while (true) {
            Thread.sleep(1000);
        }
    }

    @Override
    public float[] getData() {
        return this.window;
    }

    @Override
    public void addListener(Listener listener) {
        this.listeners.add(listener);
    }

    @Override
    public int getListeningEvents() {
        return SerialPort.LISTENING_EVENT_DATA_RECEIVED;
    }

    @Override
    public void serialEvent(SerialPortEvent event) {
        byte[] messageB = event.getReceivedData();
        String msg = new String(messageB);
        String[] fields = msg.split(",");
        SerialSignalSource.this.window[nextIdx] = Float.parseFloat(fields[0]);
        nextIdx++;
        if (nextIdx == window.length) {
            nextIdx = 0;
        }
        for (Listener listener : this.listeners) {
            listener.onData();
        }

    }

    @Override
    public byte[] getMessageDelimiter() {
        return new byte[] { '\r', '\n' };
    }

    @Override
    public boolean delimiterIndicatesEndOfMessage() {
        return true;
    }
}
