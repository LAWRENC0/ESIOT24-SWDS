package smartwastedisposal;

public class MonitoringAgent extends Thread {

    SerialCommChannel channel;
    SmartWasteDisposalDashboardView view;
    LogView logger;

    static final String SYSTEM_PREFIX = "swd:";
    static final String LOG_PREFIX = "lo:";
    static final String MSG_STATE_PREFIX = "st:";

    static final String[] states = { "CLOSED-ACCEPTING", "OPEN-USER", "CLOSED-WAIT", "CLOSED-HTEMP",
            "CLOSED-FULL",
            "OPEN-OPERATOR",
            "CLOSED-OPERATOR" };

    static final String[] udt_states = { "WAKING-UP", "AWAKE", "ASLEEP" };

    public MonitoringAgent(SerialCommChannel channel, SmartWasteDisposalDashboardView view, LogView log)
            throws Exception {
        this.view = view;
        this.logger = log;
        this.channel = channel;
    }

    public void run() {
        boolean toBeEmptied = false;
        boolean toBeRestored = false;
        while (true) {
            try {
                String msg = channel.receiveMsg();
                if (msg.startsWith(SYSTEM_PREFIX)) {
                    String cmd = msg.substring(SYSTEM_PREFIX.length());
                    // logger.log("new command: "+cmd);

                    if (cmd.startsWith(MSG_STATE_PREFIX)) {
                        try {
                            String args = cmd.substring(MSG_STATE_PREFIX.length());

                            String[] elems = args.split(":");
                            if (elems.length >= 3) {
                                int stateCode = Integer.parseInt(elems[0]);
                                float wasteLev = Float.parseFloat(elems[1]);
                                String wasteLevel;
                                float temp = Float.parseFloat(elems[2]);
                                int userDetected = Integer.parseInt(elems[3]);
                                wasteLev = -100 * wasteLev + 110;
                                if (stateCode == 4 || wasteLev > 100) {
                                    wasteLevel = "100.0%";
                                } else if (wasteLev < 0.00) {
                                    wasteLevel = "0.0%";
                                } else {
                                    wasteLevel = (String.valueOf(wasteLev)).substring(0, 3) + "%";
                                }

                                view.setSystemState(states[stateCode]);
                                view.setUserDetectedState(udt_states[userDetected]);
                                if (stateCode == 3 && !toBeRestored) { // HIGH TEMP
                                    System.out.println("The container is HOT!");
                                    view.setCurrentTemperature("HOT");
                                    toBeRestored = true;
                                    view.setErrorState(true);
                                    view.toggleRestoreFunction(true);
                                } else if (stateCode != 3 && toBeRestored) {
                                    toBeRestored = false;
                                    view.setErrorState(false);
                                    view.setCurrentTemperature(String.valueOf(temp));
                                    view.toggleRestoreFunction(false);
                                } else if (stateCode == 4 && !toBeEmptied) { // FULL
                                    System.out.println("The container is FULL!");
                                    view.setWasteLevel("FULL");
                                    toBeEmptied = true;
                                    view.setErrorState(true);
                                    view.toggleEmptyFunction(true);
                                } else if (stateCode != 4 && toBeEmptied) {
                                    toBeEmptied = false;
                                    view.setErrorState(false);
                                    view.setWasteLevel(wasteLevel);
                                    view.toggleEmptyFunction(false);
                                } else if (stateCode != 3 && stateCode != 4) {
                                    view.setWasteLevel(wasteLevel);
                                    view.setCurrentTemperature(String.valueOf(temp));
                                    view.setErrorState(false);
                                }
                            }
                        } catch (Exception ex) {
                            ex.printStackTrace();
                            System.err.println("Error in msg: " + cmd);
                        }
                    } /*
                       * else {
                       * this.logger.log("UKN2: " + msg);
                       * }
                       */
                } else if (msg.startsWith(LOG_PREFIX)) {
                    this.logger.log(msg.substring(LOG_PREFIX.length()));
                } /*
                   * else {
                   * this.logger.log("UKN1: " + msg);
                   * }
                   */
            } catch (Exception ex) {
                ex.printStackTrace();
            }
        }
    }

}