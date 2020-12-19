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

expTaylor <- function(x){
    stop <- 100
    ex <- 1
    xi <- 1
    ifac <- 1
    for (i in 1:stop) {
        xi <- xi * x
        ifac <- ifac * i
        ex <- ex + xi / ifac
    }
    ex
}

options(digits = 20)

expTaylor(20.0)

print(expTaylor(-20.0))

print(1/expTaylor(20.0))

print(exp(-20))

relative_error <- function(assume_val,true_val){
    abs(assume_val - true_val) / abs(true_val)
}


true_val = exp(-20.0)

custom_relative_error_first = relative_error(expTaylor(-20.0), true_val)
custom_relative_error_second = relative_error(1/expTaylor(20.0), true_val)

print(custom_relative_error_first)
print(custom_relative_error_second)

############################################
cum_expTaylor <- function(x){
    stop <- 100
    ex <- 1
    xi <- 1
    ifac <- 1
    res <- 1
    for (i in 1:stop) {
        xi <- xi * x
        ifac <- ifac * i
        ex <- ex + xi / ifac
        res <- c(res, ex)
    }
    res
}

get_relative_err_vec <- function(x) {
    cum_vec <- cum_expTaylor(x)
    true_val <- exp(x)
    relative_vec <- vector()
    
    for(i in 1:101)
    {
        relative_vec <- c(relative_vec,relative_error(cum_vec[i], true_val))
    }
    relative_vec
}

plot_relative_error <- function(x) {
    relative_vec <- get_relative_err_vec(x)
    relative_tb <- tibble(i = 0:100, relative_err = relative_vec)
    ggplot(relative_tb, aes(x = i, y = relative_err)) + geom_point() + labs(title = sprintf("relative error for i = 0 to 100, at x = %d",x))
}

plot_relative_error(20)

plot_relative_error(-20)

cum_expTaylor(-20)