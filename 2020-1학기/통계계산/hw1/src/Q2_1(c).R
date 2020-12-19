pryr::bits(2^(-53))

x = 1.0 + 2^(-53)

options(digits=20)
print(x)

pryr::bits(x)