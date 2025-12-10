package dru;

public class DashboardLauncher {

    static DashBoardView view = null;
    static LogView log = null;
    static DroneView drone = null;

    public static void main(String[] args) throws Exception {
        view = new DashBoardView();
        log = new LogView();
        drone = new DroneView();

        String portName = "COM23";
        DashboardController contr = new DashboardController(portName, view, log, drone);
        view.registerController(contr);

        view.display();
        log.display();
        drone.display();
    }

    
}
