package dru;

import javax.swing.*;

public class DroneView extends JFrame {

    private JTextArea drone;

    public DroneView(){
        super(".:: Messages to Drone ::.");
        setSize(600,400);
        this.setResizable(false);
        JPanel mainPanel = new JPanel();
        drone = new JTextArea(20,30);
        drone.setEditable(false);
        JScrollPane scroll = new JScrollPane(drone);
        mainPanel.add(scroll);
        this.getContentPane().add(mainPanel);

        this.setLocationRelativeTo(null);
    }

    public void droneMessage(String msg){
        SwingUtilities.invokeLater(() -> {
            drone.append( msg + "\n");
        });
    }

    public void display() {
        SwingUtilities.invokeLater(() -> {
            this.setVisible(true);
        });
    }
    
}

