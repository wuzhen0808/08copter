package dt;

import java.awt.Canvas;
import java.awt.Graphics;

public class SignalPointerView extends Canvas {
    int width;
    int height;
    int leftMargin = 40;
    int rightMargin = 40;
    int topMargin = 40;
    int botMargin = 40;
    int radius;

    float min;
    float max;

    float currentValue;

    public SignalPointerView(float min, float max) {
        this.min = min;
        this.max = max;
        this.currentValue = min;
    }

    @Override
    public void update(Graphics g){
        super.update(g);
    }
    
    @Override
    public void paint(Graphics g) {
        this.width = this.getWidth();
        this.height = this.getHeight();
        this.radius = Math.min(this.width - leftMargin - rightMargin, this.height-topMargin-botMargin) / 2;
        double angular = 2f * Math.PI * this.currentValue / (this.max - this.min);

        int x0 = this.width / 2;
        int y0 = this.height / 2;
        int[] x1y1 = getPointByAngular(angular, radius);
        g.drawLine(x0, y0, x1y1[0], x1y1[1]);

        // draw circleF
        g.drawOval(x0 - radius, y0 - radius, radius * 2, radius * 2);
        // draw dashes
        drawDash(g, 0);
        drawDash(g, 90);
        drawDash(g, 180);
        drawDash(g, 270);

    }

    void drawDash(Graphics g, int degree) {
        int[] x1y1 = getPointByAngular(degree, radius);
        g.drawString("" + degree, x1y1[0], x1y1[1]);
    }

    int[] getPointByAngular(int degree, int radius) {
        return getPointByAngular(2 * Math.PI * ((double) degree / 360d), radius);
    }

    int[] getPointByAngular(double angular, int radius) {
        int x1 = (int) (Math.cos(angular) * radius);
        int y1 = (int) (Math.sin(angular) * radius);
        x1 = -x1;
        int x11 = this.width / 2 + x1;
        int y11 = this.height / 2 - y1;
        return new int[] { x11, y11 };
    }

    int translateX(float x) {
        int centerX = this.width / 2;
        return centerX + (int) x;
    }

    int translateY(float y) {
        int centerY = this.height / 2;
        return centerY - (int) y;
    }

}
