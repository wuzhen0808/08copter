package dt;

import java.awt.Canvas;
import java.awt.Color;
import java.awt.Graphics;
import java.awt.Image;

public class SignalSeqView extends Canvas implements SignalWindow.Listener {
    static Color[] BGS = new Color[]{Color.RED, Color.GREEN, Color.BLUE};

    SignalWindow window;
    int width;
    int height;
    int leftMargin = 40;
    int rightMargin = 40;
    int topMargin = 40;
    int botMargin = 40;
    float min = Float.MAX_VALUE;
    float max = Float.MIN_VALUE;
    int seqWindow = 100;
    int pointSize = 2;

    Image offImage;

    SignalSeqView(SignalWindow window) {
        this.window = window;
        this.window.addListener(this);
    }

    @Override
    public void update(Graphics g) {
        if (this.offImage == null) {
            this.offImage = this.createImage(this.getWidth(), this.getHeight());
        }
        Graphics g2 = this.offImage.getGraphics();
        g2.clearRect(0, 0, this.getWidth(), this.getHeight());
        this.paint(g2);
        g.drawImage(this.offImage, 0, 0, null);
    }

    @Override
    public void paint(Graphics g) {

        width = this.getWidth();
        height = this.getHeight();
        this.setBackground(Color.WHITE);
        
        float[][] list = window.getData();
        for (int i = 0; i < list.length; i++) {

            for (int j = 0; j < list[i].length; j++) {
                if (list[i][j] > max) {
                    max = list[i][j];
                }
                if (list[i][j] < min) {
                    min = list[i][j];
                }
            }
        }
        
        g.setColor(Color.BLACK);
        drawYAxis(g);

        for (int i = 0; i < list.length; i++) {
            
            g.setColor(BGS[i]);
            
            for (int j = 0; j < list[i].length; j++) {
                int xI = translateToX(j);
                int yI = translateToYForDraw(list[i][j]);
                g.drawOval(xI, yI, pointSize, pointSize);
                // g.fillOval(xI, yI, pointSize, pointSize);
            }
        }

    }

    @Override
    public void onData() {
       this.repaint();
    }

    void drawYAxis(Graphics g) {

        int dashLength = 20;
        int x1 = leftMargin;
        int y1 = topMargin;
        int x2 = x1;
        int y2 = height - botMargin;

        float[] dashes = new float[] { min, max };

        // draw y axis
        g.drawLine(x1, y1, x2, y2);
        // draw dashes

        for (int i = 0; i < dashes.length; i++) {
            int yValue = translateToYForDraw(dashes[i]);
            g.drawLine(x1 - dashLength, yValue, x1, yValue);
            g.drawString("" + dashes[i], x1 - dashLength, yValue);
        }

    }

    int translateToYForDraw(float value) {
        int graphRangeY = height - topMargin - botMargin;
        float valueRange = max - min;
        int offsetY = (int) (((value - min) / valueRange) * graphRangeY);
        int yValue = topMargin + (graphRangeY - offsetY);

        return yValue;
    }

    int translateToX(int seq) {
        int graphRangeX = width - leftMargin - rightMargin;
        int offsetX = (int) (((float) seq / (float) seqWindow) * graphRangeX);
        int xValue = leftMargin + offsetX;
        return xValue;
    }

}
