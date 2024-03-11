# Author
Nagy Botond-Vilmos

# Description
This is project that I made for the `Probability and Statistics` course at the at my university.

# Context
Suppose that we have a Rubik's cube, but with the faces of a standard die in the following way:
- the front face is 2
oxx
xxx
xxo
- the up face is 1
xxx
xox
xxx
- the right face is 3
oxx
xox
xxo
- the down face is 6
oxo
oxo
oxo
- the left face is 4
oxo
xxx
oxo
- the back face is 5
oxo
xox
oxo

# Problem
We want to find the probability of the following event that starting from the solved state, we scramble the cube by making `k` random turns, and we want to find the probability that there are exactly 3 faces that have exactly 4 dots on them in any position.

# Solution
I wrote a program that can run a number of n simulations and count the number of times the event occurs. Then the probability is the number of times the event occurs divided by n.
We can know that the calculated probability is correct by using the weak law of large numbers. This law states that the probability of an event is the limit of the relative frequency of the event as the number of trials goes to infinity. This means that if we run a large number of simulations, the probability of the event will be close to the calculated probability.

# Implementation
- I chose to solve the problem using C++ because it is still the most well language for me.
- For the solution I had to create a data structure that can represent the cube and the moves that can be made on it.

# Results
The base case is when n = 10000 and k = 10.
The probability of the event is about 18.6%.

# Usage
To run the program you need to have a C++ compiler installed on your machine. Then you can run the following commands:
```bash
g++ cube.cpp -o cube
./cube <n> <k>
```
where `n` is the number of simulations and `k` is the number of random turns that are made on the cube.

# Further development
I could make the program more efficient by using a better data structure to represent the cube and the moves that can be made on it. I could also make the program more user friendly by adding a GUI to it.

# Summary
This project was a great opportunity for me to practice my programming skills and to learn more about the weak law of large numbers. I also learned more about the Rubik's cube and how to represent it in a program.
You can play with the program and see how the probability changes as you increase the number of simulations and the number of random turns that are made on the cube.

[Back](../README.md)
