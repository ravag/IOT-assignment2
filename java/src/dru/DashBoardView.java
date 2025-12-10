package dru;

import java.awt.BorderLayout;
import java.awt.Dimension;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.LayoutManager;
import java.awt.RenderingHints;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;
import java.util.LinkedList;

import javax.swing.*;

public class DashBoardView extends JFrame implements ActionListener{

    private JButton landing;
    private JButton takeOff;

    private DashboardController controller;
    
    public DashBoardView(){
        super(".:: Sweeping System ::.");
        setSize(600,150);
        this.setResizable(false);

        JPanel mainPanel = new JPanel();
        mainPanel.setLayout(new BoxLayout(mainPanel, BoxLayout.Y_AXIS));
        mainPanel.add(Box.createRigidArea(new Dimension(0,20)));

        JPanel buttonPanel = new JPanel();
        landing = new JButton("Drone Landing");
        landing.setEnabled(true);
        landing.addActionListener(this);
        takeOff = new JButton("Drone Taking Off");
        takeOff.setEnabled(true);
        takeOff.addActionListener(this);
        buttonPanel.setLayout(new BoxLayout(buttonPanel, BoxLayout.X_AXIS));
        buttonPanel.add(landing);
        buttonPanel.add(takeOff);

        mainPanel.add(buttonPanel);
        mainPanel.add(Box.createRigidArea(new Dimension(0,20)));
        setContentPane(mainPanel);

        setLocationRelativeTo(null);

        addWindowListener(new WindowAdapter(){
            public void windowClosing(WindowEvent e){
                System.exit(-1);
            }
        });
    }

    public void display(){
        SwingUtilities.invokeLater(() -> {
            this.setVisible(true);
        });
    }

    public void registerController(DashboardController contr){
        this.controller = contr;
    }

    @Override
    public void actionPerformed(ActionEvent e) {
        try {
            if(e.getSource() == landing) {
                controller.requestLanding();
            } else if (e.getSource() == takeOff) {
                controller.requestTakeOff();
            }
        } catch (Exception ex) {
            ex.printStackTrace();
        }
        
    }
    
}
