import java.io.File;
import java.io.FileNotFoundException;
import java.util.Scanner;
import java.util.Arrays;
import java.util.LinkedList;

public class Jumble {
    static final int MAP_SIZE = 20;
    static final String DICTIONARY = "/usr/share/dict/words";

    public static void main(String[] args) {
        String sortedWord;
    
        WordMap wordMap = new WordMap(MAP_SIZE);

        if (! (args.length > 0)) {
            System.err.println("Jumble: 1 or more arguments required; you " +
                               "supplied 0.");
            return;
        } else {
            for (String word : args) {
                sortedWord = sortString(word);
                wordMap.insert(sortedWord);
            }
        }

        File dict;
        Scanner dictScan;
        String dictWord;

        try {
            dict = new File(DICTIONARY);
            dictScan = new Scanner(dict);
        } catch (FileNotFoundException e) {
            System.err.println("Jumble: cannot open dictionary.");
            return;
        }
            
        while (dictScan.hasNext()) {
            dictWord = dictScan.next();
            sortedWord = sortString(dictWord);
            if (wordMap.contains(sortedWord)) {
                System.out.println(dictWord);
            }
        }

        dictScan.close();
    }

    private static String sortString(String word) {
        char[] w = word.toCharArray();
        Arrays.sort(w);

        return new String(w);
    }
}

// WordMap - Hash table data structure used internally by Jumble.
class WordMap {
    MapNode[] map;
    int size;

    WordMap(int size) {
        this.size = size;
        map = new MapNode[size];
    }    

    public void insert(String s) {
        // make a hash of the string
        long key = convertToKey(s);

        int code = (int)(key % size);
        if (code < 0) {
            code *= -1;
        }

        // insert the hash into the map                
        MapNode m = new MapNode(new Long(key), null);

        if (map[code] == null) {
            map[code] = m;
        } else {
            MapNode curr = map[code];
            while (curr.getNext() != null) {
                curr = curr.getNext();
            }
            curr.setNext(m);
        }
    }

    public boolean contains(String s) {
        // make a hash of the string
        long key = convertToKey(s);
        
        int code = (int)(key % size);
        if (code < 0) {
            code *= -1;
        }

        // search the map for the hash
        MapNode curr = map[code];

        while (curr != null) {
            if (curr.getKey() == key) {
                return true;
            }
            curr = curr.getNext();
        }
        
        return false;   // no match
    }

    // convertToKey - Hash algorithm for the map. Takes strings as input
    private long convertToKey(String s) {
        long key = 0;

        for (int i = 0; i < s.length(); ++i) {
            key = (key * 27) + (s.charAt(i) - 'a') + 1;
        }

        return key;
    }
}

class MapNode {
    private long key;
    private MapNode next;

    public MapNode(long key, MapNode next) {
        this.key = key;
        this.next = next;
    }

    // default constructor
    MapNode() {
        this(0, null);
    }

    public long getKey() {
        return key;
    }

    public MapNode getNext() {
        return next;
    }

    public void setKey(long key) {
        this.key = key;
    }

    public void setNext(MapNode next) {
        this.next = next;
    }
}
