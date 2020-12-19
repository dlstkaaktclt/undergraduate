rectangular <- function(f, a, b, n){
    h <- (b - a) / (n + 1)   
    
    xi <- vector(length = n)
    
    for(i in seq_len(n))
    {
        xi[i] <- a + i*h
    }
    
    # get xi. it removes endpoint of interval, a, b.
    
    #sum I_0 + ... I_{n} = h(sum(f(x_0) + ... f(x_n))), x_0 = a, x_{n+1} = b
    
    intgrl <- f(a)
    
    for(i in seq_len(n))
    {
        intgrl <- intgrl + f(xi[i])
    }
    
    intgrl <- h * intgrl
    
    return(intgrl)
} 
trapezoidal <- function(f, a, b, n){
    h <- (b - a) / (n + 1)   
    
    
    xi <- vector(length = n)
    
    for(i in seq_len(n))
    {
        xi[i] <- a + i*h
    }
    
    # get xi. it removes endpoint of interval, a, b.
    
    #sum I_0 + ... I_{n} = h/2 * (f(x_0) + 2f(x_1) + .. + 2f(x_n) + f(x_n+1)), x_0 = a, x_n+1 = b
    
    intgrl <- (f(a) + f(b))/2
    
    for(i in seq_len(n))
    {
        intgrl <- intgrl + f(xi[i])
    }
    
    intgrl <- h * intgrl
    
    return(intgrl)
}
simpson <- function(f, a, b, n){
    interval_num = (n+1) %/% 2  
    h <- (b - a) / interval_num
    
    xi <- vector(length = interval_num - 1)
    xmid <- vector(length = interval_num)
    
    for(i in seq_len(n))
    {
        xi[i] <- a + i*h
    }
    
    for(i in seq_len(interval_num - 1))
    {
        xi[i] <- a + i*h # get xi. it removes endpoint of interval, a, b
        xmid[i] <- xi[i]-(h / 2)
    }
    
    xmid[interval_num] = b - h / 2
    # get xmid, midpoint of xi. it removes endpoint of interval, a, b
    
    #sum I_0 + ... I_{n} = h/6 * (f(x_0) + 4f({x_0 + x_1}/2) +2f(x_1) + 4f({x_1 + x_2}/2) + 2f(x_2) + ...   .. + 4f({x_n-1 + x_n}/2) + f(x_n+1)) x_0 = a, x_{n+1} = b
    
    intgrl <- f(a) + f(b) + 4 * f(xmid[interval_num])
    
    for(i in seq_len(interval_num - 1))
    {
        intgrl <- intgrl + 2 * f(xi[i]) + 4 * f(xmid[i])
    }
    
    intgrl <- intgrl * h / 6
    
    return(intgrl)
}

integral <- function(f, a, b, n, method){
    return(method(f,a,b,n))
}