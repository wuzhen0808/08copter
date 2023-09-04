package dt;

import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;
import javax.swing.JFrame;

public class MyApp {
    public static void main(String[] args) {
        JFrame f = new JFrame();
        f.setLayout(new GridBagLayout());
        GridBagConstraints gbc = new GridBagConstraints();

        gbc.fill = GridBagConstraints.BOTH;
        gbc.gridx = 0;
        gbc.gridy = 0;
        gbc.weightx = 1;
        gbc.weighty = 0.5;
        gbc.gridwidth = 3;
        // f.add(new JButton("B2"), gbc);
        SerialSignalWindow source = new SerialSignalWindow().connect();
        SignalSeqView sView = new SignalSeqView(source);
        f.add(sView, gbc);

        gbc.gridy = 1;
        gbc.gridwidth = 1;
        SignalPointerView spView1 = new SignalPointerView(0, 0, 360, source);
        f.add(spView1, gbc);
        gbc.gridx = 1;
        SignalPointerView spView2 = new SignalPointerView(1, 0, 360, source);
        f.add(spView2, gbc);
        gbc.gridx = 2;
        SignalPointerView spView3 = new SignalPointerView(2, 0, 360, source);
        f.add(spView3, gbc);

        f.setSize(800, 500);
        f.setVisible(true);

    }
}
