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
