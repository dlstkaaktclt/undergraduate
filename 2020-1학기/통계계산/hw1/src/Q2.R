library(tidyverse) # install.packages('tidyverse')
library(lobstr)  # if not installed, use install.packages('lobstr')
# For unsigned integers
library(uint8) # devtools::install_github('coolbutuseless/uint8')
# For bitstrings
library(pryr)
# For big integers
library(gmp)
# For single precision floating point numbers
library(float)
library(Rcpp)


pryr::bits(1.0)

pryr::bits(1.0 + 2^{-52})
################
pryr::bits(2^(-53))

x = 1.0 + 2^(-53)

options(digits=20)
print(x)

pryr::bits(x)
#######################
pryr::bits(2^(-53) + 2^(-105))

y = 2^(-53) + 2^(-105)
x = 1 + y

print(x)

pryr::bits(x)
###########################

one = as.float(1)
small = as.float(2^(-24))
x = one + small
print(x)

print(as.numeric(x))
pryr::bits(as.numeric(x))



one = as.float(1)
small = as.float(2^(-24) + 2^(-47))
x = one + small
print(x)

print(as.numeric(x))
pryr::bits(as.numeric(x))
##############################
options(digits=20)
a <- 0.7
b <- 0.2
c <- 0.1

x <- (a + b) + c
print(x)

print(x == 1)

x <- a + (b + c)
print(x)

print(x == 1)

x <- (a + c) + b
print(x)

print(x == 1)



pryr::bits(a)
pryr::bits(b)
pryr::bits(c)

print(a)
print(b)
print(c)


x <- (b + c) + a
print(x)

