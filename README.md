# Stable-Marriage-Algorithm
Implements the Gale-Shapley stable marriage algorithm in C. The program reads in a file storing preference lists, and outputs the solution to the problem.

## Compilation Instructions
- navigate to the directory that contains the makefile
- run `make`
- This will create `./q2`

## Runtime Instructions
The **filename** for **Q2** is determined at runtime, the program will prompt you to enter a filename
- Run `./q2` for Gale-Shapley stable marriage algorithm

## Input file formats
Two example files have been included for your use. In general the format of the input file is as follows:
> [length of matrix (int)]  
> [n by n matrix, rows separated by newlines, columns separated by whitespace]  
> [newline]  
> [n by n matrix, rows separated by newlines, columns separated by whitespace]  
  
Example:  
> 4  
> 1  2  3  4  
> 1  4  3  2  
> 2  1  3  4  
> 4  2  3  1   
>   
> 4  3  1  2   
> 2  4  1  3  
> 4  1  3  2  
> 3  2  1  4  


## Gale-Shapley Stable Marriage Algorithm
The stable marriage problem is the problem of finding a stable matching between two equally sized sets of elements given an ordering of preferences for each element. A matching is a bijection from the elements of one set to the elements of the other set. A matching is not stable if:   
1. There is an element A of the first matched set which prefers some given element B of the second matched set over the element to which A is already matched, and
2. B also prefers A over the element to which B is already matched.

In other words, a matching is stable when there does not exist any pair (A, B) which both prefer each other to their current partner under the matching.

The stable marriage problem has been stated as follows:  
Given n men and n women, where each person has ranked all members of the opposite sex in order of preference, marry the men and women together such that there are no two people of opposite sex who would both rather have each other than their current partners. When there are no such pairs of people, the set of marriages is deemed stable.


## More Info
[More information on stable marriage problem](https://en.wikipedia.org/wiki/Stable_marriage_problem)  
[Code example](https://www.geeksforgeeks.org/stable-marriage-problem/)  
