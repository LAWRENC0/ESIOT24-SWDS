package smartwastedisposal;

public class SmartWasteDisposalDashboardController {

    static final String MSG_EMPTIED = "empd";
    static final String MSG_RESTORED = "rstd";
    static final String MSG_START = "strt";

    SerialCommChannel channel;
    SmartWasteDisposalDashboardView view;
    LogView logger;

    public SmartWasteDisposalDashboardController(String port, SmartWasteDisposalDashboardView view, LogView logger)
            throws Exception {
        this.view = view;
        this.logger = logger;

        channel = new SerialCommChannel(port, 115200);
        new MonitoringAgent(channel, view, logger).start();

        /* attesa necessaria per fare in modo che Arduino completi il reboot */

        System.out.println("Waiting Arduino for rebooting...");
        Thread.sleep(4000);
        System.out.println("Ready.");

    }

    public void notifyEmptied() {
        channel.sendMsg(MSG_EMPTIED);
    }

    public void notifyRestored() {
        channel.sendMsg(MSG_RESTORED);
    }

    public void notifyStart() {
        channel.sendMsg(MSG_START);
        view.disableStartFunction();
    }
}