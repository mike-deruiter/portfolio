#!/bin/env python3

# calc3: Cook's Algebraic Calculator, version 3
#   A simple calculator that allows floating-point arithmetic with variables.
#   Variables are intialized with the command "load [VAR]". Memory is set
#   aside for a variable (giving it a default value of 0) with the command
#   "mem [VAR]". Variables are printed with the command "print [VAR]". The
#   calculator supports addition/subtraction, muliplication/division,
#   exponentiation, unary plus & minus, & a square root function. The
#   program is exited with the command "exit".

#TODO: Throw specific exception in Parser.parse(), etc.
#TODO: Implement clear command
#TODO: Implement trigonomic functions
#TODO: Constants can be case-insensitive

# Features to include in calc5:
#   - Complex numbers
#   - Revised rt() function?

import sys, re, math

symbol_table = {}
DEBUG = True

'''
The Grammar:

line           =   COMMAND
                 | asgn
                 | addition
asgn           =   VAR '=' addition
                 | VAR
addition       =   multiplication (('+' | '-') multiplication)*
multiplication =   exponentiation (('*' | '/') exponentiation)*
exponentiation =   unary ('^' unary)
unary          =   '-' function
                 | '+' function
                 | function
function       =   FUNCTION '(' addition (',' addition) ')'
                 | primary
primary        =   '(' addition ')'
                 | NUMBER
                 | VAR
'''

class Parser():
    def __init__(self, l):
        self.lexer = l
    
    def parse(self):
        r = self.parse_line()
        if not self.lexer.eof():
            raise Exception
        else:
            return r
    
    def parse_line(self):
        if (self.lexer.peek().token_type == "COMMAND"):
            cmd = self.lexer.next()
            return Node(cmd, None, None)
        elif (self.lexer.peek().token_type == "VAR"):
            return self.parse_asgn()
        else:
            return self.parse_addition()
    
    def parse_asgn(self):
        if self.lexer.peek().token_type != "VAR":
            raise Exception
        var = self.lexer.next()
        
        if self.lexer.eof():
            return var
        
        op = self.lexer.next()
        
        right = self.parse_addition()
        
        return Node(op, var, right)
        
    def parse_addition(self):
        expr = self.parse_multiplication()

        o = self.lexer.peek()
        
        if self.lexer.eof():
            return expr
        
        if o.token_type == "PAREN" and o.value == ")":
            return expr
                     
        if o.token_type == "OP":
            while o.token_type == "OP":
                if o.value != '+' and o.value != '-':
                    return expr
                                
                oper = self.lexer.next()
                 
                right = self.parse_multiplication()
                
                expr = Node(oper, expr, right)
                
                o = self.lexer.peek()
                               
                if self.lexer.eof():
                    break

        return expr
    
    
    def parse_multiplication(self):
        expr = self.parse_exponentiation()
        
        o = self.lexer.peek()
        
        if self.lexer.eof():
            return expr
        
        if o.token_type == "PAREN" and o.value == ")":
            return expr
                
        if o.token_type == "OP":
            while o.token_type == "OP":
                if o.value != '*' and o.value != '/':
                    return expr
                    
                oper = self.lexer.next()
                
                right = self.parse_exponentiation()
                
                expr = Node(oper, expr, right)
                
                o = self.lexer.peek()
                
                if self.lexer.eof():
                    break
        
        return expr
        
    def parse_exponentiation(self):
        left = self.parse_unary()
        
        e = self.lexer.peek()
        
        if self.lexer.eof():
            return left
        
        if e.token_type != "OP" or e.value != '^':
            return left
        self.lexer.next()
        
        right = self.parse_unary()
        
        return Node(e, left, right)
        
    def parse_unary(self):
        tkn = self.lexer.peek()
        if tkn.value != '-' and tkn.value != '+':
            return self.parse_function()
        self.lexer.next()
        
        right = self.parse_function() 

        tkn.token_type = "UNARY"         
        return Node(tkn, right, None)
    
    def parse_function(self):
        tkn = self.lexer.peek()
        if tkn.token_type != "FUNCTION":
            return self.parse_primary()
        self.lexer.next()
        
        p = self.lexer.peek()
        
        if p.token_type != "PAREN" or p.value != "(":
            raise Exception
        self.lexer.next() 

        right = self.parse_addition()
        left = None
       
        p = self.lexer.peek()
        
        if p.token_type == "COMMA":
            self.lexer.next()
            left = self.parse_addition()
            p = self.lexer.peek()

        if p.token_type != "PAREN" or p.value != ")":
            raise Exception
        
        self.lexer.next()

        return Node(tkn, left, right)
        
    def parse_primary(self):
        p = self.lexer.peek()
        
        if p.token_type == "PAREN":
            if p.value != "(":
                raise Exception
            self.lexer.next()
            
            mid = self.parse_addition()
            
            p = self.lexer.peek()
            
            if p.token_type != "PAREN" or p.value != ")":
                raise Exception
            
            self.lexer.next()
            
            return mid
        if p.token_type == "VAR":
            self.lexer.next()
            return p
        if p.token_type == "NUMBER":
            self.lexer.next()
            return p
        
        raise Exception

