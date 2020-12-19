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

Rcpp::cppFunction('int float32bin(double x) {
    	float flx = (float) x; 
    	unsigned int binx = *((unsigned int*)&flx); 
    	return binx; 
	}')

float32bin(1.0)

float32bin(-1.0)