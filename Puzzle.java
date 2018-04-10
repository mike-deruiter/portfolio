// Puzzle.java - Java implemention of puzzle.c

import java.util.Stack;

public class Puzzle {
    // constants
    private static final int SUM = 100;
    private static final int NUM_DIGITS = 9;
    
    // symbolic constants for operators
    private static final int NONE = 0;
    private static final int ADD  = '+' * -1;
    private static final int SUB  = '-' * -1;
     
    private static int operators[] = new int[NUM_DIGITS - 1];
    private static Stack<Integer> postfix = new Stack<Integer>();
    
    public static void main(String[] args) {
        
        initalizeOperators();
        
        solvePuzzle();
    }
    
    private static void initalizeOperators() {
        for (int i = 0; i < operators.length; ++i) {
            operators[i] = NONE;
        }
    }
    
    private static void solvePuzzle() {
        solvePuzzle(0);
    }
    
    private static void solvePuzzle(int position) {
        if (position >= operators.length) {
            return;
        }
        
        placeOperator(position, ADD);
        placeOperator(position, SUB);
        placeOperator(position, NONE);
    }
    
    private static void placeOperator(int position, int op) {
        operators[position] = op;
        genPostfix();
        
        if (evalPostfix() == SUM) {
            printExpression();
        } else {
            solvePuzzle(position + 1);
        }
    }
    
    private static void genPostfix() {
        int digit = 1;
        postfix.push(new Integer(digit++));
        
        Integer tempOp = null;
        
        for (int op : operators) {
            switch (op) {
                case ADD:
                case SUB:
                    postfix.push(new Integer(digit++));
                    postfix.push(new Integer(op));
                    break;
                default:
                    if (isOperator(postfix.peek())) {
                        tempOp = postfix.pop();
                    }
                    
                    int tempDig = (postfix.pop().intValue() * 10) + digit++;
                    postfix.push(new Integer(tempDig));
                    
                    if (isOperator(tempOp)) {
                        postfix.push(tempOp);
                    } // if
            }         // switch
        }             // for
    }                 // method
    
    private static int evalPostfix() {
        Integer a;
        Integer i = postfix.pop();
        
        switch (i.intValue()) {
            case ADD:
                a = postfix.pop();
                return evalPostfix() + a.intValue();
            case SUB:
                a = postfix.pop();
                return evalPostfix() - a.intValue();
            default:
                return i.intValue();
        }
    }
    
    private static void printExpression() {
        int digit = 1;
        
        System.out.print((char)('0' + digit++));
        for (int op : operators) {
            if (op != NONE) {
                System.out.print((char)(op * -1));
            }
            System.out.print((char)('0' + digit++));
        }
        
        System.out.print("\n");
    }
    
    private static boolean isOperator(Integer i) {
        if (i == null) {
            return false;
        }
        
        if (i.intValue() == ADD || i.intValue() == SUB) {
            return true;
        }
        
        return false;
    }
}
