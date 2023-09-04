package dt;

import java.util.ArrayList;
import java.util.List;

import com.fazecast.jSerialComm.SerialPort;
import com.fazecast.jSerialComm.SerialPortEvent;
import com.fazecast.jSerialComm.SerialPortMessageListener;

public class SerialSignalWindow implements SignalWindow, SerialPortMessageListener {

    SerialPort port;
    int nextIdx = 0;
    int length = 0;
    int windowSize = 10;
    float[][] window = new float[3][windowSize];
    List<Listener> listeners = new ArrayList<>();

    public SerialSignalWindow() {
    }

    public SerialSignalWindow connect() {
        SerialPort port = SerialPort.getCommPort("COM4");
        port.setComPortParameters(9600, 8, 1, 0);
        port.setComPortTimeouts(SerialPort.TIMEOUT_NONBLOCKING, 0, 0);
        port.openPort(20);
        port.addDataListener(this);
        this.port = port;
        return this;
    }

    @Override
    public float[][] getData() {
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
        processMessage(messageB);
    }

    protected void processMessage(byte[] messageB){
        String msg = new String(messageB);
        String[] fields = msg.split(",");
        SerialSignalWindow.this.window[0][nextIdx] = Float.parseFloat(fields[0]);
        SerialSignalWindow.this.window[1][nextIdx] = Float.parseFloat(fields[1]);
        SerialSignalWindow.this.window[2][nextIdx] = Float.parseFloat(fields[2]);
        nextIdx++;
        if (nextIdx == windowSize) {
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
