package dt;

public class MySignalSource implements SignalSource {

    @Override
    public float[] getData() {
        return new float[] { 1, 2, 3, 0 };
    }

}