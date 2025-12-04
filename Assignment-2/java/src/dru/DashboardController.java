package dru;

public class DashboardController {
    
    SerialCommChannel channel;
    DashBoardView view;
    LogView logger;

    public DashboardController(String port, DashBoardView view, LogView logger) throws Exception {
        this.view = view;
        this.logger = logger;

        channel = new SerialCommChannel(port, 115200);
        new MonitoringAgent(channel,view,logger).start();

        System.out.println("Waiting Arduino for rebooting...");
        Thread.sleep(4000);
        System.out.println("Ready.");
    }
}
