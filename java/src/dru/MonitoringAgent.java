package dru;

public class MonitoringAgent extends Thread{
    
    SerialCommChannel channel;
    DashBoardView view;
    LogView logger;
    DroneView drone;

    static final String DRONE_PREFIX = "dr";
    static final String LOG_PREFIX = "lo";

    public MonitoringAgent(SerialCommChannel channel, DashBoardView view, LogView log, DroneView drone) throws Exception {
        this.view = view;
        this.logger = log;
        this.channel = channel;
        this.drone = drone;
    }

    public void run(){
        while(true) {
            try {
                String msg = channel.receiveMsg();
                if(msg.startsWith(DRONE_PREFIX)) {
                    String cmd = msg.substring(DRONE_PREFIX.length());
                    drone.droneMessage(">> " + cmd);
                } else if (msg.startsWith(LOG_PREFIX)) {
                    this.logger.log(msg.substring(LOG_PREFIX.length()));
                }
            } catch (Exception e) {
                e.printStackTrace();
            }
        }
    }
}