class Lexer():
    curr = None
    cmds = ["exit", "clear"]
    funcs = ["rt", "log", "sin", "cos", "tan"]
    consts = {"PI": 3.1415926, "E": 2.718281}
    
    def __init__(self):
        self.input_stream = None
        self.has_input = False
        self.decimal_pt = False
    
    def __init__(self, i_s):
        self.input_stream = i_s
        self.has_input = True
        self.decimal_pt = False
        
    def new_input_stream(self, n_i_s):
        self.input_stream = n_i_s
        self.has_input = True
        self.decimal_pt = False
    
    def is_cmd(self, substr):
        if substr in self.cmds:
            return True
        else:
            return False
    
    def is_func(self, substr):
        if substr in self.funcs:
            return True
        else:
            return False

    def is_const(self, substr):
        if substr in self.consts.keys():
            return True
        else:
            return False
    
    def is_digit(self, c):
        return re.match(r"\d", c)
    
    def is_decimal_pt(self, c):
        return c == '.'
    
    def is_operator(self, c):
        try:
            "+-*×/÷^".index(c)
            return True
        except ValueError:
            return False
            
    def is_id(self, c):
        return re.match(r"[a-zA-Z]", c)
    
    def is_whitespace(self, c):
        return re.match(r"\s", "%s" % c)
        
    def is_number(self, c):
        if self.is_decimal_pt(c):
            if (self.decimal_pt):
                return False
            self.decimal_pt = True
            return True
        return self.is_digit(c)    
        
    def read_while(self, condition):
        str_rtn = ""
        while (not self.input_stream.eof() and 
               condition(self.input_stream.peek())):
            str_rtn += self.input_stream.next()
        return str_rtn
    
    def read_number(self):
         number = self.read_while(self.is_number)
         self.decimal_pt = False
         return Token("NUMBER", float(number))
        
    def read_id(self):
        word = self.read_while(self.is_id)
        if self.is_const(word):
            return Token("NUMBER", float(consts.get(word)))
        elif self.is_cmd(word):
            token_type = "COMMAND"
        elif self.is_func(word):
            token_type = "FUNCTION"
        else:
            token_type = "VAR"
        return Token(token_type, word)
        
    def read_next(self):
        self.read_while(self.is_whitespace)
        if (self.input_stream.eof()):
            return None
        
        c = self.input_stream.peek()
        if c == '(' or c == ')':
            self.input_stream.next()
            return Token("PAREN", c)
        if c == ',':
            self.input_stream.next()
            return Token("COMMA", c)
        if c == '=':
            self.input_stream.next()
            return Token("EQUALS", c)
        if self.is_decimal_pt(c) or self.is_digit(c):
            return self.read_number()
        if self.is_id(c):
            return self.read_id()
        if c == '/':
            self.input_stream.next()
            n = self.input_stream.peek()
            if n == '/':
                # skip comment
                self.has_input = False
                return None
            else:
                return Token("OP", '/')
        if self.is_operator(c):
            self.input_stream.next()
            if c == '×':
                c = '*'
            elif c == '÷':
                c = '/'
            return Token("OP", c)

    def peek(self):
        if not self.has_input:
            return None
        if self.curr:
            return self.curr
        else:
            self.curr = self.read_next()
            return self.curr
    
    def next(self):
        if not self.has_input:
            return None
        temp_token = self.curr
        self.curr = None
        if temp_token:
            return temp_token
        else:
            return self.read_next()

    def eof(self):
        if not self.has_input:
            return True
        f = self.peek()
        return f == None

