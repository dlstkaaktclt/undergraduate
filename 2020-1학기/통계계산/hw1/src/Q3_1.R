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
