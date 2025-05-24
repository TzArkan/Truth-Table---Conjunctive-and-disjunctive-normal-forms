# Truth-Table -> Conjunctive-and-disjunctive-normal-forms
This C++ code is designed to analyze and process logical (propositional) 
formulas, with the ultimate goal of generating a truth table and 
constructing the Conjunctive Normal Form (CNF) and 
Disjunctive Normal Form (DNF) based on a binary string 
that encodes the function's truth values.

The code accomplishes the following:

Defines logical operations:

Implication, disjunction, conjunction, equivalence, negation.

Represented as custom functions that operate on binary values (0/1).

Parses and evaluates logical formulas:

Converts an infix logical expression (e.g., p&q) into postfix (Reverse Polish Notation) using the Shunting Yard algorithm.

Evaluates the postfix expression for different combinations of truth values for the variables.

Builds and prints the truth table:

For a given logical formula or a hardcoded bit string (e.g., "0111110101011110"), it:

Identifies the variables used.

Iterates through all possible truth assignments.

Computes the formula's result for each assignment.

Displays a formatted truth table.

Constructs CNF and DNF expressions:

Based on the binary string:

0 in the truth table → contributes to the CNF.

1 in the truth table → contributes to the DNF.

Each row generates a corresponding clause depending on the variable values.
