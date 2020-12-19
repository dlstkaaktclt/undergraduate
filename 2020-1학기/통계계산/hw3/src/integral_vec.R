rectangular <- function(f, a, b, n){
    h <- (b - a) / (n + 1)   
    
    xi <- seq.int(a, b, length.out = n + 2)
    xi <- xi[-length(xi)]
    # get xi. it removes right endpoint of interval, b.
    
    #sum I_0 + ... I_{n} = h(sum(f(x_0) + ... f(x_n))), x_0 = a, x_{n+1} = b
    
    intgrl <- h * sum(f(xi))
    
    return(intgrl)
} 
trapezoidal <- function(f, a, b, n){
    h <- (b - a) / (n + 1)   
    
    xi <- seq.int(a, b, length.out = n + 2)
    xi <- xi[-1]
    xi <- xi[-length(xi)]
    # get xi. it removes right endpoint of interval, b.
    
    #sum I_0 + ... I_{n} = h/2 * (f(x_0) + 2f(x_1) + .. + 2f(x_n) + f(x_n+1), x_0 = a, x_n+1 = b
    
    intgrl <- h * (0.5 * f(a) + sum(f(xi)) + 0.5 * f(b))
    
    return(intgrl)
}
simpson <- function(f, a, b, n){
    interval_num = (n+1) %/% 2  
    
    h <- (b - a) / interval_num
    i <- seq_len(interval_num - 1)
    xi <- a + i * h 
    # get xi. it removes endpoint of interval, a, b
    xmid <- c(xi - h / 2, b - h / 2)
    # get xmid, midpoint of xi. it removes endpoint of interval, a, b
    
    
    #sum I_0 + ... I_{n} = h/6 * (f(x_0) + 4f({x_0 + x_1}/2) +2f(x_1) + 4f({x_1 + x_2}/2) + 2f(x_2) + ...   .. + 4f({x_n-1 + x_n}/2) + f(x_n+1)) x_0 = a, x_{n+1} = b
    
    intgrl <- h * (f(a) + 2 * sum(f(xi)) + 4 * sum(f(xmid)) + f(b)) / 6
    
    return(intgrl)
}

integral <- function(f, a, b, n, method){
    return(method(f,a,b,n))
}