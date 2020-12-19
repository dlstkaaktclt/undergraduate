setRefClass("QRclass",
            fields = list(
                mat = "matrix",
                vec = "vector"
            )
)

sudo_sign <- function(x) # it is equal to sign, but when x is 0, it returns 1 to positive sign.
{
    if(sign(x) == 0) return(1)
    else return(sign(x))
}

householder <- function(qrobj){
    if(is.matrix(qrobj$mat))
    {
        dimension <- dim(qrobj$mat)
        n <- dimension[1]
        p <- dimension[2]
        
        if(n < p) # X has not full-column rank, we can't House holder.
        {
            return(NULL)
        }
        
        qrobj$vec <- rep(0,p)
        
        for (i in seq_len(p))
        {
            u <- qrobj$mat[i:n,i]
            u[1] <- u[1]+sudo_sign(u[1])*Matrix::norm(Matrix(u), "F")
            
            u_square <- sum(u*u)
            
            
            for (j in i - 1 + seq_len(max(p - i + 1, 0))) # in-place
            {
                qrobj$mat[i:n, j] <- qrobj$mat[i:n, j] - 2 * u * sum(u * qrobj$mat[i:n, j]) / u_square
                # re-scaling u's size to 1. I use u_square for preserve significant digit.
            }
            
            u <- u / u[1] # re-scaling make u[1] size to 1.
            qrobj$vec[i] <- sum(u * u) # save u'u 's value.
            if(length(u) != 1) # if u's size is 1, we don't need save u's value to X.
            {
                qrobj$mat[(i+1):n, i] = u[2:length(u)]
            }
            
        }
        
        return(qrobj)
    }
    return(NULL)
    
}

recoverQ <- function(qrobj)
{
    if(is.matrix(qrobj$mat))
    {
        dimension <- dim(qrobj$mat)
        n <- dimension[1]
        p <- dimension[2]
        
        if(n < p) # X has not full-column rank, we can't House holder.
        {
            return(NULL)
        }
        
        V = qrobj$mat
        V[upper.tri(V, diag = FALSE)] = 0
        diag(V) = 1
        V <- t(t(V[,1:p])/sqrt(qrobj$vec))  # set V. transpose to divide row by row, and re-transpose.
        
        T = matrix(0, ncol=p, nrow=p) # T_n is T_p
        T[1,1] = 2 # set T_1
        
        for(i in 1 + seq_len(p - 1))
        {
            T[1:(i-1),i] = -2 * T[1:(i-1),1:(i-1)] %*% t(V[,1:(i-1)]) %*% V[,i]
            T[i,i] = 2
        }
        
        return (diag(1, n)-V %*% T %*% t(V)) # return I - V_n T_n V_n^{t}. (Q)
    }
    
    return(NULL)
}

