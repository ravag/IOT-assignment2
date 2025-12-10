package dru;

public class DashboardController {

    static final String REQUEST_LANDING = "rl";
    static final String REQUEST_TAKEOFF = "rtk";
    
    SerialCommChannel channel;
    DashBoardView view;
    LogView logger;
    DroneView drone;

    public DashboardController(String port, DashBoardView view, LogView logger, DroneView drone) throws Exception {
        this.view = view;
        this.logger = logger;
        this.drone = drone;

        channel = new SerialCommChannel(port, 115200);
        new MonitoringAgent(channel,view,logger,drone).start();

        System.out.println("Waiting Arduino for rebooting...");
        Thread.sleep(4000);
        System.out.println("Ready.");
    }

    public void requestLanding(){
        channel.sendMsg(REQUEST_LANDING);
    }

    public void requestTakeOff(){
        channel.sendMsg(REQUEST_TAKEOFF);
    }
}
