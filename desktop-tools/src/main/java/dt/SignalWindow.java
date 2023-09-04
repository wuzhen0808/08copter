package dt;

interface SignalWindow {
    static interface Listener {
        void onData();
    }

    float[] getData();

    void addListener(Listener listener);
}