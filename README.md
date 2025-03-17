# Programing Languages Project 2
- Semantic checker and code generator
- Contributor : Ari Palanjian
## Usage
- First run the command ```make```
- Next run the command ```make run``` to run all tests

## Error Handling
Errors are handled on the parser level where the expected token will signal if the syntax of the program is correct.
### Example Errors:
- Line 2 contains error invalid identifier a_.
- Line 3 contains error illegal redefinition b.
- Line 5 contains error expected {'identifier','end.','~','var'}, found undef_var.
- Line 4 contains error Unexpected token: undef_var "began".

## State
- Parser Identifies and handles new var syntax
- Added associated error messages
- Working: RPN Code Generation Stored in text file