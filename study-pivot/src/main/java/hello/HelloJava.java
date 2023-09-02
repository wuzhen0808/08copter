package hello;

import org.apache.pivot.collections.Map;
import org.apache.pivot.wtk.Application;
import org.apache.pivot.wtk.DesktopApplicationContext;
import org.apache.pivot.wtk.Display;
import org.apache.pivot.wtk.Label;
import org.apache.pivot.wtk.Window;

public class HelloJava implements Application {
    private Window window = null;

    @Override
    public void startup(Display display, Map<String, String> properties) {
        window = new Window();

        Label label = new Label();
        label.setText("Hello World!");
        //label.getStyles().put(Style.font, new Font("Arial", Font.BOLD, 24));
        //label.getStyles().put(Style.color, Color.RED);
        //label.getStyles().put(Style.horizontalAlignment,
        //        HorizontalAlignment.CENTER);
        //label.getStyles().put(Style.verticalAlignment,
        //        VerticalAlignment.CENTER);

        window.setContent(label);
        window.setTitle("Hello World!");
        window.setMaximized(true);

        window.open(display);
    }

    @Override
    public boolean shutdown(boolean optional) {
        if (window != null) {
            window.close();
        }

        return false;
    }

    @Override
    public void suspend() {
    }

    @Override
    public void resume() {
    }

    public static void main(String[] args) {
        DesktopApplicationContext.main(HelloJava.class, args);
    }

}
