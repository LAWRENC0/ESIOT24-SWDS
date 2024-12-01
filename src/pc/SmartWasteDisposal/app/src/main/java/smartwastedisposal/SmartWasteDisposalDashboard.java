package smartwastedisposal;

import javax.swing.SwingUtilities;

class SmartWasteDisposalDashboardDashboard {

    static SmartWasteDisposalDashboardView view = null;
    static LogView log = null;

    public static void main(String[] args) throws Exception {
        /*
         * if (args.length != 1){
         * System.err.println("Args: <serial port>");
         * System.exit(1);
         * }
         */
        SwingUtilities.invokeAndWait(() -> {
            view = new SmartWasteDisposalDashboardView();
            log = new LogView();
        });
        String portName = "COM7";
        SmartWasteDisposalDashboardController contr = new SmartWasteDisposalDashboardController(
                portName, view, log);
        view.registerController(contr);
        SwingUtilities.invokeLater(() -> {
            view.setVisible(true);
            log.setVisible(true);
        });
    }
}