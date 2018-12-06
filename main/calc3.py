#!/usr/bin/python3

#TODO: Throw specific exception in Parser.parse(), etc.

# calc3: Cook's Algebraic Calculator, version 3
#   A simple calculator that allows floating-point arithmetic with variables.
#   Variables are intialized with the command "load [VAR]". Memory is set
#   aside for a variable (giving it a default value of 0) with the command
#   "mem [VAR]". Variables are printed with the command "print [VAR]". The
#   calculator supports addition/subtraction, muliplication/division,
#   exponentiation, unary plus & minus, & a square root function. The
#   program is exited with the command "exit".

import sys, re, math

symbol_table = []
DEBUG = True

'''
The Grammar:

line           =   COMMAND VAR
                 | asgn
                 | addition
asgn           =   VAR '=' addition
addition       =   multiplication (('+' | '-') multiplication)*
multiplication =   exponentiation (('*' | '/') exponentiation)*
exponentiation =   unary ('^' unary)
unary          =   '-' function
                 | '+' function
                 | function
function       =   FUNCTION '(' addition ')'
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
            if cmd.value == "exit":
                return Binary_Token(None, cmd, None)
            if self.lexer.peek().token_type != "VAR":
                raise Exception
            return Binary_Token(None, cmd, self.lexer.next())
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
        
        return Binary_Token(var, op, right)
        
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
                
                expr = Binary_Token(expr, oper, right)
                
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
                
                expr = Binary_Token(expr, oper, right)
                
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
        
        return Binary_Token(left, e, right)
        
    def parse_unary(self):
        tkn = self.lexer.peek()
        if tkn.value != '-' and tkn.value != '+':
            return self.parse_function()
        self.lexer.next()
        
        right = self.parse_function() 

        tkn.token_type = "UNARY"         
        return Binary_Token(None, tkn, right)
    
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
       
        p = self.lexer.peek()
        
        if p.token_type != "PAREN" or p.value != ")":
            raise Exception
        
        self.lexer.next()

        return Binary_Token(None, tkn, right)
        
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
    cmds = ["load", "mem", "print", "exit"]
    funcs = ["sqrt"]
    
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
    
    def is_digit(self, c):
        return re.match(r"\d", c)
    
    def is_decimal_pt(self, c):
        return c == '.'
    
    def is_operator(self, c):
        try:
            "+-*/^".index(c)
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
        if self.is_cmd(word):
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
        
class Symbol:
    def __init__(self, tt, v):
        self.name = tt
        self.value = v

class Token:
    def __init__(self, tt, v):
        self.token_type = tt
        self.value = v

    #DEBUG
    def get_value(self):
        return self.value

class Binary_Token:
    def __init__(self, l, o, r):
        self.left = l
        self.op = o
        self.right = r
    
    #DEBUG
    def get_value(self):
        if self.left == None:
            return "%s (%s)" % (self.op.get_value(), self.right.get_value())
        return "(%s) %s (%s)" % (self.left.get_value(), 
                             self.op.get_value(), 
                             self.right.get_value())

def symbol_table_indexOf(var_name):
    i = 0
    for s in symbol_table:
        if s.name == var_name:
            return i
        i+=1
    
    return -1

def evaluate(tkn):
    if type(tkn) is Token:
        if tkn.token_type == "VAR":
            var_name = tkn.value
            if symbol_table_indexOf(var_name) == -1:
                print("Error: " + var_name + " doesn't exist")
                raise Exception
            else:
                return symbol_table[symbol_table_indexOf(
                       var_name)].value
        else:
            return tkn.value # Returns value of NUMBER tokens
    elif tkn.op.token_type == "COMMAND":
        if tkn.op.value == "exit":
            sys.exit()
        elif tkn.op.value == "load":
            var_name = tkn.right.value
            if not symbol_table_indexOf(var_name) == -1:
                print("Error: " + var_name + " already exists")
                raise Exception
            else:
                sys.stdout.write("Enter value for " + var_name + ": ")
                sys.stdout.flush()
                try:
                    user_input = sys.stdin.readline()
                except KeyboardInterrupt:
                    print()
                    raise Exception
                s = Symbol(var_name, float(user_input))
                symbol_table.append(s)
                raise Exception
        elif tkn.op.value == "mem":
            var_name = tkn.right.value
            if not symbol_table_indexOf(var_name) == -1:
                print("Error: " + var_name + " already exists")
                raise Exception
            else:
                s = Symbol(var_name, 0.0)
                symbol_table.append(s)
                raise Exception
        elif tkn.op.value == "print":
            var_name = tkn.right.value
            if symbol_table_indexOf(var_name) == -1:
                print("Error: " + var_name + " doesn't exist")
                raise Exception
            else:
                s = symbol_table[symbol_table_indexOf(var_name)]
                return s.value
    elif tkn.op.token_type == "EQUALS":
        var_name = tkn.left.value
        i = symbol_table_indexOf(var_name)
        if i == -1:
            print("Error: " + var_name + " doesn't exist")
            raise Exception
        b = evaluate(tkn.right)
        symbol_table[i].value = b
        raise Exception # Not really an error
    elif tkn.op.token_type == "UNARY":
        a = evaluate(tkn.right)
        if tkn.op.value == '-':
            return -1 * a
        else:
            return math.fabs(a)
    elif tkn.op.token_type == "FUNCTION":
        if tkn.op.value == "sqrt":
            a = evaluate(tkn.right)
            return math.sqrt(a)
    elif tkn.op.value == "+":
        a = evaluate(tkn.left)
        b = evaluate(tkn.right)
        return a + b
    elif tkn.op.value == "*":
        a = evaluate(tkn.left)
        b = evaluate(tkn.right)
        return a * b
    elif tkn.op.value == "-":
        a = evaluate(tkn.left)
        b = evaluate(tkn.right)
        return a - b
    elif tkn.op.value == "/":
        a = evaluate(tkn.left)
        b = evaluate(tkn.right)
        return a / b
    elif tkn.op.value == "^":
        b = evaluate(tkn.right)
        a = evaluate(tkn.left)
        return a ** b
    
user_input = "0"

while user_input != "":
    try:
        user_input = sys.stdin.readline()
    except KeyboardInterrupt:
        print()
        break

    input_stream = InputStream(user_input)
    parser = Parser(Lexer(input_stream))
    
    if not DEBUG:
        try:
            return_token = parser.parse()
        except Exception:
            print("Error: Syntax")
            continue
    else:
        # Debug Mode - don't catch exceptions
        return_token = parser.parse()
       
    try:
        ans = evaluate(return_token)
    except ValueError:
        print("Error: Invalid Value")
    except Exception:
        continue

    print("%s" % ans)
