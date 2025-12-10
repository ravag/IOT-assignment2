package dru;

public class DashboardLauncher {

    static DashBoardView view = null;
    static LogView log = null;

    public static void main(String[] args) throws Exception {
        view = new DashBoardView();
        log = new LogView();

        String portName = "COM4";
        DashboardController contr = new DashboardController(portName, view, log);
        view.registerController(contr);

        view.display();
        log.display();
    }

    
}
