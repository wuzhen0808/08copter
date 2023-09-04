package dt;


import java.awt.GridLayout;

import javax.swing.JFrame;

public class MyApp {
    public static void main(String[] args) {
        JFrame f = new JFrame();
        f.setLayout(new GridLayout(2, 1));
        SerialSignalSource source = new SerialSignalSource().connect();
        SignalSeqView sView = new SignalSeqView(source);
        
        f.add(sView);
        SignalPointerView spView = new SignalPointerView(0, 360, source);
        f.add(spView);

        f.setSize(800, 500);
        f.setVisible(true);

    }
}
