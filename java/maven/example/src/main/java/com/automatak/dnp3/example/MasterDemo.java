package com.automatak.dnp3.example;

import com.automatak.dnp3.*;
import com.automatak.dnp3.impl.DNP3ManagerFactory;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;

/**
 * Example master than can be run against the example outstation
 */
public class MasterDemo {

    public static void main(String[] args) throws IOException, InterruptedException {

        // create the root class with a thread pool size of 1
        DNP3Manager manager = DNP3ManagerFactory.createManager(1);

        // You can send the log messages anywhere you want
        // but PrinintLogSubscriber just prints them to the console
        manager.addLogSubscriber(new PrintingLogSubscriber());

        // Create a tcp channel class that will connect to the loopback
        Channel channel = manager.addTCPClient("client", LogLevel.INFO, 5000, "127.0.0.1", 20000);

        // You can modify the defaults to change the way the master behaves
        MasterStackConfig config = new MasterStackConfig();

        // Create a master instance, pass in a simple singleton to print received values to the console
        Master master = channel.addMaster("master", LogLevel.INTERPRET, PrintingDataObserver.getInstance(), config);

        // This sub-interface can issue command requests
        CommandProcessor processor = master.getCommandProcessor();

        // all this stuff just to read a line of text in Java. Oh the humanity.
        String line = "";
        System.out.println("Enter a line of text (type 'quit' to exit): ");
        InputStreamReader converter = new InputStreamReader(System.in);
        BufferedReader in = new BufferedReader(converter);

        while (true) {
            System.out.println("Enter something to issue a command or type <quit> to exit");
            line = in.readLine();
            if(line.equals("quit")) break;
            else {
                ControlRelayOutputBlock crob = new ControlRelayOutputBlock(ControlCode.LATCH_ON, (short) 1, 100, 100, CommandStatus.SUCCESS);
                ListenableFuture<CommandStatus> future = processor.selectAndOperate(crob, 0);
                System.out.println("Control result: " + future.get().name());
            }
        }

        manager.shutdown();
    }
}
