We implemented a scanner that reads through the characters, and generates tokens for our expression.
Next we implented a parser that detects if the expression is defined in the language
Then, we evaluated the result using a stack .
The Implemented Grammar

X -> AY                                     =
Y -> ;X | EPSILON                           =
A -> BC                                     =
C -> +A | -A | *A | /A | %A | ^A | EPSILON  =
B ->(A) | NUM                               =
=============================================
