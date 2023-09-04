package dt;

public class MySignalWindow implements SignalWindow {

    @Override
    public float[] getData() {
        return new float[] { 1, 2, 3, 0 };
    }

    @Override
    public void addListener(Listener listener) {
        // TODO Auto-generated method stub
        throw new UnsupportedOperationException("Unimplemented method 'addListener'");
    }

}