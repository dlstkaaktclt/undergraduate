pryr::bits(2^(-53) + 2^(-105))

y = 2^(-53) + 2^(-105)
x = 1 + y

print(x)

pryr::bits(x)