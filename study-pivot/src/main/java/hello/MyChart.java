
package hello;


import org.apache.pivot.charts.LineChartView;
import org.apache.pivot.charts.ChartView.Category;
import org.apache.pivot.collections.ArrayList;
import org.apache.pivot.collections.List;
import org.apache.pivot.collections.Map;
import org.apache.pivot.wtk.Application;
import org.apache.pivot.wtk.DesktopApplicationContext;
import org.apache.pivot.wtk.Display;
import org.apache.pivot.wtk.Window;

public class MyChart implements Application {
    private Window window = null;

    @Override
    public void startup(Display display, Map<String, String> properties) {
        window = new Window();

        LineChartView chart = new LineChartView() ;
        List<Object> chartData = new ArrayList<>();
        chartData.add("1");
        chart.setChartData(chartData);
        Category cat1 = new Category("key1", "Lable1");
        chart.getCategories().add(cat1);
        
        //label.getStyles().put(Style.font, new Font("Arial", Font.BOLD, 24));
        //label.getStyles().put(Style.color, Color.RED);
        //label.getStyles().put(Style.horizontalAlignment,
        //        HorizontalAlignment.CENTER);
        //label.getStyles().put(Style.verticalAlignment,
        //        VerticalAlignment.CENTER);

        window.setContent(chart);
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
