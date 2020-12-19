LUsolve0 <- function(obj){
    res <- LUdecomp(obj)
    if(res[[2]] != 0) # matrix is singular, Ax=b doesn't have unique solution.
    {
        return(1)
    }
    
    pivot <- res[[1]]            #when Ax = b, Ludecomp decompse PA to LU.
    obj$vec <- obj$vec[pivot]  #so, for to solve PAx = Pb , convert b to Pb using pivot
    
    dimension <- dim(obj$mat)
    n <- dimension[1]
    
    
    # at LUx = Pb = c, in-place forward substitution to solve y, for Ly = c
    for (i in seq_len(n)) {
        for (j in seq_len(i-1)) {
            obj$vec[i] <- obj$vec[i] - obj$mat[i, j] * obj$vec[j]
        }
        # obj$vec[i] <- obj$vec[i] / obj$mat[i, i] <- we skip this part because L[i,i] = 1
    }
    
    #now obj$vec has y
    
    
    # at Ux = y, in-place backward substitution to solve x, for Ux = y
    for (i in rev(seq_len(n))) {
        for (j in i + seq_len(max(n - i, 0))) {
            obj$vec[i] <- obj$vec[i] - obj$mat[i, j] * obj$vec[j]
        }
        obj$vec[i] <- obj$vec[i] / obj$mat[i, i]
    }
    
    #now obj$vec has x
    
    return(0)
}

LUsolve <- function(A, b)
{
    obj <- new("LUclass", mat = A, vec = b)
    
    if(LUsolve0(obj) == 0)
    {
        return(obj$vec)
    }
    else
    {
        stop('A is singular matrix')
    }
    
}


a <- matrix(c(3,1,2,1),nrow=2,ncol=2)
b <- c(8,2)

solve(a,b)

LUsolve(a,b)


a <- matrix(c(1,2,3,2,-1,0,3,1,-1), nrow = 3)
b <- c(9,8,3)

solve(a,b)

LUsolve(a,b)