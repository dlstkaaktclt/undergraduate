(* exercise 1-2 *)
	
let rec sumprod ((f, n, k) : ((int*int -> float) * int * int)) : float =
	let rec prod ((fp, i, kp) : ((int*int -> float) * int * int)) : float =
		if (kp < 1) then 1.0
		else (fp (i,kp)) *. prod (fp,i,kp-1) in
	if (n < 1) then 0.0
	else prod (f,n,k) +. sumprod (f,n-1,k)