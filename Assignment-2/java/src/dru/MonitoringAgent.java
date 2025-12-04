package dru;

public class MonitoringAgent extends Thread{
    
    SerialCommChannel channel;
    DashBoardView view;
    LogView logger;

    static final String APP_PREFIX = "sw";
    static final String LOG_PREFIX = "lo";

    public MonitoringAgent(SerialCommChannel channel, DashBoardView view, LogView log) throws Exception {
        this.view = view;
        this.logger = logger;
        this.channel = channel;
    }

    public void run(){
        while(true) {
            try {
                String msg = channel.receiveMsg();
                if(msg.startsWith(APP_PREFIX)) {
                    String cmd = msg.substring(APP_PREFIX.length());
                    logger.log("new command: " + cmd);
                } else if (msg.startsWith(LOG_PREFIX)) {
                    this.logger.log(msg.substring(LOG_PREFIX.length()));
                }
            } catch (Exception e) {
                e.printStackTrace();
            }
        }
    }
}
