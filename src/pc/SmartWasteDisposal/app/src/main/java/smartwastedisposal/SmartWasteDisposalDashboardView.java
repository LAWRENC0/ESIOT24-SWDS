package smartwastedisposal;

import java.awt.Color;
import java.awt.Dimension;
import java.awt.Font;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;

import javax.swing.*;

class SmartWasteDisposalDashboardView extends JFrame implements ActionListener {

    private JButton emptyButton;
    private JButton restoreButton;
    private JButton startButton;
    private JTextField wasteLevel;
    private JTextField currentTemperature;
    private JTextField detectedUser;

    private JTextField systemState;
    private SmartWasteDisposalDashboardController controller;

    public SmartWasteDisposalDashboardView() {
        super(".:: Smart Waste Disposal ::.");
        setSize(600, 150);
        Font myFont = new Font("Arial", Font.PLAIN, 20);

        JPanel mainPanel = new JPanel();
        mainPanel.setLayout(new BoxLayout(mainPanel, BoxLayout.Y_AXIS));
        mainPanel.add(Box.createRigidArea(new Dimension(0, 20)));

        JPanel infoLine = new JPanel();
        infoLine.setLayout(new BoxLayout(infoLine, BoxLayout.LINE_AXIS));
        systemState = new JTextField("--");
        systemState.setEditable(false);
        systemState.setPreferredSize(new Dimension(200, 15));
        systemState.setFont(myFont);
        infoLine.add(new JLabel("System State:"));
        infoLine.add(systemState);
        wasteLevel = new JTextField("--");
        wasteLevel.setEditable(false);
        wasteLevel.setPreferredSize(new Dimension(100, 15));
        wasteLevel.setFont(myFont);
        infoLine.add(new JLabel("Waste level:"));
        infoLine.add(wasteLevel);
        currentTemperature = new JTextField("--");
        currentTemperature.setEditable(false);
        currentTemperature.setPreferredSize(new Dimension(200, 15));
        currentTemperature.setFont(myFont);
        infoLine.add(new JLabel("Current Temperature:"));
        infoLine.add(currentTemperature);
        detectedUser = new JTextField("--");
        detectedUser.setEditable(false);
        detectedUser.setPreferredSize(new Dimension(200, 15));
        detectedUser.setFont(myFont);
        infoLine.add(new JLabel("User Detected:"));
        infoLine.add(detectedUser);

        mainPanel.add(infoLine);
        mainPanel.add(Box.createRigidArea(new Dimension(0, 20)));
        mainPanel.setPreferredSize(new Dimension(200, 20));

        JPanel buttonPanel = new JPanel();
        emptyButton = new JButton("Empty the Container");
        emptyButton.setEnabled(false);
        emptyButton.addActionListener(this);
        restoreButton = new JButton("Restore the Container");
        restoreButton.setEnabled(false);
        restoreButton.addActionListener(this);
        startButton = new JButton("START");
        startButton.setEnabled(true);
        startButton.addActionListener(this);

        buttonPanel.setLayout(new BoxLayout(buttonPanel, BoxLayout.X_AXIS));
        buttonPanel.add(emptyButton);
        buttonPanel.add(restoreButton);
        buttonPanel.add(startButton);

        mainPanel.add(buttonPanel);
        mainPanel.add(Box.createRigidArea(new Dimension(0, 20)));
        setContentPane(mainPanel);

        addWindowListener(new WindowAdapter() {
            public void windowClosing(WindowEvent ev) {
                System.exit(-1);
            }
        });
    }

    public void registerController(SmartWasteDisposalDashboardController contr) {
        this.controller = contr;
    }

    public void setSystemState(String msg) {
        SwingUtilities.invokeLater(() -> {
            systemState.setText(msg);
        });
    }

    public void setWasteLevel(String wLevel) {
        SwingUtilities.invokeLater(() -> {
            wasteLevel.setText("" + wLevel);
        });
    }

    public void setCurrentTemperature(String temp) {
        SwingUtilities.invokeLater(() -> {
            currentTemperature.setText("" + temp);
        });
    }

    public void setErrorState(boolean state) {
        SwingUtilities.invokeLater(() -> {
            if (state) {
                currentTemperature.setForeground(Color.RED);
                wasteLevel.setForeground(Color.RED);
                systemState.setForeground(Color.RED);
                detectedUser.setForeground(Color.RED);
            } else {
                currentTemperature.setForeground(null);
                wasteLevel.setForeground(null);
                systemState.setForeground(null);
                detectedUser.setForeground(null);
            }
        });
    }

    public void setUserDetectedState(String state) {
        SwingUtilities.invokeLater(() -> {
            detectedUser.setText("" + state);
        });
    }

    public void toggleEmptyFunction(boolean state) {
        SwingUtilities.invokeLater(() -> {
            emptyButton.setEnabled(state);
        });
    }

    public void toggleRestoreFunction(boolean state) {
        SwingUtilities.invokeLater(() -> {
            restoreButton.setEnabled(state);
        });
    }

    public void disableStartFunction() {
        SwingUtilities.invokeLater(() -> {
            startButton.setEnabled(false);
        });
    }

    /*
     * public void updateSamples(Sample[] samples) {
     * try {
     * SwingUtilities.invokeAndWait(() -> {
     * graphicPanel.updateSamples(samples);
     * });
     * } catch (Exception ex) {}
     * }
     * 
     * public void startMaintenanceForRefilling() {
     * SwingUtilities.invokeLater(() -> {
     * refill.setEnabled(true);
     * });
     * }
     * 
     * public void startMaintenanceForRecovering() {
     * SwingUtilities.invokeLater(() -> {
     * recover.setEnabled(true);
     * });
     * }
     */

    public void actionPerformed(ActionEvent ev) {
        try {
            if (ev.getSource() == emptyButton) {
                controller.notifyEmptied();
            } else if (ev.getSource() == restoreButton) {
                controller.notifyRestored();
            } else if (ev.getSource() == startButton) {
                controller.notifyStart();
            }
        } catch (Exception ex) {
            ex.printStackTrace();

        }
    }
}