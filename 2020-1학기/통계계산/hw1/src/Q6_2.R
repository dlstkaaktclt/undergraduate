options(digits = 20)

setRefClass("LUclass",
            fields = list(
                mat = "matrix",
                vec = "vector"
            )
)


LUdecomp <- function(obj, tol = 1e-8){
    if(is.matrix(obj$mat))
    {
        dimension <- dim(obj$mat)
        dim_row <- dimension[1]
        dim_col <- dimension[2]
        if(dim_row == dim_col) # do job when matrix is square
        {
            n <- dim_row
            pivot <- seq_len(n)
            
            for (k in seq_len(n-1)) 
            {
                max_row = k
                max_val = abs(obj$mat[k,k])
                for (i in k+seq_len(max(n - k, 0))) #find max number of row for partial pivoting
                {
                    row_val = abs(obj$mat[i,k])
                    if(row_val > max_val)
                    {
                        max_val = row_val
                        max_row = i
                    }
                }
                
                if(abs(max_val) < tol) # singular problem occured. just return error.
                {
                    return(list(pivot,k))
                }
                # need to implement pivot index
                
                if(max_row != k) # change row order of matrix and pivot, in place.
                {
                    temprow <- obj$mat[k,]
                    obj$mat[k,] <- obj$mat[max_row,]
                    obj$mat[max_row,] <- temprow
                    
                    temp <- pivot[k]
                    pivot[k] <- pivot[max_row]
                    pivot[max_row] <- temp
                }
                
                for (i in k+seq_len(max(n - k, 0))) 
                {
                    obj$mat[i, k] <- obj$mat[i, k] / obj$mat[k, k]
                    for (j in k+seq_len(max(n - k, 0))) 
                    {
                        obj$mat[i, j] <- obj$mat[i, j] - obj$mat[i, k] * obj$mat[k, j]
                    }
                }
            }
            
            if(obj$mat[n,n] < tol) # important!! code for check singularity at last row
            {
                return(list(pivot,n))
            }
            
            return(list(pivot,0))
            
        }
        else
        {
            return(list(NULL,1)) #need to return error value
        }
    }
    
    return(list(NULL,1))
}


A <- matrix(c(2,-3,-8,-6,-1,1,-1,7,-2), nrow=3)
b <- 1:3

obj <- new("LUclass", mat = A, vec = b)
ret <- LUdecomp(obj)

ret

obj$mat


C <- matrix(c(0,1,0,1,0,0,0,0,0), nrow=3)

obj$mat <- C

ret <- LUdecomp(obj)

ret

obj$mat