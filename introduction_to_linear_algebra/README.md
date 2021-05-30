# Intro
Notes of "Introduction to Linear Algebra"

# Preface
## Ax = b
The vector `Ax` is **a combination of the columns of A**, x is a colum, such as: $$\begin{bmatrix} 1\\2 \end{bmatrix} \quad$$

The vector `xA` is **a combination of the rows of A**, x is a row, such as: $$\begin{bmatrix} 1&&2 \end{bmatrix} \quad$$

The solution vector x comes at three levels:
* Direct solution
* Matrix solution
* Particular solution plus nullspace solution

![matrix_alphabet](./pictures/matrix_alphabet.png)
* Augmented Matrix (增广矩阵)

# Introduction to Vectors

## Vectors and Linear Combinations
Linear algebra is built on these operations:
* adding vectors
* multiplying by scalars

We have three ways to describe vector `v`:
* two numbers
* arrow from (0, 0)
* point in the plane

## Lengths and Dot Products
* The length $\|v\|$ is the sequence root of $v \cdot v$

## Matrices
* [Four viewpoints of matrix multiplying: AB = C](./pictures/matrix_mult.png)
   * dot products

      $$ \begin{bmatrix} 1&&3\\2&&4 \end{bmatrix} \cdot \begin{bmatrix} x_1\\x_2 \end{bmatrix} = \begin{bmatrix} (1,3) \cdot (x_1, x_2) \\ (2,4) \cdot (x_1, x_2) \end{bmatrix} $$
   * columns of C are combinations(B) of columns of A

      $$ \begin{bmatrix} 1&&3\\2&&4 \end{bmatrix} \cdot \begin{bmatrix} x_1\\x_2 \end{bmatrix} = \begin{bmatrix} 1\\2 \end{bmatrix} x_1 + \begin{bmatrix} 3\\4 \end{bmatrix} x_2 $$
   * rows of C are combinations(A) of raws of B

      $$ \begin{bmatrix} x_1&&x_2 \end{bmatrix} \cdot \begin{bmatrix} 1&&3\\2&&4 \end{bmatrix} =  x_1 \begin{bmatrix} 1&&3 \end{bmatrix} + x_2 \begin{bmatrix} 2&&4 \end{bmatrix} $$
   * column of A * row of B

      $$ \begin{bmatrix} 1\\2 \end{bmatrix} \cdot \begin{bmatrix} 3&&4 \end{bmatrix} = \begin{bmatrix} 3&&4\\6&&8 \end{bmatrix} $$


* invertible matrix
   * independent columns: Ax = 0 has one solution(zero solutions)
* singular matrix
   * dependent columns: Cx = 0 has many solutions

# Solving Linear Equations

## Vectors and Linear Equations
* column picture of Ax = b
   * vector view
   * a combination of n columns of A (vectors) produces the vector b
* row picture of Ax = b
   * algebraic geometry view
   * m equations from m rows give m planes meeting at x, think about, in 3-dimensional coordinate, 3 planes get a point x

Four steps to understanding elimination using matrices:
* Elimination goes from A to a triangular U by a sequence of matrix steps Eij
   * Eij means that subtracts a multiple l of row j from row i
* The triangular system is solved by back substitution: working bottom to top
* In matrix language A is factored into LU = (lower triangular)(upper triangular)
* Elimination succeeds if A is invertible. (But it may need row exchanges.)

### What does it mean to "multiply A times x"?
* Multiplication by rows<br>
   ![mult_by_rows](./pictures/mult_by_rows.png)
* Multiplication by columns<br>
   ![mult_by_columns](./pictures/mult_by_columns.png)

## The idea of elimination
Breakdown of Elimination:
* No solution
   * Fails with no solution to 0y = 8<br>
   ![elimination_no_solution](./pictures/elimination_no_solution.png)
* Too many solutions
   * Fails with too many solutions to 0y = 0<br>
   ![elimination_many_solutions](./pictures/elimination_many_solutions.png)

## Rules for Matrix Operations

