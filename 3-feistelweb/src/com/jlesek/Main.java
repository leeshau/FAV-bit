package com.jlesek;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;
import java.util.LinkedList;
import java.util.Scanner;

public class Main {

    static char[] file;
    static final LinkedList<String> keys = new LinkedList<>();

    public static void main(String[] args) {
	    if (args.length != 2){
	        System.err.println("Enter two args (file and keys).");
	        System.exit(1);
        }
	    loadFile(args[0]);
	    loadKeys(args[1]);
        int a = 4;
    }

    private static void loadKeys(String filename) {
        try {
            Scanner sc = new Scanner(new File(filename));
            while (sc.hasNextLine())
                keys.add(sc.nextLine().split("=")[1]);

        } catch (IndexOutOfBoundsException e) {
            System.err.println("Keys not formatted properly!");
            System.exit(4);
        } catch (FileNotFoundException e) {
            System.err.println("Keys file not found!");
            System.exit(5);
        }
    }

    private static void loadFile(String filename) {
        try {
            File f = new File(filename);
            FileReader fr = new FileReader(f);
            file = new char[(int) f.length()];
            int i, j = 0;
            while ( (i = fr.read()) != -1){
                file[j++] = (char) i;
            }
        } catch (FileNotFoundException e) {
            System.err.println("File not found!");
            System.exit(2);
        } catch (IOException e) {
            System.err.println("Could not read file!");
            System.exit(3);
        }
    }
}