class InputStream():
    def __init__(self, string):
        self.pos = 0
        self.str_in = str(string)
    
    def next(self):
        c = self.str_in[self.pos]
        self.pos += 1
        return c
        
    def peek(self):
        return self.str_in[self.pos]
        
    def eof(self):
        return (self.pos == len(self.str_in))
        
class Token:
    def __init__(self, tt, v):
        self.token_type = tt
        self.value = v

    def _debug_value(self):
        return self.value

class Node:
    def __init__(self, o, l, r):
        self.op = o
        self.left = l
        self.right = r
    
    def _debug_value(self):
        if self.right== None:
            return "%s (%s)" % (self.op._debug_value(), 
                             self.left._debug_value())
        return "%s (%s) (%s)" % (self.op._debug_value(), 
                             self.left._debug_value(), 
                             self.right._debug_value())

def evaluate(e):
    if type(e) is Token:
        if e.token_type == "VAR":
            var_name = e.value
            if symbol_table.get(var_name) == None:
                symbol_table[var_name] = 0.0
            return symbol_table[var_name]
        else:
            return e.value # Returns value of NUMBER tokens
    # Type of e assumed to be Node
    elif e.op.token_type == "COMMAND":
        if e.op.value == "exit":
            sys.exit()
        else:
            print("Error: " + var_name + " recognized, " +
                  "but not implemented.")
            raise Exception
    elif e.op.token_type == "EQUALS":
        b = evaluate(e.right)
        symbol_table[e.left.value] = b
        return None
    elif e.op.token_type == "UNARY":
        a = evaluate(e.right)
        if e.op.value == '-':
            return -1 * a
        else:
            return math.fabs(a)
    elif e.op.token_type == "FUNCTION":
        a = evaluate(e.right)
        if e.op.value == "log":
            if e.left == None:
                b = 10
            else:
                b = evaluate(e.left)
            return math.log(a, b)
        elif e.op.value == "rt":
            if e.left == None:
                b = .5
            else:
                b = 1 / evaluate(e.left)
            return a ** b
        else:
            print("Error: " + e.op.value + " recognized, " +
                  "but not implemented.")
            raise Exception
    elif e.op.value == "+":
        a = evaluate(e.left)
        b = evaluate(e.right)
        return a + b
    elif e.op.value == "*":
        a = evaluate(e.left)
        b = evaluate(e.right)
        return a * b
    elif e.op.value == "-":
        a = evaluate(e.left)
        b = evaluate(e.right)
        return a - b
    elif e.op.value == "/":
        a = evaluate(e.left)
        b = evaluate(e.right)
        return a / b
    elif e.op.value == "^":
        a = evaluate(e.left)
        b = evaluate(e.right)
        return a ** b

while True:
    try:
        user_input = sys.stdin.readline()
    except KeyboardInterrupt:
        print()
        break
    if user_input == "":
        continue
    input_stream = InputStream(user_input)

    parser = Parser(Lexer(input_stream))

    try:
        return_token = parser.parse()
    except Exception as debug:
        if not DEBUG:
            print("Error: Syntax")
            continue
        raise debug
       
    try:
        ans = evaluate(return_token)
    except Exception:
        continue
    if ans == None:
        continue

    if ans == math.floor(ans):
        ans = int(ans)

    print("%s" % ans)
