package dt;

import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.JComboBox;
import javax.swing.JFrame;

public class MyApp {

    public static enum DegreeMode {
        MODE_360("0-360", 0, 360),
        MODE_2PI("0-2π", 0, (float) (2f * Math.PI));

        String title;
        float min;
        float max;

        DegreeMode(String title, float min, float max) {
            this.title = title;
            this.min = min;
            this.max = max;
        }

        float getAngular(float value) {
            if (this == MODE_2PI) {
                return value;
            } else if (this == MODE_360) {
                return (float) (2f * Math.PI * value / (this.max - this.min));
            } else {
                throw new RuntimeException();
            }
        }
    }

    public static void main(String[] args) {
        new MyApp().run();
    }
    SignalPointerView[] spViews;

    public void run() {

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
        //
        
        JComboBox<DegreeMode> cb = new JComboBox<>(DegreeMode.values());
        cb.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                DegreeMode mode = (DegreeMode)cb.getSelectedItem();
                for(SignalPointerView sPointerView:spViews){
                    sPointerView.updateDegreeMode(mode);
                }
            }

        });
        f.add(cb, gbc);
        //
        gbc.gridy = 2;
        gbc.gridwidth = 1;
        spViews = new SignalPointerView[3];
        spViews[0] = new SignalPointerView(0, DegreeMode.MODE_360, source);
        f.add(spViews[0], gbc);
        gbc.gridx = 1;
        spViews[1] = new SignalPointerView(1, DegreeMode.MODE_360, source);
        f.add(spViews[1], gbc);
        gbc.gridx = 2;
        spViews[2] = new SignalPointerView(2, DegreeMode.MODE_360, source);
        f.add(spViews[2], gbc);

        f.setSize(800, 500);
        f.setVisible(true);

    }
}
