package smartwastedisposal;

import java.awt.Font;
import java.util.Date;

import javax.swing.*;

class LogView extends JFrame {

    private JTextArea log;

    public LogView() {
        super("Log ");
        setSize(600, 600);

        JPanel mainPanel = new JPanel();
        log = new JTextArea(30, 40);
        log.setEditable(false);
        JScrollPane scrol = new JScrollPane(log);
        scrol.setFont(new Font("Arial", Font.PLAIN, 20));
        mainPanel.add(scrol);
        this.getContentPane().add(mainPanel);
    }

    public void log(String msg) {
        SwingUtilities.invokeLater(() -> {
            String date = new Date().toString();
            log.append("[" + date + "] " + msg + "\n");
        });
    }
}