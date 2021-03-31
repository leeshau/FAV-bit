package com.jlesek;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;
import java.util.ArrayList;
import java.util.LinkedList;
import java.util.Scanner;

public class Main {

    static final ArrayList<char[]> blocks = new ArrayList<>();
    static final LinkedList<String> keys = new LinkedList<>();

    public static void main(String[] args) {
        if (args.length != 2) {
            System.err.println("Enter two args (file and keys).");
            System.exit(1);
        }
        loadFile(args[0]);
        loadKeys(args[1]);

        encipher();
        int a = 4;
    }

    private static void encipher() {
        for (char[] Stringblock : blocks) {
            for (char ch : Stringblock) {
                char[] block = get8PlaceBin(Integer.toBinaryString(ch));
                char[] L = splitBlock(block, true);
                char[] R = splitBlock(block, false);

                for (String key : keys) {
                    char[] left = L.clone(), right = R.clone();

                    //left side
                    L = R.clone();

                    //right side
                    right = hashRightSide(right);
                    R = xor(left, right);

                    int a = 4;
                }
            }
        }
    }

    private static char[] xor(final char[] left, final char[] right) {
        if (left.length != right.length) {
            System.err.println(left + " and " + right + " are not the same length!");
            System.exit(6);
        } else if (!String.valueOf(left).matches("[0-1]+") || !String.valueOf(right).matches("[0-1]+")) {
            System.err.println(left + " or " + right + " are not in proper format for xor!");
            System.exit(7);
        }

        char[] xored = new char[left.length];
        for (int i = 0; i < left.length; i++)
            xored[i] = left[i] == right[i] ? '0' : '1';
        return xored;
    }

    private static char[] hashRightSide(char[] right) {

    }

    private static char[] splitBlock(char[] block, boolean left) {
        char[] side = new char[4];
        for (
                int i = (left ? 0 : 4), j = 0;
                i < (left ? 4 : 8);
                i++, j++
        )
            side[j] = block[i];
        return side;
    }

    private static char[] get8PlaceBin(String string) {
        char[] res = new char[8];
        for (int i = 0; i < 8 ; i++){
            res[res.length - 1 - i] =
                    string.length() <= i ?
                            '0' :
                            string.charAt(string.length() - 1 - i);
        }
        return res;
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
            char[] file = new char[(int) f.length()];
            int i, j = 0;
            while ((i = fr.read()) != -1) {
                file[j++] = (char) i;
            }
            char[] block = new char[8];
            int rest;
            for (i = 0; i < file.length; i++) {
                rest = i % 8;
                block[rest] = file[i];
                if (rest == 7)
                    blocks.add(block.clone());
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
