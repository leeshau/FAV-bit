package com.jlesek;

import java.io.*;
import java.nio.charset.StandardCharsets;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.LinkedList;
import java.util.Scanner;

public class Main {

    static final ArrayList<char[]> blocks = new ArrayList<>();
    static final LinkedList<String> keys = new LinkedList<>();

    private static final String OUTPUT_FILE = "output.txt";

    public static void main(String[] args) {
        if (args.length != 2) {
            System.err.println("Enter two args (file and keys).");
            System.exit(1);
        }
        loadFile(args[0]);
        loadKeys(args[1]);

        try {
            encipher();
            decipher();
        } catch (IOException e) {
            System.err.println("Something went wrong with writing out to the output file.");
            System.exit(7);
        }
    }

    private static void decipher() throws IOException {
        //reading ciphered text into blocks and converting it to binary
        blocks.clear();
        Scanner sc = new Scanner(new File(OUTPUT_FILE));
        while (sc.hasNext()) {
            int hex = sc.nextInt(16);
            String hexStr = Integer.toBinaryString(hex);
            blocks.add(hexStr.toCharArray());
        }
        FileOutputStream output = new FileOutputStream(OUTPUT_FILE, true);
//        while (sc.hasNext()) {
//            int hexInt = sc.nextInt(16);
//            String hexStr = Integer.toBinaryString(hexInt);
        for (char[] blockChars : blocks) {
            for (char c : blockChars) {
                char[] block = get8PlaceBin(Integer.toBinaryString(c));
                char[] L = splitBlock(block, true);
                char[] R = splitBlock(block, false);

                for (int i = keys.size() - 1; i >= 0; i--) {
                    char[] left = L.clone(), right = R.clone();
                    //left side
                    L = R.clone();
                    //right side
                    right = hashRightSide(right, keys.get(i).toCharArray());
                    R = xor(left, right);
                }
                String hex = Integer.toHexString(getByte(block)) + " ";
                output.write(hex.getBytes(StandardCharsets.UTF_8));
//                byte b = 0x00;
//                b |= 1 << 1;
//                b |= 1;
//                b &= ~(1 << 1);
            }
        }

        output.write('\n');

        for (char[] blockChars : blocks) {
            for (char c: blockChars)
                output.write(c);
        }
//        output.
        output.flush();
        output.close();

    }

    private static void encipher() throws IOException {
        FileOutputStream output = new FileOutputStream(OUTPUT_FILE, false);
        for (char[] stringBlock : blocks) {
            for (char c : stringBlock) {
                char[] block = get8PlaceBin(Integer.toBinaryString(c));
                char[] L = splitBlock(block, true);
                char[] R = splitBlock(block, false);

                for (String key : keys) {
                    char[] left = L.clone(), right = R.clone();
                    //left side
                    L = R.clone();
                    //right side
                    right = hashRightSide(right, key.toCharArray());
                    R = xor(left, right);
                }
                String hex = Integer.toHexString(getByte(block)) + " ";
                output.write(hex.getBytes(StandardCharsets.UTF_8));
//                byte b = 0x00;
//                b |= 1 << 1;
//                b |= 1;
//                b &= ~(1 << 1);

            }
        }
        output.write('\n');
        output.flush();
        output.close();
    }

    private static byte getByte(char[] block) {
        byte b = 0x00;
        for (int i = 7; i >= 0; i--) {
//            int a = Integer.parseInt(String.valueOf(block[i]));
//            if (a == 1)
//                b += Math.pow(2, i); //sort of hack to avoid stupid Java bitwise operations
            if (block[i] == '1')
//                b |= (-i + 7) << 1;
                b += Math.pow(2, (-i + 7)); //sort of hack to avoid stupid Java bitwise operations
        }
        return b;
    }

    private static char[] xor(final char[] left, final char[] right) {
        if (left.length != right.length) {
            System.err.println(Arrays.toString(left) + " and " + Arrays.toString(right) + " are not the same length!");
            System.exit(6);
        } else if (!String.valueOf(left).matches("[0-1]+") || !String.valueOf(right).matches("[0-1]+")) {
            System.err.println(Arrays.toString(left) + " or " + Arrays.toString(right) + " are not in proper format for xor!");
            System.exit(7);
        }

        char[] xored = new char[left.length];
        for (int i = 0; i < left.length; i++)
            xored[i] = left[i] == right[i] ? '0' : '1';
        return xored;
    }

    private static char[] hashRightSide(final char[] right, char[] key) {
        //xor with key
        char[] res = xor(right, key);
        //negate res
        for (int i = 0; i < res.length; i++)
            res[i] = res[i] == '0' ? '1' : '0';

        return res;
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
        for (int i = 0; i < 8; i++) {
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
