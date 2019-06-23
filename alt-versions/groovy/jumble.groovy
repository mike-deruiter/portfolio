#!/usr/bin/env groovy

//TODO: Add insertWords() function

final def MAP_SIZE = 20
final def DICTIONARY = "/usr/share/dict/words"
final def PROG_NAME = "jumble"

WordMap wordMap = new WordMap(MAP_SIZE)

if (args.length == 0) {
    String line
    while (line = System.in.newReader().readLine()) {
        String[] words = line.tokenize(' ')
        for (word in words) {
            def sortedWord = sortString(word)
            wordMap.insert(sortedWord)
        }
    }
}

for (file in args) {
    if (file == "-") {
        String line
        while (line = System.in.newReader().readLine()) {
            String[] words = line.tokenize(' ')
            for (word in words) {
                def sortedWord = sortString(word)
                wordMap.insert(sortedWord)
            }
        }
    } else {
        try {
            new File(file).eachLine { line ->
                String[] words = line.tokenize(' ')
                for (word in words) {
                    def sortedWord = sortString(word)
                    wordMap.insert(sortedWord)
                }
            }
        } catch (FileNotFoundException e) {
            System.err.println "$PROG_NAME: Cannot open file $file."
        }
    }
}

try {
    new File(DICTIONARY).eachLine { word ->
        def sortedWord = sortString(word)
        if (wordMap.contains(sortedWord)) {
            println word
        }
    }
} catch (FileNotFoundException e) {
    System.err.println "$PROG_NAME: Cannot open dictionary."
    return
}

private String sortString(String word) {
    char[] w = word.toCharArray()
    Arrays.sort(w)

    return w.toString()
}

class WordMap {
    MapNode[] map
    int size

    public WordMap(int size) {
        this.size = size
        map = new MapNode[size]
    }

    public void insert(String s) {
        long key = convertToKey(s)

        int code = (int)(key % size)
        if (code < 0) {
            code *= -1
        }

        MapNode m = new MapNode(key, null)
        
        if (map[code] == null)
            map[code] = m;
        else {
            MapNode curr = map[code]
            while (curr.getNext() != null) {
                curr = curr.getNext()
            }
        }
    }

    public boolean contains(String s) {
        long key = convertToKey(s)

        int code = (int)(key % size)
        if (code < 0) {
            code *= -1
        }

        MapNode curr = map[code]

        while (curr != null) {
            if (curr.getKey() == key)
                return true;
            curr = curr.getNext()
        }

        return false
    }
    
    private long convertToKey(String s) {
        long key = 0
        def c_arr = s.toCharArray()

        for (c in c_arr)
            key = (key * 27) + ((int)c - (int)'a') + 1
        
        return key
    }

    private class MapNode {
        private long key
        private MapNode next

        public MapNode(long key, MapNode next) {
            this.key = key
            this.next = next
        }

        public long getKey() {
            return key
        }

        public MapNode getNext() {
            return next
        }

        public void setKey(long key) {
            this.key = key
        }

        public void setNext(MapNode next) {
            this.next = next
        }
    }
}
