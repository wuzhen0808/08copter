package dt;

import java.awt.Canvas;
import java.awt.Color;
import java.awt.Graphics;

public class SignalSeqView extends Canvas {

    static interface DataProvider {
        float[] getData();
    }

    static public class MyDataProvider implements DataProvider {

        @Override
        public float[] getData() {
            return new float[] { 1, 2, 3, 0 };
        }

    }

    DataProvider dataProvider = new MyDataProvider();
    int width;
    int height;
    int leftMargin = 40;
    int rightMargin = 40;
    int topMargin = 40;
    int botMargin = 40;
    float min;
    float max;
    int seqWindow = 100;
    int pointSize = 3;

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

    @Override
    public void paint(Graphics g) {

        width = this.getWidth();
        height = this.getHeight();
        this.setBackground(Color.WHITE);
        this.setForeground(Color.BLACK);
        float[] list = dataProvider.getData();
        if (list.length == 0) {
            min = Float.MIN_VALUE;
            max = Float.MAX_VALUE;
        } else {
            min = Float.MAX_VALUE;
            max = Float.MIN_VALUE;

            for (int i = 0; i < list.length; i++) {
                if (list[i] > max) {
                    max = list[i];
                }
                if (list[i] < max) {
                    min = list[i];
                }
            }
        }

        drawYAxis(g);

        for (int i = 0; i < list.length; i++) {
            int xI = translateToX(i);
            int yI = translateToYForDraw(list[i]);
            g.drawOval(xI, yI, pointSize, pointSize);
            g.fillOval(xI, yI, pointSize, pointSize);
        }
    }

}
