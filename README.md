# Compiler-Design

Lexical Analyzer : Responsible for generating a stream of tokens <name,value> issued for each lexeme ( chunk of text ). It is also responsible for constructing the symbol Table.


![Compiler Flow Chart](https://user-images.githubusercontent.com/75173703/115982029-b50b1c80-a5b5-11eb-876d-10fc330a0fba.PNG)


# Lexical Analyser

Tokens ( NFA ) : <name,value> ;

eg. <RELOP,EQ>, <num,80>, <if>
  
For our purpose, we have considered 8 Lexical Categories. Actual Number of lexical categories, may vary and can be quite large.

![Lexical Categories](https://user-images.githubusercontent.com/75173703/115982032-b63c4980-a5b5-11eb-8696-f38bf054082c.PNG)

List Order of Lexical Categories with the types of token's issued by them :

![Order and Type of Tokens](https://user-images.githubusercontent.com/75173703/115982139-572b0480-a5b6-11eb-9f26-22096649f835.PNG)

Here I have implemented NFA's using Switch-case statements. However, the NFA for Keywords is implemented using Trie data-structure.

Also I have implemenented a Symbol Table using Class and a Hash-Map. Symbol table stores the variables and issues token number for variables depending on the row number.

Also, for generating tokens and navigating through the source text file, we have two techniques : Serial and Parallel.

If for a lexemme, no token is issues a Lexical Error is declared !

## NFA's Example

- Relational Operators category : RELOP

![RELOP NFA](https://user-images.githubusercontent.com/75173703/115982180-af620680-a5b6-11eb-92df-af9dd976eb20.PNG)

- Variable

![VAR NFA](https://user-images.githubusercontent.com/75173703/115982198-d1f41f80-a5b6-11eb-8488-e2fa4a285a05.PNG)

- Keywords

![KEYWORD NFA](https://user-images.githubusercontent.com/75173703/115982263-55157580-a5b7-11eb-9c4b-d0fc934e26a5.PNG)

- Numbers

![NUMBERS NFA](https://user-images.githubusercontent.com/75173703/115982305-a58cd300-a5b7-11eb-8c09-7122bf4826ad.PNG)

- Whitespaces

![WS NFA](https://user-images.githubusercontent.com/75173703/115982315-c3f2ce80-a5b7-11eb-890a-72430bb8989a.PNG)


## Psuedo code for NFA's

![NFA Psedo Code](https://user-images.githubusercontent.com/75173703/115982242-2d261200-a5b7-11eb-84b5-d6351f92a708.PNG)

## Serial Navigation

NFA which says yes first wins. ( NFA's are applied in decreasing order ).

![NFA Order](https://user-images.githubusercontent.com/75173703/115982389-3368be00-a5b8-11eb-92bb-578724b0483e.PNG)


## Parallel Navigation

Combine All NFA's in one common diagram.

In case of multiple Yes :
- Go for the longest lexeme in case of Multiple Yes
- Go for Order, if the length is also same



# Syntax Analyser

Parses the string to check if the rule of grammar were followed, and if followed - creates a parse tree for the input string.


## LL(1) Parser

# References

[Compilers Priciples, Techniques & Tools - Second Edition : Alfred V. Ahno, Monica S. Lam, Ravi Sethi, Jeffrey D. Ullman](https://en.wikipedia.org/wiki/Compilers:_Principles,_Techniques,_and_Tools#:~:text=Compilers%3A%20Principles%2C%20Techniques%2C%20and%20Tools%20is%20a%20computer%20science,compiler%20construction%20for%20programming%20languages.)
