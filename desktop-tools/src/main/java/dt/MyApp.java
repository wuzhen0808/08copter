package dt;

import javax.swing.JFrame;

public class MyApp {
    public static void main(String[] args) {
        JFrame f = new JFrame();
        SignalSeqView sView = new SignalSeqView();
        f.add(sView);
        f.setSize(400, 500);
        f.setVisible(true);
    }
}
